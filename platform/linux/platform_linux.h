#ifndef PLATFORM_LINUX_H
#define PLATFORM_LINUX_H

#include "core/os/platform.h"

class PlatformLinux : public Platform {
public:
	virtual String get_executable_path();

	PlatformLinux();
	virtual ~PlatformLinux();

protected:
};

#endif
