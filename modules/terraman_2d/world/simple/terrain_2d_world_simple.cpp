

#include "terrain_2d_world_simple.h"

#include "terrain_2d_chunk_simple.h"

#include "../../meshers/simple/terrain_2d_mesher_simple.h"
#include "../jobs/terrain_2d_light_job.h"
#include "../jobs/terrain_2d_prop_job.h"
#include "../jobs/terrain_2d_terrain_job.h"

Ref<Terrain2DChunk> Terrain2DWorldSimple::_create_chunk(int x, int y, Ref<Terrain2DChunk> chunk) {
	if (!chunk.is_valid()) {
		chunk = Ref<Terrain2DChunk>(memnew(Terrain2DChunkSimple));
	}

	if (chunk->job_get_count() == 0) {
		Ref<Terrain2DTerrain2DJob> tj;
		tj.instance();

		Ref<Terrain2DLightJob> lj;
		lj.instance();

		tj->set_mesher(Ref<Terrain2DMesher>(memnew(Terrain2DMesherSimple())));

		Ref<Terrain2DMesherSimple> liquid_mesher;
		liquid_mesher.instance();
		liquid_mesher->set_channel_index_type(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIQUID_TYPE);
		tj->set_liquid_mesher(liquid_mesher);

		Ref<Terrain2DProp2DJob> pj;
		pj.instance();
		pj->set_prop_mesher(Ref<Terrain2DMesher>(memnew(Terrain2DMesherSimple)));

		chunk->job_add(lj);
		chunk->job_add(tj);
		chunk->job_add(pj);
	}

	return Terrain2DWorld::_create_chunk(x, y, chunk);
}

Terrain2DWorldSimple::Terrain2DWorldSimple() {
	set_data_margin_start(1);
	set_data_margin_end(1);
}

Terrain2DWorldSimple ::~Terrain2DWorldSimple() {
}

void Terrain2DWorldSimple::_bind_methods() {
}
