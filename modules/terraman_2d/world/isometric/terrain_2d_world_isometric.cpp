

#include "terrain_2d_world_isometric.h"

#include "terrain_2d_chunk_isometric.h"

#include "../../meshers/isometric/terrain_2d_mesher_isometric.h"
#include "../jobs/terrain_2d_light_job.h"
#include "../jobs/terrain_2d_prop_job.h"
#include "../jobs/terrain_2d_terrain_job.h"

Ref<Terrain2DChunk> Terrain2DWorldIsometric::_create_chunk(int x, int y, Ref<Terrain2DChunk> chunk) {
	if (!chunk.is_valid()) {
		chunk = Ref<Terrain2DChunk>(memnew(Terrain2DChunkIsometric));
	}

	if (chunk->job_get_count() == 0) {
		Ref<Terrain2DTerrain2DJob> tj;
		tj.instance();

		Ref<Terrain2DLightJob> lj;
		lj.instance();

		tj->set_mesher(Ref<Terrain2DMesher>(memnew(Terrain2DMesherIsometric())));

		Ref<Terrain2DMesherIsometric> liquid_mesher;
		liquid_mesher.instance();
		liquid_mesher->set_channel_index_type(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIQUID_TYPE);
		tj->set_liquid_mesher(liquid_mesher);

		Ref<Terrain2DProp2DJob> pj;
		pj.instance();
		pj->set_prop_mesher(Ref<Terrain2DMesher>(memnew(Terrain2DMesherIsometric)));

		chunk->job_add(lj);
		chunk->job_add(tj);
		chunk->job_add(pj);
	}

	return Terrain2DWorld::_create_chunk(x, y, chunk);
}

Terrain2DWorldIsometric::Terrain2DWorldIsometric() {
	set_data_margin_start(1);
	set_data_margin_end(1);
}

Terrain2DWorldIsometric ::~Terrain2DWorldIsometric() {
}

void Terrain2DWorldIsometric::_bind_methods() {
}
