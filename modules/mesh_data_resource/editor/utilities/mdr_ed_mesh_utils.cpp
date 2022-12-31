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

#include "mdr_ed_mesh_utils.h"

#include "../../mesh_data_resource.h"
#include "modules/mesh_utils/mesh_utils.h"
#include "scene/resources/mesh.h"
#include "scene/resources/surface_tool.h"

#include "modules/modules_enabled.gen.h"

int MDREDMeshUtils::SeamTriangleHelper::get_other_side_index(int index) const {
	if (side_index_1 == index) {
		return side_index_2;
	} else {
		return side_index_1;
	}
}

int MDREDMeshUtils::SeamTriangleHelper::get_side_index(int i) const {
	if (i == 1) {
		return side_index_1;
	} else {
		return side_index_2;
	}
}
int MDREDMeshUtils::SeamTriangleHelper::get_side_index_cut() const {
	if (side_index_1_cut && side_index_2_cut) {
		return 3;
	} else if (side_index_1_cut) {
		return 1;
	} else if (side_index_2_cut) {
		return 2;
	} else {
		return 0;
	}
}
int MDREDMeshUtils::SeamTriangleHelper::get_opposite_side_index_cut() const {
	if (side_index_1_cut && side_index_2_cut) {
		return 3;
	} else if (side_index_1_cut) {
		return 2;
	} else if (side_index_2_cut) {
		return 1;
	} else {
		return 0;
	}
}
bool MDREDMeshUtils::SeamTriangleHelper::is_side_index_cut(int i) const {
	if (i == 1) {
		return side_index_1_cut;
	} else {
		return side_index_2_cut;
	}
}

bool MDREDMeshUtils::SeamTriangleHelper::is_the_same(const SeamTriangleHelper &h) const {
	return is_triangle(h.i0, h.i1, h.i2);
}

bool MDREDMeshUtils::SeamTriangleHelper::is_triangle(int pi0, int pi1, int pi2) const {
	if (pi0 == i0 || pi0 == i1 || pi0 == i2) {
		if (pi1 == i0 || pi1 == i1 || pi1 == i2) {
			if (pi2 == i0 || pi2 == i1 || pi2 == i2) {
				return true;
			}
		}
	}

	return false;
}
bool MDREDMeshUtils::SeamTriangleHelper::is_neighbour_to(int index) const {
	return (side_index_1 == index) || (side_index_2 == index);
}

bool MDREDMeshUtils::SeamTriangleHelper::needs_to_be_cut_near(int index) const {
	if (side_index_1 == index) {
		return side_index_1_cut;
	}

	if (side_index_2 == index) {
		return side_index_2_cut;
	}

	return false;
}
bool MDREDMeshUtils::SeamTriangleHelper::has_cut() const {
	return side_index_1_cut || side_index_2_cut;
}
bool MDREDMeshUtils::SeamTriangleHelper::both_sides_need_cut() const {
	return side_index_1_cut && side_index_2_cut;
}

void MDREDMeshUtils::SeamTriangleHelper::setup(int pi0, int pi1, int pi2, int porig_ind, int pindex_index, const PoolIntArray &seams) {
	processed = false;
	i0 = pi0;
	i1 = pi1;
	i2 = pi2;
	orig_index = porig_ind;
	index_index = pindex_index;

	if (porig_ind == pi0) {
		side_index_1 = pi1;
		side_index_2 = pi2;
	} else if (porig_ind == pi1) {
		side_index_1 = pi0;
		side_index_2 = pi2;
	} else if (porig_ind == pi2) {
		side_index_1 = pi1;
		side_index_2 = pi0;
	}

	determine_cuts(seams);
}

void MDREDMeshUtils::SeamTriangleHelper::determine_cuts(const PoolIntArray &seams) {
	if (orig_index < side_index_1) {
		side_index_1_cut = check_cut(orig_index, side_index_1, seams);
	} else {
		side_index_1_cut = check_cut(side_index_1, orig_index, seams);
	}

	if (orig_index < side_index_2) {
		side_index_2_cut = check_cut(orig_index, side_index_2, seams);
	} else {
		side_index_2_cut = check_cut(side_index_2, orig_index, seams);
	}
}
bool MDREDMeshUtils::SeamTriangleHelper::check_cut(int ind0, int ind1, const PoolIntArray &seams) {
	for (int stind = 0; stind < seams.size(); stind += 2) {
		int si0 = seams[stind];
		int si1 = seams[stind + 1];

		if ((si0 == ind0) && (si1 == ind1)) {
			return true;
		}
	}

	return false;
}

String MDREDMeshUtils::SeamTriangleHelper::_to_string() {
	return "[ TRI: " + itos(i0) + ", " + itos(i1) + ", " + itos(i2) + " ]";
}

MDREDMeshUtils::SeamTriangleHelper::SeamTriangleHelper() {
	i0 = 0;
	i1 = 0;
	i2 = 0;
	orig_index = 0;
	index_index = 0;

	side_index_1 = 0;
	side_index_2 = 0;
	side_index_1_cut = false;
	side_index_2_cut = false;

	processed = false;
}

void MDREDMeshUtils::remove_triangle(Ref<MeshDataResource> mdr, const int triangle_index) {
	if (triangle_index < 0) {
		return;
	}

	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		arrays.resize(ArrayMesh::ARRAY_MAX);
	}

	if (arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return;
	}

	PoolIntArray indices = arrays[ArrayMesh::ARRAY_INDEX];

	// Just remove that triangle
	int i = triangle_index * 3;

	int i0 = indices[i];
	int i1 = indices[i + 1];
	int i2 = indices[i + 2];

	remove_seam_not_ordered(mdr, i0, i1);
	remove_seam_not_ordered(mdr, i1, i2);
	remove_seam_not_ordered(mdr, i2, i0);

	indices.remove(i);
	indices.remove(i);
	indices.remove(i);

	arrays[ArrayMesh::ARRAY_INDEX] = indices;

	mdr->set_array(arrays);
}

bool MDREDMeshUtils::add_triangulated_mesh_from_points_delaunay(Ref<MeshDataResource> mdr, const PoolVector3Array &selected_points, const Vector3 &last_known_camera_facing) {
	if (selected_points.size() < 3) {
		return false;
	}

	Vector<Vector3> spv;
	spv.resize(selected_points.size());

	PoolVector3Array::Read r = selected_points.read();

	for (int i = 0; i < selected_points.size(); ++i) {
		spv.write[i] = r[i];
	}

	r.release();

	PoolIntArray tetrahedrons = MeshUtils::get_singleton()->delaunay3d_tetrahedralize(spv);

	if (tetrahedrons.size() == 0) {
		// try randomly displacing the points a bit, it can help
		Vector<Vector3> rnd_selected_points;
		rnd_selected_points.append_array(spv);

		for (int i = 0; i < rnd_selected_points.size(); ++i) {
			rnd_selected_points.write[i] = rnd_selected_points[i] + (Vector3(Math::randf(), Math::randf(), Math::randf()) * 0.1);
		}

		tetrahedrons = MeshUtils::get_singleton()->delaunay3d_tetrahedralize(rnd_selected_points);

		if (tetrahedrons.size() == 0) {
			print_error("add_triangulated_mesh_from_points_delaunay: tetrahedrons.size() == 0!");
			return false;
		}
	}

	Ref<SurfaceTool> st;
	st.instance();

	st->begin(Mesh::PRIMITIVE_TRIANGLES);

	for (int i = 0; i < tetrahedrons.size(); i += 4) {
		int i0 = tetrahedrons[i];
		int i1 = tetrahedrons[i + 1];
		int i2 = tetrahedrons[i + 2];
		int i3 = tetrahedrons[i + 3];

		Vector3 v0 = selected_points[i0];
		Vector3 v1 = selected_points[i1];
		Vector3 v2 = selected_points[i2];
		Vector3 v3 = selected_points[i3];

		bool flip = is_normal_similar(v0, v1, v2, last_known_camera_facing);

		Vector3 normal = get_face_normal(v0, v1, v2, flip);

		st->add_uv(Vector2(0, 1));
		st->add_normal(normal);
		st->add_vertex(v0);
		st->add_uv(Vector2(0.5, 0));
		st->add_normal(normal);
		st->add_vertex(v1);
		st->add_uv(Vector2(1, 1));
		st->add_normal(normal);
		st->add_vertex(v2);
		st->add_uv(Vector2(1, 1));
		st->add_normal(normal);
		st->add_vertex(v3);

		int im3 = i;

		if (!flip) {
			st->add_index(im3 + 0);
			st->add_index(im3 + 1);
			st->add_index(im3 + 2);

			st->add_index(im3 + 0);
			st->add_index(im3 + 2);
			st->add_index(im3 + 3);
		} else {
			st->add_index(im3 + 3);
			st->add_index(im3 + 2);
			st->add_index(im3 + 0);

			st->add_index(im3 + 2);
			st->add_index(im3 + 1);
			st->add_index(im3 + 0);
		}
	}

	merge_in_surface_tool(mdr, st);

	return true;
}

void MDREDMeshUtils::add_triangulated_mesh_from_points(Ref<MeshDataResource> mdr, const PoolVector3Array &selected_points, const Vector3 &last_known_camera_facing) {
	if (selected_points.size() < 3) {
		return;
	}

	Ref<SurfaceTool> st;
	st.instance();

	st->begin(Mesh::PRIMITIVE_TRIANGLES);

	Vector3 v0 = selected_points[0];
	Vector3 v1 = selected_points[1];

	for (int i = 2; i < selected_points.size(); ++i) {
		Vector3 v2 = selected_points[i];

		bool flip = is_normal_similar(v0, v1, v2, last_known_camera_facing);

		Vector3 normal = get_face_normal(v0, v1, v2, flip);

		st->add_uv(Vector2(0, 1));
		st->add_normal(normal);
		st->add_vertex(v0);
		st->add_uv(Vector2(0.5, 0));
		st->add_normal(normal);
		st->add_vertex(v1);
		st->add_uv(Vector2(1, 1));
		st->add_normal(normal);
		st->add_vertex(v2);

		int im3 = (i - 2) * 3;

		if (!flip) {
			st->add_index(im3);
			st->add_index(im3 + 1);
			st->add_index(im3 + 2);
		} else {
			st->add_index(im3 + 2);
			st->add_index(im3 + 1);
			st->add_index(im3);
		}
	}

	merge_in_surface_tool(mdr, st);
}

// Appends a triangle to the mesh. It's created from mirroring v2 to the ev0, and ev1 edge
void MDREDMeshUtils::append_triangle_to_tri_edge(Ref<MeshDataResource> mdr, const Vector3 &ev0, const Vector3 &ev1, const Vector3 &v2) {
	Vector3 vref = reflect_vertex(ev0, ev1, v2);

	add_triangle_at(mdr, ev1, ev0, vref, false);
}

void MDREDMeshUtils::add_triangle_at(Ref<MeshDataResource> mdr, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, const bool flip) {
	Ref<SurfaceTool> st;
	st.instance();

	st->begin(Mesh::PRIMITIVE_TRIANGLES);

	Vector3 normal = get_face_normal(v0, v1, v2, flip);

	st->add_uv(Vector2(0, 1));
	st->add_normal(normal);
	st->add_vertex(v0);
	st->add_uv(Vector2(0.5, 0));
	st->add_normal(normal);
	st->add_vertex(v1);
	st->add_uv(Vector2(1, 1));
	st->add_normal(normal);
	st->add_vertex(v2);

	if (!flip) {
		st->add_index(0);
		st->add_index(1);
		st->add_index(2);
	} else {
		st->add_index(2);
		st->add_index(1);
		st->add_index(0);
	}

	merge_in_surface_tool(mdr, st);
}

void MDREDMeshUtils::add_triangle(Ref<MeshDataResource> mdr) {
	Ref<SurfaceTool> st;
	st.instance();

	st->begin(Mesh::PRIMITIVE_TRIANGLES);

	Vector3 normal = get_face_normal(Vector3(-0.5, -0.5, 0), Vector3(0, 0.5, 0), Vector3(0.5, -0.5, 0));

	st->add_uv(Vector2(0, 1));
	st->add_normal(normal);
	st->add_vertex(Vector3(-0.5, -0.5, 0));
	st->add_uv(Vector2(0.5, 0));
	st->add_normal(normal);
	st->add_vertex(Vector3(0, 0.5, 0));
	st->add_uv(Vector2(1, 1));
	st->add_normal(normal);
	st->add_vertex(Vector3(0.5, -0.5, 0));

	st->add_index(0);
	st->add_index(1);
	st->add_index(2);

	merge_in_surface_tool(mdr, st);
}

// Appends a quad to the mesh. It's created to the opposite side of v2 to the ev0, and ev1 edge
void MDREDMeshUtils::append_quad_to_tri_edge(Ref<MeshDataResource> mdr, const Vector3 &ev0, const Vector3 &ev1, const Vector3 &v2) {
	Vector3 vref = reflect_vertex(ev0, ev1, v2);
	Vector3 vproj = (vref - ev0).project(ev1 - ev0);
	Vector3 eoffs = (vref - ev0) - vproj;

	Vector3 qv0 = ev0;
	Vector3 qv1 = ev0 + eoffs;
	Vector3 qv2 = ev1 + eoffs;
	Vector3 qv3 = ev1;

	add_quad_at(mdr, qv0, qv1, qv2, qv3, false);
}

void MDREDMeshUtils::add_quad_at(Ref<MeshDataResource> mdr, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const bool flip) {
	Ref<SurfaceTool> st;
	st.instance();

	st->begin(Mesh::PRIMITIVE_TRIANGLES);

	Vector3 normal = get_face_normal(v0, v1, v2, flip);

	st->add_uv(Vector2(0, 1));
	st->add_normal(normal);
	st->add_vertex(v0);
	st->add_uv(Vector2(0, 0));
	st->add_normal(normal);
	st->add_vertex(v1);
	st->add_uv(Vector2(1, 0));
	st->add_normal(normal);
	st->add_vertex(v2);
	st->add_uv(Vector2(1, 1));
	st->add_normal(normal);
	st->add_vertex(v3);

	if (!flip) {
		st->add_index(0);
		st->add_index(1);
		st->add_index(2);

		st->add_index(0);
		st->add_index(2);
		st->add_index(3);
	} else {
		st->add_index(2);
		st->add_index(1);
		st->add_index(0);

		st->add_index(3);
		st->add_index(2);
		st->add_index(0);
	}

	merge_in_surface_tool(mdr, st);
}

void MDREDMeshUtils::add_quad(Ref<MeshDataResource> mdr) {
	Ref<SurfaceTool> st;
	st.instance();

	Vector3 normal = get_face_normal(Vector3(-0.5, -0.5, 0), Vector3(-0.5, 0.5, 0), Vector3(0.5, 0.5, 0));

	st->begin(Mesh::PRIMITIVE_TRIANGLES);

	st->add_uv(Vector2(0, 1));
	st->add_normal(normal);
	st->add_vertex(Vector3(-0.5, -0.5, 0));
	st->add_uv(Vector2(0, 0));
	st->add_normal(normal);
	st->add_vertex(Vector3(-0.5, 0.5, 0));
	st->add_uv(Vector2(1, 0));
	st->add_normal(normal);
	st->add_vertex(Vector3(0.5, 0.5, 0));
	st->add_uv(Vector2(1, 1));
	st->add_normal(normal);
	st->add_vertex(Vector3(0.5, -0.5, 0));

	st->add_index(0);
	st->add_index(1);
	st->add_index(2);
	st->add_index(2);
	st->add_index(3);
	st->add_index(0);

	merge_in_surface_tool(mdr, st);
}

void MDREDMeshUtils::add_box(Ref<MeshDataResource> mdr) {
	Ref<SurfaceTool> st;
	st.instance();

	Vector3 normal = Vector3();

	st->begin(Mesh::PRIMITIVE_TRIANGLES);

	float sgn = 1;

	//z
	for (int i = 0; i < 2; ++i) {
		normal = get_face_normal(Vector3(-0.5, -0.5, sgn * 0.5), Vector3(-0.5, 0.5, sgn * 0.5), Vector3(0.5, 0.5, sgn * 0.5), sgn < 0);

		st->add_uv(Vector2(0, 1));
		st->add_normal(normal);
		st->add_vertex(Vector3(-0.5, -0.5, sgn * 0.5));
		st->add_uv(Vector2(0, 0));
		st->add_normal(normal);
		st->add_vertex(Vector3(-0.5, 0.5, sgn * 0.5));
		st->add_uv(Vector2(1, 0));
		st->add_normal(normal);
		st->add_vertex(Vector3(0.5, 0.5, sgn * 0.5));
		st->add_uv(Vector2(1, 1));
		st->add_normal(normal);
		st->add_vertex(Vector3(0.5, -0.5, sgn * 0.5));

		sgn *= -1;
	}

	//x
	for (int i = 0; i < 2; ++i) {
		normal = get_face_normal(Vector3(sgn * 0.5, -0.5, 0.5), Vector3(sgn * 0.5, 0.5, 0.5), Vector3(sgn * 0.5, 0.5, -0.5), sgn < 0);

		st->add_uv(Vector2(0, 1));
		st->add_normal(normal);
		st->add_vertex(Vector3(sgn * 0.5, -0.5, 0.5));
		st->add_uv(Vector2(0, 0));
		st->add_normal(normal);
		st->add_vertex(Vector3(sgn * 0.5, 0.5, 0.5));
		st->add_uv(Vector2(1, 0));
		st->add_normal(normal);
		st->add_vertex(Vector3(sgn * 0.5, 0.5, -0.5));
		st->add_uv(Vector2(1, 1));
		st->add_normal(normal);
		st->add_vertex(Vector3(sgn * 0.5, -0.5, -0.5));

		sgn *= -1;
	}

	//y
	for (int i = 0; i < 2; ++i) {
		normal = get_face_normal(Vector3(-0.5, sgn * 0.5, 0.5), Vector3(-0.5, sgn * 0.5, -0.5), Vector3(0.5, sgn * 0.5, -0.5), sgn < 0);

		st->add_uv(Vector2(0, 1));
		st->add_normal(normal);
		st->add_vertex(Vector3(-0.5, sgn * 0.5, 0.5));
		st->add_uv(Vector2(0, 0));
		st->add_normal(normal);
		st->add_vertex(Vector3(-0.5, sgn * 0.5, -0.5));
		st->add_uv(Vector2(1, 0));
		st->add_normal(normal);
		st->add_vertex(Vector3(0.5, sgn * 0.5, -0.5));
		st->add_uv(Vector2(1, 1));
		st->add_normal(normal);
		st->add_vertex(Vector3(0.5, sgn * 0.5, 0.5));

		sgn *= -1;
	}

	int ind = 0;

	for (int i = 0; i < 3; ++i) {
		st->add_index(ind + 0);
		st->add_index(ind + 1);
		st->add_index(ind + 2);
		st->add_index(ind + 2);
		st->add_index(ind + 3);
		st->add_index(ind + 0);

		ind += 4;

		st->add_index(ind + 0);
		st->add_index(ind + 2);
		st->add_index(ind + 1);
		st->add_index(ind + 0);
		st->add_index(ind + 3);
		st->add_index(ind + 2);

		ind += 4;
	}

	merge_in_surface_tool(mdr, st);
}

void MDREDMeshUtils::merge_in_surface_tool(Ref<MeshDataResource> mdr, Ref<SurfaceTool> st, const bool generate_normals_if_needed, const bool generate_tangents_if_needed) {
	Array arrays = get_arrays_prepared(mdr);

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		arrays.resize(ArrayMesh::ARRAY_MAX);
	}

	if (generate_normals_if_needed && arrays[ArrayMesh::ARRAY_NORMAL].is_null()) {
		//st->generate_normals();
		generate_normals_mdr(mdr);
	}

	if (generate_tangents_if_needed && arrays[ArrayMesh::ARRAY_TANGENT].is_null()) {
		st->generate_tangents();
	}

	merge_in_arrays(mdr, st->commit_to_arrays());
}
void MDREDMeshUtils::merge_in_arrays(Ref<MeshDataResource> mdr, const Array &merge) {
	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		arrays.resize(ArrayMesh::ARRAY_MAX);
	}

	PoolVector3Array vertices;
	PoolVector3Array normals;
	PoolRealArray tangents;
	PoolColorArray colors;
	PoolVector2Array uv;
	PoolVector2Array uv2;
	PoolRealArray bones;
	PoolRealArray weights;
	PoolIntArray indices;

	if (!arrays[ArrayMesh::ARRAY_VERTEX].is_null()) {
		vertices = arrays[ArrayMesh::ARRAY_VERTEX];
	}

	if (!arrays[ArrayMesh::ARRAY_NORMAL].is_null()) {
		normals = arrays[ArrayMesh::ARRAY_NORMAL];
	}

	if (!arrays[ArrayMesh::ARRAY_TANGENT].is_null()) {
		tangents = arrays[ArrayMesh::ARRAY_TANGENT];
	}

	if (!arrays[ArrayMesh::ARRAY_COLOR].is_null()) {
		colors = arrays[ArrayMesh::ARRAY_COLOR];
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		uv = arrays[ArrayMesh::ARRAY_TEX_UV];
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV2].is_null()) {
		uv2 = arrays[ArrayMesh::ARRAY_TEX_UV2];
	}

	if (!arrays[ArrayMesh::ARRAY_BONES].is_null()) {
		bones = arrays[ArrayMesh::ARRAY_BONES];
	}

	if (!arrays[ArrayMesh::ARRAY_WEIGHTS].is_null()) {
		weights = arrays[ArrayMesh::ARRAY_WEIGHTS];
	}

	if (!arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		indices = arrays[ArrayMesh::ARRAY_INDEX];
	}

	PoolVector3Array merge_vertices;
	PoolVector3Array merge_normals;
	PoolRealArray merge_tangents;
	PoolColorArray merge_colors;
	PoolVector2Array merge_uv;
	PoolVector2Array merge_uv2;
	PoolRealArray merge_bones;
	PoolRealArray merge_weights;
	PoolIntArray merge_indices;

	if (!merge[ArrayMesh::ARRAY_VERTEX].is_null()) {
		merge_vertices = merge[ArrayMesh::ARRAY_VERTEX];
	}

	if (!merge[ArrayMesh::ARRAY_NORMAL].is_null()) {
		merge_normals = merge[ArrayMesh::ARRAY_NORMAL];
	}

	if (!merge[ArrayMesh::ARRAY_TANGENT].is_null()) {
		merge_tangents = merge[ArrayMesh::ARRAY_TANGENT];
	}

	if (!merge[ArrayMesh::ARRAY_COLOR].is_null()) {
		merge_colors = merge[ArrayMesh::ARRAY_COLOR];
	}

	if (!merge[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		merge_uv = merge[ArrayMesh::ARRAY_TEX_UV];
	}

	if (!merge[ArrayMesh::ARRAY_TEX_UV2].is_null()) {
		merge_uv2 = merge[ArrayMesh::ARRAY_TEX_UV2];
	}

	if (!merge[ArrayMesh::ARRAY_BONES].is_null()) {
		merge_bones = merge[ArrayMesh::ARRAY_BONES];
	}

	if (!merge[ArrayMesh::ARRAY_WEIGHTS].is_null()) {
		merge_weights = merge[ArrayMesh::ARRAY_WEIGHTS];
	}

	if (!merge[ArrayMesh::ARRAY_INDEX].is_null()) {
		merge_indices = merge[ArrayMesh::ARRAY_INDEX];
	}

	//merge
	int ovc = vertices.size();
	vertices.append_array(merge_vertices);

	if (!arrays[ArrayMesh::ARRAY_NORMAL].is_null()) {
		if (merge_vertices.size() != merge_normals.size()) {
			for (int i = 0; i < merge_vertices.size(); ++i) {
				normals.append(Vector3());
			}
		} else {
			normals.append_array(merge_normals);
		}
	}

	if (!arrays[ArrayMesh::ARRAY_TANGENT].is_null()) {
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

	if (!arrays[ArrayMesh::ARRAY_COLOR].is_null()) {
		if (merge_vertices.size() != merge_colors.size()) {
			for (int i = 0; i < merge_vertices.size(); ++i) {
				colors.append(Color());
			}
		} else {
			colors.append_array(merge_colors);
		}
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		if (merge_vertices.size() != merge_uv.size()) {
			for (int i = 0; i < merge_vertices.size(); ++i) {
				uv.append(Vector2());
			}
		} else {
			uv.append_array(merge_uv);
		}
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV2].is_null()) {
		if (merge_vertices.size() != merge_uv2.size()) {
			for (int i = 0; i < merge_vertices.size(); ++i) {
				uv2.append(Vector2());
			}
		} else {
			uv2.append_array(merge_uv2);
		}
	}

	if (!arrays[ArrayMesh::ARRAY_BONES].is_null()) {
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

	if (!arrays[ArrayMesh::ARRAY_WEIGHTS].is_null()) {
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

	arrays[ArrayMesh::ARRAY_VERTEX] = vertices;

	if (!arrays[ArrayMesh::ARRAY_NORMAL].is_null()) {
		arrays[ArrayMesh::ARRAY_NORMAL] = normals;
	}

	if (!arrays[ArrayMesh::ARRAY_TANGENT].is_null()) {
		arrays[ArrayMesh::ARRAY_TANGENT] = tangents;
	}

	if (!arrays[ArrayMesh::ARRAY_COLOR].is_null()) {
		arrays[ArrayMesh::ARRAY_COLOR] = colors;
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		arrays[ArrayMesh::ARRAY_TEX_UV] = uv;
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV2].is_null()) {
		arrays[ArrayMesh::ARRAY_TEX_UV2] = uv2;
	}

	if (!arrays[ArrayMesh::ARRAY_BONES].is_null()) {
		arrays[ArrayMesh::ARRAY_BONES] = bones;
	}

	if (!arrays[ArrayMesh::ARRAY_WEIGHTS].is_null()) {
		arrays[ArrayMesh::ARRAY_WEIGHTS] = weights;
	}

	arrays[ArrayMesh::ARRAY_INDEX] = indices;

	mdr->set_array(arrays);
}

Array MDREDMeshUtils::get_arrays_prepared(Ref<MeshDataResource> mdr) {
	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		arrays.resize(ArrayMesh::ARRAY_MAX);

		arrays[ArrayMesh::ARRAY_VERTEX] = PoolVector3Array();
		arrays[ArrayMesh::ARRAY_NORMAL] = PoolVector3Array();
		arrays[ArrayMesh::ARRAY_TEX_UV] = PoolVector2Array();
		arrays[ArrayMesh::ARRAY_INDEX] = PoolIntArray();
	}

	return arrays;
}

// There are probably better ways to do this
bool MDREDMeshUtils::should_flip_reflected_triangle(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) {
	Vector3 reflected = reflect_vertex(v0, v1, v2);
	Vector3 nn = get_face_normal(v0, v1, v2);

	return should_triangle_flip(v0, v1, reflected, nn);
}
Vector3 MDREDMeshUtils::reflect_vertex(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) {
	return (v2 - v0).reflect((v1 - v0).normalized()) + v0;
}

Vector3 MDREDMeshUtils::get_face_normal_arr_ti(const PoolVector3Array &verts, const PoolIntArray &indices, const int triangle_index, const bool flipped) {
	return get_face_normal_arr(verts, indices, triangle_index * 3, flipped);
}
Vector3 MDREDMeshUtils::get_face_normal_arr(const PoolVector3Array &verts, const PoolIntArray &indices, const int index, const bool flipped) {
	Vector3 v0 = verts[indices[index]];
	Vector3 v1 = verts[indices[index + 1]];
	Vector3 v2 = verts[indices[index + 2]];

	return get_face_normal(v0, v1, v2, flipped);
}
Vector3 MDREDMeshUtils::get_face_normal(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, const bool flipped) {
	if (!flipped) {
		return Plane(v0, v1, v2).normal;
	} else {
		return Plane(v2, v1, v0).normal;
	}
}

bool MDREDMeshUtils::should_triangle_flip(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, const Vector3 &similar_dir_normal) {
	Vector3 normal = get_face_normal(v0, v1, v2);

	float ndns = normal.dot(similar_dir_normal);

	return ndns < 0;
}

bool MDREDMeshUtils::is_normal_similar(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, const Vector3 &similar_dir_normal) {
	Vector3 normal = get_face_normal(v0, v1, v2);

	float ndns = normal.dot(similar_dir_normal);

	return ndns >= 0;
}
bool MDREDMeshUtils::is_direction_similar(const Vector3 &d0, const Vector3 &d1) {
	float ndns = d0.dot(d1);
	return ndns >= 0;
}

void MDREDMeshUtils::flip_triangle_ti(Ref<MeshDataResource> mdr, const int triangle_index) {
	flip_triangle(mdr, triangle_index * 3);
}
void MDREDMeshUtils::flip_triangle(Ref<MeshDataResource> mdr, const int index) {
	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		arrays.resize(ArrayMesh::ARRAY_MAX);
	}

	if (arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return;
	}

	PoolIntArray indices = arrays[ArrayMesh::ARRAY_INDEX];

	int i0 = indices[index];
	int i2 = indices[index + 2];

	PoolIntArray::Write w = indices.write();

	w[index] = i2;
	w[index + 2] = i0;

	w.release();

	arrays[ArrayMesh::ARRAY_INDEX] = indices;

	mdr->set_array(arrays);
}
void MDREDMeshUtils::add_into_surface_tool(Ref<MeshDataResource> mdr, Ref<SurfaceTool> st) {
	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		arrays.resize(ArrayMesh::ARRAY_MAX);
	}

	PoolVector3Array vertices;
	PoolVector3Array normals;
	PoolRealArray tangents;
	PoolColorArray colors;
	PoolVector2Array uv;
	PoolVector2Array uv2;
	PoolRealArray bones;
	PoolRealArray weights;
	PoolIntArray indices;

	if (!arrays[ArrayMesh::ARRAY_VERTEX].is_null()) {
		vertices = arrays[ArrayMesh::ARRAY_VERTEX];
	}

	if (!arrays[ArrayMesh::ARRAY_NORMAL].is_null()) {
		normals = arrays[ArrayMesh::ARRAY_NORMAL];
	}

	if (!arrays[ArrayMesh::ARRAY_TANGENT].is_null()) {
		tangents = arrays[ArrayMesh::ARRAY_TANGENT];
	}

	if (!arrays[ArrayMesh::ARRAY_COLOR].is_null()) {
		colors = arrays[ArrayMesh::ARRAY_COLOR];
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		uv = arrays[ArrayMesh::ARRAY_TEX_UV];
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV2].is_null()) {
		uv2 = arrays[ArrayMesh::ARRAY_TEX_UV2];
	}

	if (!arrays[ArrayMesh::ARRAY_BONES].is_null()) {
		bones = arrays[ArrayMesh::ARRAY_BONES];
	}

	if (!arrays[ArrayMesh::ARRAY_WEIGHTS].is_null()) {
		weights = arrays[ArrayMesh::ARRAY_WEIGHTS];
	}

	if (!arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		indices = arrays[ArrayMesh::ARRAY_INDEX];
	}

	for (int i = 0; i < vertices.size(); ++i) {
		if (normals.size() > 0) {
			st->add_normal(normals[i]);
		}

		if (tangents.size() > 0) {
			int ti = i * 4;
			st->add_tangent(Plane(tangents[ti], tangents[ti + 1], tangents[ti + 2], tangents[ti + 3]));
		}

		if (colors.size() > 0) {
			st->add_color(colors[i]);
		}

		if (uv.size() > 0) {
			st->add_uv(uv[i]);
		}

		if (uv2.size() > 0) {
			st->add_uv2(uv2[i]);
		}

		if (bones.size() > 0) {
			int bi = i * 4;
			Vector<int> pia;

			pia.push_back(bones[bi]);
			pia.push_back(bones[bi + 1]);
			pia.push_back(bones[bi + 1]);
			pia.push_back(bones[bi + 1]);

			st->add_bones(pia);
		}

		if (weights.size() > 0) {
			int bi = i * 4;
			Vector<float> pia;

			pia.push_back(bones[bi]);
			pia.push_back(bones[bi + 1]);
			pia.push_back(bones[bi + 2]);
			pia.push_back(bones[bi + 3]);

			st->add_weights(pia);
		}

		st->add_vertex(vertices[i]);
	}

	PoolIntArray::Read r = indices.read();

	for (int i = 0; i < indices.size(); ++i) {
		st->add_index(r[i]);
	}

	r.release();
}

Array MDREDMeshUtils::generate_normals_arrs(Array arrays) {
	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		arrays.resize(ArrayMesh::ARRAY_MAX);
	}

	if (arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return arrays;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];
	PoolIntArray indices = arrays[ArrayMesh::ARRAY_INDEX];
	PoolVector3Array normals;
	normals.resize(vertices.size());
	PoolIntArray nc;
	nc.resize(vertices.size());

	PoolIntArray::Write ncw = nc.write();

	for (int i = 0; i < vertices.size(); ++i) {
		ncw[i] = 0;
	}

	ncw.release();

	for (int i = 0; i < indices.size(); i += 3) {
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		Vector3 v0 = vertices[i0];
		Vector3 v1 = vertices[i1];
		Vector3 v2 = vertices[i2];

		Vector3 n = Plane(v0, v1, v2).normal;

		if (n.is_equal_approx(Vector3())) {
			ERR_PRINT("Warning face's normal is zero! " + String(Vector3(i0, i1, i2)));
			n = Vector3(0, 0, 1);
		}

		if (nc[i0] == 0) {
			nc.set(i0, 1);
			normals.set(i0, n);
		} else {
			normals.set(i0, normals[i0].linear_interpolate(n, 0.5).normalized());
		}

		if (nc[i1] == 0) {
			nc.set(i1, 1);
			normals.set(i1, n);
		} else {
			normals.set(i1, normals[i1].linear_interpolate(n, 0.5).normalized());
		}

		if (nc[i2] == 0) {
			nc.set(i2, 1);
			normals.set(i2, n);
		} else {
			normals.set(i2, normals[i2].linear_interpolate(n, 0.5).normalized());
		}
	}

	arrays[ArrayMesh::ARRAY_NORMAL] = normals;

	return arrays;
}

void MDREDMeshUtils::generate_normals_mdr(Ref<MeshDataResource> mdr) {
	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		arrays.resize(ArrayMesh::ARRAY_MAX);
	}

	if (arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];
	PoolIntArray indices = arrays[ArrayMesh::ARRAY_INDEX];
	PoolVector3Array normals;
	normals.resize(vertices.size());
	PoolIntArray nc;
	nc.resize(vertices.size());

	PoolIntArray::Write ncw = nc.write();

	for (int i = 0; i < vertices.size(); ++i) {
		ncw[i] = 0;
	}

	ncw.release();

	for (int i = 0; i < indices.size(); i += 3) {
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		Vector3 v0 = vertices[i0];
		Vector3 v1 = vertices[i1];
		Vector3 v2 = vertices[i2];

		Vector3 n = Plane(v0, v1, v2).normal;

		if (n.is_equal_approx(Vector3())) {
			ERR_PRINT("Warning face's normal is zero! " + String(Vector3(i0, i1, i2)));
			n = Vector3(0, 0, 1);
		}

		if (nc[i0] == 0) {
			nc.set(i0, 1);
			normals.set(i0, n);
		} else {
			normals.set(i0, normals[i0].linear_interpolate(n, 0.5).normalized());
		}

		if (nc[i1] == 0) {
			nc.set(i1, 1);
			normals.set(i1, n);
		} else {
			normals.set(i1, normals[i1].linear_interpolate(n, 0.5).normalized());
		}

		if (nc[i2] == 0) {
			nc.set(i2, 1);
			normals.set(i2, n);
		} else {
			normals.set(i2, normals[i2].linear_interpolate(n, 0.5).normalized());
		}
	}

	arrays[ArrayMesh::ARRAY_NORMAL] = normals;

	mdr->set_array(arrays);
}

// Apparently surfacetool adds more verts during normal generation
// Keeping this here for now
void MDREDMeshUtils::generate_normals_surface_tool(Ref<MeshDataResource> mdr) {
	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		arrays.resize(ArrayMesh::ARRAY_MAX);
	}

	if (arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return;
	}

	Ref<SurfaceTool> st;
	st.instance();
	st->begin(Mesh::PRIMITIVE_TRIANGLES);

	add_into_surface_tool(mdr, st);

	st->generate_normals();

	mdr->set_array(st->commit_to_arrays());
}

void MDREDMeshUtils::generate_tangents(Ref<MeshDataResource> mdr) {
	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		arrays.resize(ArrayMesh::ARRAY_MAX);
	}

	if (arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return;
	}

	Ref<SurfaceTool> st;
	st.instance();
	st->begin(Mesh::PRIMITIVE_TRIANGLES);

	add_into_surface_tool(mdr, st);

	st->generate_tangents();

	mdr->set_array(st->commit_to_arrays());
}

Array MDREDMeshUtils::remove_used_vertices(Array arrays) {
	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return arrays;
	}

	if (arrays[ArrayMesh::ARRAY_VERTEX].is_null() || arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return arrays;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];
	int vert_size = vertices.size();
	PoolIntArray indices = arrays[ArrayMesh::ARRAY_INDEX];
	PoolIntArray unused_indices;

	for (int i = 0; i < vert_size; ++i) {
		if (!pool_int_arr_contains(indices, i)) {
			unused_indices.append(i);
		}
	}

	remove_vertices(arrays, unused_indices);

	return arrays;
}

Array MDREDMeshUtils::remove_vertices(Array arrays, const PoolIntArray &indices) {
	if (indices.size() == 0) {
		return arrays;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];
	PoolVector3Array normals;
	PoolRealArray tangents;
	PoolColorArray colors;
	PoolVector2Array uv;
	PoolVector2Array uv2;
	PoolRealArray bones;
	PoolRealArray weights;

	if (!arrays[ArrayMesh::ARRAY_NORMAL].is_null()) {
		normals = arrays[ArrayMesh::ARRAY_NORMAL];
	}

	if (!arrays[ArrayMesh::ARRAY_TANGENT].is_null()) {
		tangents = arrays[ArrayMesh::ARRAY_TANGENT];
	}

	if (!arrays[ArrayMesh::ARRAY_COLOR].is_null()) {
		colors = arrays[ArrayMesh::ARRAY_COLOR];
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		uv = arrays[ArrayMesh::ARRAY_TEX_UV];
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV2].is_null()) {
		uv2 = arrays[ArrayMesh::ARRAY_TEX_UV2];
	}

	if (!arrays[ArrayMesh::ARRAY_BONES].is_null()) {
		bones = arrays[ArrayMesh::ARRAY_BONES];
	}

	if (!arrays[ArrayMesh::ARRAY_WEIGHTS].is_null()) {
		weights = arrays[ArrayMesh::ARRAY_WEIGHTS];
	}

	for (int i = 0; i < indices.size(); ++i) {
		int index = indices[i];

		vertices.remove(index);

		if (!arrays[ArrayMesh::ARRAY_NORMAL].is_null()) {
			normals.remove(index);
		}

		if (!arrays[ArrayMesh::ARRAY_TANGENT].is_null()) {
			int tindex = index * 4;

			tangents.remove(tindex);
			tangents.remove(tindex);
			tangents.remove(tindex);
			tangents.remove(tindex);
		}

		if (!arrays[ArrayMesh::ARRAY_COLOR].is_null()) {
			colors.remove(index);
		}

		if (!arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
			uv.remove(index);
		}

		if (!arrays[ArrayMesh::ARRAY_TEX_UV2].is_null()) {
			uv2.remove(index);
		}

		if (!arrays[ArrayMesh::ARRAY_BONES].is_null()) {
			bones.remove(index);
			bones.remove(index);
			bones.remove(index);
			bones.remove(index);
		}

		if (!arrays[ArrayMesh::ARRAY_WEIGHTS].is_null()) {
			weights.remove(index);
			weights.remove(index);
			weights.remove(index);
			weights.remove(index);
		}
	}

	//write back
	arrays[ArrayMesh::ARRAY_VERTEX] = vertices;

	if (!arrays[ArrayMesh::ARRAY_NORMAL].is_null()) {
		arrays[ArrayMesh::ARRAY_NORMAL] = normals;
	}

	if (!arrays[ArrayMesh::ARRAY_TANGENT].is_null()) {
		arrays[ArrayMesh::ARRAY_TANGENT] = tangents;
	}

	if (!arrays[ArrayMesh::ARRAY_COLOR].is_null()) {
		arrays[ArrayMesh::ARRAY_COLOR] = colors;
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		arrays[ArrayMesh::ARRAY_TEX_UV] = uv;
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV2].is_null()) {
		arrays[ArrayMesh::ARRAY_TEX_UV2] = uv2;
	}

	if (!arrays[ArrayMesh::ARRAY_BONES].is_null()) {
		arrays[ArrayMesh::ARRAY_BONES] = bones;
	}

	if (!arrays[ArrayMesh::ARRAY_WEIGHTS].is_null()) {
		arrays[ArrayMesh::ARRAY_WEIGHTS] = weights;
	}

	if (arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return arrays;
	}

	//update indices
	PoolIntArray arr_indices = arrays[ArrayMesh::ARRAY_INDEX];

	int max_index = find_max(indices);

	for (int k = 0; k < indices.size(); ++k) {
		for (int i = 0; i < arr_indices.size(); ++i) {
			int ai = arr_indices[i];

			if (ai >= max_index) {
				arr_indices.set(i, ai - 1);
			}
		}

		max_index = find_max_capped(indices, max_index);
	}

	arrays[ArrayMesh::ARRAY_INDEX] = arr_indices;

	return arrays;
}

int MDREDMeshUtils::find_max(const PoolIntArray &arr) {
	if (arr.size() == 0) {
		return 0;
	}

	PoolIntArray::Read r = arr.read();

	int m = r[0];

	for (int i = 1; i < arr.size(); ++i) {
		int v = r[i];

		if (v > m) {
			m = v;
		}
	}

	r.release();

	return m;
}
int MDREDMeshUtils::find_max_capped(const PoolIntArray &arr, const int last) {
	if (arr.size() == 0) {
		return 0;
	}

	PoolIntArray::Read r = arr.read();

	int m = 0;

	int i = 0;

	for (; i < arr.size(); ++i) {
		int v = r[i];

		if (v < last) {
			m = v;
			break;
		}
	}

	for (; i < arr.size(); ++i) {
		int v = r[i];
		if (v > m && v < last) {
			m = v;
		}
	}

	return m;
}

PoolIntArray MDREDMeshUtils::order_seam_indices(const PoolIntArray &arr) {
	PoolIntArray ret;

	if (arr.size() == 0) {
		return ret;
	}

	for (int i = 0; i < arr.size(); i += 2) {
		int index0 = arr[i];
		int index1 = arr[i + 1];

		if (index0 > index1) {
			int t = index1;
			index1 = index0;
			index0 = t;
		}

		ret.push_back(index0);
		ret.push_back(index1);
	}

	return ret;
}

void MDREDMeshUtils::add_seam_not_ordered(Ref<MeshDataResource> mdr, const int index0, const int index1) {
	if (index0 > index1) {
		add_seam(mdr, index1, index0);
	} else {
		add_seam(mdr, index0, index1);
	}
}
void MDREDMeshUtils::remove_seam_not_ordered(Ref<MeshDataResource> mdr, const int index0, const int index1) {
	if (index0 > index1) {
		remove_seam(mdr, index1, index0);
	} else {
		remove_seam(mdr, index0, index1);
	}
}

bool MDREDMeshUtils::has_seam(Ref<MeshDataResource> mdr, const int index0, const int index1) {
	PoolIntArray seams = mdr->get_seams();

	for (int i = 0; i < seams.size(); i += 2) {
		if (seams[i] == index0 && seams[i + 1] == index1) {
			return true;
		}
	}

	return false;
}
void MDREDMeshUtils::add_seam(Ref<MeshDataResource> mdr, const int index0, const int index1) {
	if (has_seam(mdr, index0, index1)) {
		return;
	}

	PoolIntArray seams = mdr->get_seams();
	seams.push_back(index0);
	seams.push_back(index1);
	mdr->set_seams(seams);
}
void MDREDMeshUtils::remove_seam(Ref<MeshDataResource> mdr, const int index0, const int index1) {
	if (!has_seam(mdr, index0, index1)) {
		return;
	}

	PoolIntArray seams = mdr->get_seams();

	for (int i = 0; i < seams.size(); i += 2) {
		if (seams[i] == index0 && seams[i + 1] == index1) {
			seams.remove(i);
			seams.remove(i);
			mdr->set_seams(seams);
			return;
		}
	}
}

bool MDREDMeshUtils::is_verts_equal(const Vector3 &v0, const Vector3 &v1) {
	return Math::is_equal_approx(v0.x, v1.x) && Math::is_equal_approx(v0.y, v1.y) && Math::is_equal_approx(v0.z, v1.z);
}

void MDREDMeshUtils::points_to_seams(Ref<MeshDataResource> mdr, const PoolVector3Array &points) {
	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return;
	}

	if (arrays[ArrayMesh::ARRAY_VERTEX].is_null()) {
		return;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];
	PoolIntArray indices = arrays[ArrayMesh::ARRAY_INDEX];
	PoolIntArray seams;

	for (int i = 0; i < points.size(); i += 2) {
		Vector3 p0 = points[i];
		Vector3 p1 = points[i + 1];

		for (int j = 0; j < indices.size(); j += 3) {
			Vector3 v0 = vertices[indices[j]];
			Vector3 v1 = vertices[indices[j + 1]];
			Vector3 v2 = vertices[indices[j + 2]];

			int p0_index = -1;

			if (is_verts_equal(p0, v0)) {
				p0_index = indices[j];
			} else if (is_verts_equal(p0, v1)) {
				p0_index = indices[j + 1];
			} else if (is_verts_equal(p0, v2)) {
				p0_index = indices[j + 2];
			}

			if (p0_index == -1) {
				continue;
			}

			int p1_index = -1;

			if (is_verts_equal(p1, v0)) {
				p1_index = indices[j];
			} else if (is_verts_equal(p1, v1)) {
				p1_index = indices[j + 1];
			} else if (is_verts_equal(p1, v2)) {
				p1_index = indices[j + 2];
			}

			if (p1_index == -1) {
				continue;
			}

			if (p0_index == p1_index) {
				continue;
			}

			if (p0_index > p1_index) {
				int t = p0_index;
				p0_index = p1_index;
				p1_index = t;
			}

			seams.push_back(p0_index);
			seams.push_back(p1_index);
			break;
		}
	}

	mdr->set_seams(seams);
}

PoolVector3Array MDREDMeshUtils::seams_to_points(Ref<MeshDataResource> mdr) {
	PoolVector3Array points;

	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return points;
	}

	if (arrays[ArrayMesh::ARRAY_VERTEX].is_null()) {
		return points;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];
	PoolIntArray seams = mdr->get_seams();

	PoolVector3Array::Read vr = vertices.read();
	PoolIntArray::Read sr = seams.read();

	for (int i = 0; i < seams.size(); ++i) {
		points.push_back(vr[sr[i]]);
	}

	vr.release();
	sr.release();

	return points;
}

bool MDREDMeshUtils::is_matching_seam(int i0, int i1, const int si0, const int si1) {
	if (i0 > i1) {
		int t = i0;
		i0 = i1;
		i1 = t;
	}

	return (i0 == si0) && (i1 == si1);
}

bool MDREDMeshUtils::pool_int_arr_contains(const PoolIntArray &arr, const int val) {
	PoolIntArray::Read ar = arr.read();

	for (int i = 0; i < arr.size(); ++i) {
		if (ar[i] == val) {
			return true;
		}
	}

	return false;
}

void MDREDMeshUtils::apply_seam(Ref<MeshDataResource> mdr) {
	PoolVector3Array points;

	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return;
	}

	if (arrays[ArrayMesh::ARRAY_VERTEX].is_null()) {
		return;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];
	PoolIntArray indices = arrays[ArrayMesh::ARRAY_INDEX];
	PoolIntArray new_indices;
	new_indices.append_array(indices);
	PoolIntArray seams = mdr->get_seams();

	// Duplication happens later, as it requires lots of logic
	PoolIntArray duplicate_verts_indices;
	int new_vert_size = vertices.size();

	for (int i = 0; i < vertices.size(); ++i) {
		// first check if vertex is a part of at least 2 edge seams
		int test_seam_count = 0;
		PoolIntArray::Read seams_read = seams.read();
		for (int si = 0; si < seams.size(); ++si) {
			int s = seams_read[si];

			if (s == i) {
				test_seam_count += 1;

				if (test_seam_count >= 2) {
					break;
				}
			}
		}

		seams_read.release();

		if (test_seam_count < 2) {
			continue;
		}

		// Collect all triangles that use this vertex as SeamTriangleHelpers
		Vector<SeamTriangleHelper> triangles;
		PoolIntArray::Read indices_read = indices.read();
		for (int j = 0; j < indices.size(); ++j) {
			int i0 = indices_read[j];

			if (i0 != i) {
				continue;
			}

			int tri_j_offset = j % 3;
			int tri_start_index = j - tri_j_offset;

			int i1 = indices_read[tri_start_index + ((tri_j_offset + 1) % 3)];
			int i2 = indices_read[tri_start_index + ((tri_j_offset + 2) % 3)];

			SeamTriangleHelper s;
			s.setup(i0, i1, i2, i0, j, seams);
			triangles.push_back(s);
		}
		indices_read.release();

		Vector<Vector<SeamTriangleHelper>> triangle_arrays;
		while (true) {
			// First find a triangle that needs to be cut
			SeamTriangleHelper tri;
			int tri_index = -1;
			for (int it = 0; it < triangles.size(); ++it) {
				tri = triangles[it];

				if (tri.has_cut() && !tri.processed) {
					tri_index = it;
					break;
				}
			}

			if (tri_index == -1) {
				//done
				break;
			}

			tri.processed = true;
			triangles.write[tri_index].processed = true;

			if (tri.both_sides_need_cut()) {
				Vector<SeamTriangleHelper> trih;
				trih.push_back(tri);
				triangle_arrays.push_back(trih);
				continue;
			}

			Vector<SeamTriangleHelper> collected_triangles;
			collected_triangles.push_back(tri);

			// Find all neighbours and set them to processed + update the index for them
			//var side_index : int = tri.get_side_index_cut()
			SeamTriangleHelper neighbour_tri = tri;
			int find_neighbour_for_edge_index = tri.get_opposite_side_index_cut();
			int find_neighbour_for_edge = neighbour_tri.get_side_index(find_neighbour_for_edge_index);
			bool tri_found = true;
			while (tri_found) {
				tri_found = false;

				for (int ntriindx = 0; ntriindx < triangles.size(); ++ntriindx) {
					const SeamTriangleHelper &ntri = triangles[ntriindx];
					if (ntri.processed) {
						continue;
					}

					if (ntri.is_the_same(neighbour_tri)) {
						continue;
					}

					if (ntri.is_neighbour_to(find_neighbour_for_edge)) {
						neighbour_tri = ntri;
						find_neighbour_for_edge = neighbour_tri.get_other_side_index(find_neighbour_for_edge);

						neighbour_tri.processed = true;
						triangles.write[ntriindx].processed = true;
						tri_found = true;
						collected_triangles.push_back(neighbour_tri);

						if (neighbour_tri.has_cut()) {
							// Done with this "strip"
							tri_found = false;
						}

						break;
					}
				}
			}

			triangle_arrays.push_back(collected_triangles);
		}

		// triangle_arrays is guaranteed to have at least 2 entries
		// Skip processing the first strip, so we don't create unused verts
		PoolIntArray::Write new_indices_write = new_indices.write();
		for (int tind = 1; tind < triangle_arrays.size(); ++tind) {
			const Vector<SeamTriangleHelper> &tris = triangle_arrays[tind];

			duplicate_verts_indices.push_back(tris[0].orig_index);

			for (int triindx = 0; triindx < tris.size(); ++triindx) {
				const SeamTriangleHelper &tri = tris[triindx];

				new_indices_write[tri.index_index] = new_vert_size;
			}

			new_vert_size += 1;
		}
		new_indices_write.release();
	}

	arrays[ArrayMesh::ARRAY_INDEX] = new_indices;

	mdr->set_array(seam_apply_duplicate_vertices(arrays, duplicate_verts_indices));
}
void MDREDMeshUtils::apply_seam_old(Ref<MeshDataResource> mdr) {
	PoolVector3Array points;

	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return;
	}

	if (arrays[ArrayMesh::ARRAY_VERTEX].is_null()) {
		return;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];
	PoolIntArray indices = arrays[ArrayMesh::ARRAY_INDEX];

	PoolIntArray seams = mdr->get_seams();

	// Duplication happens later, as it requires lots of logic
	PoolIntArray duplicate_verts_indices;
	int new_vert_size = vertices.size();

	for (int i = 0; i < seams.size(); i += 2) {
		int si0 = seams[i];
		int si1 = seams[i + 1];

		bool first = true;
		for (int j = 0; j < indices.size(); j += 3) {
			int i0 = indices[j];
			int i1 = indices[j + 1];
			int i2 = indices[j + 2];

			int edge_int_tri_index = -1;

			if (is_matching_seam(i0, i1, si0, si1)) {
				edge_int_tri_index = 0;
			} else if (is_matching_seam(i1, i2, si0, si1)) {
				edge_int_tri_index = 1;
			} else if (is_matching_seam(i2, i0, si0, si1)) {
				edge_int_tri_index = 2;
			}

			if (edge_int_tri_index == -1) {
				continue;
			}

			if (first) {
				// Only split away the subsequent tris
				first = false;
				continue;
			}

			if (edge_int_tri_index == 0) {
				duplicate_verts_indices.push_back(i0);
				duplicate_verts_indices.push_back(i1);

				indices.push_back(new_vert_size);
				indices.push_back(new_vert_size + 1);
				indices.push_back(i2);
			} else if (edge_int_tri_index == 1) {
				duplicate_verts_indices.push_back(i1);
				duplicate_verts_indices.push_back(i2);

				indices.push_back(i0);
				indices.push_back(new_vert_size);
				indices.push_back(new_vert_size + 1);
			} else if (edge_int_tri_index == 2) {
				duplicate_verts_indices.push_back(i0);
				duplicate_verts_indices.push_back(i2);

				indices.push_back(new_vert_size);
				indices.push_back(i1);
				indices.push_back(new_vert_size + 1);
			}

			indices.remove(j);
			indices.remove(j);
			indices.remove(j);
			j -= 3;

			new_vert_size += 2;
		}
	}

	arrays[ArrayMesh::ARRAY_INDEX] = indices;
	//mdr.array = arrays

	mdr->set_array(seam_apply_duplicate_vertices(arrays, duplicate_verts_indices));
}

// This will not touch the indices!
Array MDREDMeshUtils::seam_apply_duplicate_vertices(Array arrays, const PoolIntArray &duplicate_verts_indices) {
	if (duplicate_verts_indices.size() == 0) {
		return arrays;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];

	PoolVector3Array normals;
	PoolRealArray tangents;
	PoolColorArray colors;
	PoolVector2Array uv;
	PoolVector2Array uv2;
	PoolRealArray bones;
	PoolRealArray weights;

	if (!arrays[ArrayMesh::ARRAY_NORMAL].is_null()) {
		normals = arrays[ArrayMesh::ARRAY_NORMAL];
	}

	if (!arrays[ArrayMesh::ARRAY_TANGENT].is_null()) {
		tangents = arrays[ArrayMesh::ARRAY_TANGENT];
	}

	if (!arrays[ArrayMesh::ARRAY_COLOR].is_null()) {
		colors = arrays[ArrayMesh::ARRAY_COLOR];
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		uv = arrays[ArrayMesh::ARRAY_TEX_UV];
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV2].is_null()) {
		uv2 = arrays[ArrayMesh::ARRAY_TEX_UV2];
	}

	if (!arrays[ArrayMesh::ARRAY_BONES].is_null()) {
		bones = arrays[ArrayMesh::ARRAY_BONES];
	}

	if (!arrays[ArrayMesh::ARRAY_WEIGHTS].is_null()) {
		weights = arrays[ArrayMesh::ARRAY_WEIGHTS];
	}

	PoolIntArray::Read dvir = duplicate_verts_indices.read();
	for (int i = 0; i < duplicate_verts_indices.size(); ++i) {
		int index = dvir[i];

		vertices.push_back(vertices[index]);

		if (!arrays[ArrayMesh::ARRAY_NORMAL].is_null()) {
			normals.push_back(normals[index]);
		}

		if (!arrays[ArrayMesh::ARRAY_TANGENT].is_null()) {
			tangents.push_back(tangents[index]);
			tangents.push_back(tangents[index + 1]);
			tangents.push_back(tangents[index + 2]);
			tangents.push_back(tangents[index + 3]);
		}

		if (!arrays[ArrayMesh::ARRAY_COLOR].is_null()) {
			colors.push_back(colors[index]);
		}

		if (!arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
			uv.push_back(uv[index]);
		}

		if (!arrays[ArrayMesh::ARRAY_TEX_UV2].is_null()) {
			uv2.push_back(uv2[index]);
		}

		if (!arrays[ArrayMesh::ARRAY_BONES].is_null()) {
			bones.push_back(bones[index]);
			bones.push_back(bones[index + 1]);
			bones.push_back(bones[index + 2]);
			bones.push_back(bones[index + 3]);
		}

		if (!arrays[ArrayMesh::ARRAY_WEIGHTS].is_null()) {
			weights.push_back(weights[index]);
			weights.push_back(weights[index + 1]);
			weights.push_back(weights[index + 2]);
			weights.push_back(weights[index + 3]);
		}
	}

	dvir.release();

	//write back

	arrays[ArrayMesh::ARRAY_VERTEX] = vertices;

	if (!arrays[ArrayMesh::ARRAY_NORMAL].is_null()) {
		arrays[ArrayMesh::ARRAY_NORMAL] = normals;
	}

	if (!arrays[ArrayMesh::ARRAY_TANGENT].is_null()) {
		arrays[ArrayMesh::ARRAY_TANGENT] = tangents;
	}

	if (!arrays[ArrayMesh::ARRAY_COLOR].is_null()) {
		arrays[ArrayMesh::ARRAY_COLOR] = colors;
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV].is_null()) {
		arrays[ArrayMesh::ARRAY_TEX_UV] = uv;
	}

	if (!arrays[ArrayMesh::ARRAY_TEX_UV2].is_null()) {
		arrays[ArrayMesh::ARRAY_TEX_UV2] = uv2;
	}

	if (!arrays[ArrayMesh::ARRAY_BONES].is_null()) {
		arrays[ArrayMesh::ARRAY_BONES] = bones;
	}

	if (!arrays[ArrayMesh::ARRAY_WEIGHTS].is_null()) {
		arrays[ArrayMesh::ARRAY_WEIGHTS] = weights;
	}

	return arrays;
}
