
// This file is originally from Trantor - EventLoopThreadPool.h

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

#include "event_loop_thread.h"
#include <memory>
#include <vector>

namespace trantor {
/**
 * @brief This class represents a pool of EventLoopThread objects
 *
 */
class EventLoopThreadPool {
protected:
	EventLoopThreadPool(const EventLoopThreadPool &) = delete;
	EventLoopThreadPool &operator=(const EventLoopThreadPool &) = delete;
	// some uncopyable classes maybe support move constructor....
	EventLoopThreadPool(EventLoopThreadPool &&) noexcept(true) = default;
	EventLoopThreadPool &operator=(EventLoopThreadPool &&) noexcept(true) = default;

public:
	EventLoopThreadPool() = delete;

	/**
	 * @brief Construct a new event loop thread pool instance.
	 *
	 * @param threadNum The number of threads
	 * @param name The name of the EventLoopThreadPool object.
	 */
	EventLoopThreadPool(size_t threadNum,
			const std::string &name = "EventLoopThreadPool");

	/**
	 * @brief Run all event loops in the pool.
	 * @note This function doesn't block the current thread.
	 */
	void start();

	/**
	 * @brief Wait for all event loops in the pool to quit.
	 *
	 * @note This function blocks the current thread.
	 */
	void wait();

	/**
	 * @brief Return the number of the event loop.
	 *
	 * @return size_t
	 */
	size_t size() {
		return loopThreadVector_.size();
	}

	/**
	 * @brief Get the next event loop in the pool.
	 *
	 * @return EventLoop*
	 */
	EventLoop *getNextLoop();

	/**
	 * @brief Get the event loop in the `id` position in the pool.
	 *
	 * @param id The id of the first event loop is zero. If the id >= the number
	 * of event loops, nullptr is returned.
	 * @return EventLoop*
	 */
	EventLoop *getLoop(size_t id);

	/**
	 * @brief Get all event loops in the pool.
	 *
	 * @return std::vector<EventLoop *>
	 */
	std::vector<EventLoop *> getLoops() const;

private:
	std::vector<std::shared_ptr<EventLoopThread> > loopThreadVector_;
	size_t loopIndex_;
};
} // namespace trantor
