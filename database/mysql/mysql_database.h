#ifndef MYSQL_CONNECTION
#define MYSQL_CONNECTION

#include "core/database.h"

#include <memory>

//Brynet has it aswell, and because of using namespace it is defined here aswell
//later this will be fixed better
#ifdef IS_NUM
#undef IS_NUM
#endif

#include <mysql.h>

class MysqlDatabase : public Database {
public:
	void connect(const std::string &connection_str);
	QueryResult *query(const std::string &query);
	void query_run(const std::string &query);

	QueryBuilder *get_query_builder();
	TableBuilder *get_table_builder();

	static Database *_creation_func();
	static void _register();
	static void _unregister();

	MysqlDatabase();
	~MysqlDatabase();

	MYSQL *mysql;
};

#undef IS_NUM

#endif