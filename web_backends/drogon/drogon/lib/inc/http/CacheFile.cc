/**
 *
 *  CacheFile.cc
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

#include "CacheFile.h"
#include "core/log/logger.h"
#ifdef _WIN32
#include <mman.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

using namespace drogon;

CacheFile::CacheFile(const std::string &path, bool autoDelete) :
		autoDelete_(autoDelete), path_(path) {
#ifndef _MSC_VER
	file_ = fopen(path_.data(), "wb+");
#else
	if (fopen_s(&file_, path_.data(), "wb+") != 0) {
		file_ = nullptr;
	}
#endif
}

CacheFile::~CacheFile() {
	if (data_) {
		munmap(data_, dataLength_);
	}
	if (autoDelete_ && file_) {
		fclose(file_);
#ifdef _WIN32
		_unlink(path_.data());
#else
		unlink(path_.data());
#endif
	} else if (file_) {
		fclose(file_);
	}
}

void CacheFile::append(const char *data, size_t length) {
	if (file_)
		fwrite(data, length, 1, file_);
}

size_t CacheFile::length() {
	if (file_)
		return ftell(file_);
	return 0;
}

char *CacheFile::data() {
	if (!file_)
		return nullptr;
	if (!data_) {
		fflush(file_);
#ifdef _WIN32
		auto fd = _fileno(file_);
#else
		auto fd = fileno(file_);
#endif
		dataLength_ = length();
		data_ = static_cast<char *>(
				mmap(nullptr, dataLength_, PROT_READ, MAP_SHARED, fd, 0));
		if (data_ == MAP_FAILED) {
			data_ = nullptr;
			LOG_SYSERR << "mmap:";
		}
	}
	return data_;
}
