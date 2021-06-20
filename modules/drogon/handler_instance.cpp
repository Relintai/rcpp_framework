#include "handler_instance.h"

#include "request.h"
#include "core/object.h"

DHandlerInstance::DHandlerInstance() {
	instance = nullptr;
}

DHandlerInstance::DHandlerInstance(std::function<void(Object *, DRequest *)> p_handler_func, Object *p_instance) {
	handler_func = p_handler_func;
	instance = p_instance;
}