/*************************************************************************/
/*  mesh.cpp                                                             */
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

#include "mesh.h"

#include "core/containers/local_vector.h"
#include "core/containers/pair.h"
#include "core/crypto/crypto_core.h"
#include "core/math/convex_hull.h"
#include "scene/resources/shapes/concave_polygon_shape.h"
#include "scene/resources/shapes/convex_polygon_shape.h"
#include "surface_tool.h"

#include <stdlib.h>

Mesh::ConvexDecompositionFunc Mesh::convex_decomposition_function = nullptr;

int Mesh::surface_get_triangle_count(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, get_surface_count(), 0);

	switch (surface_get_primitive_type(p_idx)) {
		case PRIMITIVE_TRIANGLES: {
			int len = (surface_get_format(p_idx) & ARRAY_FORMAT_INDEX) ? surface_get_array_index_len(p_idx) : surface_get_array_len(p_idx);
			// Don't error if zero, it's valid (we'll just skip it later).
			ERR_FAIL_COND_V_MSG((len % 3) != 0, 0, vformat("Ignoring surface %d, incorrect %s count: %d (for PRIMITIVE_TRIANGLES).", p_idx, (surface_get_format(p_idx) & ARRAY_FORMAT_INDEX) ? "index" : "vertex", len));
			return len / 3;
		} break;
		case PRIMITIVE_TRIANGLE_FAN:
		case PRIMITIVE_TRIANGLE_STRIP: {
			int len = (surface_get_format(p_idx) & ARRAY_FORMAT_INDEX) ? surface_get_array_index_len(p_idx) : surface_get_array_len(p_idx);
			// Don't error if zero, it's valid (we'll just skip it later).
			ERR_FAIL_COND_V_MSG(len != 0 && len < 3, 0, vformat("Ignoring surface %d, incorrect %s count: %d (for %s).", p_idx, (surface_get_format(p_idx) & ARRAY_FORMAT_INDEX) ? "index" : "vertex", len, (surface_get_primitive_type(p_idx) == PRIMITIVE_TRIANGLE_FAN) ? "PRIMITIVE_TRIANGLE_FAN" : "PRIMITIVE_TRIANGLE_STRIP"));
			return (len == 0) ? 0 : (len - 2);
		} break;
		default: {
		} break;
	}

	return 0;
}

int Mesh::get_triangle_count() const {
	int triangle_count = 0;

	for (int i = 0; i < get_surface_count(); i++) {
		triangle_count += surface_get_triangle_count(i);
	}

	return triangle_count;
}

Ref<TriangleMesh> Mesh::generate_triangle_mesh_from_aabb() const {
	AABB aabb = get_aabb();

	Vector3 pts[8];
	Vector3 s = aabb.position;
	Vector3 l = s + aabb.size;

	pts[0] = Vector3(s.x, s.y, s.z);
	pts[1] = Vector3(l.x, s.y, s.z);
	pts[2] = Vector3(l.x, l.y, s.z);
	pts[3] = Vector3(s.x, l.y, s.z);
	pts[4] = Vector3(l.x, l.y, l.z);
	pts[5] = Vector3(s.x, l.y, l.z);
	pts[6] = Vector3(s.x, s.y, l.z);
	pts[7] = Vector3(l.x, s.y, l.z);

	PoolVector<Vector3> face_pts;
	face_pts.resize(6 * 2 * 3);
	PoolVector<Vector3>::Write w = face_pts.write();
	int wc = 0;
	w[wc++] = pts[0];
	w[wc++] = pts[1];
	w[wc++] = pts[2];
	w[wc++] = pts[0];
	w[wc++] = pts[2];
	w[wc++] = pts[3];

	w[wc++] = pts[6];
	w[wc++] = pts[5];
	w[wc++] = pts[4];
	w[wc++] = pts[6];
	w[wc++] = pts[4];
	w[wc++] = pts[7];

	w[wc++] = pts[1];
	w[wc++] = pts[7];
	w[wc++] = pts[4];
	w[wc++] = pts[1];
	w[wc++] = pts[4];
	w[wc++] = pts[2];

	w[wc++] = pts[0];
	w[wc++] = pts[3];
	w[wc++] = pts[5];
	w[wc++] = pts[0];
	w[wc++] = pts[5];
	w[wc++] = pts[6];

	w[wc++] = pts[0];
	w[wc++] = pts[6];
	w[wc++] = pts[7];
	w[wc++] = pts[0];
	w[wc++] = pts[7];
	w[wc++] = pts[1];

	w[wc++] = pts[2];
	w[wc++] = pts[4];
	w[wc++] = pts[5];
	w[wc++] = pts[2];
	w[wc++] = pts[5];
	w[wc++] = pts[3];

	w.release();

	Ref<TriangleMesh> tmesh = Ref<TriangleMesh>(memnew(TriangleMesh));
	tmesh->create(face_pts);

	return tmesh;
}

Ref<TriangleMesh> Mesh::generate_triangle_mesh() const {
	if (triangle_mesh.is_valid()) {
		return triangle_mesh;
	}

	int faces_vertex_count = get_triangle_count() * 3;

	if (faces_vertex_count == 0) {
		return triangle_mesh;
	}

	PoolVector<Vector3> faces;
	faces.resize(faces_vertex_count);
	PoolVector<Vector3>::Write facesw = faces.write();

	int widx = 0;

	for (int i = 0; i < get_surface_count(); i++) {
		Mesh::PrimitiveType primitive = surface_get_primitive_type(i);
		if (primitive != PRIMITIVE_TRIANGLES && primitive != PRIMITIVE_TRIANGLE_FAN && primitive != PRIMITIVE_TRIANGLE_STRIP) {
			continue;
		}
		int len = (surface_get_format(i) & ARRAY_FORMAT_INDEX) ? surface_get_array_index_len(i) : surface_get_array_len(i);
		if ((primitive == PRIMITIVE_TRIANGLES && (len <= 0 || (len % 3) != 0)) || ((primitive == PRIMITIVE_TRIANGLE_FAN || primitive == PRIMITIVE_TRIANGLE_STRIP) && len < 3)) {
			// Error was already shown, just skip (including zero).
			continue;
		}

		Array a = surface_get_arrays(i);
		ERR_FAIL_COND_V(a.empty(), Ref<TriangleMesh>());

		int vc = surface_get_array_len(i);
		PoolVector<Vector3> vertices = a[ARRAY_VERTEX];
		PoolVector<Vector3>::Read vr = vertices.read();

		if (surface_get_format(i) & ARRAY_FORMAT_INDEX) {
			int ic = surface_get_array_index_len(i);
			PoolVector<int> indices = a[ARRAY_INDEX];
			PoolVector<int>::Read ir = indices.read();

			if (primitive == PRIMITIVE_TRIANGLES) {
				for (int j = 0; j < ic; j++) {
					int index = ir[j];
					facesw[widx++] = vr[index];
				}
			} else { // PRIMITIVE_TRIANGLE_FAN, PRIMITIVE_TRIANGLE_STRIP
				for (int j = 2; j < ic; j++) {
					facesw[widx++] = vr[ir[(primitive == PRIMITIVE_TRIANGLE_FAN) ? 0 : j - 2]];
					facesw[widx++] = vr[ir[j - 1]];
					facesw[widx++] = vr[ir[j]];
				}
			}

		} else {
			if (primitive == PRIMITIVE_TRIANGLES) {
				for (int j = 0; j < vc; j++) {
					facesw[widx++] = vr[j];
				}
			} else { // PRIMITIVE_TRIANGLE_FAN, PRIMITIVE_TRIANGLE_STRIP
				for (int j = 2; j < vc; j++) {
					facesw[widx++] = vr[(primitive == PRIMITIVE_TRIANGLE_FAN) ? 0 : j - 2];
					facesw[widx++] = vr[j - 1];
					facesw[widx++] = vr[j];
				}
			}
		}
	}

	facesw.release();

	triangle_mesh = Ref<TriangleMesh>(memnew(TriangleMesh));
	triangle_mesh->create(faces);

	return triangle_mesh;
}

void Mesh::generate_debug_mesh_lines(Vector<Vector3> &r_lines) {
	if (debug_lines.size() > 0) {
		r_lines = debug_lines;
		return;
	}

	Ref<TriangleMesh> tm = generate_triangle_mesh();
	if (tm.is_null()) {
		return;
	}

	PoolVector<int> triangle_indices;
	tm->get_indices(&triangle_indices);
	const int triangles_num = tm->get_triangles().size();
	PoolVector<Vector3> vertices = tm->get_vertices();

	debug_lines.resize(tm->get_triangles().size() * 6); // 3 lines x 2 points each line

	PoolVector<int>::Read ind_r = triangle_indices.read();
	PoolVector<Vector3>::Read ver_r = vertices.read();
	for (int j = 0, x = 0, i = 0; i < triangles_num; j += 6, x += 3, ++i) {
		// Triangle line 1
		debug_lines.write[j + 0] = ver_r[ind_r[x + 0]];
		debug_lines.write[j + 1] = ver_r[ind_r[x + 1]];

		// Triangle line 2
		debug_lines.write[j + 2] = ver_r[ind_r[x + 1]];
		debug_lines.write[j + 3] = ver_r[ind_r[x + 2]];

		// Triangle line 3
		debug_lines.write[j + 4] = ver_r[ind_r[x + 2]];
		debug_lines.write[j + 5] = ver_r[ind_r[x + 0]];
	}

	r_lines = debug_lines;
}
void Mesh::generate_debug_mesh_indices(Vector<Vector3> &r_points) {
	Ref<TriangleMesh> tm = generate_triangle_mesh();
	if (tm.is_null()) {
		return;
	}

	PoolVector<Vector3> vertices = tm->get_vertices();

	int vertices_size = vertices.size();
	r_points.resize(vertices_size);
	for (int i = 0; i < vertices_size; ++i) {
		r_points.write[i] = vertices[i];
	}
}

bool Mesh::surface_is_softbody_friendly(int p_idx) const {
	const uint32_t surface_format = surface_get_format(p_idx);
	return (surface_format & Mesh::ARRAY_FLAG_USE_DYNAMIC_UPDATE && (!(surface_format & Mesh::ARRAY_COMPRESS_VERTEX)) && (!(surface_format & Mesh::ARRAY_COMPRESS_NORMAL)));
}

PoolVector<Face3> Mesh::get_faces() const {
	Ref<TriangleMesh> tm = generate_triangle_mesh();
	if (tm.is_valid()) {
		return tm->get_faces();
	}
	return PoolVector<Face3>();
}

Ref<Shape> Mesh::create_convex_shape(bool p_clean, bool p_simplify) const {
	if (p_simplify) {
		Vector<Ref<Shape>> decomposed = convex_decompose(1);
		if (decomposed.size() == 1) {
			return decomposed[0];
		} else {
			ERR_PRINT("Convex shape simplification failed, falling back to simpler process.");
		}
	}

	PoolVector<Vector3> vertices;
	for (int i = 0; i < get_surface_count(); i++) {
		Array a = surface_get_arrays(i);
		ERR_FAIL_COND_V(a.empty(), Ref<ConvexPolygonShape>());
		PoolVector<Vector3> v = a[ARRAY_VERTEX];
		vertices.append_array(v);
	}

	Ref<ConvexPolygonShape> shape = memnew(ConvexPolygonShape);

	if (p_clean) {
		Geometry::MeshData md;
		Error err = ConvexHullComputer::convex_hull(vertices, md);
		if (err == OK) {
			int vertex_count = md.vertices.size();
			vertices.resize(vertex_count);
			{
				PoolVector<Vector3>::Write w = vertices.write();
				for (int idx = 0; idx < vertex_count; ++idx) {
					w[idx] = md.vertices[idx];
				}
			}
		} else {
			ERR_PRINT("Convex shape cleaning failed, falling back to simpler process.");
		}
	}

	shape->set_points(vertices);
	return shape;
}

Ref<Shape> Mesh::create_trimesh_shape() const {
	PoolVector<Face3> faces = get_faces();
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

Ref<Mesh> Mesh::create_outline(float p_margin) const {
	Array arrays;
	int index_accum = 0;
	for (int i = 0; i < get_surface_count(); i++) {
		if (surface_get_primitive_type(i) != PRIMITIVE_TRIANGLES) {
			continue;
		}

		Array a = surface_get_arrays(i);
		ERR_FAIL_COND_V(a.empty(), Ref<ArrayMesh>());

		if (i == 0) {
			arrays = a;
			PoolVector<Vector3> v = a[ARRAY_VERTEX];
			index_accum += v.size();
		} else {
			int vcount = 0;
			for (int j = 0; j < arrays.size(); j++) {
				if (arrays[j].get_type() == Variant::NIL || a[j].get_type() == Variant::NIL) {
					//mismatch, do not use
					arrays[j] = Variant();
					continue;
				}

				switch (j) {
					case ARRAY_VERTEX:
					case ARRAY_NORMAL: {
						PoolVector<Vector3> dst = arrays[j];
						PoolVector<Vector3> src = a[j];
						if (j == ARRAY_VERTEX) {
							vcount = src.size();
						}
						if (dst.size() == 0 || src.size() == 0) {
							arrays[j] = Variant();
							continue;
						}
						dst.append_array(src);
						arrays[j] = dst;
					} break;
					case ARRAY_TANGENT:
					case ARRAY_BONES:
					case ARRAY_WEIGHTS: {
						PoolVector<real_t> dst = arrays[j];
						PoolVector<real_t> src = a[j];
						if (dst.size() == 0 || src.size() == 0) {
							arrays[j] = Variant();
							continue;
						}
						dst.append_array(src);
						arrays[j] = dst;

					} break;
					case ARRAY_COLOR: {
						PoolVector<Color> dst = arrays[j];
						PoolVector<Color> src = a[j];
						if (dst.size() == 0 || src.size() == 0) {
							arrays[j] = Variant();
							continue;
						}
						dst.append_array(src);
						arrays[j] = dst;

					} break;
					case ARRAY_TEX_UV:
					case ARRAY_TEX_UV2: {
						PoolVector<Vector2> dst = arrays[j];
						PoolVector<Vector2> src = a[j];
						if (dst.size() == 0 || src.size() == 0) {
							arrays[j] = Variant();
							continue;
						}
						dst.append_array(src);
						arrays[j] = dst;

					} break;
					case ARRAY_INDEX: {
						PoolVector<int> dst = arrays[j];
						PoolVector<int> src = a[j];
						if (dst.size() == 0 || src.size() == 0) {
							arrays[j] = Variant();
							continue;
						}
						{
							int ss = src.size();
							PoolVector<int>::Write w = src.write();
							for (int k = 0; k < ss; k++) {
								w[k] += index_accum;
							}
						}
						dst.append_array(src);
						arrays[j] = dst;
						index_accum += vcount;

					} break;
				}
			}
		}
	}

	ERR_FAIL_COND_V(arrays.size() != ARRAY_MAX, Ref<ArrayMesh>());

	{
		PoolVector<int>::Write ir;
		PoolVector<int> indices = arrays[ARRAY_INDEX];
		bool has_indices = false;
		PoolVector<Vector3> vertices = arrays[ARRAY_VERTEX];
		int vc = vertices.size();
		ERR_FAIL_COND_V(!vc, Ref<ArrayMesh>());
		PoolVector<Vector3>::Write r = vertices.write();

		if (indices.size()) {
			ERR_FAIL_COND_V(indices.size() % 3 != 0, Ref<ArrayMesh>());
			vc = indices.size();
			ir = indices.write();
			has_indices = true;
		}

		RBMap<Vector3, Vector3> normal_accum;

		//fill normals with triangle normals
		for (int i = 0; i < vc; i += 3) {
			Vector3 t[3];

			if (has_indices) {
				t[0] = r[ir[i + 0]];
				t[1] = r[ir[i + 1]];
				t[2] = r[ir[i + 2]];
			} else {
				t[0] = r[i + 0];
				t[1] = r[i + 1];
				t[2] = r[i + 2];
			}

			Vector3 n = Plane(t[0], t[1], t[2]).normal;

			for (int j = 0; j < 3; j++) {
				RBMap<Vector3, Vector3>::Element *E = normal_accum.find(t[j]);
				if (!E) {
					normal_accum[t[j]] = n;
				} else {
					float d = n.dot(E->get());
					if (d < 1.0) {
						E->get() += n * (1.0 - d);
					}
					//E->get()+=n;
				}
			}
		}

		//normalize

		for (RBMap<Vector3, Vector3>::Element *E = normal_accum.front(); E; E = E->next()) {
			E->get().normalize();
		}

		//displace normals
		int vc2 = vertices.size();

		for (int i = 0; i < vc2; i++) {
			Vector3 t = r[i];

			RBMap<Vector3, Vector3>::Element *E = normal_accum.find(t);
			ERR_CONTINUE(!E);

			t += E->get() * p_margin;
			r[i] = t;
		}

		r.release();
		arrays[ARRAY_VERTEX] = vertices;

		if (!has_indices) {
			PoolVector<int> new_indices;
			new_indices.resize(vertices.size());
			PoolVector<int>::Write iw = new_indices.write();

			for (int j = 0; j < vc2; j += 3) {
				iw[j] = j;
				iw[j + 1] = j + 2;
				iw[j + 2] = j + 1;
			}

			iw.release();
			arrays[ARRAY_INDEX] = new_indices;

		} else {
			for (int j = 0; j < vc; j += 3) {
				SWAP(ir[j + 1], ir[j + 2]);
			}
			ir.release();
			arrays[ARRAY_INDEX] = indices;
		}
	}

	Ref<ArrayMesh> newmesh = memnew(ArrayMesh);
	newmesh->add_surface_from_arrays(PRIMITIVE_TRIANGLES, arrays);
	return newmesh;
}

void Mesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_aabb"), &Mesh::get_aabb);

	ClassDB::bind_method(D_METHOD("get_surface_count"), &Mesh::get_surface_count);
	ClassDB::bind_method(D_METHOD("surface_get_arrays", "surf_idx"), &Mesh::surface_get_arrays);
	ClassDB::bind_method(D_METHOD("surface_get_blend_shape_arrays", "surf_idx"), &Mesh::surface_get_blend_shape_arrays);
	ClassDB::bind_method(D_METHOD("surface_set_material", "surf_idx", "material"), &Mesh::surface_set_material);
	ClassDB::bind_method(D_METHOD("surface_get_material", "surf_idx"), &Mesh::surface_get_material);

	BIND_ENUM_CONSTANT(PRIMITIVE_POINTS);
	BIND_ENUM_CONSTANT(PRIMITIVE_LINES);
	BIND_ENUM_CONSTANT(PRIMITIVE_LINE_STRIP);
	BIND_ENUM_CONSTANT(PRIMITIVE_LINE_LOOP);
	BIND_ENUM_CONSTANT(PRIMITIVE_TRIANGLES);
	BIND_ENUM_CONSTANT(PRIMITIVE_TRIANGLE_STRIP);
	BIND_ENUM_CONSTANT(PRIMITIVE_TRIANGLE_FAN);

	BIND_ENUM_CONSTANT(BLEND_SHAPE_MODE_NORMALIZED);
	BIND_ENUM_CONSTANT(BLEND_SHAPE_MODE_RELATIVE);

	BIND_ENUM_CONSTANT(ARRAY_FORMAT_VERTEX);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_NORMAL);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_TANGENT);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_COLOR);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_TEX_UV);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_TEX_UV2);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_BONES);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_WEIGHTS);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_INDEX);

	BIND_ENUM_CONSTANT(ARRAY_COMPRESS_BASE);
	BIND_ENUM_CONSTANT(ARRAY_COMPRESS_VERTEX);
	BIND_ENUM_CONSTANT(ARRAY_COMPRESS_NORMAL);
	BIND_ENUM_CONSTANT(ARRAY_COMPRESS_TANGENT);
	BIND_ENUM_CONSTANT(ARRAY_COMPRESS_COLOR);
	BIND_ENUM_CONSTANT(ARRAY_COMPRESS_TEX_UV);
	BIND_ENUM_CONSTANT(ARRAY_COMPRESS_TEX_UV2);
	BIND_ENUM_CONSTANT(ARRAY_COMPRESS_BONES);
	BIND_ENUM_CONSTANT(ARRAY_COMPRESS_WEIGHTS);
	BIND_ENUM_CONSTANT(ARRAY_COMPRESS_INDEX);

	BIND_ENUM_CONSTANT(ARRAY_FLAG_USE_2D_VERTICES);
	BIND_ENUM_CONSTANT(ARRAY_FLAG_USE_16_BIT_BONES);
	BIND_ENUM_CONSTANT(ARRAY_FLAG_USE_OCTAHEDRAL_COMPRESSION);
	BIND_ENUM_CONSTANT(ARRAY_FLAG_USE_VERTEX_CACHE_OPTIMIZATION);

	BIND_ENUM_CONSTANT(ARRAY_COMPRESS_DEFAULT);

	BIND_ENUM_CONSTANT(ARRAY_VERTEX);
	BIND_ENUM_CONSTANT(ARRAY_NORMAL);
	BIND_ENUM_CONSTANT(ARRAY_TANGENT);
	BIND_ENUM_CONSTANT(ARRAY_COLOR);
	BIND_ENUM_CONSTANT(ARRAY_TEX_UV);
	BIND_ENUM_CONSTANT(ARRAY_TEX_UV2);
	BIND_ENUM_CONSTANT(ARRAY_BONES);
	BIND_ENUM_CONSTANT(ARRAY_WEIGHTS);
	BIND_ENUM_CONSTANT(ARRAY_INDEX);
	BIND_ENUM_CONSTANT(ARRAY_MAX);
}

void Mesh::set_storage_mode(StorageMode p_storage_mode) {
}

void Mesh::clear_cache() const {
	triangle_mesh.unref();
	debug_lines.clear();
}

Vector<Ref<Shape>> Mesh::convex_decompose(int p_max_convex_hulls) const {
	ERR_FAIL_COND_V(!convex_decomposition_function, Vector<Ref<Shape>>());

	Ref<TriangleMesh> tm = generate_triangle_mesh();
	ERR_FAIL_COND_V(!tm.is_valid(), Vector<Ref<Shape>>());

	const PoolVector<TriangleMesh::Triangle> &triangles = tm->get_triangles();
	int triangle_count = triangles.size();

	PoolVector<uint32_t> indices;
	{
		indices.resize(triangle_count * 3);
		PoolVector<uint32_t>::Write w = indices.write();
		PoolVector<TriangleMesh::Triangle>::Read triangles_read = triangles.read();
		for (int i = 0; i < triangle_count; i++) {
			for (int j = 0; j < 3; j++) {
				w[i * 3 + j] = triangles_read[i].indices[j];
			}
		}
	}

	const PoolVector<Vector3> &vertices = tm->get_vertices();
	int vertex_count = vertices.size();

	Vector<PoolVector<Vector3>> decomposed = convex_decomposition_function((real_t *)vertices.read().ptr(), vertex_count, indices.read().ptr(), triangle_count, p_max_convex_hulls, nullptr);

	Vector<Ref<Shape>> ret;

	for (int i = 0; i < decomposed.size(); i++) {
		Ref<ConvexPolygonShape> shape;
		shape.instance();
		shape->set_points(decomposed[i]);
		ret.push_back(shape);
	}

	return ret;
}

Mesh::Mesh() {
}

bool ArrayMesh::_set(const StringName &p_name, const Variant &p_value) {
	String sname = p_name;

	if (p_name == "blend_shape/names") {
		PoolVector<String> sk = p_value;
		int sz = sk.size();
		PoolVector<String>::Read r = sk.read();
		for (int i = 0; i < sz; i++) {
			add_blend_shape(r[i]);
		}
		return true;
	}

	if (p_name == "blend_shape/mode") {
		set_blend_shape_mode(BlendShapeMode(int(p_value)));
		return true;
	}

	if (sname.begins_with("surface_")) {
		int sl = sname.find("/");
		if (sl == -1) {
			return false;
		}
		int idx = sname.substr(8, sl - 8).to_int() - 1;
		String what = sname.get_slicec('/', 1);
		if (what == "material") {
			surface_set_material(idx, p_value);
		} else if (what == "name") {
			surface_set_name(idx, p_value);
		}
		return true;
	}

	if (!sname.begins_with("surfaces")) {
		return false;
	}

	int idx = sname.get_slicec('/', 1).to_int();
	String what = sname.get_slicec('/', 2);

	if (idx == surfaces.size()) {
		//create
		Dictionary d = p_value;
		ERR_FAIL_COND_V(!d.has("primitive"), false);

		if (d.has("arrays")) {
			//old format
			ERR_FAIL_COND_V(!d.has("morph_arrays"), false);
			add_surface_from_arrays(PrimitiveType(int(d["primitive"])), d["arrays"], d["morph_arrays"]);

		} else if (d.has("array_data")) {
			PoolVector<uint8_t> array_data = d["array_data"];
			PoolVector<uint8_t> array_index_data;
			if (d.has("array_index_data")) {
				array_index_data = d["array_index_data"];
			}

			ERR_FAIL_COND_V(!d.has("format"), false);
			uint32_t format = d["format"];

			uint32_t primitive = d["primitive"];

			ERR_FAIL_COND_V(!d.has("vertex_count"), false);
			int vertex_count = d["vertex_count"];

			int index_count = 0;
			if (d.has("index_count")) {
				index_count = d["index_count"];
			}

			Vector<PoolVector<uint8_t>> blend_shapes;

			if (d.has("blend_shape_data")) {
				Array blend_shape_data = d["blend_shape_data"];
				for (int i = 0; i < blend_shape_data.size(); i++) {
					PoolVector<uint8_t> shape = blend_shape_data[i];
					blend_shapes.push_back(shape);
				}
			}

			ERR_FAIL_COND_V(!d.has("aabb"), false);
			AABB aabb = d["aabb"];

			Vector<AABB> bone_aabb;
			if (d.has("skeleton_aabb")) {
				Array baabb = d["skeleton_aabb"];
				bone_aabb.resize(baabb.size());

				for (int i = 0; i < baabb.size(); i++) {
					bone_aabb.write[i] = baabb[i];
				}
			}

			add_surface(format, PrimitiveType(primitive), array_data, vertex_count, array_index_data, index_count, aabb, blend_shapes, bone_aabb);
		} else {
			ERR_FAIL_V(false);
		}

		if (d.has("material")) {
			surface_set_material(idx, d["material"]);
		}
		if (d.has("name")) {
			surface_set_name(idx, d["name"]);
		}

		return true;
	}

	return false;
}

bool ArrayMesh::_get(const StringName &p_name, Variant &r_ret) const {
	if (_is_generated()) {
		return false;
	}

	// Data must be in GPU for this routine to work.
	ERR_FAIL_COND_V(!_on_gpu, false);

	String sname = p_name;

	if (p_name == "blend_shape/names") {
		PoolVector<String> sk;
		for (int i = 0; i < blend_shapes.size(); i++) {
			sk.push_back(blend_shapes[i]);
		}
		r_ret = sk;
		return true;
	} else if (p_name == "blend_shape/mode") {
		r_ret = get_blend_shape_mode();
		return true;
	} else if (sname.begins_with("surface_")) {
		int sl = sname.find("/");
		if (sl == -1) {
			return false;
		}
		int idx = sname.substr(8, sl - 8).to_int() - 1;
		String what = sname.get_slicec('/', 1);
		if (what == "material") {
			r_ret = surface_get_material(idx);
		} else if (what == "name") {
			r_ret = surface_get_name(idx);
		}
		return true;
	} else if (!sname.begins_with("surfaces")) {
		return false;
	}

	int idx = sname.get_slicec('/', 1).to_int();
	ERR_FAIL_INDEX_V(idx, surfaces.size(), false);

	Dictionary d;

	d["array_data"] = RS::get_singleton()->mesh_surface_get_array(mesh, idx);
	d["vertex_count"] = RS::get_singleton()->mesh_surface_get_array_len(mesh, idx);
	d["array_index_data"] = RS::get_singleton()->mesh_surface_get_index_array(mesh, idx);
	d["index_count"] = RS::get_singleton()->mesh_surface_get_array_index_len(mesh, idx);
	d["primitive"] = RS::get_singleton()->mesh_surface_get_primitive_type(mesh, idx);
	d["format"] = RS::get_singleton()->mesh_surface_get_format(mesh, idx);
	d["aabb"] = RS::get_singleton()->mesh_surface_get_aabb(mesh, idx);

	Vector<AABB> skel_aabb = RS::get_singleton()->mesh_surface_get_skeleton_aabb(mesh, idx);
	Array arr;
	arr.resize(skel_aabb.size());
	for (int i = 0; i < skel_aabb.size(); i++) {
		arr[i] = skel_aabb[i];
	}
	d["skeleton_aabb"] = arr;

	Vector<PoolVector<uint8_t>> blend_shape_data = RS::get_singleton()->mesh_surface_get_blend_shapes(mesh, idx);

	Array md;
	for (int i = 0; i < blend_shape_data.size(); i++) {
		md.push_back(blend_shape_data[i]);
	}

	d["blend_shape_data"] = md;

	Ref<Material> m = surface_get_material(idx);
	if (m.is_valid()) {
		d["material"] = m;
	}
	String n = surface_get_name(idx);
	if (n != "") {
		d["name"] = n;
	}

	r_ret = d;

	return true;
}

void ArrayMesh::_get_property_list(List<PropertyInfo> *p_list) const {
	if (_is_generated()) {
		return;
	}

	if (blend_shapes.size()) {
		p_list->push_back(PropertyInfo(Variant::POOL_STRING_ARRAY, "blend_shape/names", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL));
		p_list->push_back(PropertyInfo(Variant::INT, "blend_shape/mode", PROPERTY_HINT_ENUM, "Normalized,Relative"));
	}

	for (int i = 0; i < surfaces.size(); i++) {
		p_list->push_back(PropertyInfo(Variant::DICTIONARY, "surfaces/" + itos(i), PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL));
		p_list->push_back(PropertyInfo(Variant::STRING, "surface_" + itos(i + 1) + "/name", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR));
		if (surfaces[i].is_2d) {
			p_list->push_back(PropertyInfo(Variant::OBJECT, "surface_" + itos(i + 1) + "/material", PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial,CanvasItemMaterial", PROPERTY_USAGE_EDITOR));
		} else {
			p_list->push_back(PropertyInfo(Variant::OBJECT, "surface_" + itos(i + 1) + "/material", PROPERTY_HINT_RESOURCE_TYPE, "ShaderMaterial,SpatialMaterial", PROPERTY_USAGE_EDITOR));
		}
	}
}

void ArrayMesh::_recompute_aabb() {
	// regenerate AABB
	aabb = AABB();

	for (int i = 0; i < surfaces.size(); i++) {
		if (i == 0) {
			aabb = surfaces[i].aabb;
		} else {
			aabb.merge_with(surfaces[i].aabb);
		}
	}
}

void ArrayMesh::add_surface(uint32_t p_format, PrimitiveType p_primitive, const PoolVector<uint8_t> &p_array, int p_vertex_count, const PoolVector<uint8_t> &p_index_array, int p_index_count, const AABB &p_aabb, const Vector<PoolVector<uint8_t>> &p_blend_shapes, const Vector<AABB> &p_bone_aabbs) {
	Surface s;
	s.aabb = p_aabb;
	s.is_2d = p_format & ARRAY_FLAG_USE_2D_VERTICES;
	s.creation_format = p_format;
	surfaces.push_back(s);
	_recompute_aabb();

	RenderingServer::get_singleton()->mesh_add_surface(mesh, p_format, (RS::PrimitiveType)p_primitive, p_array, p_vertex_count, p_index_array, p_index_count, p_aabb, p_blend_shapes, p_bone_aabbs);
}

void ArrayMesh::clear_cpu_surfaces() {
	for (unsigned int n = 0; n < _cpu_surfaces.size(); n++) {
		CPUSurface *s = _cpu_surfaces[n];
		DEV_ASSERT(s);
		memdelete(s);
	}

	_cpu_surfaces.clear();
}

void ArrayMesh::add_surface_from_arrays_cpu_with_probe(PrimitiveType p_primitive, const Array &p_arrays, const Array &p_blend_shapes, uint32_t p_flags, int p_surface_id) {
	uint32_t creation_format = 0;

	if (_on_gpu) {
		// query the last created surface format
		creation_format = RenderingServer::get_singleton()->mesh_surface_get_format(mesh, surfaces.size());
	} else {
		creation_format = RenderingServer::get_singleton()->mesh_find_format_from_arrays((RS::PrimitiveType)p_primitive, p_arrays, p_blend_shapes, p_flags);
	}

	Surface s = surfaces[p_surface_id];
	s.creation_flags = p_flags;
	s.creation_format = creation_format;
	surfaces.set(p_surface_id, s);

	add_surface_from_arrays_cpu(p_primitive, p_arrays, p_blend_shapes);
}

void ArrayMesh::add_surface_from_arrays_cpu(PrimitiveType p_primitive, const Array &p_arrays, const Array &p_blend_shapes) {
	CPUSurface *s = memnew(CPUSurface);
	_cpu_surfaces.push_back(s);

	s->primitive_type = p_primitive;
	s->arrays = p_arrays;
	s->blend_shapes = p_blend_shapes;

	if (p_arrays.size() > RS::ARRAY_VERTEX) {
		// This is horrible but RenderingServer uses this .. it may do a conversion to PoolVector3Array?
		// Maybe this rarely happens.
		s->num_verts = PoolVector3Array(p_arrays[RS::ARRAY_VERTEX]).size();
	}
	if (p_arrays.size() > RS::ARRAY_INDEX) {
		s->num_inds = PoolIntArray(p_arrays[RS::ARRAY_INDEX]).size();
	}
}

void ArrayMesh::add_surface_from_arrays(PrimitiveType p_primitive, const Array &p_arrays, const Array &p_blend_shapes, uint32_t p_flags) {
	ERR_FAIL_COND(p_arrays.size() != ARRAY_MAX);

	Surface s;

	if (_on_gpu) {
		RenderingServer::get_singleton()->mesh_add_surface_from_arrays(mesh, (RenderingServer::PrimitiveType)p_primitive, p_arrays, p_blend_shapes, p_flags);
	}

	/* make aABB? */ {
		Variant arr = p_arrays[ARRAY_VERTEX];
		PoolVector<Vector3> vertices = arr;
		int len = vertices.size();
		ERR_FAIL_COND(len == 0);
		PoolVector<Vector3>::Read r = vertices.read();
		const Vector3 *vtx = r.ptr();

		// check AABB
		AABB aabb;
		for (int i = 0; i < len; i++) {
			if (i == 0) {
				aabb.position = vtx[i];
			} else {
				aabb.expand_to(vtx[i]);
			}
		}

		s.aabb = aabb;
		s.is_2d = arr.get_type() == Variant::POOL_VECTOR2_ARRAY;
		s.creation_flags = p_flags;
		surfaces.push_back(s);

		_recompute_aabb();
	}

	if (_on_cpu) {
		add_surface_from_arrays_cpu_with_probe(p_primitive, p_arrays, p_blend_shapes, p_flags, surfaces.size() - 1);
	}

	clear_cache();
	_change_notify();
	emit_changed();
}

Array ArrayMesh::surface_get_arrays(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, surfaces.size(), Array());

	// preferentially read from CPU as quicker
	if (on_cpu()) {
		return _cpu_surfaces[p_surface]->arrays;
	}

	return RenderingServer::get_singleton()->mesh_surface_get_arrays(mesh, p_surface);
}
Array ArrayMesh::surface_get_blend_shape_arrays(int p_surface) const {
	ERR_FAIL_INDEX_V(p_surface, surfaces.size(), Array());

	// preferentially read from CPU as quicker
	if (on_cpu()) {
		return _cpu_surfaces[p_surface]->blend_shapes;
	}

	return RenderingServer::get_singleton()->mesh_surface_get_blend_shape_arrays(mesh, p_surface);
}

int ArrayMesh::get_surface_count() const {
	return surfaces.size();
}

void ArrayMesh::add_blend_shape(const StringName &p_name) {
	ERR_FAIL_COND_MSG(surfaces.size(), "Can't add a shape key count if surfaces are already created.");

	StringName name = p_name;

	if (blend_shapes.find(name) != -1) {
		int count = 2;
		do {
			name = String(p_name) + " " + itos(count);
			count++;
		} while (blend_shapes.find(name) != -1);
	}

	blend_shapes.push_back(name);
	RS::get_singleton()->mesh_set_blend_shape_count(mesh, blend_shapes.size());
}

int ArrayMesh::get_blend_shape_count() const {
	return blend_shapes.size();
}
StringName ArrayMesh::get_blend_shape_name(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, blend_shapes.size(), StringName());
	return blend_shapes[p_index];
}

void ArrayMesh::set_blend_shape_name(int p_index, const StringName &p_name) {
	ERR_FAIL_INDEX(p_index, blend_shapes.size());

	StringName name = p_name;
	int found = blend_shapes.find(name);
	if (found != -1 && found != p_index) {
		int count = 2;
		do {
			name = String(p_name) + " " + itos(count);
			count++;
		} while (blend_shapes.find(name) != -1);
	}

	blend_shapes.write[p_index] = name;
}

void ArrayMesh::clear_blend_shapes() {
	ERR_FAIL_COND_MSG(surfaces.size(), "Can't set shape key count if surfaces are already created.");

	blend_shapes.clear();
}

void ArrayMesh::set_blend_shape_mode(BlendShapeMode p_mode) {
	blend_shape_mode = p_mode;
	RS::get_singleton()->mesh_set_blend_shape_mode(mesh, (RS::BlendShapeMode)p_mode);
}

ArrayMesh::BlendShapeMode ArrayMesh::get_blend_shape_mode() const {
	return blend_shape_mode;
}

void ArrayMesh::surface_remove(int p_idx) {
	ERR_FAIL_INDEX(p_idx, surfaces.size());
	RenderingServer::get_singleton()->mesh_remove_surface(mesh, p_idx);
	surfaces.remove(p_idx);

	if (on_cpu()) {
		CPUSurface *s = _cpu_surfaces[p_idx];
		DEV_ASSERT(s);
		memdelete(s);
		_cpu_surfaces.remove(p_idx);
	}

	clear_cache();
	_recompute_aabb();
	_change_notify();
	emit_changed();
}

int ArrayMesh::surface_get_array_len(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, surfaces.size(), -1);

	if (on_cpu()) {
		CPUSurface *s = _cpu_surfaces[p_idx];
		DEV_ASSERT(s);
		return s->num_verts;
	}

	return RenderingServer::get_singleton()->mesh_surface_get_array_len(mesh, p_idx);
}

int ArrayMesh::surface_get_array_index_len(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, surfaces.size(), -1);

	if (on_cpu()) {
		CPUSurface *s = _cpu_surfaces[p_idx];
		DEV_ASSERT(s);
		return s->num_inds;
	}

	return RenderingServer::get_singleton()->mesh_surface_get_array_index_len(mesh, p_idx);
}

uint32_t ArrayMesh::surface_get_format(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, surfaces.size(), 0);

	// not sure whether we need to support this yet?
	if (!_on_gpu) {
		return surfaces[p_idx].creation_format;
	}

	return RenderingServer::get_singleton()->mesh_surface_get_format(mesh, p_idx);
}

ArrayMesh::PrimitiveType ArrayMesh::surface_get_primitive_type(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, surfaces.size(), PRIMITIVE_LINES);

	if (on_cpu()) {
		CPUSurface *s = _cpu_surfaces[p_idx];
		DEV_ASSERT(s);
		return s->primitive_type;
	}

	return (PrimitiveType)RenderingServer::get_singleton()->mesh_surface_get_primitive_type(mesh, p_idx);
}

void ArrayMesh::surface_set_material(int p_idx, const Ref<Material> &p_material) {
	ERR_FAIL_INDEX(p_idx, surfaces.size());
	if (surfaces[p_idx].material == p_material) {
		return;
	}
	surfaces.write[p_idx].material = p_material;

	if (_on_gpu) {
		RenderingServer::get_singleton()->mesh_surface_set_material(mesh, p_idx, p_material.is_null() ? RID() : p_material->get_rid());
	}

	_change_notify("material");
	emit_changed();
}

int ArrayMesh::surface_find_by_name(const String &p_name) const {
	for (int i = 0; i < surfaces.size(); i++) {
		if (surfaces[i].name == p_name) {
			return i;
		}
	}

	return -1;
}

void ArrayMesh::surface_set_name(int p_idx, const String &p_name) {
	ERR_FAIL_INDEX(p_idx, surfaces.size());

	surfaces.write[p_idx].name = p_name;
	emit_changed();
}

String ArrayMesh::surface_get_name(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, surfaces.size(), String());
	return surfaces[p_idx].name;
}

void ArrayMesh::surface_update_region(int p_surface, int p_offset, const PoolVector<uint8_t> &p_data) {
	ERR_FAIL_INDEX(p_surface, surfaces.size());
	RS::get_singleton()->mesh_surface_update_region(mesh, p_surface, p_offset, p_data);
	emit_changed();
}

void ArrayMesh::surface_set_custom_aabb(int p_idx, const AABB &p_aabb) {
	ERR_FAIL_INDEX(p_idx, surfaces.size());
	surfaces.write[p_idx].aabb = p_aabb;
	// set custom aabb too?
	emit_changed();
}

Ref<Material> ArrayMesh::surface_get_material(int p_idx) const {
	ERR_FAIL_INDEX_V(p_idx, surfaces.size(), Ref<Material>());
	return surfaces[p_idx].material;
}

void ArrayMesh::add_surface_from_mesh_data(const Geometry::MeshData &p_mesh_data) {
	RenderingServer::get_singleton()->mesh_add_surface_from_mesh_data(mesh, p_mesh_data);
	AABB aabb;
	for (int i = 0; i < p_mesh_data.vertices.size(); i++) {
		if (i == 0) {
			aabb.position = p_mesh_data.vertices[i];
		} else {
			aabb.expand_to(p_mesh_data.vertices[i]);
		}
	}

	Surface s;
	s.aabb = aabb;
	if (surfaces.size() == 0) {
		aabb = s.aabb;
	} else {
		aabb.merge_with(s.aabb);
	}

	clear_cache();

	surfaces.push_back(s);
	_change_notify();

	emit_changed();
}

RID ArrayMesh::get_rid() const {
	return mesh;
}
AABB ArrayMesh::get_aabb() const {
	return aabb;
}

void ArrayMesh::clear_surfaces() {
	if (!mesh.is_valid()) {
		return;
	}

	if (_on_cpu) {
		clear_cpu_surfaces();
	}

	RS::get_singleton()->mesh_clear(mesh);
	surfaces.clear();
	aabb = AABB();
}

void ArrayMesh::set_custom_aabb(const AABB &p_custom) {
	custom_aabb = p_custom;
	RS::get_singleton()->mesh_set_custom_aabb(mesh, custom_aabb);
	emit_changed();
}

AABB ArrayMesh::get_custom_aabb() const {
	return custom_aabb;
}

void ArrayMesh::regen_normalmaps() {
	Vector<Ref<SurfaceTool>> surfs;
	for (int i = 0; i < get_surface_count(); i++) {
		Ref<SurfaceTool> st = memnew(SurfaceTool);
		st->create_from(Ref<ArrayMesh>(this), i);
		surfs.push_back(st);
	}

	while (get_surface_count()) {
		surface_remove(0);
	}

	for (int i = 0; i < surfs.size(); i++) {
		surfs.write[i]->generate_tangents();
		surfs.write[i]->commit(Ref<ArrayMesh>(this));
	}
}

void ArrayMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_blend_shape", "name"), &ArrayMesh::add_blend_shape);
	ClassDB::bind_method(D_METHOD("get_blend_shape_count"), &ArrayMesh::get_blend_shape_count);
	ClassDB::bind_method(D_METHOD("get_blend_shape_name", "index"), &ArrayMesh::get_blend_shape_name);
	ClassDB::bind_method(D_METHOD("set_blend_shape_name", "index", "name"), &ArrayMesh::set_blend_shape_name);
	ClassDB::bind_method(D_METHOD("clear_blend_shapes"), &ArrayMesh::clear_blend_shapes);
	ClassDB::bind_method(D_METHOD("set_blend_shape_mode", "mode"), &ArrayMesh::set_blend_shape_mode);
	ClassDB::bind_method(D_METHOD("get_blend_shape_mode"), &ArrayMesh::get_blend_shape_mode);

	ClassDB::bind_method(D_METHOD("add_surface_from_arrays", "primitive", "arrays", "blend_shapes", "compress_flags"), &ArrayMesh::add_surface_from_arrays, DEFVAL(Array()), DEFVAL(ARRAY_COMPRESS_DEFAULT));
	ClassDB::bind_method(D_METHOD("clear_surfaces"), &ArrayMesh::clear_surfaces);
	ClassDB::bind_method(D_METHOD("surface_remove", "surf_idx"), &ArrayMesh::surface_remove);
	ClassDB::bind_method(D_METHOD("surface_update_region", "surf_idx", "offset", "data"), &ArrayMesh::surface_update_region);
	ClassDB::bind_method(D_METHOD("surface_get_array_len", "surf_idx"), &ArrayMesh::surface_get_array_len);
	ClassDB::bind_method(D_METHOD("surface_get_array_index_len", "surf_idx"), &ArrayMesh::surface_get_array_index_len);
	ClassDB::bind_method(D_METHOD("surface_get_format", "surf_idx"), &ArrayMesh::surface_get_format);
	ClassDB::bind_method(D_METHOD("surface_get_primitive_type", "surf_idx"), &ArrayMesh::surface_get_primitive_type);
	ClassDB::bind_method(D_METHOD("surface_find_by_name", "name"), &ArrayMesh::surface_find_by_name);
	ClassDB::bind_method(D_METHOD("surface_set_name", "surf_idx", "name"), &ArrayMesh::surface_set_name);
	ClassDB::bind_method(D_METHOD("surface_get_name", "surf_idx"), &ArrayMesh::surface_get_name);
	ClassDB::bind_method(D_METHOD("create_trimesh_shape"), &ArrayMesh::create_trimesh_shape);
	ClassDB::bind_method(D_METHOD("create_convex_shape", "clean", "simplify"), &ArrayMesh::create_convex_shape, DEFVAL(true), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("create_outline", "margin"), &ArrayMesh::create_outline);
	ClassDB::bind_method(D_METHOD("regen_normalmaps"), &ArrayMesh::regen_normalmaps);
	ClassDB::set_method_flags(get_class_static(), _scs_create("regen_normalmaps"), METHOD_FLAGS_DEFAULT | METHOD_FLAG_EDITOR);
	ClassDB::bind_method(D_METHOD("get_faces"), &ArrayMesh::get_faces);
	ClassDB::bind_method(D_METHOD("generate_triangle_mesh"), &ArrayMesh::generate_triangle_mesh);

	ClassDB::bind_method(D_METHOD("set_custom_aabb", "aabb"), &ArrayMesh::set_custom_aabb);
	ClassDB::bind_method(D_METHOD("get_custom_aabb"), &ArrayMesh::get_custom_aabb);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "blend_shape_mode", PROPERTY_HINT_ENUM, "Normalized,Relative", PROPERTY_USAGE_NOEDITOR), "set_blend_shape_mode", "get_blend_shape_mode");
	ADD_PROPERTY(PropertyInfo(Variant::AABB, "custom_aabb", PROPERTY_HINT_NONE, ""), "set_custom_aabb", "get_custom_aabb");

	BIND_CONSTANT(NO_INDEX_ARRAY);
	BIND_CONSTANT(ARRAY_WEIGHTS_SIZE);

	BIND_ENUM_CONSTANT(ARRAY_VERTEX);
	BIND_ENUM_CONSTANT(ARRAY_NORMAL);
	BIND_ENUM_CONSTANT(ARRAY_TANGENT);
	BIND_ENUM_CONSTANT(ARRAY_COLOR);
	BIND_ENUM_CONSTANT(ARRAY_TEX_UV);
	BIND_ENUM_CONSTANT(ARRAY_TEX_UV2);
	BIND_ENUM_CONSTANT(ARRAY_BONES);
	BIND_ENUM_CONSTANT(ARRAY_WEIGHTS);
	BIND_ENUM_CONSTANT(ARRAY_INDEX);
	BIND_ENUM_CONSTANT(ARRAY_MAX);

	BIND_ENUM_CONSTANT(ARRAY_FORMAT_VERTEX);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_NORMAL);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_TANGENT);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_COLOR);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_TEX_UV);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_TEX_UV2);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_BONES);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_WEIGHTS);
	BIND_ENUM_CONSTANT(ARRAY_FORMAT_INDEX);
}

void ArrayMesh::reload_from_file() {
	RenderingServer::get_singleton()->mesh_clear(mesh);
	surfaces.clear();
	clear_blend_shapes();
	clear_cache();

	Resource::reload_from_file();

	_change_notify();
}

void ArrayMesh::set_storage_mode(StorageMode p_storage_mode) {
	if (_storage_mode == p_storage_mode) {
		return;
	}

	bool new_on_cpu = false;
	bool new_on_gpu = false;

	switch (p_storage_mode) {
		default: {
			new_on_cpu = false;
			new_on_gpu = true;
		} break;
		case STORAGE_MODE_CPU: {
			new_on_cpu = true;
			new_on_gpu = false;
		} break;
		case STORAGE_MODE_CPU_AND_GPU: {
			new_on_cpu = true;
			new_on_gpu = true;
		} break;
	}

	// cpu to gpu?
	if (new_on_gpu && !_on_gpu) {
		// must be on cpu to go to gpu
		DEV_CHECK(_on_cpu);
		if (mesh.is_valid()) {
			// make sure mesh is clear (may not be necessary)
			RS::get_singleton()->mesh_clear(mesh);

			for (unsigned int n = 0; n < _cpu_surfaces.size(); n++) {
				CPUSurface *s = _cpu_surfaces[n];
				DEV_ASSERT(s);
				RenderingServer::get_singleton()->mesh_add_surface_from_arrays(mesh, (RenderingServer::PrimitiveType)s->primitive_type, s->arrays, s->blend_shapes, surfaces[n].creation_flags);

				ERR_CONTINUE((int)n >= surfaces.size());
				const Ref<Material> &mat = surfaces[n].material;
				RenderingServer::get_singleton()->mesh_surface_set_material(mesh, n, mat.is_null() ? RID() : mat->get_rid());
			}
		}
	}

	// gpu to cpu?
	if (new_on_cpu && !_on_cpu) {
		// must be on gpu to go to cpu
		DEV_CHECK(_on_gpu);
		clear_cpu_surfaces();

		if (mesh.is_valid()) {
			for (int n = 0; n < surfaces.size(); n++) {
				Array arrays = RenderingServer::get_singleton()->mesh_surface_get_arrays(mesh, n);
				Array blend_shapes = RenderingServer::get_singleton()->mesh_surface_get_blend_shape_arrays(mesh, n);
				PrimitiveType primitive = (PrimitiveType)RenderingServer::get_singleton()->mesh_surface_get_primitive_type(mesh, n);
				add_surface_from_arrays_cpu(primitive, arrays, blend_shapes);
			}
		} // mesh valid
	}

	// clear anything not used
	if (!new_on_cpu) {
		clear_cpu_surfaces();
	}
	if (!new_on_gpu && _on_gpu) {
		if (mesh.is_valid()) {
			RS::get_singleton()->mesh_clear(mesh);
		}
	}

	_on_cpu = new_on_cpu;
	_on_gpu = new_on_gpu;
	_storage_mode = p_storage_mode;
}

ArrayMesh::ArrayMesh() {
	mesh = RID_PRIME(RenderingServer::get_singleton()->mesh_create());
	blend_shape_mode = BLEND_SHAPE_MODE_RELATIVE;
}

ArrayMesh::~ArrayMesh() {
	RenderingServer::get_singleton()->free(mesh);
	clear_cpu_surfaces();
}
