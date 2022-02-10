
// This file is originally from Moduo -> Trantor - InetAddress.h

// Copyright (c) 2016-2021, Tao An.  All rights reserved.
// Copyright 2010, Shuo Chen.  All rights reserved.

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

#ifndef MUDUO_NET_INETADDRESS_H
#define MUDUO_NET_INETADDRESS_H

#ifdef _WIN32
#include <ws2tcpip.h>
using sa_family_t = unsigned short;
using in_addr_t = uint32_t;
using uint16_t = unsigned short;
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif
#include <string>
#include <unordered_map>
#include <mutex>

/**
 * @brief Wrapper of sockaddr_in. This is an POD interface class.
 *
 */
class InetAddress
{
  public:
    /**
     * @brief Constructs an endpoint with given port number. Mostly used in
     * TcpServer listening.
     *
     * @param port
     * @param loopbackOnly
     * @param ipv6
     */
    InetAddress(uint16_t port = 0,
                bool loopbackOnly = false,
                bool ipv6 = false);

    /**
     * @brief Constructs an endpoint with given ip and port.
     *
     * @param ip A IPv4 or IPv6 address.
     * @param port
     * @param ipv6
     */
    InetAddress(const std::string &ip, uint16_t port, bool ipv6 = false);

    /**
     * @brief Constructs an endpoint with given struct `sockaddr_in`. Mostly
     * used when accepting new connections
     *
     * @param addr
     */
    explicit InetAddress(const struct sockaddr_in &addr)
        : addr_(addr), isUnspecified_(false)
    {
    }

    /**
     * @brief Constructs an IPv6 endpoint with given struct `sockaddr_in6`.
     * Mostly used when accepting new connections
     *
     * @param addr
     */
    explicit InetAddress(const struct sockaddr_in6 &addr)
        : addr6_(addr), isIpV6_(true), isUnspecified_(false)
    {
    }

    /**
     * @brief Return the sin_family of the endpoint.
     *
     * @return sa_family_t
     */
    sa_family_t family() const
    {
        return addr_.sin_family;
    }

    /**
     * @brief Return the IP string of the endpoint.
     *
     * @return std::string
     */
    std::string toIp() const;

    /**
     * @brief Return the IP and port string of the endpoint.
     *
     * @return std::string
     */
    std::string toIpPort() const;

    /**
     * @brief Return the port number of the endpoint.
     *
     * @return uint16_t
     */
    uint16_t toPort() const;

    /**
     * @brief Check if the endpoint is IPv4 or IPv6.
     *
     * @return true
     * @return false
     */
    bool isIpV6() const
    {
        return isIpV6_;
    }

    /**
     * @brief Return true if the endpoint is an intranet endpoint.
     *
     * @return true
     * @return false
     */
    bool isIntranetIp() const;

    /**
     * @brief Return true if the endpoint is a loopback endpoint.
     *
     * @return true
     * @return false
     */
    bool isLoopbackIp() const;

    /**
     * @brief Get the pointer to the sockaddr struct.
     *
     * @return const struct sockaddr*
     */
    const struct sockaddr *getSockAddr() const
    {
        return static_cast<const struct sockaddr *>((void *)(&addr6_));
    }

    /**
     * @brief Set the sockaddr_in6 struct in the endpoint.
     *
     * @param addr6
     */
    void setSockAddrInet6(const struct sockaddr_in6 &addr6)
    {
        addr6_ = addr6;
        isIpV6_ = (addr6_.sin6_family == AF_INET6);
        isUnspecified_ = false;
    }

    /**
     * @brief Return the integer value of the IP(v4) in net endian byte order.
     *
     * @return uint32_t
     */
    uint32_t ipNetEndian() const;

    /**
     * @brief Return the pointer to the integer value of the IP(v6) in net
     * endian byte order.
     *
     * @return const uint32_t*
     */
    const uint32_t *ip6NetEndian() const;

    /**
     * @brief Return the port number in net endian byte order.
     *
     * @return uint16_t
     */
    uint16_t portNetEndian() const
    {
        return addr_.sin_port;
    }

    /**
     * @brief Set the port number in net endian byte order.
     *
     * @param port
     */
    void setPortNetEndian(uint16_t port)
    {
        addr_.sin_port = port;
    }

    /**
     * @brief Return true if the address is not initalized.
     */
    inline bool isUnspecified() const
    {
        return isUnspecified_;
    }

  private:
    union
    {
        struct sockaddr_in addr_;
        struct sockaddr_in6 addr6_;
    };
    bool isIpV6_{false};
    bool isUnspecified_{true};
};

#endif  // MUDUO_NET_INETADDRESS_H
