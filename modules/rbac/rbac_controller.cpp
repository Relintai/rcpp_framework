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

}

void RBACController::admin_add_section_links(Vector<AdminSectionLinkInfo> *links) {
	links->push_back(AdminSectionLinkInfo("Test", "test"));
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
