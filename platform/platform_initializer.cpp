#include "platform_initializer.h"

#include "core/error_macros.h"

#if PLATFORM_LINUX
#include "linux/crash_handler_linux.h"
#endif

void PlatformInitializer::allocate_crash_handler() {
	ERR_FAIL_COND(_crash_handler);

#if PLATFORM_LINUX
	_crash_handler = new CrashHandlerLinux();
	_crash_handler->enable();
#endif
}

void PlatformInitializer::free_crash_handler() {
	if (_crash_handler) {
		delete _crash_handler;
		_crash_handler = nullptr;
	}
}

void PlatformInitializer::allocate_all() {
	allocate_crash_handler();
}

void PlatformInitializer::free_all() {
	free_crash_handler();
}

CrashHandler *PlatformInitializer::_crash_handler = nullptr;