#ifndef MYSQL_TABLE_BUILDER_H
#define MYSQL_TABLE_BUILDER_H

#include <string>

#include "core/table_builder.h"

class MysqlTableBuilder : public TableBuilder {
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

	MysqlTableBuilder();
	virtual ~MysqlTableBuilder();
};

#endif