#ifndef TABLE_BUILDER_H
#define TABLE_BUILDER_H

#include <string>

class TableBuilder {
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

	virtual TableBuilder *drop_table(const std::string &name);

	virtual void finalize();

	TableBuilder();
	virtual ~TableBuilder();

	std::string result;
};

#endif