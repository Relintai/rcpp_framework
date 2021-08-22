#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "core/object.h"

#include <map>
#include <mutex>
#include <string>
#include <vector>

class User;

class UserManager : public Object {
public:
	void add_user(User *user);
	void remove_user(User *user);
	User *get_user(const std::string &user_name);
	virtual User *create_user();
	virtual void load_all();

	void set_table_name(const std::string &name);

	virtual void migrate();

	void clear();

	static UserManager *get_singleton();

	UserManager();
	~UserManager();

	std::map<std::string, User *> _users;
	std::vector<User *> _users_vec;
	std::mutex _mutex;

protected:
	static UserManager *_self;
};

#endif