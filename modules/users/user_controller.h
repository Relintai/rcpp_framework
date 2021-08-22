#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "core/object.h"

#include <string>

class User;

class UserController : public Object {
public:
	static UserController *get_singleton();

	UserController();
	~UserController();

protected:
	static UserController *_self;
};

#endif