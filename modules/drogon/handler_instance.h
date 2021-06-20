#ifndef DHANDLER_INSTANCE_H
#define DHANDLER_INSTANCE_H

#include <functional>

class Object;
class DRequest;

struct DHandlerInstance {
	std::function<void(Object *, DRequest *)> handler_func;
	Object *instance;

	DHandlerInstance();
	DHandlerInstance(std::function<void(Object *, DRequest *)> p_handler_func, Object *p_instance = nullptr);
};

#endif