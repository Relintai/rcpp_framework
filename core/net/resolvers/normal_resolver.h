// Copyright 2016, Tao An.  All rights reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Tao An

#pragma once
#include "core/containers/concurrent_task_queue.h"
#include "core/net/resolver.h"
#include <memory>
#include <thread>
#include <vector>

namespace trantor {
constexpr size_t kResolveBufferLength{ 16 * 1024 };
class NormalResolver : public Resolver,
					   public std::enable_shared_from_this<NormalResolver> {
protected:
	NormalResolver(const NormalResolver &) = delete;
	NormalResolver &operator=(const NormalResolver &) = delete;
	// some uncopyable classes maybe support move constructor....
	NormalResolver(NormalResolver &&) noexcept(true) = default;
	NormalResolver &operator=(NormalResolver &&) noexcept(true) = default;

public:
	virtual void resolve(const std::string &hostname,
			const Callback &callback) override;
	explicit NormalResolver(size_t timeout) :
			timeout_(timeout), resolveBuffer_(kResolveBufferLength) {
	}
	virtual ~NormalResolver() {
	}

private:
	static std::unordered_map<std::string,
			std::pair<trantor::InetAddress, trantor::Date> > &
	globalCache() {
		static std::unordered_map<
				std::string,
				std::pair<trantor::InetAddress, trantor::Date> >
				dnsCache_;
		return dnsCache_;
	}
	static std::mutex &globalMutex() {
		static std::mutex mutex_;
		return mutex_;
	}
	static trantor::ConcurrentTaskQueue &concurrentTaskQueue() {
		static trantor::ConcurrentTaskQueue queue(
				std::thread::hardware_concurrency() < 8 ? 8 : std::thread::hardware_concurrency(),
				"Dns Queue");
		return queue;
	}
	const size_t timeout_;
	std::vector<char> resolveBuffer_;
};
} // namespace trantor