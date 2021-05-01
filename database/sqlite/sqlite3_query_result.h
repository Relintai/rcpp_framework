#ifndef MYSQL_QUERY_RESULT_H
#define MYSQL_QUERY_RESULT_H

#include <string>
#include <vector>

#include "core/query_result.h"

#include <sqlite3.h>

class Sqlite3QueryResult : public QueryResult {
public:
	bool next_row();
	const char* get_cell(const int index);

	void query(const std::string &query, sqlite3 *conn);

	static int run_query_finished(void *data, int argc, char **argv, char **col_names);

	Sqlite3QueryResult();
	~Sqlite3QueryResult();

	char* err_msg;

public:
	struct Sqlite3QueryResultRow {
		std::vector<std::string> cells;
	};

	char **col_names;
	std::vector<Sqlite3QueryResultRow *> rows;
	int current_row;
};

#endif