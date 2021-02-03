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

#include <trantor/net/TcpServer.h>
#include <trantor/net/callbacks.h>
#include <trantor/utils/NonCopyable.h>

#include "core/http_request_parser.h"
#include "core/http_server_callbacks.h"

//using namespace brynet;
//using namespace brynet::net;
//using namespace brynet::net::http;

using namespace drogon;

class Request;

class HTTPServer {
public:
	int port;
	int threads;
	std::shared_ptr<brynet::net::TcpService> service;
	brynet::net::wrapper::HttpListenerBuilder *listenBuilder;

	static void http_callback_handler(Request *response);

	static void httpEnterCallbackDefault(const brynet::net::http::HTTPParser &httpParser, const brynet::net::http::HttpSession::Ptr &session);
	static void wsEnterCallbackDefault(const brynet::net::http::HttpSession::Ptr &httpSession, brynet::net::http::WebSocketFormat::WebSocketFrameType opcode, const std::string &payload);

	virtual void configure_old();
	virtual void initialize_old();

	void main_loop_old();

	virtual void configure();
	virtual void initialize();

	void main_loop();

	trantor::EventLoop *get_loop() const {
		return server_.getLoop();
	}

	void setHttpAsyncCallback(const HttpAsyncCallback &cb) {
		httpAsyncCallback_ = cb;
	}
	void setNewWebsocketCallback(const WebSocketNewAsyncCallback &cb) {
		newWebsocketCallback_ = cb;
	}
	void setConnectionCallback(const trantor::ConnectionCallback &cb) {
		connectionCallback_ = cb;
	}
	void setIoLoopThreadPool(
			const std::shared_ptr<trantor::EventLoopThreadPool> &pool) {
		server_.setIoLoopThreadPool(pool);
	}
	void setIoLoopNum(int numThreads) {
		server_.setIoLoopNum(numThreads);
	}
	void kickoffIdleConnections(size_t timeout) {
		server_.kickoffIdleConnections(timeout);
	}
	trantor::EventLoop *getLoop() {
		return server_.getLoop();
	}
	std::vector<trantor::EventLoop *> getIoLoops() {
		return server_.getIoLoops();
	}

	void start();
	void stop();

	void enableSSL(const std::string &certPath, const std::string &keyPath) {
		server_.enableSSL(certPath, keyPath);
	}

	HTTPServer();
	HTTPServer(trantor::EventLoop *loop, const trantor::InetAddress &listenAddr, const std::string &name,
			const std::vector<std::function<HttpResponsePtr(const HttpRequestPtr &)> > &syncAdvices);

	virtual ~HTTPServer();

protected:
	void onConnection(const trantor::TcpConnectionPtr &conn);
	void onMessage(const trantor::TcpConnectionPtr &, trantor::MsgBuffer *);
	void onRequests(const trantor::TcpConnectionPtr &, const std::vector<HttpRequestPtr> &, const std::shared_ptr<drogon::HttpRequestParser> &);
	void sendResponse(const trantor::TcpConnectionPtr &, const HttpResponsePtr &, bool isHeadMethod);
	void sendResponses(const trantor::TcpConnectionPtr &conn, const std::vector<std::pair<HttpResponsePtr, bool> > &responses, trantor::MsgBuffer &buffer);

	trantor::TcpServer server_;
	HttpAsyncCallback httpAsyncCallback_;
	WebSocketNewAsyncCallback newWebsocketCallback_;
	trantor::ConnectionCallback connectionCallback_;
	std::vector<std::function<HttpResponsePtr(const HttpRequestPtr &)> > syncAdvices_;
	bool _running{ false };
};

#endif