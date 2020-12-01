#include "database.h"

void Database::connect(const std::string &connection_str) {
}

void Database::query(const std::string &query) {
}

QueryBuilder *Database::get_query_builder() {
	return new QueryBuilder();
}

TableBuilder *Database::get_table_builder() {
	return new TableBuilder();
}

Database::Database() {
}

Database::~Database() {
}