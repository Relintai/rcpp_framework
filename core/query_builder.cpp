#include "query_builder.h"

QueryBuilder *QueryBuilder::select(const std::string &params) {
	return this;
}

QueryBuilder *QueryBuilder::where(const std::string &params) {
	return this;
}

QueryBuilder *QueryBuilder::from(const std::string &params) {
	return this;
}

QueryBuilder *QueryBuilder::limit(const int min, const int max) {
	return this;
}

QueryBuilder *QueryBuilder::insert(const std::string &table_name, const std::string &params_str) {
	return this;
}

void QueryBuilder::finalize() {
}

QueryBuilder::QueryBuilder() {
}

QueryBuilder::~QueryBuilder() {
}