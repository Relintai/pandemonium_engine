#include "table_builder.h"

#include "query_result.h"

TableBuilder *TableBuilder::create_table(const String &name) {
	return this;
}

TableBuilder *TableBuilder::integer(const String &name) {
	return this;
}

TableBuilder *TableBuilder::integer(const String &name, const int length) {
	return this;
}

TableBuilder *TableBuilder::tiny_integer(const String &name) {
	return this;
}
TableBuilder *TableBuilder::tiny_integer(const String &name, const int length) {
	return this;
}

TableBuilder *TableBuilder::small_integer(const String &name) {
	return this;
}
TableBuilder *TableBuilder::small_integer(const String &name, const int length) {
	return this;
}

TableBuilder *TableBuilder::real_float(const String &name) {
	return this;
}
TableBuilder *TableBuilder::real_float(const String &name, const int size, const int d) {
	return this;
}

TableBuilder *TableBuilder::real_double(const String &name) {
	return this;
}
TableBuilder *TableBuilder::real_double(const String &name, const int size, const int d) {
	return this;
}

TableBuilder *TableBuilder::date(const String &name) {
	return this;
}

TableBuilder *TableBuilder::varchar(const String &name, const int length) {
	return this;
}

TableBuilder *TableBuilder::text(const String &name) {
	return this;
}

TableBuilder *TableBuilder::not_null() {
	return this;
}

TableBuilder *TableBuilder::null() {
	return this;
}

TableBuilder *TableBuilder::defval(const String &val) {
	return this;
}

TableBuilder *TableBuilder::auto_increment() {
	return this;
}

TableBuilder *TableBuilder::primary_key(const String &name) {
	return this;
}

TableBuilder *TableBuilder::primary_key() {
	return this;
}

TableBuilder *TableBuilder::next_row() {
	return this;
}

TableBuilder *TableBuilder::ccreate_table() {
	return this;
}

TableBuilder *TableBuilder::drop_table() {
	return this;
}
TableBuilder *TableBuilder::drop_table_if_exists() {
	return this;
}
TableBuilder *TableBuilder::drop_table(const String &name) {
	return this;
}
TableBuilder *TableBuilder::drop_table_if_exists(const String &name) {
	return this;
}
TableBuilder *TableBuilder::cdrop_table() {
	return this;
}

TableBuilder *TableBuilder::foreign_key(const String &name) {
	return this;
}
TableBuilder *TableBuilder::references(const String &table, const String &name) {
	return this;
}

Ref<QueryResult> TableBuilder::run() {
	return Ref<QueryResult>();
}

void TableBuilder::run_query() {
}

void TableBuilder::print() {
	printf("%s\n", result.c_str());
}

TableBuilder::TableBuilder() {
}

TableBuilder::~TableBuilder() {
}