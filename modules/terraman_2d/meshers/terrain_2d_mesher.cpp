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

#include "terrain_2d_mesher.h"

#include "../defines.h"

#include "scene/3d/mesh_instance.h"

#include "../world/default/terrain_2d_chunk_default.h"
#include "../world/terrain_2d_chunk.h"
#include "scene/resources/rectangle_shape_2d.h"
#include "servers/physics_2d_server.h"

bool Terrain2DMesher::Vertex::operator==(const Vertex &p_vertex) const {
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

uint32_t Terrain2DMesher::VertexHasher::hash(const Vertex &p_vtx) {
	uint32_t h = hash_djb2_buffer((const uint8_t *)&p_vtx.vertex, sizeof(real_t) * 3);
	h = hash_djb2_buffer((const uint8_t *)&p_vtx.uv, sizeof(real_t) * 2, h);
	h = hash_djb2_buffer((const uint8_t *)&p_vtx.color, sizeof(real_t) * 4, h);
	h = hash_djb2_buffer((const uint8_t *)p_vtx.bones.ptr(), p_vtx.bones.size() * sizeof(int), h);
	h = hash_djb2_buffer((const uint8_t *)p_vtx.weights.ptr(), p_vtx.weights.size() * sizeof(float), h);
	return h;
}

int Terrain2DMesher::get_channel_index_type() const {
	return _channel_index_type;
}
void Terrain2DMesher::set_channel_index_type(const int value) {
	_channel_index_type = value;
}

int Terrain2DMesher::get_format() const {
	return _format;
}
void Terrain2DMesher::set_format(const int value) {
	_format = value;
}

int Terrain2DMesher::get_texture_scale() const {
	return _texture_scale;
}
void Terrain2DMesher::set_texture_scale(const int value) {
	_texture_scale = value;
}

Transform2D Terrain2DMesher::get_mesh_transform() {
	return _mesh_transform;
}
void Terrain2DMesher::set_mesh_transform(const Transform2D &value) {
	_mesh_transform = value;
}

Ref<Terrain2DLibrary> Terrain2DMesher::get_library() {
	return _library;
}
void Terrain2DMesher::set_library(const Ref<Terrain2DLibrary> &library) {
	_library = library;
}

Ref<Material> Terrain2DMesher::get_material() {
	return _material;
}
void Terrain2DMesher::set_material(const Ref<Material> &material) {
	_material = material;
}

float Terrain2DMesher::get_ao_strength() const {
	return _ao_strength;
}
void Terrain2DMesher::set_ao_strength(float value) {
	_ao_strength = value;
}

float Terrain2DMesher::get_base_light_value() const {
	return _base_light_value;
}
void Terrain2DMesher::set_base_light_value(float value) {
	_base_light_value = value;
}

int Terrain2DMesher::get_cell_size_x() const {
	return _cell_size_x;
}
void Terrain2DMesher::set_cell_size_x(const int val) {
	_cell_size_x = val;
}

int Terrain2DMesher::get_cell_size_y() const {
	return _cell_size_y;
}
void Terrain2DMesher::set_cell_size_y(const int val) {
	_cell_size_y = val;
}

Rect2 Terrain2DMesher::get_uv_margin() const {
	return _uv_margin;
}
void Terrain2DMesher::set_uv_margin(const Rect2 margin) {
	_uv_margin = margin;
}

Array Terrain2DMesher::build_mesh() {
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

void Terrain2DMesher::build_mesh_into(RID mesh) {
	ERR_FAIL_COND(mesh == RID());

	RS::get_singleton()->mesh_clear(mesh);

	if (_vertices.size() == 0) {
		//Nothing to do
		return;
	}

	Array arr = build_mesh();

	RS::get_singleton()->mesh_add_surface_from_arrays(mesh, RenderingServer::PRIMITIVE_TRIANGLES, arr);
}

void Terrain2DMesher::remove_doubles() {
	if (_vertices.size() == 0)
		return;

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
void Terrain2DMesher::remove_doubles_hashed() {
	if (_vertices.size() == 0)
		return;

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

void Terrain2DMesher::store_mesh() {
	Terrain2DMesherStoredMesh m;

	m.vertices = _vertices;
	m.indices = _indices;
	m.transform = _mesh_transform;

	_vertices.resize(0);
	_indices.resize(0);

	_stored_meshes.push_back(m);
}

int Terrain2DMesher::get_stored_mesh_count() const {
	return _stored_meshes.size();
}

Array Terrain2DMesher::build_stored_mesh(const int index) {
	ERR_FAIL_INDEX_V(index, _stored_meshes.size(), Array());

	Array a;
	a.resize(RenderingServer::ARRAY_MAX);

	const Terrain2DMesherStoredMesh &md = _stored_meshes[index];

	if (md.vertices.size() == 0) {
		//Nothing to do
		return a;
	}

	{
		PoolVector<Vector2> array;
		array.resize(md.vertices.size());

		PoolVector<Vector2>::Write w = array.write();

		for (int i = 0; i < md.vertices.size(); ++i) {
			w[i] = md.vertices[i].vertex;
		}

		w.release();

		a[RenderingServer::ARRAY_VERTEX] = array;
	}

	if ((_format & RenderingServer::ARRAY_FORMAT_COLOR) != 0) {
		PoolVector<Color> array;
		array.resize(md.vertices.size());
		PoolVector<Color>::Write w = array.write();

		for (int i = 0; i < md.vertices.size(); ++i) {
			w[i] = md.vertices[i].color;
		}

		w.release();
		a[RenderingServer::ARRAY_COLOR] = array;
	}

	if ((_format & RenderingServer::ARRAY_FORMAT_TEX_UV) != 0) {
		PoolVector<Vector2> array;
		array.resize(md.vertices.size());
		PoolVector<Vector2>::Write w = array.write();

		for (int i = 0; i < md.vertices.size(); ++i) {
			w[i] = md.vertices[i].uv;
			array.set(i, md.vertices[i].uv);
		}

		w.release();
		a[RenderingServer::ARRAY_TEX_UV] = array;
	}

	if (md.indices.size() > 0) {
		PoolVector<int> array;
		array.resize(md.indices.size());
		PoolVector<int>::Write w = array.write();

		for (int i = 0; i < md.indices.size(); ++i) {
			w[i] = md.indices[i];
		}

		w.release();
		a[RenderingServer::ARRAY_INDEX] = array;
	}

	return a;
}

Transform2D Terrain2DMesher::get_stored_mesh_transform(const int index) {
	ERR_FAIL_INDEX_V(index, _stored_meshes.size(), Transform2D());

	return _stored_meshes[index].transform;
}

void Terrain2DMesher::build_stored_mesh_into(const int index, RID mesh) {
	ERR_FAIL_COND(mesh == RID());

	RS::get_singleton()->mesh_clear(mesh);

	if (_vertices.size() == 0) {
		//Nothing to do
		return;
	}

	Array arr = build_stored_mesh(index);

	RS::get_singleton()->mesh_add_surface_from_arrays(mesh, RenderingServer::PRIMITIVE_TRIANGLES, arr);
}

AABB Terrain2DMesher::calculate_stored_mesh_aabb(const int index) {
	ERR_FAIL_INDEX_V(index, _stored_meshes.size(), AABB());

	const Terrain2DMesherStoredMesh &md = _stored_meshes[index];

	if (md.vertices.size() == 0) {
		//Nothing to do
		return AABB();
	}

	AABB aabb;

	int len = md.vertices.size();
	aabb.position = Vector3(md.vertices[0].vertex.x, md.vertices[0].vertex.y, 0);

	for (int i = 0; i < len; i++) {
		aabb.expand_to(Vector3(md.vertices[i].vertex.x, md.vertices[i].vertex.y, 0));
	}

	return aabb;
}

Ref<Shape2D> Terrain2DMesher::create_terrain_tile_collider_shape(Ref<Terrain2DChunk> chunk) {
	Ref<RectangleShape2D> sh;
	sh.instance();

	sh->set_extents(Vector2(_cell_size_x, _cell_size_y));

	return sh;
}

void Terrain2DMesher::reset() {
	_vertices.resize(0);
	_indices.resize(0);

	_last_color = Color();
	_last_uv = Vector2();
	_last_bones.clear();
	_last_weights.clear();

	_stored_meshes.resize(0);
}

void Terrain2DMesher::add_chunk(Ref<Terrain2DChunk> chunk) {
	ERR_FAIL_COND(!has_method("_add_chunk"));
	ERR_FAIL_COND(!chunk.is_valid());

	call("_add_chunk", chunk);
}

#ifdef MESH_DATA_RESOURCE_PRESENT
void Terrain2DMesher::add_mesh_data_resource(Ref<MeshDataResource> mesh, const Vector3 position, const Vector3 rotation, const Vector3 scale, const Rect2 uv_rect) {
	//Transform2D transform = Transform(Basis(rotation).scaled(scale), position);
	//TODO
	add_mesh_data_resource_transform(mesh, Transform2D(), uv_rect);
}

void Terrain2DMesher::add_mesh_data_resource_transform(Ref<MeshDataResource> mesh, const Transform2D transform, const Rect2 uv_rect) {
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

		add_vertex(transform.xform(vertices[i]));
	}

	int orig_indices_count = _indices.size();
	_indices.resize(_indices.size() + indices.size());

	for (int i = 0; i < indices.size(); ++i) {
		_indices.set(orig_indices_count + i, orig_vert_size + indices[i]);
	}
}

void Terrain2DMesher::add_mesh_data_resource_transform_colored(Ref<MeshDataResource> mesh, const Transform2D transform, const PoolColorArray &colors, const Rect2 uv_rect) {
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

		add_vertex(transform.xform(vertices[i]));
	}

	int orig_indices_count = _indices.size();
	_indices.resize(_indices.size() + indices.size());

	for (int i = 0; i < indices.size(); ++i) {
		_indices.set(orig_indices_count + i, orig_vert_size + indices[i]);
	}
}
#endif

void Terrain2DMesher::add_mesher(const Ref<Terrain2DMesher> &mesher) {
	call("_add_mesher", mesher);
}
void Terrain2DMesher::_add_mesher(const Ref<Terrain2DMesher> &mesher) {
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

void Terrain2DMesher::bake_colors(Ref<Terrain2DChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (has_method("_bake_colors")) {
		call("_bake_colors", chunk);
	}
}

void Terrain2DMesher::bake_liquid_colors(Ref<Terrain2DChunk> chunk) {
	ERR_FAIL_COND(!chunk.is_valid());

	if (has_method("_bake_liquid_colors")) {
		call("_bake_liquid_colors", chunk);
	}
}

PoolVector<Vector2> Terrain2DMesher::build_collider() const {
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

void Terrain2DMesher::create_tile_colliders(Ref<Terrain2DChunk> chunk) {
}

void Terrain2DMesher::bake_lights(MeshInstance *node, Vector<Ref<Terrain2DLight>> &lights) {
	ERR_FAIL_COND(node == NULL);

	Color darkColor(0, 0, 0, 1);

	for (int v = 0; v < _vertices.size(); ++v) {
		Vertex vertexv = _vertices.get(v);
		Vector2 vet = vertexv.vertex;
		//TODO
		//Vector2 vertex = node->to_global(vet);
		Vector2 vertex = vet;

		Vector3 v_lightDiffuse;

		//calculate the lights value
		for (int i = 0; i < lights.size(); ++i) {
			Ref<Terrain2DLight> light = lights.get(i);

			Vector2 lightDir = light->get_world_position() - vertex;

			float dist2 = lightDir.dot(lightDir);
			//inverse sqrt
			lightDir *= (1.0 / sqrt(dist2));

			Color cc = light->get_color();
			Vector3 cv(cc.r, cc.g, cc.b);

			Vector3 value = cv * (1.0 / (1.0 + dist2));

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
}

PoolVector<Vector2> Terrain2DMesher::get_vertices() const {
	PoolVector<Vector2> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).vertex);
	}

	return arr;
}

void Terrain2DMesher::set_vertices(const PoolVector<Vector2> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.vertex = values[i];

		_vertices.set(i, v);
	}
}

int Terrain2DMesher::get_vertex_count() const {
	return _vertices.size();
}

void Terrain2DMesher::add_vertex(const Vector2 &vertex) {
	Vertex vtx;
	vtx.vertex = vertex;
	vtx.color = _last_color;
	vtx.uv = _last_uv;
	// Todo?
	//	vtx.weights = _last_weights;
	//	vtx.bones = _last_bones;

	_vertices.push_back(vtx);
}

Vector2 Terrain2DMesher::get_vertex(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector2());

	return _vertices.get(idx).vertex;
}

void Terrain2DMesher::remove_vertex(const int idx) {
	ERR_FAIL_INDEX(idx, _vertices.size());

	_vertices.remove(idx);
}

PoolVector<Color> Terrain2DMesher::get_colors() const {
	PoolVector<Color> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).color);
	}

	return arr;
}

void Terrain2DMesher::set_colors(const PoolVector<Color> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.color = values[i];

		_vertices.set(i, v);
	}
}

void Terrain2DMesher::add_color(const Color &color) {
	_last_color = color;
}

Color Terrain2DMesher::get_color(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Color());

	return _vertices.get(idx).color;
}

PoolVector<Vector2> Terrain2DMesher::get_uvs() const {
	PoolVector<Vector2> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).uv);
	}

	return arr;
}

void Terrain2DMesher::set_uvs(const PoolVector<Vector2> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.uv = values[i];

		_vertices.set(i, v);
	}
}

void Terrain2DMesher::add_uv(const Vector2 &uv) {
	_last_uv = uv;
}

Vector2 Terrain2DMesher::get_uv(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector2());

	return _vertices.get(idx).uv;
}

PoolVector<int> Terrain2DMesher::get_indices() const {
	return _indices;
}

void Terrain2DMesher::set_indices(const PoolVector<int> &values) {
	_indices = values;
}

int Terrain2DMesher::get_indices_count() const {
	return _indices.size();
}

void Terrain2DMesher::add_indices(const int index) {
	_indices.push_back(index);
}

int Terrain2DMesher::get_index(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _indices.size(), 0);

	return _indices.get(idx);
}

void Terrain2DMesher::remove_index(const int idx) {
	ERR_FAIL_INDEX(idx, _indices.size());

	_indices.remove(idx);
}

Terrain2DMesher::Terrain2DMesher(const Ref<Terrain2DLibrary> &library) {
	_library = library;

	_cell_size_x = 32;
	_cell_size_y = 32;
	_ao_strength = 0.25;
	_base_light_value = 0.5;
	_uv_margin = Rect2(0, 0, 1, 1);
	_channel_index_type = 0;

	_format = 0;
	_texture_scale = 1;
}

Terrain2DMesher::Terrain2DMesher() {
	_cell_size_x = 32;
	_cell_size_y = 32;
	_ao_strength = 0.25;
	_base_light_value = 0.5;
	_uv_margin = Rect2(0, 0, 1, 1);
	_format = 0;
	_channel_index_type = 0;
	_texture_scale = 1;
}

Terrain2DMesher::~Terrain2DMesher() {
	if (_library.is_valid()) {
		_library.unref();
	}
}

void Terrain2DMesher::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_add_chunk", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));
	BIND_VMETHOD(MethodInfo("_bake_colors", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));
	BIND_VMETHOD(MethodInfo("_bake_liquid_colors", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DChunk")));

	ClassDB::bind_method(D_METHOD("get_channel_index_type"), &Terrain2DMesher::get_channel_index_type);
	ClassDB::bind_method(D_METHOD("set_channel_index_type", "value"), &Terrain2DMesher::set_channel_index_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "channel_index_type"), "set_channel_index_type", "get_channel_index_type");

	ClassDB::bind_method(D_METHOD("get_format"), &Terrain2DMesher::get_format);
	ClassDB::bind_method(D_METHOD("set_format", "value"), &Terrain2DMesher::set_format);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "format"), "set_format", "get_format");

	ClassDB::bind_method(D_METHOD("get_texture_scale"), &Terrain2DMesher::get_texture_scale);
	ClassDB::bind_method(D_METHOD("set_texture_scale", "value"), &Terrain2DMesher::set_texture_scale);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_scale"), "set_texture_scale", "get_texture_scale");

	ClassDB::bind_method(D_METHOD("get_mesh_transform"), &Terrain2DMesher::get_mesh_transform);
	ClassDB::bind_method(D_METHOD("set_mesh_transform", "value"), &Terrain2DMesher::set_mesh_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "mesh_transform"), "set_mesh_transform", "get_mesh_transform");

	ClassDB::bind_method(D_METHOD("get_library"), &Terrain2DMesher::get_library);
	ClassDB::bind_method(D_METHOD("set_library", "value"), &Terrain2DMesher::set_library);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "library", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DLibrary"), "set_library", "get_library");

	ClassDB::bind_method(D_METHOD("get_material"), &Terrain2DMesher::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &Terrain2DMesher::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_cell_size_x"), &Terrain2DMesher::get_cell_size_x);
	ClassDB::bind_method(D_METHOD("set_cell_size_x", "value"), &Terrain2DMesher::set_cell_size_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cell_size_x"), "set_cell_size_x", "get_cell_size_x");

	ClassDB::bind_method(D_METHOD("get_cell_size_y"), &Terrain2DMesher::get_cell_size_y);
	ClassDB::bind_method(D_METHOD("set_cell_size_y", "value"), &Terrain2DMesher::set_cell_size_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cell_size_y"), "set_cell_size_y", "get_cell_size_y");

	ClassDB::bind_method(D_METHOD("get_ao_strength"), &Terrain2DMesher::get_ao_strength);
	ClassDB::bind_method(D_METHOD("set_ao_strength", "value"), &Terrain2DMesher::set_ao_strength);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ao_strength"), "set_ao_strength", "get_ao_strength");

	ClassDB::bind_method(D_METHOD("get_base_light_value"), &Terrain2DMesher::get_base_light_value);
	ClassDB::bind_method(D_METHOD("set_base_light_value", "value"), &Terrain2DMesher::set_base_light_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "base_light_value"), "set_base_light_value", "get_base_light_value");

	ClassDB::bind_method(D_METHOD("get_uv_margin"), &Terrain2DMesher::get_uv_margin);
	ClassDB::bind_method(D_METHOD("set_uv_margin", "value"), &Terrain2DMesher::set_uv_margin);
	ADD_PROPERTY(PropertyInfo(Variant::RECT2, "uv_margin"), "set_uv_margin", "get_uv_margin");

	ClassDB::bind_method(D_METHOD("add_chunk", "chunk"), &Terrain2DMesher::add_chunk);

#ifdef MESH_DATA_RESOURCE_PRESENT
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource", "mesh", "position", "rotation", "scale", "uv_rect"), &Terrain2DMesher::add_mesh_data_resource, DEFVAL(Rect2(0, 0, 1, 1)), DEFVAL(Vector3(1.0, 1.0, 1.0)), DEFVAL(Vector3()), DEFVAL(Vector3()));
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource_transform", "mesh", "transform", "uv_rect"), &Terrain2DMesher::add_mesh_data_resource_transform, DEFVAL(Rect2(0, 0, 1, 1)));
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource_transform_colored", "mesh", "transform", "colors", "uv_rect"), &Terrain2DMesher::add_mesh_data_resource_transform_colored, DEFVAL(Rect2(0, 0, 1, 1)));
#endif

	BIND_VMETHOD(MethodInfo("_add_mesher", PropertyInfo(Variant::OBJECT, "mesher", PROPERTY_HINT_RESOURCE_TYPE, "Terrain2DMesher")));
	ClassDB::bind_method(D_METHOD("add_mesher", "mesher"), &Terrain2DMesher::add_mesher);
	ClassDB::bind_method(D_METHOD("_add_mesher", "mesher"), &Terrain2DMesher::_add_mesher);

	ClassDB::bind_method(D_METHOD("bake_colors", "chunk"), &Terrain2DMesher::bake_colors);
	ClassDB::bind_method(D_METHOD("bake_liquid_colors", "chunk"), &Terrain2DMesher::bake_liquid_colors);

	ClassDB::bind_method(D_METHOD("get_vertices"), &Terrain2DMesher::get_vertices);
	ClassDB::bind_method(D_METHOD("set_vertices", "values"), &Terrain2DMesher::set_vertices);
	ClassDB::bind_method(D_METHOD("get_vertex_count"), &Terrain2DMesher::get_vertex_count);
	ClassDB::bind_method(D_METHOD("get_vertex", "idx"), &Terrain2DMesher::get_vertex);
	ClassDB::bind_method(D_METHOD("remove_vertex", "idx"), &Terrain2DMesher::remove_vertex);
	ClassDB::bind_method(D_METHOD("add_vertex", "vertex"), &Terrain2DMesher::add_vertex);

	ClassDB::bind_method(D_METHOD("get_colors"), &Terrain2DMesher::get_colors);
	ClassDB::bind_method(D_METHOD("set_colors", "values"), &Terrain2DMesher::set_colors);
	ClassDB::bind_method(D_METHOD("get_color", "idx"), &Terrain2DMesher::get_color);
	ClassDB::bind_method(D_METHOD("add_color", "color"), &Terrain2DMesher::add_color);

	ClassDB::bind_method(D_METHOD("get_uvs"), &Terrain2DMesher::get_uvs);
	ClassDB::bind_method(D_METHOD("set_uvs", "values"), &Terrain2DMesher::set_uvs);
	ClassDB::bind_method(D_METHOD("get_uv", "idx"), &Terrain2DMesher::get_uv);
	ClassDB::bind_method(D_METHOD("add_uv", "uv"), &Terrain2DMesher::add_uv);

	ClassDB::bind_method(D_METHOD("get_indices"), &Terrain2DMesher::get_indices);
	ClassDB::bind_method(D_METHOD("set_indices", "values"), &Terrain2DMesher::set_indices);
	ClassDB::bind_method(D_METHOD("get_indices_count"), &Terrain2DMesher::get_indices_count);
	ClassDB::bind_method(D_METHOD("get_index", "idx"), &Terrain2DMesher::get_index);
	ClassDB::bind_method(D_METHOD("remove_index", "idx"), &Terrain2DMesher::remove_index);
	ClassDB::bind_method(D_METHOD("add_indices", "indice"), &Terrain2DMesher::add_indices);

	ClassDB::bind_method(D_METHOD("reset"), &Terrain2DMesher::reset);
	ClassDB::bind_method(D_METHOD("create_terrain_tile_collider_shape", "chunk"), &Terrain2DMesher::create_terrain_tile_collider_shape);

	//ClassDB::bind_method(D_METHOD("calculate_vertex_ambient_occlusion", "meshinstance_path", "radius", "intensity", "sampleCount"), &Terrain2DMesher::calculate_vertex_ambient_occlusion_path);

	ClassDB::bind_method(D_METHOD("build_mesh"), &Terrain2DMesher::build_mesh);
	ClassDB::bind_method(D_METHOD("build_mesh_into", "mesh_rid"), &Terrain2DMesher::build_mesh_into);
	ClassDB::bind_method(D_METHOD("build_collider"), &Terrain2DMesher::build_collider);

	ClassDB::bind_method(D_METHOD("remove_doubles"), &Terrain2DMesher::remove_doubles);
	ClassDB::bind_method(D_METHOD("remove_doubles_hashed"), &Terrain2DMesher::remove_doubles_hashed);

	ClassDB::bind_method(D_METHOD("store_mesh"), &Terrain2DMesher::store_mesh);
	ClassDB::bind_method(D_METHOD("get_stored_mesh_count"), &Terrain2DMesher::get_stored_mesh_count);
	ClassDB::bind_method(D_METHOD("build_stored_mesh", "index"), &Terrain2DMesher::build_stored_mesh);
	ClassDB::bind_method(D_METHOD("get_stored_mesh_transform", "index"), &Terrain2DMesher::get_stored_mesh_transform);
	ClassDB::bind_method(D_METHOD("build_stored_mesh_into", "index", "mesh"), &Terrain2DMesher::build_stored_mesh_into);
}
