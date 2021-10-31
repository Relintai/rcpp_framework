#include "admin_panel.h"

#include "core/html/form_validator.h"
#include "core/html/html_builder.h"
#include "core/http/cookie.h"
#include "core/http/http_session.h"
#include "core/http/request.h"
#include "core/http/session_manager.h"

#include "admin_controller.h"

void AdminPanel::handle_request_main(Request *request) {
	//todo check permissions

	String seg = request->get_current_path_segment();

	if (seg == "") {
		render_admin_panel_list(request);
		return;
	}

	for (int i = 0; i < _controllers.size(); ++i) {
		AdminPanelSection &s = _controllers[i];

		if (s.section_url == seg) {
			AdminController *c = s.controller;

			request->push_path();

			if (c->admin_full_render()) {
				c->admin_handle_request_main(request);
				return;
			}

			render_controller_panel(request, c);

			//request->pop_path();

			return;
		}
	}

	request->send_error(404);
}

void AdminPanel::render_admin_panel_list(Request *request) {
	//set up headers

	String rootlink = request->get_url_root();

	HTMLBuilder b;

	Vector<AdminSectionLinkInfo> links;

	b.div()->cls("content");

	for (int i = 0; i < _controllers.size(); ++i) {
		b.div()->cls("section");

		AdminPanelSection &s = _controllers[i];

		s.controller->admin_add_section_links(&links);

		for (int j = 0; j < links.size(); ++j) {
			AdminSectionLinkInfo &li = links[j];

			b.a()->href(rootlink + li.link);
			b.w(li.name);
			b.ca();
		}

		links.clear();

		b.cdiv();
	}

	b.cdiv();

	//set up footers

	request->body += b.result;
	request->compile_and_send_body();
}

void AdminPanel::render_controller_panel(Request *request, AdminController *controller) {
	//set up headers
	controller->admin_handle_request_main(request);
	//set up footers
	request->compile_and_send_body();
}

void AdminPanel::register_admin_controller(const String &section, AdminController *controller) {
	AdminPanelSection sec;

	sec.section_url = section;
	sec.controller = controller;

	_controllers.push_back(sec);
}

void AdminPanel::clear() {
	_controllers.clear();
}

AdminPanel *AdminPanel::get_singleton() {
	return _self;
}

AdminPanel::AdminPanel() :
		Controller() {

	if (_self) {
		printf("AdminPanel::AdminPanel(): Error! self is not null!/n");
	}

	_self = this;
}

AdminPanel::~AdminPanel() {
	if (_self == this) {
		_self = nullptr;
	}
}

AdminPanel *AdminPanel::_self = nullptr;
