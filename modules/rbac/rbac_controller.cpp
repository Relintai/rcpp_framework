#include "rbac_controller.h"

#include "core/error_macros.h"

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
	} else if (seg == "new_rank") {
		request->push_path();

		admin_handle_new_rank(request);
	} else if (seg == "edit_rank") {
		request->push_path();

		admin_handle_edit_rank(request);
	} else if (seg == "permission_editor") {
		request->push_path();

		admin_permission_editor(request);
	}
}

void RBACController::admin_handle_new_rank(Request *request) {

	if (request->get_method() == HTTP_METHOD_POST) {
		Ref<RBACRank> rank;
		rank.instance();

		rank->name = request->get_parameter("name");
		rank->name_internal = request->get_parameter("name_internal");
		rank->settings = request->get_parameter("settings");
		rank->rank_permissions = request->get_parameter("rank_permissions").to_int();

		RBACModel::get_singleton()->save_rank(rank);

		_permissions[rank->id] = rank;

		request->send_redirect(request->get_url_root_parent() + "edit_rank/" + String::num(rank->id));
		return;
	}

	RBACAdminRankViewData data;
	render_rank_view(request, &data);
}

void RBACController::admin_handle_edit_rank(Request *request) {
	String seg = request->get_current_path_segment();

	//check whether it's numeric
	//if (!seg.is)

	int id = seg.to_int();

	if (id == 0) {
		RLOG_MSG("RBACController::admin_handle_edit_rank: id == 0!\n");
		request->send_redirect(request->get_url_root_parent());
		return;
	}

	Ref<RBACRank> rank = _permissions[id];

	if (!rank.is_valid()) {
		RLOG_MSG("RBACController::admin_handle_edit_rank: !rank.is_valid()\n");
		request->send_redirect(request->get_url_root_parent());
		return;
	}

	RBACAdminRankViewData data;
	data.rank = rank;

	if (request->get_method() == HTTP_METHOD_POST) {
		rank->name = request->get_parameter("name");
		rank->name_internal = request->get_parameter("name_internal");
		rank->settings = request->get_parameter("settings");
		rank->rank_permissions = request->get_parameter("rank_permissions").to_int();

		RBACModel::get_singleton()->save_rank(rank);

		data.messages.push_back("Save Success!");
	}

	render_rank_view(request, &data);
}

void RBACController::render_rank_view(Request *request, RBACAdminRankViewData *data) {
	int id = 0;
	String name = "";
	String name_internal = "";
	String settings = "";
	int rank_permissions = 0;

	if (data->rank.is_valid()) {
		id = data->rank->id;
		name = data->rank->name;
		name_internal = data->rank->name_internal;
		settings = data->rank->settings;
		rank_permissions = data->rank->rank_permissions;
	}

	HTMLBuilder b;

	b.h4()->f()->a()->href(request->get_url_root_parent())->f()->w("<- Back")->ca()->ch4();
	b.h4()->f()->w("RBAC Editor")->ch4();

	b.div()->cls("messages");
	for (int i = 0; i < data->messages.size(); ++i) {
		b.w(data->messages[i])->br();
	}
	b.cdiv();

	b.form()->method("POST")->action(request->get_url_root() + String::num(id));
	{
		//b.input()->type("hidden")->name("id")->value(String::num(id))->f()->cinput();
		b.w("Name:")->br();
		b.input()->type("text")->name("name")->value(name)->f()->br();
		b.w("Name (Internal):")->br();
		b.input()->type("text")->name("name_internal")->value(name_internal)->f()->cinput()->br();
		b.w("Custom Settings:")->br();
		b.input()->type("text")->name("settings")->value(settings)->f()->cinput()->br();

		//todo rank_permissions (checkboxes + register api)

		b.input()->type("submit")->value("Save");
	}
	b.cform();

	request->body += b.result;
}

void RBACController::admin_permission_editor(Request *request) {

	String seg = request->get_current_path_segment();

	//check whether it's numeric
	//if (!seg.is)

	int id = seg.to_int();

	if (id == 0) {
		RLOG_MSG("RBACController::admin_permission_editor: id == 0!\n");
		request->send_redirect(request->get_url_root_parent());
		return;
	}

	Ref<RBACRank> rank = _permissions[id];

	if (!rank.is_valid()) {
		RLOG_MSG("RBACController::admin_permission_editor: !rank.is_valid()\n");
		request->send_redirect(request->get_url_root_parent());
		return;
	}

	RBACAdminEditPermissionView data;
	data.rank = rank;

	request->push_path();

	String segn = request->get_current_path_segment();

	if (segn == "new") {
		request->push_path();

		if (request->get_method() == HTTP_METHOD_POST) {
			if (admin_process_permission_editor_entry_edit_create_post(request, &data)) {
				return;
			}
		}

		admin_render_permission_editor_entry_edit_create_view(request, &data);
		return;
	}

	admin_render_permission_editor_main_view(request, &data);
}

void RBACController::admin_render_permission_editor_main_view(Request *request, RBACAdminEditPermissionView *data) {
	HTMLBuilder b;

	Ref<RBACRank> rank = data->rank;

	b.h4()->f()->a()->href(request->get_url_root_parent(2))->f()->w("<- Back")->ca()->ch4();
	b.h4()->f()->w("RBAC Editor")->ch4();

	b.div()->cls("heading");
	{
		b.w("[ Id ]: ")->wn(rank->id)->w(", [ Name ]: ")->w(rank->name)->w(", [ Name Internal ]: ")->w(rank->name_internal);
	}
	b.cdiv();

	b.br();

	for (int i = 0; i < rank->permissions.size(); ++i) {
		Ref<RBACPermission> perm = rank->permissions[i];

		if (!perm.is_valid()) {
			RLOG_ERR("RBACController::admin_render_permission_editor_main_view: !perm.is_valid()\n");
			continue;
		}

		b.div()->cls("row");
		{
			b.w("-- Rank: [ Id ]: ")->wn(perm->id)->w(", [ Rank Id ]: ")->wn(perm->rank_id)->w(", [ Name ]: ")->w(perm->name);
			b.w(" [ URL ]: ")->w(perm->url)->w(", [ Revoke ]: ")->wbn(perm->revoke)->w(", [ Sort Order ]: ")->wn(perm->sort_order);
			b.w(" [ Permissions ]: ")->wn(perm->permissions);
		}
		b.cdiv();
	}

	b.br();

	b.a()->href(request->get_url_root("new"));
	b.w("New Permission");
	b.ca();

	request->body += b.result;
}

void RBACController::admin_render_permission_editor_entry_edit_create_view(Request *request, RBACAdminEditPermissionView *data) {
	HTMLBuilder b;

	Ref<RBACRank> rank = data->rank;
	Ref<RBACPermission> perm = data->permission;

	String name;
	String url;
	bool revoke = false;
	int sort_order = 0;
	int permissions = 0;

	if (perm.is_valid()) {
		name = perm->name;
		url = perm->url;
		revoke = perm->revoke;
		sort_order = perm->sort_order;
		permissions = perm->permissions;
	}

	b.h4()->f()->a()->href(request->get_url_root_parent())->f()->w("<- Back")->ca()->ch4();
	b.h4()->f()->w("RBAC Editor")->ch4();
	b.br();

	b.div()->cls("messages");
	for (int i = 0; i < data->messages.size(); ++i) {
		b.w(data->messages[i])->br();
	}
	b.cdiv();
	b.br();

	b.div()->cls("heading");
	{
		b.w("Rank: [ Id ]: ")->wn(rank->id)->w(", [ Name ]: ")->w(rank->name)->w(", [ Name Internal ]: ")->w(rank->name_internal);
	}
	b.cdiv();
	b.br();

	b.form()->method("POST")->action(request->get_url_root());
	{
		b.w("Name:")->br();
		b.input()->type("text")->name("name")->value(name)->f()->br();
		b.w("URL:")->br();
		b.input()->type("text")->name("url")->value(url)->f()->cinput()->br();
		b.w("Revoke:")->br();
		b.input()->type("checkbox")->name("revoke")->value("revoke")->checked(revoke)->f()->cinput()->br();

		//todo permissions (checkboxes + register api)

		b.input()->type("submit")->value("Save");
	}
	b.cform();

	request->body += b.result;
}

bool RBACController::admin_process_permission_editor_entry_edit_create_post(Request *request, RBACAdminEditPermissionView *data) {
	Ref<RBACRank> rank = data->rank;

	Ref<RBACPermission> perm = data->permission;

	if (!perm.is_valid()) {
		perm.instance();

		perm->rank_id = rank->id;

		if (rank->permissions.size() > 0) {
			Ref<RBACPermission> p = rank->permissions[rank->permissions.size() - 1];

			perm->sort_order = p->sort_order + 1;
		}
	}

	perm->name = request->get_parameter("name");
	perm->url = request->get_parameter("url");
	perm->revoke = request->get_parameter("settings").to_int();
	perm->permissions = request->get_parameter("permissions").to_int();

	//set this up in the form by default
	//perm->sort_order = request->get_parameter("sort_order").to_int();

	RBACModel::get_singleton()->save_permission(perm);

	if (perm->id == 0) {
		RLOG_ERR("RBACController::admin_process_permission_editor_entry_edit_create_post: perm->id == 0!\n");
	}

	rank->permissions.push_back(perm);

	request->send_redirect(request->get_url_root_parent());

	return true;
}

void RBACController::admin_render_rank_list(Request *request) {
	HTMLBuilder b;

	b.h4()->f()->a()->href(request->get_url_root_parent())->f()->w("<- Back")->ca()->ch4();
	b.h4()->f()->w("RBAC Editor")->ch4();

	for (std::map<int, Ref<RBACRank> >::iterator p = _permissions.begin(); p != _permissions.end(); p++) {
		Ref<RBACRank> r = p->second;

		if (!r.is_valid()) {
			continue;
		}

		b.div()->cls("row");
		{
			b.a()->href(request->get_url_root("permission_editor/") + String::num(r->id));
			b.w("[ Id ]: ")->wn(r->id)->w(", [ Name ]: ")->w(r->name)->w(", [ Name Internal ]: ")->w(r->name_internal);
			b.ca();

			b.w(" - ");

			b.a()->href(request->get_url_root("edit_rank/") + String::num(r->id));
			b.w("[ Edit Names ]");
			b.ca();
		}
		b.cdiv();
	}

	b.br();

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
