#include "query_result.h"

#include <sstream>

bool QueryResult::next_row() {
	return false;
}

String QueryResult::get_cell(const int index) {
	return String();
}

bool QueryResult::get_cell_bool(const int index) {
	return get_cell_int(index);
}

int QueryResult::get_cell_int(const int index) {
	if (is_cell_null(index)) {
		return 0;
	}

	return get_cell(index).to_int();
}

float QueryResult::get_cell_float(const int index) {
	if (is_cell_null(index)) {
		return 0;
	}

	return get_cell(index).to_float();
}
double QueryResult::get_cell_double(const int index) {
	if (is_cell_null(index)) {
		return 0;
	}

	return get_cell(index).to_double();
}

bool QueryResult::is_cell_null(const int index) {
	return true;
}

int QueryResult::get_last_insert_rowid() {
	return 0;
}

String QueryResult::get_error_message() {
	return "";
}

QueryResult::QueryResult() {
}

QueryResult::~QueryResult() {
}

void QueryResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("next_row"), &QueryResult::next_row);

	ClassDB::bind_method(D_METHOD("get_cell", "index"), &QueryResult::get_cell);
	ClassDB::bind_method(D_METHOD("get_cell_bool", "index"), &QueryResult::get_cell_bool);
	ClassDB::bind_method(D_METHOD("get_cell_int", "index"), &QueryResult::get_cell_int);
	ClassDB::bind_method(D_METHOD("get_cell_float", "index"), &QueryResult::get_cell_float);
	ClassDB::bind_method(D_METHOD("get_cell_double", "index"), &QueryResult::get_cell_double);

	ClassDB::bind_method(D_METHOD("is_cell_null", "index"), &QueryResult::is_cell_null);

	ClassDB::bind_method(D_METHOD("get_last_insert_rowid"), &QueryResult::get_last_insert_rowid);

	ClassDB::bind_method(D_METHOD("get_error_message"), &QueryResult::get_error_message);
}
