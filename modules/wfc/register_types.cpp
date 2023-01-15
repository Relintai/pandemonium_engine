
#include "register_types.h"

#include "image_indexer.h"
#include "overlapping_wave_form_collapse.h"
#include "tiling_wave_form_collapse.h"
#include "wave_form_collapse.h"

void register_wfc_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<WaveFormCollapse>();
		ClassDB::register_class<OverlappingWaveFormCollapse>();
		ClassDB::register_class<TilingWaveFormCollapse>();
		ClassDB::register_class<ImageIndexer>();
	}
}

void unregister_wfc_types(ModuleRegistrationLevel p_level) {
}
