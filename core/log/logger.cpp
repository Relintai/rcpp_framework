
// This file contains code from Trantor - Logger.cc

// Copyright (c) 2022 Péter Magyar
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

#include "logger.h"

#include "core/string.h"
#include <cstdio>

#include <stdio.h>
#include "logger.h"
#include <thread>
#ifndef _WIN32
#include <sys/syscall.h>
#include <unistd.h>
#else
#include <sstream>
#endif
#ifdef __FreeBSD__
#include <pthread_np.h>
#endif

void RLogger::log_trace(const String &str) {
	log_trace(str.data());
}
void RLogger::log_trace(const char *str) {
	printf("T %s\n", str);
}

void RLogger::log_message(const String &str) {
	log_message(str.data());
}
void RLogger::log_message(const char *str) {
	printf("M %s\n", str);
}

void RLogger::log_warning(const String &str) {
	log_warning(str.data());
}
void RLogger::log_warning(const char *str) {
	printf("W %s\n", str);
}

void RLogger::log_error(const String &str) {
	log_error(str.data());
}
void RLogger::log_error(const char *str) {
	printf("E %s\n", str);
}

void RLogger::_log_error(const char *p_function, const char *p_file, int p_line, const char *str) {
	printf("!ERROR: (%s) %s:%d. %s\n", p_file, p_function, p_line, str);
}
void RLogger::_log_error(const char *p_function, const char *p_file, int p_line, const String &str) {
	printf("!ERROR: (%s) %s:%d. %s\n", p_file, p_function, p_line, str.c_str());
}
void RLogger::_log_msg_error(const char *p_function, const char *p_file, int p_line, const char *p_msg, const char *str) {
	printf("!ERROR: (%s) %s:%d :: %s. %s\n", p_file, p_function, p_line, str, p_msg);
}
void RLogger::_log_index_error(const char *p_function, const char *p_file, int p_line, const int index, const int size, const char *str) {
	printf("!INDEX ERROR: (%s) %s:%d :: index: %d/%d. %s\n", p_file, p_function, p_line, index, size, str);
}

// helper class for known string length at compile time
class T {
public:
	T(const char *str, unsigned len) :
			str_(str), len_(len) {
		assert(strlen(str) == len_);
	}

	const char *str_;
	const unsigned len_;
};

const char *strerror_tl(int savedErrno) {
#ifndef _MSC_VER
	return strerror(savedErrno);
#else
	static thread_local char errMsg[64];
	(void)strerror_s<sizeof errMsg>(errMsg, savedErrno);
	return errMsg;
#endif
}

inline LogStream &operator<<(LogStream &s, T v) {
	s.append(v.str_, v.len_);
	return s;
}

inline LogStream &operator<<(LogStream &s, const Logger::SourceFile &v) {
	s.append(v.data_, v.size_);
	return s;
}

static thread_local uint64_t lastSecond_{ 0 };
static thread_local char lastTimeString_[32] = { 0 };
#ifdef __linux__
static thread_local pid_t threadId_{ 0 };
#else
static thread_local uint64_t threadId_{ 0 };
#endif
//   static thread_local LogStream logStream_;

void Logger::formatTime() {
	uint64_t now = static_cast<uint64_t>(date_.secondsSinceEpoch());
	uint64_t microSec =
			static_cast<uint64_t>(date_.microSecondsSinceEpoch() -
								  date_.roundSecond().microSecondsSinceEpoch());
	if (now != lastSecond_) {
		lastSecond_ = now;
#ifndef _MSC_VER
		strncpy(lastTimeString_,
				date_.toFormattedString(false).c_str(),
				sizeof(lastTimeString_) - 1);
#else
		strncpy_s<sizeof lastTimeString_>(
				lastTimeString_,
				date_.toFormattedString(false).c_str(),
				sizeof(lastTimeString_) - 1);
#endif
	}
	logStream_ << T(lastTimeString_, 17);
	char tmp[32];
	snprintf(tmp,
			sizeof(tmp),
			".%06llu UTC ",
			static_cast<long long unsigned int>(microSec));
	logStream_ << T(tmp, 12);
#ifdef __linux__
	if (threadId_ == 0)
		threadId_ = static_cast<pid_t>(::syscall(SYS_gettid));
#elif defined __FreeBSD__
	if (threadId_ == 0) {
		threadId_ = pthread_getthreadid_np();
	}
#elif defined __OpenBSD__
	if (threadId_ == 0) {
		threadId_ = getthrid();
	}
#elif defined _WIN32
	if (threadId_ == 0) {
		std::stringstream ss;
		ss << std::this_thread::get_id();
		threadId_ = std::stoull(ss.str());
	}
#else
	if (threadId_ == 0) {
		pthread_threadid_np(NULL, &threadId_);
	}
#endif
	logStream_ << threadId_;
}
static const char *logLevelStr[Logger::LogLevel::kNumberOfLogLevels] = {
	" TRACE ",
	" DEBUG ",
	" INFO  ",
	" WARN  ",
	" ERROR ",
	" FATAL ",
};
Logger::Logger(SourceFile file, int line) :
		sourceFile_(file), fileLine_(line), level_(kInfo) {
	formatTime();
	logStream_ << T(logLevelStr[level_], 7);
}
Logger::Logger(SourceFile file, int line, LogLevel level) :
		sourceFile_(file), fileLine_(line), level_(level) {
	formatTime();
	logStream_ << T(logLevelStr[level_], 7);
}
Logger::Logger(SourceFile file, int line, LogLevel level, const char *func) :
		sourceFile_(file), fileLine_(line), level_(level) {
	formatTime();
	logStream_ << T(logLevelStr[level_], 7) << "[" << func << "] ";
}
Logger::Logger(SourceFile file, int line, bool) :
		sourceFile_(file), fileLine_(line), level_(kFatal) {
	formatTime();
	logStream_ << T(logLevelStr[level_], 7);
	if (errno != 0) {
		logStream_ << strerror_tl(errno) << " (errno=" << errno << ") ";
	}
}
RawLogger::~RawLogger() {
	if (index_ < 0) {
		auto &oFunc = Logger::outputFunc_();
		if (!oFunc)
			return;
		oFunc(logStream_.bufferData(), logStream_.bufferLength());
	} else {
		auto &oFunc = Logger::outputFunc_(index_);
		if (!oFunc)
			return;
		oFunc(logStream_.bufferData(), logStream_.bufferLength());
	}
}
Logger::~Logger() {
	logStream_ << T(" - ", 3) << sourceFile_ << ':' << fileLine_ << '\n';
	if (index_ < 0) {
		auto &oFunc = Logger::outputFunc_();
		if (!oFunc)
			return;
		oFunc(logStream_.bufferData(), logStream_.bufferLength());
		if (level_ >= kError)
			Logger::flushFunc_()();
	} else {
		auto &oFunc = Logger::outputFunc_(index_);
		if (!oFunc)
			return;
		oFunc(logStream_.bufferData(), logStream_.bufferLength());
		if (level_ >= kError)
			Logger::flushFunc_(index_)();
	}

	// logStream_.resetBuffer();
}
LogStream &Logger::stream() {
	return logStream_;
}
