
// This file is originally from Trantor - NormalResolver.h

// Copyright (c) 2016-2021, Tao An.  All rights reserved.

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

#pragma once
#include "core/containers/concurrent_task_queue.h"
#include "core/net/resolver.h"
#include <memory>
#include <thread>
#include <vector>

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
			std::pair<InetAddress, Date> > &
	globalCache() {
		static std::unordered_map<
				std::string,
				std::pair<InetAddress, Date> >
				dnsCache_;
		return dnsCache_;
	}
	static std::mutex &globalMutex() {
		static std::mutex mutex_;
		return mutex_;
	}
	static ConcurrentTaskQueue &concurrentTaskQueue() {
		static ConcurrentTaskQueue queue(
				std::thread::hardware_concurrency() < 8 ? 8 : std::thread::hardware_concurrency(),
				"Dns Queue");
		return queue;
	}
	const size_t timeout_;
	std::vector<char> resolveBuffer_;
};
