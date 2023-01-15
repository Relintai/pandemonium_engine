
#include "register_types.h"

#include "lz4_compressor.h"

void register_lz4_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<LZ4Compressor>();
	}
}

void unregister_lz4_types(ModuleRegistrationLevel p_level) {
}
