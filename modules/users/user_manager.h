#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "core/object.h"

#include <map>
#include <string>
#include <vector>
#include <mutex>

#include "user.h"

class UserManager : public Object {
public:
	void add_user(User *user);
	void remove_user(User *user);
	void logout_user(User *user);

	User *get_user_for_session(const std::string &session_id);
	User *get_user_with_id(const int user_id);

	void logout_all();
	void clear();

	static UserManager* get_singleton();

	UserManager();
	~UserManager();

	std::map<std::string, User *> _sessions;
	std::vector<User *> _users;
	std::mutex _mutex;

protected:
	static UserManager* _self;
};

#endif