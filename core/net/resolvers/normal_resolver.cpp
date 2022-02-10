
// This file is originally from Trantor - NormalResolver.cc

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

#include "normal_resolver.h"
#include "core/log/logger.h"
#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h> // memset
#include <sys/socket.h>
#endif

using namespace trantor;

std::shared_ptr<Resolver> Resolver::newResolver(trantor::EventLoop *,
		size_t timeout) {
	return std::make_shared<NormalResolver>(timeout);
}
bool Resolver::isCAresUsed() {
	return false;
}
void NormalResolver::resolve(const std::string &hostname,
		const Callback &callback) {
	{
		std::lock_guard<std::mutex> guard(globalMutex());
		auto iter = globalCache().find(hostname);
		if (iter != globalCache().end()) {
			auto &cachedAddr = iter->second;
			if (timeout_ == 0 || cachedAddr.second.after(static_cast<double>(
										 timeout_)) > trantor::Date::date()) {
				callback(cachedAddr.first);
				return;
			}
		}
	}

	concurrentTaskQueue().runTaskInQueue(
			[thisPtr = shared_from_this(), callback, hostname]() {
				{
					std::lock_guard<std::mutex> guard(thisPtr->globalMutex());
					auto iter = thisPtr->globalCache().find(hostname);
					if (iter != thisPtr->globalCache().end()) {
						auto &cachedAddr = iter->second;
						if (thisPtr->timeout_ == 0 ||
								cachedAddr.second.after(static_cast<double>(
										thisPtr->timeout_)) > trantor::Date::date()) {
							callback(cachedAddr.first);
							return;
						}
					}
				}
				struct addrinfo hints, *res;
				memset(&hints, 0, sizeof(hints));
				hints.ai_family = PF_UNSPEC;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_flags = AI_PASSIVE;
				auto error = getaddrinfo(hostname.data(), nullptr, &hints, &res);
				if (error == -1 || res == nullptr) {
					LOG_SYSERR << "InetAddress::resolve";
					callback(InetAddress{});
					return;
				}
				InetAddress inet;
				if (res->ai_family == AF_INET) {
					struct sockaddr_in addr;
					memset(&addr, 0, sizeof addr);
					addr = *reinterpret_cast<struct sockaddr_in *>(res->ai_addr);
					inet = InetAddress(addr);
				} else if (res->ai_family == AF_INET6) {
					struct sockaddr_in6 addr;
					memset(&addr, 0, sizeof addr);
					addr = *reinterpret_cast<struct sockaddr_in6 *>(res->ai_addr);
					inet = InetAddress(addr);
				}
				callback(inet);
				{
					std::lock_guard<std::mutex> guard(thisPtr->globalMutex());
					auto &addrItem = thisPtr->globalCache()[hostname];
					addrItem.first = inet;
					addrItem.second = trantor::Date::date();
				}
				return;
			});
}
