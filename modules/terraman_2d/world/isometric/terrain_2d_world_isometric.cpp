/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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