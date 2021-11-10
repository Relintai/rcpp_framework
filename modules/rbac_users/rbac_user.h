#ifndef RBAC_USER_H
#define RBAC_USER_H

#include "core/string.h"

#include "modules/users/user.h"

class Request;
class FormValidator;

class RBACUser : public User {
	RCPP_OBJECT(RBACUser, User);

public:
	RBACUser();
	~RBACUser();
};

#endif