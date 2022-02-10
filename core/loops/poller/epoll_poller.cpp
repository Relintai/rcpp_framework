
// This file is originally from Trantor - EpollPoller.cc

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

#include "epoll_poller.h"
#include "core/log/logger.h"
#include "core/loops/channel.h"

#ifdef __linux__
#include <assert.h>
#include <poll.h>
#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#elif defined _WIN32
#include "Wepoll.h"
#include <assert.h>
#include <fcntl.h>
#include <winsock2.h>
#include <iostream>
#define EPOLL_CLOEXEC _O_NOINHERIT
#endif

#if defined __linux__ || defined _WIN32

#if defined __linux__
static_assert(EPOLLIN == POLLIN, "EPOLLIN != POLLIN");
static_assert(EPOLLPRI == POLLPRI, "EPOLLPRI != POLLPRI");
static_assert(EPOLLOUT == POLLOUT, "EPOLLOUT != POLLOUT");
static_assert(EPOLLRDHUP == POLLRDHUP, "EPOLLRDHUP != POLLRDHUP");
static_assert(EPOLLERR == POLLERR, "EPOLLERR != POLLERR");
static_assert(EPOLLHUP == POLLHUP, "EPOLLHUP != POLLHUP");
#endif

namespace {
const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;
} // namespace

EpollPoller::EpollPoller(EventLoop *loop) :
		Poller(loop),
#ifdef _WIN32
		// wepoll does not suppor flags
		epollfd_(::epoll_create1(0)),
#else
		epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
#endif
		events_(kInitEventListSize) {
}
EpollPoller::~EpollPoller() {
#ifdef _WIN32
	epoll_close(epollfd_);
#else
	close(epollfd_);
#endif
}
#ifdef _WIN32
void EpollPoller::postEvent(uint64_t event) {
	epoll_post_signal(epollfd_, event);
}
#endif
void EpollPoller::poll(int timeoutMs, ChannelList *activeChannels) {
	int numEvents = ::epoll_wait(epollfd_,
			&*events_.begin(),
			static_cast<int>(events_.size()),
			timeoutMs);
	int savedErrno = errno;
	// Timestamp now(Timestamp::now());
	if (numEvents > 0) {
		// LOG_TRACE << numEvents << " events happended";
		fillActiveChannels(numEvents, activeChannels);
		if (static_cast<size_t>(numEvents) == events_.size()) {
			events_.resize(events_.size() * 2);
		}
	} else if (numEvents == 0) {
		// std::cout << "nothing happended" << std::endl;
	} else {
		// error happens, log uncommon ones
		if (savedErrno != EINTR) {
			errno = savedErrno;
			LOG_SYSERR << "EPollEpollPoller::poll()";
		}
	}
	return;
}
void EpollPoller::fillActiveChannels(int numEvents,
		ChannelList *activeChannels) const {
	assert(static_cast<size_t>(numEvents) <= events_.size());
	for (int i = 0; i < numEvents; ++i) {
#ifdef _WIN32
		if (events_[i].events == EPOLLEVENT) {
			eventCallback_(events_[i].data.u64);
			continue;
		}
#endif
		Channel *channel = static_cast<Channel *>(events_[i].data.ptr);
#ifndef NDEBUG
		int fd = channel->fd();
		ChannelMap::const_iterator it = channels_.find(fd);
		assert(it != channels_.end());
		assert(it->second == channel);
#endif
		channel->setRevents(events_[i].events);
		activeChannels->push_back(channel);
	}
	// LOG_TRACE<<"active Channels num:"<<activeChannels->size();
}
void EpollPoller::updateChannel(Channel *channel) {
	assertInLoopThread();
	const int index = channel->index();
	// LOG_TRACE << "fd = " << channel->fd()
	//  << " events = " << channel->events() << " index = " << index;
	if (index == kNew || index == kDeleted) {
// a new one, add with EPOLL_CTL_ADD
#ifndef NDEBUG
		int fd = channel->fd();
		if (index == kNew) {
			assert(channels_.find(fd) == channels_.end());
			channels_[fd] = channel;
		} else { // index == kDeleted
			assert(channels_.find(fd) != channels_.end());
			assert(channels_[fd] == channel);
		}
#endif
		channel->setIndex(kAdded);
		update(EPOLL_CTL_ADD, channel);
	} else {
// update existing one with EPOLL_CTL_MOD/DEL
#ifndef NDEBUG
		int fd = channel->fd();
		(void)fd;
		assert(channels_.find(fd) != channels_.end());
		assert(channels_[fd] == channel);
#endif
		assert(index == kAdded);
		if (channel->isNoneEvent()) {
			update(EPOLL_CTL_DEL, channel);
			channel->setIndex(kDeleted);
		} else {
			update(EPOLL_CTL_MOD, channel);
		}
	}
}
void EpollPoller::removeChannel(Channel *channel) {
	EpollPoller::assertInLoopThread();
#ifndef NDEBUG
	int fd = channel->fd();
	assert(channels_.find(fd) != channels_.end());
	assert(channels_[fd] == channel);
	size_t n = channels_.erase(fd);
	(void)n;
	assert(n == 1);
#endif
	assert(channel->isNoneEvent());
	int index = channel->index();
	assert(index == kAdded || index == kDeleted);
	if (index == kAdded) {
		update(EPOLL_CTL_DEL, channel);
	}
	channel->setIndex(kNew);
}
void EpollPoller::update(int operation, Channel *channel) {
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = channel->events();
	event.data.ptr = channel;
	int fd = channel->fd();
	if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
		if (operation == EPOLL_CTL_DEL) {
			// LOG_SYSERR << "epoll_ctl op =" << operationToString(operation) <<
			// " fd =" << fd;
		} else {
			//  LOG_SYSFATAL << "epoll_ctl op =" << operationToString(operation)
			//  << " fd =" << fd;
		}
	}
}
#else
EpollPoller::EpollPoller(EventLoop *loop) :
		Poller(loop) {
	assert(false);
}
EpollPoller::~EpollPoller() {
}
void EpollPoller::poll(int, ChannelList *) {
}
void EpollPoller::updateChannel(Channel *) {
}
void EpollPoller::removeChannel(Channel *) {
}

#endif
