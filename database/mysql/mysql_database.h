#ifndef MYSQL_CONNECTION
#define MYSQL_CONNECTION

#include "core/database/database.h"

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
	Ref<QueryResult> query(const std::string &query);
	void query_run(const std::string &query);

	Ref<QueryBuilder> get_query_builder();
	Ref<TableBuilder> get_table_builder();

	std::string escape(const std::string str);
	void escape(const std::string str, std::string *to);

	static Database *_creation_func();
	static void _register();
	static void _unregister();

	MysqlDatabase();
	~MysqlDatabase();

	MYSQL *mysql;
};

#undef IS_NUM

#endif