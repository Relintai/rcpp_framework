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
    connCount = 1;
    for (int i = 0; i < connCount; ++i)
    {
        client[i] = std::make_shared<TcpClient>(&loop,
                                                         serverAddr,
                                                         "tcpclienttest");
        client[i]->enableSSL(false, false);
        client[i]->setConnectionCallback(
            [i, &loop, &connCount](const TcpConnectionPtr &conn) {
                if (conn->connected())
                {
                    LOG_DEBUG << i << " connected!";
                    char tmp[20];
                    sprintf(tmp, "%d client!!", i);
                    conn->send(tmp);
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
                LOG_DEBUG << std::string(buf->peek(), buf->readableBytes());
                buf->retrieveAll();
                conn->shutdown();
            });
        client[i]->connect();
    }
    loop.loop();
}
