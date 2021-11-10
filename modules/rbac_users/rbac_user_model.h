#ifndef RBAC_USER_MODEL_H
#define RBAC_USER_MODEL_H

#include "modules/users/user_model.h"

class RBACUserModel : public UserModel {
	RCPP_OBJECT(RBACUserModel, UserModel);
	
public:
	RBACUserModel();
	~RBACUserModel();

protected:
};

#endif