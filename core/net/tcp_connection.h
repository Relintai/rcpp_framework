
// This file is originally from Trantor - TcpConnection.h

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

#include "core/containers/msg_buffer.h"
#include "core/net/callbacks.h"
#include "core/loops/event_loop.h"
#include "core/net/inet_address.h"
#include <functional>
#include <memory>
#include <string>

// add new class TcpConnectionListener or ConnectionListener
//set_listener(ConnectionListener)
// instead of callbacks

class SSLContext;
std::shared_ptr<SSLContext> newSSLServerContext(const std::string &certPath, const std::string &keyPath, bool useOldTLS = false, const std::vector<std::pair<std::string, std::string> > &sslConfCmds = {});

class TcpConnection {
public:
	TcpConnection() = default;
	virtual ~TcpConnection(){};

	virtual void send(const char *msg, size_t len) = 0;
	virtual void send(const void *msg, size_t len) = 0;
	virtual void send(const std::string &msg) = 0;
	virtual void send(std::string &&msg) = 0;
	virtual void send(const MsgBuffer &buffer) = 0;
	virtual void send(MsgBuffer &&buffer) = 0;
	virtual void send(const std::shared_ptr<std::string> &msgPtr) = 0;
	virtual void send(const std::shared_ptr<MsgBuffer> &msgPtr) = 0;

	virtual void sendFile(const char *fileName, size_t offset = 0, size_t length = 0) = 0;

	virtual const InetAddress &localAddr() const = 0;
	virtual const InetAddress &peerAddr() const = 0;

	virtual bool connected() const = 0;
	virtual bool disconnected() const = 0;

	virtual MsgBuffer *getRecvBuffer() = 0;


	virtual void setHighWaterMarkCallback(const HighWaterMarkCallback &cb, size_t markLen) = 0;

	virtual void setTcpNoDelay(bool on) = 0;

	virtual void shutdown() = 0;
	virtual void forceClose() = 0;


	virtual EventLoop *getLoop() = 0;

	void setContext(const std::shared_ptr<void> &context) {
		contextPtr_ = context;
	}
	void setContext(std::shared_ptr<void> &&context) {
		contextPtr_ = std::move(context);
	}

	template <typename T>
	std::shared_ptr<T> getContext() const {
		return std::static_pointer_cast<T>(contextPtr_);
	}

	bool hasContext() const {
		return (bool)contextPtr_;
	}

	void clearContext() {
		contextPtr_.reset();
	}

	virtual void keepAlive() = 0;
	virtual bool isKeepAlive() = 0;

	virtual size_t bytesSent() const = 0;
	virtual size_t bytesReceived() const = 0;

	virtual bool isSSLConnection() const = 0;

	virtual void startClientEncryption(std::function<void()> callback, bool useOldTLS = false, bool validateCert = true, std::string hostname = "", const std::vector<std::pair<std::string, std::string> > &sslConfCmds = {}) = 0;

	virtual void startServerEncryption(const std::shared_ptr<SSLContext> &ctx, std::function<void()> callback) = 0;

protected:
	bool validateCert_ = false;

private:
	std::shared_ptr<void> contextPtr_;
};

