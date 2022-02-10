
// This file is originally from Trantor - Timer.h

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

#include "core/loops/callbacks.h"
#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>

using TimerId = uint64_t;
using TimePoint = std::chrono::steady_clock::time_point;
using TimeInterval = std::chrono::microseconds;

class Timer {
protected:
	Timer(const Timer &) = delete;
	Timer &operator=(const Timer &) = delete;
	// some uncopyable classes maybe support move constructor....
	Timer(Timer &&) noexcept(true) = default;
	Timer &operator=(Timer &&) noexcept(true) = default;

public:
	Timer(const TimerCallback &cb,
			const TimePoint &when,
			const TimeInterval &interval);
	Timer(TimerCallback &&cb,
			const TimePoint &when,
			const TimeInterval &interval);
	~Timer() {
		//   std::cout<<"Timer unconstract!"<<std::endl;
	}
	void run() const;
	void restart(const TimePoint &now);
	bool operator<(const Timer &t) const;
	bool operator>(const Timer &t) const;
	const TimePoint &when() const {
		return when_;
	}
	bool isRepeat() {
		return repeat_;
	}
	TimerId id() {
		return id_;
	}

private:
	TimerCallback callback_;
	TimePoint when_;
	const TimeInterval interval_;
	const bool repeat_;
	const TimerId id_;
	static std::atomic<TimerId> timersCreated_;
};

