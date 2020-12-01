#ifndef SQLITE3_CONNECTION
#define SQLITE3_CONNECTION

#include "core/database.h"
#include "core/database_manager.h"

//Brynet has it aswell, and because of using namespace it is defined here aswell
//later this will be fixed better
//#ifdef IS_NUM
//#undef IS_NUM
//#endif

#include <cstdio>

#include <sqlite3.h>

class SQLite3Database : public Database {
public:
	static Database *_creation_func() {
		return new SQLite3Database();
	}

	static void _register() {
		DatabaseManager::_register_db_creation_func("sqlite", SQLite3Database::_creation_func);
	}

	static void _unregister() {
		DatabaseManager::_unregister_db_creation_func("sqlite");
	}

	SQLite3Database() :
			Database() {

		int ret = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
		if (ret != SQLITE_OK) {
			printf("SQLITE3 multithreading is not supported!\n");
		}

		ret = sqlite3_open("", &conn);
	}

	~SQLite3Database() {
		sqlite3_close(conn);
	}

	sqlite3 *conn;
};

//#undef IS_NUM

#endif