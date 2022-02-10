
// This file is originally from Trantor - Socket.h

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

// TODO Re enable logging

#pragma once

#include "core/net/inet_address.h"
#include "core/log/logger.h"
#include <string>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <fcntl.h>

namespace trantor {
class Socket {

protected:
	Socket(const Socket &) = delete;
	Socket &operator=(const Socket &) = delete;
	// some uncopyable classes maybe support move constructor....
	Socket(Socket &&) noexcept(true) = default;
	Socket &operator=(Socket &&) noexcept(true) = default;

public:
	static int createNonblockingSocketOrDie(int family) {
#ifdef __linux__
		int sock = ::socket(family,
				SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
				IPPROTO_TCP);
#else
		int sock = static_cast<int>(::socket(family, SOCK_STREAM, IPPROTO_TCP));
		setNonBlockAndCloseOnExec(sock);
#endif
		if (sock < 0) {
			LOG_SYSERR << "sockets::createNonblockingOrDie";
			exit(1);
		}
		LOG_TRACE << "sock=" << sock;
		return sock;
	}

	static int getSocketError(int sockfd) {
		int optval;
		socklen_t optlen = static_cast<socklen_t>(sizeof optval);
#ifdef _WIN32
		if (::getsockopt(
					sockfd, SOL_SOCKET, SO_ERROR, (char *)&optval, &optlen) < 0)
#else
		if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
#endif
		{
			return errno;
		} else {
			return optval;
		}
	}

	static int connect(int sockfd, const InetAddress &addr) {
		if (addr.isIpV6())
			return ::connect(sockfd,
					addr.getSockAddr(),
					static_cast<socklen_t>(
							sizeof(struct sockaddr_in6)));
		else
			return ::connect(sockfd,
					addr.getSockAddr(),
					static_cast<socklen_t>(
							sizeof(struct sockaddr_in)));
	}

	static bool isSelfConnect(int sockfd);

	explicit Socket(int sockfd) :
			sockFd_(sockfd) {
	}
	~Socket();
	/// abort if address in use
	void bindAddress(const InetAddress &localaddr);
	/// abort if address in use
	void listen();
	int accept(InetAddress *peeraddr);
	void closeWrite();
	int read(char *buffer, uint64_t len);
	int fd() {
		return sockFd_;
	}
	static struct sockaddr_in6 getLocalAddr(int sockfd);
	static struct sockaddr_in6 getPeerAddr(int sockfd);

	///
	/// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
	///
	void setTcpNoDelay(bool on);

	///
	/// Enable/disable SO_REUSEADDR
	///
	void setReuseAddr(bool on);

	///
	/// Enable/disable SO_REUSEPORT
	///
	void setReusePort(bool on);

	///
	/// Enable/disable SO_KEEPALIVE
	///
	void setKeepAlive(bool on);
	int getSocketError();

protected:
	int sockFd_;

public:
	// taken from muduo
	static void setNonBlockAndCloseOnExec(int sockfd) {
#ifdef _WIN32
		// TODO how to set FD_CLOEXEC on windows? is it necessary?
		u_long arg = 1;
		auto ret = ioctlsocket(sockfd, (long)FIONBIO, &arg);
		if (ret) {
			LOG_ERROR << "ioctlsocket error";
		}
#else
		// non-block
		int flags = ::fcntl(sockfd, F_GETFL, 0);
		flags |= O_NONBLOCK;
		int ret = ::fcntl(sockfd, F_SETFL, flags);
		// TODO check

		// close-on-exec
		flags = ::fcntl(sockfd, F_GETFD, 0);
		flags |= FD_CLOEXEC;
		ret = ::fcntl(sockfd, F_SETFD, flags);
		// TODO check

		(void)ret;
#endif
	}
};

} // namespace trantor
