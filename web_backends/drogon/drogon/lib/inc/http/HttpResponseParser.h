/**
 *
 *  @file HttpResponseParser.h
 *  @author An Tao
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

#include "../src/impl_forwards.h"
#include "core/net/tcp_connection.h"
#include "core/containers/msg_buffer.h"
#include <trantor/utils/NonCopyable.h>
#include <list>
#include <mutex>

namespace drogon {
class HttpResponseParser : public NonCopyable {
public:
	enum class HttpResponseParseStatus {
		kExpectResponseLine,
		kExpectHeaders,
		kExpectBody,
		kExpectChunkLen,
		kExpectChunkBody,
		kExpectLastEmptyChunk,
		kExpectClose,
		kGotAll,
	};

	explicit HttpResponseParser(const TcpConnectionPtr &connPtr);

	// default copy-ctor, dtor and assignment are fine

	// return false if any error
	bool parseResponse(MsgBuffer *buf);
	bool parseResponseOnClose();

	bool gotAll() const {
		return status_ == HttpResponseParseStatus::kGotAll;
	}

	void setForHeadMethod() {
		parseResponseForHeadMethod_ = true;
	}

	void reset();

	const HttpResponseImplPtr &responseImpl() const {
		return responsePtr_;
	}

private:
	bool processResponseLine(const char *begin, const char *end);

	HttpResponseParseStatus status_;
	HttpResponseImplPtr responsePtr_;
	bool parseResponseForHeadMethod_{ false };
	size_t leftBodyLength_{ 0 };
	size_t currentChunkLength_{ 0 };
	std::weak_ptr<TcpConnection> conn_;
};

} // namespace drogon
