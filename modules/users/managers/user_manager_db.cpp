#include "user_manager_db.h"

#include "../users/user.h"

Ref<User> UserManagerDB::_get_user(const int id) {
	if (id == 0) {
		return Ref<User>();
	}

	Ref<QueryBuilder> b = get_query_builder();

	b->select("username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_table_name);

	b->where()->wp("id", id);

	b->end_command();

	Ref<QueryResult> r = b->run();

	if (!r->next_row()) {
		return Ref<User>();
	}

	Ref<User> user;
	user = create_user();

	user->id = id;
	user->name_user_input = r->get_cell(0);
	user->email_user_input = r->get_cell(1);
	user->rank = r->get_cell_int(2);
	user->pre_salt = r->get_cell(3);
	user->post_salt = r->get_cell(4);
	user->password_hash = r->get_cell(5);
	user->banned = r->get_cell_bool(6);
	user->password_reset_token = r->get_cell(7);
	user->locked = r->get_cell_bool(8);

	return user;
}
Ref<User> UserManagerDB::_get_user_name(const String &user_name) {
	if (user_name_input == "") {
		return Ref<User>();
	}

	Ref<QueryBuilder> b = get_query_builder();

	b->select("id, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_table_name);
	b->where()->wp("username", user_name_input);
	b->end_command();

	Ref<QueryResult> r = b->run();

	if (!r->next_row()) {
		return Ref<User>();
	}

	Ref<User> user;
	user = create_user();

	user->id = r->get_cell_int(0);
	user->name_user_input = user_name_input;
	user->email_user_input = r->get_cell(1);
	user->rank = r->get_cell_int(2);
	user->pre_salt = r->get_cell(3);
	user->post_salt = r->get_cell(4);
	user->password_hash = r->get_cell(5);
	user->banned = r->get_cell_bool(6);
	user->password_reset_token = r->get_cell(7);
	user->locked = r->get_cell_bool(8);

	return user;
}
void UserManagerDB::_save_user(Ref<User> user) {
	Ref<QueryBuilder> b = get_query_builder();

	if (user->id == 0) {
		b->insert(_table_name, "username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");

		b->values();
		b->val(user->name_user_input);
		b->val(user->email_user_input);
		b->val(user->rank);
		b->val(user->pre_salt);
		b->val(user->post_salt);
		b->val(user->password_hash);
		b->val(user->banned);
		b->val(user->password_reset_token);
		b->val(user->locked);
		b->cvalues();

		b->end_command();
		b->select_last_insert_id();

		Ref<QueryResult> r = b->run();

		user->id = r->get_last_insert_rowid();
	} else {
		b->update(_table_name);
		b->set();
		b->setp("username", user->name_user_input);
		b->setp("email", user->email_user_input);
		b->setp("rank", user->rank);
		b->setp("pre_salt", user->pre_salt);
		b->setp("post_salt", user->post_salt);
		b->setp("password_hash", user->password_hash);
		b->setp("banned", user->banned);
		b->setp("password_reset_token", user->password_reset_token);
		b->setp("locked", user->locked);
		b->cset();
		b->where()->wp("id", user->id);

		// b->print();

		b->run_query();
	}
}
Ref<User> UserManagerDB::_create_user() {
	Ref<User> u;
	u.instance();

	save_user(u);

	return u;
}
bool UserManagerDB::_is_username_taken(const String &user_name) {
	Ref<QueryBuilder> b = get_query_builder();

	b->select("id")->from(_table_name)->where("username")->like(user_name_input)->end_command();

	Ref<QueryResult> r = b->run();

	return r->next_row();
}
bool UserManagerDB::_is_email_taken(const String &email) {
	Ref<QueryBuilder> b = get_query_builder();

	b->select("id")->from(_table_name)->where("username")->like(email_input)->end_command();

	Ref<QueryResult> r = b->run();

	return r->next_row();
}

Vector<Ref<User>> UserManagerDB::get_all() {
	Ref<QueryBuilder> b = get_query_builder();

	b->select("id, username, email, rank, pre_salt, post_salt, password_hash, banned, password_reset_token, locked");
	b->from(_table_name);
	b->end_command();
	// b->print();

	Vector<Ref<User>> users;

	Ref<QueryResult> r = b->run();

	while (r->next_row()) {
		Ref<User> user = create_user();

		user->id = r->get_cell_int(0);
		user->name_user_input = r->get_cell(1);
		user->email_user_input = r->get_cell(2);
		user->rank = r->get_cell_int(3);
		user->pre_salt = r->get_cell(4);
		user->post_salt = r->get_cell(5);
		user->password_hash = r->get_cell(6);
		user->banned = r->get_cell_bool(7);
		user->password_reset_token = r->get_cell(8);
		user->locked = r->get_cell_bool(9);

		users.push_back(user);
	}

	return users;
}

void UserManagerDB::create_table() {
	Ref<TableBuilder> tb = get_table_builder();

	tb->create_table(_table_name);
	tb->integer("id")->auto_increment()->next_row();
	tb->varchar("username", 60)->not_null()->next_row();
	tb->varchar("email", 100)->not_null()->next_row();
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
}
void UserManagerDB::drop_table() {
	Ref<TableBuilder> tb = get_table_builder();

	tb->drop_table_if_exists(_table_name)->run_query();
}

void UserManagerDB::create_default_entries() {
	Ref<User> user;
	user = create_user();

	user->rank = 3;
	user->name_user_input = "admin";
	user->email_user_input = "admin@admin.com";

	create_password(user, "Password");
	db_save_user(user);

	user = create_user();

	user->rank = 1;
	user->name_user_input = "user";
	user->email_user_input = "user@user.com";

	create_password(user, "Password");
	db_save_user(user);
}

UserManagerDB::UserManagerDB() {
}

UserManagerDB::~UserManagerDB() {
}

void UserManagerDB::_bind_methods() {
}
