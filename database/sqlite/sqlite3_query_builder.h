#ifndef SQLITE3_QUERY_BUILDER_H
#define SQLITE3_QUERY_BUILDER_H

#include <memory>
#include <string>

#include "core/database/query_builder.h"

class SQLite3Database;

class SQLite3QueryBuilder : public QueryBuilder {
public:
	QueryBuilder *select();
	QueryBuilder *udpate();
	QueryBuilder *del();

	QueryBuilder *where();
	QueryBuilder *from();
	QueryBuilder *insert();
	QueryBuilder *values();
	QueryBuilder *cvalues();

	QueryBuilder *str();
	QueryBuilder *cstr();

	QueryBuilder *like();

	QueryBuilder *select(const std::string &params);
	QueryBuilder *udpate(const std::string &params);
	QueryBuilder *del(const std::string &params);

	QueryBuilder *where(const std::string &params);
	QueryBuilder *from(const std::string &params);
	QueryBuilder *insert(const std::string &table_name);
	QueryBuilder *insert(const std::string &table_name, const std::string &columns);
	QueryBuilder *values(const std::string &params_str);
	QueryBuilder *val();
	QueryBuilder *val(const std::string &param);
	QueryBuilder *val(const char *param);
	QueryBuilder *val(const int param);
	QueryBuilder *val(const bool param);

	QueryBuilder *like(const std::string &str);

	QueryBuilder *set();
	QueryBuilder *cset();
	QueryBuilder *setp(const std::string &col, const std::string &param);
	QueryBuilder *setp(const std::string &col, const char *param);
	QueryBuilder *setp(const std::string &col, const int param);
	QueryBuilder *setp(const std::string &col, const bool param);

	QueryBuilder *wp(const std::string &col, const std::string &param);
	QueryBuilder *wp(const std::string &col, const char *param);
	QueryBuilder *wp(const std::string &col, const int param);
	QueryBuilder *wp(const std::string &col, const bool param);

	QueryBuilder *limit(const int num);
	QueryBuilder *offset(const int num);

	//l=logical (and, or are operators)
	QueryBuilder *land();
	QueryBuilder *lor();

	QueryBuilder *wildcard();

	QueryBuilder *select_last_insert_id();

	std::string escape(const std::string &params);

	QueryBuilder *prepare();
	QueryBuilder *set_param(const int index, const std::string &value);
	QueryBuilder *set_param(const int index, const int value);
	QueryBuilder *set_param(const int index, const float value);

	void end_command();

	Ref<QueryResult> run();
	void run_query();

	SQLite3QueryBuilder();
	~SQLite3QueryBuilder();

	SQLite3Database *_db;
};

#endif