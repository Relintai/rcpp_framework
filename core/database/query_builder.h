#ifndef QUERY_BUILDER_H
#define QUERY_BUILDER_H

#include <string>

class QueryResult;

//methods that start with an e escape their params.

class QueryBuilder {
public:
	virtual QueryBuilder *select();
	virtual QueryBuilder *where();
	virtual QueryBuilder *from();
	virtual QueryBuilder *insert();
	virtual QueryBuilder *values();
	virtual QueryBuilder *cvalues();

	virtual QueryBuilder *select(const std::string &params);
	virtual QueryBuilder *where(const std::string &params);
	virtual QueryBuilder *from(const std::string &params);
	virtual QueryBuilder *insert(const std::string &table_name);
	virtual QueryBuilder *values(const std::string &params_str);

	virtual QueryBuilder *eselect(const std::string &params);
	virtual QueryBuilder *ewhere(const std::string &params);
	virtual QueryBuilder *efrom(const std::string &params);
	virtual QueryBuilder *einsert(const std::string &table_name);
	virtual QueryBuilder *evalues(const std::string &params_str);

	virtual QueryBuilder *limit(const int num);
	virtual QueryBuilder *offset(const int num);

	virtual std::string escape(const std::string &params);

	virtual QueryBuilder *prepare();
	virtual QueryBuilder *set_param(const int index, const std::string &value);
	virtual QueryBuilder *set_param(const int index, const int value);
	virtual QueryBuilder *set_param(const int index, const float value);

	virtual void end_command();

	virtual QueryResult *run();
	virtual void run_query();

	std::string get_result();

	QueryBuilder();
	virtual ~QueryBuilder();

	std::string query_result;
};

#endif