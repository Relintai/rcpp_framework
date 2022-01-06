#include "rbac_rank.h"

#include "core/http/request.h"

Ref<RBACPermission> RBACRank::match_request(Request *request) {
	return Ref<RBACPermission>();
}

bool RBACRank::get_permissions(Request *request) {
	int perm = base_permissions;

	Ref<RBACPermission> match = match_request(request);

	if (match.is_valid()) {
		perm = match->permissions;
	}

	return perm;
}

bool RBACRank::has_permission(Request *request, const int permission) {
	int perm = base_permissions;

	Ref<RBACPermission> match = match_request(request);

	if (match.is_valid()) {
		perm = match->permissions;
	}

	return (perm & permission) != 0;
}

bool RBACRank::has_rank_permission(const int permission) {
	return (rank_permissions & permission) != 0;
}

void RBACRank::sort_permissions() {
	for (int i = 0; i < permissions.size(); ++i) {
		for (int j = i + 1; j < permissions.size(); ++j) {
			if (permissions[j]->is_smaller(permissions[i])) {
				permissions.swap(i, j);
			}
		}
	}
}

RBACRank::RBACRank() :
		Resource() {

	id = 0;
	base_permissions = 0;
	rank_permissions = 0;
}

RBACRank::~RBACRank() {
	permissions.clear();
}
