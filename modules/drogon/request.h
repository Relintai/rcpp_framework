#ifndef DREQUEST_H
#define DREQUEST_H

#include "core/http/request.h"

#include <mutex>
#include <vector>

#include "http/HttpRequestImpl.h"
#include "http/HttpResponse.h"

#include "core/http/handler_instance.h"

//using namespace drogon;

class DWebApplication;

class DRequest : public Request {
public:
	drogon::HttpRequestImplPtr request;
	std::function<void(const drogon::HttpResponsePtr &)> callback;

	const std::string &get_cookie(const std::string &key);
	void add_cookie(const ::Cookie &cookie);
	void remove_cookie(const std::string &key);

	HTTPMethod get_method() const;

	const std::string &get_parameter(const std::string &key) const;

	void send();
	void send_file(const std::string &p_file_path);
	void reset();
	std::string parser_get_path();

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

	std::vector<std::string> _path_stack;
	uint32_t _path_stack_pointer;

	std::vector<::Cookie> _added_cookies;
	std::vector<std::string> _removed_cookies;

private:
	static RequestPool<DRequest> _request_pool;
};

#endif