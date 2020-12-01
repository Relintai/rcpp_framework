#ifndef PGSQL_CONNECTION
#define PGSQL_CONNECTION

#include "core/database.h"
#include "core/database_manager.h"

//Brynet has it aswell, and because of using namespace it is defined here aswell
//later this will be fixed better
//#ifdef IS_NUM
//#undef IS_NUM
//#endif

#include <libpq-fe.h>

class PGSQLDatabase : public Database {
public:
	static Database *_creation_func() {
		return new PGSQLDatabase();
	}

	static void _register() {
		DatabaseManager::_register_db_creation_func("pgsql", PGSQLDatabase::_creation_func);
	}

	static void _unregister() {
		DatabaseManager::_unregister_db_creation_func("pgsql");
	}

	PGSQLDatabase() : Database() {
		conn = PQconnectStart("");
	}
	~PGSQLDatabase() {
		PQfinish(conn);
	}

	PGconn *conn;
};

//#undef IS_NUM

#endif