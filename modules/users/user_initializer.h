#ifndef USER_INITIALIZER_H
#define USER_INITIALIZER_H

#include "user_model.h"
#include "user_controller.h"

class UserInitializer {
public:
	static void allocate_controller();
	static void free_controller();

	static void allocate_model();
	static void free_model();

	static void allocate_all();
	static void free_all();

protected:
	static UserController *_controller;
	static UserModel *_model;
};

#endif