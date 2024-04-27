#ifndef QUERY_BUILDER_H
#define QUERY_BUILDER_H

/*************************************************************************/
/*  query_builder.h                                                      */
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

#include "core/string/string_builder.h"
#include "core/string/ustring.h"

#include "core/object/reference.h"

class QueryResult;
class PreparedStatement;

//methods that start with an e escape their params.

class QueryBuilder : public Reference {
	GDCLASS(QueryBuilder, Reference);

public:
	String get_result();
	void set_result(const String &val);

	virtual QueryBuilder *select();
	virtual QueryBuilder *update();
	virtual QueryBuilder *del();

	virtual QueryBuilder *cvalues();
	virtual QueryBuilder *next_value();

	virtual QueryBuilder *begin_transaction();
	virtual QueryBuilder *commit();

	virtual QueryBuilder *nl();

	virtual QueryBuilder *str();
	virtual QueryBuilder *cstr();

	virtual QueryBuilder *select(const String &params);
	virtual QueryBuilder *update(const String &params);
	virtual QueryBuilder *del(const String &params);

	virtual QueryBuilder *where(const String &params = "");
	virtual QueryBuilder *from(const String &params = "");
	virtual QueryBuilder *insert(const String &table_name = "", const String &columns = "");
	virtual QueryBuilder *values(const String &params_str = "");
	virtual QueryBuilder *val();
	virtual QueryBuilder *vals(const String &param);
	virtual QueryBuilder *vals(const char *param);
	virtual QueryBuilder *vali(const int param);
	virtual QueryBuilder *valb(const bool param);
	virtual QueryBuilder *valf(const float param);
	virtual QueryBuilder *vald(const double param);
	virtual QueryBuilder *valph();
	virtual QueryBuilder *valphi(const String &p_id);
	virtual QueryBuilder *valphr(const String &p_raw_id);

	virtual QueryBuilder *like(const String &str = "");

	//Object Already has set(), so think of it as set_sql
	virtual QueryBuilder *sets();
	virtual QueryBuilder *cset();

	virtual QueryBuilder *setps(const String &col, const String &param);
	virtual QueryBuilder *setps(const String &col, const char *param);
	virtual QueryBuilder *setpi(const String &col, const int param);
	virtual QueryBuilder *setpb(const String &col, const bool param);
	virtual QueryBuilder *setpf(const String &col, const float param);
	virtual QueryBuilder *setpd(const String &col, const double param);
	virtual QueryBuilder *setph(const String &col);
	virtual QueryBuilder *setphi(const String &col, const String &p_id);
	virtual QueryBuilder *setphr(const String &col, const String &p_raw_id);

	// Where param helpers
	virtual QueryBuilder *wps(const String &col, const String &param);
	virtual QueryBuilder *wps(const String &col, const char *param);
	virtual QueryBuilder *wpi(const String &col, const int param);
	virtual QueryBuilder *wpb(const String &col, const bool param);
	virtual QueryBuilder *wph(const String &col);
	virtual QueryBuilder *wphi(const String &col, const String &p_id);
	virtual QueryBuilder *wphr(const String &col, const String &p_raw_id);

	virtual QueryBuilder *nselect(const String &params);
	virtual QueryBuilder *nupdate(const String &params);
	virtual QueryBuilder *ndel(const String &params);

	virtual QueryBuilder *nwhere(const String &params);
	virtual QueryBuilder *nfrom(const String &params);
	virtual QueryBuilder *nlike(const String &str);
	virtual QueryBuilder *nvalues(const String &params_str);
	virtual QueryBuilder *nval(const String &param);
	//note col is NOT escaped
	virtual QueryBuilder *nsetp(const String &col, const String &escape_param);
	//note col is NOT escaped
	virtual QueryBuilder *nwp(const String &col, const String &escape_param);

	virtual QueryBuilder *limit(const int num);
	virtual QueryBuilder *offset(const int num);

	virtual QueryBuilder *order_by_asc(const String &col);
	virtual QueryBuilder *order_by_desc(const String &col);
	virtual QueryBuilder *order_by(const String &col = "");

	virtual QueryBuilder *corder_by();
	virtual QueryBuilder *order_by_add_col(const String &col);
	virtual QueryBuilder *asc(const String &col = "");
	virtual QueryBuilder *desc(const String &col = "");

	//l=logical (and, or are operators)
	virtual QueryBuilder *land();
	virtual QueryBuilder *lor();

	virtual QueryBuilder *wildcard();

	// Prepared statement placeholders
	virtual QueryBuilder *psph();
	virtual QueryBuilder *psphi(const String &p_id);
	virtual QueryBuilder *psphr(const String &p_raw_id);

	virtual QueryBuilder *w(const String &str);
	virtual QueryBuilder *ew(const String &str);

	virtual QueryBuilder *select_last_insert_id();

	virtual String escape(const String &params);

	virtual QueryBuilder *end_command();

	virtual QueryBuilder *reset();

	virtual Ref<QueryResult> run();
	virtual void run_query();

	virtual Ref<PreparedStatement> create_prepared_statement();

	void print();

	QueryBuilder();
	virtual ~QueryBuilder();

protected:
	static void _bind_methods();

	Ref<QueryBuilder> _cvalues_bind();
	Ref<QueryBuilder> _next_value_bind();

	Ref<QueryBuilder> _begin_transaction_bind();
	Ref<QueryBuilder> _commit_bind();

	Ref<QueryBuilder> _nl_bind();

	Ref<QueryBuilder> _str_bind();
	Ref<QueryBuilder> _cstr_bind();

	Ref<QueryBuilder> _select_bind(const String &params);
	Ref<QueryBuilder> _update_bind(const String &params);
	Ref<QueryBuilder> _del_bind(const String &params);

	Ref<QueryBuilder> _where_bind(const String &params = "");
	Ref<QueryBuilder> _from_bind(const String &params = "");
	Ref<QueryBuilder> _insert_bind(const String &table_name = "", const String &columns = "");
	Ref<QueryBuilder> _values_bind(const String &params_str = "");
	Ref<QueryBuilder> _val_bind();
	Ref<QueryBuilder> _vals_bind(const String &param);
	Ref<QueryBuilder> _vali_bind(const int param);
	Ref<QueryBuilder> _valb_bind(const bool param);
	Ref<QueryBuilder> _valf_bind(const float param);
	Ref<QueryBuilder> _vald_bind(const double param);
	Ref<QueryBuilder> _valph_bind();
	Ref<QueryBuilder> _valphi_bind(const String &p_id);
	Ref<QueryBuilder> _valphr_bind(const String &p_raw_id);

	Ref<QueryBuilder> _like_bind(const String &str);

	Ref<QueryBuilder> _sets_bind();
	Ref<QueryBuilder> _cset_bind();

	Ref<QueryBuilder> _setps_bind(const String &col, const String &param);
	Ref<QueryBuilder> _setpi_bind(const String &col, const int param);
	Ref<QueryBuilder> _setpb_bind(const String &col, const bool param);
	Ref<QueryBuilder> _setpf_bind(const String &col, const float param);
	Ref<QueryBuilder> _setpd_bind(const String &col, const double param);
	Ref<QueryBuilder> _setph_bind(const String &col);
	Ref<QueryBuilder> _setphi_bind(const String &col, const String &p_id);
	Ref<QueryBuilder> _setphr_bind(const String &col, const String &p_raw_id);

	Ref<QueryBuilder> _wps_bind(const String &col, const String &param);
	Ref<QueryBuilder> _wpi_bind(const String &col, const int param);
	Ref<QueryBuilder> _wpb_bind(const String &col, const bool param);
	Ref<QueryBuilder> _wph_bind(const String &col);
	Ref<QueryBuilder> _wphi_bind(const String &col, const String &p_id);
	Ref<QueryBuilder> _wphr_bind(const String &col, const String &p_raw_id);

	Ref<QueryBuilder> _nselect_bind(const String &params);
	Ref<QueryBuilder> _nupdate_bind(const String &params);
	Ref<QueryBuilder> _ndel_bind(const String &params);

	Ref<QueryBuilder> _nwhere_bind(const String &params);
	Ref<QueryBuilder> _nfrom_bind(const String &params);
	Ref<QueryBuilder> _nlike_bind(const String &str);
	Ref<QueryBuilder> _nvalues_bind(const String &params_str);
	Ref<QueryBuilder> _nval_bind(const String &param);
	//note col is NOT escaped
	Ref<QueryBuilder> _nsetp_bind(const String &col, const String &escape_param);
	//note col is NOT escaped
	Ref<QueryBuilder> _nwp_bind(const String &col, const String &escape_param);

	Ref<QueryBuilder> _limit_bind(const int num);
	Ref<QueryBuilder> _offset_bind(const int num);

	Ref<QueryBuilder> _order_by_asc_bind(const String &col);
	Ref<QueryBuilder> _order_by_desc_bind(const String &col);
	Ref<QueryBuilder> _order_by_bind(const String &col = "");

	Ref<QueryBuilder> _corder_by_bind();
	Ref<QueryBuilder> _order_by_add_col_bind(const String &col);
	Ref<QueryBuilder> _asc_bind(const String &col = "");
	Ref<QueryBuilder> _desc_bind(const String &col = "");

	//l=logical (and, or are operators)
	Ref<QueryBuilder> _land_bind();
	Ref<QueryBuilder> _lor_bind();

	Ref<QueryBuilder> _wildcard_bind();

	// Prepared statement placeholders
	Ref<QueryBuilder> _psph_bind();
	Ref<QueryBuilder> _psphi_bind(const String &p_id);
	Ref<QueryBuilder> _psphr_bind(const String &p_raw_id);

	Ref<QueryBuilder> _w_bind(const String &str);
	Ref<QueryBuilder> _ew_bind(const String &str);

	Ref<QueryBuilder> _select_last_insert_id_bind();

	Ref<QueryBuilder> _end_command_bind();

	Ref<QueryBuilder> _reset_bind();

	StringBuilder query_result;
};

#endif
