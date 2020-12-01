#ifndef DATABASE_H
#define DATABASE_H

#include <string>

enum QueryErrorCode {
	OK,
	ERROR
};

class QueryResult {
public:
};

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

	Database();
	~Database();
};

#endif