#include "rbac_rank.h"

#include "core/http/request.h"

bool RBACRank::has_permission(Request *request, const int permission) {
	//todo try to find a match from the permissions array

	return (base_permissions & permission) != 0;
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
