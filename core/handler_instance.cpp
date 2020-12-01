#include "handler_instance.h"

#include "request.h"
#include "object.h"

HandlerInstance::HandlerInstance() {
	instance = nullptr;
}

HandlerInstance::HandlerInstance(std::function<void(Object *, Request *)> p_handler_func, Object *p_instance) {
	handler_func = p_handler_func;
	instance = p_instance;
}