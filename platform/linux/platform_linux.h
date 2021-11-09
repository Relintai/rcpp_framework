#ifndef PLATFORM_LINUX_H
#define PLATFORM_LINUX_H

#include "core/os/platform.h"

class PlatformLinux : public Platform {
public:
	virtual String get_executable_path();

	virtual int execute(const String &p_path, const Vector<String> &p_arguments, bool p_blocking, int64_t *r_child_id = nullptr, String *r_pipe = nullptr, int *r_exitcode = nullptr, Mutex *p_pipe_mutex = nullptr, bool read_stderr = false);

	PlatformLinux();
	virtual ~PlatformLinux();

protected:
};

#endif
