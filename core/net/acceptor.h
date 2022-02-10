
// This file is originally from Trantor - Acceptor.h

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

#include "channel.h"
#include "core/net/inet_address.h"
#include "core/net/socket.h"
#include "core/loops/event_loop.h"
#include <functional>

// Inherit from socket?
// Could be called ServerSocket
// New connection callback -> make it a virtual func?

using NewConnectionCallback = std::function<void(int fd, const InetAddress &)>;
class Acceptor {
protected:
	Acceptor(const Acceptor &) = delete;
	Acceptor &operator=(const Acceptor &) = delete;
	// some uncopyable classes maybe support move constructor....
	Acceptor(Acceptor &&) noexcept(true) = default;
	Acceptor &operator=(Acceptor &&) noexcept(true) = default;

public:
	Acceptor(EventLoop *loop,
			const InetAddress &addr,
			bool reUseAddr = true,
			bool reUsePort = true);
	~Acceptor();
	const InetAddress &addr() const {
		return addr_;
	}
	void setNewConnectionCallback(const NewConnectionCallback &cb) {
		newConnectionCallback_ = cb;
	};
	void listen();

protected:
#ifndef _WIN32
	int idleFd_;
#endif
	Socket sock_;
	InetAddress addr_;
	EventLoop *loop_;
	NewConnectionCallback newConnectionCallback_;
	Channel acceptChannel_;
	void readCallback();
};