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

#include "mesh_data_resource.h"

#include "core/version.h"

#if VERSION_MAJOR >= 4
#include "core/variant/variant.h"

#define PoolVector Vector
#define POOL_INT_ARRAY PACKED_INT_ARRAY
#else
#include "core/variant.h"
#endif

const String MeshDataResource::BINDING_STRING_COLLIDER_TYPE = "None,Trimesh Collision Shape,Single Convex Collision Shape,Multiple Convex Collision Shapes,Approximated Box,Approximated Capsule,Approximated Cylinder,Approximated Sphere";

Array MeshDataResource::get_array() {
	return _arrays;
}
void MeshDataResource::set_array(const Array &p_arrays) {
	_arrays = p_arrays;

	recompute_aabb();

	emit_changed();
}
Array MeshDataResource::get_array_const() const {
	return _arrays;
}

AABB MeshDataResource::get_aabb() const {
	return _aabb;
}
void MeshDataResource::set_aabb(const AABB &aabb) {
	_aabb = aabb;

	emit_changed();
}

void MeshDataResource::add_collision_shape(const Transform &transform, const Ref<Shape> &shape) {
	MDRData d;

	d.transform = transform;
	d.shape = shape;

	_collision_shapes.push_back(d);

	emit_changed();
}
Ref<Shape> MeshDataResource::get_collision_shape(const int index) {
	ERR_FAIL_INDEX_V(index, _collision_shapes.size(), Ref<Shape>());

	return _collision_shapes[index].shape;
}
Transform MeshDataResource::get_collision_shape_offset(const int index) {
	ERR_FAIL_INDEX_V(index, _collision_shapes.size(), Transform());

	return _collision_shapes[index].transform;
}
int MeshDataResource::get_collision_shape_count() const {
	return _collision_shapes.size();
}

Vector<Variant> MeshDataResource::get_collision_shapes() {
	Vector<Variant> r;
	for (int i = 0; i < _collision_shapes.size(); i++) {
#if VERSION_MAJOR < 4
		r.push_back(_collision_shapes[i].transform);
		r.push_back(_collision_shapes[i].shape.get_ref_ptr());
#else
		r.push_back(_collision_shapes[i].transform);
		r.push_back(_collision_shapes[i].shape);
#endif
	}
	return r;
}
void MeshDataResource::set_collision_shapes(const Vector<Variant> &p_arrays) {
	ERR_FAIL_COND(p_arrays.size() % 2 == 1);

	_collision_shapes.clear();
	for (int i = 0; i < p_arrays.size(); i += 2) {
		MDRData d;

		d.transform = p_arrays[i];
		d.shape = Ref<Shape>(p_arrays[i + 1]);

		_collision_shapes.push_back(d);
	}

	emit_changed();
}

PoolIntArray MeshDataResource::get_seams() {
	return _seams;
}

void MeshDataResource::set_seams(const PoolIntArray &array) {
	_seams = array;

	emit_changed();
}

void MeshDataResource::append_arrays(const Array &p_arrays) {
	if (p_arrays.size() != Mesh::ARRAY_MAX) {
		return;
	}

	if (_arrays.size() != Mesh::ARRAY_MAX) {
		_arrays = p_arrays;
		return;
	}

	PoolVector3Array vertices = _arrays[Mesh::ARRAY_VERTEX];

	if (vertices.size() == 0) {
		_arrays = p_arrays;
		return;
	}

	PoolVector3Array normals = _arrays[Mesh::ARRAY_NORMAL];
	PoolRealArray tangents = _arrays[Mesh::ARRAY_TANGENT];
	PoolColorArray colors = _arrays[Mesh::ARRAY_COLOR];
	PoolVector2Array uv = _arrays[Mesh::ARRAY_TEX_UV];
	PoolVector2Array uv2 = _arrays[Mesh::ARRAY_TEX_UV2];
	PoolRealArray bones = _arrays[Mesh::ARRAY_BONES];
	PoolRealArray weights = _arrays[Mesh::ARRAY_WEIGHTS];
	PoolIntArray indices = _arrays[Mesh::ARRAY_INDEX];

	PoolVector3Array merge_vertices = p_arrays[Mesh::ARRAY_VERTEX];
	PoolVector3Array merge_normals = p_arrays[Mesh::ARRAY_NORMAL];
	PoolRealArray merge_tangents = p_arrays[Mesh::ARRAY_TANGENT];
	PoolColorArray merge_colors = p_arrays[Mesh::ARRAY_COLOR];
	PoolVector2Array merge_uv = p_arrays[Mesh::ARRAY_TEX_UV];
	PoolVector2Array merge_uv2 = p_arrays[Mesh::ARRAY_TEX_UV2];
	PoolRealArray merge_bones = p_arrays[Mesh::ARRAY_BONES];
	PoolRealArray merge_weights = p_arrays[Mesh::ARRAY_WEIGHTS];
	PoolIntArray merge_indices = p_arrays[Mesh::ARRAY_INDEX];

	//merge

	int ovc = vertices.size();
	vertices.append_array(merge_vertices);

	if (_arrays[Mesh::ARRAY_NORMAL] != Variant()) {
		if (merge_vertices.size() != merge_normals.size()) {
			for (int i = 0; i < merge_vertices.size(); ++i) {
				normals.append(Vector3());
			}
		} else {
			normals.append_array(merge_normals);
		}
	}

	if (_arrays[Mesh::ARRAY_TANGENT] != Variant()) {
		if (merge_vertices.size() != merge_tangents.size() * 4) {
			for (int i = 0; i < merge_vertices.size(); ++i) {
				merge_tangents.append(0);
				merge_tangents.append(0);
				merge_tangents.append(0);
				merge_tangents.append(0);
			}
		} else {
			tangents.append_array(merge_tangents);
		}
	}

	if (_arrays[Mesh::ARRAY_COLOR] != Variant()) {
		if (merge_vertices.size() != merge_colors.size()) {
			for (int i = 0; i < merge_vertices.size(); ++i) {
				colors.append(Color());
			}
		} else {
			colors.append_array(merge_colors);
		}
	}

	if (_arrays[Mesh::ARRAY_TEX_UV] != Variant()) {
		if (merge_vertices.size() != merge_uv.size()) {
			for (int i = 0; i < merge_vertices.size(); ++i) {
				uv.append(Vector2());
			}
		} else {
			uv.append_array(merge_uv);
		}
	}

	if (_arrays[Mesh::ARRAY_TEX_UV2] != Variant()) {
		if (merge_vertices.size() != merge_uv2.size()) {
			for (int i = 0; i < merge_vertices.size(); ++i) {
				uv2.append(Vector2());
			}
		} else {
			uv2.append_array(merge_uv2);
		}
	}

	if (_arrays[Mesh::ARRAY_BONES] != Variant()) {
		if (merge_vertices.size() != merge_bones.size() * 4) {
			for (int i = 0; i < merge_vertices.size(); ++i) {
				bones.append(0);
				bones.append(0);
				bones.append(0);
				bones.append(0);
			}
		} else {
			bones.append_array(merge_bones);
		}
	}

	if (_arrays[Mesh::ARRAY_WEIGHTS] != Variant()) {
		if (merge_vertices.size() != merge_weights.size() * 4) {
			for (int i = 0; i < merge_vertices.size(); ++i) {
				weights.append(0);
				weights.append(0);
				weights.append(0);
				weights.append(0);
			}
		} else {
			weights.append_array(merge_weights);
		}
	}

	for (int i = 0; i < merge_indices.size(); ++i) {
		merge_indices.set(i, merge_indices[i] + ovc);
	}

	indices.append_array(merge_indices);

	//write back

	_arrays[Mesh::ARRAY_VERTEX] = vertices;

	if (_arrays[Mesh::ARRAY_NORMAL] != Variant()) {
		_arrays[Mesh::ARRAY_NORMAL] = normals;
	}

	if (_arrays[Mesh::ARRAY_TANGENT] != Variant()) {
		_arrays[Mesh::ARRAY_TANGENT] = tangents;
	}

	if (_arrays[Mesh::ARRAY_COLOR] != Variant()) {
		_arrays[Mesh::ARRAY_COLOR] = colors;
	}

	if (_arrays[Mesh::ARRAY_TEX_UV] != Variant()) {
		_arrays[Mesh::ARRAY_TEX_UV] = uv;
	}

	if (_arrays[Mesh::ARRAY_TEX_UV2] != Variant()) {
		_arrays[Mesh::ARRAY_TEX_UV2] = uv2;
	}

	if (_arrays[Mesh::ARRAY_BONES] != Variant()) {
		_arrays[Mesh::ARRAY_BONES] = bones;
	}

	if (_arrays[Mesh::ARRAY_WEIGHTS] != Variant()) {
		_arrays[Mesh::ARRAY_WEIGHTS] = weights;
	}

	_arrays[Mesh::ARRAY_INDEX] = indices;

	emit_changed();
}

void MeshDataResource::recompute_aabb() {
	if (_arrays.size() == 0) {
		return;
	}

	Variant arr = _arrays[Mesh::ARRAY_VERTEX];

	PoolVector<Vector2> vertices_2d = arr;

	if (vertices_2d.size() > 0) {
		AABB aabb;

#if VERSION_MAJOR < 4
		PoolVector<Vector2>::Read r = vertices_2d.read();
		const Vector2 *vtx = r.ptr();
#else
		const Vector2 *vtx = vertices.ptr();
#endif
		int len = vertices_2d.size();
		aabb.position = Vector3(vtx[0].x, vtx[0].y, 0);

		for (int i = 0; i < len; i++) {
			aabb.expand_to(Vector3(vtx[i].x, vtx[i].y, 0));
		}

		_aabb = aabb;
		return;
	}

	PoolVector<Vector3> vertices = arr;
	int len = vertices.size();

	if (len == 0) {
		return;
	}

#if VERSION_MAJOR < 4
	PoolVector<Vector3>::Read r = vertices.read();
	const Vector3 *vtx = r.ptr();
#else
	const Vector3 *vtx = vertices.ptr();
#endif

	AABB aabb;
	for (int i = 0; i < len; i++) {
		if (i == 0)
			aabb.position = vtx[i];
		else
			aabb.expand_to(vtx[i]);
	}

	_aabb = aabb;
}

MeshDataResource::MeshDataResource() {
}

MeshDataResource::~MeshDataResource() {
	_arrays.clear();
	_collision_shapes.clear();
}

void MeshDataResource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_array"), &MeshDataResource::get_array);
	ClassDB::bind_method(D_METHOD("set_array", "array"), &MeshDataResource::set_array);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "array"), "set_array", "get_array");

	ClassDB::bind_method(D_METHOD("get_aabb"), &MeshDataResource::get_aabb);
	ClassDB::bind_method(D_METHOD("set_aabb", "array"), &MeshDataResource::set_aabb);
	ADD_PROPERTY(PropertyInfo(Variant::AABB, "aabb"), "set_aabb", "get_aabb");

	ClassDB::bind_method(D_METHOD("get_collision_shapes"), &MeshDataResource::get_collision_shapes);
	ClassDB::bind_method(D_METHOD("set_collision_shapes", "array"), &MeshDataResource::set_collision_shapes);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "collision_shapes"), "set_collision_shapes", "get_collision_shapes");

	ClassDB::bind_method(D_METHOD("get_seams"), &MeshDataResource::get_seams);
	ClassDB::bind_method(D_METHOD("set_seams", "array"), &MeshDataResource::set_seams);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_INT_ARRAY, "seams"), "set_seams", "get_seams");

	ClassDB::bind_method(D_METHOD("add_collision_shape", "shape"), &MeshDataResource::add_collision_shape);
	ClassDB::bind_method(D_METHOD("get_collision_shape", "index"), &MeshDataResource::get_collision_shape);
	ClassDB::bind_method(D_METHOD("get_collision_shape_count"), &MeshDataResource::get_collision_shape_count);

	ClassDB::bind_method(D_METHOD("append_arrays", "array"), &MeshDataResource::append_arrays);

	ClassDB::bind_method(D_METHOD("recompute_aabb"), &MeshDataResource::recompute_aabb);
}
