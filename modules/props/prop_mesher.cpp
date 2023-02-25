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

#include "prop_mesher.h"

#include "lights/prop_light.h"
#include "modules/opensimplex/open_simplex_noise.h"

#include "material_cache/prop_material_cache.h"
#include "tiled_wall/tiled_wall_data.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_FASTNOISE_ENABLED
#include "../fastnoise/fastnoise_noise_params.h"
#include "../fastnoise/noise.h"
#endif

const String PropMesher::BINDING_STRING_BUILD_FLAGS = "Use Lighting,Use AO,Use RAO,Bake Lights";

bool PropMesher::Vertex::operator==(const Vertex &p_vertex) const {
	if (vertex != p_vertex.vertex) {
		return false;
	}

	if (uv != p_vertex.uv) {
		return false;
	}

	if (uv2 != p_vertex.uv2) {
		return false;
	}

	if (normal != p_vertex.normal) {
		return false;
	}

	if (binormal != p_vertex.binormal) {
		return false;
	}

	if (color != p_vertex.color) {
		return false;
	}

	if (bones.size() != p_vertex.bones.size()) {
		return false;
	}

	for (int i = 0; i < bones.size(); i++) {
		if (bones[i] != p_vertex.bones[i]) {
			return false;
		}
	}

	for (int i = 0; i < weights.size(); i++) {
		if (weights[i] != p_vertex.weights[i]) {
			return false;
		}
	}

	return true;
}

uint32_t PropMesher::VertexHasher::hash(const Vertex &p_vtx) {
	uint32_t h = hash_djb2_buffer((const uint8_t *)&p_vtx.vertex, sizeof(real_t) * 3);
	h = hash_djb2_buffer((const uint8_t *)&p_vtx.normal, sizeof(real_t) * 3, h);
	h = hash_djb2_buffer((const uint8_t *)&p_vtx.binormal, sizeof(real_t) * 3, h);
	h = hash_djb2_buffer((const uint8_t *)&p_vtx.tangent, sizeof(real_t) * 3, h);
	h = hash_djb2_buffer((const uint8_t *)&p_vtx.uv, sizeof(real_t) * 2, h);
	h = hash_djb2_buffer((const uint8_t *)&p_vtx.uv2, sizeof(real_t) * 2, h);
	h = hash_djb2_buffer((const uint8_t *)&p_vtx.color, sizeof(real_t) * 4, h);
	h = hash_djb2_buffer((const uint8_t *)p_vtx.bones.ptr(), p_vtx.bones.size() * sizeof(int), h);
	h = hash_djb2_buffer((const uint8_t *)p_vtx.weights.ptr(), p_vtx.weights.size() * sizeof(float), h);
	return h;
}

int PropMesher::get_channel_index_type() const {
	return _channel_index_type;
}
void PropMesher::set_channel_index_type(const int value) {
	_channel_index_type = value;
}

int PropMesher::get_channel_index_isolevel() const {
	return _channel_index_isolevel;
}
void PropMesher::set_channel_index_isolevel(const int value) {
	_channel_index_isolevel = value;
}

int PropMesher::get_mesher_index() const {
	return _mesher_index;
}
void PropMesher::set_mesher_index(const int value) {
	_mesher_index = value;
}

int PropMesher::get_format() const {
	return _format;
}
void PropMesher::set_format(const int value) {
	_format = value;
}

int PropMesher::get_texture_scale() const {
	return _texture_scale;
}
void PropMesher::set_texture_scale(const int value) {
	_texture_scale = value;
}

Ref<Material> PropMesher::get_material() {
	return _material;
}
void PropMesher::set_material(const Ref<Material> &material) {
	_material = material;
}

float PropMesher::get_ao_strength() const {
	return _ao_strength;
}
void PropMesher::set_ao_strength(float value) {
	_ao_strength = value;
}

float PropMesher::get_base_light_value() const {
	return _base_light_value;
}
void PropMesher::set_base_light_value(float value) {
	_base_light_value = value;
}

float PropMesher::get_voxel_scale() const {
	return _voxel_scale;
}
void PropMesher::set_voxel_scale(const float voxel_scale) {
	_voxel_scale = voxel_scale;
}

Rect2 PropMesher::get_uv_margin() const {
	return _uv_margin;
}
void PropMesher::set_uv_margin(const Rect2 margin) {
	_uv_margin = margin;
}

_FORCE_INLINE_ int PropMesher::get_build_flags() const {
	return _build_flags;
}
_FORCE_INLINE_ void PropMesher::set_build_flags(const int flags) {
	_build_flags = flags;

	if ((_build_flags & PropMesher::BUILD_FLAG_USE_LIGHTING) != 0) {
		_format |= RenderingServer::ARRAY_FORMAT_COLOR;
	} else {
		_format ^= RenderingServer::ARRAY_FORMAT_COLOR;
	}
}

Array PropMesher::build_mesh() {
	Array a;
	a.resize(RenderingServer::ARRAY_MAX);

	if (_vertices.size() == 0) {
		//Nothing to do
		return a;
	}

	{
		PoolVector<Vector3> array;
		array.resize(_vertices.size());
		PoolVector<Vector3>::Write w = array.write();

		for (int i = 0; i < _vertices.size(); ++i) {
			w[i] = _vertices[i].vertex;
		}

		w.release();

		a[RenderingServer::ARRAY_VERTEX] = array;
	}

	if ((_format & RenderingServer::ARRAY_FORMAT_NORMAL) == 0) {
		generate_normals();
	}

	{
		PoolVector<Vector3> array;
		array.resize(_vertices.size());
		PoolVector<Vector3>::Write w = array.write();

		for (int i = 0; i < _vertices.size(); ++i) {
			w[i] = _vertices[i].normal;
		}

		w.release();

		a[RenderingServer::ARRAY_NORMAL] = array;
	}

	if ((_format & RenderingServer::ARRAY_FORMAT_COLOR) != 0) {
		PoolVector<Color> array;
		array.resize(_vertices.size());

		PoolVector<Color>::Write w = array.write();

		for (int i = 0; i < _vertices.size(); ++i) {
			w[i] = _vertices[i].color;
		}

		w.release();

		a[RenderingServer::ARRAY_COLOR] = array;
	}

	if ((_format & RenderingServer::ARRAY_FORMAT_TEX_UV) != 0) {
		PoolVector<Vector2> array;
		array.resize(_vertices.size());

		PoolVector<Vector2>::Write w = array.write();

		for (int i = 0; i < _vertices.size(); ++i) {
			w[i] = _vertices[i].uv;
		}

		w.release();

		a[RenderingServer::ARRAY_TEX_UV] = array;
	}

	if ((_format & RenderingServer::ARRAY_FORMAT_TEX_UV2) != 0) {
		PoolVector<Vector2> array;
		array.resize(_vertices.size());

		PoolVector<Vector2>::Write w = array.write();

		for (int i = 0; i < _vertices.size(); ++i) {
			w[i] = _vertices[i].uv2;
		}

		w.release();

		a[RenderingServer::ARRAY_TEX_UV2] = array;
	}

	if (_indices.size() > 0) {
		PoolVector<int> array;
		array.resize(_indices.size());

		PoolVector<int>::Write w = array.write();

		for (int i = 0; i < _indices.size(); ++i) {
			w[i] = _indices[i];
		}

		w.release();

		a[RenderingServer::ARRAY_INDEX] = array;
	}

	return a;
}

void PropMesher::build_mesh_into(RID mesh) {
	ERR_FAIL_COND(mesh == RID());

	RS::get_singleton()->mesh_clear(mesh);

	if (_vertices.size() == 0) {
		//Nothing to do
		return;
	}

	Array arr = build_mesh();

	RS::get_singleton()->mesh_add_surface_from_arrays(mesh, RenderingServer::PRIMITIVE_TRIANGLES, arr);

	if (_material.is_valid()) {
		RS::get_singleton()->mesh_surface_set_material(mesh, 0, _material->get_rid());
	}
}

void PropMesher::generate_normals(bool p_flip) {
	_format = _format | RenderingServer::ARRAY_FORMAT_NORMAL;

	for (int i = 0; i < _indices.size(); i += 3) {
		int i0 = _indices[i];
		int i1 = _indices[i + 1];
		int i2 = _indices[i + 2];

		ERR_FAIL_INDEX(i0, _vertices.size());
		ERR_FAIL_INDEX(i1, _vertices.size());
		ERR_FAIL_INDEX(i2, _vertices.size());

		Vertex v0 = _vertices.get(i0);
		Vertex v1 = _vertices.get(i1);
		Vertex v2 = _vertices.get(i2);

		Vector3 normal;
		if (!p_flip) {
			normal = Plane(v0.vertex, v1.vertex, v2.vertex).normal;
		} else {
			normal = Plane(v2.vertex, v1.vertex, v0.vertex).normal;
		}

		v0.normal = normal;
		v1.normal = normal;
		v2.normal = normal;

		_vertices.set(i0, v0);
		_vertices.set(i1, v1);
		_vertices.set(i2, v2);
	}
}

void PropMesher::remove_doubles() {
	if (_vertices.size() == 0) {
		return;
	}

	//print_error("before " + String::num(_vertices.size()));

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex vert = _vertices[i];
		PoolVector<int> indices;

		for (int j = i + 1; j < _vertices.size(); ++j) {
			if (_vertices[j] == vert) {
				indices.push_back(j);
			}
		}

		for (int j = 0; j < indices.size(); ++j) {
			int index = indices[j];

			_vertices.remove(index);

			//make all indices that were bigger than the one we replaced one lower
			for (int k = 0; k < _indices.size(); ++k) {
				int indx = _indices[k];

				if (indx == index) {
					_indices.set(k, i);
				} else if (indx > index) {
					_indices.set(k, --indx);
				}
			}

			for (int k = j + 1; k < indices.size(); ++k) {
				int val = indices[k];

				if (val > index) {
					indices.set(k, --val);
				}
			}
		}
	}

	//print_error("after " + String::num(_vertices.size())+ " " + String::num(duration.count()));
}

//lot faster that normal remove_doubles, but false positives can happen curtesy of hash collisions
void PropMesher::remove_doubles_hashed() {
	if (_vertices.size() == 0) {
		return;
	}

	//print_error("before " + String::num(_vertices.size()));

	PoolVector<uint32_t> hashes;
	hashes.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		hashes.set(i, VertexHasher::hash(_vertices[i]));
	}

	for (int i = 0; i < hashes.size(); ++i) {
		uint32_t hash = hashes[i];
		PoolVector<int> indices;

		for (int j = i + 1; j < hashes.size(); ++j) {
			if (hashes[j] == hash) {
				indices.push_back(j);
			}
		}

		for (int j = 0; j < indices.size(); ++j) {
			int index = indices[j];

			hashes.remove(index);
			_vertices.remove(index);

			//make all indices that were bigger than the one we replaced one lower
			for (int k = 0; k < _indices.size(); ++k) {
				int indx = _indices[k];

				if (indx == index) {
					_indices.set(k, i);
				} else if (indx > index) {
					_indices.set(k, --indx);
				}
			}

			for (int k = j + 1; k < indices.size(); ++k) {
				int val = indices[k];

				if (val > index) {
					indices.set(k, --val);
				}
			}
		}
	}

	//print_error("after " + String::num(_vertices.size()) + " " + String::num(duration.count()));
}

void PropMesher::reset() {
	_vertices.resize(0);
	_indices.resize(0);

	_last_color = Color();
	_last_normal = Vector3();
	_last_uv = Vector2();
	_last_uv2 = Vector2();
	_last_bones.clear();
	_last_weights.clear();
	_last_tangent = Plane();
}

void PropMesher::add_tiled_wall_simple(const int width, const int height, const Transform &transform, const Ref<TiledWallData> &tiled_wall_data, Ref<PropMaterialCache> cache) {
	ERR_FAIL_COND(!tiled_wall_data.is_valid());
	ERR_FAIL_COND(!cache.is_valid());
	ERR_FAIL_COND(width < 0);
	ERR_FAIL_COND(height < 0);

	if (tiled_wall_data->get_tile_count() == 0) {
		return;
	}

#ifdef MODULE_FASTNOISE_ENABLED
	int tiled_wall_vertex_start_index = _vertices.size();
#endif

	float flavour_chance = tiled_wall_data->get_flavour_tile_chance();

	//collect rects
	LocalVector<Rect2> normal_rects;
	LocalVector<Rect2> flavour_rects;
	LocalVector<TiledWallData::TextureEntry> normal_data;
	LocalVector<TiledWallData::TextureEntry> flavour_data;

	for (int i = 0; i < tiled_wall_data->get_tile_count(); ++i) {
		const Ref<Texture> &t = tiled_wall_data->get_tile_texture(i);

		if (t.is_valid()) {
			normal_rects.push_back(cache->texture_get_uv_rect(t));
			normal_data.push_back(tiled_wall_data->get_tile(i));
		}
	}

	for (int i = 0; i < tiled_wall_data->get_flavour_tile_count(); ++i) {
		const Ref<Texture> &t = tiled_wall_data->get_flavour_tile_texture(i);

		if (t.is_valid()) {
			flavour_rects.push_back(cache->texture_get_uv_rect(t));
			flavour_data.push_back(tiled_wall_data->get_flavour_tile(i));
		}
	}

	//fallback
	if (normal_rects.size() == 0) {
		normal_rects.push_back(Rect2(0, 0, 1, 1));
	}

	TiledWallData::TiledWallTilingType tiling_type = tiled_wall_data->get_tiling_type();
	float cys = 0;

	if (tiling_type == TiledWallData::TILED_WALL_TILING_TYPE_NONE) {
		Rect2 r = normal_rects[0];
		float yh = normal_data[0].y_size;
		int texture_scale = normal_data[0].texture_scale;

		if (flavour_rects.size() == 0) {
			//no flavours
			for (int x = 0; x < width; ++x) {
				cys = 0;

				for (int y = 0; y < height; ++y) {
					add_tiled_wall_mesh_rect_simple(x, cys, yh, 0, 0, transform, r, texture_scale);

					cys += yh;
				}
			}
		} else {
			//has flavours
			for (int x = 0; x < width; ++x) {
				cys = 0;

				for (int y = 0; y < height; ++y) {
					if (Math::randf() > flavour_chance) {
						add_tiled_wall_mesh_rect_simple(x, cys, yh, 0, 0, transform, r, texture_scale);

						cys += yh;
					} else {
						int indx = Math::rand() % flavour_rects.size();

						float fyh = flavour_data[indx].y_size;
						add_tiled_wall_mesh_rect_simple(x, cys, fyh, 0, 0, transform, flavour_rects[indx], flavour_data[indx].texture_scale);

						cys += fyh;
					}
				}
			}
		}
	} else if (tiling_type == TiledWallData::TILED_WALL_TILING_TYPE_HORIZONTAL) {
		Rect2 r;

		if (flavour_rects.size() == 0) {
			//no flavours
			for (int x = 0; x < width; ++x) {
				int indx = x % normal_rects.size();

				r = normal_rects[indx];
				float ysize = normal_data[indx].y_size;
				float z_offset = normal_data[indx].z_offset;
				int texture_scale = normal_data[indx].texture_scale;

				cys = 0;

				for (int y = 0; y < height; ++y) {
					add_tiled_wall_mesh_rect_simple(x, cys, ysize, z_offset, z_offset, transform, r, texture_scale);

					cys += ysize;
				}
			}
		} else {
			//has flavours
			for (int x = 0; x < width; ++x) {
				int indx = x % normal_rects.size();

				r = normal_rects[indx];
				float ysize = normal_data[indx].y_size;
				int texture_scale = normal_data[indx].texture_scale;

				cys = 0;

				for (int y = 0; y < height; ++y) {
					if (Math::randf() > flavour_chance) {
						add_tiled_wall_mesh_rect_simple(x, cys, ysize, 0, 0, transform, r, texture_scale);

						cys += ysize;
					} else {
						int findx = Math::rand() % flavour_rects.size();
						float yh = flavour_data[findx].y_size;

						add_tiled_wall_mesh_rect_simple(x, cys, yh, 0, 0, transform, flavour_rects[findx], flavour_data[indx].texture_scale);

						cys += yh;
					}
				}
			}
		}
	} else if (tiling_type == TiledWallData::TILED_WALL_TILING_TYPE_VERTICAL) {
		Rect2 r;

		if (flavour_rects.size() == 0) {
			//no flavours

			for (int x = 0; x < width; ++x) {
				cys = 0;

				float prev_z_offset = 0;

				for (int y = 0; y < height; ++y) {
					int indx = y % normal_rects.size();

					r = normal_rects[indx];
					float ysize = normal_data[indx].y_size;
					float current_z_offset = normal_data[indx].z_offset;

					if (y == 0) {
						prev_z_offset = current_z_offset;
					}

					add_tiled_wall_mesh_rect_simple(x, cys, ysize, prev_z_offset, current_z_offset, transform, r, normal_data[indx].texture_scale);

					cys += ysize;

					prev_z_offset = current_z_offset;
				}
			}
		} else {
			//has flavours
			for (int x = 0; x < width; ++x) {
				cys = 0;

				float prev_z_offset = 0;

				for (int y = 0; y < height; ++y) {
					int indx = y % normal_rects.size();

					r = normal_rects[indx];
					float ysize = normal_data[indx].y_size;
					float current_z_offset = 0;

					if (y == 0) {
						prev_z_offset = current_z_offset;
					}

					if (Math::randf() > flavour_chance) {
						current_z_offset = normal_data[indx].z_offset;

						add_tiled_wall_mesh_rect_simple(x, cys, ysize, prev_z_offset, current_z_offset, transform, r, normal_data[indx].texture_scale);
						cys += ysize;
					} else {
						int findx = Math::rand() % flavour_rects.size();
						current_z_offset = flavour_data[findx].z_offset;

						add_tiled_wall_mesh_rect_simple(x, cys, ysize, prev_z_offset, current_z_offset, transform, flavour_rects[findx], flavour_data[findx].texture_scale);
						cys += ysize;
					}

					prev_z_offset = current_z_offset;
				}
			}
		}
	} else if (tiling_type == TiledWallData::TILED_WALL_TILING_TYPE_BOTH) {
		Rect2 r;

		if (flavour_rects.size() == 0) {
			//no flavours
			for (int x = 0; x < width; ++x) {
				cys = 0;

				for (int y = 0; y < height; ++y) {
					int indx = (x + y) % normal_rects.size();

					r = normal_rects[indx];
					float ysize = normal_data[indx].y_size;

					add_tiled_wall_mesh_rect_simple(x, cys, ysize, 0, 0, transform, r, normal_data[indx].texture_scale);

					cys += ysize;
				}
			}
		} else {
			//has flavours
			for (int x = 0; x < width; ++x) {
				cys = 0;

				for (int y = 0; y < height; ++y) {
					int indx = (x + y) % normal_rects.size();

					r = normal_rects[indx];
					float ysize = normal_data[indx].y_size;

					if (Math::randf() > flavour_chance) {
						add_tiled_wall_mesh_rect_simple(x, cys, ysize, 0, 0, transform, r, normal_data[indx].texture_scale);
						cys += ysize;
					} else {
						int findx = Math::rand() % flavour_rects.size();
						float yh = flavour_data[findx].y_size;

						add_tiled_wall_mesh_rect_simple(x, cys, ysize, 0, 0, transform, flavour_rects[findx], flavour_data[findx].texture_scale);

						cys += yh;
					}
				}
			}
		}
	}

#ifdef MODULE_FASTNOISE_ENABLED
	Ref<TiledWallData> twd = tiled_wall_data;
	Ref<FastnoiseNoiseParams> offset_noise = twd->get_offset_noise();

	if (offset_noise.is_null()) {
		return;
	}

	float offset_noise_strength = tiled_wall_data->get_offset_noise_strength();
	Ref<FastNoise> noise;
	noise.instance();
	offset_noise->setup_noise(noise);

	if (twd->get_offset_noise_randomize_seed()) {
		noise->set_seed(Math::rand());
	}

	if (twd->get_offset_noise_skip_edges()) {
		Vector3 vert_min = transform.xform(Vector3(0, 0, 0));
		Vector3 vert_max = transform.xform(Vector3(width, cys, 0));

		int vs = _vertices.size();
		PoolVector<Vertex>::Write w = _vertices.write();
		Vertex *wptr = w.ptr();

		for (int i = tiled_wall_vertex_start_index; i < vs; ++i) {
			Vertex v = wptr[i];

			int sim_count = 0;

			if (Math::is_equal_approx(v.vertex.x, vert_min.x) || Math::is_equal_approx(v.vertex.x, vert_max.x)) {
				++sim_count;
			}

			if (Math::is_equal_approx(v.vertex.y, vert_min.y) || Math::is_equal_approx(v.vertex.y, vert_max.y)) {
				++sim_count;
			}

			if (sim_count == 1 && (Math::is_equal_approx(v.vertex.z, vert_min.z) || Math::is_equal_approx(v.vertex.z, vert_max.z))) {
				++sim_count;
			}

			if (sim_count > 1) {
				continue;
			}

			float n = noise->get_noise_2d(v.vertex.x, v.vertex.z) * offset_noise_strength;
			v.vertex += transform.basis.xform(Vector3(0, 0, n));

			wptr[i] = v;
		}
	} else {
		int vs = _vertices.size();
		PoolVector<Vertex>::Write w = _vertices.write();
		Vertex *wptr = w.ptr();
		for (int i = tiled_wall_vertex_start_index; i < vs; ++i) {
			Vertex v = wptr[i];

			float n = noise->get_noise_2d(v.vertex.x, v.vertex.z) * offset_noise_strength;
			v.vertex += transform.basis.xform(Vector3(0, 0, n));

			wptr[i] = v;
		}
	}

#endif
}

void PropMesher::add_tiled_wall_mesh_rect_simple(const float x, const float y, const float y_size, const float prev_z_offset, const float current_z_offset, const Transform &transform, const Rect2 &texture_rect, const int texture_scale) {
	int vc = get_vertex_count();

	float cy = CLAMP(0.0, 1.0, y_size);

	//x + 1, y
	add_normal(transform.basis.xform(Vector3(0, 0, -1)));
	add_uv(transform_uv_scaled(Vector2(1, cy), texture_rect, x, y, texture_scale));
	add_vertex(transform.xform(Vector3(x + 1, y, prev_z_offset)));

	//x, y
	add_normal(transform.basis.xform(Vector3(0, 0, -1)));
	add_uv(transform_uv_scaled(Vector2(0, cy), texture_rect, x, y, texture_scale));
	add_vertex(transform.xform(Vector3(x, y, prev_z_offset)));

	//x, y + 1
	add_normal(transform.basis.xform(Vector3(0, 0, -1)));
	add_uv(transform_uv_scaled(Vector2(0, 0), texture_rect, x, y, texture_scale));
	add_vertex(transform.xform(Vector3(x, y + y_size, current_z_offset)));

	//x + 1, y + 1
	add_normal(transform.basis.xform(Vector3(0, 0, -1)));
	add_uv(transform_uv_scaled(Vector2(1, 0), texture_rect, x, y, texture_scale));
	add_vertex(transform.xform(Vector3(x + 1, y + y_size, current_z_offset)));

	add_indices(vc + 2);
	add_indices(vc + 1);
	add_indices(vc + 0);
	add_indices(vc + 3);
	add_indices(vc + 2);
	add_indices(vc + 0);
}

_FORCE_INLINE_ Vector2 PropMesher::transform_uv(const Vector2 &uv, const Rect2 &rect) const {
	Vector2 ruv = uv;

	ruv.x *= rect.size.x;
	ruv.y *= rect.size.y;
	ruv.x += rect.position.x;
	ruv.y += rect.position.y;

	return ruv;
}

Vector2 PropMesher::transform_uv_scaled(const Vector2 &uv, const Rect2 &rect, const int x, const int y, const int texture_scale) const {
	Vector2 ruv = uv;

	int lx = x % texture_scale;
	int ly = y % texture_scale;

	float sizex = rect.size.x / static_cast<float>(texture_scale);
	float sizey = rect.size.y / static_cast<float>(texture_scale);

	ruv.x *= sizex;
	ruv.y *= sizey;

	ruv.x += rect.position.x + sizex * lx;
	ruv.y += rect.position.y + sizey * ly;

	return ruv;
}

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
void PropMesher::add_mesh_data_resource(Ref<MeshDataResource> mesh, const Vector3 position, const Vector3 rotation, const Vector3 scale, const Rect2 uv_rect) {
	Transform transform = Transform(Basis(rotation).scaled(scale), position);

	add_mesh_data_resource_transform(mesh, transform, uv_rect);
}

void PropMesher::add_mesh_data_resource_transform(Ref<MeshDataResource> mesh, const Transform transform, const Rect2 uv_rect) {
	if (mesh->get_array().size() == 0) {
		return;
	}

	const Array &arr = mesh->get_array();

	PoolVector3Array vertices = arr[Mesh::ARRAY_VERTEX];
	PoolVector3Array normals = arr[Mesh::ARRAY_NORMAL];
	PoolVector2Array uvs = arr[Mesh::ARRAY_TEX_UV];
	PoolColorArray colors = arr[Mesh::ARRAY_COLOR];
	PoolIntArray indices = arr[Mesh::ARRAY_INDEX];

	if (vertices.size() == 0) {
		return;
	}

	int orig_vert_size = _vertices.size();

	for (int i = 0; i < vertices.size(); ++i) {
		if (normals.size() > 0) {
			add_normal(transform.basis.xform(normals[i]));
		}

		if (normals.size() > 0) {
			Vector2 uv = uvs[i];

			uv.x = uv_rect.size.width * uv.x + uv_rect.position.x;
			uv.y = uv_rect.size.height * uv.y + uv_rect.position.y;

			add_uv(uv);
		}

		if (colors.size() > 0) {
			add_color(colors[i]);
		}

		add_vertex(transform.xform(vertices[i]));
	}

	int orig_indices_count = _indices.size();
	_indices.resize(_indices.size() + indices.size());

	for (int i = 0; i < indices.size(); ++i) {
		_indices.set(orig_indices_count + i, orig_vert_size + indices[i]);
	}
}

void PropMesher::add_mesh_data_resource_transform_colored(Ref<MeshDataResource> mesh, const Transform transform, const PoolColorArray &colors, const Rect2 uv_rect) {
	if (mesh->get_array().size() == 0) {
		return;
	}

	const Array &arr = mesh->get_array();

	PoolVector3Array vertices = arr[Mesh::ARRAY_VERTEX];
	PoolVector3Array normals = arr[Mesh::ARRAY_NORMAL];
	PoolVector2Array uvs = arr[Mesh::ARRAY_TEX_UV];
	PoolIntArray indices = arr[Mesh::ARRAY_INDEX];

	if (vertices.size() == 0) {
		return;
	}

	int orig_vert_size = _vertices.size();

	for (int i = 0; i < vertices.size(); ++i) {
		if (normals.size() > 0) {
			add_normal(transform.basis.xform(normals[i]));
		}

		if (normals.size() > 0) {
			Vector2 uv = uvs[i];

			uv.x = uv_rect.size.width * uv.x + uv_rect.position.x;
			uv.y = uv_rect.size.height * uv.y + uv_rect.position.y;

			add_uv(uv);
		}

		if (colors.size() > 0) {
			add_color(colors[i]);
		}

		add_vertex(transform.xform(vertices[i]));
	}

	int orig_indices_count = _indices.size();
	_indices.resize(_indices.size() + indices.size());

	for (int i = 0; i < indices.size(); ++i) {
		_indices.set(orig_indices_count + i, orig_vert_size + indices[i]);
	}
}
#endif

//Data Management functions
void PropMesher::generate_ao() {
	/*
	ERR_FAIL_COND(!_chunk.is_valid());

	int data_size_x = _chunk->get_data_size_x();
	int data_size_z = _chunk->get_data_size_z();

	ERR_FAIL_COND(data_size_x == 0 || data_size_z == 0);

	int margin_start = _chunk->get_margin_start();
	int margin_end = _chunk->get_margin_end();

	int ssize_x = _chunk->get_size_x();
	int ssize_z = _chunk->get_size_z();

	int size_x = ssize_x + margin_end;
	int size_z = ssize_z + margin_end;

	for (int z = margin_start - 1; z < size_z - 1; ++z) {
		for (int x = margin_start - 1; x < size_x - 1; ++x) {
			int current = _chunk->get_voxel(x, z, TerraChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);

			int sum = _chunk->get_voxel(x + 1, z, TerraChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);
			sum += _chunk->get_voxel(x - 1, z, TerraChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);
			sum += _chunk->get_voxel(x, z + 1, TerraChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);
			sum += _chunk->get_voxel(x, z - 1, TerraChunkDefault::DEFAULT_CHANNEL_ISOLEVEL);

			sum /= 6;

			sum -= current;

			if (sum < 0)
				sum = 0;

			_chunk->set_voxel(sum, x, z, TerraChunkDefault::DEFAULT_CHANNEL_AO);
		}
	}*/
}

float PropMesher::get_random_ao(const Vector3 &position) {
	float val = _noise->get_noise_3d(position.x, position.y, position.z);

	val *= _rao_scale_factor;

	if (val > 1) {
		val = 1;
	}

	if (val < 0) {
		val = -val;
	}

	return val;
}

Color PropMesher::get_light_color_at(const Vector3 &position, const Vector3 &normal) {
	Vector3 v_lightDiffuse;

	//calculate the lights value
	for (int i = 0; i < _lights.size(); ++i) {
		Ref<PropLight> light = _lights.get(i);

		Vector3 lightDir = light->get_position() - position;

		float dist2 = lightDir.dot(lightDir);
		//inverse sqrt
		lightDir *= (1.0 / sqrt(dist2));

		float NdotL = normal.dot(lightDir);

		if (NdotL > 1.0) {
			NdotL = 1.0;
		} else if (NdotL < 0.0) {
			NdotL = 0.0;
		}

		Color cc = light->get_color();
		Vector3 cv(cc.r, cc.g, cc.b);

		Vector3 value = cv * (NdotL / (1.0 + dist2));

		value *= light->get_size();
		v_lightDiffuse += value;

		/*
					float dist2 = Mathf.Clamp(Vector3.Distance(transformedLights[i], vertices), 0f, 15f);
					dist2 /= 35f;

					Vector3 value = Vector3.one;
					value *= ((float) lights[i].Strength) / 255f;
					value *= (1 - dist2);
					v_lightDiffuse += value;*/
	}

	return Color(v_lightDiffuse.x, v_lightDiffuse.y, v_lightDiffuse.z);
}

void PropMesher::add_mesher(const Ref<PropMesher> &mesher) {
	call("_add_mesher", mesher);
}
void PropMesher::_add_mesher(const Ref<PropMesher> &mesher) {
	int orig_size = _vertices.size();

	_vertices.append_array(mesher->_vertices);

	int s = mesher->_indices.size();

	if (s == 0) {
		return;
	}

	int orig_indices_size = _indices.size();

	_indices.resize(_indices.size() + s);
	for (int i = 0; i < s; ++i) {
		_indices.set(i + orig_indices_size, mesher->_indices[i] + orig_size);
	}
}

void PropMesher::add_light(const Ref<PropLight> &light) {
	_lights.push_back(light);
}
void PropMesher::clear_lights() {
	_lights.clear();
}

PoolVector<Vector3> PropMesher::build_collider() const {
	PoolVector<Vector3> face_points;

	if (_vertices.size() == 0) {
		return face_points;
	}

	if (_indices.size() == 0) {
		int len = (_vertices.size() / 4);

		for (int i = 0; i < len; ++i) {
			face_points.push_back(_vertices.get(i * 4).vertex);
			face_points.push_back(_vertices.get((i * 4) + 2).vertex);
			face_points.push_back(_vertices.get((i * 4) + 1).vertex);

			face_points.push_back(_vertices.get(i * 4).vertex);
			face_points.push_back(_vertices.get((i * 4) + 3).vertex);
			face_points.push_back(_vertices.get((i * 4) + 2).vertex);
		}

		return face_points;
	}

	face_points.resize(_indices.size());
	for (int i = 0; i < face_points.size(); i++) {
		face_points.set(i, _vertices.get(_indices.get(i)).vertex);
	}

	return face_points;
}

void PropMesher::bake_colors() {
	if ((get_build_flags() & PropMesher::BUILD_FLAG_USE_LIGHTING) == 0) {
		return;
	}

	bool rao = (get_build_flags() & PropMesher::BUILD_FLAG_USE_RAO) != 0;
	bool lights = (get_build_flags() & PropMesher::BUILD_FLAG_BAKE_LIGHTS) != 0;

	if (rao && lights) {
		bake_colors_lights_rao();
		return;
	}

	if (rao) {
		bake_colors_rao();
		return;
	}

	if (lights) {
		bake_colors_lights();
		return;
	}
}

void PropMesher::bake_colors_rao() {
	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex vertex = _vertices[i];
		Vector3 vert = vertex.vertex;

		Color light = Color(_base_light_value, _base_light_value, _base_light_value);

		float rao = get_random_ao(vert) * _ao_strength;

		light.r -= rao;
		light.g -= rao;
		light.b -= rao;

		light.r = CLAMP(light.r, 0, 1.0);
		light.g = CLAMP(light.g, 0, 1.0);
		light.b = CLAMP(light.b, 0, 1.0);

		Color c = vertex.color;
		light.a = c.a;
		vertex.color = light;

		_vertices.set(i, vertex);
	}
}
void PropMesher::bake_colors_lights_rao() {
	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex vertex = _vertices[i];
		Vector3 vert = vertex.vertex;

		Color light = get_light_color_at(vert, vertex.normal);

		float rao = get_random_ao(vert) * _ao_strength;

		light.r += _base_light_value;
		light.g += _base_light_value;
		light.b += _base_light_value;

		light.r -= rao;
		light.g -= rao;
		light.b -= rao;

		light.r = CLAMP(light.r, 0, 1.0);
		light.g = CLAMP(light.g, 0, 1.0);
		light.b = CLAMP(light.b, 0, 1.0);

		Color c = vertex.color;
		light.a = c.a;
		vertex.color = light;

		_vertices.set(i, vertex);
	}
}
void PropMesher::bake_colors_lights() {
	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex vertex = _vertices[i];
		Vector3 vert = vertex.vertex;

		Color light = get_light_color_at(vert, vertex.normal);

		light.r += _base_light_value;
		light.g += _base_light_value;
		light.b += _base_light_value;

		light.r = CLAMP(light.r, 0, 1.0);
		light.g = CLAMP(light.g, 0, 1.0);
		light.b = CLAMP(light.b, 0, 1.0);

		Color c = vertex.color;
		light.a = c.a;
		vertex.color = light;

		_vertices.set(i, vertex);
	}
}

#ifdef MODULE_TERRAMAN_ENABLED
void PropMesher::bake_lights(MeshInstance *node, Vector<Ref<TerrainLight>> &lights) {
	ERR_FAIL_COND(node == NULL);

	Color darkColor(0, 0, 0, 1);

	for (int v = 0; v < _vertices.size(); ++v) {
		Vertex vertexv = _vertices.get(v);
		Vector3 vet = vertexv.vertex;
		Vector3 vertex = node->to_global(vet);

		//grab normal
		Vector3 normal = vertexv.normal;

		Vector3 v_lightDiffuse;

		//calculate the lights value
		for (int i = 0; i < lights.size(); ++i) {
			Ref<TerrainLight> light = lights.get(i);

			Vector3 lightDir = light->get_world_position() - vertex;

			float dist2 = lightDir.dot(lightDir);
			//inverse sqrt
			lightDir *= (1.0 / sqrt(dist2));

			float NdotL = normal.dot(lightDir);

			if (NdotL > 1.0) {
				NdotL = 1.0;
			} else if (NdotL < 0.0) {
				NdotL = 0.0;
			}

			Color cc = light->get_color();
			Vector3 cv(cc.r, cc.g, cc.b);

			Vector3 value = cv * (NdotL / (1.0 + dist2));

			value *= light->get_size();
			v_lightDiffuse += value;

			/*
					float dist2 = Mathf.Clamp(Vector3.Distance(transformedLights[i], vertices), 0f, 15f);
					dist2 /= 35f;

					Vector3 value = Vector3.one;
					value *= ((float) lights[i].Strength) / 255f;
					value *= (1 - dist2);
					v_lightDiffuse += value;*/
		}

		Color f = vertexv.color;
		//Color f = darkColor;

		Vector3 cv2(f.r, f.g, f.b);
		cv2 += v_lightDiffuse;

		if (cv2.x > 1) {
			cv2.x = 1;
		}

		if (cv2.y > 1) {
			cv2.y = 1;
		}

		if (cv2.y > 1) {
			cv2.y = 1;
		}

		// cv2.x = Mathf.Clamp(cv2.x, 0f, 1f);
		//cv2.y = Mathf.Clamp(cv2.y, 0f, 1f);
		// cv2.z = Mathf.Clamp(cv2.z, 0f, 1f);

		f.r = cv2.x;
		f.g = cv2.y;
		f.b = cv2.z;

		//f.r = v_lightDiffuse.x;
		//f.g = v_lightDiffuse.y;
		//f.b = v_lightDiffuse.z;

		vertexv.color = f;
		_vertices.set(v, vertexv);
	}

	//	for (int i = 0; i < _colors->size(); ++i) {
	//		print_error(_colors->get(i));
	//	}
}
#endif

PoolVector<Vector3> PropMesher::get_vertices() const {
	PoolVector<Vector3> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).vertex);
	}

	return arr;
}

void PropMesher::set_vertices(const PoolVector<Vector3> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.normal = values[i];

		_vertices.set(i, v);
	}
}

int PropMesher::get_vertex_count() const {
	return _vertices.size();
}

void PropMesher::add_vertex(const Vector3 &vertex) {
	Vertex vtx;
	vtx.vertex = vertex;
	vtx.color = _last_color;
	vtx.normal = _last_normal;
	vtx.uv = _last_uv;
	vtx.uv2 = _last_uv2;
	// Todo?
	//	vtx.weights = _last_weights;
	//	vtx.bones = _last_bones;
	//	vtx.tangent = _last_tangent.normal;
	//	vtx.binormal = _last_normal.cross(_last_tangent.normal).normalized() * _last_tangent.d;

	_vertices.push_back(vtx);
}

Vector3 PropMesher::get_vertex(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector3());

	return _vertices.get(idx).vertex;
}

void PropMesher::remove_vertex(const int idx) {
	ERR_FAIL_INDEX(idx, _vertices.size());

	_vertices.remove(idx);
}

PoolVector<Vector3> PropMesher::get_normals() const {
	PoolVector<Vector3> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).normal);
	}

	return arr;
}

void PropMesher::set_normals(const PoolVector<Vector3> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.normal = values[i];

		_vertices.set(i, v);
	}
}

void PropMesher::add_normal(const Vector3 &normal) {
	_last_normal = normal;
}

Vector3 PropMesher::get_normal(int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector3());

	return _vertices.get(idx).normal;
}

PoolVector<Color> PropMesher::get_colors() const {
	PoolVector<Color> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).color);
	}

	return arr;
}

void PropMesher::set_colors(const PoolVector<Color> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.color = values[i];

		_vertices.set(i, v);
	}
}

void PropMesher::add_color(const Color &color) {
	_last_color = color;
}

Color PropMesher::get_color(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Color());

	return _vertices.get(idx).color;
}

PoolVector<Vector2> PropMesher::get_uvs() const {
	PoolVector<Vector2> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).uv);
	}

	return arr;
}

void PropMesher::set_uvs(const PoolVector<Vector2> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.uv = values[i];

		_vertices.set(i, v);
	}
}

void PropMesher::add_uv(const Vector2 &uv) {
	_last_uv = uv;
}

Vector2 PropMesher::get_uv(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector2());

	return _vertices.get(idx).uv;
}

PoolVector<Vector2> PropMesher::get_uv2s() const {
	PoolVector<Vector2> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).uv2);
	}

	return arr;
}

void PropMesher::set_uv2s(const PoolVector<Vector2> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.uv2 = values[i];

		_vertices.set(i, v);
	}
}

void PropMesher::add_uv2(const Vector2 &uv) {
	_last_uv2 = uv;
}

Vector2 PropMesher::get_uv2(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector2());

	return _vertices.get(idx).uv2;
}

PoolVector<int> PropMesher::get_indices() const {
	return _indices;
}

void PropMesher::set_indices(const PoolVector<int> &values) {
	_indices = values;
}

int PropMesher::get_indices_count() const {
	return _indices.size();
}

void PropMesher::add_indices(const int index) {
	_indices.push_back(index);
}

int PropMesher::get_index(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _indices.size(), 0);

	return _indices.get(idx);
}

void PropMesher::remove_index(const int idx) {
	ERR_FAIL_INDEX(idx, _vertices.size());

	_indices.remove(idx);
}

PropMesher::PropMesher() {
	_mesher_index = 0;
	_voxel_scale = 1;
	_ao_strength = 0.25;
	_base_light_value = 0.5;
	_uv_margin = Rect2(0, 0, 1, 1);
	_format = 0;
	_channel_index_type = 0;
	_channel_index_isolevel = 0;
	_texture_scale = 1;

	_build_flags = 0;

	_format = RenderingServer::ARRAY_FORMAT_NORMAL | RenderingServer::ARRAY_FORMAT_TEX_UV;

	_noise.instance();
	//todo add properties for these if needed
	_noise->set_octaves(4);
	_noise->set_period(30);
	_noise->set_persistence(0.3);

	_rao_scale_factor = 0.6;
	_rao_seed = 2134;
}

PropMesher::~PropMesher() {
}

void PropMesher::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_channel_index_type"), &PropMesher::get_channel_index_type);
	ClassDB::bind_method(D_METHOD("set_channel_index_type", "value"), &PropMesher::set_channel_index_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_index_type"), "set_channel_index_type", "get_channel_index_type");

	ClassDB::bind_method(D_METHOD("get_channel_index_isolevel"), &PropMesher::get_channel_index_isolevel);
	ClassDB::bind_method(D_METHOD("set_channel_index_isolevel", "value"), &PropMesher::set_channel_index_isolevel);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_index_isolevel"), "set_channel_index_isolevel", "get_channel_index_isolevel");

	ClassDB::bind_method(D_METHOD("get_mesher_index"), &PropMesher::get_mesher_index);
	ClassDB::bind_method(D_METHOD("set_mesher_index", "value"), &PropMesher::set_mesher_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mesher_index"), "set_mesher_index", "get_mesher_index");

	ClassDB::bind_method(D_METHOD("get_format"), &PropMesher::get_format);
	ClassDB::bind_method(D_METHOD("set_format", "value"), &PropMesher::set_format);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "format"), "set_format", "get_format");

	ClassDB::bind_method(D_METHOD("get_texture_scale"), &PropMesher::get_texture_scale);
	ClassDB::bind_method(D_METHOD("set_texture_scale", "value"), &PropMesher::set_texture_scale);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_scale"), "set_texture_scale", "get_texture_scale");

	ClassDB::bind_method(D_METHOD("get_material"), &PropMesher::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &PropMesher::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &PropMesher::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &PropMesher::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_ao_strength"), &PropMesher::get_ao_strength);
	ClassDB::bind_method(D_METHOD("set_ao_strength", "value"), &PropMesher::set_ao_strength);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ao_strength"), "set_ao_strength", "get_ao_strength");

	ClassDB::bind_method(D_METHOD("get_base_light_value"), &PropMesher::get_base_light_value);
	ClassDB::bind_method(D_METHOD("set_base_light_value", "value"), &PropMesher::set_base_light_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "base_light_value"), "set_base_light_value", "get_base_light_value");

	ClassDB::bind_method(D_METHOD("get_uv_margin"), &PropMesher::get_uv_margin);
	ClassDB::bind_method(D_METHOD("set_uv_margin", "value"), &PropMesher::set_uv_margin);
	ADD_PROPERTY(PropertyInfo(Variant::RECT2, "uv_margin"), "set_uv_margin", "get_uv_margin");

	ClassDB::bind_method(D_METHOD("get_build_flags"), &PropMesher::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &PropMesher::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, PropMesher::BINDING_STRING_BUILD_FLAGS), "set_build_flags", "get_build_flags");

	ClassDB::bind_method(D_METHOD("add_tiled_wall_simple", "width", "height", "transform", "tiled_wall_data", "cache"), &PropMesher::add_tiled_wall_simple);
	//ClassDB::bind_method(D_METHOD("add_tiled_wall_mesh_rect_simple", "x", "y", "y_size", "transform", "texture_rect", "texture_scale"), &PropMesher::add_tiled_wall_mesh_rect_simple);
	ClassDB::bind_method(D_METHOD("transform_uv", "uv", "rect"), &PropMesher::transform_uv);
	ClassDB::bind_method(D_METHOD("transform_uv_scaled", "uv", "rect", "x", "y", "texture_scale"), &PropMesher::transform_uv_scaled);

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource", "mesh", "position", "rotation", "scale", "uv_rect"), &PropMesher::add_mesh_data_resource, DEFVAL(Rect2(0, 0, 1, 1)), DEFVAL(Vector3(1.0, 1.0, 1.0)), DEFVAL(Vector3()), DEFVAL(Vector3()));
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource_transform", "mesh", "transform", "uv_rect"), &PropMesher::add_mesh_data_resource_transform, DEFVAL(Rect2(0, 0, 1, 1)));
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource_transform_colored", "mesh", "transform", "colors", "uv_rect"), &PropMesher::add_mesh_data_resource_transform_colored, DEFVAL(Rect2(0, 0, 1, 1)));
#endif

	ClassDB::bind_method(D_METHOD("generate_ao"), &PropMesher::generate_ao);
	ClassDB::bind_method(D_METHOD("get_random_ao", "position"), &PropMesher::get_random_ao);

	BIND_VMETHOD(MethodInfo("_add_mesher", PropertyInfo(Variant::OBJECT, "mesher", PROPERTY_HINT_RESOURCE_TYPE, "PropMesher")));
	ClassDB::bind_method(D_METHOD("add_mesher", "mesher"), &PropMesher::add_mesher);
	ClassDB::bind_method(D_METHOD("_add_mesher", "mesher"), &PropMesher::_add_mesher);

	ClassDB::bind_method(D_METHOD("add_light", "light"), &PropMesher::add_light);
	ClassDB::bind_method(D_METHOD("clear_lights"), &PropMesher::clear_lights);

	ClassDB::bind_method(D_METHOD("get_vertices"), &PropMesher::get_vertices);
	ClassDB::bind_method(D_METHOD("set_vertices", "values"), &PropMesher::set_vertices);
	ClassDB::bind_method(D_METHOD("get_vertex_count"), &PropMesher::get_vertex_count);
	ClassDB::bind_method(D_METHOD("get_vertex", "idx"), &PropMesher::get_vertex);
	ClassDB::bind_method(D_METHOD("remove_vertex", "idx"), &PropMesher::remove_vertex);
	ClassDB::bind_method(D_METHOD("add_vertex", "vertex"), &PropMesher::add_vertex);

	ClassDB::bind_method(D_METHOD("get_normals"), &PropMesher::get_normals);
	ClassDB::bind_method(D_METHOD("set_normals", "values"), &PropMesher::set_normals);
	ClassDB::bind_method(D_METHOD("get_normal", "idx"), &PropMesher::get_normal);
	ClassDB::bind_method(D_METHOD("add_normal", "normal"), &PropMesher::add_normal);

	ClassDB::bind_method(D_METHOD("get_colors"), &PropMesher::get_colors);
	ClassDB::bind_method(D_METHOD("set_colors", "values"), &PropMesher::set_colors);
	ClassDB::bind_method(D_METHOD("get_color", "idx"), &PropMesher::get_color);
	ClassDB::bind_method(D_METHOD("add_color", "color"), &PropMesher::add_color);

	ClassDB::bind_method(D_METHOD("get_uvs"), &PropMesher::get_uvs);
	ClassDB::bind_method(D_METHOD("set_uvs", "values"), &PropMesher::set_uvs);
	ClassDB::bind_method(D_METHOD("get_uv", "idx"), &PropMesher::get_uv);
	ClassDB::bind_method(D_METHOD("add_uv", "uv"), &PropMesher::add_uv);

	ClassDB::bind_method(D_METHOD("get_uv2s"), &PropMesher::get_uv2s);
	ClassDB::bind_method(D_METHOD("set_uv2s", "values"), &PropMesher::set_uv2s);
	ClassDB::bind_method(D_METHOD("get_uv2", "idx"), &PropMesher::get_uv2);
	ClassDB::bind_method(D_METHOD("add_uv2", "uv"), &PropMesher::add_uv2);

	ClassDB::bind_method(D_METHOD("get_indices"), &PropMesher::get_indices);
	ClassDB::bind_method(D_METHOD("set_indices", "values"), &PropMesher::set_indices);
	ClassDB::bind_method(D_METHOD("get_indices_count"), &PropMesher::get_indices_count);
	ClassDB::bind_method(D_METHOD("get_index", "idx"), &PropMesher::get_index);
	ClassDB::bind_method(D_METHOD("remove_index", "idx"), &PropMesher::remove_index);
	ClassDB::bind_method(D_METHOD("add_indices", "indice"), &PropMesher::add_indices);

	ClassDB::bind_method(D_METHOD("reset"), &PropMesher::reset);

	//ClassDB::bind_method(D_METHOD("calculate_vertex_ambient_occlusion", "meshinstance_path", "radius", "intensity", "sampleCount"), &PropMesher::calculate_vertex_ambient_occlusion_path);

	ClassDB::bind_method(D_METHOD("build_mesh"), &PropMesher::build_mesh);
	ClassDB::bind_method(D_METHOD("build_mesh_into", "mesh_rid"), &PropMesher::build_mesh_into);
	ClassDB::bind_method(D_METHOD("build_collider"), &PropMesher::build_collider);

	ClassDB::bind_method(D_METHOD("bake_colors"), &PropMesher::bake_colors);

	ClassDB::bind_method(D_METHOD("generate_normals", "flip"), &PropMesher::generate_normals, DEFVAL(false));

	ClassDB::bind_method(D_METHOD("remove_doubles"), &PropMesher::remove_doubles);
	ClassDB::bind_method(D_METHOD("remove_doubles_hashed"), &PropMesher::remove_doubles_hashed);
}
