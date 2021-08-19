#include "sqlite3_query_result.h"

#include <cstdio>

bool Sqlite3QueryResult::next_row() {
	return ++current_row < rows.size();
}

const char *Sqlite3QueryResult::get_cell(const int index) {
	return rows[current_row]->cells[index].c_str();
}

int Sqlite3QueryResult::get_last_insert_rowid() {
	return sqlite3_last_insert_rowid(_connection);
}

void Sqlite3QueryResult::query(const std::string &query, sqlite3 *conn) {
	_connection = conn;

	if (sqlite3_exec(conn, query.c_str(), Sqlite3QueryResult::run_query_finished, this, &err_msg) != SQLITE_OK) {
		printf("SQLite3Database::query error: \nQuery: %s \n Error:\n %s\n", query.c_str(), err_msg);
		sqlite3_free(err_msg);
	}
}

int Sqlite3QueryResult::run_query_finished(void *data, int argc, char **argv, char **col_names) {
	Sqlite3QueryResult *res = reinterpret_cast<Sqlite3QueryResult *>(data);

	//res->col_names = col_names;

	Sqlite3QueryResultRow *r = new Sqlite3QueryResultRow();

	for (int i = 0; i < argc; ++i) {
		if (argv[i]) {
			r->cells.push_back(argv[i]);
		} else {
			//todo mark nulls in cells
			r->cells.push_back("NULL");
		}
	}

	res->rows.push_back(r);

	return 0;
}

Sqlite3QueryResult::Sqlite3QueryResult() :
		QueryResult() {
	err_msg = nullptr;
	current_row = -1;
}

Sqlite3QueryResult::~Sqlite3QueryResult() {
	for (int i = 0; i < rows.size(); ++i) {
		delete rows[i];
	}
}