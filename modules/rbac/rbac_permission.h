#ifndef RBAC_PERMISSION_H
#define RBAC_PERMISSION_H

#include "core/resource.h"

#include <string>

class RBACPermission : public Resource {
	RCPP_OBJECT(RBACPermission, Resource);

public:
	int id;
	int rank_id;
	int permission_id;
	std::string name;
	std::string url;
	bool revoke;
	int sort_order;
	int permissions;

	RBACPermission();
	~RBACPermission();
};

#endif