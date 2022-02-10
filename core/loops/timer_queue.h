
// This file is originally from Trantor - TimerQueue.h

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

#include "timer.h"
#include "core/net/callbacks.h"
#include <atomic>
#include <memory>
#include <queue>
#include <unordered_set>

// class Timer;
class EventLoop;
class Channel;
using TimerPtr = std::shared_ptr<Timer>;
struct TimerPtrComparer {
	bool operator()(const TimerPtr &x, const TimerPtr &y) const {
		return *x > *y;
	}
};

class TimerQueue {
protected:
	TimerQueue(const TimerQueue &) = delete;
	TimerQueue &operator=(const TimerQueue &) = delete;
	// some uncopyable classes maybe support move constructor....
	TimerQueue(TimerQueue &&) noexcept(true) = default;
	TimerQueue &operator=(TimerQueue &&) noexcept(true) = default;

public:
	explicit TimerQueue(EventLoop *loop);
	~TimerQueue();
	TimerId addTimer(const TimerCallback &cb,
			const TimePoint &when,
			const TimeInterval &interval);
	TimerId addTimer(TimerCallback &&cb,
			const TimePoint &when,
			const TimeInterval &interval);
	void addTimerInLoop(const TimerPtr &timer);
	void invalidateTimer(TimerId id);
#ifdef __linux__
	void reset();
#else
	int64_t getTimeout() const;
	void processTimers();
#endif
protected:
	EventLoop *loop_;
#ifdef __linux__
	int timerfd_;
	std::shared_ptr<Channel> timerfdChannelPtr_;
	void handleRead();
#endif
	std::priority_queue<TimerPtr, std::vector<TimerPtr>, TimerPtrComparer>
			timers_;

	bool callingExpiredTimers_;
	bool insert(const TimerPtr &timePtr);
	std::vector<TimerPtr> getExpired();
	void reset(const std::vector<TimerPtr> &expired, const TimePoint &now);
	std::vector<TimerPtr> getExpired(const TimePoint &now);

private:
	std::unordered_set<uint64_t> timerIdSet_;
};
