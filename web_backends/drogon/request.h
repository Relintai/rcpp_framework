#ifndef DREQUEST_H
#define DREQUEST_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "web/http/request.h"

#include <mutex>

#include "http/HttpRequestImpl.h"
#include "http/HttpResponse.h"
#include "http/MultiPart.h"

// using namespace drogon;

class DrogonWebServer;

class DRequest : public Request {
public:
	drogon::HttpRequestImplPtr request;
	std::function<void(const drogon::HttpResponsePtr &)> callback;

	const String get_cookie(const String &key);
	void add_cookie(const ::Cookie &cookie);
	void remove_cookie(const String &key);

	HTTPMethod get_method() const;

	void parse_files();
	int get_file_count() const;
	int get_file_length(const int index) const;
	const uint8_t *get_file_data(const int index) const;

	const String get_parameter(const String &key) const;

	void send_redirect(const String &location, const HTTPStatusCode status_code = HTTP_STATUS_CODE_302_FOUND);
	void send();
	void send_file(const String &p_file_path);
	void reset();
	String parser_get_path();
	String get_host() const;

	void update();

	static DRequest *get();
	static void pool(DRequest *request);
	void pool();

	DRequest();
	~DRequest();

protected:
	void _progress_send_file();
	void _file_chunk_sent();
	void _response_additional_setup(const drogon::HttpResponsePtr &req);

	Vector< ::Cookie> _added_cookies;
	Vector<String> _removed_cookies;
	drogon::MultiPartParser *_multipart_parser;

private:
	static RequestPool<DRequest> _request_pool;
};

#endif