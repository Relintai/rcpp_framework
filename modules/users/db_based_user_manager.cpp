#include "db_based_user_manager.h"

#include "db_based_user.h"

User *DBBasedUserManager::create_user() {
	User *u = new DBBasedUser();

	return u;
}

void DBBasedUserManager::load_all() {
	//DBBasedUser::load_all();
}

void DBBasedUserManager::set_table_name(const std::string &path) {
	//DBBasedUser::set_path(path);
}

void DBBasedUserManager::migrate() {
	DBBasedUser u;
	u.migrate();
}

DBBasedUserManager::DBBasedUserManager() :
		UserManager() {

	printf("Using DBBasedUserManager.\n");
}

DBBasedUserManager::~DBBasedUserManager() {
}
