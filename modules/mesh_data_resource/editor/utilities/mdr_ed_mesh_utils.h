#ifndef MDR_ED_MESH_UTILS_H
#define MDR_ED_MESH_UTILS_H



#include "core/string/ustring.h"
#include "core/containers/pool_vector.h"
#include "core/math/vector3.h"
#include "core/object/reference.h"
#include "core/variant/array.h"

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

		int get_other_side_index(int index) const;

		int get_side_index(int i) const;
		int get_side_index_cut() const;
		int get_opposite_side_index_cut() const;
		bool is_side_index_cut(int i) const;

		bool is_the_same(const SeamTriangleHelper &h) const;

		bool is_triangle(int pi0, int pi1, int pi2) const;
		bool is_neighbour_to(int index) const;

		bool needs_to_be_cut_near(int index) const;
		bool has_cut() const;
		bool both_sides_need_cut() const;

		void setup(int pi0, int pi1, int pi2, int porig_ind, int pindex_index, const PoolIntArray &seams);

		void determine_cuts(const PoolIntArray &seams);
		bool check_cut(int ind0, int ind1, const PoolIntArray &seams);

		String _to_string();

		SeamTriangleHelper();
	};

	static void remove_triangle(Ref<MeshDataResource> mdr, const int triangle_index);
	static bool add_triangulated_mesh_from_points_delaunay(Ref<MeshDataResource> mdr, const PoolVector3Array &selected_points, const Vector3 &last_known_camera_facing);
	static void add_triangulated_mesh_from_points(Ref<MeshDataResource> mdr, const PoolVector3Array &selected_points, const Vector3 &last_known_camera_facing);

	// Appends a triangle to the mesh. It's created from mirroring v2 to the ev0, and ev1 edge
	static void append_triangle_to_tri_edge(Ref<MeshDataResource> mdr, const Vector3 &ev0, const Vector3 &ev1, const Vector3 &v2);

	static void add_triangle_at(Ref<MeshDataResource> mdr, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, const bool flip = false);

	static void add_triangle(Ref<MeshDataResource> mdr);

	// Appends a quad to the mesh. It's created to the opposite side of v2 to the ev0, and ev1 edge
	static void append_quad_to_tri_edge(Ref<MeshDataResource> mdr, const Vector3 &ev0, const Vector3 &ev1, const Vector3 &v2);
	static void add_quad_at(Ref<MeshDataResource> mdr, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const bool flip = false);
	static void add_quad(Ref<MeshDataResource> mdr);
	static void add_box(Ref<MeshDataResource> mdr);
	static void merge_in_surface_tool(Ref<MeshDataResource> mdr, Ref<SurfaceTool> st, const bool generate_normals_if_needed = true, const bool generate_tangents_if_needed = true);
	static void merge_in_arrays(Ref<MeshDataResource> mdr, const Array &merge);
	static Array get_arrays_prepared(Ref<MeshDataResource> mdr);

	// There are probably better ways to do this
	static bool should_flip_reflected_triangle(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2);
	static Vector3 reflect_vertex(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2);

	static Vector3 get_face_normal_arr_ti(const PoolVector3Array &verts, const PoolIntArray &indices, const int triangle_index, const bool flipped = false);
	static Vector3 get_face_normal_arr(const PoolVector3Array &verts, const PoolIntArray &indices, const int index, const bool flipped = false);
	static Vector3 get_face_normal(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, const bool flipped = false);

	static bool should_triangle_flip(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, const Vector3 &similar_dir_normal);

	static bool is_normal_similar(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2, const Vector3 &similar_dir_normal);
	static bool is_direction_similar(const Vector3 &d0, const Vector3 &d1);

	static void flip_triangle_ti(Ref<MeshDataResource> mdr, const int triangle_index);
	static void flip_triangle(Ref<MeshDataResource> mdr, const int index);
	static void add_into_surface_tool(Ref<MeshDataResource> mdr, Ref<SurfaceTool> st);

	static Array generate_normals_arrs(Array arrays);
	static void generate_normals_mdr(Ref<MeshDataResource> mdr);

	// Apparently surfacetool adds more verts during normal generation
	// Keeping this here for now
	static void generate_normals_surface_tool(Ref<MeshDataResource> mdr);
	static void generate_tangents(Ref<MeshDataResource> mdr);

	static Array remove_used_vertices(Array arrays);

	static Array remove_vertices(Array arrays, const PoolIntArray &indices);

	static int find_max(const PoolIntArray &arr);
	static int find_max_capped(const PoolIntArray &arr, const int last);

	static PoolIntArray order_seam_indices(const PoolIntArray &arr);

	static void add_seam_not_ordered(Ref<MeshDataResource> mdr, const int index0, const int index1);
	static void remove_seam_not_ordered(Ref<MeshDataResource> mdr, const int index0, const int index1);

	static bool has_seam(Ref<MeshDataResource> mdr, const int index0, const int index1);
	static void add_seam(Ref<MeshDataResource> mdr, const int index0, const int index1);
	static void remove_seam(Ref<MeshDataResource> mdr, const int index0, const int index1);

	static bool is_verts_equal(const Vector3 &v0, const Vector3 &v1);

	static void points_to_seams(Ref<MeshDataResource> mdr, const PoolVector3Array &points);
	static PoolVector3Array seams_to_points(Ref<MeshDataResource> mdr);
	static bool is_matching_seam(int i0, int i1, const int si0, const int si1);

	static bool pool_int_arr_contains(const PoolIntArray &arr, const int val);

	static void apply_seam(Ref<MeshDataResource> mdr);
	static void apply_seam_old(Ref<MeshDataResource> mdr);

	// This will not touch the indices!
	static Array seam_apply_duplicate_vertices(Array arrays, const PoolIntArray &duplicate_verts_indices);
};

#endif
