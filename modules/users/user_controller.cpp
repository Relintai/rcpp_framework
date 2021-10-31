#include "user_controller.h"

#include "core/html/form_validator.h"
#include "core/html/html_builder.h"
#include "core/http/cookie.h"
#include "core/http/http_session.h"
#include "core/http/request.h"
#include "core/http/session_manager.h"
#include "user_model.h"

void UserController::handle_request_default(Request *request) {
	if (request->session) {
		Ref<User> u = request->reference_data["user"];

		if (u.is_valid()) {
			handle_request(u, request);

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

void UserController::handle_login_request_default(Request *request) {
	LoginRequestData data;

	if (request->get_method() == HTTP_METHOD_POST) {

		//this is probbaly not needed
		//it's ok for now as I need to test the validators more
		std::vector<std::string> errors;
		_login_validator->validate(request, &errors);
		for (int i = 0; i < errors.size(); ++i) {
			data.error_str += errors[i] + "<br>";
		}
		//not needed end

		data.uname_val = request->get_parameter("username");
		data.pass_val = request->get_parameter("password");

		Ref<User> user = UserModel::get_singleton()->get_user(data.uname_val);

		if (user.is_valid()) {
			if (!UserModel::get_singleton()->check_password(user, data.pass_val)) {
				data.error_str += "Invalid username or password!";
			} else {
				HTTPSession *session = request->get_or_create_session();

				session->add_int("user_id", user->id);
				SessionManager::get_singleton()->save_session(session);

				::Cookie c = ::Cookie("session_id", session->session_id);
				c.path = "/";

				request->add_cookie(c);

				render_login_success(request);

				return;
			}
		} else {
			data.error_str += "Invalid username or password!";
		}
	}

	render_login_request_default(request, &data);
}

void UserController::render_login_request_default(Request *request, LoginRequestData *data) {
	HTMLBuilder b;

	b.w("Login");
	b.br();

	{
		if (data->error_str.size() != 0) {
			b.div()->cls("error");

			b.w(data->error_str);

			b.cdiv();
		}
	}

	b.div()->cls("login");
	{

		//todo href path helper
		b.form()->method("POST")->href("/user/login");
		{
			b.w("Username");
			b.br();
			b.input()->type("text")->name("username")->value(data->uname_val);
			b.cinput();
			b.br();

			b.w("Password");
			b.br();
			b.input()->type("password")->name("password");
			b.cinput();
			b.br();

			b.input()->type("submit")->value("Send");
			b.cinput();
		}
		b.cform();
	}
	b.cdiv();

	request->body += b.result;

	request->compile_and_send_body();
}

void UserController::handle_register_request_default(Request *request) {
	RegisterRequestData data;

	if (request->get_method() == HTTP_METHOD_POST) {

		std::vector<std::string> errors;

		_registration_validator->validate(request, &errors);

		for (int i = 0; i < errors.size(); ++i) {
			data.error_str += errors[i] + "<br>";
		}

		data.uname_val = request->get_parameter("username");
		data.email_val = request->get_parameter("email");
		data.pass_val = request->get_parameter("password");
		data.pass_check_val = request->get_parameter("password_check");

		//todo username length etc check
		//todo pw length etc check

		if (UserModel::get_singleton()->is_username_taken(data.uname_val)) {
			data.error_str += "Username already taken!<br>";
		}

		if (UserModel::get_singleton()->is_email_taken(data.email_val)) {
			data.error_str += "Email already in use!<br>";
		}

		if (data.pass_val != data.pass_check_val) {
			data.error_str += "The passwords did not match!<br>";
		}

		if (data.error_str.size() == 0) {
			Ref<User> user;
			user.instance();

			user->name_user_input = data.uname_val;
			user->email_user_input = data.email_val;
			//todo
			user->rank = 1;
			UserModel::get_singleton()->create_password(user, data.pass_val);
			UserModel::get_singleton()->save_user(user);

			render_register_success(request);
			return;
		}
	}

	render_register_request_default(request, &data);
}

void UserController::render_register_success(Request *request) {
	HTMLBuilder b;

	b.div()->cls("success");
	{
		b.w("Registration successful! You can now log in!");
		b.br();
		b.a()->href("/user/login");
		b.w(">> Login <<");
		b.ca();
	}
	b.cdiv();

	request->body += b.result;

	request->compile_and_send_body();
}

void UserController::render_register_request_default(Request *request, RegisterRequestData *data) {
	HTMLBuilder b;

	b.w("Registration");
	b.br();

	{
		if (data->error_str.size() != 0) {
			b.div()->cls("error");

			b.w(data->error_str);

			b.cdiv();
		}
	}

	b.div()->cls("register");
	{
		//todo href path helper
		b.form()->method("POST")->href("/user/register");
		{
			b.w("Username");
			b.br();
			b.input()->type("text")->name("username")->value(data->uname_val);
			b.cinput();
			b.br();

			b.w("Email");
			b.br();
			b.input()->type("email")->name("email")->value(data->email_val);
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
		}
		b.cform();
	}
	b.cdiv();

	request->body += b.result;

	request->compile_and_send_body();
}

void UserController::render_already_logged_in_error(Request *request) {
	request->body += "You are already logged in.";

	request->compile_and_send_body();
}

void UserController::render_login_success(Request *request) {
	request->body = "Login Success!<br>";

	//request->compile_and_send_body();
	request->send_redirect("/user/settings");
}

void UserController::handle_request(Ref<User> &user, Request *request) {
	const std::string &segment = request->get_current_path_segment();

	if (segment == "") {
		handle_main_page_request(user, request);
	} else if (segment == "settings") {
		handle_settings_request(user, request);
	} else if (segment == "password_reset") {
		handle_password_reset_request(user, request);
	} else if (segment == "logout") {
		handle_logout_request(user, request);
	} else if (segment == "delete") {
		handle_delete_request(user, request);
	} else if (segment == "login") {
		render_already_logged_in_error(request);
	} else if (segment == "register") {
		render_already_logged_in_error(request);
	} else {
		request->send_error(404);
	}
}

void UserController::handle_main_page_request(Ref<User> &user, Request *request) {
	request->body += "handle_main_page_request";

	request->compile_and_send_body();
}

void UserController::handle_settings_request(Ref<User> &user, Request *request) {

	SettingsRequestData data;

	if (request->get_method() == HTTP_METHOD_POST) {

		data.uname_val = request->get_parameter("username");
		data.email_val = request->get_parameter("email");
		data.pass_val = request->get_parameter("password");
		data.pass_check_val = request->get_parameter("password_check");

		bool changed = false;

		std::vector<std::string> errors;

		bool valid = _profile_validator->validate(request, &errors);

		for (int i = 0; i < errors.size(); ++i) {
			data.error_str += errors[i] + "<br>";
		}

		if (valid) {
			if (data.uname_val == user->name_user_input) {
				data.uname_val = "";
			}

			if (data.email_val == user->email_user_input) {
				data.email_val = "";
			}

			if (data.uname_val != "") {
				if (UserModel::get_singleton()->is_username_taken(data.uname_val)) {
					data.error_str += "Username already taken!<br>";
				} else {
					//todo sanitize for html special chars!
					user->name_user_input = data.uname_val;
					changed = true;
					data.uname_val = "";
				}
			}

			if (data.email_val != "") {
				if (UserModel::get_singleton()->is_email_taken(data.email_val)) {
					data.error_str += "Email already in use!<br>";
				} else {
					//todo sanitize for html special chars!
					//also send email
					user->email_user_input = data.email_val;
					changed = true;
					data.email_val = "";
				}
			}

			if (data.pass_val != "") {
				if (data.pass_val != data.pass_check_val) {
					data.error_str += "The passwords did not match!<br>";
				} else {
					UserModel::get_singleton()->create_password(user, data.pass_val);

					changed = true;
				}
			}

			if (changed) {
				UserModel::get_singleton()->save_user(user);
			}
		}
	}

	render_settings_request(user, request, &data);
}

void UserController::render_settings_request(Ref<User> &user, Request *request, SettingsRequestData *data) {
	HTMLBuilder b;

	b.w("Settings");
	b.br();

	{
		if (data->error_str.size() != 0) {
			b.div()->cls("error");

			b.w(data->error_str);

			b.cdiv();
		}
	}

	b.div()->cls("settings");
	{
		//todo href path helper
		b.form()->method("POST")->href("/user/settings");
		{
			b.w("Username");
			b.br();
			b.input()->type("text")->name("username")->placeholder(user->name_user_input)->value(data->uname_val);
			b.cinput();
			b.br();

			b.w("Email");
			b.br();
			b.input()->type("email")->name("email")->placeholder(user->email_user_input)->value(data->email_val);
			b.cinput();
			b.br();

			b.w("Password");
			b.br();
			b.input()->type("password")->placeholder("*******")->name("password");
			b.cinput();
			b.br();

			b.w("Password again");
			b.br();
			b.input()->type("password")->placeholder("*******")->name("password_check");
			b.cinput();
			b.br();

			b.input()->type("submit")->value("Save");
			b.cinput();
		}
		b.cform();
	}
	b.cdiv();

	request->body += b.result;

	request->compile_and_send_body();
}

void UserController::handle_password_reset_request(Ref<User> &user, Request *request) {
	request->body += "handle_password_reset_request";

	request->compile_and_send_body();
}

void UserController::handle_logout_request(Ref<User> &user, Request *request) {
	request->remove_cookie("session_id");

	UserModel::get_singleton()->save_user(user);

	SessionManager::get_singleton()->delete_session(request->session->session_id);
	request->session = nullptr;

	HTMLBuilder b;
	b.w("Logout successful!");
	request->body += b.result;

	request->compile_and_send_body();
}

void UserController::handle_delete_request(Ref<User> &user, Request *request) {
	request->body += "handle_delete_request";

	request->compile_and_send_body();
}

void UserController::create_validators() {
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

	if (!_profile_validator) {
		_profile_validator = new FormValidator();

		_profile_validator->new_field("username", "Username")->ignore_if_not_exists()->need_to_be_alpha_numeric()->need_minimum_length(5)->need_maximum_length(20);
		_profile_validator->new_field("email", "Email")->ignore_if_not_exists()->need_to_be_email();

		FormField *pw = _profile_validator->new_field("password", "Password");
		pw->ignore_if_not_exists();
		pw->need_to_have_lowercase_character()->need_to_have_uppercase_character();
		pw->need_minimum_length(5);

		_profile_validator->new_field("password_check", "Password check")->ignore_if_other_field_not_exists("password")->need_to_match("password");
	}
}

void UserController::user_session_setup_middleware(Object *instance, Request *request) {
	if (request->session) {
		int user_id = request->session->get_int("user_id");

		if (user_id != 0) {

			Ref<User> u = UserModel::get_singleton()->get_user(user_id);

			if (u.is_valid()) {
				request->reference_data["user"] = u;
			} else {
				//log
				request->session->remove_int("user_id");
			}
		}
	}

	request->next_stage();
}

UserController *UserController::get_singleton() {
	return _self;
}

UserController::UserController() :
		Object() {

	if (_self) {
		printf("UserController::UserController(): Error! self is not null!/n");
	}

	_self = this;

	create_validators();
}

UserController::~UserController() {
	if (_self == this) {
		_self = nullptr;
	}
}

UserController *UserController::_self = nullptr;

FormValidator *UserController::_login_validator = nullptr;
FormValidator *UserController::_registration_validator = nullptr;
FormValidator *UserController::_profile_validator = nullptr;
