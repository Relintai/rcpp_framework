#include "db_based_user.h"

#include "core/database/database_manager.h"
#include "core/database/query_builder.h"
#include "core/database/query_result.h"
#include "core/database/table_builder.h"
#include "user_manager.h"
#include <cstdio>

void DBBasedUser::save() {
	QueryBuilder *b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	if (get_id() == 0) {
		b->insert(_table_name, "username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");

		b->values();
		b->eval(_nameui);
		b->eval(_emailui);
		b->val(_rank);
		b->val(_pre_salt);
		b->val(_post_salt);
		b->val(_password_hash);
		b->val(_banned);
		b->val(_password_reset_token);
		b->val(_locked);
		b->cvalues();

		b->end_command();
		b->select_last_insert_id();

		QueryResult *r = b->run();

		set_id(r->get_last_insert_rowid());

		delete r;

	} else {
		b->udpate(_table_name);
		b->set();
		b->esetp("username", _nameui);
		b->esetp("email", _emailui);
		b->setp("rank", _rank);
		b->setp("pre_salt", _pre_salt);
		b->setp("post_salt", _post_salt);
		b->setp("password_hash", _password_hash);
		b->setp("banned", _banned);
		b->setp("password_reset_token", _password_reset_token);
		b->setp("locked", _locked);
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

	for (int i = 0; i < _sessions.size(); ++i) {
		b->insert(_table_name + "_sessions")->values()->val(get_id())->val(_sessions[i])->cvalues()->end_command();
	}

	//b->print();

	b->run_query();

	delete b;
}

void DBBasedUser::load() {
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
		_nameui = r->get_cell(0);
		_emailui = r->get_cell(1);
		_rank = r->get_cell_int(2);
		_pre_salt = r->get_cell(3);
		_post_salt = r->get_cell(4);
		_password_hash = r->get_cell(5);
		_banned = r->get_cell_bool(6);
		_password_reset_token = r->get_cell(7);
		_locked = r->get_cell_bool(8);
	}

	delete r;

	b->query_result = "";

	b->select("session_id");
	b->from(_table_name + "_sessions");
	b->where()->wp("user_id", get_id());
	b->end_command();

	r = b->run();

	while (r->next_row()) {
		_sessions.push_back(r->get_cell(0));
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
		u->set_id(r->get_cell_int(0));
		u->load();

		//printf("%s\n", u->to_json().c_str());

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