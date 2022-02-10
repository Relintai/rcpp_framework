
// This file is originally from Trantor - TcpServer.h

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

#include "core/log/logger.h"
#include "core/loops/callbacks.h"
#include "core/loops/event_loop_thread_pool.h"
#include "core/loops/timing_wheel.h"
#include "core/net/inet_address.h"
#include "core/net/tcp_connection.h"
#include <signal.h>
#include <memory>
#include <set>
#include <string>

namespace trantor {
class Acceptor;
class SSLContext;
/**
 * @brief This class represents a TCP server.
 *
 */
class TcpServer {
protected:
	TcpServer(const TcpServer &) = delete;
	TcpServer &operator=(const TcpServer &) = delete;
	// some uncopyable classes maybe support move constructor....
	TcpServer(TcpServer &&) noexcept(true) = default;
	TcpServer &operator=(TcpServer &&) noexcept(true) = default;

public:
	/**
	 * @brief Construct a new TCP server instance.
	 *
	 * @param loop The event loop in which the acceptor of the server is
	 * handled.
	 * @param address The address of the server.
	 * @param name The name of the server.
	 * @param reUseAddr The SO_REUSEADDR option.
	 * @param reUsePort The SO_REUSEPORT option.
	 */
	TcpServer(EventLoop *loop,
			const InetAddress &address,
			const std::string &name,
			bool reUseAddr = true,
			bool reUsePort = true);
	~TcpServer();

	/**
	 * @brief Start the server.
	 *
	 */
	void start();

	/**
	 * @brief Stop the server.
	 *
	 */
	void stop();

	/**
	 * @brief Set the number of event loops in which the I/O of connections to
	 * the server is handled.
	 *
	 * @param num
	 */
	void setIoLoopNum(size_t num) {
		assert(!started_);
		loopPoolPtr_ = std::make_shared<EventLoopThreadPool>(num);
		loopPoolPtr_->start();
	}

	/**
	 * @brief Set the event loops pool in which the I/O of connections to
	 * the server is handled.
	 *
	 * @param pool
	 */
	void setIoLoopThreadPool(const std::shared_ptr<EventLoopThreadPool> &pool) {
		assert(pool->size() > 0);
		assert(!started_);
		loopPoolPtr_ = pool;
		loopPoolPtr_->start();
	}

	/**
	 * @brief Set the message callback.
	 *
	 * @param cb The callback is called when some data is received on a
	 * connection to the server.
	 */
	void setRecvMessageCallback(const RecvMessageCallback &cb) {
		recvMessageCallback_ = cb;
	}
	void setRecvMessageCallback(RecvMessageCallback &&cb) {
		recvMessageCallback_ = std::move(cb);
	}

	/**
	 * @brief Set the connection callback.
	 *
	 * @param cb The callback is called when a connection is established or
	 * closed.
	 */
	void setConnectionCallback(const ConnectionCallback &cb) {
		connectionCallback_ = cb;
	}
	void setConnectionCallback(ConnectionCallback &&cb) {
		connectionCallback_ = std::move(cb);
	}

	/**
	 * @brief Set the write complete callback.
	 *
	 * @param cb The callback is called when data to send is written to the
	 * socket of a connection.
	 */
	void setWriteCompleteCallback(const WriteCompleteCallback &cb) {
		writeCompleteCallback_ = cb;
	}
	void setWriteCompleteCallback(WriteCompleteCallback &&cb) {
		writeCompleteCallback_ = std::move(cb);
	}

	/**
	 * @brief Get the name of the server.
	 *
	 * @return const std::string&
	 */
	const std::string &name() const {
		return serverName_;
	}

	/**
	 * @brief Get the IP and port string of the server.
	 *
	 * @return const std::string
	 */
	const std::string ipPort() const;

	/**
	 * @brief Get the address of the server.
	 *
	 * @return const trantor::InetAddress&
	 */
	const trantor::InetAddress &address() const;

	/**
	 * @brief Get the event loop of the server.
	 *
	 * @return EventLoop*
	 */
	EventLoop *getLoop() const {
		return loop_;
	}

	/**
	 * @brief Get the I/O event loops of the server.
	 *
	 * @return std::vector<EventLoop *>
	 */
	std::vector<EventLoop *> getIoLoops() const {
		return loopPoolPtr_->getLoops();
	}

	/**
	 * @brief An idle connection is a connection that has no read or write, kick
	 * off it after timeout seconds.
	 *
	 * @param timeout
	 */
	void kickoffIdleConnections(size_t timeout) {
		loop_->runInLoop([this, timeout]() {
			assert(!started_);
			idleTimeout_ = timeout;
		});
	}

	/**
	 * @brief Enable SSL encryption.
	 *
	 * @param certPath The path of the certificate file.
	 * @param keyPath The path of the private key file.
	 * @param useOldTLS If true, the TLS 1.0 and 1.1 are supported by the
	 * server.
	 * @param sslConfCmds The commands used to call the SSL_CONF_cmd function in
	 * OpenSSL.
	 * @note It's well known that TLS 1.0 and 1.1 are not considered secure in
	 * 2020. And it's a good practice to only use TLS 1.2 and above.
	 */
	void enableSSL(const std::string &certPath,
			const std::string &keyPath,
			bool useOldTLS = false,
			const std::vector<std::pair<std::string, std::string> >
					&sslConfCmds = {});

private:
	EventLoop *loop_;
	std::unique_ptr<Acceptor> acceptorPtr_;
	void newConnection(int fd, const InetAddress &peer);
	std::string serverName_;
	std::set<TcpConnectionPtr> connSet_;

	RecvMessageCallback recvMessageCallback_;
	ConnectionCallback connectionCallback_;
	WriteCompleteCallback writeCompleteCallback_;

	size_t idleTimeout_{ 0 };
	std::map<EventLoop *, std::shared_ptr<TimingWheel> > timingWheelMap_;
	void connectionClosed(const TcpConnectionPtr &connectionPtr);
	std::shared_ptr<EventLoopThreadPool> loopPoolPtr_;
#ifndef _WIN32
	class IgnoreSigPipe {
	public:
		IgnoreSigPipe() {
			::signal(SIGPIPE, SIG_IGN);
			LOG_TRACE << "Ignore SIGPIPE";
		}
	};

	IgnoreSigPipe initObj;
#endif
	bool started_{ false };

	// OpenSSL SSL context Object;
	std::shared_ptr<SSLContext> sslCtxPtr_;
};

} // namespace trantor
