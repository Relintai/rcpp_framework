#include "rbac_rank.h"

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
	rank_permissions = 0;
}

RBACRank::~RBACRank() {
	permissions.clear();
}
