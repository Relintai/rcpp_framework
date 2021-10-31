#include "admin_panel.h"

#include "core/html/form_validator.h"
#include "core/html/html_builder.h"
#include "core/http/cookie.h"
#include "core/http/http_session.h"
#include "core/http/request.h"
#include "core/http/session_manager.h"

void AdminPanel::handle_request_main(Request *request) {
}

void AdminPanel::create_validators() {
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
