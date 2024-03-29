#include "core/containers/concurrent_task_queue.h"
#include "core/log/logger.h"
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <time.h>
#include <stdio.h>
using namespace std::chrono_literals;

int main()
{
    ConcurrentTaskQueue queue(5, "concurrT");
    std::atomic_int sum;
    sum = 0;
    for (int i = 0; i < 4; ++i)
    {
        queue.runTaskInQueue([&sum]() {
            LOG_DEBUG << "add sum";
            for (int i = 0; i < 10000; ++i)
            {
                ++sum;
            }
        });
    }

    queue.runTaskInQueue([&sum]() {
        for (int i = 0; i < 20; ++i)
        {
            LOG_DEBUG << "sum=" << sum;
            std::this_thread::sleep_for(100us);
        }
    });

    getc(stdin);
    LOG_DEBUG << "sum=" << sum;
}
