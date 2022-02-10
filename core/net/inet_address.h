
// This file is originally from Moduo -> Trantor - InetAddress.h

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

#ifndef MUDUO_NET_INETADDRESS_H
#define MUDUO_NET_INETADDRESS_H

#ifdef _WIN32
#include <ws2tcpip.h>
using sa_family_t = unsigned short;
using in_addr_t = uint32_t;
using uint16_t = unsigned short;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include <mutex>
#include <string>
#include <unordered_map>

class InetAddress {
public:
	InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);

	InetAddress(const std::string &ip, uint16_t port, bool ipv6 = false);

	explicit InetAddress(const struct sockaddr_in &addr) :
			addr_(addr), isUnspecified_(false) {
	}

	explicit InetAddress(const struct sockaddr_in6 &addr) :
			addr6_(addr), isIpV6_(true), isUnspecified_(false) {
	}

	sa_family_t family() const {
		return addr_.sin_family;
	}

	std::string toIp() const;
	std::string toIpPort() const;
	uint16_t toPort() const;

	bool isIpV6() const {
		return isIpV6_;
	}

	bool isIntranetIp() const;
	bool isLoopbackIp() const;

	const struct sockaddr *getSockAddr() const {
		return static_cast<const struct sockaddr *>((void *)(&addr6_));
	}

	void setSockAddrInet6(const struct sockaddr_in6 &addr6) {
		addr6_ = addr6;
		isIpV6_ = (addr6_.sin6_family == AF_INET6);
		isUnspecified_ = false;
	}

	uint32_t ipNetEndian() const;
	const uint32_t *ip6NetEndian() const;

	uint16_t portNetEndian() const {
		return addr_.sin_port;
	}

	void setPortNetEndian(uint16_t port) {
		addr_.sin_port = port;
	}

	inline bool isUnspecified() const {
		return isUnspecified_;
	}

private:
	union {
		struct sockaddr_in addr_;
		struct sockaddr_in6 addr6_;
	};

	bool isIpV6_{ false };
	bool isUnspecified_{ true };
};

#endif // MUDUO_NET_INETADDRESS_H
