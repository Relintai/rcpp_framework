#ifndef RBAC_USER_CONTROLLER_H
#define RBAC_USER_CONTROLLER_H

#include "modules/users/user_controller.h"

class Request;

class RBACUserController : public UserController {
	RCPP_OBJECT(RBACUserController, UserController);
	
public:
	//just session setup
	static void rbac_user_session_setup_middleware(Object *instance, Request *request);
	//this one also handles missing read permission / redirect
	static void rbac_default_user_session_middleware(Object *instance, Request *request);

	RBACUserController();
	~RBACUserController();

protected:
};

#endif