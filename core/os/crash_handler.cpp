#include "crash_handler.h"

#include "core/error_macros.h"

void CrashHandler::enable() {
}
void CrashHandler::disable() {
}

CrashHandler *CrashHandler::get_singleton() {
	return _self;
}

CrashHandler::CrashHandler() {
    enabled = false;

	_self = this;
}
CrashHandler::~CrashHandler() {
    disable();

	if (_self == this) {
		_self = nullptr;
	}
}

CrashHandler *CrashHandler::_self = nullptr;
