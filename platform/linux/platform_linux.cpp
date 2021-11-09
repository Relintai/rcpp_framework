#include "platform_linux.h"

#include "core/error_macros.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

//Based on Godot Engine's implementation (MIT license)
//Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
//Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
String PlatformLinux::get_executable_path() {
#ifdef __linux__
	//fix for running from a symlink
	char buf[256];
	memset(buf, 0, 256);
	ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf));
	String b;

	if (len > 0) {
		b.parse_utf8(buf, len);
	}

	if (b == "") {
		RLOG_WARN("Couldn't get executable path from /proc/self/exe, using argv[0]\n");

		return Platform::get_executable_path();
	}

	return b;
#elif defined(__OpenBSD__) || defined(__NetBSD__)
	char resolved_path[MAXPATHLEN];

	realpath(Platform::get_executable_path().utf8().get_data(), resolved_path);

	return String(resolved_path);
#elif defined(__FreeBSD__)
	int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
	char buf[MAXPATHLEN];
	size_t len = sizeof(buf);
	if (sysctl(mib, 4, buf, &len, NULL, 0) != 0) {
		RLOG_WARN("Couldn't get executable path from sysctl\n");
		return Platform::get_executable_path();
	}
	String b;
	b.parse_utf8(buf);
	return b;
#elif defined(__APPLE__)
	char temp_path[1];
	uint32_t buff_size = 1;
	_NSGetExecutablePath(temp_path, &buff_size);

	char *resolved_path = new char[buff_size + 1];

	if (_NSGetExecutablePath(resolved_path, &buff_size) == 1)
		RLOG_WARN("MAXPATHLEN is too small\n");

	String path(resolved_path);
	delete[] resolved_path;

	return path;
#else
	RLOG_ERR("Warning, don't know how to obtain executable path on this OS! Please override this function properly.\n");
	return Platform::get_executable_path();
#endif
}

PlatformLinux::PlatformLinux() : Platform() {
}
PlatformLinux::~PlatformLinux() {
}
