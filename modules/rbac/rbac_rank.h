#ifndef RBAC_RANK_H
#define RBAC_RANK_H

#include "core/resource.h"

#include <string>
#include <vector>

#include "rbac_permission.h"

class RBACRank : public Resource {
	RCPP_OBJECT(RBACRank, Resource);

public:
	int id;

	std::string name;
	std::string name_internal;
	std::string settings;

	std::vector<Ref<RBACPermission> > groups;

	RBACRank();
	~RBACRank();
};

#endif