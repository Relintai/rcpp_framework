
// This file is originally from Trantor - TimingWheel.cc

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

#include "core/loops/timing_wheel.h"


TimingWheel::TimingWheel(EventLoop *loop,
		size_t maxTimeout,
		float ticksInterval,
		size_t bucketsNumPerWheel) :
		loop_(loop),
		ticksInterval_(ticksInterval),
		bucketsNumPerWheel_(bucketsNumPerWheel) {
	assert(maxTimeout > 1);
	assert(ticksInterval > 0);
	assert(bucketsNumPerWheel_ > 1);
	size_t maxTickNum = static_cast<size_t>(maxTimeout / ticksInterval);
	auto ticksNum = bucketsNumPerWheel;
	wheelsNum_ = 1;
	while (maxTickNum > ticksNum) {
		++wheelsNum_;
		ticksNum *= bucketsNumPerWheel_;
	}
	wheels_.resize(wheelsNum_);
	for (size_t i = 0; i < wheelsNum_; ++i) {
		wheels_[i].resize(bucketsNumPerWheel_);
	}
	timerId_ = loop_->runEvery(ticksInterval_, [this]() {
		++ticksCounter_;
		size_t t = ticksCounter_;
		size_t pow = 1;
		for (size_t i = 0; i < wheelsNum_; ++i) {
			if ((t % pow) == 0) {
				EntryBucket tmp;
				{
					// use tmp val to make this critical area as short as
					// possible.
					wheels_[i].front().swap(tmp);
					wheels_[i].pop_front();
					wheels_[i].push_back(EntryBucket());
				}
			}
			pow = pow * bucketsNumPerWheel_;
		}
	});
}

TimingWheel::~TimingWheel() {
	loop_->assertInLoopThread();
	loop_->invalidateTimer(timerId_);
	for (auto iter = wheels_.rbegin(); iter != wheels_.rend(); ++iter) {
		iter->clear();
	}
	LOG_TRACE << "TimingWheel destruct!";
}

void TimingWheel::insertEntry(size_t delay, EntryPtr entryPtr) {
	if (delay <= 0)
		return;
	if (!entryPtr)
		return;
	if (loop_->isInLoopThread()) {
		insertEntryInloop(delay, entryPtr);
	} else {
		loop_->runInLoop(
				[this, delay, entryPtr]() { insertEntryInloop(delay, entryPtr); });
	}
}

void TimingWheel::insertEntryInloop(size_t delay, EntryPtr entryPtr) {
	loop_->assertInLoopThread();

	delay = static_cast<size_t>(delay / ticksInterval_ + 1);
	size_t t = ticksCounter_;
	for (size_t i = 0; i < wheelsNum_; ++i) {
		if (delay <= bucketsNumPerWheel_) {
			wheels_[i][delay - 1].insert(entryPtr);
			break;
		}
		if (i < (wheelsNum_ - 1)) {
			entryPtr = std::make_shared<CallbackEntry>(
					[this, delay, i, t, entryPtr]() {
						if (delay > 0) {
							wheels_[i][(delay + (t % bucketsNumPerWheel_) - 1) %
									   bucketsNumPerWheel_]
									.insert(entryPtr);
						}
					});
		} else {
			// delay is too long to put entry at valid position in wheels;
			wheels_[i][bucketsNumPerWheel_ - 1].insert(entryPtr);
		}
		delay = (delay + (t % bucketsNumPerWheel_) - 1) / bucketsNumPerWheel_;
		t = t / bucketsNumPerWheel_;
	}
}