/*************************************************************************/
/*  terrain_light_job.cpp                                                */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "terrain_light_job.h"

#include "../../defines.h"

#include "../../library/terrain_library.h"
#include "../../library/terrain_surface.h"

#include "../../meshers/terrain_mesher.h"
#include "../default/terrain_chunk_default.h"

void TerrainLightJob::phase_light() {
	Ref<TerrainChunkDefault> chunk = _chunk;

	if ((chunk->get_build_flags() & TerrainChunkDefault::BUILD_FLAG_GENERATE_AO) != 0) {
		if (!chunk->channel_get(TerrainChunkDefault::DEFAULT_CHANNEL_AO)) {
			generate_ao();
		}
	}

	bool gr = (chunk->get_build_flags() & TerrainChunkDefault::BUILD_FLAG_AUTO_GENERATE_RAO) != 0;

	if (!gr && (chunk->get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_LIGHTING) == 0) {
		next_phase();
		return;
	}

	bool bl = (chunk->get_build_flags() & TerrainChunkDefault::BUILD_FLAG_BAKE_LIGHTS) != 0;

	if (bl && should_do()) {
		chunk->clear_baked_lights();

		if (should_return()) {
			return;
		}
	}

	if (gr && should_do()) {
		generate_random_ao(chunk->get_voxel_world()->get_current_seed());

		if (should_return()) {
			return;
		}
	}

	if (bl && should_do()) {
		chunk->bake_lights();

		if (should_return()) {
			return;
		}
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
