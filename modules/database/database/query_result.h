#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include "core/string.h"

#include "core/reference.h"

class QueryResult : public Reference {
	RCPP_OBJECT(QueryResult, Reference);

public:
	virtual bool next_row();
	virtual const char *get_cell(const int index);
	virtual const String get_cell_str(const int index);
	virtual const bool get_cell_bool(const int index);
	virtual const int get_cell_int(const int index);
	virtual const float get_cell_float(const int index);
	virtual const double get_cell_double(const int index);
    
	virtual bool is_cell_null(const int index);

	virtual int get_last_insert_rowid();

	QueryResult();
	virtual ~QueryResult();
};

#endif