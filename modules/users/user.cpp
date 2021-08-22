#include "user.h"

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "user_manager.h"
#include <rapidjson/writer.h>
#include <tinydir/tinydir.h>
#include <cstdio>

#include "core/database/database_manager.h"
#include "core/database/query_builder.h"
#include "core/database/query_result.h"
#include "core/database/table_builder.h"

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

	QueryBuilder *b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	if (get_id() == 0) {
		b->insert(_table_name, "username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");

		b->values();
		b->eval(name_user_input);
		b->eval(email_user_input);
		b->val(rank);
		b->val(pre_salt);
		b->val(post_salt);
		b->val(password_hash);
		b->val(banned);
		b->val(password_reset_token);
		b->val(locked);
		b->cvalues();

		b->end_command();
		b->select_last_insert_id();

		QueryResult *r = b->run();

		set_id(r->get_last_insert_rowid());

		delete r;

	} else {
		b->udpate(_table_name);
		b->set();
		b->esetp("username", name_user_input);
		b->esetp("email", email_user_input);
		b->setp("rank", rank);
		b->setp("pre_salt", pre_salt);
		b->setp("post_salt", post_salt);
		b->setp("password_hash", password_hash);
		b->setp("banned", banned);
		b->setp("password_reset_token", password_reset_token);
		b->setp("locked", locked);
		b->cset();
		b->where()->wp("id", get_id());

		//b->print();

		b->run_query();
	}

	if (get_id() == 0) {
		return;
	}

	b->reset();

	b->del(_table_name + "_sessions")->where()->wp("user_id", get_id())->end_command();
	//b->print();

	b->end_command();
	b->run_query();

	b->reset();

	for (int i = 0; i < sessions.size(); ++i) {
		b->insert(_table_name + "_sessions")->values()->val(get_id())->val(sessions[i])->cvalues()->end_command();
	}

	//b->print();

	b->run_query();

	delete b;
}

void User::load() {

	unregister_sessions();

	if (get_id() == 0) {
		return;
	}

	QueryBuilder *b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->select("username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_table_name);

	b->where()->wp("id", get_id());

	b->end_command();

	QueryResult *r = b->run();

	if (r->next_row()) {
		name_user_input = r->get_cell(0);
		email_user_input = r->get_cell(1);
		rank = r->get_cell_int(2);
		pre_salt = r->get_cell(3);
		post_salt = r->get_cell(4);
		password_hash = r->get_cell(5);
		banned = r->get_cell_bool(6);
		password_reset_token = r->get_cell(7);
		locked = r->get_cell_bool(8);
	}

	delete r;

	b->query_result = "";

	b->select("session_id");
	b->from(_table_name + "_sessions");
	b->where()->wp("user_id", get_id());
	b->end_command();

	r = b->run();

	while (r->next_row()) {
		sessions.push_back(r->get_cell(0));
	}

	delete r;

	delete b;

	register_sessions();
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


void User::migrate() {
	TableBuilder *tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

	tb->drop_table_if_exists(_table_name)->run_query();
	tb->drop_table_if_exists(_table_name + "_sessions")->run_query();
	//tb->print();

	tb->result = "";

	tb->create_table(_table_name);
	tb->integer("id")->auto_increment()->next_row();
	tb->varchar("username", 60)->not_null()->next_row();
	tb->varchar("email", 100)->not_null()->next_row();
	tb->integer("rank")->not_null()->next_row();
	tb->varchar("pre_salt", 100)->next_row();
	tb->varchar("post_salt", 100)->next_row();
	tb->varchar("password_hash", 100)->next_row();
	tb->integer("banned")->next_row();
	tb->varchar("password_reset_token", 100)->next_row();
	tb->integer("locked")->next_row();
	tb->primary_key("id");
	tb->ccreate_table();
	tb->run_query();
	//tb->print();

	tb->result = "";

	tb->create_table(_table_name + "_sessions");
	tb->integer("user_id")->not_null()->next_row();
	tb->varchar("session_id", 100)->next_row();
	tb->foreign_key("user_id");
	tb->references("user", "id");
	tb->ccreate_table();
	//tb->print();
	tb->run_query();

	delete tb;
}

void User::db_load_all() {
	QueryBuilder *b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->select("id");
	b->from(_table_name);
	b->end_command();
	b->print();

	QueryResult *r = b->run();

	while (r->next_row()) {
		User *u = new User();
		u->set_id(r->get_cell_int(0));
		u->load();

		//printf("%s\n", u->to_json().c_str());

		UserManager::get_singleton()->add_user(u);
	}

	delete r;

	delete b;
}


bool User::check_password(const std::string &p_password) {
	return hash_password(p_password) == password_hash;
}

void User::create_password(const std::string &p_password) {
	//todo improve a bit
	pre_salt = hash_password(name_user_input + email_user_input);
	post_salt = hash_password(email_user_input + name_user_input);

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

				user->sessions.push_back(session->session_id);

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

			if (u->email_user_input == email_val) {
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

			user->name_user_input = uname_val;
			user->email_user_input = email_val;
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
			if (uname_val == name_user_input) {
				uname_val = "";
			}

			if (email_val == email_user_input) {
				email_val = "";
			}

			if (uname_val != "") {
				User *user = UserManager::get_singleton()->get_user(uname_val);

				if (user) {
					error_str += "Username already taken!<br>";
				} else {
					//todo sanitize for html special chars!
					name_user_input = uname_val;
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

					if (u->email_user_input == email_val) {
						email_found = true;
						break;
					}
				}

				if (email_found) {
					error_str += "Email already in use!<br>";
				} else {
					//todo sanitize for html special chars!
					//also send email
					email_user_input = email_val;
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
	b.input()->type("text")->name("username")->placeholder(name_user_input)->value(uname_val);
	b.cinput();
	b.br();

	b.w("Email");
	b.br();
	b.input()->type("email")->name("email")->placeholder(email_user_input)->value(email_val);
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

	for (int i = 0; i < sessions.size(); ++i) {
		if (sessions[i] == request->session->session_id) {
			sessions[i] = sessions[sessions.size() - 1];
			sessions.pop_back();
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

	document->AddMember("name", rapidjson::Value(name_user_input.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("email", rapidjson::Value(email_user_input.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("rank", rank, document->GetAllocator());
	document->AddMember("pre_salt", rapidjson::Value(pre_salt.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("post_salt", rapidjson::Value(post_salt.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("password_hash", rapidjson::Value(password_hash.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("banned", banned, document->GetAllocator());
	document->AddMember("password_reset_token", rapidjson::Value(password_reset_token.c_str(), document->GetAllocator()), document->GetAllocator());
	document->AddMember("locked", locked, document->GetAllocator());

	rapidjson::Value sa(rapidjson::Type::kArrayType);
	rapidjson::Document::AllocatorType &allocator = document->GetAllocator();

	for (int i = 0; i < sessions.size(); i++) {
		sa.PushBack(rapidjson::Value(sessions[i].c_str(), document->GetAllocator()), allocator);
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
	name_user_input = uobj["name"].GetString();
	email_user_input = uobj["email"].GetString();
	rank = uobj["rank"].GetInt();
	pre_salt = uobj["pre_salt"].GetString();
	post_salt = uobj["post_salt"].GetString();
	password_hash = uobj["password_hash"].GetString();
	banned = uobj["banned"].GetBool();

	password_reset_token = uobj["password_reset_token"].GetString();
	locked = uobj["locked"].GetBool();

	const rapidjson::Value &sess = uobj["sessions"].GetArray();

	for (rapidjson::Value::ConstValueIterator itr = sess.Begin(); itr != sess.End(); ++itr) {
		sessions.push_back(itr->GetString());
	}
}

User::User() :
		Resource() {

	rank = 0;
	banned = false;
	locked = false;
}

User::~User() {
	unregister_sessions();
}

FormValidator *User::_login_validator = nullptr;
FormValidator *User::_registration_validator = nullptr;
FormValidator *User::_profile_validator = nullptr;

std::string User::_path = "./";
std::string User::_table_name = "users";
