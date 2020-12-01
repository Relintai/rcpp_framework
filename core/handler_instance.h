#ifndef HANDLER_INSTANCE_H
#define HANDLER_INSTANCE_H

#include <functional>

class Object;
class Request;

struct HandlerInstance {
	std::function<void(Object *, Request *)> handler_func;
	Object *instance;

	HandlerInstance();
	HandlerInstance(std::function<void(Object *, Request *)> p_handler_func, Object *p_instance = nullptr);
};

#endif