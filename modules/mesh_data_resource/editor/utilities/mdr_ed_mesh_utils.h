#ifndef MDR_ED_MESH_UTILS_H
#define MDR_ED_MESH_UTILS_H

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

#include "core/ustring.h"
#include "core/pool_vector.h"
#include "core/math/vector3.h"
#include "core/reference.h"
#include "core/array.h"

class MeshDataResource;
class SurfaceTool;

class MDREDMeshUtils {
public:
	struct SeamTriangleHelper {
		int i0;
		int i1;
		int i2;
		int orig_index;
		int index_index;

		int side_index_1;
		int side_index_2;
		bool side_index_1_cut;
		bool side_index_2_cut;

		bool processed;

		int get_other_side_index(int index);

		int get_side_index(int i);
		int get_side_index_cut();
		int get_opposite_side_index_cut();
		bool is_side_index_cut(int i);

		bool is_the_same(SeamTriangleHelper h);

		bool is_triangle(int pi0, int pi1, int pi2);
		bool is_neighbour_to(int index);

		bool needs_to_be_cut_near(int index);
		bool has_cut();
		bool both_sides_need_cut();

		void setup(int pi0, int pi1, int pi2, int porig_ind, int pindex_index, PoolIntArray seams);

		void determine_cuts(PoolIntArray seams);
		bool check_cut(int ind0, int ind1, PoolIntArray seams);

		String _to_string();

		SeamTriangleHelper();
	};

	static void remove_triangle(Ref<MeshDataResource> mdr, int triangle_index);
	static bool add_triangulated_mesh_from_points_delaunay(Ref<MeshDataResource> mdr, PoolVector3Array selected_points, Vector3 last_known_camera_facing);
	static void add_triangulated_mesh_from_points(Ref<MeshDataResource> mdr, PoolVector3Array selected_points, Vector3 last_known_camera_facing);

	// Appends a triangle to the mesh. It's created from miroring v2 to the ev0, and ev1 edge
	static void append_triangle_to_tri_edge(Ref<MeshDataResource> mdr, Vector3 ev0, Vector3 ev1, Vector3 v2);

	static void add_triangle_at(Ref<MeshDataResource> mdr, Vector3 v0, Vector3 v1, Vector3 v2, bool flip = false);

	static void add_triangle(Ref<MeshDataResource> mdr);

	// Appends a quad to the mesh. It's created to the opposite side of v2 to the ev0, and ev1 edge
	static void append_quad_to_tri_edge(Ref<MeshDataResource> mdr, Vector3 ev0, Vector3 ev1, Vector3 v2);
	static void add_quad_at(Ref<MeshDataResource> mdr, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3, bool flip = false);
	static void add_quad(Ref<MeshDataResource> mdr);
	static void add_box(Ref<MeshDataResource> mdr);
	static void merge_in_surface_tool(Ref<MeshDataResource> mdr, Ref<SurfaceTool> st, bool generate_normals_if_needed = true, bool generate_tangents_if_needed = true);
	static void merge_in_arrays(Ref<MeshDataResource> mdr, Array merge);
	static Array get_arrays_prepared(Ref<MeshDataResource> mdr);

	// There are probably better ways to do this
	static bool should_flip_reflected_triangle(Vector3 v0, Vector3 v1, Vector3 v2);
	static Vector3 reflect_vertex(Vector3 v0, Vector3 v1, Vector3 v2);

	static Vector3 get_face_normal_arr_ti(PoolVector3Array verts, PoolIntArray indices, int triangle_index, bool flipped = false);
	static Vector3 get_face_normal_arr(PoolVector3Array verts, PoolIntArray indices, int index, bool flipped = false);
	static Vector3 get_face_normal(Vector3 v0, Vector3 v1, Vector3 v2, bool flipped = false);

	static bool should_triangle_flip(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 similar_dir_normal);

	static bool is_normal_similar(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 similar_dir_normal);
	static bool is_direction_similar(Vector3 d0, Vector3 d1);

	static void flip_triangle_ti(Ref<MeshDataResource> mdr, int triangle_index);
	static void flip_triangle(Ref<MeshDataResource> mdr, int index);
	static void add_into_surface_tool(Ref<MeshDataResource> mdr, Ref<SurfaceTool> st);

	static Array generate_normals_arrs(Array arrays);
	static void generate_normals_mdr(Ref<MeshDataResource> mdr);

	// Apparently surfacetool adds more verts during normal generation
	// Keeping this here for now
	static void generate_normals_surface_tool(Ref<MeshDataResource> mdr);
	static void generate_tangents(Ref<MeshDataResource> mdr);

	static Array remove_used_vertices(Array arrays);

	static Array remove_vertices(Array arrays, PoolIntArray indices);

	static int find_max(PoolIntArray arr);
	static int find_max_capped(PoolIntArray arr, int last);

	static PoolIntArray order_seam_indices(PoolIntArray arr);

	static void add_seam_not_ordered(Ref<MeshDataResource> mdr, int index0, int index1);
	static void remove_seam_not_ordered(Ref<MeshDataResource> mdr, int index0, int index1);

	static bool has_seam(Ref<MeshDataResource> mdr, int index0, int index1);
	static void add_seam(Ref<MeshDataResource> mdr, int index0, int index1);
	static void remove_seam(Ref<MeshDataResource> mdr, int index0, int index1);

	static bool is_verts_equal(Vector3 v0, Vector3 v1);

	static void points_to_seams(Ref<MeshDataResource> mdr, PoolVector3Array points);
	static PoolVector3Array seams_to_points(Ref<MeshDataResource> mdr);
	static bool is_matching_seam(int i0, int i1, int si0, int si1);

	static bool pool_int_arr_contains(PoolIntArray arr, int val);

	static void apply_seam(Ref<MeshDataResource> mdr);
	static void apply_seam_old(Ref<MeshDataResource> mdr);

	// This will not touch the indices!
	static Array seam_apply_duplicate_vertices(Array arrays, PoolIntArray duplicate_verts_indices);
};

#endif
