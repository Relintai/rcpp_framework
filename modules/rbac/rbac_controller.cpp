#include "rbac_controller.h"

#include "core/html/form_validator.h"
#include "core/html/html_builder.h"
#include "core/http/cookie.h"
#include "core/http/http_session.h"
#include "core/http/request.h"
#include "core/http/session_manager.h"
#include "rbac_model.h"

void RBACController::handle_request_main(Request *request) {
}

void RBACController::create_validators() {
}

void RBACController::admin_handle_request_main(Request *request) {
	String seg = request->get_current_path_segment();

	if (seg == "") {
		admin_render_rank_list(request);
		return;
	}

}

void RBACController::admin_render_rank_list(Request *request) {
	HTMLBuilder b;

	b.h3()->f()->w("RBAC Editor")->ch3();
	b.h4()->f()->a()->href(request->get_url_root_parent())->f()->w("<- Back")->ca()->ch4();

	for (std::map<int, Ref<RBACRank> >::iterator p = _permissions.begin(); p != _permissions.end(); p++) {
		Ref<RBACRank> r = p->second;

		if (!r.is_valid()) {
			continue;
		}

		b.div()->cls("row");
		{
			b.a()->href(request->get_url_root("edit_permissions/") + String::num(r->id));
			b.w("Id: ")->w(String::num(r->id))->w(", Name: ")->w(r->name)->w(", Name Internal: ")->w(r->name_internal);
			b.ca();
			
			b.a()->href(request->get_url_root("edit_rank/") + String::num(r->id));
			b.w("Edit Names");
			b.ca();
		}
		b.cdiv();
	}

	b.a()->href(request->get_url_root("new_rank"));
	b.w("New Rank");
	b.ca();

	request->body += b.result;
}

void RBACController::admin_render_rank_editor(Request *request) {

}


String RBACController::admin_get_section_name() {
	return "Role Based Access Control";
}

void RBACController::admin_add_section_links(Vector<AdminSectionLinkInfo> *links) {
	links->push_back(AdminSectionLinkInfo("Editor", ""));
}

void RBACController::initialize() {
	_permissions = RBACModel::get_singleton()->load_permissions();
}

RBACController *RBACController::get_singleton() {
	return _self;
}

RBACController::RBACController() :
		AdminController() {

	if (_self) {
		printf("RBACController::RBACController(): Error! self is not null!/n");
	}

	_self = this;
}

RBACController::~RBACController() {
	if (_self == this) {
		_self = nullptr;
	}
}

RBACController *RBACController::_self = nullptr;
