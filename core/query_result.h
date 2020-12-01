#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

class QueryResult {
public:
    virtual bool next_row();
    virtual const char*get_cell(const int index);

	QueryResult();
	virtual ~QueryResult();
};

#endif