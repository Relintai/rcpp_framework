#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <condition_variable>
#include <iostream>
#include <string>

#include <brynet/base/AppStatus.hpp>
#include <brynet/net/http/HttpFormat.hpp>
#include <brynet/net/http/HttpService.hpp>
#include <brynet/net/http/WebSocketFormat.hpp>
#include <brynet/net/wrapper/HttpServiceBuilder.hpp>
#include <brynet/net/wrapper/ServiceBuilder.hpp>

using namespace brynet;
using namespace brynet::net;
using namespace brynet::net::http;

class Request;

class HTTPServer {
public:
	int port;
	int threads;
	std::shared_ptr<TcpService> service;
    wrapper::HttpListenerBuilder *listenBuilder;

    static void http_callback_handler(Request *response);

	static void httpEnterCallbackDefault(const HTTPParser &httpParser, const HttpSession::Ptr &session);
	static void wsEnterCallbackDefault(const HttpSession::Ptr &httpSession, WebSocketFormat::WebSocketFrameType opcode, const std::string &payload);

    virtual void configure();
	virtual void initialize();

	void main_loop();

    HTTPServer();
    virtual ~HTTPServer();
};

#endif