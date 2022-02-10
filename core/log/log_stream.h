
// This file is originally from Moduo -> Trantor - LogStream.h

// Copyright (c) 2016-2021, Tao An.  All rights reserved.
// Copyright (c) 2010, Shuo Chen.  All rights reserved.

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

// Taken from muduo lib and modified. Classes in this file are used internally.
#include <assert.h>
#include <string.h> // memcpy
#include <string>

namespace trantor {
namespace detail {
static constexpr size_t kSmallBuffer{ 4000 };
static constexpr size_t kLargeBuffer{ 4000 * 1000 };

template <int SIZE>
class FixedBuffer {
protected:
	FixedBuffer(const FixedBuffer &) = delete;
	FixedBuffer &operator=(const FixedBuffer &) = delete;
	// some uncopyable classes maybe support move constructor....
	FixedBuffer(FixedBuffer &&) noexcept(true) = default;
	FixedBuffer &operator=(FixedBuffer &&) noexcept(true) = default;

public:
	FixedBuffer() :
			cur_(data_) {
		setCookie(cookieStart);
	}

	~FixedBuffer() {
		setCookie(cookieEnd);
	}

	bool append(const char * /*restrict*/ buf, size_t len) {
		if ((size_t)(avail()) > len) {
			memcpy(cur_, buf, len);
			cur_ += len;
			return true;
		}
		return false;
	}

	const char *data() const {
		return data_;
	}
	int length() const {
		return static_cast<int>(cur_ - data_);
	}

	// write to data_ directly
	char *current() {
		return cur_;
	}
	int avail() const {
		return static_cast<int>(end() - cur_);
	}
	void add(size_t len) {
		cur_ += len;
	}

	void reset() {
		cur_ = data_;
	}
	void zeroBuffer() {
		memset(data_, 0, sizeof(data_));
	}

	// for used by GDB
	const char *debugString();
	void setCookie(void (*cookie)()) {
		cookie_ = cookie;
	}
	// for used by unit test
	std::string toString() const {
		return std::string(data_, length());
	}
	// StringPiece toStringPiece() const { return StringPiece(data_, length());
	// }

private:
	const char *end() const {
		return data_ + sizeof data_;
	}
	// Must be outline function for cookies.
	static void cookieStart();
	static void cookieEnd();

	void (*cookie_)();
	char data_[SIZE];
	char *cur_;
};

} // namespace detail

class LogStream {
	using self = LogStream;

protected:
	LogStream(const LogStream &) = delete;
	LogStream &operator=(const LogStream &) = delete;
	// some uncopyable classes maybe support move constructor....
	LogStream(LogStream &&) noexcept(true) = default;
	LogStream &operator=(LogStream &&) noexcept(true) = default;

public:
	using Buffer = detail::FixedBuffer<detail::kSmallBuffer>;

	self &operator<<(bool v) {
		append(v ? "1" : "0", 1);
		return *this;
	}

	self &operator<<(short);
	self &operator<<(unsigned short);
	self &operator<<(int);
	self &operator<<(unsigned int);
	self &operator<<(long);
	self &operator<<(unsigned long);
	self &operator<<(const long long &);
	self &operator<<(const unsigned long long &);

	self &operator<<(const void *);

	self &operator<<(float &v) {
		*this << static_cast<double>(v);
		return *this;
	}
	self &operator<<(const double &);
	self &operator<<(const long double &v);

	self &operator<<(char v) {
		append(&v, 1);
		return *this;
	}

	// self& operator<<(signed char);
	// self& operator<<(unsigned char);
	template <int N>
	self &operator<<(const char (&buf)[N]) {
		assert(strnlen(buf, N) == N - 1);
		append(buf, N - 1);
		return *this;
	}

	self &operator<<(char *str) {
		if (str) {
			append(str, strlen(str));
		} else {
			append("(null)", 6);
		}
		return *this;
	}

	self &operator<<(const char *str) {
		if (str) {
			append(str, strlen(str));
		} else {
			append("(null)", 6);
		}
		return *this;
	}

	self &operator<<(const unsigned char *str) {
		return operator<<(reinterpret_cast<const char *>(str));
	}

	self &operator<<(const std::string &v) {
		append(v.c_str(), v.size());
		return *this;
	}

	void append(const char *data, size_t len) {
		if (exBuffer_.empty()) {
			if (!buffer_.append(data, len)) {
				exBuffer_.append(buffer_.data(), buffer_.length());
				exBuffer_.append(data, len);
			}
		} else {
			exBuffer_.append(data, len);
		}
	}
	// const Buffer& buffer() const { return buffer_; }
	const char *bufferData() const {
		if (!exBuffer_.empty()) {
			return exBuffer_.data();
		}
		return buffer_.data();
	}

	size_t bufferLength() const {
		if (!exBuffer_.empty()) {
			return exBuffer_.length();
		}
		return buffer_.length();
	}
	void resetBuffer() {
		buffer_.reset();
		exBuffer_.clear();
	}

    LogStream() {}

private:
	template <typename T>
	void formatInteger(T);

	Buffer buffer_;
	std::string exBuffer_;
};

class Fmt // : boost::noncopyable
{
public:
	template <typename T>
	Fmt(const char *fmt, T val);

	const char *data() const {
		return buf_;
	}
	int length() const {
		return length_;
	}

private:
	char buf_[48];
	int length_;
};

inline LogStream &operator<<(LogStream &s, const Fmt &fmt) {
	s.append(fmt.data(), fmt.length());
	return s;
}

} // namespace trantor
