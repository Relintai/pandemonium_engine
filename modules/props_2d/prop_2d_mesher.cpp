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

#include "prop_2d_mesher.h"

#include "lights/prop_2d_light.h"
#include "modules/opensimplex/open_simplex_noise.h"

#include "./singleton/prop_2d_cache.h"
#include "material_cache/prop_2d_material_cache.h"
#include "tiled_wall/tiled_wall_2d_data.h"

#include "modules/modules_enabled.gen.h"

const String Prop2DMesher::BINDING_STRING_BUILD_FLAGS = "Use Lighting,Use AO,Use RAO,Bake Lights";

bool Prop2DMesher::Vertex::operator==(const Vertex &p_vertex) const {
	if (vertex != p_vertex.vertex)
		return false;

	if (uv != p_vertex.uv)
		return false;

	if (color != p_vertex.color)
		return false;

	if (bones.size() != p_vertex.bones.size())
		return false;

	for (int i = 0; i < bones.size(); i++) {
		if (bones[i] != p_vertex.bones[i])
			return false;
	}

	for (int i = 0; i < weights.size(); i++) {
		if (weights[i] != p_vertex.weights[i])
			return false;
	}

	return true;
}

uint32_t Prop2DMesher::VertexHasher::hash(const Vertex &p_vtx) {
	uint32_t h = hash_djb2_buffer((const uint8_t *)&p_vtx.vertex, sizeof(real_t) * 3);
	h = hash_djb2_buffer((const uint8_t *)&p_vtx.uv, sizeof(real_t) * 2, h);
	h = hash_djb2_buffer((const uint8_t *)&p_vtx.color, sizeof(real_t) * 4, h);
	h = hash_djb2_buffer((const uint8_t *)p_vtx.bones.ptr(), p_vtx.bones.size() * sizeof(int), h);
	h = hash_djb2_buffer((const uint8_t *)p_vtx.weights.ptr(), p_vtx.weights.size() * sizeof(float), h);
	return h;
}

int Prop2DMesher::get_format() const {
	return _format;
}
void Prop2DMesher::set_format(const int value) {
	_format = value;
}

int Prop2DMesher::get_texture_scale() const {
	return _texture_scale;
}
void Prop2DMesher::set_texture_scale(const int value) {
	_texture_scale = value;
}

Ref<Material> Prop2DMesher::get_material() {
	return _material;
}
void Prop2DMesher::set_material(const Ref<Material> &material) {
	_material = material;
}

float Prop2DMesher::get_pixels_per_unit() const {
	return _pixels_per_unit;
}
void Prop2DMesher::set_pixels_per_unit(const float value) {
	_pixels_per_unit = value;
}

float Prop2DMesher::get_ao_strength() const {
	return _ao_strength;
}
void Prop2DMesher::set_ao_strength(float value) {
	_ao_strength = value;
}

float Prop2DMesher::get_base_light_value() const {
	return _base_light_value;
}
void Prop2DMesher::set_base_light_value(float value) {
	_base_light_value = value;
}

float Prop2DMesher::get_voxel_scale() const {
	return _voxel_scale;
}
void Prop2DMesher::set_voxel_scale(const float voxel_scale) {
	_voxel_scale = voxel_scale;
}

Rect2 Prop2DMesher::get_uv_margin() const {
	return _uv_margin;
}
void Prop2DMesher::set_uv_margin(const Rect2 margin) {
	_uv_margin = margin;
}

_FORCE_INLINE_ int Prop2DMesher::get_build_flags() const {
	return _build_flags;
}
_FORCE_INLINE_ void Prop2DMesher::set_build_flags(const int flags) {
	_build_flags = flags;

	if ((_build_flags & Prop2DMesher::BUILD_FLAG_USE_LIGHTING) != 0) {
		_format |= RenderingServer::ARRAY_FORMAT_COLOR;
	} else {
		_format ^= RenderingServer::ARRAY_FORMAT_COLOR;
	}
}

Array Prop2DMesher::build_mesh() {
	Array a;
	a.resize(RenderingServer::ARRAY_MAX);

	if (_vertices.size() == 0) {
		//Nothing to do
		return a;
	}

	{
		PoolVector<Vector2> array;
		array.resize(_vertices.size());
		PoolVector<Vector2>::Write w = array.write();

		for (int i = 0; i < _vertices.size(); ++i) {
			w[i] = _vertices[i].vertex;
		}

		w.release();

		a[RenderingServer::ARRAY_VERTEX] = array;
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

void Prop2DMesher::build_mesh_into(RID mesh) {
	ERR_FAIL_COND(mesh == RID());

	RS::get_singleton()->mesh_clear(mesh);

	if (_vertices.size() == 0) {
		//Nothing to do
		return;
	}

	Array arr = build_mesh();

	RS::get_singleton()->mesh_add_surface_from_arrays(mesh, RenderingServer::PRIMITIVE_TRIANGLES, arr);

	if (_material.is_valid())
		RS::get_singleton()->mesh_surface_set_material(mesh, 0, _material->get_rid());
}

void Prop2DMesher::remove_doubles() {
	if (_vertices.size() == 0)
		return;

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
}

//lot faster that normal remove_doubles, but false positives can happen curtesy of hash collisions
void Prop2DMesher::remove_doubles_hashed() {
	if (_vertices.size() == 0)
		return;

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
}

void Prop2DMesher::reset() {
	_vertices.resize(0);
	_indices.resize(0);

	_last_color = Color();
	_last_uv = Vector2();
	_last_bones.clear();
	_last_weights.clear();
}

void Prop2DMesher::add_tiled_wall_simple(const int width, const int height, const Transform2D &transform, const Ref<TiledWall2DData> &tiled_wall_data, Ref<Prop2DMaterialCache> cache) {
	ERR_FAIL_COND(!tiled_wall_data.is_valid());
	ERR_FAIL_COND(!cache.is_valid());
	ERR_FAIL_COND(width < 0);
	ERR_FAIL_COND(height < 0);

	if (tiled_wall_data->get_texture_count() == 0) {
		return;
	}

	float flavour_chance = tiled_wall_data->get_flavour_chance();

	//collect rects
	Vector<Rect2> normal_rects;
	Vector<Rect2> flavour_rects;
	Vector<Vector2> normal_sizes;
	Vector<Vector2> flavour_sizes;

	for (int i = 0; i < tiled_wall_data->get_texture_count(); ++i) {
		const Ref<Texture> &t = tiled_wall_data->get_texture(i);

		if (t.is_valid()) {
			normal_rects.push_back(cache->texture_get_uv_rect(t));
			normal_sizes.push_back(cache->texture_get_rect(t).size);
		}
	}

	for (int i = 0; i < tiled_wall_data->get_flavour_texture_count(); ++i) {
		const Ref<Texture> &t = tiled_wall_data->get_flavour_texture(i);

		if (t.is_valid()) {
			flavour_rects.push_back(cache->texture_get_uv_rect(t));
			flavour_sizes.push_back(cache->texture_get_rect(t).size);
		}
	}

	//fallback
	if (normal_rects.size() == 0) {
		normal_rects.push_back(Rect2(0, 0, 1, 1));
		normal_sizes.push_back(Vector2(1, 1));
	}

	TiledWall2DData::TiledWall2DTilingType tiling_type = tiled_wall_data->get_tiling_type();

	if (tiling_type == TiledWall2DData::TILED_WALL_TILING_TYPE_NONE) {
		Rect2 r = normal_rects[0];
		Vector2 rs = normal_sizes[0];

		if (flavour_rects.size() == 0) {
			//no flavours
			for (int x = 0; x < width; ++x) {
				for (int y = 0; y < height; ++y) {
					add_tiled_wall_mesh_rect_simple(x, y, transform, r, rs);
				}
			}
		} else {
			//has flavours
			for (int x = 0; x < width; ++x) {
				for (int y = 0; y < height; ++y) {
					if (Math::randf() > flavour_chance) {
						add_tiled_wall_mesh_rect_simple(x, y, transform, r, rs);
					} else {
						int ind = Math::rand() % flavour_rects.size();

						add_tiled_wall_mesh_rect_simple(x, y, transform, flavour_rects[ind], flavour_sizes[ind]);
					}
				}
			}
		}
	} else if (tiling_type == TiledWall2DData::TILED_WALL_TILING_TYPE_HORIZONTAL) {
		Rect2 r;
		Vector2 rs;

		if (flavour_rects.size() == 0) {
			//no flavours
			for (int x = 0; x < width; ++x) {
				int ind = x % normal_rects.size();
				r = normal_rects[ind];
				rs = normal_sizes[ind];

				for (int y = 0; y < height; ++y) {
					add_tiled_wall_mesh_rect_simple(x, y, transform, r, rs);
				}
			}
		} else {
			//has flavours
			for (int x = 0; x < width; ++x) {
				int ind = x % normal_rects.size();
				r = normal_rects[ind];
				rs = normal_sizes[ind];

				for (int y = 0; y < height; ++y) {
					if (Math::randf() > flavour_chance) {
						add_tiled_wall_mesh_rect_simple(x, y, transform, r, rs);
					} else {
						int find = Math::rand() % flavour_rects.size();
						add_tiled_wall_mesh_rect_simple(x, y, transform, flavour_rects[find], flavour_sizes[find]);
					}
				}
			}
		}
	} else if (tiling_type == TiledWall2DData::TILED_WALL_TILING_TYPE_VERTICAL) {
		Rect2 r;
		Vector2 rs;

		if (flavour_rects.size() == 0) {
			//no flavours
			for (int x = 0; x < width; ++x) {
				for (int y = 0; y < height; ++y) {
					int ind = y % normal_rects.size();
					r = normal_rects[ind];
					rs = normal_sizes[ind];

					add_tiled_wall_mesh_rect_simple(x, y, transform, r, rs);
				}
			}
		} else {
			//has flavours
			for (int x = 0; x < width; ++x) {
				for (int y = 0; y < height; ++y) {
					int ind = y % normal_rects.size();
					r = normal_rects[ind];
					rs = normal_sizes[ind];

					if (Math::randf() > flavour_chance) {
						add_tiled_wall_mesh_rect_simple(x, y, transform, r, rs);
					} else {
						int find = Math::rand() % flavour_rects.size();
						add_tiled_wall_mesh_rect_simple(x, y, transform, flavour_rects[find], flavour_sizes[find]);
					}
				}
			}
		}
	} else if (tiling_type == TiledWall2DData::TILED_WALL_TILING_TYPE_BOTH) {
		Rect2 r;
		Vector2 rs;

		if (flavour_rects.size() == 0) {
			//no flavours
			for (int x = 0; x < width; ++x) {
				for (int y = 0; y < height; ++y) {
					int ind = (x + y) % normal_rects.size();
					r = normal_rects[ind];
					rs = normal_sizes[ind];

					add_tiled_wall_mesh_rect_simple(x, y, transform, r, rs);
				}
			}
		} else {
			//has flavours
			for (int x = 0; x < width; ++x) {
				for (int y = 0; y < height; ++y) {
					int ind = (x + y) % normal_rects.size();
					r = normal_rects[ind];
					rs = normal_sizes[ind];

					if (Math::randf() > flavour_chance) {
						add_tiled_wall_mesh_rect_simple(x, y, transform, r, rs);
					} else {
						int find = Math::rand() % flavour_rects.size();
						add_tiled_wall_mesh_rect_simple(x, y, transform, flavour_rects[find], flavour_sizes[find]);
					}
				}
			}
		}
	}
}

void Prop2DMesher::add_tiled_wall_mesh_rect_simple(const int x, const int y, const Transform2D &transform, const Rect2 &texture_uv_rect, const Vector2 &size) {
	int vc = get_vertex_count();

	int sx = x * size.x;
	int sy = y * size.y;

	//x + 1, y
	add_uv(transform_uv(Vector2(1, 0), texture_uv_rect));
	add_vertex(transform.xform(Vector2(sx + size.x, sy)));

	//x, y
	add_uv(transform_uv(Vector2(0, 0), texture_uv_rect));
	add_vertex(transform.xform(Vector2(sx, sy)));

	//x, y + 1
	add_uv(transform_uv(Vector2(0, 1), texture_uv_rect));
	add_vertex(transform.xform(Vector2(sx, sy + size.y)));

	//x + 1, y + 1
	add_uv(transform_uv(Vector2(1, 1), texture_uv_rect));
	add_vertex(transform.xform(Vector2(sx + size.x, sy + size.y)));

	add_indices(vc + 2);
	add_indices(vc + 1);
	add_indices(vc + 0);
	add_indices(vc + 3);
	add_indices(vc + 2);
	add_indices(vc + 0);
}

_FORCE_INLINE_ Vector2 Prop2DMesher::transform_uv(const Vector2 &uv, const Rect2 &rect) const {
	Vector2 ruv = uv;

	ruv.x *= rect.size.x;
	ruv.y *= rect.size.y;
	ruv.x += rect.position.x;
	ruv.y += rect.position.y;

	return ruv;
}

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
void Prop2DMesher::add_mesh_data_resource(Ref<MeshDataResource> mesh, const Vector3 position, const Vector3 rotation, const Vector3 scale, const Rect2 uv_rect) {
	Transform transform = Transform(Basis(rotation).scaled(scale), position);

	add_mesh_data_resource_transform(mesh, transform, uv_rect);
}

void Prop2DMesher::add_mesh_data_resource_transform(Ref<MeshDataResource> mesh, const Transform transform, const Rect2 uv_rect) {
	if (mesh->get_array().size() == 0)
		return;

	const Array &arr = mesh->get_array();

	PoolVector2Array vertices = arr[Mesh::ARRAY_VERTEX];
	PoolVector2Array uvs = arr[Mesh::ARRAY_TEX_UV];
	PoolColorArray colors = arr[Mesh::ARRAY_COLOR];
	PoolIntArray indices = arr[Mesh::ARRAY_INDEX];

	if (vertices.size() == 0)
		return;

	int orig_vert_size = _vertices.size();

	for (int i = 0; i < vertices.size(); ++i) {
		if (uvs.size() > 0) {
			Vector2 uv = uvs[i];

			uv.x = uv_rect.size.width * uv.x + uv_rect.position.x;
			uv.y = uv_rect.size.height * uv.y + uv_rect.position.y;

			add_uv(uv);
		}

		if (colors.size() > 0)
			add_color(colors[i]);

		//add_vertex(transform.xform(vertices[i]));
	}

	int orig_indices_count = _indices.size();
	_indices.resize(_indices.size() + indices.size());

	for (int i = 0; i < indices.size(); ++i) {
		_indices.set(orig_indices_count + i, orig_vert_size + indices[i]);
	}
}

void Prop2DMesher::add_mesh_data_resource_transform_colored(Ref<MeshDataResource> mesh, const Transform transform, const PoolColorArray &colors, const Rect2 uv_rect) {
	if (mesh->get_array().size() == 0)
		return;

	const Array &arr = mesh->get_array();

	PoolVector2Array vertices = arr[Mesh::ARRAY_VERTEX];
	PoolVector2Array uvs = arr[Mesh::ARRAY_TEX_UV];
	PoolIntArray indices = arr[Mesh::ARRAY_INDEX];

	if (vertices.size() == 0)
		return;

	int orig_vert_size = _vertices.size();

	for (int i = 0; i < vertices.size(); ++i) {
		if (uvs.size() > 0) {
			Vector2 uv = uvs[i];

			uv.x = uv_rect.size.width * uv.x + uv_rect.position.x;
			uv.y = uv_rect.size.height * uv.y + uv_rect.position.y;

			add_uv(uv);
		}

		if (colors.size() > 0)
			add_color(colors[i]);

		//add_vertex(transform.xform(vertices[i]));
	}

	int orig_indices_count = _indices.size();
	_indices.resize(_indices.size() + indices.size());

	for (int i = 0; i < indices.size(); ++i) {
		_indices.set(orig_indices_count + i, orig_vert_size + indices[i]);
	}
}
#endif

//Data Management functions
void Prop2DMesher::generate_ao() {
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

float Prop2DMesher::get_random_ao(const Vector2 &position) {
	float val = _noise->get_noise_2d(position.x, position.y);

	val *= _rao_scale_factor;

	if (val > 1)
		val = 1;

	if (val < 0)
		val = -val;

	return val;
}

Color Prop2DMesher::get_light_color_at(const Vector2 &position) {
	Vector3 v_lightDiffuse;

	//calculate the lights value
	for (int i = 0; i < _lights.size(); ++i) {
		Ref<Prop2DLight> light = _lights.get(i);

		Vector2 lightDir = light->get_position() - position;

		float dist2 = lightDir.dot(lightDir);
		//inverse sqrt
		lightDir *= (1.0 / sqrt(dist2));

		Color cc = light->get_color();
		Vector3 cv(cc.r, cc.g, cc.b);

		Vector3 value = cv * (_pixels_per_unit / (_pixels_per_unit + dist2));

		value *= light->get_size();
		v_lightDiffuse += value;
	}

	return Color(v_lightDiffuse.x, v_lightDiffuse.y, v_lightDiffuse.z);
}

void Prop2DMesher::add_mesher(const Ref<Prop2DMesher> &mesher) {
	call("_add_mesher", mesher);
}
void Prop2DMesher::_add_mesher(const Ref<Prop2DMesher> &mesher) {
	int orig_size = _vertices.size();

	_vertices.append_array(mesher->_vertices);

	int s = mesher->_indices.size();

	if (s == 0)
		return;

	int orig_indices_size = _indices.size();

	_indices.resize(_indices.size() + s);
	for (int i = 0; i < s; ++i) {
		_indices.set(i + orig_indices_size, mesher->_indices[i] + orig_size);
	}
}

void Prop2DMesher::add_light(const Ref<Prop2DLight> &light) {
	_lights.push_back(light);
}
void Prop2DMesher::clear_lights() {
	_lights.clear();
}

PoolVector<Vector2> Prop2DMesher::build_collider() const {
	PoolVector<Vector2> face_points;

	if (_vertices.size() == 0)
		return face_points;

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

Rect2 Prop2DMesher::calculate_rect() {
	int size = _vertices.size();

	if (size == 0) {
		return Rect2();
	}

	Rect2 rect;

	rect.position = _vertices[0].vertex;

	for (int i = 1; i < size; ++i) {
		rect.expand_to(_vertices[i].vertex);
	}

	return rect;
}

void Prop2DMesher::bake_colors() {
	if ((get_build_flags() & Prop2DMesher::BUILD_FLAG_USE_LIGHTING) == 0) {
		return;
	}

	bool rao = (get_build_flags() & Prop2DMesher::BUILD_FLAG_USE_RAO) != 0;
	bool lights = (get_build_flags() & Prop2DMesher::BUILD_FLAG_BAKE_LIGHTS) != 0;

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

void Prop2DMesher::bake_colors_rao() {
	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex vertex = _vertices[i];
		Vector2 vert = vertex.vertex;

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
void Prop2DMesher::bake_colors_lights_rao() {
	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex vertex = _vertices[i];
		Vector2 vert = vertex.vertex;

		Color light = get_light_color_at(vert);

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
void Prop2DMesher::bake_colors_lights() {
	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex vertex = _vertices[i];
		Vector2 vert = vertex.vertex;

		Color light = get_light_color_at(vert);

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

#ifdef MODULE_TERRAMAN_2D_ENABLED
void Prop2DMesher::bake_lights(MeshInstance *node, Vector<Ref<Terrain2DLight>> &lights) {
	/*
	ERR_FAIL_COND(node == NULL);

	Color darkColor(0, 0, 0, 1);

	for (int v = 0; v < _vertices.size(); ++v) {
		Vertex vertexv = _vertices.get(v);
		Vector3 vet = Vector3()//vertexv.vertex;
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
		}

		Color f = vertexv.color;
		//Color f = darkColor;

		Vector3 cv2(f.r, f.g, f.b);
		cv2 += v_lightDiffuse;

		if (cv2.x > 1)
			cv2.x = 1;

		if (cv2.y > 1)
			cv2.y = 1;

		if (cv2.y > 1)
			cv2.y = 1;

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
	*/
}
#endif

PoolVector<Vector2> Prop2DMesher::get_vertices() const {
	PoolVector<Vector2> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).vertex);
	}

	return arr;
}

void Prop2DMesher::set_vertices(const PoolVector<Vector2> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.vertex = values[i];

		_vertices.set(i, v);
	}
}

int Prop2DMesher::get_vertex_count() const {
	return _vertices.size();
}

void Prop2DMesher::add_vertex(const Vector2 &vertex) {
	Vertex vtx;
	vtx.vertex = vertex;
	vtx.color = _last_color;
	vtx.uv = _last_uv;
	// Todo?
	//	vtx.weights = _last_weights;
	//	vtx.bones = _last_bones;

	_vertices.push_back(vtx);
}

Vector2 Prop2DMesher::get_vertex(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector2());

	return _vertices.get(idx).vertex;
}

void Prop2DMesher::remove_vertex(const int idx) {
	ERR_FAIL_INDEX(idx, _vertices.size());

	_vertices.remove(idx);
}

PoolVector<Color> Prop2DMesher::get_colors() const {
	PoolVector<Color> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).color);
	}

	return arr;
}

void Prop2DMesher::set_colors(const PoolVector<Color> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.color = values[i];

		_vertices.set(i, v);
	}
}

void Prop2DMesher::add_color(const Color &color) {
	_last_color = color;
}

Color Prop2DMesher::get_color(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Color());

	return _vertices.get(idx).color;
}

PoolVector<Vector2> Prop2DMesher::get_uvs() const {
	PoolVector<Vector2> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).uv);
	}

	return arr;
}

void Prop2DMesher::set_uvs(const PoolVector<Vector2> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.uv = values[i];

		_vertices.set(i, v);
	}
}

void Prop2DMesher::add_uv(const Vector2 &uv) {
	_last_uv = uv;
}

Vector2 Prop2DMesher::get_uv(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector2());

	return _vertices.get(idx).uv;
}

PoolVector<int> Prop2DMesher::get_indices() const {
	return _indices;
}

void Prop2DMesher::set_indices(const PoolVector<int> &values) {
	_indices = values;
}

int Prop2DMesher::get_indices_count() const {
	return _indices.size();
}

void Prop2DMesher::add_indices(const int index) {
	_indices.push_back(index);
}

int Prop2DMesher::get_index(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _indices.size(), 0);

	return _indices.get(idx);
}

void Prop2DMesher::remove_index(const int idx) {
	ERR_FAIL_INDEX(idx, _indices.size());

	_indices.remove(idx);
}

Prop2DMesher::Prop2DMesher() {
	_voxel_scale = 1;
	_ao_strength = 0.25;
	_base_light_value = 0.5;
	_uv_margin = Rect2(0, 0, 1, 1);
	_format = 0;
	_texture_scale = 1;

	if (Prop2DCache::get_singleton()) {
		_pixels_per_unit = Prop2DCache::get_singleton()->get_default_pixels_per_unit();
	} else {
		//Just fall back to the default.
		//Note that this will be removed when I get to it, as it can be calculated.
		_pixels_per_unit = 64;
	}

	_build_flags = 0;

	_format = RenderingServer::ARRAY_FORMAT_TEX_UV;

	_noise.instance();
	//todo add properties for these if needed
	_noise->set_octaves(4);
	_noise->set_period(30);
	_noise->set_persistence(0.3);

	_rao_scale_factor = 0.6;
	_rao_seed = 2134;
}

Prop2DMesher::~Prop2DMesher() {
}

void Prop2DMesher::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_format"), &Prop2DMesher::get_format);
	ClassDB::bind_method(D_METHOD("set_format", "value"), &Prop2DMesher::set_format);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "format"), "set_format", "get_format");

	ClassDB::bind_method(D_METHOD("get_texture_scale"), &Prop2DMesher::get_texture_scale);
	ClassDB::bind_method(D_METHOD("set_texture_scale", "value"), &Prop2DMesher::set_texture_scale);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_scale"), "set_texture_scale", "get_texture_scale");

	ClassDB::bind_method(D_METHOD("get_material"), &Prop2DMesher::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &Prop2DMesher::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_pixels_per_unit"), &Prop2DMesher::get_pixels_per_unit);
	ClassDB::bind_method(D_METHOD("set_pixels_per_unit", "value"), &Prop2DMesher::set_pixels_per_unit);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "pixels_per_unit"), "set_pixels_per_unit", "get_pixels_per_unit");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &Prop2DMesher::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &Prop2DMesher::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_ao_strength"), &Prop2DMesher::get_ao_strength);
	ClassDB::bind_method(D_METHOD("set_ao_strength", "value"), &Prop2DMesher::set_ao_strength);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ao_strength"), "set_ao_strength", "get_ao_strength");

	ClassDB::bind_method(D_METHOD("get_base_light_value"), &Prop2DMesher::get_base_light_value);
	ClassDB::bind_method(D_METHOD("set_base_light_value", "value"), &Prop2DMesher::set_base_light_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "base_light_value"), "set_base_light_value", "get_base_light_value");

	ClassDB::bind_method(D_METHOD("get_uv_margin"), &Prop2DMesher::get_uv_margin);
	ClassDB::bind_method(D_METHOD("set_uv_margin", "value"), &Prop2DMesher::set_uv_margin);
	ADD_PROPERTY(PropertyInfo(Variant::RECT2, "uv_margin"), "set_uv_margin", "get_uv_margin");

	ClassDB::bind_method(D_METHOD("get_build_flags"), &Prop2DMesher::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &Prop2DMesher::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, Prop2DMesher::BINDING_STRING_BUILD_FLAGS), "set_build_flags", "get_build_flags");

	ClassDB::bind_method(D_METHOD("add_tiled_wall_simple", "width", "height", "transform", "tiled_wall_data", "cache"), &Prop2DMesher::add_tiled_wall_simple);
	ClassDB::bind_method(D_METHOD("add_tiled_wall_mesh_rect_simple", "x", "y", "transform", "texture_rect"), &Prop2DMesher::add_tiled_wall_mesh_rect_simple);
	ClassDB::bind_method(D_METHOD("transform_uv", "uv", "rect"), &Prop2DMesher::transform_uv);

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource", "mesh", "position", "rotation", "scale", "uv_rect"), &Prop2DMesher::add_mesh_data_resource, DEFVAL(Vector3()), DEFVAL(Vector3()), DEFVAL(Vector3(1.0, 1.0, 1.0)), DEFVAL(Rect2(0, 0, 1, 1)));
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource_transform", "mesh", "transform", "uv_rect"), &Prop2DMesher::add_mesh_data_resource_transform, DEFVAL(Rect2(0, 0, 1, 1)));
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource_transform_colored", "mesh", "transform", "colors", "uv_rect"), &Prop2DMesher::add_mesh_data_resource_transform_colored, DEFVAL(Rect2(0, 0, 1, 1)));
#endif

	ClassDB::bind_method(D_METHOD("generate_ao"), &Prop2DMesher::generate_ao);
	ClassDB::bind_method(D_METHOD("get_random_ao", "position"), &Prop2DMesher::get_random_ao);

	BIND_VMETHOD(MethodInfo("_add_mesher", PropertyInfo(Variant::OBJECT, "mesher", PROPERTY_HINT_RESOURCE_TYPE, "Prop2DMesher")));
	ClassDB::bind_method(D_METHOD("add_mesher", "mesher"), &Prop2DMesher::add_mesher);
	ClassDB::bind_method(D_METHOD("_add_mesher", "mesher"), &Prop2DMesher::_add_mesher);

	ClassDB::bind_method(D_METHOD("add_light", "light"), &Prop2DMesher::add_light);
	ClassDB::bind_method(D_METHOD("clear_lights"), &Prop2DMesher::clear_lights);

	ClassDB::bind_method(D_METHOD("get_vertices"), &Prop2DMesher::get_vertices);
	ClassDB::bind_method(D_METHOD("set_vertices", "values"), &Prop2DMesher::set_vertices);
	ClassDB::bind_method(D_METHOD("get_vertex_count"), &Prop2DMesher::get_vertex_count);
	ClassDB::bind_method(D_METHOD("get_vertex", "idx"), &Prop2DMesher::get_vertex);
	ClassDB::bind_method(D_METHOD("remove_vertex", "idx"), &Prop2DMesher::remove_vertex);
	ClassDB::bind_method(D_METHOD("add_vertex", "vertex"), &Prop2DMesher::add_vertex);

	ClassDB::bind_method(D_METHOD("get_colors"), &Prop2DMesher::get_colors);
	ClassDB::bind_method(D_METHOD("set_colors", "values"), &Prop2DMesher::set_colors);
	ClassDB::bind_method(D_METHOD("get_color", "idx"), &Prop2DMesher::get_color);
	ClassDB::bind_method(D_METHOD("add_color", "color"), &Prop2DMesher::add_color);

	ClassDB::bind_method(D_METHOD("get_uvs"), &Prop2DMesher::get_uvs);
	ClassDB::bind_method(D_METHOD("set_uvs", "values"), &Prop2DMesher::set_uvs);
	ClassDB::bind_method(D_METHOD("get_uv", "idx"), &Prop2DMesher::get_uv);
	ClassDB::bind_method(D_METHOD("add_uv", "uv"), &Prop2DMesher::add_uv);

	ClassDB::bind_method(D_METHOD("get_indices"), &Prop2DMesher::get_indices);
	ClassDB::bind_method(D_METHOD("set_indices", "values"), &Prop2DMesher::set_indices);
	ClassDB::bind_method(D_METHOD("get_indices_count"), &Prop2DMesher::get_indices_count);
	ClassDB::bind_method(D_METHOD("get_index", "idx"), &Prop2DMesher::get_index);
	ClassDB::bind_method(D_METHOD("remove_index", "idx"), &Prop2DMesher::remove_index);
	ClassDB::bind_method(D_METHOD("add_indices", "indice"), &Prop2DMesher::add_indices);

	ClassDB::bind_method(D_METHOD("reset"), &Prop2DMesher::reset);

	//ClassDB::bind_method(D_METHOD("calculate_vertex_ambient_occlusion", "meshinstance_path", "radius", "intensity", "sampleCount"), &Prop2DMesher::calculate_vertex_ambient_occlusion_path);

	ClassDB::bind_method(D_METHOD("build_mesh"), &Prop2DMesher::build_mesh);
	ClassDB::bind_method(D_METHOD("build_mesh_into", "mesh_rid"), &Prop2DMesher::build_mesh_into);
	ClassDB::bind_method(D_METHOD("build_collider"), &Prop2DMesher::build_collider);

	ClassDB::bind_method(D_METHOD("bake_colors"), &Prop2DMesher::bake_colors);

	ClassDB::bind_method(D_METHOD("remove_doubles"), &Prop2DMesher::remove_doubles);
	ClassDB::bind_method(D_METHOD("remove_doubles_hashed"), &Prop2DMesher::remove_doubles_hashed);
}
