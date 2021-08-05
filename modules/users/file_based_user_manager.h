#ifndef FILE_BASED_USER_MANAGER_H
#define FILE_BASED_USER_MANAGER_H

#include "core/object.h"

#include "user_manager.h"

class User;

class FileBasedUserManager : public UserManager {

public:
	virtual User *create_user();
	void load_all();

	void set_path(const std::string &path);

	FileBasedUserManager();
	~FileBasedUserManager();

protected:
};

#endif