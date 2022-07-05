#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include "core/ustring.h"

#include "core/reference.h"

class QueryResult : public Reference {
	GDCLASS(QueryResult, Reference);

public:
	virtual bool next_row();
	virtual const char *get_cell_chr(const int index);
	virtual String get_cell(const int index);
	virtual bool get_cell_bool(const int index);
	virtual int get_cell_int(const int index);
	virtual float get_cell_float(const int index);
	virtual double get_cell_double(const int index);

	virtual bool is_cell_null(const int index);

	virtual int get_last_insert_rowid();

	QueryResult();
	virtual ~QueryResult();

protected:
	static void _bind_methods();
};

#endif
