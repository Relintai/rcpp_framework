#ifndef SQLITE3_TABLE_BUILDER_H
#define SQLITE3_TABLE_BUILDER_H

#include <string>

#include "core/database/table_builder.h"

class SQLite3TableBuilder : public TableBuilder {
public:
	TableBuilder *create_table(const std::string &name);
	TableBuilder *integer(const std::string &name);
	TableBuilder *date(const std::string &name);
	TableBuilder *varchar(const std::string &name, const int length);
	TableBuilder *not_null();
	TableBuilder *null();
	TableBuilder *auto_increment();
	TableBuilder *primary_key(const std::string &name);
	TableBuilder *primary_key();
	TableBuilder *next_row();

	TableBuilder *drop_table(const std::string &name);

	void finalize();

	SQLite3TableBuilder();
	virtual ~SQLite3TableBuilder();
};

#endif