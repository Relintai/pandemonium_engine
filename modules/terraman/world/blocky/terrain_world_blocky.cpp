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

#include "terrain_world_blocky.h"

#include "terrain_chunk_blocky.h"

#include "../../meshers/blocky/terrain_mesher_blocky.h"
#include "../jobs/terrain_light_job.h"
#include "../jobs/terrain_prop_job.h"
#include "../jobs/terrain_terrain_job.h"

Ref<TerrainChunk> TerrainWorldBlocky::_create_chunk(int x, int z, Ref<TerrainChunk> chunk) {
	if (!chunk.is_valid()) {
		chunk = Ref<TerrainChunk>(memnew(TerrainChunkBlocky));
	}

	if (chunk->job_get_count() == 0) {
		Ref<TerrainTerrainJob> tj;
		tj.instance();

		Ref<TerrainLightJob> lj;
		lj.instance();

		Ref<TerrainMesherJobStep> s;
		s.instance();
		s->set_job_type(TerrainMesherJobStep::TYPE_NORMAL);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerrainMesherJobStep::TYPE_NORMAL_LOD);
		s->set_lod_index(1);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerrainMesherJobStep::TYPE_NORMAL_LOD);
		s->set_lod_index(2);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerrainMesherJobStep::TYPE_MERGE_VERTS);
		tj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerrainMesherJobStep::TYPE_BAKE_TEXTURE);
		tj->add_jobs_step(s);

		tj->set_mesher(Ref<TerrainMesher>(memnew(TerrainMesherBlocky())));

		Ref<TerrainMesherBlocky> liquid_mesher;
		liquid_mesher.instance();
		liquid_mesher->set_channel_index_type(TerrainChunkDefault::DEFAULT_CHANNEL_LIQUID_TYPE);
		liquid_mesher->set_channel_index_isolevel(TerrainChunkDefault::DEFAULT_CHANNEL_LIQUID_ISOLEVEL);
		tj->set_liquid_mesher(liquid_mesher);

		Ref<TerrainPropJob> pj;
		pj.instance();
		pj->set_prop_mesher(Ref<TerrainMesher>(memnew(TerrainMesherBlocky)));

		s.instance();
		s->set_job_type(TerrainMesherJobStep::TYPE_NORMAL);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerrainMesherJobStep::TYPE_MERGE_VERTS);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerrainMesherJobStep::TYPE_BAKE_TEXTURE);
		pj->add_jobs_step(s);

		s.instance();
		s->set_job_type(TerrainMesherJobStep::TYPE_SIMPLIFY_MESH);
#ifdef MESH_UTILS_PRESENT
		Ref<FastQuadraticMeshSimplifier> fqms;
		fqms.instance();
		s->set_fqms(fqms);
		s->set_simplification_steps(2);
#endif
		pj->add_jobs_step(s);

		chunk->job_add(lj);
		chunk->job_add(tj);
		chunk->job_add(pj);
	}

	return TerrainWorld::_create_chunk(x, z, chunk);
}

TerrainWorldBlocky::TerrainWorldBlocky() {
	set_data_margin_start(1);
	set_data_margin_end(1);
}

TerrainWorldBlocky ::~TerrainWorldBlocky() {
}

void TerrainWorldBlocky::_bind_methods() {
}
