
#include "register_types.h"

#include "overlapping_wave_form_collapse.h"
#include "tiling_wave_form_collapse.h"
#include "wave_form_collapse.h"
#include "image_indexer.h"

void register_wfc_types(ModuleRegistrationLevel p_level) {
	ClassDB::register_class<WaveFormCollapse>();
	ClassDB::register_class<OverlappingWaveFormCollapse>();
	ClassDB::register_class<TilingWaveFormCollapse>();
	ClassDB::register_class<ImageIndexer>();
}

void unregister_wfc_types(ModuleRegistrationLevel p_level) {
}
