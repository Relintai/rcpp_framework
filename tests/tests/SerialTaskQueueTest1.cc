#include <trantor/utils/SerialTaskQueue.h>
#include "core/log/logger.h"
#include <iostream>
#include <thread>
#include <stdio.h>

using namespace std::chrono_literals;
int main()
{
    Logger::setLogLevel(Logger::kTrace);
    SerialTaskQueue queue1("test queue1");
    SerialTaskQueue queue2("");
    queue1.runTaskInQueue([&]() {
        for (int i = 0; i < 5; ++i)
        {
            std::this_thread::sleep_for(1s);
            printf("task(%s) i=%d\n", queue1.getName().c_str(), i);
        }
    });
    queue2.runTaskInQueue([&]() {
        for (int i = 0; i < 5; ++i)
        {
            std::this_thread::sleep_for(1s);
            printf("task(%s) i=%d\n", queue2.getName().c_str(), i);
        }
    });
    queue1.waitAllTasksFinished();
    queue2.waitAllTasksFinished();
}
