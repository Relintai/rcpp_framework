#include "user.h"

#include "core/html/html_builder.h"
#include "core/http/http_session.h"
#include "core/http/request.h"
#include "core/http/session_manager.h"
#include "core/utils.h"
#include "user_manager.h"

void User::save() {
}

void User::load() {
	//unregister_sessions()
	//load_code
	//register_sessions()
}

void User::load(const std::string &p_name) {
	name = p_name;

	load();
}

void User::changed() {
	save();
}

void User::update() {
}

void User::register_sessions() {
	if (sessions.size() == 0) {
		return;
	}

	SessionManager *sm = SessionManager::get_singleton();

	if (!sm) {
		printf("ERROR: UserManager::add_user SessionManager singleton is null, please allocate one!\n");
		return;
	}

	for (int i = 0; i < sessions.size(); ++i) {
		HTTPSession *session = new HTTPSession();
		session->session_id = sessions[i];
		session->add_object("user", this);

		sm->add_session(session);
	}
}

void User::unregister_sessions() {
	if (sessions.size() == 0) {
		return;
	}

	SessionManager *sm = SessionManager::get_singleton();

	if (!sm) {
		printf("ERROR: UserManager::remove_user SessionManager singleton is null, please allocate one!\n");
		return;
	}

	for (int i = 0; i < sessions.size(); ++i) {
		sm->delete_session(sessions[i]);
	}
}

void User::handle_request_default(Request *request) {
	if (request->session) {
		User *u = dynamic_cast<User *>(request->session->get_object("user"));

		if (u) {
			u->handle_request(request);

			return;
		}
	}

	const std::string &segment = request->get_current_path_segment();

	if (segment == "") {
		handle_login_request_default(request);

		return;
	} else if (segment == "login") {
		handle_login_request_default(request);

		return;
	} else if (segment == "register") {
		handle_register_request_default(request);

		return;
	}

	handle_login_request_default(request);
}

void User::handle_login_request_default(Request *request) {
	std::string error_str = "";

	std::string uname_val = "";
	std::string pass_val = "";

	if (request->get_method() == HTTP_METHOD_POST) {
		uname_val = request->get_parameter("username");
		pass_val = request->get_parameter("password");

		User *user = UserManager::get_singleton()->get_user(uname_val);

		if (user) {
			//todo
		} else {
			error_str = "Invalid username or password!";
		}
	}

	HTMLBuilder b;

	if (error_str.size() != 0) {
		b.div()->cls("error");

		b.w(error_str);

		b.cdiv();
	}

	b.div()->cls("login");

	//todo href path helper
	b.form()->method("POST")->href("/user/login");
	b.w("Username");
	b.br();
	b.input()->type("text")->name("username")->value(uname_val);
	b.cinput();
	b.br();
	b.w("Password");
	b.br();
	b.input()->type("password")->name("password");
	b.cinput();
	b.br();
	b.input()->type("submit")->value("Send");
	b.cinput();
	b.cform();

	b.cdiv();

	request->body += b.result;

	request->compile_and_send_body();
}

void User::handle_register_request_default(Request *request) {
	request->body += "handle_register_request_default";

	request->compile_and_send_body();
}

void User::handle_request(Request *request) {
	const std::string &segment = request->get_current_path_segment();

	if (segment == "") {
		handle_main_page_request(request);
	} else if (segment == "settings") {
		handle_settings_request(request);
	} else if (segment == "password_reset") {
		handle_password_reset_request(request);
	} else if (segment == "logout") {
		handle_logout_request(request);
	} else if (segment == "delete") {
		handle_delete_request(request);
	} else {
		request->send_error(404);
	}
}

void User::handle_main_page_request(Request *request) {
	request->body += "handle_main_page_request";

	request->compile_and_send_body();
}

void User::handle_settings_request(Request *request) {
	request->body += "handle_settings_request";

	request->compile_and_send_body();
}
void User::handle_password_reset_request(Request *request) {
	request->body += "handle_password_reset_request";

	request->compile_and_send_body();
}
void User::handle_logout_request(Request *request) {
	request->body += "handle_logout_request";

	request->compile_and_send_body();
}
void User::handle_delete_request(Request *request) {
	request->body += "handle_delete_request";

	request->compile_and_send_body();
}

User::User() :
		Object() {

	id = 0;
	rank = 0;
	banned = false;
	locked = false;
}

User::~User() {
	unregister_sessions();
}
