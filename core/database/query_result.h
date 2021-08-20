#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include <string>

class QueryResult {
public:
	virtual bool next_row();
	virtual const char *get_cell(const int index);
	virtual const std::string get_cell_str(const int index);
	virtual const bool get_cell_bool(const int index);
	virtual const int get_cell_int(const int index);
    
	virtual bool is_cell_null(const int index);

	virtual int get_last_insert_rowid();

	QueryResult();
	virtual ~QueryResult();
};

#endif