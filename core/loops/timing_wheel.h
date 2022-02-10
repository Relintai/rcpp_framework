
// This file is originally from Trantor - TimingWheel.h

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

#include "core/loops/event_loop.h"
#include "core/log/logger.h"
#include <map>
#include <mutex>
#include <deque>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <atomic>
#include <assert.h>

#define TIMING_BUCKET_NUM_PER_WHEEL 100
#define TIMING_TICK_INTERVAL 1.0

namespace trantor
{
using EntryPtr = std::shared_ptr<void>;

using EntryBucket = std::unordered_set<EntryPtr>;
using BucketQueue = std::deque<EntryBucket>;

/**
 * @brief This class implements a timer strategy with high performance and low
 * accuracy. This is usually used internally.
 *
 */
class TimingWheel
{
  public:
    class CallbackEntry
    {
      public:
        CallbackEntry(std::function<void()> cb) : cb_(std::move(cb))
        {
        }
        ~CallbackEntry()
        {
            cb_();
        }

      private:
        std::function<void()> cb_;
    };

    /**
     * @brief Construct a new timing wheel instance.
     *
     * @param loop The event loop in which the timing wheel runs.
     * @param maxTimeout The maximum timeout of the timing wheel.
     * @param ticksInterval The internal timer tick interval.  It affects the
     * accuracy of the timing wheel.
     * @param bucketsNumPerWheel The number of buckets per wheel.
     * @note The max delay of the timing wheel is about
     * ticksInterval*(bucketsNumPerWheel^wheelsNum) seconds.
     * @example Four wheels with 200 buckets per wheel means the timing wheel
     * can work with a timeout up to 200^4 seconds, about 50 years;
     */
    TimingWheel(trantor::EventLoop *loop,
                size_t maxTimeout,
                float ticksInterval = TIMING_TICK_INTERVAL,
                size_t bucketsNumPerWheel = TIMING_BUCKET_NUM_PER_WHEEL);

    void insertEntry(size_t delay, EntryPtr entryPtr);

    void insertEntryInloop(size_t delay, EntryPtr entryPtr);

    EventLoop *getLoop()
    {
        return loop_;
    }

    ~TimingWheel();

  private:
    std::vector<BucketQueue> wheels_;

    std::atomic<size_t> ticksCounter_{0};

    trantor::TimerId timerId_;
    trantor::EventLoop *loop_;

    float ticksInterval_;
    size_t wheelsNum_;
    size_t bucketsNumPerWheel_;
};
}  // namespace trantor
