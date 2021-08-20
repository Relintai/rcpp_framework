#include "db_based_user.h"

#include "core/database/database_manager.h"
#include "core/database/query_builder.h"
#include "core/database/query_result.h"
#include "core/database/table_builder.h"
#include "user_manager.h"
#include <cstdio>
#include <sstream>

void DBBasedUser::save() {
	QueryBuilder *b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	if (id == 0) {
		b->insert(_table_name, "username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked")->values();
		b->eval(name);
		b->eval(email);
		b->val(rank);
		b->val(pre_salt);
		b->val(post_salt);
		b->val(password_hash);
		b->val(banned);
		b->val(password_reset_token);
		b->val(locked);

		b->cvalues()->end_command();
		b->select_last_insert_id();

		QueryResult *r = b->run();

		id = r->get_last_insert_rowid();

		delete r;

	} else {
		//update
	}

	if (id == 0) {
		return;
	}

	//todo better way
	std::stringstream ss;
	ss << id;
	std::string uid = ss.str();

	//todo
	b->query_result = "DELETE FROM " + _table_name + "_sessions WHERE user_id=" + uid;
	b->end_command();
	b->run_query();

	b->query_result = "";

	for (int i = 0; i < sessions.size(); ++i) {
		b->query_result += "INSERT INTO " + _table_name + "_sessions VALUES(" + uid + ", '" + sessions[i] + "');";
	}

	b->run_query();

	delete b;
}

void DBBasedUser::load() {
	unregister_sessions();

	if (id == 0) {
		return;
	}

	QueryBuilder *b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->select("username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_table_name);

	//todo better way
	std::stringstream ss;
	ss << id;
	std::string uid = ss.str();
	b->where("id=" + uid);

	b->end_command();

	QueryResult *r = b->run();

	if (r->next_row()) {
		ss.clear();

		name = r->get_cell(0);
		email = r->get_cell(1);

		ss << r->get_cell(2);
		ss >> rank;

		pre_salt = r->get_cell(3);
		post_salt = r->get_cell(4);
		password_hash = r->get_cell(5);

		ss << r->get_cell(6);
		ss >> banned;

		password_reset_token = r->get_cell(7);

		ss << r->get_cell(8);
		ss >> locked;
	}

	delete r;

	b->query_result = "";

	b->select("session_id");
	b->from(_table_name + "_sessions");
	b->where("user_id=" + uid);
	b->end_command();

	r = b->run();

	while (r->next_row()) {
		sessions.push_back(r->get_cell(0));
	}

	delete r;

	delete b;

	register_sessions();
}

void DBBasedUser::migrate() {
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

void DBBasedUser::load_all() {
	QueryBuilder *b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->select("id");
	b->from(_table_name);
	b->end_command();
	b->print();

	QueryResult *r = b->run();

	while (r->next_row()) {
		DBBasedUser *u = new DBBasedUser();
		//todo better way
		const char *c = r->get_cell(0);
		std::stringstream ss;
		ss << c;
		ss >> u->id;

		u->load();

		UserManager::get_singleton()->add_user(u);
	}

	delete r;

	delete b;
}

DBBasedUser::DBBasedUser() :
		User() {
}

DBBasedUser::~DBBasedUser() {
}

std::string DBBasedUser::_table_name = "users";