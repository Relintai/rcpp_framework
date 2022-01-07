#ifndef RBAC_USER_CONTROLLER_H
#define RBAC_USER_CONTROLLER_H

#include "modules/users/user_controller.h"

class Request;

class RBACUserController : public UserController {
	RCPP_OBJECT(RBACUserController, UserController);

public:
	// just session setup
	static void rbac_user_session_setup_middleware(Object *instance, Request *request);
	// this one also handles missing read permission / redirect
	static void rbac_default_user_session_middleware(Object *instance, Request *request);

	// db

	Ref<User> db_get_user(const int id);
	Ref<User> db_get_user(const String &user_name_input);

	Vector<Ref<User> > db_get_all();

	Ref<User> create_user();

	RBACUserController();
	~RBACUserController();

protected:
};

#endif