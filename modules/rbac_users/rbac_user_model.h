#ifndef RBAC_USER_MODEL_H
#define RBAC_USER_MODEL_H

#include "modules/users/user_model.h"

class RBACUserModel : public UserModel {
	RCPP_OBJECT(RBACUserModel, UserModel);
	
public:
	Ref<User> get_user(const int id);
	Ref<User> get_user(const String &user_name_input);

	Vector<Ref<User> > get_all();

	Ref<User> create_user(); 

	RBACUserModel();
	~RBACUserModel();

protected:
};

#endif