#include "session_manager.h"

#include "http_session.h"
#include <stdlib.h>
#include <time.h>

#include "database/database_manager.h"
#include "database/query_builder.h"
#include "database/query_result.h"
#include "database/table_builder.h"

#include "crypto/hash/sha256.h"

#include "request.h"

#include "cookie.h"

void SessionManager::add_session(Ref<HTTPSession> &session) {
	if (!session.is_valid()) {
		printf("SessionManager::add_session: ERROR, session is null!\n");
		return;
	}

	_mutex.lock();

	_sessions_vec.push_back(session);
	_sessions[session->session_id] = session;

	_mutex.unlock();
}

void SessionManager::remove_session(Ref<HTTPSession> &session) {
	if (!session.is_valid()) {
		printf("SessionManager::remove_session: ERROR, session is null!\n");
		return;
	}

	_mutex.lock();

	_sessions.erase(session->session_id);

	for (int i = 0; i < _sessions_vec.size(); ++i) {
		if (_sessions_vec[i] == session) {
			_sessions_vec[i] = _sessions_vec[_sessions_vec.size() - 1];
			_sessions_vec.pop_back();
			_mutex.unlock();
			return;
		}
	}

	_mutex.unlock();
}

void SessionManager::delete_session(const String &session_id) {
	_mutex.lock();

	Ref<HTTPSession> s = _sessions[session_id];

	_sessions.erase(session_id);

	for (int i = 0; i < _sessions_vec.size(); ++i) {
		Ref<HTTPSession> sess = _sessions_vec[i];

		if (sess->session_id == session_id) {

			_sessions_vec[i] = _sessions_vec[_sessions_vec.size() - 1];
			_sessions_vec.pop_back();

			break;
		}
	}

	_mutex.unlock();

	if (!s.is_valid()) {
		return;
	}

	if (!s->id) {
		return;
	}

	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->del(_data_table_name)->where()->wp("session_db_id", s->id)->end_command();
	b->del(_table_name)->where()->wp("id", s->id)->end_command();
	b->run_query();
}

void SessionManager::save_session(Ref<HTTPSession> &session) {
	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	if (!session->id) {
		b->insert(_table_name, "session_id");
		b->values();
		b->val(session->session_id);
		b->cvalues();
		b->end_command();
		b->select_last_insert_id();

		session->id = b->run()->get_last_insert_rowid();

		b->reset();
	}

	b->del(_data_table_name)->where()->wp("session_db_id", session->id)->end_command();
	int id = session->id;

	std::map<String, Variant> *m = session->get_data();
	for (std::map<String, Variant>::iterator it = m->begin(); it != m->end(); it++) {
		const Variant &val = it->second;

		if (val.is_simple_type()) {
			b->insert(_data_table_name, "session_db_id,key,value")->values()->val(id)->val(it->first)->val(val.to_string())->cvalues()->end_command();
		}
	}

	b->run_query();
}

Ref<HTTPSession> SessionManager::get_session(const String &session_id) {
	return _sessions[session_id];
}

Ref<HTTPSession> SessionManager::create_session() {
	Ref<HTTPSession> session = new HTTPSession();

	while (true) {
		session->session_id = generate_session_id(session->session_id);

		_mutex.lock();

		if (_sessions[session->session_id] == nullptr) {

			_sessions_vec.push_back(session);
			_sessions[session->session_id] = session;

			_mutex.unlock();

			return session;
		}

		_mutex.unlock();
	}

	save_session(session);

	return session;
}

void SessionManager::load_sessions() {
	clear();

	Ref<QueryBuilder> b = DatabaseManager::get_singleton()->ddb->get_query_builder();

	b->select("id, session_id");
	b->from(_table_name);
	// b->print();
	Ref<QueryResult> r = b->run();

	while (r->next_row()) {
		int id = r->get_cell_int(0);
		String session_id = r->get_cell(1);

		Ref<HTTPSession> s = new HTTPSession();
		s->id = id;

		s->session_id = session_id;

		add_session(s);
	}

	b->reset();

	b->select("session_db_id,key,value");
	b->from(_data_table_name);
	// b->print();
	r = b->run();

	while (r->next_row()) {
		int session_db_id = r->get_cell_int(0);

		Ref<HTTPSession> s;

		for (int i = 0; i < _sessions_vec.size(); ++i) {
			Ref<HTTPSession> ss = _sessions_vec[i];

			if (ss.is_valid() && session_db_id == ss->id) {
				s = ss;
				break;
			}
		}

		if (!s.is_valid()) {
			printf("Error: SessionManager::load_sessions(): %d sid doesn't exists!\n", session_db_id);

			continue;
		}

		String key = r->get_cell(1);
		String value = r->get_cell(2);

		s->add(key, Variant::parse_string(value));
	}
}

void SessionManager::clear() {
	_mutex.lock();

	_sessions.clear();
	_sessions_vec.clear();

	_mutex.unlock();
}

String SessionManager::generate_session_id(const String &base) {
	// todo make something simpler / better

	Ref<SHA256> h = SHA256::get();
	String sid = base;

	sid += rand();
	h->compute(sid);

	sid = h->get_hash();
	sid.resize(20);

	return sid;
}

void SessionManager::migrate() {
	drop_table();
	create_table();
}

void SessionManager::create_table() {
	Ref<TableBuilder> tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

	tb->create_table(_table_name);
	tb->integer("id")->auto_increment()->next_row();
	tb->varchar("session_id", 100)->next_row();
	tb->primary_key("id");
	tb->ccreate_table();
	// tb->print();
	tb->run_query();

	tb->result = "";

	tb->create_table(_data_table_name);
	tb->integer("session_db_id")->not_null()->next_row();
	tb->varchar("key", 100)->next_row();
	tb->text("value")->not_null()->next_row();
	tb->foreign_key("session_db_id");
	tb->references(_table_name, "id");
	tb->ccreate_table();
	// tb->print();
	tb->run_query();
}
void SessionManager::drop_table() {
	Ref<TableBuilder> tb = DatabaseManager::get_singleton()->ddb->get_table_builder();

	tb->drop_table_if_exists(_table_name)->run_query();
	tb->drop_table_if_exists(_data_table_name)->run_query();
}

SessionManager *SessionManager::get_singleton() {
	return _self;
}

SessionManager::SessionManager() :
		Object() {

	if (_self) {
		printf("SessionManager::SessionManager(): Error! self is not null!/n");
	}

	_self = this;

	srand(time(NULL));
}

SessionManager::~SessionManager() {
	clear();

	if (_self == this) {
		_self = nullptr;
	}
}

SessionManager *SessionManager::_self = nullptr;
String SessionManager::_table_name = "sessions";
String SessionManager::_data_table_name = "session_data";

bool SessionSetupMiddleware::on_before_handle_request_main(Request *request) {
	const String sid = request->get_cookie("session_id");

	if (sid == "") {
		// You could create a session here if you want to always assign sessions to visitors.
		// Example code:
		// HTTPSession *session = SessionManager::get_singleton()->create_session();
		// request->session = session;
		// request->add_cookie(::Cookie("session_id", session->session_id));

		return false;
	}

	request->session = SessionManager::get_singleton()->get_session(sid);

	return false;
}

SessionSetupMiddleware::SessionSetupMiddleware() {
}
SessionSetupMiddleware::~SessionSetupMiddleware() {
}
