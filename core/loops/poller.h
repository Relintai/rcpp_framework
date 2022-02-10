/**
 *
 *  Poller.h
 *  An Tao
 *
 *  Public header file in trantor lib.
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the License file.
 *
 *
 */

#pragma once

#include "event_loop.h"

#include <map>
#include <memory>

namespace trantor {
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
} // namespace trantor
