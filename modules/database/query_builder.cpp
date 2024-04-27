/*************************************************************************/
/*  query_builder.cpp                                                    */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "query_builder.h"

#include "query_result.h"
#include "prepared_statement.h"

String QueryBuilder::get_result() {
	return query_result;
}
void QueryBuilder::set_result(const String &val) {
	query_result = val;
}

QueryBuilder *QueryBuilder::select() {
	return this;
}
QueryBuilder *QueryBuilder::update() {
	return this;
}
QueryBuilder *QueryBuilder::del() {
	return this;
}

QueryBuilder *QueryBuilder::cvalues() {
	return this;
}
QueryBuilder *QueryBuilder::next_value() {
	return this;
}

QueryBuilder *QueryBuilder::begin_transaction() {
	return this;
}
QueryBuilder *QueryBuilder::commit() {
	return this;
}

QueryBuilder *QueryBuilder::nl() {
	query_result += "\n";

	return this;
}

QueryBuilder *QueryBuilder::str() {
	return this;
}
QueryBuilder *QueryBuilder::cstr() {
	return this;
}

QueryBuilder *QueryBuilder::select(const String &params) {
	return nselect(escape(params));
}
QueryBuilder *QueryBuilder::update(const String &params) {
	return nupdate(escape(params));
}
QueryBuilder *QueryBuilder::del(const String &params) {
	return ndel(escape(params));
}
QueryBuilder *QueryBuilder::where(const String &params) {
	return nwhere(escape(params));
}

QueryBuilder *QueryBuilder::from(const String &params) {
	return nfrom(escape(params));
}

QueryBuilder *QueryBuilder::insert(const String &table_name, const String &columns) {
	return this;
}
QueryBuilder *QueryBuilder::values(const String &params_str) {
	return nvalues(escape(params_str));
}

QueryBuilder *QueryBuilder::val() {
	return this;
}

QueryBuilder *QueryBuilder::vals(const String &param) {
	return nval(escape(param));
}

QueryBuilder *QueryBuilder::vals(const char *param) {
	return this;
}

QueryBuilder *QueryBuilder::vali(const int param) {
	return this;
}
QueryBuilder *QueryBuilder::valb(const bool param) {
	return this;
}

QueryBuilder *QueryBuilder::valf(const float param) {
	return this;
}
QueryBuilder *QueryBuilder::vald(const double param) {
	return this;
}

QueryBuilder *QueryBuilder::like(const String &str) {
	return nlike(escape(str));
}

QueryBuilder *QueryBuilder::sets() {
	return this;
}
QueryBuilder *QueryBuilder::cset() {
	return this;
}
QueryBuilder *QueryBuilder::setps(const String &col, const String &param) {
	return nsetp(col, escape(param));
}
QueryBuilder *QueryBuilder::setps(const String &col, const char *param) {
	return this;
}
QueryBuilder *QueryBuilder::setpi(const String &col, const int param) {
	return this;
}
QueryBuilder *QueryBuilder::setpb(const String &col, const bool param) {
	return this;
}
QueryBuilder *QueryBuilder::setpf(const String &col, const float param) {
	return this;
}
QueryBuilder *QueryBuilder::setpd(const String &col, const double param) {
	return this;
}

QueryBuilder *QueryBuilder::wps(const String &col, const String &param) {
	return nwp(col, escape(param));
}
QueryBuilder *QueryBuilder::wps(const String &col, const char *param) {
	return this;
}
QueryBuilder *QueryBuilder::wpi(const String &col, const int param) {
	return this;
}
QueryBuilder *QueryBuilder::wpb(const String &col, const bool param) {
	return this;
}

QueryBuilder *QueryBuilder::nselect(const String &params) {
	return this;
}
QueryBuilder *QueryBuilder::nupdate(const String &params) {
	return this;
}
QueryBuilder *QueryBuilder::ndel(const String &params) {
	return this;
}

QueryBuilder *QueryBuilder::nwhere(const String &params) {
	return this;
}
QueryBuilder *QueryBuilder::nfrom(const String &params) {
	return this;
}
QueryBuilder *QueryBuilder::nlike(const String &str) {
	return this;
}
QueryBuilder *QueryBuilder::nvalues(const String &params_str) {
	return this;
}

QueryBuilder *QueryBuilder::nval(const String &param) {
	return this;
}
QueryBuilder *QueryBuilder::nsetp(const String &col, const String &escape_param) {
	return this;
}

QueryBuilder *QueryBuilder::nwp(const String &col, const String &escape_param) {
	return this;
}

QueryBuilder *QueryBuilder::limit(const int num) {
	return this;
}

QueryBuilder *QueryBuilder::offset(const int num) {
	return this;
}

QueryBuilder *QueryBuilder::order_by_asc(const String &col) {
	query_result += "ORDER BY " + col + " ASC, ";

	return this;
}
QueryBuilder *QueryBuilder::order_by_desc(const String &col) {
	query_result += "ORDER BY " + col + " DESC, ";

	return this;
}
QueryBuilder *QueryBuilder::order_by(const String &col) {
	if (col.empty()) {
		query_result += "ORDER BY ";
	} else {
		query_result += "ORDER BY " + col + ", ";
	}

	return this;
}

QueryBuilder *QueryBuilder::corder_by() {
	ERR_FAIL_COND_V(query_result.length() <= 2, this);

	query_result[query_result.length() - 2] = ' ';

	return this;
}
QueryBuilder *QueryBuilder::order_by_add_col(const String &col) {
	query_result += col + ", ";

	return this;
}
QueryBuilder *QueryBuilder::asc(const String &col) {
	if (col.empty()) {
		query_result += "ASC, ";
	} else {
		query_result += col + " ASC, ";
	}

	return this;
}
QueryBuilder *QueryBuilder::desc(const String &col) {
	if (col.empty()) {
		query_result += "DESC, ";
	} else {
		query_result += col + " DESC, ";
	}

	return this;
}

QueryBuilder *QueryBuilder::land() {
	return this;
}
QueryBuilder *QueryBuilder::lor() {
	return this;
}

QueryBuilder *QueryBuilder::wildcard() {
	return this;
}

QueryBuilder *QueryBuilder::w(const String &str) {
	query_result += str;

	return this;
}
QueryBuilder *QueryBuilder::ew(const String &str) {
	return w(escape(str));
}

QueryBuilder *QueryBuilder::select_last_insert_id() {
	return this;
}

String QueryBuilder::escape(const String &params) {
	return params;
}

QueryBuilder *QueryBuilder::end_command() {
	return this;
}

QueryBuilder *QueryBuilder::reset() {
	query_result = "";

	return this;
}

Ref<QueryResult> QueryBuilder::run() {
	return Ref<QueryResult>();
}

void QueryBuilder::run_query() {
}

Ref<PreparedStatement> QueryBuilder::create_prepared_statement() {
	return Ref<PreparedStatement>();
}

void QueryBuilder::print() {
	//printf("%s\n", query_result.get_data());
	ERR_PRINT(query_result);
}

QueryBuilder::QueryBuilder() {
}

QueryBuilder::~QueryBuilder() {
}

void QueryBuilder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_result"), &QueryBuilder::get_result);
	ClassDB::bind_method(D_METHOD("set_result", "value"), &QueryBuilder::set_result);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "result"), "set_result", "get_result");

	ClassDB::bind_method(D_METHOD("cvalues"), &QueryBuilder::_cvalues_bind);
	ClassDB::bind_method(D_METHOD("next_value"), &QueryBuilder::_next_value_bind);

	ClassDB::bind_method(D_METHOD("begin_transaction"), &QueryBuilder::_begin_transaction_bind);
	ClassDB::bind_method(D_METHOD("commit"), &QueryBuilder::_commit_bind);

	ClassDB::bind_method(D_METHOD("nl"), &QueryBuilder::_nl_bind);

	ClassDB::bind_method(D_METHOD("str"), &QueryBuilder::_str_bind);
	ClassDB::bind_method(D_METHOD("cstr"), &QueryBuilder::_cstr_bind);

	ClassDB::bind_method(D_METHOD("select", "params"), &QueryBuilder::_select_bind, "");
	ClassDB::bind_method(D_METHOD("update", "params"), &QueryBuilder::_update_bind, "");
	ClassDB::bind_method(D_METHOD("del", "params"), &QueryBuilder::_del_bind, "");

	ClassDB::bind_method(D_METHOD("where", "params"), &QueryBuilder::_where_bind, "");
	ClassDB::bind_method(D_METHOD("from", "params"), &QueryBuilder::_from_bind, "");
	ClassDB::bind_method(D_METHOD("insert", "table_name", "columns"), &QueryBuilder::_insert_bind, "", "");
	ClassDB::bind_method(D_METHOD("values", "params_str"), &QueryBuilder::_values_bind, "");
	ClassDB::bind_method(D_METHOD("val"), &QueryBuilder::_val_bind);
	ClassDB::bind_method(D_METHOD("vals", "param"), &QueryBuilder::_vals_bind);
	ClassDB::bind_method(D_METHOD("vali", "param"), &QueryBuilder::_vali_bind);
	ClassDB::bind_method(D_METHOD("valb", "param"), &QueryBuilder::_valb_bind);
	ClassDB::bind_method(D_METHOD("valf", "param"), &QueryBuilder::_valf_bind);
	ClassDB::bind_method(D_METHOD("vald", "param"), &QueryBuilder::_vald_bind);

	ClassDB::bind_method(D_METHOD("like", "str"), &QueryBuilder::_like_bind, "");
	ClassDB::bind_method(D_METHOD("sets"), &QueryBuilder::_sets_bind);
	ClassDB::bind_method(D_METHOD("cset"), &QueryBuilder::_cset_bind);

	ClassDB::bind_method(D_METHOD("setps", "col", "param"), &QueryBuilder::_setps_bind);
	ClassDB::bind_method(D_METHOD("setpi", "col", "param"), &QueryBuilder::_setpi_bind);
	ClassDB::bind_method(D_METHOD("setpb", "col", "param"), &QueryBuilder::_setpb_bind);
	ClassDB::bind_method(D_METHOD("setpf", "col", "param"), &QueryBuilder::_setpf_bind);
	ClassDB::bind_method(D_METHOD("setpd", "col", "param"), &QueryBuilder::_setpd_bind);

	ClassDB::bind_method(D_METHOD("wps", "col", "param"), &QueryBuilder::_wps_bind);
	ClassDB::bind_method(D_METHOD("wpi", "col", "param"), &QueryBuilder::_wpi_bind);
	ClassDB::bind_method(D_METHOD("wpb", "col", "param"), &QueryBuilder::_wpb_bind);

	ClassDB::bind_method(D_METHOD("nselect", "params"), &QueryBuilder::_nselect_bind);
	ClassDB::bind_method(D_METHOD("nupdate", "params"), &QueryBuilder::_nupdate_bind);
	ClassDB::bind_method(D_METHOD("ndel", "params"), &QueryBuilder::_ndel_bind);

	ClassDB::bind_method(D_METHOD("nwhere", "params"), &QueryBuilder::_nwhere_bind);
	ClassDB::bind_method(D_METHOD("nfrom", "params"), &QueryBuilder::_nfrom_bind);
	ClassDB::bind_method(D_METHOD("nlike", "str"), &QueryBuilder::_nlike_bind);
	ClassDB::bind_method(D_METHOD("nvalues", "params_str"), &QueryBuilder::_nvalues_bind);
	ClassDB::bind_method(D_METHOD("nval", "param"), &QueryBuilder::_nval_bind);
	ClassDB::bind_method(D_METHOD("nsetp", "col", "escape_param"), &QueryBuilder::_nsetp_bind);
	ClassDB::bind_method(D_METHOD("nwp", "col", "escape_param"), &QueryBuilder::_nwp_bind);

	ClassDB::bind_method(D_METHOD("limit", "num"), &QueryBuilder::_limit_bind);
	ClassDB::bind_method(D_METHOD("offset", "num"), &QueryBuilder::_offset_bind);

	ClassDB::bind_method(D_METHOD("order_by_asc", "col"), &QueryBuilder::_order_by_asc_bind);
	ClassDB::bind_method(D_METHOD("order_by_desc", "col"), &QueryBuilder::_order_by_desc_bind);
	ClassDB::bind_method(D_METHOD("order_by", "col"), &QueryBuilder::_order_by_bind);
	ClassDB::bind_method(D_METHOD("corder_by"), &QueryBuilder::_corder_by_bind);
	ClassDB::bind_method(D_METHOD("order_by_add_col", "col"), &QueryBuilder::_order_by_add_col_bind);
	ClassDB::bind_method(D_METHOD("asc", "col"), &QueryBuilder::_asc_bind, "");
	ClassDB::bind_method(D_METHOD("desc", "col"), &QueryBuilder::_desc_bind, "");

	ClassDB::bind_method(D_METHOD("land"), &QueryBuilder::_land_bind);
	ClassDB::bind_method(D_METHOD("lor"), &QueryBuilder::_lor_bind);

	ClassDB::bind_method(D_METHOD("wildcard"), &QueryBuilder::_wildcard_bind);

	ClassDB::bind_method(D_METHOD("w", "str"), &QueryBuilder::_w_bind);
	ClassDB::bind_method(D_METHOD("ew", "str"), &QueryBuilder::_ew_bind);

	ClassDB::bind_method(D_METHOD("select_last_insert_id"), &QueryBuilder::_select_last_insert_id_bind);

	ClassDB::bind_method(D_METHOD("escape", "param"), &QueryBuilder::escape);

	ClassDB::bind_method(D_METHOD("end_command"), &QueryBuilder::_end_command_bind);

	ClassDB::bind_method(D_METHOD("reset"), &QueryBuilder::_reset_bind);

	ClassDB::bind_method(D_METHOD("run"), &QueryBuilder::run);
	ClassDB::bind_method(D_METHOD("run_query"), &QueryBuilder::run_query);

	ClassDB::bind_method(D_METHOD("create_prepared_statement"), &QueryBuilder::create_prepared_statement);
}

Ref<QueryBuilder> QueryBuilder::_cvalues_bind() {
	return Ref<QueryBuilder>(cvalues());
}
Ref<QueryBuilder> QueryBuilder::_next_value_bind() {
	return Ref<QueryBuilder>(next_value());
}

Ref<QueryBuilder> QueryBuilder::_begin_transaction_bind() {
	return Ref<QueryBuilder>(begin_transaction());
}
Ref<QueryBuilder> QueryBuilder::_commit_bind() {
	return Ref<QueryBuilder>(commit());
}

Ref<QueryBuilder> QueryBuilder::_nl_bind() {
	return Ref<QueryBuilder>(nl());
}

Ref<QueryBuilder> QueryBuilder::_str_bind() {
	return Ref<QueryBuilder>(str());
}
Ref<QueryBuilder> QueryBuilder::_cstr_bind() {
	return Ref<QueryBuilder>(cstr());
}

Ref<QueryBuilder> QueryBuilder::_select_bind(const String &params) {
	return Ref<QueryBuilder>(select(params));
}
Ref<QueryBuilder> QueryBuilder::_update_bind(const String &params) {
	return Ref<QueryBuilder>(update(params));
}
Ref<QueryBuilder> QueryBuilder::_del_bind(const String &params) {
	return Ref<QueryBuilder>(del(params));
}

Ref<QueryBuilder> QueryBuilder::_where_bind(const String &params) {
	return Ref<QueryBuilder>(where(params));
}
Ref<QueryBuilder> QueryBuilder::_from_bind(const String &params) {
	return Ref<QueryBuilder>(from(params));
}
Ref<QueryBuilder> QueryBuilder::_insert_bind(const String &table_name, const String &columns) {
	return Ref<QueryBuilder>(insert(table_name, columns));
}
Ref<QueryBuilder> QueryBuilder::_values_bind(const String &params_str) {
	return Ref<QueryBuilder>(values(params_str));
}
Ref<QueryBuilder> QueryBuilder::_val_bind() {
	return Ref<QueryBuilder>(val());
}
Ref<QueryBuilder> QueryBuilder::_vals_bind(const String &param) {
	return Ref<QueryBuilder>(vals(param));
}
Ref<QueryBuilder> QueryBuilder::_vali_bind(const int param) {
	return Ref<QueryBuilder>(vali(param));
}
Ref<QueryBuilder> QueryBuilder::_valb_bind(const bool param) {
	return Ref<QueryBuilder>(valb(param));
}
Ref<QueryBuilder> QueryBuilder::_valf_bind(const float param) {
	return Ref<QueryBuilder>(valf(param));
}
Ref<QueryBuilder> QueryBuilder::_vald_bind(const double param) {
	return Ref<QueryBuilder>(vald(param));
}

Ref<QueryBuilder> QueryBuilder::_like_bind(const String &str) {
	return Ref<QueryBuilder>(like(str));
}

Ref<QueryBuilder> QueryBuilder::_sets_bind() {
	return Ref<QueryBuilder>(sets());
}
Ref<QueryBuilder> QueryBuilder::_cset_bind() {
	return Ref<QueryBuilder>(cset());
}

Ref<QueryBuilder> QueryBuilder::_setps_bind(const String &col, const String &param) {
	return Ref<QueryBuilder>(setps(col, param));
}
Ref<QueryBuilder> QueryBuilder::_setpi_bind(const String &col, const int param) {
	return Ref<QueryBuilder>(setpi(col, param));
}
Ref<QueryBuilder> QueryBuilder::_setpb_bind(const String &col, const bool param) {
	return Ref<QueryBuilder>(setpb(col, param));
}
Ref<QueryBuilder> QueryBuilder::_setpf_bind(const String &col, const float param) {
	return Ref<QueryBuilder>(setpf(col, param));
}
Ref<QueryBuilder> QueryBuilder::_setpd_bind(const String &col, const double param) {
	return Ref<QueryBuilder>(setpd(col, param));
}

Ref<QueryBuilder> QueryBuilder::_wps_bind(const String &col, const String &param) {
	return Ref<QueryBuilder>(wps(col, param));
}
Ref<QueryBuilder> QueryBuilder::_wpi_bind(const String &col, const int param) {
	return Ref<QueryBuilder>(wpi(col, param));
}
Ref<QueryBuilder> QueryBuilder::_wpb_bind(const String &col, const bool param) {
	return Ref<QueryBuilder>(wpb(col, param));
}

Ref<QueryBuilder> QueryBuilder::_nselect_bind(const String &params) {
	return Ref<QueryBuilder>(nselect(params));
}
Ref<QueryBuilder> QueryBuilder::_nupdate_bind(const String &params) {
	return Ref<QueryBuilder>(nupdate(params));
}
Ref<QueryBuilder> QueryBuilder::_ndel_bind(const String &params) {
	return Ref<QueryBuilder>(ndel(params));
}

Ref<QueryBuilder> QueryBuilder::_nwhere_bind(const String &params) {
	return Ref<QueryBuilder>(nwhere(params));
}
Ref<QueryBuilder> QueryBuilder::_nfrom_bind(const String &params) {
	return Ref<QueryBuilder>(nfrom(params));
}
Ref<QueryBuilder> QueryBuilder::_nlike_bind(const String &str) {
	return Ref<QueryBuilder>(nlike(str));
}
Ref<QueryBuilder> QueryBuilder::_nvalues_bind(const String &params_str) {
	return Ref<QueryBuilder>(nvalues(params_str));
}
Ref<QueryBuilder> QueryBuilder::_nval_bind(const String &param) {
	return Ref<QueryBuilder>(nval(param));
}
//note col is NOT escaped
Ref<QueryBuilder> QueryBuilder::_nsetp_bind(const String &col, const String &escape_param) {
	return Ref<QueryBuilder>(nsetp(col, escape_param));
}
//note col is NOT escaped
Ref<QueryBuilder> QueryBuilder::_nwp_bind(const String &col, const String &escape_param) {
	return Ref<QueryBuilder>(nwp(col, escape_param));
}

Ref<QueryBuilder> QueryBuilder::_limit_bind(const int num) {
	return Ref<QueryBuilder>(limit(num));
}
Ref<QueryBuilder> QueryBuilder::_offset_bind(const int num) {
	return Ref<QueryBuilder>(offset(num));
}

Ref<QueryBuilder> QueryBuilder::_order_by_asc_bind(const String &col) {
	return Ref<QueryBuilder>(order_by_asc(col));
}
Ref<QueryBuilder> QueryBuilder::_order_by_desc_bind(const String &col) {
	return Ref<QueryBuilder>(order_by_desc(col));
}
Ref<QueryBuilder> QueryBuilder::_order_by_bind(const String &col) {
	return Ref<QueryBuilder>(order_by(col));
}

Ref<QueryBuilder> QueryBuilder::_corder_by_bind() {
	return Ref<QueryBuilder>(corder_by());
}
Ref<QueryBuilder> QueryBuilder::_order_by_add_col_bind(const String &col) {
	return Ref<QueryBuilder>(order_by_add_col(col));
}
Ref<QueryBuilder> QueryBuilder::_asc_bind(const String &col) {
	return Ref<QueryBuilder>(asc(col));
}
Ref<QueryBuilder> QueryBuilder::_desc_bind(const String &col) {
	return Ref<QueryBuilder>(desc(col));
}

//l=logical (and, or are operators)
Ref<QueryBuilder> QueryBuilder::_land_bind() {
	return Ref<QueryBuilder>(land());
}
Ref<QueryBuilder> QueryBuilder::_lor_bind() {
	return Ref<QueryBuilder>(lor());
}

Ref<QueryBuilder> QueryBuilder::_wildcard_bind() {
	return Ref<QueryBuilder>(wildcard());
}

Ref<QueryBuilder> QueryBuilder::_w_bind(const String &str) {
	return Ref<QueryBuilder>(w(str));
}
Ref<QueryBuilder> QueryBuilder::_ew_bind(const String &str) {
	return Ref<QueryBuilder>(ew(str));
}

Ref<QueryBuilder> QueryBuilder::_select_last_insert_id_bind() {
	return Ref<QueryBuilder>(select_last_insert_id());
}

Ref<QueryBuilder> QueryBuilder::_end_command_bind() {
	return Ref<QueryBuilder>(end_command());
}

Ref<QueryBuilder> QueryBuilder::_reset_bind() {
	return Ref<QueryBuilder>(reset());
}
