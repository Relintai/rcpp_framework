
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

#include "core/net/tcp_client.h"

#include "connector.h"
#include "core/log/logger.h"
#include "core/loops/event_loop.h"
#include "core/net/connections/tcp_connection_impl.h"

#include <algorithm>
#include <functional>

#include "core/net/socket.h"

#include <stdio.h> // snprintf

using namespace std::placeholders;

// void removeConnector(const ConnectorPtr &)
// {
//     // connector->
// }
#ifndef _WIN32
TcpClient::IgnoreSigPipe TcpClient::initObj;
#endif

static void defaultConnectionCallback(const TcpConnectionPtr &conn) {
	LOG_TRACE << conn->localAddr().toIpPort() << " -> " << conn->peerAddr().toIpPort() << " is " << (conn->connected() ? "UP" : "DOWN");
	// do not call conn->forceClose(), because some users want to register
	// message callback only.
}

static void defaultMessageCallback(const TcpConnectionPtr &, MsgBuffer *buf) {
	buf->retrieveAll();
}

TcpClient::TcpClient(EventLoop *loop, const InetAddress &serverAddr, const std::string &nameArg) :
		loop_(loop),
		connector_(new Connector(loop, serverAddr, false)),
		name_(nameArg),
		connectionCallback_(defaultConnectionCallback),
		messageCallback_(defaultMessageCallback),
		retry_(false),
		connect_(true) {

	connector_->setNewConnectionCallback(std::bind(&TcpClient::newConnection, this, _1));

	connector_->setErrorCallback([this]() {
		if (connectionErrorCallback_) {
			connectionErrorCallback_();
		}
	});

	LOG_TRACE << "TcpClient::TcpClient[" << name_ << "] - connector ";
}

TcpClient::~TcpClient() {
	LOG_TRACE << "TcpClient::~TcpClient[" << name_ << "] - connector ";

	TcpConnectionImplPtr conn;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		conn = std::dynamic_pointer_cast<TcpConnectionImpl>(connection_);
	}

	if (conn) {
		assert(loop_ == conn->getLoop());
		// TODO: not 100% safe, if we are in different thread
		auto loop = loop_;
		loop_->runInLoop([conn, loop]() {
			conn->setCloseCallback([loop](const TcpConnectionPtr &connPtr) {
				loop->queueInLoop([connPtr]() {
					static_cast<TcpConnectionImpl *>(connPtr.get())->connectDestroyed();
				});
			});
		});

		conn->forceClose();
	} else {
		/// TODO need test in this condition
		connector_->stop();
	}
}

void TcpClient::connect() {
	// TODO: check state
	LOG_TRACE << "TcpClient::connect[" << name_ << "] - connecting to "
			  << connector_->serverAddress().toIpPort();
	connect_ = true;
	connector_->start();
}

void TcpClient::disconnect() {
	connect_ = false;

	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (connection_) {
			connection_->shutdown();
		}
	}
}

void TcpClient::stop() {
	connect_ = false;
	connector_->stop();
}

void TcpClient::newConnection(int sockfd) {
	loop_->assertInLoopThread();
	InetAddress peerAddr(Socket::getPeerAddr(sockfd));
	InetAddress localAddr(Socket::getLocalAddr(sockfd));
	// TODO poll with zero timeout to double confirm the new connection
	// TODO use make_shared if necessary
	std::shared_ptr<TcpConnectionImpl> conn;

	if (sslCtxPtr_) {
#ifdef USE_OPENSSL
		conn = std::make_shared<TcpConnectionImpl>(loop_, sockfd, localAddr, peerAddr, sslCtxPtr_, false, validateCert_, SSLHostName_);
#else
		LOG_FATAL << "OpenSSL is not found in your system!";
		abort();
#endif
	} else {
		conn = std::make_shared<TcpConnectionImpl>(loop_, sockfd, localAddr, peerAddr);
	}

	conn->setConnectionCallback(connectionCallback_);
	conn->setRecvMsgCallback(messageCallback_);
	conn->setWriteCompleteCallback(writeCompleteCallback_);
	conn->setCloseCallback(std::bind(&TcpClient::removeConnection, this, _1));

	{
		std::lock_guard<std::mutex> lock(mutex_);
		connection_ = conn;
	}

	conn->setSSLErrorCallback([this](SSLError err) {
		if (sslErrorCallback_) {
			sslErrorCallback_(err);
		}
	});

	conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr &conn) {
	loop_->assertInLoopThread();
	assert(loop_ == conn->getLoop());

	{
		std::lock_guard<std::mutex> lock(mutex_);
		assert(connection_ == conn);
		connection_.reset();
	}

	loop_->queueInLoop(std::bind(&TcpConnectionImpl::connectDestroyed, std::dynamic_pointer_cast<TcpConnectionImpl>(conn)));

	if (retry_ && connect_) {
		LOG_TRACE << "TcpClient::connect[" << name_ << "] - Reconnecting to " << connector_->serverAddress().toIpPort();
		connector_->restart();
	}
}

void TcpClient::enableSSL(bool useOldTLS, bool validateCert, std::string hostname, const std::vector<std::pair<std::string, std::string> > &sslConfCmds) {
#ifdef USE_OPENSSL
	/* Create a new OpenSSL context */
	sslCtxPtr_ = newSSLContext(useOldTLS, validateCert, sslConfCmds);

	validateCert_ = validateCert;

	if (!hostname.empty()) {
		std::transform(hostname.begin(), hostname.end(), hostname.begin(), tolower);
		SSLHostName_ = std::move(hostname);
	}

#else
	LOG_FATAL << "OpenSSL is not found in your system!";
	abort();
#endif
}
