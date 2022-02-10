/**
 *
 *  @file SessionManager.h
 *  An Tao
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  https://github.com/an-tao/drogon
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Drogon
 *
 */

#pragma once

#include "CacheMap.h"
#include "Session.h"
#include "core/loops/event_loop.h"
#include <trantor/utils/NonCopyable.h>
#include <memory>
#include <mutex>
#include <string>

namespace drogon {
class SessionManager : public NonCopyable {
public:
	SessionManager(EventLoop *loop, size_t timeout);
	~SessionManager() {
		sessionMapPtr_.reset();
	}
	SessionPtr getSession(const std::string &sessionID, bool needToSet);
	void changeSessionId(const SessionPtr &sessionPtr);

private:
	std::unique_ptr<CacheMap<std::string, SessionPtr> > sessionMapPtr_;
	EventLoop *loop_;
	size_t timeout_;
};
} // namespace drogon
