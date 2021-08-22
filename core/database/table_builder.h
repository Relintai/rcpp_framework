#ifndef TABLE_BUILDER_H
#define TABLE_BUILDER_H

#include <string>

#include "core/reference.h"

class QueryResult;

class TableBuilder : public Reference {
public:
	virtual TableBuilder *create_table(const std::string &name);
	virtual TableBuilder *integer(const std::string &name);
	virtual TableBuilder *date(const std::string &name);
	virtual TableBuilder *varchar(const std::string &name, const int length);
	virtual TableBuilder *not_null();
	virtual TableBuilder *null();
	virtual TableBuilder *auto_increment();
	virtual TableBuilder *primary_key(const std::string &name);
	virtual TableBuilder *primary_key();
	virtual TableBuilder *next_row();
	virtual TableBuilder *ccreate_table();

	virtual TableBuilder *drop_table();
	virtual TableBuilder *drop_table_if_exists();
	virtual TableBuilder *drop_table(const std::string &name);
	virtual TableBuilder *drop_table_if_exists(const std::string &name);
	virtual TableBuilder *cdrop_table();

	virtual TableBuilder *foreign_key(const std::string &name);
	virtual TableBuilder *references(const std::string &table, const std::string &name);

	virtual QueryResult *run();
	virtual void run_query();

	void print();

	TableBuilder();
	virtual ~TableBuilder();

	std::string result;
};

#endif