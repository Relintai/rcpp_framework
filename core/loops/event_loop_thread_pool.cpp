
// This file is originally from Trantor - EventLoopThreadPool.cc

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

#include "event_loop_thread_pool.h"

EventLoopThreadPool::EventLoopThreadPool(size_t threadNum,
		const std::string &name) :
		loopIndex_(0) {
	for (size_t i = 0; i < threadNum; ++i) {
		loopThreadVector_.emplace_back(std::make_shared<EventLoopThread>(name));
	}
}
void EventLoopThreadPool::start() {
	for (unsigned int i = 0; i < loopThreadVector_.size(); ++i) {
		loopThreadVector_[i]->run();
	}
}
// void EventLoopThreadPool::stop(){
//    for(unsigned int i=0;i<loopThreadVector_.size();i++)
//    {
//        loopThreadVector_[i].stop();
//    }
//}
void EventLoopThreadPool::wait() {
	for (unsigned int i = 0; i < loopThreadVector_.size(); ++i) {
		loopThreadVector_[i]->wait();
	}
}
EventLoop *EventLoopThreadPool::getNextLoop() {
	if (loopThreadVector_.size() > 0) {
		EventLoop *loop = loopThreadVector_[loopIndex_]->getLoop();
		++loopIndex_;
		if (loopIndex_ >= loopThreadVector_.size())
			loopIndex_ = 0;
		return loop;
	}
	return nullptr;
}
EventLoop *EventLoopThreadPool::getLoop(size_t id) {
	if (id < loopThreadVector_.size())
		return loopThreadVector_[id]->getLoop();
	return nullptr;
}
std::vector<EventLoop *> EventLoopThreadPool::getLoops() const {
	std::vector<EventLoop *> ret;
	for (auto &loopThread : loopThreadVector_) {
		ret.push_back(loopThread->getLoop());
	}
	return ret;
}