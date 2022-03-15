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

#include "terrain_2d_mesher_simple.h"

#include "core/math/math_funcs.h"

#include "../../library/terrain_2d_material_cache.h"

void Terrain2DMesherSimple::_add_chunk(Ref<Terrain2DChunk> p_chunk) {
	Ref<Terrain2DChunkDefault> chunk = p_chunk;

	ERR_FAIL_COND(!chunk.is_valid());
	ERR_FAIL_COND(chunk->get_margin_end() < 1);
	ERR_FAIL_COND(chunk->get_margin_start() < 1);

	add_chunk_normal(chunk);
}

void Terrain2DMesherSimple::add_chunk_normal(Ref<Terrain2DChunkDefault> chunk) {
	//if ((get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_GENERATE_AO) != 0)
	//	if (!chunk->get_channel(Terrain2DChunkDefault::DEFAULT_CHANNEL_AO))
	//		chunk->generate_ao();

	uint8_t *channel_type = chunk->channel_get(_channel_index_type);

	if (!channel_type)
		return;

	uint8_t *channel_flags = chunk->channel_get(Terrain2DChunkDefault::DEFAULT_CHANNEL_FLAGS);

	if (!channel_flags)
		return;

	int x_size = chunk->get_size_x();
	int y_size = chunk->get_size_y();
	int cell_size_x = get_cell_size_x();
	int cell_size_y = get_cell_size_y();

	Transform2D mesh_transform_terrain = chunk->mesh_transform_terrain_get();
	Transform2D mesh_transform_wall_north = chunk->mesh_transform_wall_north_get();
	Transform2D mesh_transform_wall_south = chunk->mesh_transform_wall_south_get();
	Transform2D mesh_transform_wall_east = chunk->mesh_transform_wall_east_get();
	Transform2D mesh_transform_wall_west = chunk->mesh_transform_wall_west_get();

	uint8_t *channel_color_r = NULL;
	uint8_t *channel_color_g = NULL;
	uint8_t *channel_color_b = NULL;
	uint8_t *channel_ao = NULL;
	uint8_t *channel_rao = NULL;

	Color base_light(_base_light_value, _base_light_value, _base_light_value);
	Color light[4]{ Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1), Color(1, 1, 1) };

	bool use_lighting = (get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0;
	bool use_ao = (get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_AO) != 0;
	bool use_rao = (get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_RAO) != 0;

	if (use_lighting) {
		channel_color_r = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
		channel_color_g = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
		channel_color_b = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);

		if (use_ao)
			channel_ao = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_AO);

		if (use_rao)
			channel_rao = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);
	}

	Ref<Terrain2DMaterialCache> mcache;

	if (chunk->material_cache_key_has()) {
		mcache = _library->material_cache_get(chunk->material_cache_key_get());
	}

	int texture_scale = get_texture_scale();

	int margin_start = chunk->get_margin_start();
	//z_size + margin_start is fine, x, and z are in data space.
	for (int y = margin_start; y < y_size + margin_start; ++y) {
		for (int x = margin_start; x < x_size + margin_start; ++x) {
			int indexes[4] = {
				chunk->get_data_index(x, y),
				chunk->get_data_index(x + 1, y),
				chunk->get_data_index(x, y + 1),
				chunk->get_data_index(x + 1, y + 1)
			};

			uint8_t type = channel_type[indexes[0]];

			if (type == 0)
				continue;

			Ref<Terrain2DSurface> surface;

			if (!mcache.is_valid()) {
				surface = _library->terra_surface_get(type - 1);
			} else {
				surface = mcache->surface_id_get(type - 1);
			}

			if (!surface.is_valid())
				continue;

			int flags = channel_flags[indexes[0]];

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

			Vector2 uvs[] = {
				Vector2(0, 0),
				Vector2(1, 0),
				Vector2(0, 1),
				Vector2(1, 1)
			};

			if (texture_scale > 1) {
				for (int i = 0; i < 4; ++i) {
					uvs[i] = surface->transform_uv_scaled(uvs[i], x % texture_scale, y % texture_scale, texture_scale);
				}
			} else {
				for (int i = 0; i < 4; ++i) {
					uvs[i] = surface->transform_uv(uvs[i]);
				}
			}

			Vector2 verts_normal[] = {
				Vector2(0, 0),
				Vector2(cell_size_x, 0),
				Vector2(0, cell_size_y),
				Vector2(cell_size_x, cell_size_y)
			};

			// Note that +y is down!
			Vector2 verts_wall[] = {
				Vector2(0, -cell_size_y),
				Vector2(cell_size_x, -cell_size_y),
				Vector2(0, 0),
				Vector2(cell_size_x, 0)
			};

			bool render_normal = true;

			int vc = get_vertex_count();

			bool hole = (flags & Terrain2DChunkDefault::FLAG_CHANNEL_WALL_HOLE) != 0;

			if ((flags & Terrain2DChunkDefault::FLAG_CHANNEL_WALL_NORTH) != 0) {
				render_normal = false;

				Vector2 vert_start_offset = mesh_transform_terrain.xform(Vector2(x * cell_size_x, y * cell_size_y));

				for (int i = 0; i < 4; ++i) {
					if (use_lighting) {
						add_color(light[i]);
					} else {
						add_color(Color(1, 1, 1, 1));
					}

					add_uv(uvs[i]);

					if (hole) {
						add_vertex(mesh_transform_wall_north.xform(verts_normal[i]) + vert_start_offset);
					} else {
						add_vertex(mesh_transform_wall_north.xform(verts_wall[i]) + vert_start_offset);
					}
				}

				add_indices(vc + 0);
				add_indices(vc + 1);
				add_indices(vc + 2);
				add_indices(vc + 1);
				add_indices(vc + 2);
				add_indices(vc + 3);

				vc += 4;
			}

			if ((flags & Terrain2DChunkDefault::FLAG_CHANNEL_WALL_WEST) != 0) {
				render_normal = false;

				Vector2 vert_start_offset = mesh_transform_terrain.xform(Vector2(x * cell_size_x, (y + 1) * cell_size_y));

				for (int i = 0; i < 4; ++i) {
					if (use_lighting) {
						add_color(light[i]);
					} else {
						add_color(Color(1, 1, 1, 1));
					}

					add_uv(uvs[i]);

					if (hole) {
						add_vertex(mesh_transform_wall_west.xform(verts_normal[i]) + vert_start_offset);
					} else {
						add_vertex(mesh_transform_wall_west.xform(verts_wall[i]) + vert_start_offset);
					}
				}

				add_indices(vc + 0);
				add_indices(vc + 1);
				add_indices(vc + 2);
				add_indices(vc + 1);
				add_indices(vc + 2);
				add_indices(vc + 3);

				vc += 4;
			}

			if ((flags & Terrain2DChunkDefault::FLAG_CHANNEL_WALL_SOUTH) != 0) {
				render_normal = false;

				Vector2 vert_start_offset = mesh_transform_terrain.xform(Vector2(x * cell_size_x, (y + 1) * cell_size_y));

				for (int i = 0; i < 4; ++i) {
					if (use_lighting) {
						add_color(light[i]);
					} else {
						add_color(Color(1, 1, 1, 1));
					}

					add_uv(uvs[i]);

					if (hole) {
						add_vertex(mesh_transform_wall_south.xform(verts_normal[i]) + vert_start_offset);
					} else {
						add_vertex(mesh_transform_wall_south.xform(verts_wall[i]) + vert_start_offset);
					}
				}

				add_indices(vc + 0);
				add_indices(vc + 1);
				add_indices(vc + 2);
				add_indices(vc + 1);
				add_indices(vc + 2);
				add_indices(vc + 3);

				vc += 4;
			}

			if ((flags & Terrain2DChunkDefault::FLAG_CHANNEL_WALL_EAST) != 0) {
				render_normal = false;

				Vector2 vert_start_offset = mesh_transform_terrain.xform(Vector2((x + 1) * cell_size_x, (y + 1) * cell_size_y));

				for (int i = 0; i < 4; ++i) {
					if (use_lighting) {
						add_color(light[i]);
					} else {
						add_color(Color(1, 1, 1, 1));
					}

					add_uv(uvs[i]);

					if (hole) {
						add_vertex(mesh_transform_wall_east.xform(verts_normal[i]) + vert_start_offset);
					} else {
						add_vertex(mesh_transform_wall_east.xform(verts_wall[i]) + vert_start_offset);
					}
				}

				add_indices(vc + 0);
				add_indices(vc + 1);
				add_indices(vc + 2);
				add_indices(vc + 1);
				add_indices(vc + 2);
				add_indices(vc + 3);

				vc += 4;
			}

			if (render_normal) {
				Vector2 vert_start_offset = mesh_transform_terrain.xform(Vector2(x * cell_size_x, y * cell_size_y));

				for (int i = 0; i < 4; ++i) {
					if (use_lighting) {
						add_color(light[i]);
					} else {
						add_color(Color(1, 1, 1, 1));
					}

					add_uv(uvs[i]);
					add_vertex(mesh_transform_terrain.xform(verts_normal[i]) + vert_start_offset);
				}

				add_indices(vc + 0);
				add_indices(vc + 1);
				add_indices(vc + 2);
				add_indices(vc + 1);
				add_indices(vc + 2);
				add_indices(vc + 3);
			}
		}
	}

	if ((chunk->get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
		bake_colors(chunk);
	}
}

Terrain2DMesherSimple::Terrain2DMesherSimple() {
}

Terrain2DMesherSimple::~Terrain2DMesherSimple() {
}

void Terrain2DMesherSimple::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_add_chunk", "buffer"), &Terrain2DMesherSimple::_add_chunk);
}
