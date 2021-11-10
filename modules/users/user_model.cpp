#include "user_model.h"

#include "core/database/database.h"
#include "core/database/database_manager.h"
#include "core/database/query_builder.h"
#include "core/database/query_result.h"
#include "core/database/table_builder.h"

#include "core/hash/sha256.h"

Ref<User> UserModel::get_user(const int id) {
	if (id == 0) {
		return Ref<User>();
	}

	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->select("username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_table_name);

	b->where()->wp("id", id);

	b->end_command();

	Ref<QueryResult> r = b->run();

	if (!r->next_row()) {
		return Ref<User>();
	}

	Ref<User> user;
	user = create_user();

	user->id = id;
	user->name_user_input = r->get_cell(0);
	user->email_user_input = r->get_cell(1);
	user->rank = r->get_cell_int(2);
	user->pre_salt = r->get_cell(3);
	user->post_salt = r->get_cell(4);
	user->password_hash = r->get_cell(5);
	user->banned = r->get_cell_bool(6);
	user->password_reset_token = r->get_cell(7);
	user->locked = r->get_cell_bool(8);

	return user;
}

Ref<User> UserModel::get_user(const String &user_name_input) {
	if (user_name_input == "") {
		return Ref<User>();
	}

	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->select("id, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_table_name);
	b->where()->wp("username", user_name_input);
	b->end_command();

	Ref<QueryResult> r = b->run();

	if (!r->next_row()) {
		return Ref<User>();
	}

	Ref<User> user;
	user = create_user();

	user->id = r->get_cell_int(0);
	user->name_user_input = user_name_input;
	user->email_user_input = r->get_cell(1);
	user->rank = r->get_cell_int(2);
	user->pre_salt = r->get_cell(3);
	user->post_salt = r->get_cell(4);
	user->password_hash = r->get_cell(5);
	user->banned = r->get_cell_bool(6);
	user->password_reset_token = r->get_cell(7);
	user->locked = r->get_cell_bool(8);

	return user;
}

void UserModel::save_user(Ref<User> &user) {
	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	if (user->id == 0) {
		b->insert(_table_name, "username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");

		b->values();
		b->val(user->name_user_input);
		b->val(user->email_user_input);
		b->val(user->rank);
		b->val(user->pre_salt);
		b->val(user->post_salt);
		b->val(user->password_hash);
		b->val(user->banned);
		b->val(user->password_reset_token);
		b->val(user->locked);
		b->cvalues();

		b->end_command();
		b->select_last_insert_id();

		Ref<QueryResult> r = b->run();

		user->id = r->get_last_insert_rowid();
	} else {
		b->update(_table_name);
		b->set();
		b->setp("username", user->name_user_input);
		b->setp("email", user->email_user_input);
		b->setp("rank", user->rank);
		b->setp("pre_salt", user->pre_salt);
		b->setp("post_salt", user->post_salt);
		b->setp("password_hash", user->password_hash);
		b->setp("banned", user->banned);
		b->setp("password_reset_token", user->password_reset_token);
		b->setp("locked", user->locked);
		b->cset();
		b->where()->wp("id", user->id);

		//b->print();

		b->run_query();
	}
}

Vector<Ref<User> > UserModel::get_all() {
	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->select("id, username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_table_name);
	b->end_command();
	//b->print();

	Vector<Ref<User> > users;

	Ref<QueryResult> r = b->run();

	while (r->next_row()) {
		Ref<User> user = create_user();

		user->id = r->get_cell_int(0);
		user->name_user_input = r->get_cell(1);
		user->email_user_input = r->get_cell(2);
		user->rank = r->get_cell_int(3);
		user->pre_salt = r->get_cell(4);
		user->post_salt = r->get_cell(5);
		user->password_hash = r->get_cell(6);
		user->banned = r->get_cell_bool(7);
		user->password_reset_token = r->get_cell(8);
		user->locked = r->get_cell_bool(9);

		users.push_back(user);
	}

	return users;
}

Ref<User> UserModel::create_user() {
	Ref<User> u;
	u.instance();

	return u;
}

bool UserModel::is_username_taken(const String &user_name_input) {
	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->select("id")->from(_table_name)->where("username")->like(user_name_input)->end_command();

	Ref<QueryResult> r = b->run();

	return r->next_row();
}
bool UserModel::is_email_taken(const String &email_input) {
	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->select("id")->from(_table_name)->where("username")->like(email_input)->end_command();

	Ref<QueryResult> r = b->run();

	return r->next_row();
}

bool UserModel::check_password(const Ref<User> &user, const String &p_password) {
	return hash_password(user, p_password) == user->password_hash;
}

void UserModel::create_password(Ref<User> &user, const String &p_password) {
	if (!user.is_valid()) {
		printf("Error UserModel::create_password !user.is_valid()!\n");
		return;
	}

	//todo improve a bit
	user->pre_salt = hash_password(user, user->name_user_input + user->email_user_input);
	user->post_salt = hash_password(user, user->email_user_input + user->name_user_input);

	user->password_hash = hash_password(user, p_password);
}

String UserModel::hash_password(const Ref<User> &user, const String &p_password) {
	if (!user.is_valid()) {
		printf("Error UserModel::hash_password !user.is_valid()!\n");
		return "";
	}

	Ref<SHA256> s = SHA256::get();

	String p = user->pre_salt + p_password + user->post_salt;

	String c = s->compute(p);

	return c;
}

void UserModel::create_table() {
	Ref<TableBuilder> tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

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
}
void UserModel::drop_table() {
	Ref<TableBuilder> tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

	tb->drop_table_if_exists(_table_name)->run_query();
}
void UserModel::migrate() {
	drop_table();
	create_table();
}

UserModel *UserModel::get_singleton() {
	return _self;
}

UserModel::UserModel() :
		Object() {

	if (_self) {
		printf("UserModel::UserModel(): Error! self is not null!/n");
	}

	_self = this;
}

UserModel::~UserModel() {
	if (_self == this) {
		_self = nullptr;
	}
}

UserModel *UserModel::_self = nullptr;

String UserModel::_path = "./";
String UserModel::_table_name = "users";