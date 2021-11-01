#include "query_builder.h"

#include "query_result.h"

QueryBuilder *QueryBuilder::select() {
	return this;
}
QueryBuilder *QueryBuilder::update() {
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

QueryBuilder *QueryBuilder::select(const String &params) {
	return nselect(escape(params));
}
QueryBuilder *QueryBuilder::update(const String &params) {
	return nupdate(escape(params));
}
QueryBuilder *QueryBuilder::del(const String &params) {
	return ndel(escape(params));
}
QueryBuilder *QueryBuilder::where(const String &params) {
	return nwhere(escape(params));
}

QueryBuilder *QueryBuilder::from(const String &params) {
	return nfrom(escape(params));
}

QueryBuilder *QueryBuilder::insert(const String &table_name) {
	return this;
}
QueryBuilder *QueryBuilder::insert(const String &table_name, const String &columns) {
	return this;
}
QueryBuilder *QueryBuilder::values(const String &params_str) {
	return nvalues(escape(params_str));
}

QueryBuilder *QueryBuilder::val() {
	return this;
}

QueryBuilder *QueryBuilder::val(const String &param) {
	return nval(escape(param));
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

QueryBuilder *QueryBuilder::like(const String &str) {
	return nlike(escape(str));
}

QueryBuilder *QueryBuilder::set() {
	return this;
}
QueryBuilder *QueryBuilder::cset() {
	return this;
}
QueryBuilder *QueryBuilder::setp(const String &col, const String &param) {
	return nsetp(col, escape(param));
}
QueryBuilder *QueryBuilder::setp(const String &col, const char *param) {
	return this;
}
QueryBuilder *QueryBuilder::setp(const String &col, const int param) {
	return this;
}
QueryBuilder *QueryBuilder::setp(const String &col, const bool param) {
	return this;
}

QueryBuilder *QueryBuilder::wp(const String &col, const String &param) {
	return nwp(col, escape(param));
}
QueryBuilder *QueryBuilder::wp(const String &col, const char *param) {
	return this;
}
QueryBuilder *QueryBuilder::wp(const String &col, const int param) {
	return this;
}
QueryBuilder *QueryBuilder::wp(const String &col, const bool param) {
	return this;
}

QueryBuilder *QueryBuilder::nselect(const String &params) {
	return this;
}
QueryBuilder *QueryBuilder::nupdate(const String &params) {
	return this;
}
QueryBuilder *QueryBuilder::ndel(const String &params) {
	return this;
}

QueryBuilder *QueryBuilder::nwhere(const String &params) {
	return this;
}
QueryBuilder *QueryBuilder::nfrom(const String &params) {
	return this;
}
QueryBuilder *QueryBuilder::nlike(const String &str) {
	return this;
}
QueryBuilder *QueryBuilder::nvalues(const String &params_str) {
	return this;
}

QueryBuilder *QueryBuilder::nval(const String &param) {
	return val(escape(param));
}
QueryBuilder *QueryBuilder::nsetp(const String &col, const String &escape_param) {
	return setp(col, escape(escape_param));
}

QueryBuilder *QueryBuilder::nwp(const String &col, const String &escape_param) {
	return this;
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

QueryBuilder *QueryBuilder::w(const String &str) {
	query_result += str + " ";

	return this;
}
QueryBuilder *QueryBuilder::ew(const String &str) {
	return w(escape(str));
}

QueryBuilder *QueryBuilder::select_last_insert_id() {
	return this;
}

String QueryBuilder::escape(const String &params) {
	return params;
}

QueryBuilder *QueryBuilder::prepare() {
	return this;
}
QueryBuilder *QueryBuilder::set_param(const int index, const String &value) {
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

String QueryBuilder::get_result() {
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