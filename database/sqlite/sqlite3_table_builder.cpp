#include "sqlite3_table_builder.h"

#include "core/database/query_result.h"

#include "sqlite3_database.h"

TableBuilder *SQLite3TableBuilder::create_table(const std::string &name) {
	result += "CREATE TABLE " + name + " ( ";

	return this;
}

TableBuilder *SQLite3TableBuilder::integer(const std::string &name) {
	result += name + " INTEGER ";

	return this;
}

TableBuilder *SQLite3TableBuilder::date(const std::string &name) {
	result += name + " DATE ";

	return this;
}

TableBuilder *SQLite3TableBuilder::varchar(const std::string &name, const int length) {
	result += name + " VARCHAR(" + std::to_string(length) + ") ";

	return this;
}

TableBuilder *SQLite3TableBuilder::not_null() {
	//result += "NOT NULL ";

	return this;
}

TableBuilder *SQLite3TableBuilder::null() {
	result += "NULL ";

	return this;
}

TableBuilder *SQLite3TableBuilder::auto_increment() {
	//result += "AUTO_INCREMENT ";

	return this;
}

TableBuilder *SQLite3TableBuilder::primary_key(const std::string &name) {
	result += "PRIMARY KEY (" + name + ") ";

	return this;
}

TableBuilder *SQLite3TableBuilder::primary_key() {
	result += "PRIMARY KEY ";

	return this;
}

TableBuilder *SQLite3TableBuilder::next_row() {
	result += ", ";

	return this;
}

TableBuilder *SQLite3TableBuilder::ccreate_table() {
	result += ");";

	return this;
}

TableBuilder *SQLite3TableBuilder::drop_table() {
	result += "DROP TABLE ";

	return this;
}
TableBuilder *SQLite3TableBuilder::drop_table_if_exists() {
	result += "DROP TABLE IF EXISTS ";

	return this;
}
TableBuilder *SQLite3TableBuilder::drop_table(const std::string &name) {
	result += "DROP TABLE " + name + ";";

	return this;
}
TableBuilder *SQLite3TableBuilder::drop_table_if_exists(const std::string &name) {
	result += "DROP TABLE IF EXISTS " + name + ";";

	return this;
}
TableBuilder *SQLite3TableBuilder::cdrop_table() {
	result += ";";

	return this;
}

TableBuilder *SQLite3TableBuilder::foreign_key(const std::string &name) {
	result += "FOREIGN KEY (" + name + ") ";

	return this;
}
TableBuilder *SQLite3TableBuilder::references(const std::string &table, const std::string &name) {
	result += "REFERENCES " + table + " (" + name + ") ";

	return this;
}

Ref<QueryResult> SQLite3TableBuilder::run() {
	if (!_db) {
		printf("SQLite3TableBuilder::run !db!\n");

		return nullptr;
	}

	return _db->query(result);
}

void SQLite3TableBuilder::run_query() {
	if (!_db) {
		printf("SQLite3TableBuilder::run_query !db!\n");

		return;
	}

	_db->query_run(result);
}

SQLite3TableBuilder::SQLite3TableBuilder() {
}

SQLite3TableBuilder::~SQLite3TableBuilder() {
}