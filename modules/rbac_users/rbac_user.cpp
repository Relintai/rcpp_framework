#include "rbac_user.h"

bool RBACUser::has_permission(Request *request, const int permission) {
	if (!rbac_rank.is_valid()) {
		return false;
	}

	return rbac_rank->has_permission(request, permission);
}

RBACUser::RBACUser() :
		User() {
}

RBACUser::~RBACUser() {
}
