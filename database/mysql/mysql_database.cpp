#include "mysql_database.h"

#include "core/database_manager.h"

Database *MysqlDatabase::_creation_func() {
	return new MysqlDatabase();
}

void MysqlDatabase::_register() {
	DatabaseManager::_register_db_creation_func("mysql", MysqlDatabase::_creation_func);
}

void MysqlDatabase::_unregister() {
	DatabaseManager::_unregister_db_creation_func("mysql");
}