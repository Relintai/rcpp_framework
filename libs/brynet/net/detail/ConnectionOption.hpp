#pragma once

#include <brynet/net/SSLHelper.hpp>
#include <brynet/net/TcpConnection.hpp>

class ConnectionOption final {
public:
	std::vector<TcpConnection::EnterCallback> enterCallback;
	SSLHelper::Ptr sslHelper;
	bool useSSL = false;
	bool forceSameThreadLoop = false;
	size_t maxRecvBufferSize = 128;
};
