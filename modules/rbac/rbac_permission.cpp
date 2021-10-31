#include "rbac_permission.h"

RBACPermission::RBACPermission() :
		Resource() {

	id = 0;
	rank_id = 0;
	permission_id = 0;
	revoke = false;
	sort_order = 0;
	permissions = 0;
}

RBACPermission::~RBACPermission() {
}
