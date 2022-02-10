
// This file is originally from Moduo -> Trantor - TcpClient.h

// Copyright (c) 2016-2021, Tao An.  All rights reserved.
// Copyright 2010, Shuo Chen.  All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//   * Neither the name of Tao An nor the names of other contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "core/loops/event_loop.h"
#include "core/net/inet_address.h"
#include "tcp_connection.h"
#include <signal.h>
#include <atomic>
#include <functional>
#include <thread>

class Connector;
using ConnectorPtr = std::shared_ptr<Connector>;
class SSLContext;

//maybe:
//Reference -> Socket -> ConnectionListener (Channel?) -> Connector (ClientSocket) -> TcpClient -> HTTP...
//                                                     -> Acceptor (ServerSocket) -> TcpServer -> HTTPServer
//Reference -> TcpConnection (with a connectionlistener member) -> TcpConnectionDefault 
//Also todo move around the core net classes a bit more


//should be ConnectionListener derived
//Inherit from Connector (which Could be called ClientSocket)? 

class TcpClient {
protected:
	TcpClient(const TcpClient &) = delete;
	TcpClient &operator=(const TcpClient &) = delete;
	// some uncopyable classes maybe support move constructor....
	TcpClient(TcpClient &&) noexcept(true) = default;
	TcpClient &operator=(TcpClient &&) noexcept(true) = default;

public:
	TcpClient(EventLoop *loop, const InetAddress &serverAddr, const std::string &nameArg);
	~TcpClient();

	void connect();
	void disconnect();

	void stop();

	TcpConnectionPtr connection() const {
		std::lock_guard<std::mutex> lock(mutex_);
		return connection_;
	}

	EventLoop *getLoop() const {
		return loop_;
	}

	bool retry() const {
		return retry_;
	}

	void enableRetry() {
		retry_ = true;
	}

	const std::string &name() const {
		return name_;
	}

	void setConnectionCallback(const ConnectionCallback &cb) {
		connectionCallback_ = cb;
	}
	void setConnectionCallback(ConnectionCallback &&cb) {
		connectionCallback_ = std::move(cb);
	}

	void setConnectionErrorCallback(const ConnectionErrorCallback &cb) {
		connectionErrorCallback_ = cb;
	}

	void setMessageCallback(const RecvMessageCallback &cb) {
		messageCallback_ = cb;
	}
	void setMessageCallback(RecvMessageCallback &&cb) {
		messageCallback_ = std::move(cb);
	}

	/// Not thread safe.
	void setWriteCompleteCallback(const WriteCompleteCallback &cb) {
		writeCompleteCallback_ = cb;
	}
	void setWriteCompleteCallback(WriteCompleteCallback &&cb) {
		writeCompleteCallback_ = std::move(cb);
	}

	void setSSLErrorCallback(const SSLErrorCallback &cb) {
		sslErrorCallback_ = cb;
	}
	void setSSLErrorCallback(SSLErrorCallback &&cb) {
		sslErrorCallback_ = std::move(cb);
	}

	void enableSSL(bool useOldTLS = false, bool validateCert = true, std::string hostname = "", const std::vector<std::pair<std::string, std::string> > &sslConfCmds = {});

private:
	/// Not thread safe, but in loop
	void newConnection(int sockfd);
	/// Not thread safe, but in loop
	void removeConnection(const TcpConnectionPtr &conn);

	EventLoop *loop_;
	ConnectorPtr connector_; // avoid revealing Connector
	const std::string name_;
	ConnectionCallback connectionCallback_;
	ConnectionErrorCallback connectionErrorCallback_;
	RecvMessageCallback messageCallback_;
	WriteCompleteCallback writeCompleteCallback_;
	SSLErrorCallback sslErrorCallback_;
	std::atomic_bool retry_; // atomic
	std::atomic_bool connect_; // atomic
	// always in loop thread
	mutable std::mutex mutex_;
	TcpConnectionPtr connection_; // @GuardedBy mutex_
	std::shared_ptr<SSLContext> sslCtxPtr_;
	bool validateCert_{ false };
	std::string SSLHostName_;

#ifndef _WIN32
	class IgnoreSigPipe {
	public:
		IgnoreSigPipe() {
			::signal(SIGPIPE, SIG_IGN);
		}
	};

	static IgnoreSigPipe initObj;
#endif
};
