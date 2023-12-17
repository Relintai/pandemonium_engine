

#include "terrain_light_job.h"

#include "../../defines.h"

#include "../../library/terrain_library.h"
#include "../../library/terrain_surface.h"

#include "../../meshers/terrain_mesher.h"
#include "../default/terrain_chunk_default.h"

void TerrainLightJob::phase_light() {
	Ref<TerrainChunkDefault> chunk = _chunk;

	if ((chunk->get_build_flags() & TerrainChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
		if (!chunk->channel_get(TerrainChunkDefault::DEFAULT_CHANNEL_AO))
			generate_ao();

	bool gr = (chunk->get_build_flags() & TerrainChunkDefault::BUILD_FLAG_AUTO_GENERATE_RAO) != 0;

	if (!gr && (chunk->get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_LIGHTING) == 0) {
		next_phase();
		return;
	}

	bool bl = (chunk->get_build_flags() & TerrainChunkDefault::BUILD_FLAG_BAKE_LIGHTS) != 0;

	if (bl && should_do()) {
		chunk->clear_baked_lights();

		if (should_return())
			return;
	}

	if (gr && should_do()) {
		generate_random_ao(chunk->get_voxel_world()->get_current_seed());

		if (should_return())
			return;
	}

	if (bl && should_do()) {
		chunk->bake_lights();

		if (should_return())
			return;
	}

	reset_stages();
	next_phase();
}

void TerrainLightJob::_execute_phase() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<TerrainLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());

	phase_light();
	set_complete(true);
	next_job();
}

TerrainLightJob::TerrainLightJob() {
}

TerrainLightJob::~TerrainLightJob() {
}

void TerrainLightJob::_bind_methods() {
}
