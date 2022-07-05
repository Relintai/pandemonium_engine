#include "query_result.h"

#include <sstream>

bool QueryResult::next_row() {
	return false;
}

const char *QueryResult::get_cell(const int index) {
	return "";
}

const String QueryResult::get_cell_str(const int index) {
	return String(get_cell(index));
}

const bool QueryResult::get_cell_bool(const int index) {
	return get_cell_int(index);
}

const int QueryResult::get_cell_int(const int index) {
	if (is_cell_null(index)) {
		return 0;
	}

	//todo better way
	std::stringstream ss;
	ss.str(get_cell(index));

	int r;
	ss >> r;

	return r;
}

const float QueryResult::get_cell_float(const int index) {
	if (is_cell_null(index)) {
		return 0;
	}

	//todo better way
	std::stringstream ss;
	ss.str(get_cell(index));

	float r;
	ss >> r;

	return r;
}
const double QueryResult::get_cell_double(const int index) {
	if (is_cell_null(index)) {
		return 0;
	}

	//todo better way
	std::stringstream ss;
	ss.str(get_cell(index));

	double r;
	ss >> r;

	return r;
}

bool QueryResult::is_cell_null(const int index) {
	return true;
}

int QueryResult::get_last_insert_rowid() {
	return 0;
}

QueryResult::QueryResult() {
}

QueryResult::~QueryResult() {
}
