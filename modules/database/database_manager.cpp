#include "database_manager.h"

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

