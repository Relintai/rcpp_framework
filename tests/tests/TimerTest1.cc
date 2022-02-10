#include "core/loops/event_loop.h"
#include "core/log/logger.h"
#include <iostream>
int main()
{
    EventLoop loop;
    LOG_FATAL << Date::date().roundDay().microSecondsSinceEpoch();
    Date begin = Date::date().roundSecond().after(2);
    auto id = loop.runAt(begin, [begin, &loop]() {
        LOG_DEBUG << "test begin:";
        srand((unsigned int)time(NULL));
        for (int i = 0; i < 10000; ++i)
        {
            int aa = rand() % 10000;
            double s = (double)aa / 1000.0 + 1;
            loop.runAt(begin.after(s),
                       [s]() { LOG_ERROR << "run After:" << s; });
        }
        LOG_DEBUG << "timer created!";
    });
    std::cout << "id=" << id << std::endl;
    loop.loop();
}
