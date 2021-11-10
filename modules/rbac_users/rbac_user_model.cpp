#include "rbac_user_model.h"

#include "modules/rbac_users/rbac_user.h"

#include "modules/rbac/rbac_controller.h"

Ref<User> RBACUserModel::get_user(const int id) {
	Ref<RBACUser> u = UserModel::get_user(id);

	if (u.is_valid()) {
		u->rbac_rank = RBACController::get_singleton()->get_rank(u->rank);
	}

	return u;
}
Ref<User> RBACUserModel::get_user(const String &user_name_input) {
	Ref<RBACUser> u = UserModel::get_user(user_name_input);

	if (u.is_valid()) {
		u->rbac_rank = RBACController::get_singleton()->get_rank(u->rank);
	}

	return u;
}

Vector<Ref<User> > RBACUserModel::get_all() {
	Vector<Ref<User> > users = UserModel::get_all();

	for (int i = 0; i < users.size(); ++i) {
		Ref<RBACUser> u = users[i];

		if (u.is_valid()) {
			u->rbac_rank = RBACController::get_singleton()->get_rank(u->rank);
		}
	}

	return users;
}

Ref<User> RBACUserModel::create_user() {
	Ref<RBACUser> u;
	u.instance();

	u->rank = RBACController::get_singleton()->get_default_rank_id();
	u->rbac_rank = RBACController::get_singleton()->get_rank(u->rank);

	return u;
}

RBACUserModel::RBACUserModel() :
		UserModel() {
}

RBACUserModel::~RBACUserModel() {
}
