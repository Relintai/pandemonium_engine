/*************************************************************************/
/*  database_manager.cpp                                                 */
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

#include "database_manager.h"

#include "core/object/object.h"
#include "database.h"

Ref<Database> DatabaseManager::get_ddb() {
	return _ddb;
}
void DatabaseManager::set_ddb(const Ref<Database> &db) {
	_ddb = db;
	emit_signal("default_database_changed", db);
}

void DatabaseManager::add_database(const Ref<Database> &db, bool set_as_default) {
	ERR_FAIL_COND(!db.is_valid());

	_databases.push_back(db);
	emit_signal("database_added", db);

	if (set_as_default) {
		_ddb = db;
		emit_signal("default_database_changed", db);
	}
}
void DatabaseManager::remove_database(const int index, const bool unset_if_default) {
	ERR_FAIL_INDEX(index, _databases.size());

	Ref<Database> db = _databases[index];

	_databases.remove(index);
	emit_signal("database_removed", db);

	if (unset_if_default && _ddb == db) {
		_ddb.unref();
		emit_signal("default_database_changed", db);
	}
}
Ref<Database> DatabaseManager::get_database(const int index) {
	ERR_FAIL_INDEX_V(index, _databases.size(), Ref<Database>());

	return _databases[index];
}
int DatabaseManager::get_database_count() {
	return _databases.size();
}

Vector<Ref<Database>> DatabaseManager::get_databases() {
	return _databases;
}

Vector<Variant> DatabaseManager::get_databases_bind() {
	Vector<Variant> r;
	for (int i = 0; i < _databases.size(); i++) {
		r.push_back(_databases[i].get_ref_ptr());
	}
	return r;
}

void DatabaseManager::initialized() {
	emit_signal("initialized");
}

void DatabaseManager::load() {
	//go thourgh settings, and create all the defined db backends
	//add them to ProjectSettings
}

void DatabaseManager::migrate(const bool p_clear, const bool p_should_seed, const int p_seed) {
	emit_signal("migration", p_clear, p_should_seed, p_seed);
}

DatabaseManager *DatabaseManager::get_singleton() {
	return _instance;
}

DatabaseManager::DatabaseManager() {
	_instance = this;
}

DatabaseManager::~DatabaseManager() {
	_instance = nullptr;
}

void DatabaseManager::_bind_methods() {
	ADD_SIGNAL(MethodInfo("initialized"));
	ADD_SIGNAL(MethodInfo("migration", PropertyInfo(Variant::BOOL, "clear"), PropertyInfo(Variant::BOOL, "should_seed"), PropertyInfo(Variant::INT, "pseed")));

	ADD_SIGNAL(MethodInfo("default_database_changed", PropertyInfo(Variant::OBJECT, "db", PROPERTY_HINT_RESOURCE_TYPE, "Database")));
	ADD_SIGNAL(MethodInfo("database_added", PropertyInfo(Variant::OBJECT, "db", PROPERTY_HINT_RESOURCE_TYPE, "Database")));
	ADD_SIGNAL(MethodInfo("database_removed", PropertyInfo(Variant::OBJECT, "db", PROPERTY_HINT_RESOURCE_TYPE, "Database")));

	ClassDB::bind_method(D_METHOD("get_ddb"), &DatabaseManager::get_ddb);
	ClassDB::bind_method(D_METHOD("set_ddb"), &DatabaseManager::set_ddb);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "ddb", PROPERTY_HINT_RESOURCE_TYPE, "Database"), "set_ddb", "get_ddb");

	ClassDB::bind_method(D_METHOD("add_database", "db", "set_as_default"), &DatabaseManager::add_database, true);
	ClassDB::bind_method(D_METHOD("remove_database", "index", "unset_if_default"), &DatabaseManager::remove_database, true);
	ClassDB::bind_method(D_METHOD("get_database", "index"), &DatabaseManager::get_database);
	ClassDB::bind_method(D_METHOD("get_database_count"), &DatabaseManager::get_database_count);

	ClassDB::bind_method(D_METHOD("get_databases"), &DatabaseManager::get_databases_bind);

	ClassDB::bind_method(D_METHOD("initialized"), &DatabaseManager::initialized);
	ClassDB::bind_method(D_METHOD("load"), &DatabaseManager::load);

	ClassDB::bind_method(D_METHOD("migrate", "clear", "should_seed", "pseed"), &DatabaseManager::migrate);
}

DatabaseManager *DatabaseManager::_instance = nullptr;
