
// This file is originally from Trantor - ConcurrentTaskQueue.cc

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

#include <assert.h>
#include "concurrent_task_queue.h"
#include "core/log/logger.h"
#ifdef __linux__
#include <sys/prctl.h>
#endif

using namespace trantor;
ConcurrentTaskQueue::ConcurrentTaskQueue(size_t threadNum,
		const std::string &name) :
		queueCount_(threadNum), queueName_(name), stop_(false) {
	assert(threadNum > 0);
	for (unsigned int i = 0; i < queueCount_; ++i) {
		threads_.push_back(
				std::thread(std::bind(&ConcurrentTaskQueue::queueFunc, this, i)));
	}
}
void ConcurrentTaskQueue::runTaskInQueue(const std::function<void()> &task) {
	LOG_TRACE << "copy task into queue";
	std::lock_guard<std::mutex> lock(taskMutex_);
	taskQueue_.push(task);
	taskCond_.notify_one();
}
void ConcurrentTaskQueue::runTaskInQueue(std::function<void()> &&task) {
	LOG_TRACE << "move task into queue";
	std::lock_guard<std::mutex> lock(taskMutex_);
	taskQueue_.push(std::move(task));
	taskCond_.notify_one();
}
void ConcurrentTaskQueue::queueFunc(int queueNum) {
	char tmpName[32];
	snprintf(tmpName, sizeof(tmpName), "%s%d", queueName_.c_str(), queueNum);
#ifdef __linux__
	::prctl(PR_SET_NAME, tmpName);
#endif
	while (!stop_) {
		std::function<void()> r;
		{
			std::unique_lock<std::mutex> lock(taskMutex_);
			while (!stop_ && taskQueue_.size() == 0) {
				taskCond_.wait(lock);
			}
			if (taskQueue_.size() > 0) {
				LOG_TRACE << "got a new task!";
				r = std::move(taskQueue_.front());
				taskQueue_.pop();
			} else
				continue;
		}
		r();
	}
}

size_t ConcurrentTaskQueue::getTaskCount() {
	std::lock_guard<std::mutex> guard(taskMutex_);
	return taskQueue_.size();
}

void ConcurrentTaskQueue::stop() {
	if (!stop_) {
		stop_ = true;
		taskCond_.notify_all();
		for (auto &t : threads_)
			t.join();
	}
}
ConcurrentTaskQueue::~ConcurrentTaskQueue() {
	stop();
}
