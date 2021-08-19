#include "sqlite3_query_builder.h"

#include "sqlite3_database.h"
#include "sqlite3_query_result.h"

QueryBuilder *SQLite3QueryBuilder::select() {
	query_result += "SELECT ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::where() {
	query_result += "WHERE ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::from() {
	query_result += "FROM ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::insert() {
	query_result += "INSERT INTO ";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::values() {
	query_result += "VALUES(";

	return this;
}
QueryBuilder *SQLite3QueryBuilder::cvalues() {
	query_result += ") ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::select(const std::string &params) {
	query_result += "SELECT " + params + " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::where(const std::string &params) {
	query_result += "WHERE " + params + " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::from(const std::string &params) {
	query_result += "FROM " + params + " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::insert(const std::string &table_name) {
	query_result += "INSERT INTO " + table_name + " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::values(const std::string &params_str) {
	query_result += "VALUES(" + params_str + ") ";

	return this;
}

std::string SQLite3QueryBuilder::escape(const std::string &params) {
	if (!_db) {
		printf("SQLite3QueryBuilder::escape !db!\n");

		return "";
	}

	return _db->escape(params);
}

QueryBuilder *SQLite3QueryBuilder::prepare() {
	return this;
}

QueryBuilder *SQLite3QueryBuilder::set_param(const int index, const std::string &value) {
	return this;
}
QueryBuilder *SQLite3QueryBuilder::set_param(const int index, const int value) {
	return this;
}
QueryBuilder *SQLite3QueryBuilder::set_param(const int index, const float value) {
	return this;
}

void SQLite3QueryBuilder::end_command() {
	query_result += ";";
}

QueryResult *SQLite3QueryBuilder::run() {
	end_command();

	if (!_db) {
		printf("SQLite3QueryBuilder::run !db!\n");

		return nullptr;
	}

	return _db->query(query_result);
}

void SQLite3QueryBuilder::run_query() {
	end_command();

	if (!_db) {
		printf("SQLite3QueryBuilder::run_query !db!\n");

		return;
	}

	_db->query_run(query_result);
}

QueryBuilder *SQLite3QueryBuilder::limit(const int num) {
	//query_result += "LIMIT " + num + " ";

	return this;
}

QueryBuilder *SQLite3QueryBuilder::offset(const int num) {
	//query_result += "OFFSET " + num + " ";

	return this;
}

SQLite3QueryBuilder::SQLite3QueryBuilder() {
}
SQLite3QueryBuilder::~SQLite3QueryBuilder() {
}