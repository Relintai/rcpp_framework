#pragma once

#include <brynet/net/detail/TCPServiceDetail.hpp>

using ConnectionOption = ConnectionOption;
class TcpService : public TcpServiceDetail,
				   public std::enable_shared_from_this<TcpService> {
public:
	using Ptr = std::shared_ptr<TcpService>;
	using FrameCallback = TcpServiceDetail::FrameCallback;

public:
	static Ptr Create() {
		struct make_shared_enabler : public TcpService {
		};
		return std::make_shared<make_shared_enabler>();
	}

	void startWorkerThread(size_t threadNum,
			FrameCallback callback = nullptr) {
		TcpServiceDetail::startWorkerThread(threadNum, callback);
	}

	void stopWorkerThread() {
		TcpServiceDetail::stopWorkerThread();
	}

	bool addTcpConnection(TcpSocket::Ptr socket, ConnectionOption options) {
		return TcpServiceDetail::addTcpConnection(std::move(socket), options);
	}

	EventLoop::Ptr getRandomEventLoop() {
		return TcpServiceDetail::getRandomEventLoop();
	}

private:
	TcpService() = default;
};
