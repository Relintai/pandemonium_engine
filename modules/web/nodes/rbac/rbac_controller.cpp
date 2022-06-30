#include "rbac_controller.h"

#include "core/error_macros.h"

#include "web/html/form_validator.h"
#include "web/html/html_builder.h"
#include "web/http/cookie.h"
#include "web/http/http_session.h"
#include "web/http/request.h"
#include "web/http/session_manager.h"
#include "rbac_default_permissions.h"
#include "web_modules/users/user.h"

#include "database/database.h"
#include "database/database_manager.h"
#include "database/query_builder.h"
#include "database/query_result.h"
#include "database/table_builder.h"

void RBACController::handle_request_main(Request *request) {
}

void RBACController::create_validators() {
}

void RBACController::admin_handle_request_main(Request *request) {
	String seg = request->get_current_path_segment();

	if (seg == "") {
		admin_render_rank_list(request);
		return;
	} else if (seg == "new_rank") {
		request->push_path();

		admin_handle_new_rank(request);
	} else if (seg == "edit_rank") {
		request->push_path();

		admin_handle_edit_rank(request);
	} else if (seg == "permission_editor") {
		request->push_path();

		admin_permission_editor(request);
	}
}

void RBACController::admin_handle_new_rank(Request *request) {

	if (request->get_method() == HTTP_METHOD_POST) {
		Ref<RBACRank> rank;
		rank.instance();

		rank->name = request->get_parameter("name");
		rank->name_internal = request->get_parameter("name_internal");
		rank->settings = request->get_parameter("settings");

		int base_permissions = 0;

		for (int i = 0; i < _registered_permissions.size(); ++i) {
			String param = request->get_parameter("perm_check_" + String::num(i));

			if (param != "") {
				base_permissions |= _registered_permissions[i].value;
			}
		}

		rank->base_permissions = base_permissions;

		int rank_permissions = 0;

		for (int i = 0; i < _registered_rank_permissions.size(); ++i) {
			String param = request->get_parameter("perm_rank_check_" + String::num(i));

			if (param != "") {
				rank_permissions |= _registered_rank_permissions[i].value;
			}
		}

		rank->rank_permissions = rank_permissions;

		db_save_rank(rank);

		_ranks[rank->id] = rank;

		request->send_redirect(request->get_url_root_parent() + "edit_rank/" + String::num(rank->id));
		return;
	}

	RBACAdminRankViewData data;
	render_rank_view(request, &data);
}

void RBACController::admin_handle_edit_rank(Request *request) {
	String seg = request->get_current_path_segment();

	//check whether it's numeric
	//if (!seg.is)

	int id = seg.to_int();

	if (id == 0) {
		RLOG_MSG("RBACController::admin_handle_edit_rank: id == 0!\n");
		request->send_redirect(request->get_url_root_parent());
		return;
	}

	Ref<RBACRank> rank = _ranks[id];

	if (!rank.is_valid()) {
		RLOG_MSG("RBACController::admin_handle_edit_rank: !rank.is_valid()\n");
		request->send_redirect(request->get_url_root_parent());
		return;
	}

	RBACAdminRankViewData data;
	data.rank = rank;

	if (request->get_method() == HTTP_METHOD_POST) {
		rank->name = request->get_parameter("name");
		rank->name_internal = request->get_parameter("name_internal");
		rank->settings = request->get_parameter("settings");

		int base_permissions = 0;

		for (int i = 0; i < _registered_permissions.size(); ++i) {
			String param = request->get_parameter("perm_check_" + String::num(i));

			if (param != "") {
				base_permissions |= _registered_permissions[i].value;
			}
		}

		rank->base_permissions = base_permissions;

		int rank_permissions = 0;

		for (int i = 0; i < _registered_rank_permissions.size(); ++i) {
			String param = request->get_parameter("perm_rank_check_" + String::num(i));

			if (param != "") {
				rank_permissions |= _registered_rank_permissions[i].value;
			}
		}

		rank->rank_permissions = rank_permissions;

		db_save_rank(rank);

		data.messages.push_back("Save Success!");
	}

	render_rank_view(request, &data);
}

void RBACController::render_rank_view(Request *request, RBACAdminRankViewData *data) {
	int id = 0;
	String name = "";
	String name_internal = "";
	String settings = "";
	int base_permissions = 0;
	int rank_permissions = 0;

	if (data->rank.is_valid()) {
		id = data->rank->id;
		name = data->rank->name;
		name_internal = data->rank->name_internal;
		settings = data->rank->settings;
		base_permissions = data->rank->base_permissions;
		rank_permissions = data->rank->rank_permissions;
	}

	HTMLBuilder b;

	b.h4()->f()->a()->href(request->get_url_root_parent())->f()->w("<- Back")->ca()->ch4();
	b.h4()->f()->w("RBAC Editor")->ch4();

	b.div()->cls("messages");
	for (int i = 0; i < data->messages.size(); ++i) {
		b.w(data->messages[i])->br();
	}
	b.cdiv();

	b.form()->method("POST")->action(request->get_url_root() + String::num(id));
	{
		b.csrf_token(request);

		//b.input()->type("hidden")->name("id")->value(String::num(id))->f()->cinput();
		b.w("Name:")->br();
		b.input()->type("text")->name("name")->value(name)->f()->br();
		b.w("Name (Internal):")->br();
		b.input()->type("text")->name("name_internal")->value(name_internal)->f()->cinput()->br();
		b.w("Custom Settings:")->br();
		b.input()->type("text")->name("settings")->value(settings)->f()->cinput()->br();

		b.w("Base Permissions:")->br();

		for (int i = 0; i < _registered_permissions.size(); ++i) {
			String checkbox_name = "perm_check_" + String::num(i);

			b.input()->type("checkbox")->name(checkbox_name)->value(checkbox_name)->id(checkbox_name)->checked((base_permissions & _registered_permissions[i].value) != 0);
			b.label()->fora(checkbox_name)->f()->w(_registered_permissions[i].name)->clabel();
		}

		b.br();

		b.w("Rank Permissions:")->br();

		for (int i = 0; i < _registered_rank_permissions.size(); ++i) {
			String checkbox_name = "perm_rank_check_" + String::num(i);

			b.input()->type("checkbox")->name(checkbox_name)->value(checkbox_name)->id(checkbox_name)->checked((rank_permissions & _registered_rank_permissions[i].value) != 0);
			b.label()->fora(checkbox_name)->f()->w(_registered_rank_permissions[i].name)->clabel();
		}

		b.br();

		b.input()->type("submit")->value("Save");
	}
	b.cform();

	request->body += b.result;
}

void RBACController::admin_permission_editor(Request *request) {

	String seg = request->get_current_path_segment();

	//check whether it's numeric
	//if (!seg.is)

	int id = seg.to_int();

	if (id == 0) {
		RLOG_MSG("RBACController::admin_permission_editor: id == 0!\n");
		request->send_redirect(request->get_url_root_parent());
		return;
	}

	Ref<RBACRank> rank = _ranks[id];

	if (!rank.is_valid()) {
		RLOG_MSG("RBACController::admin_permission_editor: !rank.is_valid()\n");
		request->send_redirect(request->get_url_root_parent());
		return;
	}

	RBACAdminEditPermissionView data;
	data.rank = rank;

	request->push_path();

	String segn = request->get_current_path_segment();

	if (segn == "") {
		admin_render_permission_editor_main_view(request, &data);
		return;
	}

	if (segn == "new") {
		request->push_path();

		if (request->get_method() == HTTP_METHOD_POST) {
			if (admin_process_permission_editor_entry_edit_create_post(request, &data)) {
				return;
			}
		}

		admin_render_permission_editor_entry_edit_create_view(request, &data);
		return;
	}

	if (segn.is_uint()) {
		int perm_index = segn.to_int();

		request->push_path();

		if (perm_index < 0 || perm_index >= rank->permissions.size()) {
			RLOG_ERR("(perm_index < 0 || perm_index >= rank->permissions.size())!\n");
			request->send_redirect(request->get_url_root_parent());
			return;
		}

		data.permission = rank->permissions[perm_index];

		if (!data.permission.is_valid()) {
			RLOG_ERR("(!data.permission.is_valid()\n");
			request->send_error(503);
			return;
		}

		if (request->get_method() == HTTP_METHOD_POST) {
			if (admin_process_permission_editor_entry_edit_create_post(request, &data)) {
				return;
			}
		}

		admin_render_permission_editor_entry_edit_create_view(request, &data);
		return;
	}

	request->send_error(404);
}

void RBACController::admin_render_permission_editor_main_view(Request *request, RBACAdminEditPermissionView *data) {
	HTMLBuilder b;

	Ref<RBACRank> rank = data->rank;

	b.h4()->f()->a()->href(request->get_url_root_parent(2))->f()->w("<- Back")->ca()->ch4();
	b.h4()->f()->w("RBAC Editor")->ch4();

	b.div()->cls("heading");
	{
		b.w("[ Id ]: ")->wn(rank->id)->w(", [ Name ]: ")->w(rank->name)->w(", [ Name Internal ]: ")->w(rank->name_internal);
	}
	b.cdiv();

	b.br();

	for (int i = 0; i < rank->permissions.size(); ++i) {
		Ref<RBACPermission> perm = rank->permissions[i];

		if (!perm.is_valid()) {
			RLOG_ERR("RBACController::admin_render_permission_editor_main_view: !perm.is_valid()\n");
			continue;
		}

		b.div()->cls("row");
		{
			b.a()->href(request->get_url_root() + String::num(i));

			b.w("-- Rank: [ Id ]: ")->wn(perm->id)->w(", [ Rank Id ]: ")->wn(perm->rank_id)->w(", [ Name ]: ")->w(perm->name);
			b.w(" [ URL ]: ")->w(perm->url)->w(", [ Sort Order ]: ")->wn(perm->sort_order);
			b.w(" [ Permissions ]: ");

			int pcount = 0;
			int perms = perm->permissions;
			for (int i = 0; i < _registered_permissions.size(); ++i) {
				if ((_registered_permissions[i].value & perms) != 0) {
					if (pcount > 0) {
						b.w(", ");
					}

					b.w(_registered_permissions[i].name);

					++pcount;
				}
			}

			if (pcount == 0) {
				b.w("- None -");
			}

			b.ca();
		}
		b.cdiv();
	}

	b.br();

	b.a()->href(request->get_url_root("new"));
	b.w("New Permission");
	b.ca();

	request->body += b.result;
}

void RBACController::admin_render_permission_editor_entry_edit_create_view(Request *request, RBACAdminEditPermissionView *data) {
	HTMLBuilder b;

	Ref<RBACRank> rank = data->rank;
	Ref<RBACPermission> perm = data->permission;

	String name;
	String url;
	int sort_order = 0;
	int permissions = 0;

	if (perm.is_valid()) {
		name = perm->name;
		url = perm->url;
		sort_order = perm->sort_order;
		permissions = perm->permissions;
	}

	b.h4()->f()->a()->href(request->get_url_root_parent())->f()->w("<- Back")->ca()->ch4();
	b.h4()->f()->w("RBAC Editor")->ch4();
	b.br();

	b.div()->cls("messages");
	for (int i = 0; i < data->messages.size(); ++i) {
		b.w(data->messages[i])->br();
	}
	b.cdiv();
	b.br();

	b.div()->cls("heading");
	{
		b.w("Rank: [ Id ]: ")->wn(rank->id)->w(", [ Name ]: ")->w(rank->name)->w(", [ Name Internal ]: ")->w(rank->name_internal);
	}
	b.cdiv();
	b.br();

	b.form()->method("POST")->action(request->get_url_root());
	{
		b.csrf_token(request);

		b.w("Name:")->br();
		b.input()->type("text")->name("name")->value(name)->f()->br();
		b.w("URL:")->br();
		b.input()->type("text")->name("url")->value(url)->f()->cinput()->br();

		b.w("Permissions:")->br();

		for (int i = 0; i < _registered_permissions.size(); ++i) {
			String checkbox_name = "perm_check_" + String::num(i);

			b.input()->type("checkbox")->name(checkbox_name)->value(checkbox_name)->id(checkbox_name)->checked((permissions & _registered_permissions[i].value) != 0);
			b.label()->fora(checkbox_name)->f()->w(_registered_permissions[i].name)->clabel();
		}

		b.br();

		b.input()->type("submit")->value("Save");
	}
	b.cform();

	request->body += b.result;
}

bool RBACController::admin_process_permission_editor_entry_edit_create_post(Request *request, RBACAdminEditPermissionView *data) {
	Ref<RBACRank> rank = data->rank;

	Ref<RBACPermission> perm = data->permission;

	if (!perm.is_valid()) {
		perm.instance();

		perm->rank_id = rank->id;

		if (rank->permissions.size() > 0) {
			Ref<RBACPermission> p = rank->permissions[rank->permissions.size() - 1];

			perm->sort_order = p->sort_order + 1;
		}

		rank->permissions.push_back(perm);
	}

	perm->name = request->get_parameter("name");
	perm->url = request->get_parameter("url");

	int permissions = 0;

	for (int i = 0; i < _registered_permissions.size(); ++i) {
		String param = request->get_parameter("perm_check_" + String::num(i));

		if (param != "") {
			permissions |= _registered_permissions[i].value;
		}
	}

	perm->permissions = permissions;

	//set this up in the form by default
	//perm->sort_order = request->get_parameter("sort_order").to_int();

	db_save_permission(perm);

	if (perm->id == 0) {
		RLOG_ERR("RBACController::admin_process_permission_editor_entry_edit_create_post: perm->id == 0!\n");
	}

	request->send_redirect(request->get_url_root_parent());

	return true;
}

void RBACController::admin_render_rank_list(Request *request) {
	HTMLBuilder b;

	b.h4()->f()->a()->href(request->get_url_root_parent())->f()->w("<- Back")->ca()->ch4();
	b.h4()->f()->w("RBAC Editor")->ch4();

	for (std::map<int, Ref<RBACRank> >::iterator p = _ranks.begin(); p != _ranks.end(); p++) {
		Ref<RBACRank> r = p->second;

		if (!r.is_valid()) {
			continue;
		}

		b.div()->cls("row");
		{
			b.a()->href(request->get_url_root("permission_editor/") + String::num(r->id));
			b.w("[ Id ]: ")->wn(r->id)->w(", [ Name ]: ")->w(r->name)->w(", [ Name Internal ]: ")->w(r->name_internal);
			b.w(", [ Base Permissions ]: ");

			int pcount = 0;
			int perms = r->base_permissions;
			for (int i = 0; i < _registered_permissions.size(); ++i) {
				if ((_registered_permissions[i].value & perms) != 0) {
					if (pcount > 0) {
						b.w(", ");
					}

					b.w(_registered_permissions[i].name);

					++pcount;
				}
			}

			if (pcount == 0) {
				b.w("- None -");
			}

			b.w(", [ Rank Permissions ]: ");

			pcount = 0;
			perms = r->rank_permissions;
			for (int i = 0; i < _registered_rank_permissions.size(); ++i) {
				if ((_registered_rank_permissions[i].value & perms) != 0) {
					if (pcount > 0) {
						b.w(", ");
					}

					b.w(_registered_rank_permissions[i].name);

					++pcount;
				}
			}

			if (pcount == 0) {
				b.w("- None -");
			}

			b.ca();

			b.w(" - ");

			b.a()->href(request->get_url_root("edit_rank/") + String::num(r->id));
			b.w("[ Edit ]");
			b.ca();
		}
		b.cdiv();
	}

	b.br();

	b.a()->href(request->get_url_root("new_rank"));
	b.w("New Rank");
	b.ca();

	request->body += b.result;
}

void RBACController::admin_render_rank_editor(Request *request) {
}

String RBACController::admin_get_section_name() {
	return "Role Based Access Control";
}

void RBACController::admin_add_section_links(Vector<AdminSectionLinkInfo> *links) {
	links->push_back(AdminSectionLinkInfo("Editor", ""));
}

void RBACController::register_permission(const String &name, const int val) {
	_registered_permissions.push_back(PermissionEntry(name, val));
}
void RBACController::register_rank_permission(const String &name, const int val) {
	_registered_rank_permissions.push_back(PermissionEntry(name, val));
}
void RBACController::clear_registered_permissions() {
	_registered_permissions.clear();
	_registered_rank_permissions.clear();
}

void RBACController::initialize() {
	_ranks = db_load_ranks();
	_default_rank_id = db_get_default_rank();
	_default_user_rank_id = db_get_default_user_rank();

	register_permissions();
}

void RBACController::register_permissions() {
	register_permission("Create", User::PERMISSION_CREATE);
	register_permission("Read", User::PERMISSION_READ);
	register_permission("Update", User::PERMISSION_UPDATE);
	register_permission("Delete", User::PERMISSION_DELETE);

	register_rank_permission("Admin Panel", RBAC_RANK_PERMISSION_ADMIN_PANEL);
	register_rank_permission("Use Redirect", RBAC_RANK_PERMISSION_USE_REDIRECT);
}

Ref<RBACRank> RBACController::get_rank(int rank_id) {
	return _ranks[rank_id];
}

int RBACController::get_default_user_rank_id() {
	return _default_user_rank_id;
}
Ref<RBACRank> RBACController::get_default_user_rank() {
	return _ranks[get_default_user_rank_id()];
}

int RBACController::get_default_rank_id() {
	return _default_rank_id;
}

Ref<RBACRank> RBACController::get_default_rank() {
	return _ranks[get_default_rank_id()];
}

String &RBACController::get_redirect_url() {
	return _redirect_url;
}

bool RBACController::continue_on_missing_default_rank() {
	//todo, add setting
	return false;
}

//DB

std::map<int, Ref<RBACRank> > RBACController::db_load_ranks() {
	std::map<int, Ref<RBACRank> > ranks;

	Ref<QueryBuilder> qb = get_query_builder();

	qb->select("id,name,name_internal,settings,base_permissions,rank_permissions")->from(_rbac_ranks_table);
	Ref<QueryResult> res =  qb->run();

	while (res->next_row()) {
		Ref<RBACRank> r;
		r.instance();

		r->id = res->get_cell_int(0);
		r->name = res->get_cell_str(1);
		r->name_internal = res->get_cell_str(2);
		r->settings = res->get_cell_str(3);
		r->base_permissions = res->get_cell_int(4);
		r->rank_permissions = res->get_cell_int(5);

		ranks[r->id] = r;
	}

	qb->reset();
	qb->select("id,rank_id,name,url,sort_order,permissions")->from(_rbac_permissions_table);
	res = qb->run();

	while (res->next_row()) {
		Ref<RBACPermission> p;
		p.instance();

		p->id = res->get_cell_int(0);
		p->rank_id = res->get_cell_int(1);
		p->name = res->get_cell_str(2);
		p->url = res->get_cell_str(3);
		p->sort_order = res->get_cell_int(4);
		p->permissions = res->get_cell_int(5);

		Ref<RBACRank> r = ranks[p->rank_id];

		if (!r.is_valid()) {
			RLOG_ERR("RBACModel::load_permissions: !r.is_valid()!");
			continue;
		}

		r->permissions.push_back(p);
	}

	for (std::map<int, Ref<RBACRank> >::iterator i = ranks.begin(); i != ranks.end(); ++i) {
		Ref<RBACRank> r = i->second;

		if (r.is_valid()) {
			r->sort_permissions();
		}
	}

	return ranks;
}

void RBACController::db_save(const Ref<RBACRank> &rank) {
	db_save_rank(rank);

	for (int i = 0; i < rank->permissions.size(); ++i) {
		Ref<RBACPermission> permission = rank->permissions[i];

		int rid = rank->id;

		if (permission->rank_id != rid) {
			permission->rank_id = rid;
		}

		db_save_permission(permission);
	}
}

void RBACController::db_save_rank(const Ref<RBACRank> &rank) {
	Ref<QueryBuilder> qb = get_query_builder();

	if (rank->id == 0) {
		qb->insert(_rbac_ranks_table, "name,name_internal,settings,base_permissions,rank_permissions")->values();
		qb->val(rank->name)->val(rank->name_internal)->val(rank->settings)->val(rank->base_permissions)->val(rank->rank_permissions);
		qb->cvalues();
		qb->select_last_insert_id();
		Ref<QueryResult> res = qb->run();
		//qb->print();

		Ref<RBACRank> r = rank;

		r->id = res->get_last_insert_rowid();
	} else {
		qb->update(_rbac_ranks_table)->set();
		qb->setp("name", rank->name);
		qb->setp("name_internal", rank->name_internal);
		qb->setp("settings", rank->settings);
		qb->setp("base_permissions", rank->base_permissions);
		qb->setp("rank_permissions", rank->rank_permissions);
		qb->cset();
		qb->where()->wp("id", rank->id);
		qb->end_command();
		qb->run_query();
		//qb->print();
	}
}

void RBACController::db_save_permission(const Ref<RBACPermission> &permission) {
	Ref<QueryBuilder> qb = get_query_builder();

	if (permission->id == 0) {
		qb->insert(_rbac_permissions_table, "rank_id,name,url,sort_order,permissions")->values();
		qb->val(permission->rank_id)->val(permission->name)->val(permission->url);
		qb->val(permission->sort_order)->val(permission->permissions);
		qb->cvalues();
		qb->select_last_insert_id();
		Ref<QueryResult> res = qb->run();
		//qb->print();

		Ref<RBACPermission> r = permission;

		r->id = res->get_last_insert_rowid();
	} else {
		qb->update(_rbac_permissions_table)->set();
		qb->setp("rank_id", permission->rank_id);
		qb->setp("name", permission->name);
		qb->setp("url", permission->url);
		qb->setp("sort_order", permission->sort_order);
		qb->setp("permissions", permission->permissions);
		qb->cset();
		qb->where()->wp("id", permission->id);
		qb->end_command();
		qb->run_query();
		//qb->print();
	}
}

int RBACController::db_get_default_rank() {
	//todo, load this, and save it to a table (probably a new settings class)
	return 3;
}

int RBACController::db_get_default_user_rank() {
	//todo, load this, and save it to a table (probably a new settings class)
	return 2;
}

String RBACController::db_get_redirect_url() {
	//todo, load this, and save it to a table (probably a new settings class)
	return String("/user/login");
}

void RBACController::create_table() {
	Ref<TableBuilder> tb = get_table_builder();

	tb->create_table(_rbac_ranks_table);
	tb->integer("id")->auto_increment()->next_row();
	tb->varchar("name", 60)->not_null()->next_row();
	tb->varchar("name_internal", 100)->not_null()->next_row();
	tb->varchar("settings", 200)->not_null()->next_row();
	tb->integer("base_permissions")->not_null()->next_row();
	tb->integer("rank_permissions")->not_null()->next_row();
	tb->primary_key("id");
	tb->ccreate_table();
	//tb->run_query();
	//tb->print();

	//tb->result = "";

	tb->create_table(_rbac_permissions_table);
	tb->integer("id")->auto_increment()->next_row();
	tb->integer("rank_id")->not_null()->next_row();
	tb->varchar("name", 60)->not_null()->next_row();
	tb->varchar("url", 100)->not_null()->next_row();
	tb->integer("sort_order")->not_null()->next_row();
	tb->integer("permissions")->not_null()->next_row();

	tb->primary_key("id");
	tb->foreign_key("rank_id")->references(_rbac_ranks_table, "id");
	tb->ccreate_table();
	tb->run_query();
	//tb->print();
}
void RBACController::drop_table() {
	Ref<TableBuilder> tb = get_table_builder();

	tb->drop_table_if_exists(_rbac_permissions_table)->drop_table_if_exists(_rbac_ranks_table)->run_query();
	//tb->print();
}

void RBACController::create_default_entries() {
	Ref<RBACRank> admin;
	admin.instance();

	admin->name = "Admin";
	admin->base_permissions = User::PERMISSION_ALL;
	admin->rank_permissions = RBAC_RANK_PERMISSION_ADMIN_PANEL;

	db_save_rank(admin);

	Ref<RBACRank> user;
	user.instance();

	user->name = "User";
	//user->base_permissions = User::PERMISSION_READ;
	//user->rank_permissions = 0;

	//temporary!
	user->base_permissions = User::PERMISSION_ALL;
	user->rank_permissions = RBAC_RANK_PERMISSION_ADMIN_PANEL;

	db_save_rank(user);

	Ref<RBACRank> guest;
	guest.instance();

	guest->name = "Guest";
	guest->base_permissions = User::PERMISSION_READ;
	guest->rank_permissions = RBAC_RANK_PERMISSION_USE_REDIRECT;

	db_save_rank(guest);
}


RBACController *RBACController::get_singleton() {
	return _self;
}

RBACController::RBACController() :
		AdminNode() {

	if (_self) {
		printf("RBACController::RBACController(): Error! self is not null!/n");
	}

	_default_rank_id = 0;
	_default_user_rank_id = 0;

	_rbac_ranks_table = "rbac_ranks";
	_rbac_permissions_table = "rbac_permissions";

	_self = this;
}

RBACController::~RBACController() {
	if (_self == this) {
		_self = nullptr;
	}
}

RBACController *RBACController::_self = nullptr;
