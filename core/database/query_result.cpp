#include "query_result.h"

bool QueryResult::next_row() {
	return false;
}

const char *QueryResult::get_cell(const int index) {
	return "";
}

int QueryResult::get_last_insert_rowid() {
	return 0;
}

QueryResult::QueryResult() {
}

QueryResult::~QueryResult() {
}
