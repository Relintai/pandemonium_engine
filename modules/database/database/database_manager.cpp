#include "database_manager.h"

void DatabaseManager::load() {
	//go thourgh settings, and create all the defined db backends
}

uint32_t DatabaseManager::create_database(const String &name) {
    Database *db = _create_database(name);

    if (!db) {
		RLOG_MSG("(DatabaseManager) create_database: " + name + ", returned db is null!");
        return -1;
	}

	RLOG_MSG("(DatabaseManager) Database " + name + " successfully created!");

    databases.push_back(db);

	if (ddb == nullptr) {
		RLOG_MSG("(DatabaseManager) Database " + name + " has been set as the default database!");
		ddb = db;
	}

    return databases.size() - 1;
}

DatabaseManager *DatabaseManager::get_singleton() {
	return _instance;
}

void DatabaseManager::_register_db_creation_func(const String &name, std::function<Database *()> func) {
	ERR_FAIL_COND_MSG(!func, "_register_db_creation_func: " + name + ", func is wrong!");

	_db_creation_func_map[name] = func;
}

void DatabaseManager::_unregister_db_creation_func(const String &name) {
	_db_creation_func_map.erase(name);
}

Database *DatabaseManager::_create_database(const String &name) {
	std::function<Database *()> func = _db_creation_func_map[name];

	ERR_FAIL_COND_V_MSG(!func, nullptr, "_create_database: " + name + ", func is wrong!");

    Database *db = func();

	ERR_FAIL_COND_V_MSG(!db, nullptr, "_create_database: " + name + ", returned db is null!");

    return db;
}

DatabaseManager::DatabaseManager() {
	_instance = this;

	ddb = nullptr;
}

DatabaseManager::~DatabaseManager() {
	_instance = nullptr;

	for (uint32_t i = 0; i < databases.size(); ++i) {
		delete databases[i];
	}
}

DatabaseManager *DatabaseManager::_instance = nullptr;

std::map<String, std::function<Database *()> > DatabaseManager::_db_creation_func_map;