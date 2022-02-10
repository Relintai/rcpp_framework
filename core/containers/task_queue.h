
// This file is originally from Trantor - TaskQueue.h

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

#include <functional>
#include <future>
#include <string>

namespace trantor {
/**
 * @brief This class is a pure virtual class that can be implemented as a
 * SerialTaskQueue or a ConcurrentTaskQueue.
 *
 */
class TaskQueue {
protected:
	TaskQueue(const TaskQueue &) = delete;
	TaskQueue &operator=(const TaskQueue &) = delete;
	// some uncopyable classes maybe support move constructor....
	TaskQueue(TaskQueue &&) noexcept(true) = default;
	TaskQueue &operator=(TaskQueue &&) noexcept(true) = default;

public:
	TaskQueue() {}
	virtual void runTaskInQueue(const std::function<void()> &task) = 0;
	virtual void runTaskInQueue(std::function<void()> &&task) = 0;
	virtual std::string getName() const {
		return "";
	};

	/**
	 * @brief Run a task in the queue sychronously. This means that the task is
	 * executed before the method returns.
	 *
	 * @param task
	 */
	void syncTaskInQueue(const std::function<void()> &task) {
		std::promise<int> prom;
		std::future<int> fut = prom.get_future();
		runTaskInQueue([&]() {
			task();
			prom.set_value(1);
		});
		fut.get();
	};
	virtual ~TaskQueue() {
	}
};
} // namespace trantor
