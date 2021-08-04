#include "session_manager.h"

#include "http_session.h"
#include <stdlib.h>
#include <time.h>

#include "core/hash/sha256.h"

void SessionManager::add_session(HTTPSession *session) {
	if (!session) {
		printf("SessionManager::add_session: ERROR, session is null!\n");
		return;
	}

	std::lock_guard<std::mutex> lock(_mutex);

	_sessions_vec.push_back(session);
	_sessions[session->session_id] = session;
}

void SessionManager::remove_session(HTTPSession *session) {
	if (!session) {
		printf("SessionManager::remove_session: ERROR, session is null!\n");
		return;
	}

	std::lock_guard<std::mutex> lock(_mutex);

	_sessions.erase(session->session_id);

	for (int i = 0; i < _sessions_vec.size(); ++i) {
		if (_sessions_vec[i] == session) {
			_sessions_vec[i] = _sessions_vec[_sessions_vec.size() - 1];
			_sessions_vec.pop_back();
			return;
		}
	}
}

void SessionManager::delete_session(const std::string &session_id) {
	std::lock_guard<std::mutex> lock(_mutex);

	_sessions.erase(session_id);

	for (int i = 0; i < _sessions_vec.size(); ++i) {
		if (_sessions_vec[i]->session_id == session_id) {
			_sessions_vec[i] = _sessions_vec[_sessions_vec.size() - 1];
			_sessions_vec.pop_back();
			return;
		}
	}
}

HTTPSession *SessionManager::get_session(const std::string &session_id) {
	return _sessions[session_id];
}

HTTPSession *SessionManager::create_session() {
	HTTPSession *session = new HTTPSession();

	std::unique_lock<std::mutex> lock(_mutex, std::defer_lock);

	while (true) {
		session->session_id = generate_session_id(session->session_id);

		lock.lock();

		if (_sessions[session->session_id] == nullptr) {

			_sessions_vec.push_back(session);
			_sessions[session->session_id] = session;

			add_session(session);

			lock.unlock();

			return session;
		}

		lock.unlock();
	}

	return session;
}

void SessionManager::clear() {
	std::lock_guard<std::mutex> lock(_mutex);

	for (int i = 0; i < _sessions_vec.size(); ++i) {
		delete _sessions_vec[i];
	}

	_sessions.clear();
	_sessions_vec.clear();
}

std::string generate_session_id(const std::string &base) {
	//todo make something simpler / better

	SHA256 *h = SHA256::get();
	std::string sid = base;

	sid += rand();
	h->compute(sid);

	sid = h->get_hash();
	sid.resize(20);

	delete h;

	return sid;
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