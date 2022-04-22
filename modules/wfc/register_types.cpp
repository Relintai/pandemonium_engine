
#include "register_types.h"

#include "overlapping_wave_form_collapse.h"
#include "tiling_wave_form_collapse.h"
#include "wave_form_collapse.h"

void register_wfc_types() {
	ClassDB::register_class<WaveFormCollapse>();
	ClassDB::register_class<OverlappingWaveFormCollapse>();
	ClassDB::register_class<TilingWaveFormCollapse>();
}

void unregister_wfc_types() {
}
