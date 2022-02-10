
// This file is originally from Trantor - EventLoopThread.h

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
#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>
#include <thread>

namespace trantor {
/**
 * @brief This class represents an event loop thread.
 *
 */
class EventLoopThread {
protected:
	EventLoopThread(const EventLoopThread &) = delete;
	EventLoopThread &operator=(const EventLoopThread &) = delete;
	// some uncopyable classes maybe support move constructor....
	EventLoopThread(EventLoopThread &&) noexcept(true) = default;
	EventLoopThread &operator=(EventLoopThread &&) noexcept(true) = default;

public:
	explicit EventLoopThread(const std::string &threadName = "EventLoopThread");
	~EventLoopThread();

	/**
	 * @brief Wait for the event loop to exit.
	 * @note This method blocks the current thread until the event loop exits.
	 */
	void wait();

	/**
	 * @brief Get the pointer of the event loop of the thread.
	 *
	 * @return EventLoop*
	 */
	EventLoop *getLoop() const {
		return loop_;
	}

	/**
	 * @brief Run the event loop of the thread. This method doesn't block the
	 * current thread.
	 *
	 */
	void run();

private:
	EventLoop *loop_;
	std::string loopThreadName_;
	void loopFuncs();
	std::promise<EventLoop *> promiseForLoopPointer_;
	std::promise<int> promiseForRun_;
	std::promise<int> promiseForLoop_;
	std::once_flag once_;
	std::thread thread_;
};

} // namespace trantor
