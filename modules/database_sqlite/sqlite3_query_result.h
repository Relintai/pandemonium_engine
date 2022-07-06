#ifndef MYSQL_QUERY_RESULT_H
#define MYSQL_QUERY_RESULT_H

#include "core/string.h"
#include <vector>

#include "database/query_result.h"

#include "./sqlite/sqlite3.h"

class Sqlite3QueryResult : public QueryResult {
	RCPP_OBJECT(Sqlite3QueryResult, QueryResult);

public:
	bool next_row();
	const char* get_cell(const int index);

	bool is_cell_null(const int index);

	int get_last_insert_rowid();

	void query(const String &query, sqlite3 *conn);

	static int run_query_finished(void *data, int argc, char **argv, char **col_names);

	Sqlite3QueryResult();
	~Sqlite3QueryResult();

	char* err_msg;

public:
	struct Cell {
		bool null;
		String data;

		Cell() {
			null = false;
		}
	};

	struct Sqlite3QueryResultRow {
		std::vector<Cell> cells;
	};

	char **col_names;
	std::vector<Sqlite3QueryResultRow *> rows;
	int current_row;

	sqlite3 *_connection;
};

#endif