#include "mysql_query_builder.h"

#include "mysql_database.h"
#include "mysql_query_result.h"

QueryBuilder *MysqlQueryBuilder::select() {
	query_result += "SELECT ";

	return this;
}
QueryBuilder *MysqlQueryBuilder::where() {
	query_result += "WHERE ";

	return this;
}
QueryBuilder *MysqlQueryBuilder::from() {
	query_result += "FROM ";

	return this;
}
QueryBuilder *MysqlQueryBuilder::insert() {
	query_result += "INSERT INTO ";

	return this;
}
QueryBuilder *MysqlQueryBuilder::values() {
	query_result += "VALUES(";

	return this;
}
QueryBuilder *MysqlQueryBuilder::cvalues() {
	query_result += ") ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::select(const std::string &params) {
	query_result += "SELECT " + params + " ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::where(const std::string &params) {
	query_result += "WHERE " + params + " ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::from(const std::string &params) {
	query_result += "FROM " + params + " ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::insert(const std::string &table_name) {
	query_result += "INSERT INTO " + table_name + " ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::values(const std::string &params_str) {
	query_result += "VALUES(" + params_str + ") ";

	return this;
}

QueryBuilder *MysqlQueryBuilder::limit(const int min, const int max) {

	return this;
}

std::string MysqlQueryBuilder::escape(const std::string &params) {
	if (!_db) {
		printf("MysqlQueryBuilder::escape !db!\n");

		return "";
	}

	return _db->escape(params);
}

QueryBuilder *MysqlQueryBuilder::prepare() {
	return this;
}

QueryBuilder *MysqlQueryBuilder::set_param(const int index, const std::string &value) {
	return this;
}
QueryBuilder *MysqlQueryBuilder::set_param(const int index, const int value) {
	return this;
}
QueryBuilder *MysqlQueryBuilder::set_param(const int index, const float value) {
	return this;
}

void MysqlQueryBuilder::end_command() {
	query_result += ";";
}

QueryResult *MysqlQueryBuilder::run() {
	return nullptr;
}

QueryBuilder *MysqlQueryBuilder::limit(const int min, const int max) {
	return this;
}

MysqlQueryBuilder::MysqlQueryBuilder() {
}
MysqlQueryBuilder::~MysqlQueryBuilder() {
}