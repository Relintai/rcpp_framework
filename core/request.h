#ifndef REQUEST_H
#define REQUEST_H

#include <mutex>
#include <vector>

#include <brynet/net/http/HttpFormat.hpp>
#include <brynet/net/http/HttpService.hpp>

using namespace brynet;
using namespace brynet::net;
using namespace brynet::net::http;

class Request {
public:
	const HTTPParser *http_parser;
	const HttpSession::Ptr *session;
	HttpResponse *response;

    uint32_t current_middleware_index;
    std::function<void(Request *)> handler_func;
    std::vector<std::function<void(Request *)> > *middleware_stack;

    void next_stage();
	void send();
	void reset();

	Request();
	~Request();
};

class RequestPool {
public:
	static Request *get_request();
	static void return_request(Request *request);

	RequestPool();
	~RequestPool();

protected:
	static std::mutex _mutex;
	static std::vector<Request *> _requests;
};

#endif