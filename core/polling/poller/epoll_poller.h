
// This file is originally from Trantor - EpollPoller.h

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
#include "core/polling/poller.h"

#if defined __linux__ || defined _WIN32
#include <map>
#include <memory>
using EventList = std::vector<struct epoll_event>;
#endif

class Channel;

class EpollPoller : public Poller {
public:
	explicit EpollPoller(EventLoop *loop);
	virtual ~EpollPoller();
	virtual void poll(int timeoutMs, ChannelList *activeChannels) override;
	virtual void updateChannel(Channel *channel) override;
	virtual void removeChannel(Channel *channel) override;
#ifdef _WIN32
	virtual void postEvent(uint64_t event) override;
	virtual void setEventCallback(const EventCallback &cb) override {
		eventCallback_ = cb;
	}
#endif

private:
#if defined __linux__ || defined _WIN32
	static const int kInitEventListSize = 16;
#ifdef _WIN32
	void *epollfd_;
	EventCallback eventCallback_{ [](uint64_t event) {} };
#else
	int epollfd_;
#endif
	EventList events_;
	void update(int operation, Channel *channel);
#ifndef NDEBUG
	using ChannelMap = std::map<int, Channel *>;
	ChannelMap channels_;
#endif
	void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
#endif
};
