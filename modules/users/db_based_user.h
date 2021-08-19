#ifndef DB_BASED_USER_H
#define DB_BASED_USER_H

#include "user.h"

class DBBasedUser : public User {
public:
	static std::string get_path();
	static void set_path(const std::string &path);

	void save();
	void load();

	void migrate();

	static void load_all();

	DBBasedUser();
	~DBBasedUser();

protected:
	static std::string _table_name;
};

#endif