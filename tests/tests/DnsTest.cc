#include "core/net/resolver.h"
#include <iostream>
void dns(const std::shared_ptr<Resolver> &resolver)
{
    auto now = Date::now();
    resolver->resolve("www.baidu.com", [now](const InetAddress &addr) {
        auto interval = Date::now().microSecondsSinceEpoch() -
                        now.microSecondsSinceEpoch();
        std::cout << "baidu:" << addr.toIp() << " " << interval / 1000 << "ms"
                  << std::endl;
    });
    resolver->resolve("www.google.com",
                      [now](const InetAddress &addr) {
                          auto interval =
                              Date::now().microSecondsSinceEpoch() -
                              now.microSecondsSinceEpoch();
                          std::cout << "google:" << addr.toIp() << " "
                                    << interval / 1000 << "ms" << std::endl;
                      });
    resolver->resolve("www.sina.com", [now](const InetAddress &addr) {
        auto interval = Date::now().microSecondsSinceEpoch() -
                        now.microSecondsSinceEpoch();
        std::cout << "sina:" << addr.toIp() << " " << interval / 1000 << "ms"
                  << std::endl;
    });
    resolver->resolve("www.xjfisfjaskfeiakdjfg.com",
                      [now](const InetAddress &addr) {
                          auto interval =
                              Date::now().microSecondsSinceEpoch() -
                              now.microSecondsSinceEpoch();
                          std::cout << "bad address:" << addr.toIp() << " "
                                    << interval / 1000 << "ms" << std::endl;
                      });
    resolver->resolve("localhost", [now](const InetAddress &addr) {
        auto interval = Date::now().microSecondsSinceEpoch() -
                        now.microSecondsSinceEpoch();
        std::cout << "localhost:" << addr.toIp() << " " << interval / 1000
                  << "ms" << std::endl;
    });
}
int main()
{
    EventLoop loop;
    auto resolver = Resolver::newResolver(&loop);
    dns(resolver);
    loop.runAfter(1.0, [resolver]() { dns(resolver); });
    loop.loop();
}
