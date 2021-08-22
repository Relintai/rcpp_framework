#include "user.h"

#include "rapidjson/filewritestream.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>
#include "rapidjson/document.h"
#include "user_manager.h"
#include <tinydir/tinydir.h>
#include <cstdio>

#include "core/hash/sha256.h"
#include "core/html/form_validator.h"
#include "core/html/html_builder.h"
#include "core/http/cookie.h"
#include "core/http/http_session.h"
#include "core/http/request.h"
#include "core/http/session_manager.h"
#include "core/utils.h"
#include "user_manager.h"

std::string User::get_name_ui() {
	return _nameui;
}
void User::set_name_ui(const std::string &value) {
	_nameui = value;
}

std::string User::get_email_ui() {
	return _emailui;
}
void User::set_email_ui(const std::string &value) {
	_emailui = value;
}

int User::get_rank() {
	return _rank;
}
void User::set_rank(const int value) {
	_rank = value;
}

std::string User::get_pre_salt() {
	return _pre_salt;
}
void User::set_pre_salt(const std::string &value) {
	_pre_salt = value;
}

std::string User::get_post_salt() {
	return _post_salt;
}
void User::set_post_salt(const std::string &value) {
	_post_salt = value;
}

std::string User::get_password_hash() {
	return _password_hash;
}
void User::set_password_hash(const std::string &value) {
	_password_hash = value;
}

bool User::get_banned() {
	return _banned;
}
void User::set_banned(const bool value) {
	_banned = value;
}

std::vector<std::string> User::get_sessions() {
	return _sessions;
}
void User::set_sessions(const std::vector<std::string> &value) {
	_sessions = value;
}

std::string User::get_password_reset_token() {
	return _password_reset_token;
}
void User::set_password_reset_token(const std::string &value) {
	_password_reset_token = value;
}

bool User::get_locked() {
	return _locked;
}
void User::set_locked(const bool value) {
	_locked = value;
}

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
	set_id(p_id);

	load();
}

void User::changed() {
	save();
}

void User::update() {
}

bool User::check_password(const std::string &p_password) {
	return hash_password(p_password) == _password_hash;
}

void User::create_password(const std::string &p_password) {
	//todo improve a bit
	_pre_salt = hash_password(_nameui + _emailui);
	_post_salt = hash_password(_emailui + _nameui);

	_password_hash = hash_password(p_password);
}

std::string User::hash_password(const std::string &p_password) {
	SHA256 *s = SHA256::get();

	std::string p = _pre_salt + p_password + _post_salt;

	std::string c = s->compute(p);

	delete s;

	return c;
}

void User::register_sessions() {
	if (_sessions.size() == 0) {
		return;
	}

	SessionManager *sm = SessionManager::get_singleton();

	if (!sm) {
		printf("ERROR: UserManager::add_user SessionManager singleton is null, please allocate one!\n");
		return;
	}

	for (int i = 0; i < _sessions.size(); ++i) {
		HTTPSession *session = new HTTPSession();
		session->session_id = _sessions[i];
		session->add_object("user", this);

		sm->add_session(session);
	}
}

void User::unregister_sessions() {
	if (_sessions.size() == 0) {
		return;
	}

	SessionManager *sm = SessionManager::get_singleton();

	if (!sm) {
		printf("ERROR: UserManager::remove_user SessionManager singleton is null, please allocate one!\n");
		return;
	}

	for (int i = 0; i < _sessions.size(); ++i) {
		sm->delete_session(_sessions[i]);
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

				user->_sessions.push_back(session->session_id);

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

			if (u->_emailui == email_val) {
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

			user->_nameui = uname_val;
			user->_emailui = email_val;
			//todo
			user->_rank = 1;
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

	std::string error_str = "";

	std::string uname_val;
	std::string email_val;
	std::string pass_val;
	std::string pass_check_val;

	if (request->get_method() == HTTP_METHOD_POST) {

		uname_val = request->get_parameter("username");
		email_val = request->get_parameter("email");
		pass_val = request->get_parameter("password");
		pass_check_val = request->get_parameter("password_check");

		bool changed = false;

		std::vector<std::string> errors;

		bool valid = _profile_validator->validate(request, &errors);

		for (int i = 0; i < errors.size(); ++i) {
			error_str += errors[i] + "<br>";
		}

		if (valid) {
			if (uname_val == _nameui) {
				uname_val = "";
			}

			if (email_val == _emailui) {
				email_val = "";
			}

			if (uname_val != "") {
				User *user = UserManager::get_singleton()->get_user(uname_val);

				if (user) {
					error_str += "Username already taken!<br>";
				} else {
					//todo sanitize for html special chars!
					_nameui = uname_val;
					changed = true;
					uname_val = "";
				}
			}

			if (email_val != "") {
				UserManager *um = UserManager::get_singleton();

				bool email_found = false;

				//todo better way + should be thread safe
				for (int i = 0; i < um->_users_vec.size(); ++i) {
					User *u = um->_users_vec[i];

					if (!u) {
						continue;
					}

					if (u == this) {
						continue;
					}

					if (u->_emailui == email_val) {
						email_found = true;
						break;
					}
				}

				if (email_found) {
					error_str += "Email already in use!<br>";
				} else {
					//todo sanitize for html special chars!
					//also send email
					_emailui = email_val;
					changed = true;
					email_val = "";
				}
			}

			if (pass_val != "") {
				if (pass_val != pass_check_val) {
					error_str += "The passwords did not match!<br>";
				} else {
					create_password(pass_val);
					changed = true;
				}
			}

			if (changed) {
				save();
			}
		}
	}

	HTMLBuilder b;

	b.w("Settings");
	b.br();

	if (error_str.size() != 0) {
		b.div()->cls("error");

		b.w(error_str);

		b.cdiv();
	}

	b.div()->cls("settings");

	//todo href path helper
	b.form()->method("POST")->href("/user/settings");

	b.w("Username");
	b.br();
	b.input()->type("text")->name("username")->placeholder(_nameui)->value(uname_val);
	b.cinput();
	b.br();

	b.w("Email");
	b.br();
	b.input()->type("email")->name("email")->placeholder(_emailui)->value(email_val);
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
	b.cform();

	b.cdiv();

	request->body += b.result;

	request->compile_and_send_body();
}
void User::handle_password_reset_request(Request *request) {
	request->body += "handle_password_reset_request";

	request->compile_and_send_body();
}
void User::handle_logout_request(Request *request) {
	request->remove_cookie("session_id");

	for (int i = 0; i < _sessions.size(); ++i) {
		if (_sessions[i] == request->session->session_id) {
			_sessions[i] = _sessions[_sessions.size() - 1];
			_sessions.pop_back();
		}
	}

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

void User::file_save() {
}
void User::file_load() {
	FILE *f = fopen(_file_path.c_str(), "r");

	if (!f) {
		printf("FileBasedUser::load: Error opening file! %s\n", _file_path.c_str());
		return;
	}

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); // same as rewind(f);

	std::string fd;
	fd.resize(fsize);

	fread(&fd[0], 1, fsize, f);
	fclose(f);

	from_json(fd);
}
void User::file_ensure_directory_exist() {
}
std::string User::file_get_base_path() {
	return _path;
}

void User::file_load_all() {
	tinydir_dir dir;
	if (tinydir_open(&dir, _path.c_str()) == -1) {
		return;
	}

	while (dir.has_next) {
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1) {
			tinydir_next(&dir);
			continue;
		}

		if (!file.is_dir) {
			std::string np = file.path;
			np = np.substr(_path.size(), np.size() - _path.size());

			User *u = new User();
			u->load(np);

			UserManager::get_singleton()->add_user(u);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);
}

std::string User::file_get_path() {
	return _path;
}

void User::file_set_path(const std::string &path) {
	_path = path;

	if (_path.size() > 0) {
		if (_path[_path.size() - 1] != '/') {
			_path += '/';
		}
	}
}


std::string User::to_json(rapidjson::Document *into) {
	rapidjson::Document *document;

	if (into) {
		document = into;
	} else {
		document = new rapidjson::Document();
	}

	document->SetObject();

	document->AddMember("id", get_id(), document->GetAllocator());

	document->AddMember("name", rapidjson::Value(_nameui.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("email", rapidjson::Value(_emailui.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("rank", _rank, document->GetAllocator());
	document->AddMember("pre_salt", rapidjson::Value(_pre_salt.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("post_salt", rapidjson::Value(_post_salt.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("password_hash", rapidjson::Value(_password_hash.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("banned", _banned, document->GetAllocator());
	document->AddMember("password_reset_token", rapidjson::Value(_password_reset_token.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("locked", _locked, document->GetAllocator());

	rapidjson::Value sa(rapidjson::Type::kArrayType);
	rapidjson::Document::AllocatorType &allocator = document->GetAllocator();

	for (int i = 0; i < _sessions.size(); i++) {
		sa.PushBack(rapidjson::Value(_sessions[i].c_str(), document->GetAllocator()), allocator);
	}

	document->AddMember("sessions", sa, document->GetAllocator());

	if (into) {
		return "";
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document->Accept(writer);

	std::string s = buffer.GetString();

	delete document;

	return s;
}
void User::from_json(const std::string &p_data) {

	rapidjson::Document data;
	data.Parse(p_data.c_str());

	rapidjson::Value uobj = data.GetObject();

	set_id(uobj["id"].GetInt());
	_nameui = uobj["name"].GetString();
	_emailui = uobj["email"].GetString();
	_rank = uobj["rank"].GetInt();
	_pre_salt = uobj["pre_salt"].GetString();
	_post_salt = uobj["post_salt"].GetString();
	_password_hash = uobj["password_hash"].GetString();
	_banned = uobj["banned"].GetBool();

	_password_reset_token = uobj["password_reset_token"].GetString();
	_locked = uobj["locked"].GetBool();

	const rapidjson::Value &sess = uobj["sessions"].GetArray();

	for (rapidjson::Value::ConstValueIterator itr = sess.Begin(); itr != sess.End(); ++itr) {
		_sessions.push_back(itr->GetString());
	}
}

User::User() :
		Resource() {

	_rank = 0;
	_banned = false;
	_locked = false;
}

User::~User() {
	unregister_sessions();
}

FormValidator *User::_login_validator = nullptr;
FormValidator *User::_registration_validator = nullptr;
FormValidator *User::_profile_validator = nullptr;

std::string User::_path = "./";