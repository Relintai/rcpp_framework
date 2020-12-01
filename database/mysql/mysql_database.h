#ifndef MYSQL_CONNECTION
#define MYSQL_CONNECTION

#include "core/database.h"
#include "core/database_manager.h"

//Brynet has it aswell, and because of using namespace it is defined here aswell
//later this will be fixed better
#ifdef IS_NUM
#undef IS_NUM
#endif

#include <mysql.h>

class MysqlDatabase : public Database {
public:
	static Database *_creation_func() {
		return new MysqlDatabase();
	}

	static void _register() {
		DatabaseManager::_register_db_creation_func("mysql", MysqlDatabase::_creation_func);
	}

	static void _unregister() {
		DatabaseManager::_unregister_db_creation_func("mysql");
	}

	MysqlDatabase() : Database() {
		mysql = new MYSQL();
	}
	~MysqlDatabase() {
		delete mysql;
	}

	MYSQL *mysql;
};

#undef IS_NUM

#endif