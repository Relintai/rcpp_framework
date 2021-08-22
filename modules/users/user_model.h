#ifndef USER_MODEL_H
#define USER_MODEL_H

#include "core/object.h"

#include <string>

class User;

class UserModel : public Object {
public:
	static UserModel *get_singleton();

	UserModel();
	~UserModel();

protected:
	static UserModel *_self;
};

#endif