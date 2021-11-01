#ifndef BRY_REQUEST_H
#define BRY_REQUEST_H

#include "core/string.h"

#include "core/http/request.h"

#include <mutex>
#include <vector>

#include <brynet/net/http/HttpFormat.hpp>
#include <brynet/net/http/HttpService.hpp>

#include "core/http/handler_instance.h"

class WebApplication;

class BryRequest : public Request {
public:
	HTTPParser::Ptr http_parser;
	HttpSession::Ptr session;
	HttpResponse *response;

	void send();
	void send_file(const String &p_file_path);
	void reset();
	String parser_get_path();

	void update();

	static BryRequest *get();
	static void pool(BryRequest *request);
	void pool();

	BryRequest();
	~BryRequest();

protected:
	void _progress_send_file();
	void _file_chunk_sent();

	std::vector<String> _path_stack;
	uint32_t _path_stack_pointer;

private:
	static RequestPool<BryRequest> _request_pool;
};


#endif