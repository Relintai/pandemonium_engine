

#include "register_types.h"

#include "database.h"
#include "database_connection.h"
#include "database_manager.h"
#include "database_multi_threaded.h"
#include "database_single_threaded.h"
#include "query_builder.h"
#include "query_result.h"
#include "table_builder.h"

#include "core/config/engine.h"

DatabaseManager *_database_manager = nullptr;

void register_database_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		_database_manager = memnew(DatabaseManager);
		ClassDB::register_class<DatabaseManager>();
		Engine::get_singleton()->add_singleton(Engine::Singleton("DatabaseManager", DatabaseManager::get_singleton()));
	}

	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<Database>();
		ClassDB::register_class<DatabaseConnection>();
		ClassDB::register_class<DatabaseMultiThreaded>();
		ClassDB::register_class<DatabaseSingleThreaded>();
		ClassDB::register_class<QueryBuilder>();
		ClassDB::register_class<QueryResult>();
		ClassDB::register_class<TableBuilder>();
	}
}

void unregister_database_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		if (_database_manager) {
			memdelete(_database_manager);
		}
	}
}
