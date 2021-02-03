/**
 *
 *  @file HttpRequest.h
 *  An Tao
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  https://github.com/an-tao/drogon
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Drogon
 *
 */

#pragma once

#include <core/attribute.h>
#include <core/http_types.h>
#include <core/session.h>
#include <core/string_view.h>
#include <core/upload_file.h>
#include <json/json.h>
#include <trantor/net/InetAddress.h>
#include <trantor/utils/Date.h>
#include <memory>
#include <string>
#include <unordered_map>

#include "cache_file.h"
#include "core/http_utils.h"
#include <core/http_request.h>
#include <core/utilities.h>

#include <assert.h>
#include <stdio.h>
#include <trantor/net/EventLoop.h>
#include <trantor/net/InetAddress.h>
#include <trantor/utils/Logger.h>
#include <trantor/utils/MsgBuffer.h>
#include <trantor/utils/NonCopyable.h>
#include <algorithm>
#include <string>
#include <thread>
#include <unordered_map>

namespace drogon {

class HttpRequest;
using HttpRequestPtr = std::shared_ptr<HttpRequest>;

/**
 * @brief This template is used to convert a request object to a custom
 * type object. Users must specialize the template for a particular type.
 */
template <typename T>
T fromRequest(const HttpRequest &req) {
	//LOG_ERROR << "You must specialize the fromRequest template for the type of "
	//		  << DrClassMap::demangle(typeid(T).name());
	exit(1);
}

/**
 * @brief This template is used to create a request object from a custom
 * type object by calling the newCustomHttpRequest(). Users must specialize
 * the template for a particular type.
 */
template <typename T>
HttpRequestPtr toRequest(T &&) {
	//LOG_ERROR << "You must specialize the toRequest template for the type of "
	//		  << DrClassMap::demangle(typeid(T).name());
	exit(1);
}

template <>
HttpRequestPtr toRequest<const Json::Value &>(const Json::Value &pJson);
template <>
HttpRequestPtr toRequest(Json::Value &&pJson);
template <>
inline HttpRequestPtr toRequest<Json::Value &>(Json::Value &pJson) {
	return toRequest((const Json::Value &)pJson);
}

template <>
std::shared_ptr<Json::Value> fromRequest(const HttpRequest &req);

/// Abstract class for webapp developer to get or set the Http request;
class HttpRequest {
public:
	friend class HttpRequestParser;

	explicit HttpRequest(trantor::EventLoop *loop) :
			creationDate_(trantor::Date::now()), loop_(loop) {
	}

	void reset() {
		method_ = Invalid;
		version_ = Version::kUnknown;
		flagForParsingJson_ = false;
		headers_.clear();
		cookies_.clear();
		flagForParsingParameters_ = false;
		path_.clear();
		matchedPathPattern_ = "";
		query_.clear();
		parameters_.clear();
		jsonPtr_.reset();
		sessionPtr_.reset();
		attributesPtr_.reset();
		cacheFilePtr_.reset();
		expectPtr_.reset();
		content_.clear();
		contentType_ = CT_TEXT_PLAIN;
		flagForParsingContentType_ = false;
		contentTypeString_.clear();
		keepAlive_ = true;
		jsonParsingErrorPtr_.reset();
	}

	trantor::EventLoop *getLoop() {
		return loop_;
	}

	void setVersion(Version v) {
		version_ = v;
		if (version_ == Version::kHttp10) {
			keepAlive_ = false;
		}
	}

	void setPath(const char *start, const char *end) {
		if (utils::needUrlDecoding(start, end)) {
			path_ = utils::urlDecode(start, end);
		} else {
			path_.append(start, end);
		}
	}

	virtual void setPath(const std::string &path) {
		path_ = path;
	}

	/**
     * @brief This template enables implicit type conversion. For using this
     * template, user must specialize the fromRequest template. For example a
     * shared_ptr<Json::Value> specialization version is available above, so
     * we can use the following code to get a json object:
     * @code
       std::shared_ptr<Json::Value> jsonPtr = *requestPtr;
       @endcode
     * With this template, user can use their favorite JSON library instead of
     * the default jsoncpp library or convert the request to an object of any
     * custom type.
     */
	template <typename T>
	operator T() const {
		return fromRequest<T>(*this);
	}

	/**
     * @brief This template enables explicit type conversion, see the above
     * template.
     */
	template <typename T>
	T as() const {
		return fromRequest<T>(*this);
	}

	bool setMethod(const char *start, const char *end);

	/// Return the method string of the request, such as GET, POST, etc.
	virtual const char *methodString() const;
	const char *getMethodString() const {
		return methodString();
	}

	/// Return the enum type method of the request.
	virtual HttpMethod method() const {
		return method_;
	}

	HttpMethod getMethod() const {
		return method();
	}

	/// Get the header string identified by the field parameter
	virtual const std::string &getHeader(const std::string &field) const {
		auto lowField = field;
		std::transform(lowField.begin(),
				lowField.end(),
				lowField.begin(),
				tolower);
		return getHeaderBy(lowField);
	}
	/// Get the header string identified by the field parameter
	virtual const std::string &getHeader(std::string &&field) const {
		std::transform(field.begin(), field.end(), field.begin(), tolower);
		return getHeaderBy(field);
	}

	/// Set the header string identified by the field parameter
	virtual void addHeader(const std::string &key,
			const std::string &value) {
		headers_[key] = value;
	}

	/// Get the cookie string identified by the field parameter
	virtual const std::string &getCookie(const std::string &field) const {
		const static std::string defaultVal;
		auto it = cookies_.find(field);
		if (it != cookies_.end()) {
			return it->second;
		}
		return defaultVal;
	}

	/// Get all headers of the request
	virtual const std::unordered_map<std::string, std::string> &headers() const {
		return headers_;
	}

	/// Get all headers of the request
	const std::unordered_map<std::string, std::string> &getHeaders() const {
		return headers();
	}

	/// Get all cookies of the request
	virtual const std::unordered_map<std::string, std::string> &cookies() const {
		return cookies_;
	}

	/// Get all cookies of the request
	const std::unordered_map<std::string, std::string> &getCookies() const {
		return cookies();
	}

	/// Get the query string of the request.
	/**
     * The query string is the substring after the '?' in the URL string.
     */
	virtual const std::string &query() const {
		return query_;
	}

	/// Get the query string of the request.
	const std::string &getQuery() const {
		return query();
	}

	/// Get the content string of the request, which is the body part of the
	/// request.
	string_view body() const {
		return string_view(bodyData(), bodyLength());
	}

	/// Get the content string of the request, which is the body part of the
	/// request.
	string_view getBody() const {
		return body();
	}

	void setQuery(const char *start, const char *end) {
		query_.assign(start, end);
	}

	void setQuery(const std::string &query) {
		query_ = query;
	}

	string_view bodyView() const {
		if (cacheFilePtr_) {
			return cacheFilePtr_->getStringView();
		}
		return content_;
	}

	virtual const char *bodyData() const {
		if (cacheFilePtr_) {
			return cacheFilePtr_->getStringView().data();
		}
		return content_.data();
	}
	virtual size_t bodyLength() const {
		if (cacheFilePtr_) {
			return cacheFilePtr_->getStringView().length();
		}
		return content_.length();
	}

	void appendToBody(const char *data, size_t length);

	void reserveBodySize(size_t length);

	string_view queryView() const {
		return query_;
	}

	string_view contentView() const {
		if (cacheFilePtr_)
			return cacheFilePtr_->getStringView();
		return content_;
	}

	/// Set the content string of the request.
	virtual void setBody(const std::string &body) {
		content_ = body;
	}

	/// Set the content string of the request.
	virtual void setBody(std::string &&body) {
		content_ = std::move(body);
	}

	/// Get the path of the request.
	virtual const std::string &path() const {
		return path_;
	}

	/// Get the path of the request.
	const std::string &getPath() const {
		return path();
	}

	/// Get the matched path pattern after routing
	string_view getMatchedPathPattern() const {
		return matchedPathPattern();
	}

	/// Get the matched path pattern after routing
	string_view matchedPathPattern() const {
		return string_view(matchedPathPatternData(),
				matchedPathPatternLength());
	}
	virtual const char *matchedPathPatternData() const {
		return matchedPathPattern_.data();
	}
	virtual size_t matchedPathPatternLength() const {
		return matchedPathPattern_.length();
	}

	void setMatchedPathPattern(const std::string &pathPattern) {
		matchedPathPattern_ = pathPattern;
	}
	const std::string &expect() const {
		const static std::string none{ "" };
		if (expectPtr_)
			return *expectPtr_;
		return none;
	}
	bool keepAlive() const {
		return keepAlive_;
	}

	/// Return the enum type version of the request.
	/**
     * kHttp10 means Http version is 1.0
     * kHttp11 means Http verison is 1.1
     */
	virtual Version version() const {
		return version_;
	}
	/// Return the enum type version of the request.
	Version getVersion() const {
		return version();
	}

	/// Get the session to which the request belongs.

	virtual SessionPtr session() const {
		return sessionPtr_;
	}

	void setSession(const SessionPtr &session) {
		sessionPtr_ = session;
	}

	/// Get the session to which the request belongs.
	SessionPtr getSession() const {
		return session();
	}

	/// Get the attributes store, users can add/get any type of data to/from
	/// this store
	virtual const AttributesPtr &attributes() const {
		if (!attributesPtr_) {
			attributesPtr_ = std::make_shared<Attributes>();
		}
		return attributesPtr_;
	}

	/// Get the attributes store, users can add/get any type of data to/from
	/// this store
	const AttributesPtr &getAttributes() const {
		return attributes();
	}

	/// Get parameters of the request.
	virtual const std::unordered_map<std::string, std::string> &parameters()
			const {
		parseParametersOnce();
		return parameters_;
	}

	/// Get parameters of the request.
	const std::unordered_map<std::string, std::string> &getParameters() const {
		return parameters();
	}

	/// Get a parameter identified by the @param key
	virtual const std::string &getParameter(
			const std::string &key) const {
		const static std::string defaultVal;
		parseParametersOnce();
		auto iter = parameters_.find(key);
		if (iter != parameters_.end())
			return iter->second;
		return defaultVal;
	}

	/// Return the remote IP address and port
	virtual const trantor::InetAddress &peerAddr() const {
		return peer_;
	}

	const trantor::InetAddress &getPeerAddr() const {
		return peerAddr();
	}

	/// Return the local IP address and port
	virtual const trantor::InetAddress &localAddr() const {
		return local_;
	}

	const trantor::InetAddress &getLocalAddr() const {
		return localAddr();
	}

	/// Return the creation timestamp set by the framework.
	virtual const trantor::Date &creationDate() const {
		return creationDate_;
	}

	const trantor::Date &getCreationDate() const {
		return creationDate();
	}

	void setCreationDate(const trantor::Date &date) {
		creationDate_ = date;
	}

	void setPeerAddr(const trantor::InetAddress &peer) {
		peer_ = peer;
	}

	void setLocalAddr(const trantor::InetAddress &local) {
		local_ = local;
	}

	void addHeader(const char *start, const char *colon, const char *end);

	const std::string &getHeaderBy(const std::string &lowerField) const {
		const static std::string defaultVal;
		auto it = headers_.find(lowerField);
		if (it != headers_.end()) {
			return it->second;
		}
		return defaultVal;
	}

	/// Get the Json object of the request
	/**
     * The content type of the request must be 'application/json', and the query
     * string (the part after the question mark in the URI) must be empty,
     * otherwise the method returns an empty shared_ptr object.
     */
	virtual const std::shared_ptr<Json::Value> &jsonObject() const {
		// Not multi-thread safe but good, because we basically call this
		// function in a single thread
		if (!flagForParsingJson_) {
			flagForParsingJson_ = true;
			parseJson();
		}
		return jsonPtr_;
	}

	/// Get the Json object of the request
	const std::shared_ptr<Json::Value> &getJsonObject() const {
		return jsonObject();
	}

	/**
     * @brief Get the error message of parsing the JSON body received from peer.
     * This method usually is called after getting a empty shared_ptr object
     * by the getJsonObject() method.
     *
     * @return const std::string& The error message. An empty string is returned
     * when no error occurs.
     */
	virtual const std::string &getJsonError() const {
		const static std::string none{ "" };
		if (jsonParsingErrorPtr_)
			return *jsonParsingErrorPtr_;
		return none;
	}

	/// Get the content type
	virtual ContentType contentType() const {
		if (!flagForParsingContentType_) {
			flagForParsingContentType_ = true;
			auto &contentTypeString = getHeaderBy("content-type");
			if (contentTypeString == "") {
				contentType_ = CT_NONE;
			} else {
				auto pos = contentTypeString.find(';');
				if (pos != std::string::npos) {
					contentType_ = parseContentType(
							string_view(contentTypeString.data(), pos));
				} else {
					contentType_ =
							parseContentType(string_view(contentTypeString));
				}
			}
		}
		return contentType_;
	}
	ContentType getContentType() const {
		return contentType();
	}

	/// Set the Http method
	virtual void setMethod(const HttpMethod method) {
		method_ = method;
		return;
	}

	/// Set the path of the request

	void setSecure(bool secure) {
		isOnSecureConnection_ = secure;
	}

	/// Set the parameter of the request

	virtual void setParameter(const std::string &key,
			const std::string &value) {
		flagForParsingParameters_ = true;
		parameters_[key] = value;
	}

	const std::string &getContent() const {
		return content_;
	}

	void swap(HttpRequest &that) noexcept;

	void setContent(const std::string &content) {
		content_ = content;
	}

	/// Set or get the content type

	virtual void setContentTypeCode(const ContentType type) {
		contentType_ = type;
		flagForParsingContentType_ = true;
		auto &typeStr = webContentTypeToString(type);
		setContentType(std::string(typeStr.data(), typeStr.length()));
	}

	/// Set the request content-type string, The string
	/// must contain the header name and CRLF.
	/// For example, "content-type: text/plain\r\n"

	virtual void setCustomContentTypeString(const std::string &type) {
		contentType_ = CT_NONE;
		flagForParsingContentType_ = true;
		contentTypeString_ = type;
	}

	// virtual void setContentTypeCodeAndCharacterSet(ContentType type, const
	// std::string &charSet = "utf-8")
	// {
	//     contentType_ = type;
	//     setContentType(webContentTypeAndCharsetToString(type, charSet));
	// }

	/// Add a cookie

	virtual void addCookie(const std::string &key,
			const std::string &value) {
		cookies_[key] = value;
	}

	/**
     * @brief Set the request object to the pass-through mode or not. It's not
     * by default when a new request object is created.
     * In pass-through mode, no addtional headers (including server, date,
     * content-type and content-length, etc.) are added to the request. This
     * mode is useful for some applications such as a proxy.
     *
     * @param flag
     */
	virtual void setPassThrough(bool flag) {
		passThrough_ = flag;
	}

	bool passThrough() const {
		return passThrough_;
	}

	void appendToBuffer(trantor::MsgBuffer *output) const;

	virtual ~HttpRequest();

	/// The following methods are a series of factory methods that help users
	/// create request objects.

	/// Create a normal request with http method Get and version Http1.1.
	static HttpRequestPtr newHttpRequest();

	/// Create a http request with:
	/// Method: Get
	/// Version: Http1.1
	/// Content type: application/json, the @param data is serialized into the
	/// content of the request.
	static HttpRequestPtr newHttpJsonRequest(const Json::Value &data);

	/// Create a http request with:
	/// Method: Post
	/// Version: Http1.1
	/// Content type: application/x-www-form-urlencoded
	static HttpRequestPtr newHttpFormPostRequest();

	/// Create a http file upload request with:
	/// Method: Post
	/// Version: Http1.1
	/// Content type: multipart/form-data
	/// The @param files represents pload files which are transferred to the
	/// server via the multipart/form-data format
	static HttpRequestPtr newFileUploadRequest(
			const std::vector<UploadFile> &files);

	/**
     * @brief Create a custom HTTP request object. For using this template,
     * users must specialize the toRequest template.
     */
	template <typename T>
	static HttpRequestPtr newCustomHttpRequest(T &&obj) {
		return toRequest(std::forward<T>(obj));
	}

	virtual bool isOnSecureConnection() const noexcept {
		return isOnSecureConnection_;
	}

protected:
	void setContentType(const std::string &contentType) {
		contentTypeString_ = contentType;
	}
	void setContentType(std::string &&contentType) {
		contentTypeString_ = std::move(contentType);
	}

private:
	void parseParameters() const;
	void parseParametersOnce() const {
		// Not multi-thread safe but good, because we basically call this
		// function in a single thread
		if (!flagForParsingParameters_) {
			flagForParsingParameters_ = true;
			parseParameters();
		}
	}
	void createTmpFile();
	void parseJson() const;
	mutable bool flagForParsingParameters_{ false };
	mutable bool flagForParsingJson_{ false };
	HttpMethod method_{ Invalid };
	Version version_{ Version::kUnknown };
	std::string path_;
	string_view matchedPathPattern_{ "" };
	std::string query_;
	std::unordered_map<std::string, std::string> headers_;
	std::unordered_map<std::string, std::string> cookies_;
	mutable std::unordered_map<std::string, std::string> parameters_;
	mutable std::shared_ptr<Json::Value> jsonPtr_;
	SessionPtr sessionPtr_;
	mutable AttributesPtr attributesPtr_;
	trantor::InetAddress peer_;
	trantor::InetAddress local_;
	trantor::Date creationDate_;
	std::unique_ptr<CacheFile> cacheFilePtr_;
	mutable std::unique_ptr<std::string> jsonParsingErrorPtr_;
	std::unique_ptr<std::string> expectPtr_;
	bool keepAlive_{ true };
	bool isOnSecureConnection_{ false };
	bool passThrough_{ false };

protected:
	std::string content_;
	trantor::EventLoop *loop_;
	mutable ContentType contentType_{ CT_TEXT_PLAIN };
	mutable bool flagForParsingContentType_{ false };
	std::string contentTypeString_;
};

template <>
inline HttpRequestPtr toRequest<const Json::Value &>(const Json::Value &pJson) {
	return HttpRequest::newHttpJsonRequest(pJson);
}

template <>
inline HttpRequestPtr toRequest(Json::Value &&pJson) {
	return HttpRequest::newHttpJsonRequest(std::move(pJson));
}

template <>
inline std::shared_ptr<Json::Value> fromRequest(const HttpRequest &req) {
	return req.getJsonObject();
}

using HttpRequestPtr = std::shared_ptr<HttpRequest>;

inline void swap(HttpRequest &one, HttpRequest &two) noexcept {
	one.swap(two);
}

} // namespace drogon
