#ifndef MYSQL_QUERY_RESULT_H
#define MYSQL_QUERY_RESULT_H

#include "core/query_result.h"

#include <mysql.h>

class MysqlQueryResult : public QueryResult {
public:
	bool next_row();
	const char* get_cell(const int index);

	MysqlQueryResult();
	~MysqlQueryResult();

	MYSQL_ROW current_row;
	MYSQL_RES *result;
};

#endif