#include "mysql_query_builder.h"


QueryBuilder *MysqlQueryBuilder::select(const std::string &params) {
	query_result += "SELECT " + params;

	return this;
}

QueryBuilder *MysqlQueryBuilder::where(const std::string &params) {
	query_result += " WHERE " + params;

	return this;
}

QueryBuilder *MysqlQueryBuilder::from(const std::string &params) {
	query_result += " FROM " + params;

	return this;
}

QueryBuilder *MysqlQueryBuilder::limit(const int min, const int max) {
	return this;
}

QueryBuilder *MysqlQueryBuilder::insert(const std::string &table_name, const std::string &params_str) {
	query_result += " INSERT INTO " + table_name + " VALUES( " + params_str + " );";

	return this;
}

void MysqlQueryBuilder::finalize() {
	query_result += ";";
}

MysqlQueryBuilder::MysqlQueryBuilder() {

}
MysqlQueryBuilder::~MysqlQueryBuilder() {
	
}