#ifndef USER_H
#define USER_H

#include "core/object.h"

#include <string>
#include <vector>

class User : public Object {
public:
	int id;
	std::string name;
	std::string email;
	int rank;
	std::string pre_salt;
	std::string post_salt;
	std::string password_hash;
	bool banned;
	std::vector<std::string> sessions;
	std::string password_reset_token;
	bool locked;

	virtual void save();
	virtual void load();
	virtual void load(const std::string &p_name);
	virtual void changed();
	virtual void update();

	void register_sessions();
	void unregister_sessions();

	User();
	~User();
};

#endif