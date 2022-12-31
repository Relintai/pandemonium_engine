/*
Copyright (c) 2019-2023 Péter Magyar

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

#include "terrain_2d_light_job.h"

#include "../../defines.h"

#include "../../library/terrain_2d_library.h"
#include "../../library/terrain_2d_surface.h"

#include "../../meshers/terrain_2d_mesher.h"
#include "../default/terrain_2d_chunk_default.h"

int Terrain2DLightJob::get_rao_octaves() {
	return _rao_octaves;
}
void Terrain2DLightJob::set_rao_octaves(const int val) {
	_rao_octaves = val;
}

int Terrain2DLightJob::get_rao_period() {
	return _rao_period;
}
void Terrain2DLightJob::set_rao_period(const int val) {
	_rao_period = val;
}

float Terrain2DLightJob::get_rao_persistence() {
	return _rao_persistence;
}
void Terrain2DLightJob::set_rao_persistence(const float val) {
	_rao_persistence = val;
}

float Terrain2DLightJob::get_rao_scale_factor() {
	return _rao_scale_factor;
}
void Terrain2DLightJob::set_rao_scale_factor(const float val) {
	_rao_scale_factor = val;
}

void Terrain2DLightJob::phase_light() {
	Ref<Terrain2DChunkDefault> chunk = _chunk;

	if ((chunk->get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
		if (!chunk->channel_get(Terrain2DChunkDefault::DEFAULT_CHANNEL_AO))
			generate_ao();

	bool gr = (chunk->get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_AUTO_GENERATE_RAO) != 0;

	if (!gr && (chunk->get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_LIGHTING) == 0) {
		next_phase();
		return;
	}

	bool bl = (chunk->get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_BAKE_LIGHTS) != 0;

	if (bl && should_do()) {
		chunk->clear_baked_lights();

		if (should_return())
			return;
	}

	if (gr && should_do()) {
		generate_random_ao(chunk->get_voxel_world()->get_current_seed(), _rao_octaves, _rao_period, _rao_persistence, _rao_scale_factor);

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

void Terrain2DLightJob::_execute_phase() {
	ERR_FAIL_COND(!_chunk.is_valid());

	Ref<Terrain2DLibrary> library = _chunk->get_library();

	ERR_FAIL_COND(!library.is_valid());

	phase_light();
	set_complete(true);
	next_job();
}

Terrain2DLightJob::Terrain2DLightJob() {
	_rao_octaves = 4;
	_rao_period = 30;
	_rao_persistence = 0.3;
	_rao_scale_factor = 0.6;
}

Terrain2DLightJob::~Terrain2DLightJob() {
}

void Terrain2DLightJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_rao_octaves"), &Terrain2DLightJob::get_rao_octaves);
	ClassDB::bind_method(D_METHOD("set_rao_octaves", "value"), &Terrain2DLightJob::set_rao_octaves);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rao_octaves"), "set_rao_octaves", "get_rao_octaves");

	ClassDB::bind_method(D_METHOD("get_rao_period"), &Terrain2DLightJob::get_rao_period);
	ClassDB::bind_method(D_METHOD("set_rao_period", "value"), &Terrain2DLightJob::set_rao_period);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rao_period"), "set_rao_period", "get_rao_period");

	ClassDB::bind_method(D_METHOD("get_rao_persistence"), &Terrain2DLightJob::get_rao_persistence);
	ClassDB::bind_method(D_METHOD("set_rao_persistence", "value"), &Terrain2DLightJob::set_rao_persistence);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rao_persistence"), "set_rao_persistence", "get_rao_persistence");

	ClassDB::bind_method(D_METHOD("get_rao_scale_factor"), &Terrain2DLightJob::get_rao_scale_factor);
	ClassDB::bind_method(D_METHOD("set_rao_scale_factor", "value"), &Terrain2DLightJob::set_rao_scale_factor);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rao_scale_factor"), "set_rao_scale_factor", "get_rao_scale_factor");
}
