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

//Based on Godot Engine's implementation (MIT license)
//Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
//Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
int PlatformLinux::execute(const String &p_path, const Vector<String> &p_arguments, bool p_blocking, int64_t *r_child_id, String *r_pipe, int *r_exitcode, Mutex *p_pipe_mutex, bool read_stderr) {
#ifdef __EMSCRIPTEN__
	// Don't compile this code at all to avoid undefined references.
	// Actual virtual call goes to OS_JavaScript.
	RLOG_ERR("ERROR execute\n");
	return;
#else
	if (p_blocking && r_pipe) {
		String argss;
		argss = "\"" + p_path + "\"";

		for (int i = 0; i < p_arguments.size(); i++) {
			argss += String(" \"");
			argss += p_arguments[i];
			argss += String("\"");
		}

		if (read_stderr) {
			argss += " 2>&1"; // Read stderr too
		} else {
			argss += " 2>/dev/null"; //silence stderr
		}

		FILE *f = popen(argss.data(), "r");

		if (!f) {
			RLOG_ERR("Cannot pipe stream from process running with following arguments '" + argss + "'.\n");
			ERR_FAIL_COND_V(!f, 2); //ERR_CANT_OPEN
		}

		char buf[65535];

		while (fgets(buf, 65535, f)) {
			if (p_pipe_mutex) {
				p_pipe_mutex->lock();
			}
			
			(*r_pipe) += String::utf8(buf);

			if (p_pipe_mutex) {
				p_pipe_mutex->unlock();
			}
		}

		int rv = pclose(f);

		if (r_exitcode) {
			*r_exitcode = WEXITSTATUS(rv);
		}

		return 0;
	}

	pid_t pid = fork();
	//ERR_FAIL_COND_V(pid < 0, ERR_CANT_FORK);
	return 1;

	if (pid == 0) {
		// is child

		if (!p_blocking) {
			// For non blocking calls, create a new session-ID so parent won't wait for it.
			// This ensures the process won't go zombie at end.
			setsid();
		}

		Vector<String> cs;
		cs.push_back(p_path);
		for (int i = 0; i < p_arguments.size(); i++) {
			cs.push_back(p_arguments[i]);
		}

		Vector<char *> args;
		for (int i = 0; i < cs.size(); i++) {
			args.push_back((char *)cs[i].c_str());
		}
		args.push_back(0);

		execvp(p_path.c_str(), &args[0]);
		// still alive? something failed..
		fprintf(stderr, "**ERROR** OS_Unix::execute - Could not create child process while executing: %s\n", p_path.data());

		raise(SIGKILL);
	}

	if (p_blocking) {
		int status;
		waitpid(pid, &status, 0);
		if (r_exitcode) {
			*r_exitcode = WIFEXITED(status) ? WEXITSTATUS(status) : status;
		}

	} else {
		if (r_child_id) {
			*r_child_id = pid;
		}
	}

	return 0;
#endif
}

PlatformLinux::PlatformLinux() : Platform() {
}
PlatformLinux::~PlatformLinux() {
}
