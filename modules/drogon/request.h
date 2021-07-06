#ifndef DREQUEST_H
#define DREQUEST_H

#include "core/http/request.h"

#include <mutex>
#include <vector>

#include "http/HttpRequestImpl.h"
#include "http/HttpResponse.h"

#include "core/http/handler_instance.h"

using namespace drogon;

class DWebApplication;

class DRequest : public Request {
public:
	HttpResponsePtr response;
	HttpRequestImplPtr request;
	std::function<void(const HttpResponsePtr &)> callback;

	void send();
	void send_file(const std::string &p_file_path);
	void reset();

	void update();

	static DRequest *get();
	static void pool(DRequest *request);
	void pool();

	DRequest();
	~DRequest();

protected:
	void _progress_send_file();
	void _file_chunk_sent();

	std::vector<std::string> _path_stack;
	uint32_t _path_stack_pointer;

private:
	static RequestPool<DRequest> _request_pool;
};

#endif