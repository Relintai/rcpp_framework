#ifndef USER_INITIALIZER_H
#define USER_INITIALIZER_H

#include "rbac_user_model.h"
#include "rbac_user_controller.h"

class UserInitializer {
public:
	static void allocate_controller();
	static void free_controller();

	static void allocate_model();
	static void free_model();

	static void allocate_all();
	static void free_all();

protected:
	static RBACUserController *_controller;
	static RBACUserModel *_model;
};

#endif