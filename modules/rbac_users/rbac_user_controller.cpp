#include "rbac_user_controller.h"

#include "core/http/http_session.h"
#include "core/http/request.h"

#include "rbac_user_model.h"

#include "modules/rbac/rbac_controller.h"
#include "modules/rbac/rbac_default_permissions.h"
#include "rbac_user.h"

void RBACUserController::rbac_user_session_setup_middleware(Object *instance, Request *request) {
	if (request->session) {
		int user_id = request->session->get_int("user_id");

		if (user_id != 0) {

			Ref<RBACUser> u = UserModel::get_singleton()->get_user(user_id);

			if (u.is_valid()) {
				request->reference_data["user"] = u;
			} else {
				//log
				request->session->remove_int("user_id");
			}
		}
	}

	request->next_stage();
}

void RBACUserController::rbac_default_user_session_middleware(Object *instance, Request *request) {
	//note: add a new file handler middleware func, so basic file handling is easy to set up before this

	Ref<RBACRank> rank;

	if (request->session) {
		int user_id = request->session->get_int("user_id");

		if (user_id != 0) {

			Ref<RBACUser> u = UserModel::get_singleton()->get_user(user_id);

			if (u.is_valid()) {
				rank = u->rbac_rank;

				request->reference_data["user"] = u;
			} else {
				//log
				request->session->remove_int("user_id");
			}
		}
	}

	if (!rank.is_valid()) {
		rank = RBACController::get_singleton()->get_default_rank();

		if (!rank.is_valid()) {
			if (RBACController::get_singleton()->continue_on_missing_default_rank()) {
				RLOG_ERR("RBACController::get_singleton()->get_default_rank() has not been set up properly!!! Continuing!");
				request->next_stage();
			} else {
				RLOG_ERR("RBACController::get_singleton()->get_default_rank() has not been set up properly!!! Sending 404!");
				request->send_error(404);
			}

			return;
		}
	}

	if (!rank->has_permission(request, RBAC_PERMISSION_READ)) {
		if (rank->has_rank_permission(RBAC_RANK_PERMISSION_USE_REDIRECT)) {
			//Note this can make the webapp prone to enumerations, if not done correctly
			//e.g. redirect from /admin, but sending 404 on a non existing uri, which does not have
			//a special rbac entry
			request->send_redirect(RBACController::get_singleton()->get_redirect_url());
			return;
		}

		request->send_error(404);
		return;
	}

	request->next_stage();
}

RBACUserController::RBACUserController() :
		UserController() {
}

RBACUserController::~RBACUserController() {
}