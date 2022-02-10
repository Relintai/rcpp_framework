
// This file is originally from Trantor - EventLoopThread.cc

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

#include "event_loop_thread.h"
#include "core/log/logger.h"
#ifdef __linux__
#include <sys/prctl.h>
#endif

using namespace trantor;
EventLoopThread::EventLoopThread(const std::string &threadName) :
		loop_(nullptr),
		loopThreadName_(threadName),
		thread_([this]() { loopFuncs(); }) {
	auto f = promiseForLoopPointer_.get_future();
	loop_ = f.get();
}
EventLoopThread::~EventLoopThread() {
	run();
	if (loop_) {
		loop_->quit();
	}
	if (thread_.joinable()) {
		thread_.join();
	}
}
// void EventLoopThread::stop() {
//    if(loop_)
//        loop_->quit();
//}
void EventLoopThread::wait() {
	thread_.join();
}
void EventLoopThread::loopFuncs() {
#ifdef __linux__
	::prctl(PR_SET_NAME, loopThreadName_.c_str());
#endif
	EventLoop loop;
	loop.queueInLoop([this]() { promiseForLoop_.set_value(1); });
	promiseForLoopPointer_.set_value(&loop);
	auto f = promiseForRun_.get_future();
	(void)f.get();
	loop.loop();
	// LOG_DEBUG << "loop out";
	loop_ = NULL;
}
void EventLoopThread::run() {
	std::call_once(once_, [this]() {
		auto f = promiseForLoop_.get_future();
		promiseForRun_.set_value(1);
		// Make sure the event loop loops before returning.
		(void)f.get();
	});
}
