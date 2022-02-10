
// This file is originally from Trantor - Connector.h

// Copyright (c) 2016-2021, Tao An.  All rights reserved.

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
#include <atomic>
#include <memory>

// inherit from socket?
// Could be called ClientSocket
// New connection callback -> make it a virtual func?

class Connector : public std::enable_shared_from_this<Connector> {
protected:
	Connector(const Connector &) = delete;
	Connector &operator=(const Connector &) = delete;
	// some uncopyable classes maybe support move constructor....
	Connector(Connector &&) noexcept(true) = default;
	Connector &operator=(Connector &&) noexcept(true) = default;

public:
	using NewConnectionCallback = std::function<void(int sockfd)>;
	using ConnectionErrorCallback = std::function<void()>;

	Connector(EventLoop *loop, const InetAddress &addr, bool retry = true);
	Connector(EventLoop *loop, InetAddress &&addr, bool retry = true);

	void setNewConnectionCallback(const NewConnectionCallback &cb) {
		newConnectionCallback_ = cb;
	}

	void setNewConnectionCallback(NewConnectionCallback &&cb) {
		newConnectionCallback_ = std::move(cb);
	}

	void setErrorCallback(const ConnectionErrorCallback &cb) {
		errorCallback_ = cb;
	}

	void setErrorCallback(ConnectionErrorCallback &&cb) {
		errorCallback_ = std::move(cb);
	}

	const InetAddress &serverAddress() const {
		return serverAddr_;
	}

	void start();
	void restart();
	void stop();

private:
	NewConnectionCallback newConnectionCallback_;
	ConnectionErrorCallback errorCallback_;

	enum class Status {
		Disconnected,
		Connecting,
		Connected
	};
	
	static constexpr int kMaxRetryDelayMs = 30 * 1000;
	static constexpr int kInitRetryDelayMs = 500;
	std::shared_ptr<Channel> channelPtr_;
	EventLoop *loop_;
	InetAddress serverAddr_;

	std::atomic_bool connect_{ false };
	std::atomic<Status> status_{ Status::Disconnected };

	int retryInterval_{ kInitRetryDelayMs };
	int maxRetryInterval_{ kMaxRetryDelayMs };

	bool retry_;

	void startInLoop();
	void connect();
	void connecting(int sockfd);
	int removeAndResetChannel();
	void handleWrite();
	void handleError();
	void retry(int sockfd);
};
