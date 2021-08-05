#include "file_based_user_manager.h"

#include "file_based_user.h"

User *FileBasedUserManager::create_user() {
	User *u = new FileBasedUser();

	return u;
}

FileBasedUserManager::FileBasedUserManager() :
		UserManager() {
}

FileBasedUserManager::~FileBasedUserManager() {
}
