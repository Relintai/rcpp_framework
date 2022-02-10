
// This file is originally from Trantor - Channel.h

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

#include "core/log/logger.h"
#include <assert.h>
#include <functional>
#include <memory>


class EventLoop;
/**
 * @brief This class is used to implement reactor pattern. A Channel object
 * manages a socket fd. Users use a Channel object to receive write or read
 * events on the socket it manages.
 *
 */
class Channel {
protected:
	Channel(const Channel &) = delete;
	Channel &operator=(const Channel &) = delete;
	// some uncopyable classes maybe support move constructor....
	Channel(Channel &&) noexcept(true) = default;
	Channel &operator=(Channel &&) noexcept(true) = default;

public:
	using EventCallback = std::function<void()>;
	/**
	 * @brief Construct a new Channel instance.
	 *
	 * @param loop The event loop in which the channel works.
	 * @param fd The socket fd.
	 */
	Channel(EventLoop *loop, int fd);

	/**
	 * @brief Set the read callback.
	 *
	 * @param cb The callback is called when read event occurs on the socket.
	 * @note One should call the enableReading() method to ensure that the
	 * callback would be called when some data is received on the socket.
	 */
	void setReadCallback(const EventCallback &cb) {
		readCallback_ = cb;
	};
	void setReadCallback(EventCallback &&cb) {
		readCallback_ = std::move(cb);
	}

	/**
	 * @brief Set the write callback.
	 *
	 * @param cb The callback is called when write event occurs on the socket.
	 * @note One should call the enableWriting() method to ensure that the
	 * callback would be called when the socket can be written.
	 */
	void setWriteCallback(const EventCallback &cb) {
		writeCallback_ = cb;
	};
	void setWriteCallback(EventCallback &&cb) {
		writeCallback_ = std::move(cb);
	}

	/**
	 * @brief Set the close callback.
	 *
	 * @param cb The callback is called when the socket is closed.
	 */
	void setCloseCallback(const EventCallback &cb) {
		closeCallback_ = cb;
	}
	void setCloseCallback(EventCallback &&cb) {
		closeCallback_ = std::move(cb);
	}

	/**
	 * @brief Set the error callback.
	 *
	 * @param cb The callback is called when an error occurs on the socket.
	 */
	void setErrorCallback(const EventCallback &cb) {
		errorCallback_ = cb;
	}
	void setErrorCallback(EventCallback &&cb) {
		errorCallback_ = std::move(cb);
	}

	/**
	 * @brief Set the event callback.
	 *
	 * @param cb The callback is called when any event occurs on the socket.
	 * @note If the event callback is set to the channel, any other callback
	 * wouldn't be called again.
	 */
	void setEventCallback(const EventCallback &cb) {
		eventCallback_ = cb;
	}
	void setEventCallback(EventCallback &&cb) {
		eventCallback_ = std::move(cb);
	}

	/**
	 * @brief Return the fd of the socket.
	 *
	 * @return int
	 */
	int fd() const {
		return fd_;
	}

	/**
	 * @brief Return the events enabled on the socket.
	 *
	 * @return int
	 */
	int events() const {
		return events_;
	}

	/**
	 * @brief Return the events that occurred on the socket.
	 *
	 * @return int
	 */
	int revents() const {
		return revents_;
	}

	/**
	 * @brief Check whether there is no event enabled on the socket.
	 *
	 * @return true
	 * @return false
	 */
	bool isNoneEvent() const {
		return events_ == kNoneEvent;
	};

	/**
	 * @brief Disable all events on the socket.
	 *
	 */
	void disableAll() {
		events_ = kNoneEvent;
		update();
	}

	/**
	 * @brief Remove the socket from the poller in the event loop.
	 *
	 */
	void remove();

	/**
	 * @brief Return the event loop.
	 *
	 * @return EventLoop*
	 */
	EventLoop *ownerLoop() {
		return loop_;
	};

	/**
	 * @brief Enable the read event on the socket.
	 *
	 */
	void enableReading() {
		events_ |= kReadEvent;
		update();
	}

	/**
	 * @brief Disable the read event on the socket.
	 *
	 */
	void disableReading() {
		events_ &= ~kReadEvent;
		update();
	}

	/**
	 * @brief Enable the write event on the socket.
	 *
	 */
	void enableWriting() {
		events_ |= kWriteEvent;
		update();
	}

	/**
	 * @brief Disable the write event on the socket.
	 *
	 */
	void disableWriting() {
		events_ &= ~kWriteEvent;
		update();
	}

	/**
	 * @brief Check whether the write event is enabled on the socket.
	 *
	 * @return true
	 * @return false
	 */
	bool isWriting() const {
		return events_ & kWriteEvent;
	}

	/**
	 * @brief Check whether the read event is enabled on the socket.
	 *
	 * @return true
	 * @return false
	 */
	bool isReading() const {
		return events_ & kReadEvent;
	}

	/**
	 * @brief Set and update the events enabled.
	 *
	 * @param events
	 */
	void updateEvents(int events) {
		events_ = events;
		update();
	}

	/**
	 * @brief This method is used to ensure that the callback owner is valid
	 * when a callback is called.
	 *
	 * @param obj The callback owner. Usually, the owner is also the owner of
	 * the channel.
	 * @note The 'obj' is kept in a weak_ptr object, so this method does not
	 * cause a circular reference problem.
	 */
	void tie(const std::shared_ptr<void> &obj) {
		tie_ = obj;
		tied_ = true;
	}

	static const int kNoneEvent;
	static const int kReadEvent;
	static const int kWriteEvent;

private:
	friend class EventLoop;
	friend class EpollPoller;
	friend class KQueue;
	void update();
	void handleEvent();
	void handleEventSafely();
	int setRevents(int revt) {
		// LOG_TRACE<<"revents="<<revt;
		revents_ = revt;
		return revt;
	};
	int index() {
		return index_;
	};
	void setIndex(int index) {
		index_ = index;
	};
	EventLoop *loop_;
	const int fd_;
	int events_;
	int revents_;
	int index_;
	bool addedToLoop_{ false };
	EventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback errorCallback_;
	EventCallback closeCallback_;
	EventCallback eventCallback_;
	std::weak_ptr<void> tie_;
	bool tied_;
};
