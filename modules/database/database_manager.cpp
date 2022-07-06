#include "database_manager.h"

#include "database.h"

Ref<Database> DatabaseManager::get_ddb() {
	return _ddb;
}
void DatabaseManager::set_ddb(const Ref<Database> &db) {
	_ddb = db;
}

void DatabaseManager::load() {
	//go thourgh settings, and create all the defined db backends
	//add them to ProjectSettings
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

DatabaseManager *DatabaseManager::_instance = nullptr;
