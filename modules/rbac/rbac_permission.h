#ifndef RBAC_PERMISSION_H
#define RBAC_PERMISSION_H

#include "core/string.h"

#include "core/resource.h"

class RBACPermission : public Resource {
	RCPP_OBJECT(RBACPermission, Resource);

public:
	int id;
	int rank_id;
	String name;
	String url;
	bool revoke;
	int sort_order;
	int permissions;

	RBACPermission();
	~RBACPermission();
};

#endif