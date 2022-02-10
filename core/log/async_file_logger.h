
// This file is originally from Trantor - AsyncFileLogger.h

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

#include "core/math/date.h"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>

namespace trantor {
using StringPtr = std::shared_ptr<std::string>;
using StringPtrQueue = std::queue<StringPtr>;

/**
 * @brief This class implements utility functions for writing logs to files
 * asynchronously.
 *
 */
class AsyncFileLogger {
protected:
	AsyncFileLogger(const AsyncFileLogger &) = delete;
	AsyncFileLogger &operator=(const AsyncFileLogger &) = delete;
	// some uncopyable classes maybe support move constructor....
	AsyncFileLogger(AsyncFileLogger &&) noexcept(true) = default;
	AsyncFileLogger &operator=(AsyncFileLogger &&) noexcept(true) = default;

public:
	/**
	 * @brief Write the message to the log file.
	 *
	 * @param msg
	 * @param len
	 */
	void output(const char *msg, const uint64_t len);

	/**
	 * @brief Flush data from memory buffer to the log file.
	 *
	 */
	void flush();

	/**
	 * @brief Start writing log files.
	 *
	 */
	void startLogging();

	/**
	 * @brief Set the size limit of log files. When the log file size reaches
	 * the limit, the log file is switched.
	 *
	 * @param limit
	 */
	void setFileSizeLimit(uint64_t limit) {
		sizeLimit_ = limit;
	}

	/**
	 * @brief Set the log file name.
	 *
	 * @param baseName The base name of the log file.
	 * @param extName The extended name of the log file.
	 * @param path The location where the log file is stored.
	 */
	void setFileName(const std::string &baseName,
			const std::string &extName = ".log",
			const std::string &path = "./") {
		fileBaseName_ = baseName;
		extName[0] == '.' ? fileExtName_ = extName : fileExtName_ = std::string(".") + extName;
		filePath_ = path;
		if (filePath_.length() == 0)
			filePath_ = "./";
		if (filePath_[filePath_.length() - 1] != '/')
			filePath_ = filePath_ + "/";
	}
	~AsyncFileLogger();
	AsyncFileLogger();

protected:
	std::mutex mutex_;
	std::condition_variable cond_;
	StringPtr logBufferPtr_;
	StringPtr nextBufferPtr_;
	StringPtrQueue writeBuffers_;
	StringPtrQueue tmpBuffers_;
	void writeLogToFile(const StringPtr buf);
	std::unique_ptr<std::thread> threadPtr_;
	bool stopFlag_{ false };
	void logThreadFunc();
	std::string filePath_{ "./" };
	std::string fileBaseName_{ "trantor" };
	std::string fileExtName_{ ".log" };
	uint64_t sizeLimit_{ 20 * 1024 * 1024 };

	class LoggerFile {
	protected:
		LoggerFile(const LoggerFile &) = delete;
		LoggerFile &operator=(const LoggerFile &) = delete;
		// some uncopyable classes maybe support move constructor....
		LoggerFile(LoggerFile &&) noexcept(true) = default;
		LoggerFile &operator=(LoggerFile &&) noexcept(true) = default;

	public:
		LoggerFile(const std::string &filePath,
				const std::string &fileBaseName,
				const std::string &fileExtName);
		~LoggerFile();
		void writeLog(const StringPtr buf);
		uint64_t getLength();
		explicit operator bool() const {
			return fp_ != nullptr;
		}
		void flush();

	protected:
		FILE *fp_{ nullptr };
		Date creationDate_;
		std::string fileFullName_;
		std::string filePath_;
		std::string fileBaseName_;
		std::string fileExtName_;
		static uint64_t fileSeq_;
	};
  
	std::unique_ptr<LoggerFile> loggerFilePtr_;

	uint64_t lostCounter_{ 0 };
	void swapBuffer();
};

} // namespace trantor
