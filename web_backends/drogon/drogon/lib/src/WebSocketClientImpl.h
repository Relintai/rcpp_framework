/**
 *
 *  @file WebSocketClientImpl.h
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
#include <drogon/WebSocketClient.h>
#include "core/loops/event_loop.h"
#include "core/net/tcp_client.h"
#include <trantor/utils/NonCopyable.h>

#include <memory>
#include <string>

namespace drogon {
class WebSocketClientImpl
		: public WebSocketClient,
		  public std::enable_shared_from_this<WebSocketClientImpl> {
public:
	WebSocketConnectionPtr getConnection() override;

	void setMessageHandler(
			const std::function<void(std::string &&message,
					const WebSocketClientPtr &,
					const WebSocketMessageType &)> &callback)
			override {
		messageCallback_ = callback;
	}

	void setConnectionClosedHandler(
			const std::function<void(const WebSocketClientPtr &)> &callback)
			override {
		connectionClosedCallback_ = callback;
	}

	void connectToServer(const HttpRequestPtr &request,
			const WebSocketRequestCallback &callback) override;

	EventLoop *getLoop() override {
		return loop_;
	}

	WebSocketClientImpl(EventLoop *loop,
			const InetAddress &addr,
			bool useSSL = false,
			bool useOldTLS = false,
			bool validateCert = true);

	WebSocketClientImpl(EventLoop *loop,
			const std::string &hostString,
			bool useOldTLS = false,
			bool validateCert = true);

	~WebSocketClientImpl() override;

private:
	std::shared_ptr<TcpClient> tcpClientPtr_;
	EventLoop *loop_;
	InetAddress serverAddr_;
	std::string domain_;
	bool useSSL_{ false };
	bool useOldTLS_{ false };
	bool validateCert_{ true };
	bool upgraded_{ false };
	std::string wsKey_;
	std::string wsAccept_;

	HttpRequestPtr upgradeRequest_;
	std::function<void(std::string &&,
			const WebSocketClientPtr &,
			const WebSocketMessageType &)>
			messageCallback_ = [](std::string &&,
									   const WebSocketClientPtr &,
									   const WebSocketMessageType &) {};
	std::function<void(const WebSocketClientPtr &)> connectionClosedCallback_ =
			[](const WebSocketClientPtr &) {};
	WebSocketRequestCallback requestCallback_;
	WebSocketConnectionImplPtr websockConnPtr_;

	void connectToServerInLoop();
	void sendReq(const TcpConnectionPtr &connPtr);
	void onRecvMessage(const TcpConnectionPtr &, MsgBuffer *);
	void onRecvWsMessage(const TcpConnectionPtr &,
			MsgBuffer *);
	void reconnect();
	void createTcpClient();
	std::shared_ptr<Resolver> resolver_;
};

} // namespace drogon
