#include "db_based_user.h"

#include "core/database/database_manager.h"
#include "core/database/query_builder.h"
#include "core/database/query_result.h"
#include "core/database/table_builder.h"
#include "user_manager.h"
#include <cstdio>

void DBBasedUser::save() {
	QueryBuilder *b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	if (id == 0) {
		b->insert(_table_name, "username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");

		b->values();
		b->eval(nameui);
		b->eval(emailui);
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

		id = r->get_last_insert_rowid();

		delete r;

	} else {
		b->udpate(_table_name);
		b->set();
		b->esetp("username", nameui);
		b->esetp("email", emailui);
		b->setp("rank", rank);
		b->setp("pre_salt", pre_salt);
		b->setp("post_salt", post_salt);
		b->setp("password_hash", password_hash);
		b->setp("banned", banned);
		b->setp("password_reset_token", password_reset_token);
		b->setp("locked", locked);
		b->cset();
		b->where()->wp("id", id);

		//b->print();

		b->run_query();
	}

	if (id == 0) {
		return;
	}

	b->reset();

	b->del(_table_name + "_sessions")->where()->wp("user_id", id)->end_command();
	//b->print();

	b->end_command();
	b->run_query();

	b->reset();

	for (int i = 0; i < sessions.size(); ++i) {
		b->insert(_table_name + "_sessions")->values()->val(id)->val(sessions[i])->cvalues()->end_command();
	}

	//b->print();

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

	b->where()->wp("id", id);

	b->end_command();

	QueryResult *r = b->run();

	if (r->next_row()) {
		nameui = r->get_cell(0);
		emailui = r->get_cell(1);
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
	b->where()->wp("user_id", id);
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
		u->id = r->get_cell_int(0);
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