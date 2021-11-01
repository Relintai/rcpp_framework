#include "mysql_database.h"

#include <cstdio>

#include "core/database/database_manager.h"

#include "mysql_query_builder.h"
#include "mysql_query_result.h"
#include "mysql_table_builder.h"

void MysqlDatabase::connect(const std::string &connection_str) {
	mysql = mysql_init(mysql);
	mysql_options(mysql, MYSQL_OPT_NONBLOCK, 0);

	std::string host = "127.0.0.1";
	std::string user = "";
	std::string password = "";
	std::string dbname = "testappdb";
	int port = 3306;

	mysql = mysql_real_connect(mysql, host.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, NULL, 0);

	if (mysql) {
		printf("mysql connected\n");
	}
}

Ref<QueryResult> MysqlDatabase::query(const std::string &query) {
	if (!mysql)
		return nullptr;

	//printf("%s\n", query.c_str());

	int error = mysql_real_query(mysql, query.c_str(), query.length());

	if (error) {
		const char *merr = mysql_error(mysql);

		printf("MySQL error: %s\n", merr);

		return nullptr;
	}

	MYSQL_RES *result = mysql_use_result(mysql);

	MysqlQueryResult *res = new MysqlQueryResult();

	res->result = result;
	//res->next_row();

	return Ref<QueryResult>(res);
}

void MysqlDatabase::query_run(const std::string &query) {
	if (!mysql)
		return;

	//printf("%s\n", query.c_str());

	int error = mysql_real_query(mysql, query.c_str(), query.length());

	if (error) {
		const char *merr = mysql_error(mysql);

		printf("MySQL error: %s\n", merr);
		return;
	}

	//printf("query OK\n");
	//printf("----------------\n");

	/*
	printf("----------------\n");

	MYSQL_RES *result = mysql_use_result(mysql);
	//MYSQL_RES *result = mysql_store_result(mysql);

	MYSQL_ROW row;
	while (row = mysql_fetch_row(result)) {
		printf("%s\n", row[0]);
		//printf("%s\n", row[1]);
	}

	printf("----------------\n");

	mysql_free_result(result);
	*/
}

Ref<QueryBuilder> MysqlDatabase::get_query_builder() {
	MysqlQueryBuilder *b = new MysqlQueryBuilder();
	b->_db = this;

	return Ref<QueryBuilder>(b);
}

Ref<TableBuilder> MysqlDatabase::get_table_builder() {
	return Ref<TableBuilder>(new MysqlTableBuilder());
}

std::string MysqlDatabase::escape(const std::string str) {
	std::string res;
	res.reserve(str.size() + 100);

	mysql_real_escape_string(mysql, res.data(), str.c_str(), str.size());

	return res;
}
void MysqlDatabase::escape(const std::string str, std::string *to) {
	to->reserve(str.size() + 100);

	mysql_real_escape_string(mysql, to->data(), str.c_str(), str.size());
}

MysqlDatabase::MysqlDatabase() :
		Database() {

	mysql = nullptr;
}

MysqlDatabase::~MysqlDatabase() {
	mysql_close(mysql);

	delete mysql;
}

Database *MysqlDatabase::_creation_func() {
	return new MysqlDatabase();
}

void MysqlDatabase::_register() {
	DatabaseManager::_register_db_creation_func("mysql", MysqlDatabase::_creation_func);
}

void MysqlDatabase::_unregister() {
	DatabaseManager::_unregister_db_creation_func("mysql");
}