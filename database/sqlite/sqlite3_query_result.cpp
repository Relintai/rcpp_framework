#include "sqlite3_query_result.h"

#include <cstdio>

bool Sqlite3QueryResult::next_row() {
    return false;
}

const char* Sqlite3QueryResult::get_cell(const int index) {
    return nullptr;
}

Sqlite3QueryResult::Sqlite3QueryResult() : QueryResult() {
}

Sqlite3QueryResult::~Sqlite3QueryResult() {
}