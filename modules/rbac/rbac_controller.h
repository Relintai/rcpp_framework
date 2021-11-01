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