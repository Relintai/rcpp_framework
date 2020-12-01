#include "database.h"

void Database::connect(const std::string &connection_str) {
}

void Database::query(const std::string &query) {
}

QueryBuilder *Database::get_builder() {
	return new QueryBuilder();
}

Database::Database() {
}

Database::~Database() {
}