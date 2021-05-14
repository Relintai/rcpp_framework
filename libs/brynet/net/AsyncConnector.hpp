#pragma once

#include <brynet/net/detail/ConnectorDetail.hpp>

using ConnectOption = ConnectOption;

class AsyncConnector : public AsyncConnectorDetail,
					   public std::enable_shared_from_this<AsyncConnector> {
public:
	using Ptr = std::shared_ptr<AsyncConnector>;

	void startWorkerThread() {
		AsyncConnectorDetail::startWorkerThread();
	}

	void stopWorkerThread() {
		AsyncConnectorDetail::stopWorkerThread();
	}

	void asyncConnect(const ConnectOption &option) {
		AsyncConnectorDetail::asyncConnect(option);
	}

	static Ptr Create() {
		class make_shared_enabler : public AsyncConnector {
		};
		return std::make_shared<make_shared_enabler>();
	}

private:
	AsyncConnector() = default;
};
