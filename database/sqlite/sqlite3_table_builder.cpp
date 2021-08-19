#include "sqlite3_table_builder.h"

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
	result += name + " VARCHAR(" + std::to_string(length) + ")";

	return this;
}

TableBuilder *SQLite3TableBuilder::not_null() {
	result += "NOT NULL ";

	return this;
}

TableBuilder *SQLite3TableBuilder::null() {
	result += "NULL ";

	return this;
}

TableBuilder *SQLite3TableBuilder::auto_increment() {
	result += "AUTO_INCREMENT ";

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

void SQLite3TableBuilder::finalize() {
	result += ");";
}

TableBuilder *SQLite3TableBuilder::drop_table(const std::string &name) {
	result += "DROP TABLE " + name + ";";

	return this;
}

SQLite3TableBuilder::SQLite3TableBuilder() {
}

SQLite3TableBuilder::~SQLite3TableBuilder() {
}