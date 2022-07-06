#include "db_settings.h"

#include "database/database.h"
#include "database/database_manager.h"
#include "database/query_builder.h"
#include "database/query_result.h"
#include "database/table_builder.h"

void DBSettings::set_value(const String &key, const Variant &value) {
	const Variant &v = _data[key];

	int id = 0;

	if (!v.is_null()) {
		id = _key_map[key];
	}

	_data[key] = value;

	Ref<QueryBuilder> qb = DatabaseManager::get_singleton()->ddb->get_query_builder();

	if (id == 0) {
		qb->insert(_table, "key,value")->values()->val(key)->val(value.to_string())->cvalues();
		qb->select_last_insert_id();
		Ref<QueryResult> res = qb->run();

		id = res->get_last_insert_rowid();

		 _key_map[key] = id;
	} else {
		qb->update(_table)->set()->setp("key", key)->setp("value", value.to_string())->cset()->where()->wp("id", id);
		qb->run_query();
	}
}

void DBSettings::load() {
	_data.clear();
	_key_map.clear();

	Ref<QueryBuilder> qb = DatabaseManager::get_singleton()->ddb->get_query_builder();
	qb->select("id,key,value")->from(_table);

	Ref<QueryResult> res = qb->run();

	while (res->next_row()) {
		int id = res->get_cell_int(0);
		String key = res->get_cell_str(1);
		String value = res->get_cell_str(2);

		_data[key] = Variant::parse_string(value);
		_key_map[key] = id;
	}
}

void DBSettings::migrate() {
	Ref<TableBuilder> tb = DatabaseManager::get_singleton()->ddb->get_table_builder();
	tb->drop_table_if_exists(_table);

	tb->create_table(_table);
	tb->integer("id")->auto_increment()->next_row();
	tb->varchar("key", 300)->not_null()->next_row();
	tb->text("value")->not_null()->next_row();
	tb->primary_key("id");
	tb->ccreate_table();

	tb->run_query();
}

void DBSettings::set_table(const String &table) {
	_table = table;
}

DBSettings *DBSettings::get_singleton() {
	return _db_settings_singleton;
}

DBSettings::DBSettings(const bool singleton) :
		Settings(singleton) {

	_table = "settings";

	if (singleton) {
		_db_settings_singleton = this;
	}
}

DBSettings::~DBSettings() {
}

DBSettings *DBSettings::_db_settings_singleton = nullptr;