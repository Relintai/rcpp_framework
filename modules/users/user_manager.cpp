#include "user_manager.h"

#include "core/http/http_session.h"
#include "core/http/session_manager.h"

#include "user.h"
#include <stdlib.h>

void UserManager::add_user(User *user) {
	if (!user) {
		printf("UserManager::add_user: ERROR, user is null!\n");
		return;
	}

	std::lock_guard<std::mutex> lock(_mutex);

	_users_vec.push_back(user);
	_users[user->nameui] = user;
}

void UserManager::remove_user(User *user) {
	if (!user) {
		printf("UserManager::remove_user: ERROR, user is null!\n");
		return;
	}

	std::lock_guard<std::mutex> lock(_mutex);

	_users.erase(user->nameui);

	for (int i = 0; i < _users_vec.size(); ++i) {
		if (_users_vec[i] == user) {
			_users_vec[i] = _users_vec[_users_vec.size() - 1];
			_users_vec.pop_back();
			break;
		}
	}
}

User *UserManager::get_user(const std::string &user_name) {
	return _users[user_name];
}

User *UserManager::create_user() {
	User *u = new User();

	return u;
}

void UserManager::load_all() {
}

void UserManager::clear() {
	SessionManager *sm = SessionManager::get_singleton();

	std::lock_guard<std::mutex> lock(_mutex);

	for (int i = 0; i < _users_vec.size(); ++i) {
		delete _users_vec[i];
	}

	_users.clear();
	_users_vec.clear();
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

	if (_self == this) {
		_self = nullptr;
	}
}

UserManager *UserManager::_self = nullptr;