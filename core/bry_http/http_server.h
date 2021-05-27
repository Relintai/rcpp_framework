#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <condition_variable>
#include <iostream>
#include <map>
#include <mutex>
#include <string>

#include <brynet/base/AppStatus.hpp>
#include <brynet/net/http/HttpFormat.hpp>
#include <brynet/net/http/HttpService.hpp>
#include <brynet/net/http/WebSocketFormat.hpp>
#include <brynet/net/wrapper/HttpServiceBuilder.hpp>
#include <brynet/net/wrapper/ServiceBuilder.hpp>

class Request;
class BryWebApplication;

class HTTPServer {
public:
	int port;
	int threads;
	std::shared_ptr<TcpService> service;
	HttpListenerBuilder *listenBuilder;

	void http_callback_handler(Request *response);

	void httpEnterCallbackDefault(const HTTPParser &httpParser, const HttpSession::Ptr &session);
	void wsEnterCallbackDefault(const HttpSession::Ptr &httpSession, WebSocketFormat::WebSocketFrameType opcode, const std::string &payload);
	void closedCallbackDefault(const HttpSession::Ptr &session);

	virtual void configure();
	virtual void initialize();

	void loop_once();
	void main_loop();

	HTTPServer();
	virtual ~HTTPServer();

	//move this to a sublcass
	BryWebApplication *application;

protected:
	std::map<HttpSession *, Request *> _request_map;
	std::mutex _request_map_mutex;
};

#endif