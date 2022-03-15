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

#include "terrain_2d_mesher_default.h"

#include "../../library/terrain_2d_material_cache.h"
#include "../../world/default/terrain_2d_chunk_default.h"

#include "../../defines.h"

#include visual_server_h
#include mesh_instance_h

_FORCE_INLINE_ int Terrain2DMesherDefault::get_build_flags() const {
	return _build_flags;
}
_FORCE_INLINE_ void Terrain2DMesherDefault::set_build_flags(const int flags) {
	_build_flags = flags;

	if ((_build_flags & Terrain2DChunkDefault::BUILD_FLAG_USE_LIGHTING) != 0) {
		_format |= VisualServer::ARRAY_FORMAT_COLOR;
	} else {
		_format ^= VisualServer::ARRAY_FORMAT_COLOR;
	}
}

void Terrain2DMesherDefault::create_tile_colliders(Ref<Terrain2DChunk> ch) {
	Ref<Terrain2DChunkDefault> chunk = ch;

	ERR_FAIL_COND(!chunk.is_valid());

	if (!chunk->get_default_tile_shape().is_valid()) {
		chunk->set_default_tile_shape(create_terrain_tile_collider_shape(chunk));
	}

	uint8_t *channel_flags = chunk->channel_get(Terrain2DChunkDefault::DEFAULT_CHANNEL_FLAGS);

	if (!channel_flags)
		return;

	Ref<Shape2D> tile_collider_shape = chunk->get_default_tile_shape();

	RID body_rid = chunk->_2d_body_rid;

	if (body_rid == RID()) {
		body_rid = Physics2DServer::get_singleton()->body_create();
		chunk->_2d_body_rid = body_rid;

		if (chunk->is_in_tree()) {
			RID space = chunk->get_voxel_world()->get_world_2d()->get_space();
			Physics2DServer::get_singleton()->body_set_space(body_rid, space);
		}

		Physics2DServer::get_singleton()->body_set_state(body_rid, Physics2DServer::BODY_STATE_TRANSFORM, chunk->get_transform());
	}

#if TOOLS_ENABLED
	chunk->_debug_terrain_collider_transforms.clear();
#endif
	Physics2DServer::get_singleton()->body_clear_shapes(body_rid);

	Physics2DServer::get_singleton()->body_set_mode(body_rid, Physics2DServer::BODY_MODE_STATIC);
	Physics2DServer::get_singleton()->body_attach_object_instance_id(body_rid, get_instance_id());
	//TODO
	Physics2DServer::get_singleton()->body_set_collision_layer(body_rid, 1);
	Physics2DServer::get_singleton()->body_set_collision_mask(body_rid, 1);

	int x_size = chunk->get_size_x();
	int y_size = chunk->get_size_y();
	int cell_size_x = get_cell_size_x();
	int cell_size_y = get_cell_size_y();

	Transform2D mesh_transform_terrain = chunk->mesh_transform_terrain_get();

	Ref<Terrain2DMaterialCache> mcache;

	if (chunk->material_cache_key_has()) {
		mcache = _library->material_cache_get(chunk->material_cache_key_get());
	}

#if TOOLS_ENABLED
	SceneTree *st = SceneTree::get_singleton();

	bool debug_shapes = false;
	if (st) {
		if (Engine::get_singleton()->is_editor_hint()) {
			//debug_shapes = show_collision;
		} else {
			debug_shapes = st->is_debugging_collisions_hint();
		}
	}
#endif

	int margin_start = chunk->get_margin_start();
	//z_size + margin_start is fine, x, and z are in data space.
	for (int dy = margin_start; dy < y_size + margin_start; ++dy) {
		for (int dx = margin_start; dx < x_size + margin_start; ++dx) {
			int index = chunk->get_data_index(dx, dy);

			int x = dx - margin_start;
			int y = dy - margin_start;

			int flags = channel_flags[index];

			if ((flags & Terrain2DChunkDefault::FLAG_CHANNEL_COLLIDER) != 0) {
				Transform2D coll_transform(0, mesh_transform_terrain.xform(Vector2((x)*cell_size_x, y * cell_size_y)));

#if TOOLS_ENABLED
				if (debug_shapes) {
					chunk->_debug_terrain_collider_transforms.push_back(coll_transform);
				}
#endif

				Physics2DServer::get_singleton()->body_add_shape(body_rid, tile_collider_shape->get_rid(), coll_transform);
			}
		}
	}
}

void Terrain2DMesherDefault::_bake_colors(Ref<Terrain2DChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	//if ((get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_LIGHTING) == 0)
	//	return;

	if (_vertices.size() == 0)
		return;

	uint8_t *channel_color_r = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
	uint8_t *channel_color_g = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
	uint8_t *channel_color_b = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);
	uint8_t *channel_ao = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_AO);
	uint8_t *channel_rao = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);

	Color base_light(_base_light_value, _base_light_value, _base_light_value);

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex vertex = _vertices[i];
		Vector2 vert = vertex.vertex;

		unsigned int x = (unsigned int)(vert.x / _cell_size_x);
		unsigned int y = (unsigned int)(vert.y / _cell_size_y);

		if (chunk->validate_data_position(x, y)) {
			int indx = chunk->get_data_index(x, y);

			Color light = Color(
					channel_color_r[indx] / 255.0,
					channel_color_g[indx] / 255.0,
					channel_color_b[indx] / 255.0);

			float ao = (channel_ao[indx] / 255.0) * _ao_strength;
			float rao = channel_rao[indx] / 255.0;

			ao += rao;

			light.r += _base_light_value;
			light.g += _base_light_value;
			light.b += _base_light_value;

			light.r -= ao;
			light.g -= ao;
			light.b -= ao;

			light.r = CLAMP(light.r, 0, 1.0);
			light.g = CLAMP(light.g, 0, 1.0);
			light.b = CLAMP(light.b, 0, 1.0);

			Color c = vertex.color;
			light.a = c.a;
			vertex.color = light;

			_vertices.set(i, vertex);
		} else {
			vertex.color = base_light;
			_vertices.set(i, vertex);
		}
	}
}

void Terrain2DMesherDefault::_bake_liquid_colors(Ref<Terrain2DChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if ((get_build_flags() & Terrain2DChunkDefault::BUILD_FLAG_USE_LIGHTING) == 0)
		return;

	if (_vertices.size() == 0)
		return;

	uint8_t *channel_color_r = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R);
	uint8_t *channel_color_g = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G);
	uint8_t *channel_color_b = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B);
	uint8_t *channel_ao = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_AO);
	uint8_t *channel_rao = chunk->channel_get_valid(Terrain2DChunkDefault::DEFAULT_CHANNEL_RANDOM_AO);

	Color base_light(_base_light_value, _base_light_value, _base_light_value);

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex vertex = _vertices[i];
		Vector2 vert = vertex.vertex;

		//Is this needed?
		if (vert.x < 0 || vert.y < 0) {
			continue;
		}

		unsigned int x = (unsigned int)(vert.x / _cell_size_x);
		unsigned int y = (unsigned int)(vert.y / _cell_size_y);

		if (chunk->validate_data_position(x, y)) {
			int indx = chunk->get_data_index(x, y);

			Color light = Color(
					channel_color_r[indx] / 255.0,
					channel_color_g[indx] / 255.0,
					channel_color_b[indx] / 255.0);

			float ao = (channel_ao[indx] / 255.0) * _ao_strength;
			float rao = channel_rao[indx] / 255.0;

			ao += rao;

			light.r += _base_light_value;
			light.g += _base_light_value;
			light.b += _base_light_value;

			light.r -= ao;
			light.g -= ao;
			light.b -= ao;

			light.r = CLAMP(light.r, 0, 1.0);
			light.g = CLAMP(light.g, 0, 1.0);
			light.b = CLAMP(light.b, 0, 1.0);

			Color c = vertex.color;
			light.a = c.a;
			vertex.color = light;

			_vertices.set(i, vertex);
		} else {
			vertex.color = base_light;
			_vertices.set(i, vertex);
		}
	}
}

Terrain2DMesherDefault::Terrain2DMesherDefault() {
	_build_flags = Terrain2DChunkDefault::BUILD_FLAG_CREATE_COLLIDER;

	_format = VisualServer::ARRAY_FORMAT_NORMAL | VisualServer::ARRAY_FORMAT_TEX_UV;
}

Terrain2DMesherDefault::~Terrain2DMesherDefault() {
}

void Terrain2DMesherDefault::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_bake_colors", "chunk"), &Terrain2DMesherDefault::_bake_colors);
	ClassDB::bind_method(D_METHOD("_bake_liquid_colors", "chunk"), &Terrain2DMesherDefault::_bake_liquid_colors);

	ClassDB::bind_method(D_METHOD("get_build_flags"), &Terrain2DMesherDefault::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &Terrain2DMesherDefault::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, Terrain2DChunkDefault::BINDING_STRING_BUILD_FLAGS), "set_build_flags", "get_build_flags");
}
