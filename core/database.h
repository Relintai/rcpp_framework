#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>

enum QueryErrorCode {
	OK,
	ERROR
};

class QueryBuilder;
class TableBuilder;
class QueryResult;

class Database {
public:
	//probably needs refcount, depending on what db engines do, todo
	//virtual QueryResult *query(const std::string &sql);
	//virtual QueryResult *query_async(const std::string &sql);
	//or
	//virtual QueryErrorCode query(QueryResult *result, const std::string &sql);
	//virtual QueryErrorCode query_async(QueryResult *result, const std::string &sql);

	//also
	//virtual QueryResult *query_prepared(const std::string &sql, param1, param2, ...);

    //query interface (codeigniter 3 style)
    //virtual void where(""); etc

	virtual void connect(const std::string &connection_str);
	virtual QueryResult *query(const std::string &query);
	virtual void query_run(const std::string &query);

	virtual QueryBuilder *get_query_builder();
	virtual TableBuilder *get_table_builder();

	Database();
	~Database();

private:
	//std::vector<QueryBuilder *> _builders;
};

#endif