#ifndef PLATFORM_H
#define PLATFORM_H

#include "arg_parser.h"

#include "core/threading/mutex.h"

class Platform {
public:
	virtual void arg_setup(int argc, char **argv, char **envp);

	virtual String get_executable_path();

	virtual int execute(const String &p_path, const Vector<String> &p_arguments, bool p_blocking, int64_t *r_child_id = nullptr, String *r_pipe = nullptr, int *r_exitcode = nullptr, Mutex *p_pipe_mutex = nullptr, bool read_stderr = false);

	static Platform *get_singleton();

	Platform();
	virtual ~Platform();

	ArgParser arg_parser;

protected:
	static Platform *_self;
};

#endif
