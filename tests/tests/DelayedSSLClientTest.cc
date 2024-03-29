#include "core/net/tcp_client.h"
#include "core/log/logger.h"
#include "core/loops/event_loop_thread.h"
#include <string>
#include <iostream>
#include <atomic>

#define USE_IPV6 0
int main()
{
    Logger::setLogLevel(Logger::kTrace);
    LOG_DEBUG << "TcpClient class test!";
    EventLoop loop;
#if USE_IPV6
    InetAddress serverAddr("::1", 8888, true);
#else
    InetAddress serverAddr("127.0.0.1", 8888);
#endif
    std::shared_ptr<TcpClient> client[10];
    std::atomic_int connCount;
    connCount = 10;
    for (int i = 0; i < 10; ++i)
    {
        client[i] = std::make_shared<TcpClient>(&loop,
                                                         serverAddr,
                                                         "tcpclienttest");
        client[i]->setConnectionCallback(
            [i, &loop, &connCount](const TcpConnectionPtr &conn) {
                if (conn->connected())
                {
                }
                else
                {
                    LOG_DEBUG << i << " disconnected";
                    --connCount;
                    if (connCount == 0)
                        loop.quit();
                }
            });
        client[i]->setMessageCallback(
            [](const TcpConnectionPtr &conn, MsgBuffer *buf) {
                auto msg = std::string(buf->peek(), buf->readableBytes());

                LOG_INFO << msg;
                if (msg == "hello")
                {
                    buf->retrieveAll();
                    conn->startClientEncryption(
                        [conn]() {
                            LOG_INFO << "SSL established";
                            conn->send("Hello");
                        },
                        false,
                        false);
                }
                if (conn->isSSLConnection())
                {
                    buf->retrieveAll();
                }
            });
        client[i]->connect();
    }
    loop.loop();
}
