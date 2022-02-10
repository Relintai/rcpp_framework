
// This file is originally from Trantor - LockFreeQueue.h

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
#include <assert.h>
#include <atomic>
#include <memory>
#include <type_traits>

namespace trantor {
/**
 * @brief This class template represents a lock-free multiple producers single
 * consumer queue
 *
 * @tparam T The type of the items in the queue.
 */
template <typename T>
class MpscQueue {
protected:
	MpscQueue(const MpscQueue &) = delete;
	MpscQueue &operator=(const MpscQueue &) = delete;
	// some uncopyable classes maybe support move constructor....
	MpscQueue(MpscQueue &&) noexcept(true) = default;
	MpscQueue &operator=(MpscQueue &&) noexcept(true) = default;

public:
	MpscQueue() :
			head_(new BufferNode), tail_(head_.load(std::memory_order_relaxed)) {
	}
	~MpscQueue() {
		T output;
		while (this->dequeue(output)) {
		}
		BufferNode *front = head_.load(std::memory_order_relaxed);
		delete front;
	}

	/**
	 * @brief Put a item into the queue.
	 *
	 * @param input
	 * @note This method can be called in multiple threads.
	 */
	void enqueue(T &&input) {
		BufferNode *node{ new BufferNode(std::move(input)) };
		BufferNode *prevhead{ head_.exchange(node, std::memory_order_acq_rel) };
		prevhead->next_.store(node, std::memory_order_release);
	}
	void enqueue(const T &input) {
		BufferNode *node{ new BufferNode(input) };
		BufferNode *prevhead{ head_.exchange(node, std::memory_order_acq_rel) };
		prevhead->next_.store(node, std::memory_order_release);
	}

	/**
	 * @brief Get a item from the queue.
	 *
	 * @param output
	 * @return false if the queue is empty.
	 * @note This method must be called in a single thread.
	 */
	bool dequeue(T &output) {
		BufferNode *tail = tail_.load(std::memory_order_relaxed);
		BufferNode *next = tail->next_.load(std::memory_order_acquire);

		if (next == nullptr) {
			return false;
		}
		output = std::move(*(next->dataPtr_));
		delete next->dataPtr_;
		tail_.store(next, std::memory_order_release);
		delete tail;
		return true;
	}

	bool empty() {
		BufferNode *tail = tail_.load(std::memory_order_relaxed);
		BufferNode *next = tail->next_.load(std::memory_order_acquire);
		return next == nullptr;
	}

private:
	struct BufferNode {
		BufferNode() = default;
		BufferNode(const T &data) :
				dataPtr_(new T(data)) {
		}
		BufferNode(T &&data) :
				dataPtr_(new T(std::move(data))) {
		}
		T *dataPtr_;
		std::atomic<BufferNode *> next_{ nullptr };
	};

	std::atomic<BufferNode *> head_;
	std::atomic<BufferNode *> tail_;
};

} // namespace trantor
