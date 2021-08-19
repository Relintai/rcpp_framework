#ifndef MYSQL_QUERY_BUILDER_H
#define MYSQL_QUERY_BUILDER_H

#include <memory>
#include <string>

#include "core/database/query_builder.h"

class MysqlDatabase;
class MysqlQueryResult;

class MysqlQueryBuilder : public QueryBuilder {
public:
	QueryBuilder *select();
	QueryBuilder *where();
	QueryBuilder *from();
	QueryBuilder *insert();
	QueryBuilder *values();
	QueryBuilder *cvalues();

	QueryBuilder *select(const std::string &params);
	QueryBuilder *where(const std::string &params);
	QueryBuilder *from(const std::string &params);
	QueryBuilder *insert(const std::string &table_name);
	QueryBuilder *values(const std::string &params_str);

	QueryBuilder *limit(const int min, const int max);

	std::string escape(const std::string &params);

	QueryBuilder *prepare();
	QueryBuilder *set_param(const int index, const std::string &value);
	QueryBuilder *set_param(const int index, const int value);
	QueryBuilder *set_param(const int index, const float value);

	void end_command();

	QueryResult *run();

	MysqlQueryBuilder();
	~MysqlQueryBuilder();

	MysqlDatabase *_db;
	MysqlQueryResult *_result;
};

#endif