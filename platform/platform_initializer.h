#ifndef PLATFORM_INITIALIZER_H
#define PLATFORM_INITIALIZER_H

#include "core/os/platform.h"
#include "core/os/crash_handler.h"


class PlatformInitializer {
public:	
	static void arg_setup(int argc, char **argv, char **envp);

	static void allocate_platform();
	static void free_platform();

	static void allocate_crash_handler();
	static void free_crash_handler();

	static void allocate_all();
	static void free_all();

protected:
	static CrashHandler *_crash_handler;
	static Platform *_platform;
};

#endif