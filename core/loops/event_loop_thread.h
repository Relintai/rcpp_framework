/**
 *
 *  @file EventLoopThread.h
 *  @author An Tao
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
