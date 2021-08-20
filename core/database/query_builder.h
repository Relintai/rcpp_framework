#ifndef QUERY_BUILDER_H
#define QUERY_BUILDER_H

#include <string>

class QueryResult;

//methods that start with an e escape their params.

class QueryBuilder {
public:
	virtual QueryBuilder *select();
	virtual QueryBuilder *udpate();
	virtual QueryBuilder *del();

	virtual QueryBuilder *where();
	virtual QueryBuilder *from();
	virtual QueryBuilder *insert();
	virtual QueryBuilder *values();
	virtual QueryBuilder *cvalues();

	virtual QueryBuilder *select(const std::string &params);
	virtual QueryBuilder *udpate(const std::string &params);
	virtual QueryBuilder *del(const std::string &params);

	virtual QueryBuilder *where(const std::string &params);
	virtual QueryBuilder *from(const std::string &params);
	virtual QueryBuilder *insert(const std::string &table_name);
	virtual QueryBuilder *insert(const std::string &table_name, const std::string &columns);
	virtual QueryBuilder *values(const std::string &params_str);
	virtual QueryBuilder *val();
	virtual QueryBuilder *val(const std::string &param);
	virtual QueryBuilder *val(const char *param);
	virtual QueryBuilder *val(const int param);
	virtual QueryBuilder *val(const bool param);

	virtual QueryBuilder *set();
	virtual QueryBuilder *cset();

	virtual QueryBuilder *setp(const std::string &col, const std::string &param);
	virtual QueryBuilder *setp(const std::string &col, const char *param);
	virtual QueryBuilder *setp(const std::string &col, const int param);
	virtual QueryBuilder *setp(const std::string &col, const bool param);

	virtual QueryBuilder *wp(const std::string &col, const std::string &param);
	virtual QueryBuilder *wp(const std::string &col, const char *param);
	virtual QueryBuilder *wp(const std::string &col, const int param);
	virtual QueryBuilder *wp(const std::string &col, const bool param);

	virtual QueryBuilder *eselect(const std::string &params);
	virtual QueryBuilder *eudpate(const std::string &params);
	virtual QueryBuilder *edel(const std::string &params);

	virtual QueryBuilder *ewhere(const std::string &params);
	virtual QueryBuilder *efrom(const std::string &params);
	virtual QueryBuilder *einsert(const std::string &table_name);
	virtual QueryBuilder *evalues(const std::string &params_str);
	virtual QueryBuilder *eval(const std::string &param);
	//note col is NOT escaped
	virtual QueryBuilder *esetp(const std::string &col, const std::string &escape_param);
	//note col is NOT escaped
	virtual QueryBuilder *ewp(const std::string &col, const std::string &escape_param);

	virtual QueryBuilder *limit(const int num);
	virtual QueryBuilder *offset(const int num);

	virtual QueryBuilder *w(const std::string &str);
	virtual QueryBuilder *ew(const std::string &str);

	virtual QueryBuilder *select_last_insert_id();

	virtual std::string escape(const std::string &params);

	virtual QueryBuilder *prepare();
	virtual QueryBuilder *set_param(const int index, const std::string &value);
	virtual QueryBuilder *set_param(const int index, const int value);
	virtual QueryBuilder *set_param(const int index, const float value);

	virtual void end_command();

	virtual QueryBuilder *reset();

	virtual QueryResult *run();
	virtual void run_query();

	std::string get_result();

	void print();

	QueryBuilder();
	virtual ~QueryBuilder();

	std::string query_result;
};

#endif