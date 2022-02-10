
// This file is originally from Trantor - ConcurrentTaskQueue.h

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

#include "core/containers/task_queue.h"
#include <list>
#include <memory>
#include <queue>
#include <string>
#include <vector>

namespace trantor {
/**
 * @brief This class implements a task queue running in parallel. Basically this
 * can be called a threads pool.
 *
 */
class ConcurrentTaskQueue : public TaskQueue {
public:
	ConcurrentTaskQueue() {}

	/**
	 * @brief Construct a new concurrent task queue instance.
	 *
	 * @param threadNum The number of threads in the queue.
	 * @param name The name of the queue.
	 */
	ConcurrentTaskQueue(size_t threadNum, const std::string &name);

	/**
	 * @brief Run a task in the queue.
	 *
	 * @param task
	 */
	virtual void runTaskInQueue(const std::function<void()> &task);
	virtual void runTaskInQueue(std::function<void()> &&task);

	/**
	 * @brief Get the name of the queue.
	 *
	 * @return std::string
	 */
	virtual std::string getName() const {
		return queueName_;
	};

	/**
	 * @brief Get the number of tasks to be executed in the queue.
	 *
	 * @return size_t
	 */
	size_t getTaskCount();

	/**
	 * @brief Stop all threads in the queue.
	 *
	 */
	void stop();

	~ConcurrentTaskQueue();

private:
	size_t queueCount_;
	std::string queueName_;

	std::queue<std::function<void()> > taskQueue_;
	std::vector<std::thread> threads_;

	std::mutex taskMutex_;
	std::condition_variable taskCond_;
	std::atomic_bool stop_;
	void queueFunc(int queueNum);
};

} // namespace trantor
