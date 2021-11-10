#ifndef RBAC_USER_CONTROLLER_H
#define RBAC_USER_CONTROLLER_H

#include "modules/users/user_controller.h"

class RBACUserController : public UserController {
	RCPP_OBJECT(RBACUserController, UserController);
	
public:
	RBACUserController();
	~RBACUserController();

protected:
};

#endif