#include "register_types.h"

#include "core/object/class_db.h"

#include "fastnoise_noise_params.h"
#include "noise.h"

void register_fastnoise_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<FastNoise>();
		ClassDB::register_class<FastnoiseNoiseParams>();
	}
}

void unregister_fastnoise_types(ModuleRegistrationLevel p_level) {
}
