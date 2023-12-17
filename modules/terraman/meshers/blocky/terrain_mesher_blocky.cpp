

#include "terrain_mesher_blocky.h"

#include "core/math/math_funcs.h"

#include "../../library/terrain_material_cache.h"

bool TerrainMesherBlocky::get_always_add_colors() const {
	return _always_add_colors;
}
void TerrainMesherBlocky::set_always_add_colors(const bool value) {
	_always_add_colors = value;
}

void TerrainMesherBlocky::_add_chunk(Ref<TerrainChunk> p_chunk) {
	Ref<TerrainChunkDefault> chunk = p_chunk;

	ERR_FAIL_COND(!chunk.is_valid());
	ERR_FAIL_COND(chunk->get_margin_end() < 1);
	ERR_FAIL_COND(chunk->get_margin_start() < 1);

	if (_lod_index == 0) {
		add_chunk_normal(chunk);
	} else {
		//todo give error message if the chunk is badly sized for the given lod index?

		add_chunk_lod(chunk);
	}
}

void TerrainMesherBlocky::add_chunk_normal(Ref<TerrainChunkDefault> chunk) {
	//if ((get_build_flags() & TerrainChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
	//	if (!chunk->get_channel(TerrainChunkDefault::DEFAULT_CHANNEL_AO))
	//		chunk->generate_ao();

	int x_size = chunk->get_size_x();
	int z_size = chunk->get_size_z();
	float world_height = chunk->get_world_height();

	float voxel_scale = get_voxel_scale();

	uint8_t *channel_type = chunk->channel_get(_channel_index_type);

	if (!channel_type)
		return;

	uint8_t *channel_isolevel = chunk->channel_get(_channel_index_isolevel);

	if (!channel_isolevel)
		return;

	uint8_t *channel_color_r = NULL;
	uint8_t *channel_color_g = NULL;
	uint8_t *channel_color_b = NULL;
	uint8_t *channel_ao = NULL;
	uint8_t *channel_rao = NULL;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light[4]{ Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };

	bool use_lighting = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	if (use_lighting) {
		channel_color_r = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
		channel_color_g = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
		channel_color_b = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

		if (use_ao)
			channel_ao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_AO);

		if (use_rao)
			channel_rao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
	}

	Ref<TerrainMaterialCache> mcache;

	if (!get_is_liquid_mesher()) {
		if (chunk->material_cache_key_has()) {
			mcache = _library->material_cache_get(chunk->material_cache_key_get());
		}
	} else {
		if (chunk->liquid_material_cache_key_has()) {
			mcache = _library->liquid_material_cache_get(chunk->liquid_material_cache_key_get());
		}
	}

	int margin_start = chunk->get_margin_start();
	//z_size + margin_start is fine, x, and z are in data space.
	for (int z = margin_start; z < z_size + margin_start; ++z) {
		for (int x = margin_start; x < x_size + margin_start; ++x) {
			int indexes[4] = {
				chunk->get_data_index(x + 1, z),
				chunk->get_data_index(x, z),
				chunk->get_data_index(x, z + 1),
				chunk->get_data_index(x + 1, z + 1)
			};

			uint8_t type = channel_type[indexes[0]];

			if (type == 0)
				continue;

			Ref<TerrainSurface> surface;

			if (!mcache.is_valid()) {
				surface = _library->terra_surface_get(type - 1);
			} else {
				surface = mcache->surface_id_get(type - 1);
			}

			if (!surface.is_valid())
				continue;

			uint8_t isolevels[] = {
				channel_isolevel[indexes[0]],
				channel_isolevel[indexes[1]],
				channel_isolevel[indexes[2]],
				channel_isolevel[indexes[3]],
			};

			if (use_lighting) {
				for (int i = 0; i < 4; ++i) {
					int indx = indexes[i];

					light[i] = Color(channel_color_r[indx] / 255.0,
							channel_color_g[indx] / 255.0,
							channel_color_b[indx] / 255.0);

					float ao = 0;

					if (use_ao)
						ao = channel_ao[indx] / 255.0;

					if (use_rao) {
						float rao = channel_rao[indx] / 255.0;
						ao += rao;
					}

					light[i] += base_light;

					if (ao > 0)
						light[i] -= Color(ao, ao, ao) * _ao_strength;

					light[i].r = CLAMP(light[i].r, 0, 1.0);
					light[i].g = CLAMP(light[i].g, 0, 1.0);
					light[i].b = CLAMP(light[i].b, 0, 1.0);
				}
			}

			int vc = get_vertex_count();
			add_indices(vc + 2);
			add_indices(vc + 1);
			add_indices(vc + 0);
			add_indices(vc + 3);
			add_indices(vc + 2);
			add_indices(vc + 0);

			Vector2 uvs[] = {
				surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale())
			};

			Vector3 verts[] = {
				Vector3(x + 1, isolevels[0] / 255.0 * world_height, z) * voxel_scale,
				Vector3(x, isolevels[1] / 255.0 * world_height, z) * voxel_scale,
				Vector3(x, isolevels[2] / 255.0 * world_height, z + 1) * voxel_scale,
				Vector3(x + 1, isolevels[3] / 255.0 * world_height, z + 1) * voxel_scale
			};

			Vector3 normals[] = {
				(verts[0] - verts[1]).cross(verts[0] - verts[2]).normalized(),
				(verts[0] - verts[1]).cross(verts[1] - verts[2]).normalized(),
				(verts[1] - verts[2]).cross(verts[2] - verts[0]).normalized(),
				(verts[2] - verts[3]).cross(verts[3] - verts[0]).normalized(),
			};

			for (int i = 0; i < 4; ++i) {
				add_normal(normals[i]);

				if (use_lighting || _always_add_colors)
					add_color(light[i]);

				add_uv(uvs[i]);
				add_vertex(verts[i]);
			}
		}
	}
}

void TerrainMesherBlocky::add_chunk_lod(Ref<TerrainChunkDefault> chunk) {
	//if ((get_build_flags() & TerrainChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
	//	if (!chunk->get_channel(TerrainChunkDefault::DEFAULT_CHANNEL_AO))
	//		chunk->generate_ao();

	create_margin_zmin(chunk);
	create_margin_zmax(chunk);
	create_margin_xmin(chunk);
	create_margin_xmax(chunk);
	create_margin_corners(chunk);

	int x_size = chunk->get_size_x();
	int z_size = chunk->get_size_z();
	float world_height = chunk->get_world_height();

	float voxel_scale = get_voxel_scale();

	uint8_t *channel_type = chunk->channel_get(_channel_index_type);

	if (!channel_type)
		return;

	uint8_t *channel_isolevel = chunk->channel_get(_channel_index_isolevel);

	if (!channel_isolevel)
		return;

	uint8_t *channel_color_r = NULL;
	uint8_t *channel_color_g = NULL;
	uint8_t *channel_color_b = NULL;
	uint8_t *channel_ao = NULL;
	uint8_t *channel_rao = NULL;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light[4]{ Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };

	bool use_lighting = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	if (use_lighting) {
		channel_color_r = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
		channel_color_g = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
		channel_color_b = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

		if (use_ao)
			channel_ao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_AO);

		if (use_rao)
			channel_rao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
	}

	Ref<TerrainMaterialCache> mcache;

	if (!get_is_liquid_mesher()) {
		if (chunk->material_cache_key_has()) {
			mcache = _library->material_cache_get(chunk->material_cache_key_get());
		}
	} else {
		if (chunk->liquid_material_cache_key_has()) {
			mcache = _library->liquid_material_cache_get(chunk->liquid_material_cache_key_get());
		}
	}

	//todo this should be calculated from size's factors
	int lod_skip = _lod_index * 2;
	//int margin_start = chunk->get_margin_start() ;

	int margin_start = chunk->get_margin_start();
	//z_size + margin_start is fine, x, and z are in data space.
	for (int z = lod_skip; z < z_size + margin_start - lod_skip; z += lod_skip) {
		for (int x = lod_skip; x < x_size + margin_start - lod_skip; x += lod_skip) {
			int indexes[4] = {
				chunk->get_data_index(x + lod_skip, z),
				chunk->get_data_index(x, z),
				chunk->get_data_index(x, z + lod_skip),
				chunk->get_data_index(x + lod_skip, z + lod_skip)
			};

			uint8_t type = channel_type[indexes[0]];

			if (type == 0)
				continue;

			Ref<TerrainSurface> surface;

			if (!mcache.is_valid()) {
				surface = _library->terra_surface_get(type - 1);
			} else {
				surface = mcache->surface_id_get(type - 1);
			}

			if (!surface.is_valid())
				continue;

			uint8_t isolevels[] = {
				channel_isolevel[indexes[0]],
				channel_isolevel[indexes[1]],
				channel_isolevel[indexes[2]],
				channel_isolevel[indexes[3]],
			};

			if (use_lighting) {
				for (int i = 0; i < 4; ++i) {
					int indx = indexes[i];

					light[i] = Color(channel_color_r[indx] / 255.0,
							channel_color_g[indx] / 255.0,
							channel_color_b[indx] / 255.0);

					float ao = 0;

					if (use_ao)
						ao = channel_ao[indx] / 255.0;

					if (use_rao) {
						float rao = channel_rao[indx] / 255.0;
						ao += rao;
					}

					light[i] += base_light;

					if (ao > 0)
						light[i] -= Color(ao, ao, ao) * _ao_strength;

					light[i].r = CLAMP(light[i].r, 0, 1.0);
					light[i].g = CLAMP(light[i].g, 0, 1.0);
					light[i].b = CLAMP(light[i].b, 0, 1.0);
				}
			}

			int vc = get_vertex_count();
			add_indices(vc + 2);
			add_indices(vc + 1);
			add_indices(vc + 0);
			add_indices(vc + 3);
			add_indices(vc + 2);
			add_indices(vc + 0);

			Vector2 uvs[] = {
				surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
				surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale())
			};

			Vector3 verts[] = {
				Vector3(x + lod_skip, isolevels[0] / 255.0 * world_height, z) * voxel_scale,
				Vector3(x, isolevels[1] / 255.0 * world_height, z) * voxel_scale,
				Vector3(x, isolevels[2] / 255.0 * world_height, z + lod_skip) * voxel_scale,
				Vector3(x + lod_skip, isolevels[3] / 255.0 * world_height, z + lod_skip) * voxel_scale
			};

			Vector3 normals[] = {
				(verts[0] - verts[1]).cross(verts[0] - verts[2]).normalized(),
				(verts[0] - verts[1]).cross(verts[1] - verts[2]).normalized(),
				(verts[1] - verts[2]).cross(verts[2] - verts[0]).normalized(),
				(verts[2] - verts[3]).cross(verts[3] - verts[0]).normalized(),
			};

			for (int i = 0; i < 4; ++i) {
				add_normal(normals[i]);

				if (use_lighting || _always_add_colors)
					add_color(light[i]);

				add_uv(uvs[i]);
				add_vertex(verts[i]);
			}
		}
	}
}

void TerrainMesherBlocky::create_margin_zmin(Ref<TerrainChunkDefault> chunk) {
	//if ((get_build_flags() & TerrainChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
	//	if (!chunk->get_channel(TerrainChunkDefault::DEFAULT_CHANNEL_AO))
	//		chunk->generate_ao();

	int x_size = chunk->get_size_x();
	int x_data_size = chunk->get_data_size_x();
	float world_height = chunk->get_world_height();

	float voxel_scale = get_voxel_scale();

	uint8_t *channel_type = chunk->channel_get(_channel_index_type);

	if (!channel_type)
		return;

	uint8_t *channel_isolevel = chunk->channel_get(_channel_index_isolevel);

	if (!channel_isolevel)
		return;

	uint8_t *channel_color_r = NULL;
	uint8_t *channel_color_g = NULL;
	uint8_t *channel_color_b = NULL;
	uint8_t *channel_ao = NULL;
	uint8_t *channel_rao = NULL;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light[4]{ Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };

	bool use_lighting = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	if (use_lighting) {
		channel_color_r = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
		channel_color_g = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
		channel_color_b = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

		if (use_ao)
			channel_ao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_AO);

		if (use_rao)
			channel_rao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
	}

	Ref<TerrainMaterialCache> mcache;

	if (!get_is_liquid_mesher()) {
		if (chunk->material_cache_key_has()) {
			mcache = _library->material_cache_get(chunk->material_cache_key_get());
		}
	} else {
		if (chunk->liquid_material_cache_key_has()) {
			mcache = _library->liquid_material_cache_get(chunk->liquid_material_cache_key_get());
		}
	}

	int margin_start = chunk->get_margin_start();

	int lod_skip = _lod_index * 2;
	//z_size + margin_start is fine, x, and z are in data space.
	int z = 1;
	float oolod = 1.0 / static_cast<float>(lod_skip);

	int lastz = lod_skip;

	for (int x = margin_start + 1; x < x_size + margin_start - 1; ++x) {
		int prev_main_x = x - (x % lod_skip);
		int next_main_x = prev_main_x + lod_skip;
		next_main_x = CLAMP(next_main_x, 0, x_data_size);

		int indexes[4] = {
			chunk->get_data_index(x + 1, z), //x + 1
			chunk->get_data_index(x, z), //x
			//chunk->get_data_index(x + 1, z), //x + 1
			//chunk->get_data_index(x, z), //x
			chunk->get_data_index(prev_main_x, lastz),
			chunk->get_data_index(next_main_x, lastz),
		};

		uint8_t type = channel_type[indexes[0]];

		if (type == 0)
			continue;

		Ref<TerrainSurface> surface;

		if (!mcache.is_valid()) {
			surface = _library->terra_surface_get(type - 1);
		} else {
			surface = mcache->surface_id_get(type - 1);
		}

		if (!surface.is_valid())
			continue;

		uint8_t isolevels[] = {
			channel_isolevel[indexes[0]],
			channel_isolevel[indexes[1]],
			channel_isolevel[indexes[2]],
			channel_isolevel[indexes[3]],
		};

		float x_interp = oolod * (x - prev_main_x);
		float xp1_interp = 1.0 - (oolod * (next_main_x - (x + 1)));

		if (use_lighting) {
			for (int i = 0; i < 4; ++i) {
				int indx = indexes[i];

				light[i] = Color(channel_color_r[indx] / 255.0,
						channel_color_g[indx] / 255.0,
						channel_color_b[indx] / 255.0);

				float ao = 0;

				if (use_ao)
					ao = channel_ao[indx] / 255.0;

				if (use_rao) {
					float rao = channel_rao[indx] / 255.0;
					ao += rao;
				}

				light[i] += base_light;

				if (ao > 0)
					light[i] -= Color(ao, ao, ao) * _ao_strength;

				light[i].r = CLAMP(light[i].r, 0, 1.0);
				light[i].g = CLAMP(light[i].g, 0, 1.0);
				light[i].b = CLAMP(light[i].b, 0, 1.0);
			}
		}

		int vc = get_vertex_count();
		add_indices(vc + 2);
		add_indices(vc + 1);
		add_indices(vc + 0);
		add_indices(vc + 3);
		add_indices(vc + 2);
		add_indices(vc + 0);

		Vector2 uvs[] = {
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale())
		};

		float vi0 = Math::lerp(isolevels[2], isolevels[3], x_interp);
		float vi1 = Math::lerp(isolevels[2], isolevels[3], xp1_interp);

		Vector3 verts[] = {
			Vector3(x + 1, isolevels[0] / 255.0 * world_height, z) * voxel_scale,
			Vector3(x, isolevels[1] / 255.0 * world_height, z) * voxel_scale,
			Vector3(x, vi0 / 255.0 * world_height, lastz) * voxel_scale,
			Vector3(x + 1, vi1 / 255.0 * world_height, lastz) * voxel_scale
		};

		Vector3 normals[] = {
			(verts[0] - verts[1]).cross(verts[0] - verts[2]).normalized(),
			(verts[0] - verts[1]).cross(verts[1] - verts[2]).normalized(),
			(verts[1] - verts[2]).cross(verts[2] - verts[0]).normalized(),
			(verts[2] - verts[3]).cross(verts[3] - verts[0]).normalized(),
		};

		for (int i = 0; i < 4; ++i) {
			add_normal(normals[i]);

			if (use_lighting || _always_add_colors)
				add_color(light[i]);

			add_uv(uvs[i]);
			add_vertex(verts[i]);
		}
	}
}

void TerrainMesherBlocky::create_margin_zmax(Ref<TerrainChunkDefault> chunk) {
	//if ((get_build_flags() & TerrainChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
	//	if (!chunk->get_channel(TerrainChunkDefault::DEFAULT_CHANNEL_AO))
	//		chunk->generate_ao();

	int x_size = chunk->get_size_x();
	int x_data_size = chunk->get_data_size_x();
	float world_height = chunk->get_world_height();

	float voxel_scale = get_voxel_scale();

	uint8_t *channel_type = chunk->channel_get(_channel_index_type);

	if (!channel_type)
		return;

	uint8_t *channel_isolevel = chunk->channel_get(_channel_index_isolevel);

	if (!channel_isolevel)
		return;

	uint8_t *channel_color_r = NULL;
	uint8_t *channel_color_g = NULL;
	uint8_t *channel_color_b = NULL;
	uint8_t *channel_ao = NULL;
	uint8_t *channel_rao = NULL;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light[4]{ Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };

	bool use_lighting = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	if (use_lighting) {
		channel_color_r = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
		channel_color_g = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
		channel_color_b = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

		if (use_ao)
			channel_ao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_AO);

		if (use_rao)
			channel_rao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
	}

	Ref<TerrainMaterialCache> mcache;

	if (!get_is_liquid_mesher()) {
		if (chunk->material_cache_key_has()) {
			mcache = _library->material_cache_get(chunk->material_cache_key_get());
		}
	} else {
		if (chunk->liquid_material_cache_key_has()) {
			mcache = _library->liquid_material_cache_get(chunk->liquid_material_cache_key_get());
		}
	}

	int margin_start = chunk->get_margin_start();

	int lod_skip = _lod_index * 2;
	//z_size + margin_start is fine, x, and z are in data space.
	int z = chunk->get_size_z();
	float oolod = 1.0 / static_cast<float>(lod_skip);

	for (int x = margin_start + 1; x < x_size + margin_start - 1; ++x) {
		int prev_main_x = x - (x % lod_skip);
		int next_main_x = prev_main_x + lod_skip;
		next_main_x = CLAMP(next_main_x, 0, x_data_size);

		int indexes[4] = {
			chunk->get_data_index(next_main_x, z), //x + 1
			chunk->get_data_index(prev_main_x, z), //x
			//chunk->get_data_index(x + 1, z), //x + 1
			//chunk->get_data_index(x, z), //x
			chunk->get_data_index(x, z + 1),
			chunk->get_data_index(x + 1, z + 1),
		};

		uint8_t type = channel_type[indexes[0]];

		if (type == 0)
			continue;

		Ref<TerrainSurface> surface;

		if (!mcache.is_valid()) {
			surface = _library->terra_surface_get(type - 1);
		} else {
			surface = mcache->surface_id_get(type - 1);
		}

		if (!surface.is_valid())
			continue;

		uint8_t isolevels[] = {
			channel_isolevel[indexes[0]],
			channel_isolevel[indexes[1]],
			channel_isolevel[indexes[2]],
			channel_isolevel[indexes[3]],
		};

		float x_interp = oolod * (x - prev_main_x);
		float xp1_interp = 1.0 - (oolod * (next_main_x - (x + 1)));

		if (use_lighting) {
			for (int i = 0; i < 4; ++i) {
				int indx = indexes[i];

				light[i] = Color(channel_color_r[indx] / 255.0,
						channel_color_g[indx] / 255.0,
						channel_color_b[indx] / 255.0);

				float ao = 0;

				if (use_ao)
					ao = channel_ao[indx] / 255.0;

				if (use_rao) {
					float rao = channel_rao[indx] / 255.0;
					ao += rao;
				}

				light[i] += base_light;

				if (ao > 0)
					light[i] -= Color(ao, ao, ao) * _ao_strength;

				light[i].r = CLAMP(light[i].r, 0, 1.0);
				light[i].g = CLAMP(light[i].g, 0, 1.0);
				light[i].b = CLAMP(light[i].b, 0, 1.0);
			}
		}

		int vc = get_vertex_count();
		add_indices(vc + 2);
		add_indices(vc + 1);
		add_indices(vc + 0);
		add_indices(vc + 3);
		add_indices(vc + 2);
		add_indices(vc + 0);

		Vector2 uvs[] = {
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale())
		};

		float vi0 = Math::lerp(isolevels[1], isolevels[0], x_interp);
		float vi1 = Math::lerp(isolevels[1], isolevels[0], xp1_interp);

		Vector3 verts[] = {
			Vector3(x + 1, vi1 / 255.0 * world_height, z) * voxel_scale,
			Vector3(x, vi0 / 255.0 * world_height, z) * voxel_scale,
			Vector3(x, isolevels[2] / 255.0 * world_height, z + 1) * voxel_scale,
			Vector3(x + 1, isolevels[3] / 255.0 * world_height, z + 1) * voxel_scale
		};

		Vector3 normals[] = {
			(verts[0] - verts[1]).cross(verts[0] - verts[2]).normalized(),
			(verts[0] - verts[1]).cross(verts[1] - verts[2]).normalized(),
			(verts[1] - verts[2]).cross(verts[2] - verts[0]).normalized(),
			(verts[2] - verts[3]).cross(verts[3] - verts[0]).normalized(),
		};

		for (int i = 0; i < 4; ++i) {
			add_normal(normals[i]);

			if (use_lighting || _always_add_colors)
				add_color(light[i]);

			add_uv(uvs[i]);
			add_vertex(verts[i]);
		}
	}
}

void TerrainMesherBlocky::create_margin_xmin(Ref<TerrainChunkDefault> chunk) {
	//if ((get_build_flags() & TerrainChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
	//	if (!chunk->get_channel(TerrainChunkDefault::DEFAULT_CHANNEL_AO))
	//		chunk->generate_ao();

	int z_size = chunk->get_size_x();
	int z_data_size = chunk->get_data_size_x();
	float world_height = chunk->get_world_height();

	float voxel_scale = get_voxel_scale();

	uint8_t *channel_type = chunk->channel_get(_channel_index_type);

	if (!channel_type)
		return;

	uint8_t *channel_isolevel = chunk->channel_get(_channel_index_isolevel);

	if (!channel_isolevel)
		return;

	uint8_t *channel_color_r = NULL;
	uint8_t *channel_color_g = NULL;
	uint8_t *channel_color_b = NULL;
	uint8_t *channel_ao = NULL;
	uint8_t *channel_rao = NULL;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light[4]{ Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };

	bool use_lighting = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	if (use_lighting) {
		channel_color_r = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
		channel_color_g = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
		channel_color_b = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

		if (use_ao)
			channel_ao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_AO);

		if (use_rao)
			channel_rao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
	}

	Ref<TerrainMaterialCache> mcache;

	if (!get_is_liquid_mesher()) {
		if (chunk->material_cache_key_has()) {
			mcache = _library->material_cache_get(chunk->material_cache_key_get());
		}
	} else {
		if (chunk->liquid_material_cache_key_has()) {
			mcache = _library->liquid_material_cache_get(chunk->liquid_material_cache_key_get());
		}
	}

	int margin_start = chunk->get_margin_start();

	int lod_skip = _lod_index * 2;
	//z_size + margin_start is fine, x, and z are in data space.
	int x = 1;
	float oolod = 1.0 / static_cast<float>(lod_skip);

	int lastx = lod_skip;

	for (int z = margin_start + 1; z < z_size + margin_start - 1; ++z) {
		int prev_main_z = z - (z % lod_skip);
		int next_main_z = prev_main_z + lod_skip;
		next_main_z = CLAMP(next_main_z, 0, z_data_size);

		int indexes[4] = {
			chunk->get_data_index(lastx, prev_main_z), //x + 1
			chunk->get_data_index(x, z), //x
			chunk->get_data_index(x, z + 1),
			chunk->get_data_index(lastx, next_main_z),
		};

		uint8_t type = channel_type[indexes[0]];

		if (type == 0)
			continue;

		Ref<TerrainSurface> surface;

		if (!mcache.is_valid()) {
			surface = _library->terra_surface_get(type - 1);
		} else {
			surface = mcache->surface_id_get(type - 1);
		}

		if (!surface.is_valid())
			continue;

		uint8_t isolevels[] = {
			channel_isolevel[indexes[0]],
			channel_isolevel[indexes[1]],
			channel_isolevel[indexes[2]],
			channel_isolevel[indexes[3]],
		};

		float z_interp = oolod * (z - prev_main_z);
		float zp1_interp = 1.0 - (oolod * (next_main_z - (z + 1)));

		if (use_lighting) {
			for (int i = 0; i < 4; ++i) {
				int indx = indexes[i];

				light[i] = Color(channel_color_r[indx] / 255.0,
						channel_color_g[indx] / 255.0,
						channel_color_b[indx] / 255.0);

				float ao = 0;

				if (use_ao)
					ao = channel_ao[indx] / 255.0;

				if (use_rao) {
					float rao = channel_rao[indx] / 255.0;
					ao += rao;
				}

				light[i] += base_light;

				if (ao > 0)
					light[i] -= Color(ao, ao, ao) * _ao_strength;

				light[i].r = CLAMP(light[i].r, 0, 1.0);
				light[i].g = CLAMP(light[i].g, 0, 1.0);
				light[i].b = CLAMP(light[i].b, 0, 1.0);
			}
		}

		int vc = get_vertex_count();
		add_indices(vc + 2);
		add_indices(vc + 1);
		add_indices(vc + 0);
		add_indices(vc + 3);
		add_indices(vc + 2);
		add_indices(vc + 0);

		Vector2 uvs[] = {
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale())
		};

		float vi0 = Math::lerp(isolevels[0], isolevels[3], z_interp);
		float vi1 = Math::lerp(isolevels[0], isolevels[3], zp1_interp);

		Vector3 verts[] = {
			Vector3(lastx, vi0 / 255.0 * world_height, z) * voxel_scale,
			Vector3(x, isolevels[1] / 255.0 * world_height, z) * voxel_scale,
			Vector3(x, isolevels[2] / 255.0 * world_height, z + 1) * voxel_scale,
			Vector3(lastx, vi1 / 255.0 * world_height, z + 1) * voxel_scale
		};

		Vector3 normals[] = {
			(verts[0] - verts[1]).cross(verts[0] - verts[2]).normalized(),
			(verts[0] - verts[1]).cross(verts[1] - verts[2]).normalized(),
			(verts[1] - verts[2]).cross(verts[2] - verts[0]).normalized(),
			(verts[2] - verts[3]).cross(verts[3] - verts[0]).normalized(),
		};

		for (int i = 0; i < 4; ++i) {
			add_normal(normals[i]);

			if (use_lighting || _always_add_colors)
				add_color(light[i]);

			add_uv(uvs[i]);
			add_vertex(verts[i]);
		}
	}
}

void TerrainMesherBlocky::create_margin_xmax(Ref<TerrainChunkDefault> chunk) {
	//if ((get_build_flags() & TerrainChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
	//	if (!chunk->get_channel(TerrainChunkDefault::DEFAULT_CHANNEL_AO))
	//		chunk->generate_ao();

	int z_size = chunk->get_size_x();
	int z_data_size = chunk->get_data_size_x();
	float world_height = chunk->get_world_height();

	float voxel_scale = get_voxel_scale();

	uint8_t *channel_type = chunk->channel_get(_channel_index_type);

	if (!channel_type)
		return;

	uint8_t *channel_isolevel = chunk->channel_get(_channel_index_isolevel);

	if (!channel_isolevel)
		return;

	uint8_t *channel_color_r = NULL;
	uint8_t *channel_color_g = NULL;
	uint8_t *channel_color_b = NULL;
	uint8_t *channel_ao = NULL;
	uint8_t *channel_rao = NULL;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light[4]{ Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };

	bool use_lighting = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	if (use_lighting) {
		channel_color_r = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
		channel_color_g = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
		channel_color_b = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

		if (use_ao)
			channel_ao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_AO);

		if (use_rao)
			channel_rao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
	}

	Ref<TerrainMaterialCache> mcache;

	if (!get_is_liquid_mesher()) {
		if (chunk->material_cache_key_has()) {
			mcache = _library->material_cache_get(chunk->material_cache_key_get());
		}
	} else {
		if (chunk->liquid_material_cache_key_has()) {
			mcache = _library->liquid_material_cache_get(chunk->liquid_material_cache_key_get());
		}
	}

	int margin_start = chunk->get_margin_start();

	int lod_skip = _lod_index * 2;
	//z_size + margin_start is fine, x, and z are in data space.
	int x = chunk->get_size_x();
	float oolod = 1.0 / static_cast<float>(lod_skip);

	for (int z = margin_start + 1; z < z_size + margin_start - 1; ++z) {
		int prev_main_z = z - (z % lod_skip);
		int next_main_z = prev_main_z + lod_skip;
		next_main_z = CLAMP(next_main_z, 0, z_data_size);

		int indexes[4] = {
			chunk->get_data_index(x + 1, z), //x + 1
			chunk->get_data_index(x, prev_main_z), //x
			chunk->get_data_index(x, next_main_z),
			chunk->get_data_index(x + 1, z + 1),
		};

		uint8_t type = channel_type[indexes[0]];

		if (type == 0)
			continue;

		Ref<TerrainSurface> surface;

		if (!mcache.is_valid()) {
			surface = _library->terra_surface_get(type - 1);
		} else {
			surface = mcache->surface_id_get(type - 1);
		}

		if (!surface.is_valid())
			continue;

		uint8_t isolevels[] = {
			channel_isolevel[indexes[0]],
			channel_isolevel[indexes[1]],
			channel_isolevel[indexes[2]],
			channel_isolevel[indexes[3]],
		};

		float z_interp = oolod * (z - prev_main_z);
		float zp1_interp = 1.0 - (oolod * (next_main_z - (z + 1)));

		if (use_lighting) {
			for (int i = 0; i < 4; ++i) {
				int indx = indexes[i];

				light[i] = Color(channel_color_r[indx] / 255.0,
						channel_color_g[indx] / 255.0,
						channel_color_b[indx] / 255.0);

				float ao = 0;

				if (use_ao)
					ao = channel_ao[indx] / 255.0;

				if (use_rao) {
					float rao = channel_rao[indx] / 255.0;
					ao += rao;
				}

				light[i] += base_light;

				if (ao > 0)
					light[i] -= Color(ao, ao, ao) * _ao_strength;

				light[i].r = CLAMP(light[i].r, 0, 1.0);
				light[i].g = CLAMP(light[i].g, 0, 1.0);
				light[i].b = CLAMP(light[i].b, 0, 1.0);
			}
		}

		int vc = get_vertex_count();
		add_indices(vc + 2);
		add_indices(vc + 1);
		add_indices(vc + 0);
		add_indices(vc + 3);
		add_indices(vc + 2);
		add_indices(vc + 0);

		Vector2 uvs[] = {
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 0), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale()),
			surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 1), x % get_texture_scale(), z % get_texture_scale(), get_texture_scale())
		};

		float vi0 = Math::lerp(isolevels[1], isolevels[2], z_interp);
		float vi1 = Math::lerp(isolevels[1], isolevels[2], zp1_interp);

		Vector3 verts[] = {
			Vector3(x + 1, isolevels[0] / 255.0 * world_height, z) * voxel_scale,
			Vector3(x, vi0 / 255.0 * world_height, z) * voxel_scale,
			Vector3(x, vi1 / 255.0 * world_height, z + 1) * voxel_scale,
			Vector3(x + 1, isolevels[3] / 255.0 * world_height, z + 1) * voxel_scale
		};

		Vector3 normals[] = {
			(verts[0] - verts[1]).cross(verts[0] - verts[2]).normalized(),
			(verts[0] - verts[1]).cross(verts[1] - verts[2]).normalized(),
			(verts[1] - verts[2]).cross(verts[2] - verts[0]).normalized(),
			(verts[2] - verts[3]).cross(verts[3] - verts[0]).normalized(),
		};

		for (int i = 0; i < 4; ++i) {
			add_normal(normals[i]);

			if (use_lighting || _always_add_colors)
				add_color(light[i]);

			add_uv(uvs[i]);
			add_vertex(verts[i]);
		}
	}
}

void TerrainMesherBlocky::create_margin_corners(Ref<TerrainChunkDefault> chunk) {
	create_face(chunk, 1, 2, 1, 2);
	create_face(chunk, 1, 2, chunk->get_size_z() + chunk->get_margin_start() - 1, chunk->get_size_z() + chunk->get_margin_start());
	create_face(chunk, chunk->get_size_x() + chunk->get_margin_start() - 1, chunk->get_size_x() + chunk->get_margin_start(), 1, 2);
	create_face(chunk, chunk->get_size_x() + chunk->get_margin_start() - 1, chunk->get_size_x() + chunk->get_margin_start(), chunk->get_size_z() + chunk->get_margin_start() - 1, chunk->get_size_z() + chunk->get_margin_start());
}

void TerrainMesherBlocky::create_face(Ref<TerrainChunkDefault> chunk, int dataxmin, int dataxmax, int datazmin, int datazmax) {
	//if ((get_build_flags() & TerrainChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
	//	if (!chunk->get_channel(TerrainChunkDefault::DEFAULT_CHANNEL_AO))
	//		chunk->generate_ao();

	float world_height = chunk->get_world_height();

	float voxel_scale = get_voxel_scale();

	uint8_t *channel_type = chunk->channel_get(_channel_index_type);

	if (!channel_type)
		return;

	uint8_t *channel_isolevel = chunk->channel_get(_channel_index_isolevel);

	if (!channel_isolevel)
		return;

	uint8_t *channel_color_r = NULL;
	uint8_t *channel_color_g = NULL;
	uint8_t *channel_color_b = NULL;
	uint8_t *channel_ao = NULL;
	uint8_t *channel_rao = NULL;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light[4]{ Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };

	bool use_lighting = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & TerrainChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	if (use_lighting) {
		channel_color_r = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
		channel_color_g = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
		channel_color_b = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

		if (use_ao)
			channel_ao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_AO);

		if (use_rao)
			channel_rao = chunk->channel_get_valid(TerrainChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
	}

	Ref<TerrainMaterialCache> mcache;

	if (!get_is_liquid_mesher()) {
		if (chunk->material_cache_key_has()) {
			mcache = _library->material_cache_get(chunk->material_cache_key_get());
		}
	} else {
		if (chunk->liquid_material_cache_key_has()) {
			mcache = _library->liquid_material_cache_get(chunk->liquid_material_cache_key_get());
		}
	}

	int indexes[4] = {
		chunk->get_data_index(dataxmax, datazmin), //x + 1
		chunk->get_data_index(dataxmin, datazmin), //x
		chunk->get_data_index(dataxmin, datazmax),
		chunk->get_data_index(dataxmax, datazmax),
	};

	uint8_t type = channel_type[indexes[0]];

	if (type == 0)
		return;

	Ref<TerrainSurface> surface;

	if (!mcache.is_valid()) {
		surface = _library->terra_surface_get(type - 1);
	} else {
		surface = mcache->surface_id_get(type - 1);
	}

	if (!surface.is_valid())
		return;

	uint8_t isolevels[] = {
		channel_isolevel[indexes[0]],
		channel_isolevel[indexes[1]],
		channel_isolevel[indexes[2]],
		channel_isolevel[indexes[3]],
	};

	if (use_lighting) {
		for (int i = 0; i < 4; ++i) {
			int indx = indexes[i];

			light[i] = Color(channel_color_r[indx] / 255.0,
					channel_color_g[indx] / 255.0,
					channel_color_b[indx] / 255.0);

			float ao = 0;

			if (use_ao)
				ao = channel_ao[indx] / 255.0;

			if (use_rao) {
				float rao = channel_rao[indx] / 255.0;
				ao += rao;
			}

			light[i] += base_light;

			if (ao > 0)
				light[i] -= Color(ao, ao, ao) * _ao_strength;

			light[i].r = CLAMP(light[i].r, 0, 1.0);
			light[i].g = CLAMP(light[i].g, 0, 1.0);
			light[i].b = CLAMP(light[i].b, 0, 1.0);
		}
	}

	int vc = get_vertex_count();
	add_indices(vc + 2);
	add_indices(vc + 1);
	add_indices(vc + 0);
	add_indices(vc + 3);
	add_indices(vc + 2);
	add_indices(vc + 0);

	Vector2 uvs[] = {
		surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 0), dataxmin % get_texture_scale(), datazmin % get_texture_scale(), get_texture_scale()),
		surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 0), dataxmin % get_texture_scale(), datazmin % get_texture_scale(), get_texture_scale()),
		surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(0, 1), dataxmin % get_texture_scale(), datazmin % get_texture_scale(), get_texture_scale()),
		surface->transform_uv_scaled(TerrainSurface::TERRAIN_SIDE_TOP, Vector2(1, 1), dataxmin % get_texture_scale(), datazmin % get_texture_scale(), get_texture_scale())
	};

	Vector3 verts[] = {
		Vector3(dataxmax, isolevels[0] / 255.0 * world_height, datazmin) * voxel_scale,
		Vector3(dataxmin, isolevels[1] / 255.0 * world_height, datazmin) * voxel_scale,
		Vector3(dataxmin, isolevels[2] / 255.0 * world_height, datazmax) * voxel_scale,
		Vector3(dataxmax, isolevels[3] / 255.0 * world_height, datazmax) * voxel_scale
	};

	Vector3 normals[] = {
		(verts[0] - verts[1]).cross(verts[0] - verts[2]).normalized(),
		(verts[0] - verts[1]).cross(verts[1] - verts[2]).normalized(),
		(verts[1] - verts[2]).cross(verts[2] - verts[0]).normalized(),
		(verts[2] - verts[3]).cross(verts[3] - verts[0]).normalized(),
	};

	for (int i = 0; i < 4; ++i) {
		add_normal(normals[i]);

		if (use_lighting || _always_add_colors)
			add_color(light[i]);

		add_uv(uvs[i]);
		add_vertex(verts[i]);
	}
}

TerrainMesherBlocky::TerrainMesherBlocky() {
	_always_add_colors = false;
}

TerrainMesherBlocky::~TerrainMesherBlocky() {
}

void TerrainMesherBlocky::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_add_chunk", "buffer"), &TerrainMesherBlocky::_add_chunk);

	ClassDB::bind_method(D_METHOD("get_always_add_colors"), &TerrainMesherBlocky::get_always_add_colors);
	ClassDB::bind_method(D_METHOD("set_always_add_colors", "value"), &TerrainMesherBlocky::set_always_add_colors);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "always_add_colors"), "set_always_add_colors", "get_always_add_colors");
}
