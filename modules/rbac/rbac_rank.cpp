#include "rbac_rank.h"

RBACRank::RBACRank() :
		Resource() {

	id = 0;
}

RBACRank::~RBACRank() {
	groups.clear();
}
