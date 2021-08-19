#include "query_builder.h"

#include "query_result.h"

QueryBuilder *QueryBuilder::select() {
	return this;
}
QueryBuilder *QueryBuilder::where() {

	return this;
}
QueryBuilder *QueryBuilder::from() {
	return this;
}
QueryBuilder *QueryBuilder::insert() {
	return this;
}
QueryBuilder *QueryBuilder::values() {
	return this;
}
QueryBuilder *QueryBuilder::cvalues() {
	return this;
}

QueryBuilder *QueryBuilder::select(const std::string &params) {
	return this;
}

QueryBuilder *QueryBuilder::where(const std::string &params) {
	return this;
}

QueryBuilder *QueryBuilder::from(const std::string &params) {
	return this;
}

QueryBuilder *QueryBuilder::insert(const std::string &table_name) {
	return this;
}
QueryBuilder *QueryBuilder::values(const std::string &params_str) {
	return this;
}

QueryBuilder *QueryBuilder::eselect(const std::string &params) {
	select(escape(params));

	return this;
}
QueryBuilder *QueryBuilder::ewhere(const std::string &params) {
	where(escape(params));

	return this;
}
QueryBuilder *QueryBuilder::efrom(const std::string &params) {
	from(escape(params));

	return this;
}
QueryBuilder *QueryBuilder::einsert(const std::string &table_name) {
	insert(escape(table_name));

	return this;
}
QueryBuilder *QueryBuilder::evalues(const std::string &params_str) {
	values(escape(params_str));

	return this;
}

QueryBuilder *QueryBuilder::limit(const int min, const int max) {
	return this;
}

std::string QueryBuilder::escape(const std::string &params) {
	return params;
}

QueryBuilder *QueryBuilder::prepare() {
	return this;
}
QueryBuilder *QueryBuilder::set_param(const int index, const std::string &value) {
	return this;
}
QueryBuilder *QueryBuilder::set_param(const int index, const int value) {
	return this;
}
QueryBuilder *QueryBuilder::set_param(const int index, const float value) {
	return this;
}

void QueryBuilder::end_command() {
}

QueryResult *QueryBuilder::run() {
	return nullptr;
}

std::string QueryBuilder::get_result() {
	end_command();

	return query_result;
}

QueryBuilder::QueryBuilder() {
}

QueryBuilder::~QueryBuilder() {
}