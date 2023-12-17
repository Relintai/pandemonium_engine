/*************************************************************************/
/*  importer_mesh.cpp                                                    */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "importer_mesh.h"

#include "core/math/random_pcg.h"
#include "core/object/method_bind_ext.gen.inc"
#include "scene/resources/mesh/surface_tool.h"

#include <cstdint>

void ImporterMesh::Surface::split_normals(const LocalVector<int> &p_indices, const LocalVector<Vector3> &p_normals) {
	_split_normals(arrays, p_indices, p_normals);

	for (int i = 0; i < blend_shape_data.size(); ++i) {
		_split_normals(blend_shape_data.write[i].arrays, p_indices, p_normals);
	}
}

void ImporterMesh::Surface::_split_normals(Array &r_arrays, const LocalVector<int> &p_indices, const LocalVector<Vector3> &p_normals) {
	ERR_FAIL_COND(r_arrays.size() != RS::ARRAY_MAX);

	const PoolVector3Array &vertices = r_arrays[RS::ARRAY_VERTEX];
	int current_vertex_count = vertices.size();
	int new_vertex_count = p_indices.size();
	int final_vertex_count = current_vertex_count + new_vertex_count;
	const int *indices_ptr = p_indices.ptr();

	for (int i = 0; i < r_arrays.size(); i++) {
		if (i == RS::ARRAY_INDEX) {
			continue;
		}

		if (r_arrays[i].get_type() == Variant::NIL) {
			continue;
		}

		switch (r_arrays[i].get_type()) {
			case Variant::POOL_VECTOR3_ARRAY: {
				PoolVector3Array data = r_arrays[i];
				data.resize(final_vertex_count);
				PoolVector3Array::Write w = data.write();
				Vector3 *data_ptr = w.ptr();
				if (i == RS::ARRAY_NORMAL) {
					const Vector3 *normals_ptr = p_normals.ptr();
					memcpy(&data_ptr[current_vertex_count], normals_ptr, sizeof(Vector3) * new_vertex_count);
				} else {
					for (int j = 0; j < new_vertex_count; j++) {
						data_ptr[current_vertex_count + j] = data_ptr[indices_ptr[j]];
					}
				}
				r_arrays[i] = data;
			} break;
			case Variant::POOL_VECTOR2_ARRAY: {
				PoolVector2Array data = r_arrays[i];
				data.resize(final_vertex_count);
				PoolVector2Array::Write w = data.write();
				Vector2 *data_ptr = w.ptr();
				for (int j = 0; j < new_vertex_count; j++) {
					data_ptr[current_vertex_count + j] = data_ptr[indices_ptr[j]];
				}
				r_arrays[i] = data;
			} break;
			case Variant::POOL_REAL_ARRAY: {
				PoolRealArray data = r_arrays[i];
				int elements = data.size() / current_vertex_count;
				data.resize(final_vertex_count * elements);
				PoolRealArray::Write w = data.write();
				float *data_ptr = w.ptr();
				for (int j = 0; j < new_vertex_count; j++) {
					memcpy(&data_ptr[(current_vertex_count + j) * elements], &data_ptr[indices_ptr[j] * elements], sizeof(float) * elements);
				}
				r_arrays[i] = data;
			} break;
			case Variant::POOL_INT_ARRAY: {
				PoolIntArray data = r_arrays[i];
				int elements = data.size() / current_vertex_count;
				data.resize(final_vertex_count * elements);
				PoolIntArray::Write w = data.write();
				int32_t *data_ptr = w.ptr();
				for (int j = 0; j < new_vertex_count; j++) {
					memcpy(&data_ptr[(current_vertex_count + j) * elements], &data_ptr[indices_ptr[j] * elements], sizeof(int32_t) * elements);
				}
				r_arrays[i] = data;
			} break;
			case Variant::POOL_BYTE_ARRAY: {
				PoolByteArray data = r_arrays[i];
				int elements = data.size() / current_vertex_count;
				data.resize(final_vertex_count * elements);
				PoolByteArray::Write w = data.write();
				uint8_t *data_ptr = w.ptr();
				for (int j = 0; j < new_vertex_count; j++) {
					memcpy(&data_ptr[(current_vertex_count + j) * elements], &data_ptr[indices_ptr[j] * elements], sizeof(uint8_t) * elements);
				}
				r_arrays[i] = data;
			} break;
			case Variant::POOL_COLOR_ARRAY: {
				PoolColorArray data = r_arrays[i];
				data.resize(final_vertex_count);
				PoolColorArray::Write w = data.write();
				Color *data_ptr = w.ptr();
				for (int j = 0; j < new_vertex_count; j++) {
					data_ptr[current_vertex_count + j] = data_ptr[indices_ptr[j]];
				}
				r_arrays[i] = data;
			} break;
			default: {
				ERR_FAIL_MSG("Unhandled array type.");
			} break;
		}
	}
}

void ImporterMesh::add_blend_shape(const String &p_name) {
	ERR_FAIL_COND(surfaces.size() > 0);
	blend_shapes.push_back(p_name);
}

int ImporterMesh::get_blend_shape_count() const {
	return blend_shapes.size();
}

String ImporterMesh::get_blend_shape_name(int p_blend_shape) const {
	ERR_FAIL_INDEX_V(p_blend_shape, blend_shapes.size(), String());
	return blend_shapes[p_blend_shape];
}

void ImporterMesh::set_blend_shape_mode(Mesh::BlendShapeMode p_blend_shape_mode) {
	blend_shape_mode = p_blend_shape_mode;
}

Mesh::BlendShapeMode ImporterMesh::get_blend_shape_mode() const {
	return blend_shape_mode;
}

void ImporterMesh::add_surface(Mesh::PrimitiveType p_primitive, const Array &p_arrays, const Array &p_blend_shapes, const Dictionary &p_lods, const Ref<Material> &p_material, const String &p_name, const uint32_t p_flags) {
	ERR_FAIL_COND(p_blend_shapes.size() != blend_shapes.size());
	ERR_FAIL_COND(p_arrays.size() != Mesh::ARRAY_MAX);
	Surface s;
	s.primitive = p_primitive;
	s.arrays = p_arrays;
	s.name = p_name;
	s.flags = p_flags;

	Vector<Vector3> vertex_array = p_arrays[Mesh::ARRAY_VERTEX];
	int vertex_count = vertex_array.size();
	ERR_FAIL_COND(vertex_count == 0);

	for (int i = 0; i < blend_shapes.size(); i++) {
		Array bsdata = p_blend_shapes[i];
		ERR_FAIL_COND(bsdata.size() != Mesh::ARRAY_MAX);
		Vector<Vector3> vertex_data = bsdata[Mesh::ARRAY_VERTEX];
		ERR_FAIL_COND(vertex_data.size() != vertex_count);
		Surface::BlendShape bs;
		bs.arrays = bsdata;
		s.blend_shape_data.push_back(bs);
	}

	List<Variant> lods;
	p_lods.get_key_list(&lods);
	for (List<Variant>::Element *E = lods.front(); E; E = E->next()) {
		Variant e = E->get();

		ERR_CONTINUE(!e.is_num());
		Surface::LOD lod;
		lod.distance = e;
		lod.indices = p_lods[e];
		ERR_CONTINUE(lod.indices.size() == 0);
		s.lods.push_back(lod);
	}

	s.material = p_material;

	surfaces.push_back(s);
	mesh.unref();
}

int ImporterMesh::get_surface_count() const {
	return surfaces.size();
}

Mesh::PrimitiveType ImporterMesh::get_surface_primitive_type(int p_surface) {
	ERR_FAIL_INDEX_V(p_surface, surfaces.size(), Mesh::PRIMITIVE_TRIANGLES);
	return surfaces[p_surface].primitive;
}
Array ImporterMesh::get_surface_arrays(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, surfaces.size(), Array());
	return surfaces[p_surface].arrays;
}
String ImporterMesh::get_surface_name(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, surfaces.size(), String());
	return surfaces[p_surface].name;
}
void ImporterMesh::set_surface_name(int p_surface, const String &p_name) {
	ERR_FAIL_INDEX(p_surface, surfaces.size());
	surfaces.write[p_surface].name = p_name;
	mesh.unref();
}

Array ImporterMesh::get_surface_blend_shape_arrays(int p_surface, int p_blend_shape) const {
	ERR_FAIL_INDEX_V(p_surface, surfaces.size(), Array());
	ERR_FAIL_INDEX_V(p_blend_shape, surfaces[p_surface].blend_shape_data.size(), Array());
	return surfaces[p_surface].blend_shape_data[p_blend_shape].arrays;
}
int ImporterMesh::get_surface_lod_count(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, surfaces.size(), 0);
	return surfaces[p_surface].lods.size();
}
Vector<int> ImporterMesh::get_surface_lod_indices(int p_surface, int p_lod) const {
	ERR_FAIL_INDEX_V(p_surface, surfaces.size(), Vector<int>());
	ERR_FAIL_INDEX_V(p_lod, surfaces[p_surface].lods.size(), Vector<int>());

	return surfaces[p_surface].lods[p_lod].indices;
}

float ImporterMesh::get_surface_lod_size(int p_surface, int p_lod) const {
	ERR_FAIL_INDEX_V(p_surface, surfaces.size(), 0);
	ERR_FAIL_INDEX_V(p_lod, surfaces[p_surface].lods.size(), 0);
	return surfaces[p_surface].lods[p_lod].distance;
}

uint32_t ImporterMesh::get_surface_format(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, surfaces.size(), 0);
	return surfaces[p_surface].flags;
}

Ref<Material> ImporterMesh::get_surface_material(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, surfaces.size(), Ref<Material>());
	return surfaces[p_surface].material;
}

void ImporterMesh::set_surface_material(int p_surface, const Ref<Material> &p_material) {
	ERR_FAIL_INDEX(p_surface, surfaces.size());
	surfaces.write[p_surface].material = p_material;
	mesh.unref();
}

bool ImporterMesh::has_mesh() const {
	return mesh.is_valid();
}

Ref<ArrayMesh> ImporterMesh::get_mesh(const Ref<ArrayMesh> &p_base) {
	ERR_FAIL_COND_V(surfaces.size() == 0, Ref<ArrayMesh>());

	if (mesh.is_null()) {
		if (p_base.is_valid()) {
			mesh = p_base;
		}
		if (mesh.is_null()) {
			mesh.instance();
		}
		mesh->set_name(get_name());
		if (has_meta("import_id")) {
			mesh->set_meta("import_id", get_meta("import_id"));
		}
		for (int i = 0; i < blend_shapes.size(); i++) {
			mesh->add_blend_shape(blend_shapes[i]);
		}
		mesh->set_blend_shape_mode(blend_shape_mode);
		for (int i = 0; i < surfaces.size(); i++) {
			Array bs_data;
			if (surfaces[i].blend_shape_data.size()) {
				for (int j = 0; j < surfaces[i].blend_shape_data.size(); j++) {
					bs_data.push_back(surfaces[i].blend_shape_data[j].arrays);
				}
			}

			mesh->add_surface_from_arrays(surfaces[i].primitive, surfaces[i].arrays, bs_data, surfaces[i].flags);
			if (surfaces[i].material.is_valid()) {
				mesh->surface_set_material(mesh->get_surface_count() - 1, surfaces[i].material);
			}
			if (!surfaces[i].name.empty()) {
				mesh->surface_set_name(mesh->get_surface_count() - 1, surfaces[i].name);
			}
		}
	}

	return mesh;
}

void ImporterMesh::clear() {
	surfaces.clear();
	blend_shapes.clear();
	mesh.unref();
}

void ImporterMesh::_set_data(const Dictionary &p_data) {
	clear();
	if (p_data.has("blend_shape_names")) {
		blend_shapes = p_data["blend_shape_names"];
	}
	if (p_data.has("surfaces")) {
		Array surface_arr = p_data["surfaces"];
		for (int i = 0; i < surface_arr.size(); i++) {
			Dictionary s = surface_arr[i];
			ERR_CONTINUE(!s.has("primitive"));
			ERR_CONTINUE(!s.has("arrays"));
			Mesh::PrimitiveType prim = Mesh::PrimitiveType(int(s["primitive"]));
			ERR_CONTINUE(prim >= Mesh::PRIMITIVE_TRIANGLES);
			Array arr = s["arrays"];
			Dictionary lods;
			String name;
			if (s.has("name")) {
				name = s["name"];
			}
			if (s.has("lods")) {
				lods = s["lods"];
			}
			Array b_shapes;
			if (s.has("b_shapes")) {
				b_shapes = s["b_shapes"];
			}
			Ref<Material> material;
			if (s.has("material")) {
				material = s["material"];
			}
			uint32_t flags = 0;
			if (s.has("flags")) {
				flags = s["flags"];
			}
			add_surface(prim, arr, b_shapes, lods, material, name, flags);
		}
	}
}

Dictionary ImporterMesh::_get_data() const {
	Dictionary data;
	if (blend_shapes.size()) {
		data["blend_shape_names"] = blend_shapes;
	}
	Array surface_arr;
	for (int i = 0; i < surfaces.size(); i++) {
		Dictionary d;
		d["primitive"] = surfaces[i].primitive;
		d["arrays"] = surfaces[i].arrays;
		if (surfaces[i].blend_shape_data.size()) {
			Array bs_data;
			for (int j = 0; j < surfaces[i].blend_shape_data.size(); j++) {
				bs_data.push_back(surfaces[i].blend_shape_data[j].arrays);
			}
			d["blend_shapes"] = bs_data;
		}
		if (surfaces[i].lods.size()) {
			Dictionary lods;
			for (int j = 0; j < surfaces[i].lods.size(); j++) {
				lods[surfaces[i].lods[j].distance] = surfaces[i].lods[j].indices;
			}
			d["lods"] = lods;
		}

		if (surfaces[i].material.is_valid()) {
			d["material"] = surfaces[i].material;
		}

		if (!surfaces[i].name.empty()) {
			d["name"] = surfaces[i].name;
		}

		if (surfaces[i].flags != 0) {
			d["flags"] = surfaces[i].flags;
		}

		surface_arr.push_back(d);
	}
	data["surfaces"] = surface_arr;
	return data;
}

Vector<Face3> ImporterMesh::get_faces() const {
	Vector<Face3> faces;
	for (int i = 0; i < surfaces.size(); i++) {
		if (surfaces[i].primitive == Mesh::PRIMITIVE_TRIANGLES) {
			Vector<Vector3> vertices = surfaces[i].arrays[Mesh::ARRAY_VERTEX];
			Vector<int> indices = surfaces[i].arrays[Mesh::ARRAY_INDEX];
			if (indices.size()) {
				for (int j = 0; j < indices.size(); j += 3) {
					Face3 f;
					f.vertex[0] = vertices[indices[j + 0]];
					f.vertex[1] = vertices[indices[j + 1]];
					f.vertex[2] = vertices[indices[j + 2]];
					faces.push_back(f);
				}
			} else {
				for (int j = 0; j < vertices.size(); j += 3) {
					Face3 f;
					f.vertex[0] = vertices[j + 0];
					f.vertex[1] = vertices[j + 1];
					f.vertex[2] = vertices[j + 2];
					faces.push_back(f);
				}
			}
		}
	}

	return faces;
}

Ref<Shape> ImporterMesh::create_trimesh_shape() const {
	Vector<Face3> faces = get_faces();
	if (faces.size() == 0) {
		return Ref<Shape>();
	}

	PoolVector<Vector3> face_points;
	face_points.resize(faces.size() * 3);

	for (int i = 0; i < face_points.size(); i += 3) {
		Face3 f = faces.get(i / 3);
		face_points.set(i, f.vertex[0]);
		face_points.set(i + 1, f.vertex[1]);
		face_points.set(i + 2, f.vertex[2]);
	}

	Ref<ConcavePolygonShape> shape = memnew(ConcavePolygonShape);
	shape->set_faces(face_points);
	return shape;
}

Ref<NavigationMesh> ImporterMesh::create_navigation_mesh() {
	Vector<Face3> faces = get_faces();
	if (faces.size() == 0) {
		return Ref<NavigationMesh>();
	}

	HashMap<Vector3, int> unique_vertices;
	LocalVector<int> face_indices;

	for (int i = 0; i < faces.size(); i++) {
		for (int j = 0; j < 3; j++) {
			Vector3 v = faces[i].vertex[j];
			int idx;
			if (unique_vertices.has(v)) {
				idx = unique_vertices[v];
			} else {
				idx = unique_vertices.size();
				unique_vertices[v] = idx;
			}
			face_indices.push_back(idx);
		}
	}

	PoolVector<Vector3> vertices;
	vertices.resize(unique_vertices.size());

	const Vector3 *key = NULL;
	while ((key = unique_vertices.next(key))) {
		Vector3 k = *key;
		vertices.set(unique_vertices[k], k);
	}

	Ref<NavigationMesh> nm;
	nm.instance();
	nm->set_vertices(vertices);

	Vector<int> v3;
	v3.resize(3);
	for (uint32_t i = 0; i < face_indices.size(); i += 3) {
		v3.write[0] = face_indices[i + 0];
		v3.write[1] = face_indices[i + 1];
		v3.write[2] = face_indices[i + 2];
		nm->add_polygon(v3);
	}

	return nm;
}

struct EditorSceneFormatImporterMeshLightmapSurface {
	Ref<Material> material;
	LocalVector<SurfaceTool::Vertex> vertices;
	Mesh::PrimitiveType primitive = Mesh::PrimitiveType::PRIMITIVE_TRIANGLES;
	uint32_t format = 0;
	String name;
};

void ImporterMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_blend_shape", "name"), &ImporterMesh::add_blend_shape);
	ClassDB::bind_method(D_METHOD("get_blend_shape_count"), &ImporterMesh::get_blend_shape_count);
	ClassDB::bind_method(D_METHOD("get_blend_shape_name", "blend_shape_idx"), &ImporterMesh::get_blend_shape_name);

	ClassDB::bind_method(D_METHOD("set_blend_shape_mode", "mode"), &ImporterMesh::set_blend_shape_mode);
	ClassDB::bind_method(D_METHOD("get_blend_shape_mode"), &ImporterMesh::get_blend_shape_mode);

	ClassDB::bind_method(D_METHOD("add_surface", "primitive", "arrays", "blend_shapes", "lods", "material", "name", "flags"), &ImporterMesh::add_surface, DEFVAL(Array()), DEFVAL(Dictionary()), DEFVAL(Ref<Material>()), DEFVAL(String()), DEFVAL(0));

	ClassDB::bind_method(D_METHOD("get_surface_count"), &ImporterMesh::get_surface_count);
	ClassDB::bind_method(D_METHOD("get_surface_primitive_type", "surface_idx"), &ImporterMesh::get_surface_primitive_type);
	ClassDB::bind_method(D_METHOD("get_surface_name", "surface_idx"), &ImporterMesh::get_surface_name);
	ClassDB::bind_method(D_METHOD("get_surface_arrays", "surface_idx"), &ImporterMesh::get_surface_arrays);
	ClassDB::bind_method(D_METHOD("get_surface_blend_shape_arrays", "surface_idx", "blend_shape_idx"), &ImporterMesh::get_surface_blend_shape_arrays);
	ClassDB::bind_method(D_METHOD("get_surface_lod_count", "surface_idx"), &ImporterMesh::get_surface_lod_count);
	ClassDB::bind_method(D_METHOD("get_surface_lod_size", "surface_idx", "lod_idx"), &ImporterMesh::get_surface_lod_size);
	ClassDB::bind_method(D_METHOD("get_surface_lod_indices", "surface_idx", "lod_idx"), &ImporterMesh::get_surface_lod_indices);
	ClassDB::bind_method(D_METHOD("get_surface_material", "surface_idx"), &ImporterMesh::get_surface_material);
	ClassDB::bind_method(D_METHOD("get_surface_format", "surface_idx"), &ImporterMesh::get_surface_format);

	ClassDB::bind_method(D_METHOD("set_surface_name", "surface_idx", "name"), &ImporterMesh::set_surface_name);
	ClassDB::bind_method(D_METHOD("set_surface_material", "surface_idx", "material"), &ImporterMesh::set_surface_material);

	ClassDB::bind_method(D_METHOD("get_mesh", "base_mesh"), &ImporterMesh::get_mesh, DEFVAL(Ref<ArrayMesh>()));
	ClassDB::bind_method(D_METHOD("clear"), &ImporterMesh::clear);

	ClassDB::bind_method(D_METHOD("_set_data", "data"), &ImporterMesh::_set_data);
	ClassDB::bind_method(D_METHOD("_get_data"), &ImporterMesh::_get_data);

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "_data", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR), "_set_data", "_get_data");
}
