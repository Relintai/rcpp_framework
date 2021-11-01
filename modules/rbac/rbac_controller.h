#ifndef RBAC_CONTROLLER_H
#define RBAC_CONTROLLER_H

#include <map>

#include "modules/admin_panel/admin_controller.h"

#include "core/containers/vector.h"
#include "core/string.h"

#include "rbac_permission.h"
#include "rbac_rank.h"

class Request;
class FormValidator;

class RBACController : public AdminController {
	RCPP_OBJECT(RBACController, AdminController);

public:
	void handle_request_main(Request *request);
	void create_validators();

	void admin_handle_request_main(Request *request);
	String admin_get_section_name();
	void admin_add_section_links(Vector<AdminSectionLinkInfo> *links);

	struct RBACAdminRankViewData {
		Ref<RBACRank> rank;
		Vector<String> messages;

		int id = 0;
		String name = "";
		String name_internal = "";
		String settings = "";
		int rank_permissions = 0;
	};

	void admin_handle_new_rank(Request *request);
	void admin_handle_edit_rank(Request *request);
	void render_rank_view(Request *request, RBACAdminRankViewData *data);

	void admin_render_rank_list(Request *request);
	void admin_render_rank_editor(Request *request);

	void initialize();

	static RBACController *get_singleton();

	RBACController();
	~RBACController();

protected:
	static RBACController *_self;

	std::map<int, Ref<RBACRank> > _permissions;
};

#endif