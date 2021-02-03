/**
 *  @file HttpResponse.h
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

#include <core/cookie.h>
#include <core/http_types.h>
#include <core/string_view.h>

#include <core/http_view_data.h>
#include <json/json.h>
#include <memory>
#include <string>

#include "core/http_utils.h"

#include "http_message_body.h"

#include <core/http_response.h>
#include <core/utilities.h>
#include <trantor/net/InetAddress.h>
#include <trantor/utils/Date.h>
#include <trantor/utils/MsgBuffer.h>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

namespace drogon {
/// Abstract class for webapp developer to get or set the Http response;
class HttpResponse;
using HttpResponsePtr = std::shared_ptr<HttpResponse>;


/**
 * @brief This template is used to convert a response object to a custom
 * type object. Users must specialize the template for a particular type.
 */
template <typename T>
T fromResponse(const HttpResponse &resp) {
	// LOG_ERROR
	//     << "You must specialize the fromResponse template for the type of "
	//    << DrClassMap::demangle(typeid(T).name());
	exit(1);
}

/**
 * @brief This template is used to create a response object from a custom
 * type object by calling the newCustomHttpResponse(). Users must specialize
 * the template for a particular type.
 */
template <typename T>
HttpResponsePtr toResponse(T &&) {
	// LOG_ERROR << "You must specialize the toResponse template for the type of "
	//          << DrClassMap::demangle(typeid(T).name());
	exit(1);
}
template <>
HttpResponsePtr toResponse<const Json::Value &>(const Json::Value &pJson);
template <>
HttpResponsePtr toResponse(Json::Value &&pJson);
template <>
inline HttpResponsePtr toResponse<Json::Value &>(Json::Value &pJson) {
	return toResponse((const Json::Value &)pJson);
}

class HttpResponse {
	friend class HttpResponseParser;

public:
	/**
     * @brief This template enables automatic type conversion. For using this
     * template, user must specialize the fromResponse template. For example a
     * shared_ptr<Json::Value> specialization version is available above, so
     * we can use the following code to get a json object:
     * @code
     *  std::shared_ptr<Json::Value> jsonPtr = *responsePtr;
     *  @endcode
     * With this template, user can use their favorite JSON library instead of
     * the default jsoncpp library or convert the response to an object of any
     * custom type.
     */
	template <typename T>
	operator T() const {
		return fromResponse<T>(*this);
	}

	/**
     * @brief This template enables explicit type conversion, see the above
     * template.
     */
	template <typename T>
	T as() const {
	}

	/// Get the status code such as 200, 404
	virtual HttpStatusCode statusCode() const {
		return statusCode_;
	}

	HttpStatusCode getStatusCode() const {
		return statusCode();
	}

	/// Set the status code of the response.
	virtual void setStatusCode(HttpStatusCode code) {
		statusCode_ = code;
		setStatusMessage(statusCodeToString(code));
	}

	/// Get the creation timestamp of the response.
	virtual const trantor::Date &creationDate() const {
		return creationDate_;
	}

	const trantor::Date &getCreationDate() const {
		return creationDate();
	}

	/// Set the http version, http1.0 or http1.1
	virtual void setVersion(const Version v) {
		version_ = v;
		if (version_ == Version::kHttp10) {
			closeConnection_ = true;
		}
	}

	/// Set if close the connection after the request is sent.
	/**
     * @param on if the parameter is false, the connection keeps alive on the
     * condition that the client request has a 'keep-alive' head, otherwise it
     * is closed immediately after sending the last byte of the response. It's
     * false by default when the response is created.
     */
	virtual void setCloseConnection(bool on) {
		closeConnection_ = on;
	}

	/// Get the status set by the setCloseConnetion() method.

	virtual bool ifCloseConnection() const {
		return closeConnection_;
	}

	/// Set the response content type, such as text/html, text/plaint, image/png
	/// and so on. If the content type
	/// is a text type, the character set is utf8.
	virtual void setContentTypeCode(ContentType type) {
		contentType_ = type;
		setContentType(webContentTypeToString(type));
		flagForParsingContentType_ = true;
	}

	/// Set the response content type and the content-type string, The string
	/// must contain the header name and CRLF.
	/// For example, "content-type: text/plain\r\n"
	void setContentTypeCodeAndCustomString(ContentType type,
			const string_view &typeString) {
		setContentTypeCodeAndCustomString(type,
				typeString.data(),
				typeString.length());
	}
	template <int N>
	void setContentTypeCodeAndCustomString(ContentType type,
			const char (&typeString)[N]) {
		assert(N > 0);
		setContentTypeCodeAndCustomString(type, typeString, N - 1);
	}

	/// Set the reponse content type and the character set.
	/// virtual void setContentTypeCodeAndCharacterSet(ContentType type, const
	/// std::string &charSet = "utf-8") = 0;

	const std::string &getHeaderBy(const std::string &lowerKey) const {
		const static std::string defaultVal;
		auto iter = headers_.find(lowerKey);
		if (iter == headers_.end()) {
			return defaultVal;
		}
		return iter->second;
	}

	/// Get the response content type.
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

	/// Get the header string identified by the key parameter.
	/**
     * @note
     * If there is no the header, a empty string is retured.
     * The key is case insensitive
     */
	virtual const std::string &getHeader(const std::string &key) const {
		auto field = key;
		transform(field.begin(), field.end(), field.begin(), ::tolower);
		return getHeaderBy(field);
	}

	virtual const std::string &getHeader(std::string &&key) const {
		transform(key.begin(), key.end(), key.begin(), ::tolower);
		return getHeaderBy(key);
	}

	/// Remove the header identified by the key parameter.
	virtual void removeHeader(const std::string &key) {
		auto field = key;
		transform(field.begin(), field.end(), field.begin(), ::tolower);
		removeHeaderBy(field);
	}

	/// Remove the header identified by the key parameter.
	virtual void removeHeader(std::string &&key) {
		transform(key.begin(), key.end(), key.begin(), ::tolower);
		removeHeaderBy(key);
	}

	void removeHeaderBy(const std::string &lowerKey) {
		headers_.erase(lowerKey);
	}

	/// Get all headers of the response
	virtual const std::unordered_map<std::string, std::string> &headers()
			const {
		return headers_;
	}

	/// Get all headers of the response
	const std::unordered_map<std::string, std::string> &getHeaders() const {
		return headers();
	}

	/// Add a header.
	virtual void addHeader(const std::string &key,
			const std::string &value) {
		fullHeaderString_.reset();
		auto field = key;
		transform(field.begin(), field.end(), field.begin(), ::tolower);
		headers_[std::move(field)] = value;
	}

	/// Add a header.
	virtual void addHeader(const std::string &key, std::string &&value) {
		fullHeaderString_.reset();
		auto field = key;
		transform(field.begin(), field.end(), field.begin(), ::tolower);
		headers_[std::move(field)] = std::move(value);
	}

	void addHeader(const char *start, const char *colon, const char *end);

	/// Add a cookie
	virtual void addCookie(const std::string &key,
			const std::string &value) {
		cookies_[key] = Cookie(key, value);
	}

	/// Add a cookie
	virtual void addCookie(const Cookie &cookie) {
		cookies_[cookie.key()] = cookie;
	}

	virtual void addCookie(Cookie &&cookie) {
		cookies_[cookie.key()] = std::move(cookie);
	}

	/// Get the cookie identified by the key parameter.
	/// If there is no the cookie, the empty cookie is retured.
	virtual const Cookie &getCookie(const std::string &key) const {
		static const Cookie defaultCookie;
		auto it = cookies_.find(key);
		if (it != cookies_.end()) {
			return it->second;
		}
		return defaultCookie;
	}

	/// Get all cookies.
	virtual const std::unordered_map<std::string, Cookie> &cookies()
			const {
		return cookies_;
	}

	/// Get all cookies.
	const std::unordered_map<std::string, Cookie> &getCookies() const {
		return cookies();
	}

	/// Remove the cookie identified by the key parameter.
	virtual void removeCookie(const std::string &key) {
		cookies_.erase(key);
	}

	/// Set the response body(content).
	/**
     * @note The body must match the content type
     */
	virtual void setBody(const std::string &body) {
		bodyPtr_ = std::make_shared<HttpMessageStringBody>(body);
	}
	/// Set the response body(content).
	virtual void setBody(std::string &&body) {
		bodyPtr_ = std::make_shared<HttpMessageStringBody>(std::move(body));
	}

	/// Set the response body(content).
	template <int N>
	void setBody(const char (&body)[N]) {
		assert(strnlen(body, N) == N - 1);
		setBody(body, N - 1);
	}

	/// Get the response body.
	string_view body() const {
		return string_view{ getBodyData(), getBodyLength() };
	}

	/// Get the response body.
	string_view getBody() const {
		return body();
	}

	/// Return the enum type version of the response.
	/**
     * kHttp10 means Http version is 1.0
     * kHttp11 means Http verison is 1.1
     */
	virtual Version version() const {
		return version_;
	}

	/// Return the enum type version of the response.
	Version getVersion() const {
		return version();
	}

	/// Reset the reponse object to its initial state
	virtual void clear();

	/// Set the expiration time of the response cache in memory.
	/// in seconds, 0 means always cache, negative means not cache, default is
	/// -1.
	virtual void setExpiredTime(ssize_t expiredTime) {
		expriedTime_ = expiredTime;
		datePos_ = std::string::npos;
		if (expriedTime_ < 0 && version_ == Version::kHttp10) {
			fullHeaderString_.reset();
		}
	}

	/// Get the expiration time of the response.
	virtual ssize_t expiredTime() const {
		return expriedTime_;
	}
	ssize_t getExpiredTime() const {
		return expiredTime();
	}

	/// Get the json object from the server response.
	/// If the response is not in json format, then a empty shared_ptr is
	/// retured.
	virtual const std::shared_ptr<Json::Value> &jsonObject() const {
		// Not multi-thread safe but good, because we basically call this
		// function in a single thread
		if (!flagForParsingJson_) {
			flagForParsingJson_ = true;
			parseJson();
		}
		return jsonPtr_;
	}
	const std::shared_ptr<Json::Value> &getJsonObject() const {
		return jsonObject();
	}

	void setJsonObject(const Json::Value &pJson) {
		flagForParsingJson_ = true;
		flagForSerializingJson_ = false;
		jsonPtr_ = std::make_shared<Json::Value>(pJson);
	}
	void setJsonObject(Json::Value &&pJson) {
		flagForParsingJson_ = true;
		flagForSerializingJson_ = false;
		jsonPtr_ = std::make_shared<Json::Value>(std::move(pJson));
	}
	bool shouldBeCompressed() const;
	void generateBodyFromJson() const;
	const std::string &sendfileName() const {
		return sendfileName_;
	}
	void setSendfile(const std::string &filename) {
		sendfileName_ = filename;
	}
	void makeHeaderString() {
		fullHeaderString_ = std::make_shared<trantor::MsgBuffer>(128);
		makeHeaderString(*fullHeaderString_);
	}

	void gunzip() {
		if (bodyPtr_) {
			auto gunzipBody =
					utils::gzipDecompress(bodyPtr_->data(), bodyPtr_->length());
			removeHeader("content-encoding");
			bodyPtr_ =
					std::make_shared<HttpMessageStringBody>(move(gunzipBody));
		}
	}
#ifdef USE_BROTLI
	void brDecompress() {
		if (bodyPtr_) {
			auto gunzipBody =
					utils::brotliDecompress(bodyPtr_->data(), bodyPtr_->length());
			removeHeader("content-encoding");
			bodyPtr_ =
					std::make_shared<HttpMessageStringBody>(move(gunzipBody));
		}
	}
#endif

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

	/**
     * @brief Set the reponse object to the pass-through mode or not. It's not
     * by default when a new response object is created.
     * In pass-through mode, no addtional headers (including server, date,
     * content-type and content-length, etc.) are added to the response. This
     * mode is useful for some applications such as a proxy.
     *
     * @param flag
     */
	virtual void setPassThrough(bool flag) {
		passThrough_ = flag;
	}

	void redirect(const std::string &url) {
		headers_["location"] = url;
	}

	std::shared_ptr<trantor::MsgBuffer> renderToBuffer();
	void renderToBuffer(trantor::MsgBuffer &buffer);
	std::shared_ptr<trantor::MsgBuffer> renderHeaderForHeadMethod();

	/* The following methods are a series of factory methods that help users
     * create response objects. */

	/// Create a normal response with a status code of 200ok and a content type
	/// of text/html.
	static HttpResponsePtr newHttpResponse();
	/// Create a response which returns a 404 page.
	static HttpResponsePtr newNotFoundResponse();
	/// Create a response which returns a json object. Its content type is set
	/// to set/json.
	static HttpResponsePtr newHttpJsonResponse(const Json::Value &data);
	static HttpResponsePtr newHttpJsonResponse(Json::Value &&data);
	/// Create a response that returns a page rendered by a view named
	/// viewName.
	/**
     * @param viewName The name of the view
     * @param data is the data displayed on the page.
     * @note For more details, see the wiki pages, the "View" section.
     */
	static HttpResponsePtr newHttpViewResponse(
			const std::string &viewName,
			const HttpViewData &data = HttpViewData());

	/// Create a response that returns a redirection page, redirecting to
	/// another page located in the location parameter.
	/**
     * @param location The location to redirect
     * @param status The HTTP status code, k302Found by default. Users could set
     * it to one of the 301, 302, 303, 307, ...
     */
	static HttpResponsePtr newRedirectionResponse(
			const std::string &location,
			HttpStatusCode status = k302Found);

	/// Create a response that returns a file to the client.
	/**
     * @param fullPath is the full path to the file.
     * @param attachmentFileName if the parameter is not empty, the browser
     * does not open the file, but saves it as an attachment.
     * @param type if the parameter is CT_NONE, the content type is set by
     * drogon based on the file extension.
     */
	static HttpResponsePtr newFileResponse(
			const std::string &fullPath,
			const std::string &attachmentFileName = "",
			ContentType type = CT_NONE);

	/**
     * @brief Create a custom HTTP response object. For using this template,
     * users must specialize the toResponse template.
     */
	template <typename T>
	static HttpResponsePtr newCustomHttpResponse(T &&obj) {
		return toResponse(std::forward<T>(obj));
	}

	HttpResponse() :
			creationDate_(trantor::Date::now()) {
	}
	HttpResponse(HttpStatusCode code, ContentType type) :
			statusCode_(code),
			statusMessage_(statusCodeToString(code)),
			creationDate_(trantor::Date::now()),
			contentType_(type),
			flagForParsingContentType_(true),
			contentTypeString_(webContentTypeToString(type)) {
	}

	virtual ~HttpResponse();

	// virtual void setContentTypeCodeAndCharacterSet(ContentType type, const
	// std::string &charSet = "utf-8")
	// {
	//     contentType_ = type;
	//     setContentType(webContentTypeAndCharsetToString(type, charSet));
	// }

	void swap(HttpResponse &that) noexcept;

protected:
	void makeHeaderString(trantor::MsgBuffer &headerString);

private:
	virtual void setBody(const char *body, size_t len) {
		bodyPtr_ = std::make_shared<HttpMessageStringViewBody>(body, len);
	}
	virtual const char *getBodyData() const {
		if (!flagForSerializingJson_ && jsonPtr_) {
			generateBodyFromJson();
		} else if (!bodyPtr_) {
			return nullptr;
		}
		return bodyPtr_->data();
	}

	virtual size_t getBodyLength() const {
		if (bodyPtr_)
			return bodyPtr_->length();
		return 0;
	}

	void parseJson() const;

	virtual void setContentTypeCodeAndCustomString(
			ContentType type,
			const char *typeString,
			size_t typeStringLength) {
		contentType_ = type;
		flagForParsingContentType_ = true;
		setContentType(string_view{ typeString, typeStringLength });
	}

	std::unordered_map<std::string, std::string> headers_;
	std::unordered_map<std::string, Cookie> cookies_;

	HttpStatusCode statusCode_{ kUnknown };
	string_view statusMessage_;

	trantor::Date creationDate_;
	Version version_{ Version::kHttp11 };
	bool closeConnection_{ false };
	mutable std::shared_ptr<HttpMessageBody> bodyPtr_;
	ssize_t expriedTime_{ -1 };
	std::string sendfileName_;
	mutable std::shared_ptr<Json::Value> jsonPtr_;

	std::shared_ptr<trantor::MsgBuffer> fullHeaderString_;
	mutable std::shared_ptr<trantor::MsgBuffer> httpString_;
	mutable size_t datePos_{ static_cast<size_t>(-1) };
	mutable int64_t httpStringDate_{ -1 };
	mutable bool flagForParsingJson_{ false };
	mutable bool flagForSerializingJson_{ true };
	mutable ContentType contentType_{ CT_TEXT_PLAIN };
	mutable bool flagForParsingContentType_{ false };
	mutable std::shared_ptr<std::string> jsonParsingErrorPtr_;
	string_view contentTypeString_{
		"Content-Type: text/html; charset=utf-8\r\n"
	};
	bool passThrough_{ false };
	void setContentType(const string_view &contentType) {
		contentTypeString_ = contentType;
	}
	void setStatusMessage(const string_view &message) {
		statusMessage_ = message;
	}
};

template <>
inline HttpResponsePtr toResponse<const Json::Value &>(const Json::Value &pJson) {
	return HttpResponse::newHttpJsonResponse(pJson);
}

template <>
inline HttpResponsePtr toResponse(Json::Value &&pJson) {
	return HttpResponse::newHttpJsonResponse(std::move(pJson));
}

template <>
inline std::shared_ptr<Json::Value> fromResponse(const HttpResponse &resp) {
	return resp.getJsonObject();
}

using HttpResponsePtr = std::shared_ptr<HttpResponse>;

inline void swap(HttpResponse &one, HttpResponse &two) noexcept {
	one.swap(two);
}

} // namespace drogon
