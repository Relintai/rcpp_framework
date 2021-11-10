#ifndef RBAC_RANK_H
#define RBAC_RANK_H

#include "core/string.h"
#include "core/containers/vector.h"

#include "core/resource.h"

#include "rbac_permission.h"

class Request;

class RBACRank : public Resource {
	RCPP_OBJECT(RBACRank, Resource);

public:
	int id;

	String name;
	String name_internal;
	String settings;

	int base_permissions;
	int rank_permissions;

	Vector<Ref<RBACPermission> > permissions;

	bool has_permission(Request *request, const int permission);

	void sort_permissions();

	RBACRank();
	~RBACRank();
};

#endif