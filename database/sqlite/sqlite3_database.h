#ifndef SQLITE3_CONNECTION
#define SQLITE3_CONNECTION

#include "core/database/database.h"

//Brynet has it aswell, and because of using namespace it is defined here aswell
//later this will be fixed better
//#ifdef IS_NUM
//#undef IS_NUM
//#endif

#include <cstdio>

#include "./sqlite/sqlite3.h"

class SQLite3Database : public Database {
public:
	static Database *_creation_func();
	static void _register();
	static void _unregister();

	QueryBuilder *get_query_builder();

	void connect(const std::string &connection_str);
	QueryResult *query(const std::string &query);
	void query_run(const std::string &query);

	std::string escape(const std::string str);
	void escape(const std::string str, std::string *to);

	SQLite3Database();
	~SQLite3Database();

	sqlite3 *conn;
};

//#undef IS_NUM

#endif