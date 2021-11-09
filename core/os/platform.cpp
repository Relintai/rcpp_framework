#include "platform.h"

#include "core/error_macros.h"

void Platform::arg_setup(int argc, char **argv, char **envp) {
	arg_parser.setup(argc, argv, envp);
}

String Platform::get_executable_path() {
	return arg_parser.executable_name;
}

int Platform::execute(const String &p_path, const Vector<String> &p_arguments, bool p_blocking, int64_t *r_child_id, String *r_pipe, int *r_exitcode, Mutex *p_pipe_mutex, bool read_stderr) {
	return 0;
}

Platform *Platform::get_singleton() {
	return _self;
}

Platform::Platform() {
	_self = this;
}
Platform::~Platform() {
	if (_self == this) {
		_self = nullptr;
	}
}

Platform *Platform::_self = nullptr;
