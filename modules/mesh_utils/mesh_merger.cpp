

#include "mesh_merger.h"

#include "defines.h"

#include "scene/3d/mesh_instance.h"

#include "modules/modules_enabled.gen.h"

bool MeshMerger::Vertex::operator==(const Vertex &p_vertex) const {
	if (vertex != p_vertex.vertex)
		return false;

	if (uv != p_vertex.uv)
		return false;

	if (uv2 != p_vertex.uv2)
		return false;

	if (normal != p_vertex.normal)
		return false;

	if (binormal != p_vertex.binormal)
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

uint32_t MeshMerger::VertexHasher::hash(const Vertex &p_vtx) {
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

int MeshMerger::get_mesher_index() const {
	return _mesher_index;
}
void MeshMerger::set_mesher_index(const int value) {
	_mesher_index = value;
}

int MeshMerger::get_format() const {
	return _format;
}
void MeshMerger::set_format(const int value) {
	_format = value;
}

Ref<Material> MeshMerger::get_material() {
	return _material;
}
void MeshMerger::set_material(const Ref<Material> &material) {
	_material = material;
}

float MeshMerger::get_ao_strength() const {
	return _ao_strength;
}
void MeshMerger::set_ao_strength(float value) {
	_ao_strength = value;
}

float MeshMerger::get_base_light_value() const {
	return _base_light_value;
}
void MeshMerger::set_base_light_value(float value) {
	_base_light_value = value;
}

float MeshMerger::get_voxel_scale() const {
	return _voxel_scale;
}
void MeshMerger::set_voxel_scale(const float voxel_scale) {
	_voxel_scale = voxel_scale;
}

int MeshMerger::get_lod_size() const {
	return _lod_size;
}
void MeshMerger::set_lod_size(const int lod_size) {
	_lod_size = lod_size;
}

Rect2 MeshMerger::get_uv_margin() const {
	return _uv_margin;
}
void MeshMerger::set_uv_margin(const Rect2 margin) {
	_uv_margin = margin;
}

Array MeshMerger::build_mesh() {
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

	if ((_format & RenderingServer::ARRAY_FORMAT_NORMAL) != 0) {
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

	if ((_format & RenderingServer::ARRAY_FORMAT_BONES) != 0) {
		int size = _vertices.size() * 4;

		PoolVector<int> bone_array;
		bone_array.resize(size);

		PoolVector<int>::Write wb = bone_array.write();

		for (int i = 0; i < _vertices.size(); ++i) {
			const Vertex &vertex = _vertices[i];
			int curr = i * 4;

			for (int j = 0; j < vertex.bones.size(); ++j) {
				wb[curr + j] = vertex.bones[j];
			}
		}

		wb.release();

		a[RenderingServer::ARRAY_BONES] = bone_array;
	}

	if ((_format & RenderingServer::ARRAY_FORMAT_WEIGHTS) != 0) {
		int size = _vertices.size() * 4;

		PoolVector<float> bone_weights_array;
		bone_weights_array.resize(size);

		PoolVector<float>::Write wbw = bone_weights_array.write();

		for (int i = 0; i < _vertices.size(); ++i) {
			const Vertex &vertex = _vertices[i];
			int curr = i * 4;

			for (int j = 0; j < vertex.bones.size(); ++j) {
				wbw[curr + j] = vertex.weights[j];
			}
		}

		wbw.release();

		a[RenderingServer::ARRAY_WEIGHTS] = bone_weights_array;
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

void MeshMerger::build_mesh_into(RID mesh) {
	ERR_FAIL_COND(mesh == RID());

	RS::get_singleton()->mesh_clear(mesh);

	if (_vertices.size() == 0) {
		//Nothing to do
		return;
	}

	Array arr = build_mesh();

	RS::get_singleton()->mesh_add_surface_from_arrays(mesh, RenderingServer::PRIMITIVE_TRIANGLES, arr);

	if (_material.is_valid())
		RS::get_singleton()->mesh_surface_set_material(mesh, 0, get_material()->get_rid());
}

void MeshMerger::generate_normals(bool p_flip) {
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
		if (!p_flip)
			normal = Plane(v0.vertex, v1.vertex, v2.vertex).normal;
		else
			normal = Plane(v2.vertex, v1.vertex, v0.vertex).normal;

		v0.normal = normal;
		v1.normal = normal;
		v2.normal = normal;

		_vertices.set(i0, v0);
		_vertices.set(i1, v1);
		_vertices.set(i2, v2);
	}
}

void MeshMerger::remove_doubles() {
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
void MeshMerger::remove_doubles_hashed() {
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

void MeshMerger::reset() {
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

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED

void MeshMerger::add_mesh_data_resource(Ref<MeshDataResource> mesh, const Transform transform, Rect2 uv_rect) {
	ERR_FAIL_COND(mesh->get_array().size() == 0);

	Array verts = mesh->get_array().get(Mesh::ARRAY_VERTEX);

	for (int i = 0; i < verts.size(); ++i) {
		Vector3 vert = verts[i];

		vert = transform.xform(vert);

		add_vertex(vert);
	}

	if (mesh->get_array().size() <= Mesh::ARRAY_NORMAL)
		return;

	Array normals = mesh->get_array().get(Mesh::ARRAY_NORMAL);

	for (int i = 0; i < normals.size(); ++i) {
		Vector3 normal = normals[i];

		normal = transform.basis.xform(normal);

		add_normal(normal);
	}

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_TANGENT)
		return;

	Array tangents = mesh->get_array().get(Mesh::ARRAY_TANGENT);

	for (int i = 0; i < verts.size(); ++i) {

		Plane p(tangents[i * 4 + 0], tangents[i * 4 + 1], tangents[i * 4 + 2], tangents[i * 4 + 3]);

		Vector3 tangent = p.normal;
		Vector3 binormal = p.normal.cross(tangent).normalized() * p.d;

		tangent = local_transform.basis.xform(tangent);
		binormal = local_transform.basis.xform(binormal);

		add_t(normal);
		add_binorm
	}*/

	if (mesh->get_array().size() <= Mesh::ARRAY_COLOR)
		return;

	Array colors = mesh->get_array().get(Mesh::ARRAY_COLOR);

	for (int i = 0; i < colors.size(); ++i) {
		Color color = colors[i];

		add_color(color);
	}

	if (mesh->get_array().size() <= Mesh::ARRAY_TEX_UV)
		return;

	Array tex_uv = mesh->get_array().get(Mesh::ARRAY_TEX_UV);

	for (int i = 0; i < tex_uv.size(); ++i) {
		Vector2 uv = tex_uv[i];

		uv.x *= uv_rect.size.x;
		uv.y *= uv_rect.size.y;

		uv.x += uv_rect.position.x;
		uv.y += uv_rect.position.y;

		add_uv(uv);
	}

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_TEX_UV2)
		return;

	Array tex_uv2 = mesh->get_array().get(Mesh::ARRAY_TEX_UV2);

	for (int i = 0; i < tex_uv.size(); ++i) {
		Vector2 uv = tex_uv2[i];

		add_uv2(uv);
	}*/

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_BONES)
		return;

	Array bones = mesh->get_array().get(Mesh::ARRAY_BONES);

	for (int i = 0; i < bones.size(); ++i) {
		int bone = bones[i];

		add_bone(bone);
	}*/

	/*
	if (mesh->get_array().size() <= Mesh::ARRAY_WEIGHTS)
		return;

	Array weights = mesh->get_array().get(Mesh::ARRAY_WEIGHTS);

	for (int i = 0; i < weights.size(); ++i) {
		float weight = weights[i];

		add_weight(weight);
	}*/

	if (mesh->get_array().size() <= Mesh::ARRAY_INDEX)
		return;

	Array indices = mesh->get_array().get(Mesh::ARRAY_INDEX);
	int ic = get_vertex_count() - verts.size();

	for (int i = 0; i < indices.size(); ++i) {
		int index = indices[i];

		add_indices(ic + index);
	}
}

void MeshMerger::add_mesh_data_resource_bone(Ref<MeshDataResource> mesh, const Vector<int> &bones, const Vector<float> &weights, const Transform transform, const Rect2 uv_rect) {
	if (mesh->get_array().size() == 0)
		return;

	const Array &arr = mesh->get_array();

	PoolVector3Array vertices = arr[Mesh::ARRAY_VERTEX];
	PoolVector3Array normals = arr[Mesh::ARRAY_NORMAL];
	PoolVector2Array uvs = arr[Mesh::ARRAY_TEX_UV];
	PoolColorArray colors = arr[Mesh::ARRAY_COLOR];
	PoolIntArray indices = arr[Mesh::ARRAY_INDEX];

	//PoolIntArray bone_array = arr[Mesh::ARRAY_BONES];
	//PoolRealArray weights_array = arr[Mesh::ARRAY_WEIGHTS];

	add_bones(bones);
	add_bone_weights(weights);
	add_color(Color(0.7, 0.7, 0.7));

	int orig_vert_size = _vertices.size();
	//_vertices.resize(_vertices.size() + vertices.size());

	//bone_array.resize(bone_array.size() + cvertices.size() * 4)
	//			weights_array.resize(weights_array.size() + cvertices.size() * 4)

	for (int i = 0; i < vertices.size(); ++i) {
		add_normal(transform.basis.xform(normals[i]));

		Vector2 uv = uvs[i];

		uv.x = uv_rect.size.width * uv.x + uv_rect.position.x;
		uv.y = uv_rect.size.height * uv.y + uv_rect.position.y;

		add_uv(uv);
		add_vertex(transform.xform(vertices[i]));
	}

	int orig_indices_count = _indices.size();
	_indices.resize(_indices.size() + indices.size());

	for (int i = 0; i < indices.size(); ++i) {
		_indices.set(orig_indices_count + i, orig_vert_size + indices[i]);
	}
}

#endif

void MeshMerger::add_mesher(const Ref<MeshMerger> &mesher) {
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

PoolVector<Vector3> MeshMerger::build_collider() const {
	PoolVector<Vector3> face_points;

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

PoolVector<Vector3> MeshMerger::get_vertices() const {
	PoolVector<Vector3> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).vertex);
	}

	return arr;
}

void MeshMerger::set_vertices(const PoolVector<Vector3> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.normal = values[i];

		_vertices.set(i, v);
	}
}

int MeshMerger::get_vertex_count() const {
	return _vertices.size();
}

void MeshMerger::add_vertex(const Vector3 &vertex) {
	Vertex vtx;
	vtx.vertex = vertex;
	vtx.color = _last_color;
	vtx.normal = _last_normal;
	vtx.uv = _last_uv;
	vtx.uv2 = _last_uv2;
	vtx.weights = _last_weights;
	vtx.bones = _last_bones;
	//	vtx.tangent = _last_tangent.normal;
	//	vtx.binormal = _last_normal.cross(_last_tangent.normal).normalized() * _last_tangent.d;

	_vertices.push_back(vtx);
}

Vector3 MeshMerger::get_vertex(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector3());

	return _vertices.get(idx).vertex;
}

void MeshMerger::remove_vertex(const int idx) {
	ERR_FAIL_INDEX(idx, _vertices.size());

	_vertices.remove(idx);
}

PoolVector<Vector3> MeshMerger::get_normals() const {
	PoolVector<Vector3> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).normal);
	}

	return arr;
}

void MeshMerger::set_normals(const PoolVector<Vector3> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.normal = values[i];

		_vertices.set(i, v);
	}
}

void MeshMerger::add_normal(const Vector3 &normal) {
	_last_normal = normal;
}

Vector3 MeshMerger::get_normal(int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector3());

	return _vertices.get(idx).normal;
}

PoolVector<Color> MeshMerger::get_colors() const {
	PoolVector<Color> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).color);
	}

	return arr;
}

void MeshMerger::set_colors(const PoolVector<Color> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.color = values[i];

		_vertices.set(i, v);
	}
}

void MeshMerger::add_color(const Color &color) {
	_last_color = color;
}

Color MeshMerger::get_color(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Color());

	return _vertices.get(idx).color;
}

PoolVector<Vector2> MeshMerger::get_uvs() const {
	PoolVector<Vector2> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).uv);
	}

	return arr;
}

void MeshMerger::set_uvs(const PoolVector<Vector2> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.uv = values[i];

		_vertices.set(i, v);
	}
}

void MeshMerger::add_uv(const Vector2 &uv) {
	_last_uv = uv;
}

Vector2 MeshMerger::get_uv(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector2());

	return _vertices.get(idx).uv;
}

PoolVector<Vector2> MeshMerger::get_uv2s() const {
	PoolVector<Vector2> arr;

	arr.resize(_vertices.size());
	for (int i = 0; i < _vertices.size(); ++i) {
		arr.set(i, _vertices.get(i).uv2);
	}

	return arr;
}

void MeshMerger::set_uv2s(const PoolVector<Vector2> &values) {
	ERR_FAIL_COND(values.size() != _vertices.size());

	for (int i = 0; i < _vertices.size(); ++i) {
		Vertex v = _vertices[i];

		v.uv2 = values[i];

		_vertices.set(i, v);
	}
}

void MeshMerger::add_uv2(const Vector2 &uv) {
	_last_uv2 = uv;
}

Vector2 MeshMerger::get_uv2(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector2());

	return _vertices.get(idx).uv2;
}

Vector<int> MeshMerger::get_bones(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector<int>());

	return _vertices[idx].bones;
}
void MeshMerger::add_bones(const Vector<int> &vector) {
	_last_bones = vector;
}

Vector<float> MeshMerger::get_bone_weights(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _vertices.size(), Vector<float>());

	return _vertices[idx].weights;
}
void MeshMerger::add_bone_weights(const Vector<float> &arr) {
	_last_weights = arr;
}

PoolVector<int> MeshMerger::get_indices() const {
	return _indices;
}

void MeshMerger::set_indices(const PoolVector<int> &values) {
	_indices = values;
}

int MeshMerger::get_indices_count() const {
	return _indices.size();
}

void MeshMerger::add_indices(const int index) {
	_indices.push_back(index);
}

int MeshMerger::get_index(const int idx) const {
	ERR_FAIL_INDEX_V(idx, _indices.size(), 0);

	return _indices.get(idx);
}

void MeshMerger::remove_index(const int idx) {
	ERR_FAIL_INDEX(idx, _indices.size());

	_indices.remove(idx);
}

MeshMerger::MeshMerger() {
	_mesher_index = 0;
	_voxel_scale = 1;
	_lod_size = 1;
	_ao_strength = 0.25;
	_base_light_value = 0.5;
	_uv_margin = Rect2(0, 0, 1, 1);
	_format = 0;
}

MeshMerger::~MeshMerger() {
	if (_material.is_valid()) {
		_material.unref();
	}
}

void MeshMerger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_mesher_index"), &MeshMerger::get_mesher_index);
	ClassDB::bind_method(D_METHOD("set_mesher_index", "value"), &MeshMerger::set_mesher_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mesher_index"), "set_mesher_index", "get_mesher_index");

	ClassDB::bind_method(D_METHOD("get_format"), &MeshMerger::get_format);
	ClassDB::bind_method(D_METHOD("set_format", "value"), &MeshMerger::set_format);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "format"), "set_format", "get_format");

	ClassDB::bind_method(D_METHOD("get_material"), &MeshMerger::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &MeshMerger::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_voxel_scale"), &MeshMerger::get_voxel_scale);
	ClassDB::bind_method(D_METHOD("set_voxel_scale", "value"), &MeshMerger::set_voxel_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "voxel_scale"), "set_voxel_scale", "get_voxel_scale");

	ClassDB::bind_method(D_METHOD("get_lod_size"), &MeshMerger::get_lod_size);
	ClassDB::bind_method(D_METHOD("set_lod_size", "value"), &MeshMerger::set_lod_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_size"), "set_lod_size", "get_lod_size");

	ClassDB::bind_method(D_METHOD("get_ao_strength"), &MeshMerger::get_ao_strength);
	ClassDB::bind_method(D_METHOD("set_ao_strength", "value"), &MeshMerger::set_ao_strength);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "ao_strength"), "set_ao_strength", "get_ao_strength");

	ClassDB::bind_method(D_METHOD("get_base_light_value"), &MeshMerger::get_base_light_value);
	ClassDB::bind_method(D_METHOD("set_base_light_value", "value"), &MeshMerger::set_base_light_value);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "base_light_value"), "set_base_light_value", "get_base_light_value");

	ClassDB::bind_method(D_METHOD("get_uv_margin"), &MeshMerger::get_uv_margin);
	ClassDB::bind_method(D_METHOD("set_uv_margin", "value"), &MeshMerger::set_uv_margin);
	ADD_PROPERTY(PropertyInfo(Variant::RECT2, "uv_margin"), "set_uv_margin", "get_uv_margin");

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource", "mesh", "transform", "uv_rect"), &MeshMerger::add_mesh_data_resource, DEFVAL(Transform()), DEFVAL(Rect2(0, 0, 1, 1)));
	ClassDB::bind_method(D_METHOD("add_mesh_data_resource_bone", "mesh", "bones", "wrights", "transform", "uv_rect"), &MeshMerger::add_mesh_data_resource_bone, DEFVAL(Transform()), DEFVAL(Rect2(0, 0, 1, 1)));
#endif

	BIND_VMETHOD(MethodInfo("_add_mesher", PropertyInfo(Variant::OBJECT, "mesher", PROPERTY_HINT_RESOURCE_TYPE, "MeshMerger")));
	ClassDB::bind_method(D_METHOD("add_mesher", "mesher"), &MeshMerger::add_mesher);

	ClassDB::bind_method(D_METHOD("get_vertices"), &MeshMerger::get_vertices);
	ClassDB::bind_method(D_METHOD("set_vertices", "values"), &MeshMerger::set_vertices);
	ClassDB::bind_method(D_METHOD("get_vertex_count"), &MeshMerger::get_vertex_count);
	ClassDB::bind_method(D_METHOD("get_vertex", "idx"), &MeshMerger::get_vertex);
	ClassDB::bind_method(D_METHOD("remove_vertex", "idx"), &MeshMerger::remove_vertex);
	ClassDB::bind_method(D_METHOD("add_vertex", "vertex"), &MeshMerger::add_vertex);

	ClassDB::bind_method(D_METHOD("get_normals"), &MeshMerger::get_normals);
	ClassDB::bind_method(D_METHOD("set_normals", "values"), &MeshMerger::set_normals);
	ClassDB::bind_method(D_METHOD("get_normal", "idx"), &MeshMerger::get_normal);
	ClassDB::bind_method(D_METHOD("add_normal", "normal"), &MeshMerger::add_normal);

	ClassDB::bind_method(D_METHOD("get_colors"), &MeshMerger::get_colors);
	ClassDB::bind_method(D_METHOD("set_colors", "values"), &MeshMerger::set_colors);
	ClassDB::bind_method(D_METHOD("get_color", "idx"), &MeshMerger::get_color);
	ClassDB::bind_method(D_METHOD("add_color", "color"), &MeshMerger::add_color);

	ClassDB::bind_method(D_METHOD("get_uvs"), &MeshMerger::get_uvs);
	ClassDB::bind_method(D_METHOD("set_uvs", "values"), &MeshMerger::set_uvs);
	ClassDB::bind_method(D_METHOD("get_uv", "idx"), &MeshMerger::get_uv);
	ClassDB::bind_method(D_METHOD("add_uv", "uv"), &MeshMerger::add_uv);

	ClassDB::bind_method(D_METHOD("get_uv2s"), &MeshMerger::get_uv2s);
	ClassDB::bind_method(D_METHOD("set_uv2s", "values"), &MeshMerger::set_uv2s);
	ClassDB::bind_method(D_METHOD("get_uv2", "idx"), &MeshMerger::get_uv2);
	ClassDB::bind_method(D_METHOD("add_uv2", "uv"), &MeshMerger::add_uv2);

	ClassDB::bind_method(D_METHOD("get_bones", "idx"), &MeshMerger::get_bones);
	ClassDB::bind_method(D_METHOD("add_bones", "arr"), &MeshMerger::add_bones);

	ClassDB::bind_method(D_METHOD("get_bone_weights", "idx"), &MeshMerger::get_bone_weights);
	ClassDB::bind_method(D_METHOD("add_bone_weights", "arr"), &MeshMerger::add_bone_weights);

	ClassDB::bind_method(D_METHOD("get_indices"), &MeshMerger::get_indices);
	ClassDB::bind_method(D_METHOD("set_indices", "values"), &MeshMerger::set_indices);
	ClassDB::bind_method(D_METHOD("get_indices_count"), &MeshMerger::get_indices_count);
	ClassDB::bind_method(D_METHOD("get_index", "idx"), &MeshMerger::get_index);
	ClassDB::bind_method(D_METHOD("remove_index", "idx"), &MeshMerger::remove_index);
	ClassDB::bind_method(D_METHOD("add_indices", "indice"), &MeshMerger::add_indices);

	ClassDB::bind_method(D_METHOD("reset"), &MeshMerger::reset);

	ClassDB::bind_method(D_METHOD("build_mesh"), &MeshMerger::build_mesh);
	ClassDB::bind_method(D_METHOD("build_mesh_into", "mesh_rid"), &MeshMerger::build_mesh_into);
	ClassDB::bind_method(D_METHOD("build_collider"), &MeshMerger::build_collider);

	ClassDB::bind_method(D_METHOD("generate_normals", "flip"), &MeshMerger::generate_normals, DEFVAL(false));

	ClassDB::bind_method(D_METHOD("remove_doubles"), &MeshMerger::remove_doubles);
	ClassDB::bind_method(D_METHOD("remove_doubles_hashed"), &MeshMerger::remove_doubles_hashed);
}
