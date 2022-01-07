#include "admin_node.h"

#include "core/html/form_validator.h"
#include "core/html/html_builder.h"
#include "core/http/cookie.h"
#include "core/http/http_session.h"
#include "core/http/request.h"
#include "core/http/session_manager.h"

void AdminNode::admin_handle_request_main(Request *request) {

}

String AdminNode::admin_get_section_name() {
	return "";
}

void AdminNode::admin_add_section_links(Vector<AdminSectionLinkInfo> *links) {
	
}

bool AdminNode::admin_full_render() {
	return false;
}

AdminNode::AdminNode() :
		WebNode() {

}

AdminNode::~AdminNode() {
}
