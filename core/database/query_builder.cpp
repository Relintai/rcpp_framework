#include "query_builder.h"

#include "query_result.h"

QueryBuilder *QueryBuilder::select() {
	return this;
}
QueryBuilder *QueryBuilder::udpate() {
	return this;
}
QueryBuilder *QueryBuilder::del() {
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

QueryBuilder *QueryBuilder::str() {
	return this;
}
QueryBuilder *QueryBuilder::cstr() {
	return this;
}

QueryBuilder *QueryBuilder::like() {
	return this;
}

QueryBuilder *QueryBuilder::select(const std::string &params) {
	return this;
}
QueryBuilder *QueryBuilder::udpate(const std::string &params) {
	return this;
}
QueryBuilder *QueryBuilder::del(const std::string &params) {
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
QueryBuilder *QueryBuilder::insert(const std::string &table_name, const std::string &columns) {
	return this;
}
QueryBuilder *QueryBuilder::values(const std::string &params_str) {
	return this;
}

QueryBuilder *QueryBuilder::val() {
	return this;
}

QueryBuilder *QueryBuilder::val(const std::string &param) {
	return this;
}

QueryBuilder *QueryBuilder::val(const char *param) {
	return this;
}

QueryBuilder *QueryBuilder::val(const int param) {
	return this;
}
QueryBuilder *QueryBuilder::val(const bool param) {
	return this;
}

QueryBuilder *QueryBuilder::like(const std::string &str) {
	return this;
}

QueryBuilder *QueryBuilder::set() {
	return this;
}
QueryBuilder *QueryBuilder::cset() {
	return this;
}
QueryBuilder *QueryBuilder::setp(const std::string &col, const std::string &param) {
	return this;
}
QueryBuilder *QueryBuilder::setp(const std::string &col, const char *param) {
	return this;
}
QueryBuilder *QueryBuilder::setp(const std::string &col, const int param) {
	return this;
}
QueryBuilder *QueryBuilder::setp(const std::string &col, const bool param) {
	return this;
}

QueryBuilder *QueryBuilder::wp(const std::string &col, const std::string &param) {
	return this;
}
QueryBuilder *QueryBuilder::wp(const std::string &col, const char *param) {
	return this;
}
QueryBuilder *QueryBuilder::wp(const std::string &col, const int param) {
	return this;
}
QueryBuilder *QueryBuilder::wp(const std::string &col, const bool param) {
	return this;
}

QueryBuilder *QueryBuilder::eselect(const std::string &params) {
	return select(escape(params));
}
QueryBuilder *QueryBuilder::eudpate(const std::string &params) {
	return udpate(escape(params));
}
QueryBuilder *QueryBuilder::edel(const std::string &params) {
	return del(escape(params));
}

QueryBuilder *QueryBuilder::ewhere(const std::string &params) {
	return where(escape(params));
}
QueryBuilder *QueryBuilder::efrom(const std::string &params) {
	return from(escape(params));
}
QueryBuilder *QueryBuilder::einsert(const std::string &table_name) {
	return insert(escape(table_name));

	return this;
}
QueryBuilder *QueryBuilder::elike(const std::string &str) {
	return like(escape(str));
}
QueryBuilder *QueryBuilder::evalues(const std::string &params_str) {
	return values(escape(params_str));
}

QueryBuilder *QueryBuilder::eval(const std::string &param) {
	return val(escape(param));
}
QueryBuilder *QueryBuilder::esetp(const std::string &col, const std::string &escape_param) {
	return setp(col, escape(escape_param));
}

QueryBuilder *QueryBuilder::ewp(const std::string &col, const std::string &escape_param) {
	return wp(col, escape(escape_param));
}

QueryBuilder *QueryBuilder::limit(const int num) {
	return this;
}

QueryBuilder *QueryBuilder::offset(const int num) {
	return this;
}

QueryBuilder *QueryBuilder::land() {
	return this;
}
QueryBuilder *QueryBuilder::lor() {
	return this;
}

QueryBuilder *QueryBuilder::wildcard() {
	return this;
}

QueryBuilder *QueryBuilder::w(const std::string &str) {
	query_result += str + " ";

	return this;
}
QueryBuilder *QueryBuilder::ew(const std::string &str) {
	return w(escape(str));
}

QueryBuilder *QueryBuilder::select_last_insert_id() {
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

QueryBuilder *QueryBuilder::reset() {
	query_result = "";

	return this;
}

Ref<QueryResult> QueryBuilder::run() {
	return Ref<QueryResult>();
}

void QueryBuilder::run_query() {
}

std::string QueryBuilder::get_result() {
	end_command();

	return query_result;
}

void QueryBuilder::print() {
	printf("%s\n", query_result.c_str());
}

QueryBuilder::QueryBuilder() {
}

QueryBuilder::~QueryBuilder() {
}