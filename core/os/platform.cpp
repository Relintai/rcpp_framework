#include "platform.h"

#include "core/error_macros.h"

void Platform::arg_setup(int argc, char **argv, char **envp) {
	arg_parser.setup(argc, argv, envp);
}

String Platform::get_executable_path() {
	return arg_parser.executable_name;
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
