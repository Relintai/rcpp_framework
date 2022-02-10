
// This file is originally from Trantor - TimerQueue.cc

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

#include "event_loop.h"

#include "channel.h"
#include "timer_queue.h"
#ifdef __linux__
#include <sys/timerfd.h>
#endif
#include <string.h>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#endif

using namespace trantor;
#ifdef __linux__
static int createTimerfd() {
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (timerfd < 0) {
		std::cerr << "create timerfd failed!" << std::endl;
	}
	return timerfd;
}

static struct timespec howMuchTimeFromNow(const TimePoint &when) {
	auto microSeconds = std::chrono::duration_cast<std::chrono::microseconds>(
			when - std::chrono::steady_clock::now())
								.count();
	if (microSeconds < 100) {
		microSeconds = 100;
	}
	struct timespec ts;
	ts.tv_sec = static_cast<time_t>(microSeconds / 1000000);
	ts.tv_nsec = static_cast<long>((microSeconds % 1000000) * 1000);
	return ts;
}
static void resetTimerfd(int timerfd, const TimePoint &expiration) {
	// wake up loop by timerfd_settime()
	struct itimerspec newValue;
	struct itimerspec oldValue;
	memset(&newValue, 0, sizeof(newValue));
	memset(&oldValue, 0, sizeof(oldValue));
	newValue.it_value = howMuchTimeFromNow(expiration);
	int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
	if (ret) {
		// LOG_SYSERR << "timerfd_settime()";
	}
}
static void readTimerfd(int timerfd, const TimePoint &) {
	uint64_t howmany;
	ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
	if (n != sizeof howmany) {
		LOG_ERROR << "TimerQueue::handleRead() reads " << n
				  << " bytes instead of 8";
	}
}

void TimerQueue::handleRead() {
	loop_->assertInLoopThread();
	const auto now = std::chrono::steady_clock::now();
	readTimerfd(timerfd_, now);

	std::vector<TimerPtr> expired = getExpired(now);

	callingExpiredTimers_ = true;
	// cancelingTimers_.clear();
	// safe to callback outside critical section
	for (auto const &timerPtr : expired) {
		if (timerIdSet_.find(timerPtr->id()) != timerIdSet_.end()) {
			timerPtr->run();
		}
	}
	callingExpiredTimers_ = false;

	reset(expired, now);
}
#else
static int64_t howMuchTimeFromNow(const TimePoint &when) {
	auto microSeconds = std::chrono::duration_cast<std::chrono::microseconds>(
			when - std::chrono::steady_clock::now())
								.count();
	if (microSeconds < 1000) {
		microSeconds = 1000;
	}
	return microSeconds / 1000;
}
void TimerQueue::processTimers() {
	loop_->assertInLoopThread();
	const auto now = std::chrono::steady_clock::now();

	std::vector<TimerPtr> expired = getExpired(now);

	callingExpiredTimers_ = true;
	// cancelingTimers_.clear();
	// safe to callback outside critical section
	for (auto const &timerPtr : expired) {
		if (timerIdSet_.find(timerPtr->id()) != timerIdSet_.end()) {
			timerPtr->run();
		}
	}
	callingExpiredTimers_ = false;

	reset(expired, now);
}
#endif
///////////////////////////////////////
TimerQueue::TimerQueue(EventLoop *loop) :
		loop_(loop),
#ifdef __linux__
		timerfd_(createTimerfd()),
		timerfdChannelPtr_(new Channel(loop, timerfd_)),
#endif
		timers_(),
		callingExpiredTimers_(false) {
#ifdef __linux__
	timerfdChannelPtr_->setReadCallback(
			std::bind(&TimerQueue::handleRead, this));
	// we are always reading the timerfd, we disarm it with timerfd_settime.
	timerfdChannelPtr_->enableReading();
#endif
}
#ifdef __linux__
void TimerQueue::reset() {
	loop_->runInLoop([this]() {
		timerfdChannelPtr_->disableAll();
		timerfdChannelPtr_->remove();
		close(timerfd_);
		timerfd_ = createTimerfd();
		timerfdChannelPtr_ = std::make_shared<Channel>(loop_, timerfd_);
		timerfdChannelPtr_->setReadCallback(
				std::bind(&TimerQueue::handleRead, this));
		// we are always reading the timerfd, we disarm it with timerfd_settime.
		timerfdChannelPtr_->enableReading();
		if (!timers_.empty()) {
			const auto nextExpire = timers_.top()->when();
			resetTimerfd(timerfd_, nextExpire);
		}
	});
}
#endif
TimerQueue::~TimerQueue() {
#ifdef __linux__
	auto chlPtr = timerfdChannelPtr_;
	auto fd = timerfd_;
	loop_->runInLoop([chlPtr, fd]() {
		chlPtr->disableAll();
		chlPtr->remove();
		::close(fd);
	});
#endif
}

TimerId TimerQueue::addTimer(const TimerCallback &cb,
		const TimePoint &when,
		const TimeInterval &interval) {
	std::shared_ptr<Timer> timerPtr =
			std::make_shared<Timer>(cb, when, interval);

	loop_->runInLoop([this, timerPtr]() { addTimerInLoop(timerPtr); });
	return timerPtr->id();
}
TimerId TimerQueue::addTimer(TimerCallback &&cb,
		const TimePoint &when,
		const TimeInterval &interval) {
	std::shared_ptr<Timer> timerPtr =
			std::make_shared<Timer>(std::move(cb), when, interval);

	loop_->runInLoop([this, timerPtr]() { addTimerInLoop(timerPtr); });
	return timerPtr->id();
}
void TimerQueue::addTimerInLoop(const TimerPtr &timer) {
	loop_->assertInLoopThread();
	timerIdSet_.insert(timer->id());
	if (insert(timer)) {
// the earliest timer changed
#ifdef __linux__
		resetTimerfd(timerfd_, timer->when());
#endif
	}
}

void TimerQueue::invalidateTimer(TimerId id) {
	loop_->runInLoop([this, id]() { timerIdSet_.erase(id); });
}

bool TimerQueue::insert(const TimerPtr &timerPtr) {
	loop_->assertInLoopThread();
	bool earliestChanged = false;
	if (timers_.size() == 0 || *timerPtr < *timers_.top()) {
		earliestChanged = true;
	}
	timers_.push(timerPtr);
	// std::cout<<"after push new
	// timer:"<<timerPtr->when().microSecondsSinceEpoch()/1000000<<std::endl;
	return earliestChanged;
}
#ifndef __linux__
int64_t TimerQueue::getTimeout() const {
	loop_->assertInLoopThread();
	if (timers_.empty()) {
		return 10000;
	} else {
		return howMuchTimeFromNow(timers_.top()->when());
	}
}
#endif

std::vector<TimerPtr> TimerQueue::getExpired(const TimePoint &now) {
	std::vector<TimerPtr> expired;
	while (!timers_.empty()) {
		if (timers_.top()->when() < now) {
			expired.push_back(timers_.top());
			timers_.pop();
		} else
			break;
	}
	return expired;
}
void TimerQueue::reset(const std::vector<TimerPtr> &expired,
		const TimePoint &now) {
	loop_->assertInLoopThread();
	for (auto const &timerPtr : expired) {
		auto iter = timerIdSet_.find(timerPtr->id());
		if (iter != timerIdSet_.end()) {
			if (timerPtr->isRepeat()) {
				timerPtr->restart(now);
				insert(timerPtr);
			} else {
				timerIdSet_.erase(iter);
			}
		}
	}
#ifdef __linux__
	if (!timers_.empty()) {
		const auto nextExpire = timers_.top()->when();
		resetTimerfd(timerfd_, nextExpire);
	}
#endif
}
