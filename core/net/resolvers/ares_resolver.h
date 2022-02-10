
// This file is originally from Trantor - AresResolver.h

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

#include "core/loops/event_loop_thread.h"
#include "core/net/resolver.h"
#include <string.h>
#include <map>
#include <memory>

extern "C" {
struct hostent;
struct ares_channeldata;
using ares_channel = struct ares_channeldata *;
}

class AresResolver : public Resolver,
					 public std::enable_shared_from_this<AresResolver> {
protected:
	AresResolver(const AresResolver &) = delete;
	AresResolver &operator=(const AresResolver &) = delete;
	// some uncopyable classes maybe support move constructor....
	AresResolver(AresResolver &&) noexcept(true) = default;
	AresResolver &operator=(AresResolver &&) noexcept(true) = default;

public:
	AresResolver(EventLoop *loop, size_t timeout);
	~AresResolver();

	virtual void resolve(const std::string &hostname,
			const Callback &cb) override {
		bool cached = false;
		InetAddress inet;
		{
			std::lock_guard<std::mutex> lock(globalMutex());
			auto iter = globalCache().find(hostname);
			if (iter != globalCache().end()) {
				auto &cachedAddr = iter->second;
				if (timeout_ == 0 ||
						cachedAddr.second.after(timeout_) > Date::date()) {
					struct sockaddr_in addr;
					memset(&addr, 0, sizeof addr);
					addr.sin_family = AF_INET;
					addr.sin_port = 0;
					addr.sin_addr = cachedAddr.first;
					inet = InetAddress(addr);
					cached = true;
				}
			}
		}
		if (cached) {
			cb(inet);
			return;
		}
		if (loop_->isInLoopThread()) {
			resolveInLoop(hostname, cb);
		} else {
			loop_->queueInLoop([thisPtr = shared_from_this(), hostname, cb]() {
				thisPtr->resolveInLoop(hostname, cb);
			});
		}
	}

private:
	struct QueryData {
		AresResolver *owner_;
		Callback callback_;
		std::string hostname_;
		QueryData(AresResolver *o,
				const Callback &cb,
				const std::string &hostname) :
				owner_(o), callback_(cb), hostname_(hostname) {
		}
	};
	void resolveInLoop(const std::string &hostname, const Callback &cb);
	void init();
	EventLoop *loop_;
	ares_channel ctx_{ nullptr };
	bool timerActive_{ false };
	using ChannelList = std::map<int, std::unique_ptr<Channel> >;
	ChannelList channels_;
	static std::unordered_map<std::string,
			std::pair<struct in_addr, Date> > &
	globalCache() {
		static std::unordered_map<std::string,
				std::pair<struct in_addr, Date> >
				dnsCache;
		return dnsCache;
	}
	static std::mutex &globalMutex() {
		static std::mutex mutex_;
		return mutex_;
	}
	static EventLoop *getLoop() {
		static EventLoopThread loopThread;
		loopThread.run();
		return loopThread.getLoop();
	}
	const size_t timeout_{ 60 };

	void onRead(int sockfd);
	void onTimer();
	void onQueryResult(int status,
			struct hostent *result,
			const std::string &hostname,
			const Callback &callback);
	void onSockCreate(int sockfd, int type);
	void onSockStateChange(int sockfd, bool read, bool write);

	static void ares_hostcallback_(void *data,
			int status,
			int timeouts,
			struct hostent *hostent);
#ifdef _WIN32
	static int ares_sock_createcallback_(SOCKET sockfd, int type, void *data);
#else
	static int ares_sock_createcallback_(int sockfd, int type, void *data);
#endif
	static void ares_sock_statecallback_(void *data,
#ifdef _WIN32
			SOCKET sockfd,
#else
			int sockfd,
#endif
			int read,
			int write);
	struct LibraryInitializer {
		LibraryInitializer();
		~LibraryInitializer();
	};
	static LibraryInitializer libraryInitializer_;
};
