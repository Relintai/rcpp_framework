#ifndef PLATFORM_INITIALIZER_H
#define PLATFORM_INITIALIZER_H

#include "core/os/crash_handler.h"


class PlatformInitializer {
public:
	static void allocate_crash_handler();
	static void free_crash_handler();

	static void allocate_all();
	static void free_all();

protected:
	static CrashHandler *_crash_handler;
};

#endif