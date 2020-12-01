#ifndef QUERY_BUILDER_H
#define QUERY_BUILDER_H

#include <string>

class QueryBuilder {
public:
	virtual QueryBuilder *select(const std::string &params);
	virtual QueryBuilder *where(const std::string &params);
	virtual QueryBuilder *from(const std::string &params);
	virtual QueryBuilder *limit(const int min, const int max);
	virtual QueryBuilder *insert(const std::string &table_name, const std::string &params_str);
	virtual void finalize();

	QueryBuilder();
	virtual ~QueryBuilder();

	std::string query_result;
};

#endif