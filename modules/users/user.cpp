#include "user.h"

#include "core/hash/sha256.h"
#include "core/html/form_validator.h"
#include "core/html/html_builder.h"
#include "core/http/cookie.h"
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
	//name = p_name;

	//load();
}

void User::load(const int p_id) {
	id = p_id;

	load();
}

void User::changed() {
	save();
}

void User::update() {
}

bool User::check_password(const std::string &p_password) {
	return hash_password(p_password) == password_hash;
}

void User::create_password(const std::string &p_password) {
	//todo improve a bit
	pre_salt = hash_password(nameui + emailui);
	post_salt = hash_password(emailui + nameui);

	password_hash = hash_password(p_password);
}

std::string User::hash_password(const std::string &p_password) {
	SHA256 *s = SHA256::get();

	std::string p = pre_salt + p_password + post_salt;

	std::string c = s->compute(p);

	delete s;

	return c;
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

	_mutex.lock();

	for (int i = 0; i < sessions.size(); ++i) {
		HTTPSession *session = new HTTPSession();
		session->session_id = sessions[i];
		session->add_object("user", this);

		sm->add_session(session);
	}

	_mutex.unlock();
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

	_mutex.lock();

	for (int i = 0; i < sessions.size(); ++i) {
		sm->delete_session(sessions[i]);
	}

	_mutex.unlock();
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

		//this is probbaly not needed
		//it's ok for now as I need to test the validators more
		std::vector<std::string> errors;
		_login_validator->validate(request, &errors);
		for (int i = 0; i < errors.size(); ++i) {
			error_str += errors[i] + "<br>";
		}
		//not needed end

		uname_val = request->get_parameter("username");
		pass_val = request->get_parameter("password");

		User *user = UserManager::get_singleton()->get_user(uname_val);

		if (user) {
			if (!user->check_password(pass_val)) {
				error_str += "Invalid username or password!";
			} else {
				HTTPSession *session = request->get_or_create_session();

				session->add_object("user", user);

				user->_mutex.lock();
				user->sessions.push_back(session->session_id);
				user->_mutex.unlock();

				user->save();

				request->add_cookie(::Cookie("session_id", session->session_id));

				//todo implement redirect!

				request->body += "Login Success!<br>";

				request->compile_and_send_body();

				return;
			}
		} else {
			error_str += "Invalid username or password!";
		}
	}

	HTMLBuilder b;

	b.w("Login");
	b.br();

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
	std::string error_str = "";

	std::string uname_val = "";
	std::string email_val = "";
	std::string pass_val = "";
	std::string pass_check_val = "";

	if (request->get_method() == HTTP_METHOD_POST) {

		std::vector<std::string> errors;

		_registration_validator->validate(request, &errors);

		for (int i = 0; i < errors.size(); ++i) {
			error_str += errors[i] + "<br>";
		}

		uname_val = request->get_parameter("username");
		email_val = request->get_parameter("email");
		pass_val = request->get_parameter("password");
		pass_check_val = request->get_parameter("password_check");

		//todo username length etc check
		//todo pw length etc check

		User *user = UserManager::get_singleton()->get_user(uname_val);

		if (user) {
			error_str += "Username already taken!<br>";
		}

		UserManager *um = UserManager::get_singleton();

		bool email_found = false;

		for (int i = 0; i < um->_users_vec.size(); ++i) {
			User *u = um->_users_vec[i];

			if (!u) {
				continue;
			}

			if (u->emailui == email_val) {
				email_found = true;
				break;
			}
		}

		if (email_found) {
			error_str += "Email already in use!<br>";
		}

		if (pass_val != pass_check_val) {
			error_str += "The passwords did not match!<br>";
		}

		if (error_str.size() == 0) {
			user = UserManager::get_singleton()->create_user();

			user->nameui = uname_val;
			user->emailui = email_val;
			//todo
			user->rank = 1;
			user->create_password(pass_val);
			user->save();

			UserManager::get_singleton()->add_user(user);

			HTMLBuilder b;

			b.div()->cls("success");
			b.w("Registration successful! You can now log in!");
			b.br();
			b.a()->href("/user/login");
			b.w(">> Login <<");
			b.ca();
			b.cdiv();

			request->body += b.result;

			request->compile_and_send_body();
			return;
		}
	}

	HTMLBuilder b;

	b.w("Registration");
	b.br();

	if (error_str.size() != 0) {
		b.div()->cls("error");

		b.w(error_str);

		b.cdiv();
	}

	b.div()->cls("register");

	//todo href path helper
	b.form()->method("POST")->href("/user/register");

	b.w("Username");
	b.br();
	b.input()->type("text")->name("username")->value(uname_val);
	b.cinput();
	b.br();

	b.w("Email");
	b.br();
	b.input()->type("email")->name("email")->value(email_val);
	b.cinput();
	b.br();

	b.w("Password");
	b.br();
	b.input()->type("password")->name("password");
	b.cinput();
	b.br();

	b.w("Password again");
	b.br();
	b.input()->type("password")->name("password_check");
	b.cinput();
	b.br();

	b.input()->type("submit")->value("Register");
	b.cinput();
	b.cform();

	b.cdiv();

	request->body += b.result;

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
	} else if (segment == "login") {
		request->body += "You are already logged in.";

		request->compile_and_send_body();
	} else if (segment == "register") {
		request->body += "You are already logged in.";

		request->compile_and_send_body();
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
	request->remove_cookie("session_id");

	_mutex.lock();

	for (int i = 0; i < sessions.size(); ++i) {
		if (sessions[i] == request->session->session_id) {
			sessions[i] = sessions[sessions.size() - 1];
			sessions.pop_back();
		}
	}

	_mutex.unlock();

	save();

	SessionManager::get_singleton()->delete_session(request->session->session_id);
	request->session = nullptr;

	HTMLBuilder b;
	b.w("Logout successful!");
	request->body += b.result;

	request->compile_and_send_body();
}
void User::handle_delete_request(Request *request) {
	request->body += "handle_delete_request";

	request->compile_and_send_body();
}

void User::create_validators() {
	if (!_login_validator) {
		//Login
		_login_validator = new FormValidator();

		_login_validator->new_field("username", "Username")->need_to_exist()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
		FormField *pw = _login_validator->new_field("password", "Password");
		pw->need_to_exist();
		pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
		pw->need_minimum_length(5);
	}

	if (!_registration_validator) {
		//Registration
		_registration_validator = new FormValidator();

		_registration_validator->new_field("username", "Username")->need_to_exist()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
		_registration_validator->new_field("email", "Email")->need_to_exist()->need_to_be_email();

		FormField *pw = _registration_validator->new_field("password", "Password");
		pw->need_to_exist();
		pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
		pw->need_minimum_length(5);

		_registration_validator->new_field("password_check", "Password check")->need_to_match("password");

		_registration_validator->new_field("email", "Email")->need_to_exist()->need_to_be_email();
	}
}

User::User() :
		Object() {

	id = 0;
	rank = 0;
	banned = false;
	locked = false;

	_login_validator = nullptr;
	_registration_validator = nullptr;

	create_validators();
}

User::~User() {
	unregister_sessions();
}

FormValidator *User::_login_validator = nullptr;
FormValidator *User::_registration_validator = nullptr;