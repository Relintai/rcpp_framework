/**
 *
 *  @file HttpServer.h
 *  @author An Tao
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

#include "impl_forwards.h"
#include "core/net/tcp_server.h"
#include "core/loops/callbacks.h"
#include <trantor/utils/NonCopyable.h>
#include <functional>
#include <string>
#include <vector>

namespace drogon {
class HttpServer : NonCopyable {
public:
	HttpServer(EventLoop *loop,
			const InetAddress &listenAddr,
			const std::string &name,
			const std::vector<
					std::function<HttpResponsePtr(const HttpRequestPtr &)> >
					&syncAdvices,
			const std::vector<std::function<void(const HttpRequestPtr &,
					const HttpResponsePtr &)> >
					&preSendingAdvices);

	~HttpServer();

	EventLoop *getLoop() const {
		return server_.getLoop();
	}

	void setHttpAsyncCallback(const HttpAsyncCallback &cb) {
		httpAsyncCallback_ = cb;
	}

	void setNewWebsocketCallback(const WebSocketNewAsyncCallback &cb) {
		newWebsocketCallback_ = cb;
	}
	void setConnectionCallback(const ConnectionCallback &cb) {
		connectionCallback_ = cb;
	}
	void setIoLoopThreadPool(
			const std::shared_ptr<EventLoopThreadPool> &pool) {
		server_.setIoLoopThreadPool(pool);
	}
	void setIoLoopNum(int numThreads) {
		server_.setIoLoopNum(numThreads);
	}
	void kickoffIdleConnections(size_t timeout) {
		server_.kickoffIdleConnections(timeout);
	}
	EventLoop *getLoop() {
		return server_.getLoop();
	}
	std::vector<EventLoop *> getIoLoops() {
		return server_.getIoLoops();
	}
	void start();
	void stop();

	void enableSSL(
			const std::string &certPath,
			const std::string &keyPath,
			bool useOldTLS,
			const std::vector<std::pair<std::string, std::string> > &sslConfCmds) {
		server_.enableSSL(certPath, keyPath, useOldTLS, sslConfCmds);
	}

	const InetAddress &address() const {
		return server_.address();
	}

private:
	void onConnection(const TcpConnectionPtr &conn);
	void onMessage(const TcpConnectionPtr &, MsgBuffer *);
	void onRequests(const TcpConnectionPtr &,
			const std::vector<HttpRequestImplPtr> &,
			const std::shared_ptr<HttpRequestParser> &);
	void sendResponse(const TcpConnectionPtr &,
			const HttpResponsePtr &,
			bool isHeadMethod);
	void sendResponses(
			const TcpConnectionPtr &conn,
			const std::vector<std::pair<HttpResponsePtr, bool> > &responses,
			MsgBuffer &buffer);
	TcpServer server_;
	HttpAsyncCallback httpAsyncCallback_;
	WebSocketNewAsyncCallback newWebsocketCallback_;
	ConnectionCallback connectionCallback_;
	const std::vector<std::function<HttpResponsePtr(const HttpRequestPtr &)> >
			&syncAdvices_;
	const std::vector<
			std::function<void(const HttpRequestPtr &, const HttpResponsePtr &)> >
			&preSendingAdvices_;
};

} // namespace drogon
