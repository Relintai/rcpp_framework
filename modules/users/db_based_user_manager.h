#ifndef DB_BASED_USER_MANAGER_H
#define DB_BASED_USER_MANAGER_H

#include "core/object.h"

#include "user_manager.h"

class User;

class DBBasedUserManager : public UserManager {

public:
	virtual User *create_user();
	void load_all();

	void set_table_name(const std::string &name);

	virtual void migrate();

	DBBasedUserManager();
	~DBBasedUserManager();

protected:
};

#endif