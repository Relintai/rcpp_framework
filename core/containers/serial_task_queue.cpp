
// This file is originally from Trantor - SerialTaskQueue.cc

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

#include "core/log/logger.h"
#include "serial_task_queue.h"
#ifdef __linux__
#include <sys/prctl.h>
#endif


SerialTaskQueue::SerialTaskQueue(const std::string &name) :
		queueName_(name.empty() ? "SerailTaskQueue" : name),
		loopThread_(queueName_) {
	loopThread_.run();
}
void SerialTaskQueue::stop() {
	stop_ = true;
	loopThread_.getLoop()->quit();
	loopThread_.wait();
}
SerialTaskQueue::~SerialTaskQueue() {
	if (!stop_)
		stop();
	LOG_TRACE << "destruct SerialTaskQueue('" << queueName_ << "')";
}
void SerialTaskQueue::runTaskInQueue(const std::function<void()> &task) {
	loopThread_.getLoop()->runInLoop(task);
}
void SerialTaskQueue::runTaskInQueue(std::function<void()> &&task) {
	loopThread_.getLoop()->runInLoop(std::move(task));
}

void SerialTaskQueue::waitAllTasksFinished() {
	syncTaskInQueue([]() {

	});
}
