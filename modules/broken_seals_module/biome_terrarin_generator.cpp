/*
Copyright (c) 2020-2022 Péter Magyar

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

#include "biome_terrarin_generator.h"

#include "core/version.h"

#include "core/engine.h"

#include "core/math/math_funcs.h"

#ifdef VOXELMAN_PRESENT
#include "../voxelman/world/default/voxel_chunk_default.h"
#include "../voxelman/world/voxel_chunk.h"
#endif

#include "../entity_spell_system/singletons/ess.h"
#include "../entity_spell_system/spawners/ess_entity_spawner.h"
#include "../opensimplex/open_simplex_noise.h"

int BiomeTerrarinGenerator::get_current_seed() {
	return _current_seed;
}
void BiomeTerrarinGenerator::set_current_seed(int value) {
	_current_seed = value;
}

#ifdef VOXELMAN_PRESENT
void BiomeTerrarinGenerator::generate_simple_terrarin(Ref<VoxelChunk> chunk, bool spawn_mobs) {
	Ref<OpenSimplexNoise> noise;
	noise.instance();
	noise->set_seed(10 * get_current_seed());
	noise->set_octaves(4);
	noise->set_period(280.0);
	noise->set_persistence(0.8);

	Ref<OpenSimplexNoise> terr_noise;
	terr_noise.instance();
	terr_noise->set_seed(10 * 321 + 112 * get_current_seed());
	terr_noise->set_octaves(4);
	terr_noise->set_period(90.0);
	terr_noise->set_persistence(0.9);

	Ref<OpenSimplexNoise> det_noise;
	det_noise.instance();
	det_noise->set_seed(10 * 3231 + 112 * get_current_seed());
	det_noise->set_octaves(6);
	det_noise->set_period(80.0);
	det_noise->set_persistence(0.3);

	for (int x = -chunk->get_margin_start(); x < chunk->get_size_x() + chunk->get_margin_end(); ++x) {
		for (int z = -chunk->get_margin_start(); z < chunk->get_size_z() + chunk->get_margin_end(); ++z) {
			float val = noise->get_noise_2d(x + (chunk->get_position_x() * chunk->get_size_x()), z + (chunk->get_position_z() * chunk->get_size_z()));
			val *= val;
			val *= 200.0;
			val += 2.0;

			float tv = terr_noise->get_noise_2d(x + (chunk->get_position_x() * chunk->get_size_x()), z + (chunk->get_position_z() * chunk->get_size_z()));
			tv *= tv * tv * tv;
			val += tv * 2.0;

			float dval = noise->get_noise_2d(x + (chunk->get_position_x() * chunk->get_size_x()), z + (chunk->get_position_z() * chunk->get_size_z()));

			val += dval * 6.0;

			int v = (int(val));

			v -= chunk->get_position_y() * (chunk->get_size_y());

			if (v > chunk->get_size_y() + chunk->get_margin_end())
				v = chunk->get_size_y() + chunk->get_margin_end();

			for (int y = -chunk->get_margin_start(); y < v; y++) {
				Math::seed(x + (chunk->get_position_x() * chunk->get_size_x()) + z + (chunk->get_position_z() * chunk->get_size_z()) + y + (chunk->get_position_y() * chunk->get_size_y()));

				if (v < 2)
					chunk->set_voxel(1, x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_TYPE);
				else if (v == 2)
					chunk->set_voxel(3, x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_TYPE);
				else
					chunk->set_voxel(2, x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_TYPE);

				float val2 = (val - static_cast<int>(val)) * 4.0;
				val2 = static_cast<int>(val2);
				val2 /= 4.0;

				chunk->set_voxel(static_cast<int>(255.0 * val2), x, y, z, VoxelChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);
			}
		}
	}

	//if (!Engine::get_singleton()->is_editor_hint() && chunk->get_position_y() == 0 && spawn_mobs) {
	//		Vector3 v = Vector3(chunk->get_position_x() * chunk->get_size_x() * chunk->get_voxel_scale() + chunk->get_size_x() / 2,
	//							(chunk->get_position_y() + 1) * chunk->get_size_y() * chunk->get_voxel_scale(),
	//							chunk->get_position_z()) *
	//					(chunk->get_size_z() * chunk->get_voxel_scale() + chunk->get_size_z() / 2);
	//
	//		ESS::get_singleton()->get_entity_spawner()->call("spawn_mob", 0, Math::rand() % 3, v);
	//	}
}
#endif

BiomeTerrarinGenerator::BiomeTerrarinGenerator() {
	_current_seed = 0;
}

BiomeTerrarinGenerator::~BiomeTerrarinGenerator() {
}

void BiomeTerrarinGenerator::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_current_seed"), &BiomeTerrarinGenerator::get_current_seed);
	ClassDB::bind_method(D_METHOD("set_current_seed", "value"), &BiomeTerrarinGenerator::set_current_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "current_seed"), "set_current_seed", "get_current_seed");

#ifdef VOXELMAN_PRESENT
	ClassDB::bind_method(D_METHOD("generate_simple_terrarin", "chunk", "spawn_mobs"), &BiomeTerrarinGenerator::generate_simple_terrarin);
#endif
}
