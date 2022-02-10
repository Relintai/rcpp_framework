/**
 *
 *  SharedLibManager.h
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

#include <sys/stat.h>
#include "core/loops/event_loop_thread.h"
#include <trantor/utils/NonCopyable.h>
#include <unordered_map>
#include <vector>

//not sure wheter these are needed or not yet
#ifndef COMPILER_COMMAND
#define COMPILER_COMMAND "/usr/bin/c++"
#endif

#ifndef COMPILATION_FLAGS
#define COMPILATION_FLAGS "-std=c++17"
#endif

#ifndef COMPILER_ID
#define COMPILER_ID "GNU"
#endif

#ifndef INCLUDING_DIRS
#define INCLUDING_DIRS " -I/usr/include -I/usr/local/include"
#endif

namespace drogon {
class SharedLibManager : public NonCopyable {
public:
	SharedLibManager(const std::vector<std::string> &libPaths,
			const std::string &outputPath);
	~SharedLibManager();

private:
	void managerLibs();
	std::vector<std::string> libPaths_;
	std::string outputPath_;
	struct DLStat {
		void *handle{ nullptr };
		struct timespec mTime = { 0, 0 };
	};
	std::unordered_map<std::string, DLStat> dlMap_;
	void *compileAndLoadLib(const std::string &sourceFile, void *oldHld);
	void *loadLib(const std::string &soFile, void *oldHld);
	bool shouldCompileLib(const std::string &soFile,
			const struct stat &sourceStat);
	TimerId timeId_;
	EventLoopThread workingThread_;
};
} // namespace drogon
