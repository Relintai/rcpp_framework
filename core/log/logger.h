
// This file contains code from Trantor - Logger.h

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

#ifndef LOGGER_H
#define LOGGER_H

#include "core/math/date.h"
#include "log_stream.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>

class String;

class RLogger {
public:
	static void log_trace(const String &str);
	static void log_trace(const char *str);

	static void log_message(const String &str);
	static void log_message(const char *str);

	static void log_warning(const String &str);
	static void log_warning(const char *str);

	static void log_error(const String &str);
	static void log_error(const char *str);

	static void _log_error(const char *p_function, const char *p_file, int p_line, const char *str);
	static void _log_error(const char *p_function, const char *p_file, int p_line, const String &str);
	static void _log_msg_error(const char *p_function, const char *p_file, int p_line, const char *p_msg, const char *str);
	static void _log_index_error(const char *p_function, const char *p_file, int p_line, const int index, const int size, const char *str);
};

/**
 * @brief This class implements log functions.
 *
 */
class Logger {
protected:
	Logger(const Logger &) = delete;
	Logger &operator=(const Logger &) = delete;
	// some uncopyable classes maybe support move constructor....
	Logger(Logger &&) noexcept(true) = default;
	Logger &operator=(Logger &&) noexcept(true) = default;

public:
	enum LogLevel {
		kTrace = 0,
		kDebug,
		kInfo,
		kWarn,
		kError,
		kFatal,
		kNumberOfLogLevels
	};

	/**
	 * @brief Calculate of basename of source files in compile time.
	 *
	 */
	class SourceFile {
	public:
		template <int N>
		inline SourceFile(const char (&arr)[N]) :
				data_(arr), size_(N - 1) {
			// std::cout<<data_<<std::endl;
			const char *slash = strrchr(data_, '/'); // builtin function
			if (slash) {
				data_ = slash + 1;
				size_ -= static_cast<int>(data_ - arr);
			}
		}

		explicit SourceFile(const char *filename) :
				data_(filename) {
			const char *slash = strrchr(filename, '/');
			if (slash) {
				data_ = slash + 1;
			}
			size_ = static_cast<int>(strlen(data_));
		}

		const char *data_;
		int size_;
	};
	Logger(SourceFile file, int line);
	Logger(SourceFile file, int line, LogLevel level);
	Logger(SourceFile file, int line, bool isSysErr);
	Logger(SourceFile file, int line, LogLevel level, const char *func);
	~Logger();
	Logger &setIndex(int index) {
		index_ = index;
		return *this;
	}
	LogStream &stream();

	/**
	 * @brief Set the output function.
	 *
	 * @param outputFunc The function to output a log message.
	 * @param flushFunc The function to flush.
	 * @note Logs are output to the standard output by default.
	 */
	static void setOutputFunction(
			std::function<void(const char *msg, const uint64_t len)> outputFunc,
			std::function<void()> flushFunc,
			int index = -1) {
		if (index < 0) {
			outputFunc_() = outputFunc;
			flushFunc_() = flushFunc;
		} else {
			outputFunc_(index) = outputFunc;
			flushFunc_(index) = flushFunc;
		}
	}

	/**
	 * @brief Set the log level. Logs below the level are not printed.
	 *
	 * @param level
	 */
	static void setLogLevel(LogLevel level) {
		logLevel_() = level;
	}

	/**
	 * @brief Get the current log level.
	 *
	 * @return LogLevel
	 */
	static LogLevel logLevel() {
		return logLevel_();
	}

protected:
	static void defaultOutputFunction(const char *msg, const uint64_t len) {
		fwrite(msg, 1, len, stdout);
	}
	static void defaultFlushFunction() {
		fflush(stdout);
	}
	void formatTime();
	static LogLevel &logLevel_() {
#ifdef RELEASE
		static LogLevel logLevel = LogLevel::kInfo;
#else
		static LogLevel logLevel = LogLevel::kDebug;
#endif
		return logLevel;
	}
	static std::function<void(const char *msg, const uint64_t len)>
			&outputFunc_() {
		static std::function<void(const char *msg, const uint64_t len)>
				outputFunc = Logger::defaultOutputFunction;
		return outputFunc;
	}
	static std::function<void()> &flushFunc_() {
		static std::function<void()> flushFunc = Logger::defaultFlushFunction;
		return flushFunc;
	}
	static std::function<void(const char *msg, const uint64_t len)>
			&outputFunc_(size_t index) {
		static std::vector<
				std::function<void(const char *msg, const uint64_t len)> >
				outputFuncs;
		if (index < outputFuncs.size()) {
			return outputFuncs[index];
		}
		while (index >= outputFuncs.size()) {
			outputFuncs.emplace_back(outputFunc_());
		}
		return outputFuncs[index];
	}
	static std::function<void()> &flushFunc_(size_t index) {
		static std::vector<std::function<void()> > flushFuncs;
		if (index < flushFuncs.size()) {
			return flushFuncs[index];
		}
		while (index >= flushFuncs.size()) {
			flushFuncs.emplace_back(flushFunc_());
		}
		return flushFuncs[index];
	}
	friend class RawLogger;
	LogStream logStream_;
	Date date_{ Date::now() };
	SourceFile sourceFile_;
	int fileLine_;
	LogLevel level_;
	int index_{ -1 };
};

class RawLogger {
protected:
	RawLogger(const RawLogger &) = delete;
	RawLogger &operator=(const RawLogger &) = delete;
	// some uncopyable classes maybe support move constructor....
	RawLogger(RawLogger &&) noexcept(true) = default;
	RawLogger &operator=(RawLogger &&) noexcept(true) = default;

public:
    RawLogger() {}
	~RawLogger();
	RawLogger &setIndex(int index) {
		index_ = index;
		return *this;
	}
	LogStream &stream() {
		return logStream_;
	}

private:
	LogStream logStream_;
	int index_{ -1 };
};


#ifdef NDEBUG
#define LOG_TRACE                                                          \
	if (0)                                                                 \
	Logger(__FILE__, __LINE__, Logger::kTrace, __func__) \
			.stream()
#else
#define LOG_TRACE                                                          \
	if (Logger::logLevel() <= Logger::kTrace)            \
	Logger(__FILE__, __LINE__, Logger::kTrace, __func__) \
			.stream()
#define LOG_TRACE_TO(index)                                                \
	if (Logger::logLevel() <= Logger::kTrace)            \
	Logger(__FILE__, __LINE__, Logger::kTrace, __func__) \
			.setIndex(index)                                               \
			.stream()

#endif

#define LOG_DEBUG                                                          \
	if (Logger::logLevel() <= Logger::kDebug)            \
	Logger(__FILE__, __LINE__, Logger::kDebug, __func__) \
			.stream()
#define LOG_DEBUG_TO(index)                                                \
	if (Logger::logLevel() <= Logger::kDebug)            \
	Logger(__FILE__, __LINE__, Logger::kDebug, __func__) \
			.setIndex(index)                                               \
			.stream()
#define LOG_INFO                                               \
	if (Logger::logLevel() <= Logger::kInfo) \
	Logger(__FILE__, __LINE__).stream()
#define LOG_INFO_TO(index)                                     \
	if (Logger::logLevel() <= Logger::kInfo) \
	Logger(__FILE__, __LINE__).setIndex(index).stream()
#define LOG_WARN \
	Logger(__FILE__, __LINE__, Logger::kWarn).stream()
#define LOG_WARN_TO(index)                                      \
	Logger(__FILE__, __LINE__, Logger::kWarn) \
			.setIndex(index)                                    \
			.stream()
#define LOG_ERROR \
	Logger(__FILE__, __LINE__, Logger::kError).stream()
#define LOG_ERROR_TO(index)                                      \
	Logger(__FILE__, __LINE__, Logger::kError) \
			.setIndex(index)                                     \
			.stream()
#define LOG_FATAL \
	Logger(__FILE__, __LINE__, Logger::kFatal).stream()
#define LOG_FATAL_TO(index)                                      \
	Logger(__FILE__, __LINE__, Logger::kFatal) \
			.setIndex(index)                                     \
			.stream()
#define LOG_SYSERR Logger(__FILE__, __LINE__, true).stream()
#define LOG_SYSERR_TO(index) \
	Logger(__FILE__, __LINE__, true).setIndex(index).stream()

#define LOG_RAW RawLogger().stream()
#define LOG_RAW_TO(index) RawLogger().setIndex(index).stream()

#define LOG_TRACE_IF(cond)                                                  \
	if ((Logger::logLevel() <= Logger::kTrace) && (cond)) \
	Logger(__FILE__, __LINE__, Logger::kTrace, __func__)  \
			.stream()
#define LOG_DEBUG_IF(cond)                                                \
	if ((Tensor::Logger::logLevel() <= Tensor::Logger::kDebug) && (cond)) \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__)  \
			.stream()
#define LOG_INFO_IF(cond)                                                \
	if ((Tensor::Logger::logLevel() <= Tensor::Logger::kInfo) && (cond)) \
	Tensor::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN_IF(cond) \
	if (cond)             \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define LOG_ERROR_IF(cond) \
	if (cond)              \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define LOG_FATAL_IF(cond) \
	if (cond)              \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()

#ifdef NDEBUG
#define DLOG_TRACE                                                         \
	if (0)                                                                 \
	Logger(__FILE__, __LINE__, Logger::kTrace, __func__) \
			.stream()
#define DLOG_DEBUG                                                       \
	if (0)                                                               \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__) \
			.stream()
#define DLOG_INFO \
	if (0)        \
	Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN \
	if (0)        \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR \
	if (0)         \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL \
	if (0)         \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()

#define DLOG_TRACE_IF(cond)                                                \
	if (0)                                                                 \
	Logger(__FILE__, __LINE__, Logger::kTrace, __func__) \
			.stream()
#define DLOG_DEBUG_IF(cond)                                              \
	if (0)                                                               \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__) \
			.stream()
#define DLOG_INFO_IF(cond) \
	if (0)                 \
	Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN_IF(cond) \
	if (0)                 \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR_IF(cond) \
	if (0)                  \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL_IF(cond) \
	if (0)                  \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()
#else
#define DLOG_TRACE                                                         \
	if (Logger::logLevel() <= Logger::kTrace)            \
	Logger(__FILE__, __LINE__, Logger::kTrace, __func__) \
			.stream()
#define DLOG_DEBUG                                                       \
	if (Tensor::Logger::logLevel() <= Tensor::Logger::kDebug)            \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__) \
			.stream()
#define DLOG_INFO                                            \
	if (Tensor::Logger::logLevel() <= Tensor::Logger::kInfo) \
	Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()

#define DLOG_TRACE_IF(cond)                                                 \
	if ((Logger::logLevel() <= Logger::kTrace) && (cond)) \
	Logger(__FILE__, __LINE__, Logger::kTrace, __func__)  \
			.stream()
#define DLOG_DEBUG_IF(cond)                                               \
	if ((Tensor::Logger::logLevel() <= Tensor::Logger::kDebug) && (cond)) \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kDebug, __func__)  \
			.stream()
#define DLOG_INFO_IF(cond)                                               \
	if ((Tensor::Logger::logLevel() <= Tensor::Logger::kInfo) && (cond)) \
	Tensor::Logger(__FILE__, __LINE__).stream()
#define DLOG_WARN_IF(cond) \
	if (cond)              \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kWarn).stream()
#define DLOG_ERROR_IF(cond) \
	if (cond)               \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kError).stream()
#define DLOG_FATAL_IF(cond) \
	if (cond)               \
	Tensor::Logger(__FILE__, __LINE__, Tensor::Logger::kFatal).stream()
#endif

const char *strerror_tl(int savedErrno);

#endif
