#ifndef MYSQL_QUERY_RESULT_H
#define MYSQL_QUERY_RESULT_H

#include "core/query_result.h"

#include <sqlite3.h>

class Sqlite3QueryResult : public QueryResult {
public:
	bool next_row();
	const char* get_cell(const int index);

	Sqlite3QueryResult();
	~Sqlite3QueryResult();
};

#endif