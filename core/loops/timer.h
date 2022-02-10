/**
 *
 *  Timer.h
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

#include "core/loops/callbacks.h"
#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>

namespace trantor {
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

} // namespace trantor
