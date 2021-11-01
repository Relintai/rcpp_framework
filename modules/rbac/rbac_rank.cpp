#include "rbac_rank.h"

RBACRank::RBACRank() :
		Resource() {

	id = 0;
	rank_permissions = 0;
}

RBACRank::~RBACRank() {
	permissions.clear();
}
