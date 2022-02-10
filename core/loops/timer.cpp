
// This file is originally from Trantor - Timer.cc

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

#include "timer.h"
#include "core/log/logger.h"
#include "core/loops/event_loop.h"

std::atomic<TimerId> Timer::timersCreated_ = ATOMIC_VAR_INIT(InvalidTimerId);
Timer::Timer(const TimerCallback &cb,
		const TimePoint &when,
		const TimeInterval &interval) :
		callback_(cb),
		when_(when),
		interval_(interval),
		repeat_(interval.count() > 0),
		id_(++timersCreated_) {
}
Timer::Timer(TimerCallback &&cb,
		const TimePoint &when,
		const TimeInterval &interval) :
		callback_(std::move(cb)),
		when_(when),
		interval_(interval),
		repeat_(interval.count() > 0),
		id_(++timersCreated_) {
	// LOG_TRACE<<"Timer move contrustor";
}
void Timer::run() const {
	callback_();
}
void Timer::restart(const TimePoint &now) {
	if (repeat_) {
		when_ = now + interval_;
	} else
		when_ = std::chrono::steady_clock::now();
}
bool Timer::operator<(const Timer &t) const {
	return when_ < t.when_;
}
bool Timer::operator>(const Timer &t) const {
	return when_ > t.when_;
}
