#include "mysql_query_result.h"

#include <cstdio>

bool MysqlQueryResult::next_row() {
    current_row = mysql_fetch_row(result);

    //null if no result
    return current_row;
}

const char* MysqlQueryResult::get_cell(const int index) {
    if (!current_row)
        return "";

    return current_row[index];
}

MysqlQueryResult::MysqlQueryResult() : QueryResult() {
    result = nullptr;
}

MysqlQueryResult::~MysqlQueryResult() {
    if(result) {
	    mysql_free_result(result);
    }
}