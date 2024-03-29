#include "core/log/logger.h"
#include "core/log/async_file_logger.h"
#include <thread>
#include <chrono>
#include <stdlib.h>

using namespace std::chrono_literals;
int main()
{
    AsyncFileLogger asyncFileLogger;
    asyncFileLogger.setFileName("async_test");
    asyncFileLogger.startLogging();
    Logger::setOutputFunction(
        [&](const char *msg, const uint64_t len) {
            asyncFileLogger.output(msg, len);
        },
        [&]() { asyncFileLogger.flush(); });
    asyncFileLogger.setFileSizeLimit(100000000);
    int i = 0;
    while (i < 1000000)
    {
        ++i;
        if (i % 100 == 0)
        {
            LOG_ERROR << "this is the " << i << "th log";
            continue;
        }
        LOG_INFO << "this is the " << i << "th log";
        ++i;
        LOG_DEBUG << "this is the " << i << "th log";
        std::this_thread::sleep_for(1s);
    }
}
