
// This file is originally from Trantor - Poller.h

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

#include "event_loop.h"

#include <map>
#include <memory>

class Channel;
#ifdef _WIN32
using EventCallback = std::function<void(uint64_t)>;
#endif
class Poller {
protected:
	Poller(const Poller &) = delete;
	Poller &operator=(const Poller &) = delete;
	// some uncopyable classes maybe support move constructor....
	Poller(Poller &&) noexcept(true) = default;
	Poller &operator=(Poller &&) noexcept(true) = default;

public:
	explicit Poller(EventLoop *loop) :
			ownerLoop_(loop){};
	virtual ~Poller() {
	}
	void assertInLoopThread() {
		ownerLoop_->assertInLoopThread();
	}
	virtual void poll(int timeoutMs, ChannelList *activeChannels) = 0;
	virtual void updateChannel(Channel *channel) = 0;
	virtual void removeChannel(Channel *channel) = 0;
#ifdef _WIN32
	virtual void postEvent(uint64_t event) = 0;
	virtual void setEventCallback(const EventCallback &cb) = 0;
#endif
	virtual void resetAfterFork() {
	}
	static Poller *newPoller(EventLoop *loop);

private:
	EventLoop *ownerLoop_;
};
