#include "mysql_table_builder.h"

TableBuilder *MysqlTableBuilder::create_table(const std::string &name) {
	result += "CREATE TABLE " + name + " ( ";

	return this;
}

TableBuilder *MysqlTableBuilder::integer(const std::string &name) {
	result += name + " INTEGER ";

	return this;
}

TableBuilder *MysqlTableBuilder::date(const std::string &name) {
	result += name + " DATE ";

	return this;
}

TableBuilder *MysqlTableBuilder::varchar(const std::string &name, const int length) {
	result += name + " VARCHAR(" + std::to_string(length) + ")";

	return this;
}

TableBuilder *MysqlTableBuilder::not_null() {
	result += "NOT NULL ";

	return this;
}

TableBuilder *MysqlTableBuilder::null() {
	result += "NULL ";

	return this;
}

TableBuilder *MysqlTableBuilder::auto_increment() {
	result += "AUTO_INCREMENT ";

	return this;
}

TableBuilder *MysqlTableBuilder::primary_key(const std::string &name) {
	result += "PRIMARY KEY (" + name + ") ";

	return this;
}

TableBuilder *MysqlTableBuilder::primary_key() {
	result += "PRIMARY KEY ";

	return this;
}

TableBuilder *MysqlTableBuilder::next_row() {
	result += ", ";

	return this;
}

void MysqlTableBuilder::finalize() {
	result += ");";
}

TableBuilder *MysqlTableBuilder::drop_table(const std::string &name) {
	result += "DROP TABLE " + name + ";";

	return this;
}

MysqlTableBuilder::MysqlTableBuilder() {
}

MysqlTableBuilder::~MysqlTableBuilder() {
}