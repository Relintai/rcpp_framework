#include "user_manager.h"

void UserManager::add_user(User *user) {
	if (!user) {
		printf("UserManager::add_user: ERROR, user is null!\n");
		return;
	}

	std::lock_guard<std::mutex> lock(_mutex);

	_users.push_back(user);

	for (int i = 0; i < user->sessions.size(); ++i) {
		_sessions[user->sessions[i]] = user;
	}
}
void UserManager::remove_user(User *user) {
	if (!user) {
		printf("UserManager::remove_user: ERROR, user is null!\n");
		return;
	}

	std::lock_guard<std::mutex> lock(_mutex);

	for (int i = 0; i < user->sessions.size(); ++i) {
		_sessions.erase(user->sessions[i]);
	}

	for (int i = 0; i < _users.size(); ++i) {
		if (_users[i] == user) {
			_users[i] = _users[_users.size() - 1];
			_users.pop_back();
			return;
		}
	}
}
void UserManager::logout_user(User *user) {
	if (!user) {
		printf("UserManager::logout_user: ERROR, user is null!\n");
		return;
	}

	std::lock_guard<std::mutex> lock(_mutex);

	for (int i = 0; i < user->sessions.size(); ++i) {
		_sessions.erase(user->sessions[i]);
	}

	user->sessions.clear();
}

User *UserManager::get_user_for_session(const std::string &session_id) {
	User *u = _sessions[session_id];

	if (!u) {
		printf("UserManager::get_user_for_session: ERROR, user is null! sessid:%s\n", session_id.c_str());
	}

	return u;
}
User *UserManager::get_user_with_id(const int user_id) {
	for (int i = 0; i < _users.size(); ++i) {

		User *u = _users[i];

		if (!u) {
			printf("UserManager::get_user_with_id: ERROR, user (%d) is null!\n", user_id);
			return u;
		}

		if (u->id == user_id) {
			return u;
		}
	}

	printf("UserManager::get_user_with_id: ERROR, user (%d) not found!\n", user_id);

	return nullptr;
}

void UserManager::logout_all() {
	for (int i = 0; i < _users.size(); ++i) {
		logout_user(_users[i]);
	}
}

void UserManager::clear() {
	std::lock_guard<std::mutex> lock(_mutex);

	for (int i = 0; i < _users.size(); ++i) {
		delete _users[i];
	}

	_sessions.clear();
	_users.clear();
}

UserManager *UserManager::get_singleton() {
	return _self;
}

UserManager::UserManager() :
		Object() {

	if (_self) {
		printf("UserManager::UserManager(): Error! self is not null!/n");
	}

	_self = this;
}

UserManager::~UserManager() {
	clear();
}

UserManager *UserManager::_self = nullptr;