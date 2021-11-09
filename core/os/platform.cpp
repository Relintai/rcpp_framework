#include "platform.h"

#include "core/error_macros.h"

void Platform::arg_setup(int argc, char **argv, char **envp) {
	arg_parser.setup(argc, argv, envp);
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
