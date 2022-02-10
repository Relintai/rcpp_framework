
// This file is originally from Trantor - KQueue.h

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

#if (defined(__unix__) && !defined(__linux__)) || \
		(defined(__APPLE__) && defined(__MACH__))
#define USE_KQUEUE
#include <memory>
#include <unordered_map>
#include <vector>
using EventList = std::vector<struct kevent>;
#endif

class Channel;

class KQueue : public Poller {
public:
	explicit KQueue(EventLoop *loop);
	virtual ~KQueue();
	virtual void poll(int timeoutMs, ChannelList *activeChannels) override;
	virtual void updateChannel(Channel *channel) override;
	virtual void removeChannel(Channel *channel) override;
	virtual void resetAfterFork() override;

private:
#ifdef USE_KQUEUE
	static const int kInitEventListSize = 16;
	int kqfd_;
	EventList events_;
	using ChannelMap = std::unordered_map<int, std::pair<int, Channel *> >;
	ChannelMap channels_;

	void fillActiveChannels(int numEvents, ChannelList *activeChannels) const;
	void update(Channel *channel);
#endif
};
