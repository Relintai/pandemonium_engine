/*************************************************************************/
/*  user_manager_db.cpp                                                  */
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

#include "user_manager_db.h"

#include "core/containers/local_vector.h"

#include "../users/user.h"

#include "../../database/database.h"
#include "../../database/database_connection.h"
#include "../../database/database_manager.h"
#include "../../database/query_builder.h"
#include "../../database/query_result.h"
#include "../../database/table_builder.h"

#include "core/log/logger.h"

String UserManagerDB::get_database_table_name() {
	return _database_table_name;
}
void UserManagerDB::set_database_table_name(const String &val) {
	_database_table_name = val;
}

Ref<Database> UserManagerDB::get_database() {
	if (_database.is_valid()) {
		return _database;
	}

	return DatabaseManager::get_singleton()->get_ddb();
}

void UserManagerDB::set_database(const Ref<Database> &db) {
	_database = db;

	// todo send event to children when it's implemented?
}

Ref<DatabaseConnection> UserManagerDB::get_database_connection() {
	Ref<Database> db = get_database();

	ERR_FAIL_COND_V(!db.is_valid(), Ref<DatabaseConnection>());

	Ref<DatabaseConnection> conn = db->get_connection();

	ERR_FAIL_COND_V(!conn.is_valid(), Ref<DatabaseConnection>());

	return conn;
}

Ref<TableBuilder> UserManagerDB::get_table_builder() {
	Ref<Database> db = get_database();

	ERR_FAIL_COND_V(!db.is_valid(), Ref<TableBuilder>());

	Ref<DatabaseConnection> conn = db->get_connection();

	ERR_FAIL_COND_V(!conn.is_valid(), Ref<TableBuilder>());

	return conn->get_table_builder();
}

Ref<QueryBuilder> UserManagerDB::get_query_builder() {
	Ref<Database> db = get_database();

	ERR_FAIL_COND_V(!db.is_valid(), Ref<QueryBuilder>());

	Ref<DatabaseConnection> conn = db->get_connection();

	ERR_FAIL_COND_V(!conn.is_valid(), Ref<QueryBuilder>());

	return conn->get_query_builder();
}

Ref<User> UserManagerDB::_get_user(const int id) {
	if (id <= 0) {
		return Ref<User>();
	}

	Ref<QueryBuilder> b = get_query_builder();

	// username_internal, email_internal doesn't need to be queried
	b->select("username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_database_table_name);

	b->where()->wpi("id", id);

	b->end_command();

	Ref<QueryResult> r = b->run();

	if (!r->next_row()) {
		return Ref<User>();
	}

	Ref<User> user;
	user = create_user();

	user->set_user_id(id);
	user->set_user_name(r->get_cell(0));
	user->set_email(r->get_cell(1));
	user->set_rank(r->get_cell_int(2));
	user->set_pre_salt(r->get_cell(3));
	user->set_post_salt(r->get_cell(4));
	user->set_password_hash(r->get_cell(5));
	user->set_banned(r->get_cell_bool(6));
	user->set_password_reset_token(r->get_cell(7));
	user->set_locked(r->get_cell_bool(8));

	return user;
}
Ref<User> UserManagerDB::_get_user_name(const String &p_user_name) {
	if (p_user_name.empty()) {
		return Ref<User>();
	}

	String user_name = User::string_to_internal_format(p_user_name);

	Ref<QueryBuilder> b = get_query_builder();

	b->select("id, username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_database_table_name);
	b->where()->wps("username_internal", user_name);
	b->end_command();

	Ref<QueryResult> r = b->run();

	if (!r->next_row()) {
		return Ref<User>();
	}

	Ref<User> user;
	user = create_user();

	user->set_user_id(r->get_cell_int(0));
	user->set_user_name(r->get_cell(1));
	user->set_email(r->get_cell(2));
	user->set_rank(r->get_cell_int(3));
	user->set_pre_salt(r->get_cell(4));
	user->set_post_salt(r->get_cell(5));
	user->set_password_hash(r->get_cell(6));
	user->set_banned(r->get_cell_bool(7));
	user->set_password_reset_token(r->get_cell(8));
	user->set_locked(r->get_cell_bool(9));

	return user;
}
Ref<User> UserManagerDB::_get_user_email(const String &p_user_email) {
	if (p_user_email.empty()) {
		return Ref<User>();
	}

	String user_email = User::string_to_internal_format(p_user_email);

	Ref<QueryBuilder> b = get_query_builder();

	b->select("id, username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_database_table_name);
	b->where()->wps("email_internal", user_email);
	b->end_command();

	Ref<QueryResult> r = b->run();

	if (!r->next_row()) {
		return Ref<User>();
	}

	Ref<User> user;
	user = create_user();

	user->set_user_id(r->get_cell_int(0));
	user->set_user_name(r->get_cell(1));
	user->set_email(r->get_cell(2));
	user->set_rank(r->get_cell_int(3));
	user->set_pre_salt(r->get_cell(4));
	user->set_post_salt(r->get_cell(5));
	user->set_password_hash(r->get_cell(6));
	user->set_banned(r->get_cell_bool(7));
	user->set_password_reset_token(r->get_cell(8));
	user->set_locked(r->get_cell_bool(9));

	return user;
}

void UserManagerDB::_save_user(Ref<User> user) {
	Ref<QueryBuilder> b = get_query_builder();

	if (user->get_user_id() == -1) {
		b->insert(_database_table_name, "username, username_internal, email, email_internal, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");

		user->write_lock();

		b->values();
		b->vals(user->get_user_name());
		b->vals(user->get_user_name_internal());
		b->vals(user->get_email());
		b->vals(user->get_email_internal());
		b->vali(user->get_rank());
		b->vals(user->get_pre_salt());
		b->vals(user->get_post_salt());
		b->vals(user->get_password_hash());
		b->valb(user->get_banned());
		b->vals(user->get_password_reset_token());
		b->valb(user->get_locked());
		b->cvalues();

		b->end_command();
		b->select_last_insert_id();

		Ref<QueryResult> r = b->run();

		user->set_user_id(r->get_last_insert_rowid());

		user->write_unlock();
	} else {
		b->update(_database_table_name);
		b->sets();

		user->read_lock();

		b->setps("username", user->get_user_name());
		b->setps("username_internal", user->get_user_name_internal());
		b->setps("email", user->get_email());
		b->setps("email_internal", user->get_email_internal());
		b->setpi("rank", user->get_rank());
		b->setps("pre_salt", user->get_pre_salt());
		b->setps("post_salt", user->get_post_salt());
		b->setps("password_hash", user->get_password_hash());
		b->setpb("banned", user->get_banned());
		b->setps("password_reset_token", user->get_password_reset_token());
		b->setpb("locked", user->get_locked());
		b->cset();
		b->where()->wpi("id", user->get_user_id());

		user->read_unlock();

		// b->print();

		b->run_query();
	}
}
Ref<User> UserManagerDB::_create_user(Ref<User> p_user) {
	if (!p_user.is_valid()) {
		p_user.instance();
	}

	return UserManager::_create_user(p_user);
}
bool UserManagerDB::_is_username_taken(const String &p_user_name) {
	Ref<QueryBuilder> b = get_query_builder();

	String user_name = User::string_to_internal_format(p_user_name);

	b->select("id")->from(_database_table_name)->where("username_internal")->like(user_name)->end_command();

	Ref<QueryResult> r = b->run();

	return r->next_row();
}
bool UserManagerDB::_is_email_taken(const String &p_email) {
	Ref<QueryBuilder> b = get_query_builder();

	String email = User::string_to_internal_format(p_email);

	b->select("id")->from(_database_table_name)->where("email_internal")->like(email)->end_command();

	Ref<QueryResult> r = b->run();

	return r->next_row();
}

Array UserManagerDB::_get_all_users() {
	Ref<QueryBuilder> b = get_query_builder();

	b->select("id, username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_database_table_name);
	b->end_command();
	// b->print();

	Array users;

	Ref<QueryResult> r = b->run();

	while (r->next_row()) {
		Ref<User> user = create_user();

		user->set_user_id(r->get_cell_int(0));
		user->set_user_name(r->get_cell(1));
		user->set_email(r->get_cell(2));
		user->set_rank(r->get_cell_int(3));
		user->set_pre_salt(r->get_cell(4));
		user->set_post_salt(r->get_cell(5));
		user->set_password_hash(r->get_cell(6));
		user->set_banned(r->get_cell_bool(7));
		user->set_password_reset_token(r->get_cell(8));
		user->set_locked(r->get_cell_bool(9));

		users.push_back(user.get_ref_ptr());
	}

	return users;
}

void UserManagerDB::create_table() {
	call("_create_table");
}
void UserManagerDB::drop_table() {
	call("_drop_table");
}
void UserManagerDB::update_table(const int p_current_table_version) {
	call("_update_table", p_current_table_version);
}
void UserManagerDB::create_default_entries(const int p_seed) {
	call("_create_default_entries", p_seed);
}

void UserManagerDB::_create_table() {
	Ref<TableBuilder> tb = get_table_builder();

	tb->create_table(_database_table_name);
	tb->integer("id")->auto_increment()->next_row();
	tb->varchar("username", 60)->not_null()->next_row();
	tb->varchar("username_internal", 60)->not_null()->next_row();
	tb->varchar("email", 100)->not_null()->next_row();
	tb->varchar("email_internal", 100)->not_null()->next_row();
	tb->integer("rank")->not_null()->next_row();
	tb->varchar("pre_salt", 100)->next_row();
	tb->varchar("post_salt", 100)->next_row();
	tb->varchar("password_hash", 100)->next_row();
	tb->integer("banned")->next_row();
	tb->varchar("password_reset_token", 100)->next_row();
	tb->integer("locked")->next_row();
	tb->primary_key("id");
	tb->ccreate_table();
	tb->run_query();
	// tb->print();

	get_database_connection()->set_table_version(_database_table_name, 1);
}
void UserManagerDB::_drop_table() {
	Ref<TableBuilder> tb = get_table_builder();

	tb->drop_table_if_exists(_database_table_name)->run_query();
	
	get_database_connection()->set_table_version(_database_table_name, -1);
}

void UserManagerDB::_update_table(const int p_current_table_version) {
	if (p_current_table_version <= 0) {
		// Added username_internal after username
		// Added email_internal after email

		Ref<TableBuilder> tb = get_table_builder();

		tb->alter_table(_database_table_name);
		tb->add_column();
		tb->varchar("username_internal", 60)->not_null();
		tb->end_command();

		tb->alter_table(_database_table_name);
		tb->add_column();
		tb->varchar("email_internal", 100)->not_null();
		tb->end_command();

		tb->run_query();

		// Now update users
		Ref<QueryBuilder> b = get_query_builder();

		b->select("id,username,email");
		b->from(_database_table_name);
		b->end_command();
		// b->print();

		Ref<QueryResult> r = b->run();

		struct UserData {
			int id;
			String username;
			String email;
		};

		LocalVector<UserData> user_data;

		while (r->next_row()) {
			UserData u;

			u.id = r->get_next_cell_int();
			u.username = r->get_next_cell();
			u.email = r->get_next_cell();

			user_data.push_back(u);
		}

		for (uint32_t i = 0; i < user_data.size(); ++i) {
			UserData u = user_data[i];

			b->reset();

			b->update(_database_table_name);
			b->sets();

			b->setps("username_internal", User::string_to_internal_format(u.username));
			b->setps("email_internal", User::string_to_internal_format(u.email));
			b->cset();
			b->where()->wpi("id", u.id);

			b->run_query();
		}

		get_database_connection()->set_table_version(_database_table_name, 1);

		PLOG_IMPORTANT("Updated user tables to version 1!");
	}
}

void UserManagerDB::_create_default_entries(const int p_seed) {
	/*
	Ref<User> user;
	user = create_user();

	user->set_rank(3);
	user->set_user_name("admin");
	user->set_email("admin@admin.com");

	user->create_password("Password");
	user->save();

	user = create_user();

	user->set_rank(1);
	user->set_user_name("user");
	user->set_email("user@user.com");

	user->create_password("Password");
	user->save();
	*/
}

void UserManagerDB::migrate(const bool p_clear, const bool p_should_seed, const int p_seed) {
	call("_migrate", p_clear, p_should_seed, p_seed);
}

void UserManagerDB::_migrate(const bool p_clear, const bool p_should_seed, const int p_seed) {
	if (p_clear) {
		drop_table();
		create_table();
	} else {
		Ref<DatabaseConnection> conn = get_database_connection();
		ERR_FAIL_COND(!conn.is_valid());
		int ver = conn->get_table_version(_database_table_name);
		update_table(ver);
	}

	if (p_should_seed) {
		create_default_entries(p_seed);
	}
}

UserManagerDB::UserManagerDB() {
	_database_table_name = "users";
}

UserManagerDB::~UserManagerDB() {
}

void UserManagerDB::_notification(const int what) {
	switch (what) {
		case NOTIFICATION_ENTER_TREE: {
			DatabaseManager::get_singleton()->connect("migration", this, "migrate");
		} break;
		case NOTIFICATION_EXIT_TREE: {
			DatabaseManager::get_singleton()->disconnect("migration", this, "migrate");
		} break;
		default:
			break;
	}
}

void UserManagerDB::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_database_table_name"), &UserManagerDB::get_database_table_name);
	ClassDB::bind_method(D_METHOD("set_database_table_name", "val"), &UserManagerDB::set_database_table_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "database_table_name"), "set_database_table_name", "get_database_table_name");

	ClassDB::bind_method(D_METHOD("get_database"), &UserManagerDB::get_database);
	ClassDB::bind_method(D_METHOD("set_database", "val"), &UserManagerDB::set_database);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "database", PROPERTY_HINT_RESOURCE_TYPE, "Database", 0), "set_database", "get_database");

	ClassDB::bind_method(D_METHOD("get_database_connection"), &UserManagerDB::get_database_connection);
	ClassDB::bind_method(D_METHOD("get_table_builder"), &UserManagerDB::get_table_builder);
	ClassDB::bind_method(D_METHOD("get_query_builder"), &UserManagerDB::get_query_builder);

	BIND_VMETHOD(MethodInfo("_create_table"));
	BIND_VMETHOD(MethodInfo("_drop_table"));
	BIND_VMETHOD(MethodInfo("_update_table", PropertyInfo(Variant::INT, "current_table_version")));
	BIND_VMETHOD(MethodInfo("_create_default_entries", PropertyInfo(Variant::INT, "pseed")));

	ClassDB::bind_method(D_METHOD("create_table"), &UserManagerDB::create_table);
	ClassDB::bind_method(D_METHOD("drop_table"), &UserManagerDB::drop_table);
	ClassDB::bind_method(D_METHOD("update_table", "current_table_version"), &UserManagerDB::update_table);
	ClassDB::bind_method(D_METHOD("create_default_entries", "pseed"), &UserManagerDB::create_default_entries);

	ClassDB::bind_method(D_METHOD("_create_table"), &UserManagerDB::_create_table);
	ClassDB::bind_method(D_METHOD("_drop_table"), &UserManagerDB::_drop_table);
	ClassDB::bind_method(D_METHOD("_update_table", "current_table_version"), &UserManagerDB::_update_table);
	ClassDB::bind_method(D_METHOD("_create_default_entries", "pseed"), &UserManagerDB::_create_default_entries);

	BIND_VMETHOD(MethodInfo("_migrate", PropertyInfo(Variant::BOOL, "clear"), PropertyInfo(Variant::BOOL, "should_seed"), PropertyInfo(Variant::INT, "pseed")));
	ClassDB::bind_method(D_METHOD("migrate", "clear", "pseed"), &UserManagerDB::migrate);
	ClassDB::bind_method(D_METHOD("_migrate", "clear", "pseed"), &UserManagerDB::_migrate);
}
