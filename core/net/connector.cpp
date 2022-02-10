
// This file is originally from Trantor - Connector.cc

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

#include "connector.h"
#include "core/loops/channel.h"
#include "core/net/socket.h"

Connector::Connector(EventLoop *loop, const InetAddress &addr, bool retry) :
		loop_(loop), serverAddr_(addr), retry_(retry) {
}

Connector::Connector(EventLoop *loop, InetAddress &&addr, bool retry) :
		loop_(loop), serverAddr_(std::move(addr)), retry_(retry) {
}

void Connector::start() {
	connect_ = true;
	loop_->runInLoop([this]() { startInLoop(); });
}

void Connector::restart() {
}

void Connector::stop() {
}

void Connector::startInLoop() {
	loop_->assertInLoopThread();
	assert(status_ == Status::Disconnected);
	if (connect_) {
		connect();
	} else {
		LOG_DEBUG << "do not connect";
	}
}

void Connector::connect() {
	int sockfd = Socket::createNonblockingSocketOrDie(serverAddr_.family());
	errno = 0;
	int ret = Socket::connect(sockfd, serverAddr_);
	int savedErrno = (ret == 0) ? 0 : errno;
	switch (savedErrno) {
		case 0:
		case EINPROGRESS:
		case EINTR:
		case EISCONN:
			LOG_TRACE << "connecting";
			connecting(sockfd);
			break;

		case EAGAIN:
		case EADDRINUSE:
		case EADDRNOTAVAIL:
		case ECONNREFUSED:
		case ENETUNREACH:
			if (retry_) {
				retry(sockfd);
			}
			break;

		case EACCES:
		case EPERM:
		case EAFNOSUPPORT:
		case EALREADY:
		case EBADF:
		case EFAULT:
		case ENOTSOCK:
			LOG_SYSERR << "connect error in Connector::startInLoop "
					   << savedErrno;
#ifndef _WIN32
			::close(sockfd);
#else
			closesocket(sockfd);
#endif
			if (errorCallback_)
				errorCallback_();
			break;

		default:
			LOG_SYSERR << "Unexpected error in Connector::startInLoop "
					   << savedErrno;
#ifndef _WIN32
			::close(sockfd);
#else
			closesocket(sockfd);
#endif
			if (errorCallback_)
				errorCallback_();
			break;
	}
}

void Connector::connecting(int sockfd) {
	status_ = Status::Connecting;
	assert(!channelPtr_);
	channelPtr_.reset(new Channel(loop_, sockfd));
	channelPtr_->setWriteCallback(
			std::bind(&Connector::handleWrite, shared_from_this()));
	channelPtr_->setErrorCallback(
			std::bind(&Connector::handleError, shared_from_this()));
	channelPtr_->setCloseCallback(
			std::bind(&Connector::handleError, shared_from_this()));
	LOG_TRACE << "connecting:" << sockfd;
	channelPtr_->enableWriting();
}

int Connector::removeAndResetChannel() {
	channelPtr_->disableAll();
	channelPtr_->remove();
	int sockfd = channelPtr_->fd();
	// Can't reset channel_ here, because we are inside Channel::handleEvent
	loop_->queueInLoop([this]() { channelPtr_.reset(); });
	return sockfd;
}

void Connector::handleWrite() {
	if (status_ == Status::Connecting) {
		int sockfd = removeAndResetChannel();
		int err = Socket::getSocketError(sockfd);
		if (err) {
			LOG_WARN << "Connector::handleWrite - SO_ERROR = " << err << " "
					 << strerror_tl(err);
			if (retry_) {
				retry(sockfd);
			} else {
#ifndef _WIN32
				::close(sockfd);
#else
				closesocket(sockfd);
#endif
			}
			if (errorCallback_) {
				errorCallback_();
			}
		} else if (Socket::isSelfConnect(sockfd)) {
			LOG_WARN << "Connector::handleWrite - Self connect";
			if (retry_) {
				retry(sockfd);
			} else {
#ifndef _WIN32
				::close(sockfd);
#else
				closesocket(sockfd);
#endif
			}
			if (errorCallback_) {
				errorCallback_();
			}
		} else {
			status_ = Status::Connected;
			if (connect_) {
				newConnectionCallback_(sockfd);
			} else {
#ifndef _WIN32
				::close(sockfd);
#else
				closesocket(sockfd);
#endif
			}
		}
	} else {
		// what happened?
		assert(status_ == Status::Disconnected);
	}
}

void Connector::handleError() {
	if (status_ == Status::Connecting) {
		status_ = Status::Disconnected;
		int sockfd = removeAndResetChannel();
		int err = Socket::getSocketError(sockfd);
		LOG_TRACE << "SO_ERROR = " << err << " " << strerror_tl(err);
		if (retry_) {
			retry(sockfd);
		} else {
#ifndef _WIN32
			::close(sockfd);
#else
			closesocket(sockfd);
#endif
		}
		if (errorCallback_) {
			errorCallback_();
		}
	}
}

void Connector::retry(int sockfd) {
	assert(retry_);
#ifndef _WIN32
	::close(sockfd);
#else
	closesocket(sockfd);
#endif
	status_ = Status::Disconnected;
	if (connect_) {
		LOG_INFO << "Connector::retry - Retry connecting to "
				 << serverAddr_.toIpPort() << " in " << retryInterval_
				 << " milliseconds. ";
		loop_->runAfter(retryInterval_ / 1000.0,
				std::bind(&Connector::startInLoop, shared_from_this()));
		retryInterval_ = retryInterval_ * 2;
		if (retryInterval_ > maxRetryInterval_)
			retryInterval_ = maxRetryInterval_;
	} else {
		LOG_DEBUG << "do not connect";
	}
}
