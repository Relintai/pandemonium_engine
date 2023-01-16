
#include "register_types.h"

#include "core/config/engine.h"

#include "unit_test.h"
#include "unit_test_db.h"
#include "unit_test_runner.h"

static UnitTestDB *unit_test_manager = NULL;

void register_unit_test_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		unit_test_manager = memnew(UnitTestDB);
		ClassDB::register_class<UnitTestDB>();
		Engine::get_singleton()->add_singleton(Engine::Singleton("UnitTestDB", UnitTestDB::get_singleton()));
	}

	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<UnitTest>();
		ClassDB::register_class<UnitTestRunner>();
	}
}

void unregister_unit_test_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SINGLETON) {
		if (unit_test_manager) {
			memdelete(unit_test_manager);
		}
	}
}
