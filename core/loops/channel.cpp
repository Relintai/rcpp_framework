
// This file is originally from Trantor - Channel.cc

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

#include "channel.h"
#include "event_loop.h"

#ifdef _WIN32
#include "Wepoll.h"
#define POLLIN EPOLLIN
#define POLLPRI EPOLLPRI
#define POLLOUT EPOLLOUT
#define POLLHUP EPOLLHUP
#define POLLNVAL 0
#define POLLERR EPOLLERR
#else
#include <poll.h>
#endif
#include <iostream>
namespace trantor {
const int Channel::kNoneEvent = 0;

const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd) :
		loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), tied_(false) {
}

void Channel::remove() {
	assert(events_ == kNoneEvent);
	addedToLoop_ = false;
	loop_->removeChannel(this);
}

void Channel::update() {
	loop_->updateChannel(this);
}

void Channel::handleEvent() {
	// LOG_TRACE<<"revents_="<<revents_;
	if (tied_) {
		std::shared_ptr<void> guard = tie_.lock();
		if (guard) {
			handleEventSafely();
		}
	} else {
		handleEventSafely();
	}
}
void Channel::handleEventSafely() {
	if (eventCallback_) {
		eventCallback_();
		return;
	}
	if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
		// LOG_TRACE<<"handle close";
		if (closeCallback_)
			closeCallback_();
	}
	if (revents_ & (POLLNVAL | POLLERR)) {
		// LOG_TRACE<<"handle error";
		if (errorCallback_)
			errorCallback_();
	}
#ifdef __linux__
	if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
#else
	if (revents_ & (POLLIN | POLLPRI))
#endif
	{
		// LOG_TRACE<<"handle read";
		if (readCallback_)
			readCallback_();
	}
#ifdef _WIN32
	if ((revents_ & POLLOUT) && !(revents_ & POLLHUP))
#else
	if (revents_ & POLLOUT)
#endif
	{
		// LOG_TRACE<<"handle write";
		if (writeCallback_)
			writeCallback_();
	}
}

} // namespace trantor
