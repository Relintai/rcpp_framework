
// This file is originally from Trantor - SerialTaskQueue.h

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

#include "task_queue.h"
#include "core/loops/event_loop_thread.h"
#include <string>
#include <queue>
#include <mutex>
#include <atomic>


/**
 * @brief This class represents a task queue in which all tasks are executed one
 * by one.
 *
 */
class SerialTaskQueue : public TaskQueue
{
  public:
    /**
     * @brief Run a task in the queue.
     *
     * @param task
     */
    virtual void runTaskInQueue(const std::function<void()> &task);
    virtual void runTaskInQueue(std::function<void()> &&task);

    /**
     * @brief Get the name of the queue.
     *
     * @return std::string
     */
    virtual std::string getName() const
    {
        return queueName_;
    };

    /**
     * @brief Wait until all tasks in the queue are finished.
     *
     */
    void waitAllTasksFinished();

    SerialTaskQueue() = delete;

    /**
     * @brief Construct a new serail task queue instance.
     *
     * @param name
     */
    explicit SerialTaskQueue(const std::string &name);

    virtual ~SerialTaskQueue();

    /**
     * @brief Check whether a task is running in the queue.
     *
     * @return true
     * @return false
     */
    bool isRuningTask()
    {
        return loopThread_.getLoop()
                   ? loopThread_.getLoop()->isCallingFunctions()
                   : false;
    }

    /**
     * @brief Get the number of tasks in the queue.
     *
     * @return size_t
     */
    size_t getTaskCount();

    /**
     * @brief Stop the queue.
     *
     */
    void stop();

  protected:
    std::string queueName_;
    EventLoopThread loopThread_;
    bool stop_{false};
};
