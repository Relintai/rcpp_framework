#include "sqlite3_database.h"

#include "core/database/database_manager.h"

#include "sqlite3_query_result.h"

Database *SQLite3Database::_creation_func() {
	return new SQLite3Database();
}

void SQLite3Database::_register() {
	DatabaseManager::_register_db_creation_func("sqlite", SQLite3Database::_creation_func);
}

void SQLite3Database::_unregister() {
	DatabaseManager::_unregister_db_creation_func("sqlite");
}

void SQLite3Database::connect(const std::string &connection_str) {
	int ret = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
	if (ret != SQLITE_OK) {
		printf("SQLITE3 multithreading is not supported!\n");
	}

	ret = sqlite3_open(connection_str.c_str(), &conn);
}

QueryResult *SQLite3Database::query(const std::string &query) {
	Sqlite3QueryResult *res = new Sqlite3QueryResult();

	res->query(query, conn);

	return res;
}

void SQLite3Database::query_run(const std::string &query) {
	char *err_msg;

    if (sqlite3_exec(conn, query.c_str(), NULL, NULL, &err_msg) != SQLITE_OK) {
		printf("SQLite3Database::query_run error: \nQuery: %s \n Error:\n %s\n", query.c_str(), err_msg);
		sqlite3_free(err_msg);
	}
}

SQLite3Database::SQLite3Database() :
		Database() {
}

SQLite3Database::~SQLite3Database() {
	if (conn)
		sqlite3_close(conn);
}
