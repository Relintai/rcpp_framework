/*************************************************************************/
/*  crash_handler_x11.cpp                                                */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "crash_handler_linux.h"

#ifdef DEBUG_ENABLED
#define CRASH_HANDLER_ENABLED 1
#endif

#ifdef CRASH_HANDLER_ENABLED
#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

#include "core/string.h"
#include "core/containers/vector.h"

#include "core/os/platform.h"

int execute(const String &p_path, const Vector<String> &p_arguments, bool p_blocking, int64_t *r_child_id, String *r_pipe, int *r_exitcode) {
#ifdef __EMSCRIPTEN__
	// Don't compile this code at all to avoid undefined references.
	// Actual virtual call goes to OS_JavaScript.
	ERR_FAIL_V(ERR_BUG);
#else
	bool read_stderr = false;
//Mutex *p_pipe_mutex;

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
			return 2;
		}

		//ERR_FAIL_COND_V_MSG(!f, ERR_CANT_OPEN, "Cannot pipe stream from process running with following arguments '" + argss + "'.");

		char buf[65535];

		while (fgets(buf, 65535, f)) {
			//if (p_pipe_mutex) {
			//	p_pipe_mutex->lock();
			//}
			
			(*r_pipe) += String::utf8(buf);

			//if (p_pipe_mutex) {
			//	p_pipe_mutex->unlock();
			//}
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

static void handle_crash(int sig) {
	void *bt_buffer[256];
	size_t size = backtrace(bt_buffer, 256);
	String _execpath = Platform::get_singleton()->get_executable_path();

	// Dump the backtrace to stderr with a message to the user
	fprintf(stderr, "\n================================================================\n");
	fprintf(stderr, "%s: Program crashed with signal %d\n", __FUNCTION__, sig);

	fprintf(stderr, "Dumping the backtrace.\n");
	char **strings = backtrace_symbols(bt_buffer, size);
	if (strings) {
		for (size_t i = 1; i < size; i++) {
			char fname[1024];
			Dl_info info;

			snprintf(fname, 1024, "%s", strings[i]);

			// Try to demangle the function name to provide a more readable one
			if (dladdr(bt_buffer[i], &info) && info.dli_sname) {
				if (info.dli_sname[0] == '_') {
					int status;
					char *demangled = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);

					if (status == 0 && demangled) {
						snprintf(fname, 1024, "%s", demangled);
					}

					if (demangled) {
						free(demangled);
					}
				}
			}

			Vector<String> args;

			char str[1024];
			snprintf(str, 1024, "%p", bt_buffer[i]);
			args.push_back(str);
			args.push_back("-e");
			args.push_back(_execpath);

			String output = "";

			// Try to get the file/line number using addr2line
			int ret;
			int err = execute(String("addr2line"), args, true, nullptr, &output, &ret);

			if (err == 0) {
				output.erase(output.size() - 1, 1);
			}

			fprintf(stderr, "[%ld] %s (%s)\n", (long int)i, fname, output.c_str());
		}

		free(strings);
	}
	fprintf(stderr, "-- END OF BACKTRACE --\n");
	fprintf(stderr, "================================================================\n");

	// Abort to pass the error to the OS
	abort();
}
#endif

CrashHandlerLinux::CrashHandlerLinux() : CrashHandler() {
}

CrashHandlerLinux::~CrashHandlerLinux() {
}

void CrashHandlerLinux::disable() {
	if (!enabled) {
		return;
	}

#ifdef CRASH_HANDLER_ENABLED
	signal(SIGSEGV, nullptr);
	signal(SIGFPE, nullptr);
	signal(SIGILL, nullptr);
#endif

	enabled = false;
}

void CrashHandlerLinux::enable() {
	if (enabled) {
		return;
	}

#ifdef CRASH_HANDLER_ENABLED
	signal(SIGSEGV, handle_crash);
	signal(SIGFPE, handle_crash);
	signal(SIGILL, handle_crash);
#endif

	enabled = true;
}
