#include "query_result.h"

bool QueryResult::next_row() {
	return false;
}

const char *QueryResult::get_cell(const int index) {
	return "";
}

QueryResult::QueryResult() {
}

QueryResult::~QueryResult() {
}
