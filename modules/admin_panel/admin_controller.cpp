#include "admin_controller.h"

#include "core/html/form_validator.h"
#include "core/html/html_builder.h"
#include "core/http/cookie.h"
#include "core/http/http_session.h"
#include "core/http/request.h"
#include "core/http/session_manager.h"

void AdminController::admin_handle_request_main(Request *request) {

}

void AdminController::admin_add_section_links(Vector<AdminSectionLinkInfo> *links) {
	
}

bool AdminController::admin_full_render() {
	return false;
}

AdminController::AdminController() :
		Controller() {

}

AdminController::~AdminController() {
}
