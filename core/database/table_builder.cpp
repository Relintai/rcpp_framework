#include "table_builder.h"

TableBuilder *TableBuilder::create_table(const std::string &name) {
	return this;
}

TableBuilder *TableBuilder::integer(const std::string &name) {
	return this;
}

TableBuilder *TableBuilder::date(const std::string &name) {
	return this;
}

TableBuilder *TableBuilder::varchar(const std::string &name, const int length) {
	return this;
}

TableBuilder *TableBuilder::not_null() {
	return this;
}

TableBuilder *TableBuilder::null() {
	return this;
}

TableBuilder *TableBuilder::auto_increment() {
	return this;
}

TableBuilder *TableBuilder::primary_key(const std::string &name) {
	return this;
}

TableBuilder *TableBuilder::primary_key() {
	return this;
}

TableBuilder *TableBuilder::next_row() {
	return this;
}

TableBuilder *TableBuilder::drop_table(const std::string &name) {

}

void TableBuilder::finalize() {
}

TableBuilder::TableBuilder() {
}

TableBuilder::~TableBuilder() {
}