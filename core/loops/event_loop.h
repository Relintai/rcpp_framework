
// This file is originally from Moduo -> Trantor - EventLoop.h

// Copyright (c) 2016-2021, Tao An.  All rights reserved.
// Copyright (c) 2010, Shuo Chen.  All rights reserved.

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

#include "core/containers/lock_free_queue.h"
#include "core/math/date.h"
#include <chrono>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class Poller;
class TimerQueue;
class Channel;
using ChannelList = std::vector<Channel *>;
using Func = std::function<void()>;
using TimerId = uint64_t;
enum {
	InvalidTimerId = 0
};

/**
 * @brief As the name implies, this class represents an event loop that runs in
 * a perticular thread. The event loop can handle network I/O events and timers
 * in asynchronous mode.
 * @note An event loop object always belongs to a separate thread, and there is
 * one event loop object at most in a thread. We can call an event loop object
 * the event loop of the thread it belongs to, or call that thread the thread of
 * the event loop.
 */
class EventLoop {
protected:
	EventLoop(const EventLoop &) = delete;
	EventLoop &operator=(const EventLoop &) = delete;
	// some uncopyable classes maybe support move constructor....
	EventLoop(EventLoop &&) noexcept(true) = default;
	EventLoop &operator=(EventLoop &&) noexcept(true) = default;

public:
	EventLoop();
	~EventLoop();

	/**
	 * @brief Run the event loop. This method will be blocked until the event
	 * loop exits.
	 *
	 */
	void loop();

	/**
	 * @brief Let the event loop quit.
	 *
	 */
	void quit();

	/**
	 * @brief Assertion that the current thread is the thread to which the event
	 * loop belongs. If the assertion fails, the program aborts.
	 */
	void assertInLoopThread() {
		if (!isInLoopThread()) {
			abortNotInLoopThread();
		}
	};
#ifdef __linux__
	/**
	 * @brief Make the timer queue works after calling the fork() function.
	 *
	 */
	void resetTimerQueue();
#endif
	/**
	 * @brief Make the event loop works after calling the fork() function.
	 *
	 */
	void resetAfterFork();

	/**
	 * @brief Return true if the current thread is the thread to which the event
	 * loop belongs.
	 *
	 * @return true
	 * @return false
	 */
	bool isInLoopThread() const {
		return threadId_ == std::this_thread::get_id();
	};

	/**
	 * @brief Get the event loop of the current thread. Return nullptr if there
	 * is no event loop in the current thread.
	 *
	 * @return EventLoop*
	 */
	static EventLoop *getEventLoopOfCurrentThread();

	/**
	 * @brief Run the function f in the thread of the event loop.
	 *
	 * @param f
	 * @note If the current thread is the thread of the event loop, the function
	 * f is executed directly before the method exiting.
	 */
	void runInLoop(const Func &f);
	void runInLoop(Func &&f);

	/**
	 * @brief Run the function f in the thread of the event loop.
	 *
	 * @param f
	 * @note The difference between this method and the runInLoop() method is
	 * that the function f is executed after the method exiting no matter if the
	 * current thread is the thread of the event loop.
	 */
	void queueInLoop(const Func &f);
	void queueInLoop(Func &&f);

	/**
	 * @brief Run a function at a time point.
	 *
	 * @param time The time to run the function.
	 * @param cb The function to run.
	 * @return TimerId The ID of the timer.
	 */
	TimerId runAt(const Date &time, const Func &cb);
	TimerId runAt(const Date &time, Func &&cb);

	/**
	 * @brief Run a function after a period of time.
	 *
	 * @param delay Represent the period of time in seconds.
	 * @param cb The function to run.
	 * @return TimerId The ID of the timer.
	 */
	TimerId runAfter(double delay, const Func &cb);
	TimerId runAfter(double delay, Func &&cb);

	/**
	 * @brief Run a function after a period of time.
	 * @note Users could use chrono literals to represent a time duration
	 * For example:
	 * @code
	   runAfter(5s, task);
	   runAfter(10min, task);
	   @endcode
	 */
	TimerId runAfter(const std::chrono::duration<double> &delay, const Func &cb) {
		return runAfter(delay.count(), cb);
	}
	TimerId runAfter(const std::chrono::duration<double> &delay, Func &&cb) {
		return runAfter(delay.count(), std::move(cb));
	}

	/**
	 * @brief Repeatedly run a function every period of time.
	 *
	 * @param interval The duration in seconds.
	 * @param cb The function to run.
	 * @return TimerId The ID of the timer.
	 */
	TimerId runEvery(double interval, const Func &cb);
	TimerId runEvery(double interval, Func &&cb);

	/**
	 * @brief Repeatedly run a function every period of time.
	 * Users could use chrono literals to represent a time duration
	 * For example:
	 * @code
	   runEvery(5s, task);
	   runEvery(10min, task);
	   runEvery(0.1h, task);
	   @endcode
	 */
	TimerId runEvery(const std::chrono::duration<double> &interval,
			const Func &cb) {
		return runEvery(interval.count(), cb);
	}
	TimerId runEvery(const std::chrono::duration<double> &interval, Func &&cb) {
		return runEvery(interval.count(), std::move(cb));
	}

	/**
	 * @brief Invalidate the timer identified by the given ID.
	 *
	 * @param id The ID of the timer.
	 */
	void invalidateTimer(TimerId id);

	/**
	 * @brief Move the EventLoop to the current thread, this method must be
	 * called before the loop is running.
	 *
	 */
	void moveToCurrentThread();

	/**
	 * @brief Update channel status. This method is usually used internally.
	 *
	 * @param chl
	 */
	void updateChannel(Channel *chl);

	/**
	 * @brief Remove a channel from the event loop. This method is usually used
	 * internally.
	 *
	 * @param chl
	 */
	void removeChannel(Channel *chl);

	/**
	 * @brief Return the index of the event loop.
	 *
	 * @return size_t
	 */
	size_t index() {
		return index_;
	}

	/**
	 * @brief Set the index of the event loop.
	 *
	 * @param index
	 */
	void setIndex(size_t index) {
		index_ = index;
	}

	/**
	 * @brief Return true if the event loop is running.
	 *
	 * @return true
	 * @return false
	 */
	bool isRunning() {
		return looping_ && (!quit_);
	}

	/**
	 * @brief Check if the event loop is calling a function.
	 *
	 * @return true
	 * @return false
	 */
	bool isCallingFunctions() {
		return callingFuncs_;
	}

	/**
	 * @brief Run functions when the event loop quits
	 *
	 * @param cb the function to run
	 * @note the function runs on the thread that quits the EventLoop
	 */
	void runOnQuit(Func &&cb);
	void runOnQuit(const Func &cb);

private:
	void abortNotInLoopThread();
	void wakeup();
	void wakeupRead();
	bool looping_;
	std::thread::id threadId_;
	bool quit_;
	std::unique_ptr<Poller> poller_;

	ChannelList activeChannels_;
	Channel *currentActiveChannel_;

	bool eventHandling_;
	MpscQueue<Func> funcs_;
	std::unique_ptr<TimerQueue> timerQueue_;
	MpscQueue<Func> funcsOnQuit_;
	bool callingFuncs_{ false };
#ifdef __linux__
	int wakeupFd_;
	std::unique_ptr<Channel> wakeupChannelPtr_;
#elif defined _WIN32
#else
	int wakeupFd_[2];
	std::unique_ptr<Channel> wakeupChannelPtr_;
#endif

	void doRunInLoopFuncs();
#ifdef _WIN32
	size_t index_{ size_t(-1) };
#else
	size_t index_{ std::numeric_limits<size_t>::max() };
#endif
	EventLoop **threadLocalLoopPtr_;
};
