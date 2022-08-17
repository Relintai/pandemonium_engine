#ifndef SQLITE3_QUERY_RESULT_H
#define SQLITE3_QUERY_RESULT_H

#include "core/ustring.h"
#include "core/containers/vector.h"

#include "../database/query_result.h"

struct sqlite3;

class Sqlite3QueryResult : public QueryResult {
	GDCLASS(Sqlite3QueryResult, QueryResult);

public:
	bool next_row();
	String get_cell(const int index);
	bool is_cell_null(const int index);
	int get_last_insert_rowid();
	String get_error_message();

	void query(const String &query, sqlite3 *conn);

	static int run_query_finished(void *data, int argc, char **argv, char **col_names);

	Sqlite3QueryResult();
	~Sqlite3QueryResult();

protected:
	struct Cell {
		bool null;
		String data;

		Cell() {
			null = false;
		}
	};

	struct Sqlite3QueryResultRow {
		Vector<Cell> cells;
	};

	char **col_names;
	Vector<Sqlite3QueryResultRow *> rows;
	int current_row;
	char *err_msg;

	sqlite3 *_connection;
};

#endif
