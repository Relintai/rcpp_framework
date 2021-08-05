#include "file_based_user_manager.h"

#include "file_based_user.h"

User *FileBasedUserManager::create_user() {
	User *u = new FileBasedUser();

	return u;
}

void FileBasedUserManager::load_all() {
	FileBasedUser::load_all();
}

void FileBasedUserManager::set_path(const std::string &path) {
	FileBasedUser::set_path(path);
}

FileBasedUserManager::FileBasedUserManager() :
		UserManager() {

	printf("Using FileBasedUserManager. Note: Please DON'T USE THIS IN PRODUCTION IT'S NOT SAFE FOR THAT!\n");
}

FileBasedUserManager::~FileBasedUserManager() {
}
