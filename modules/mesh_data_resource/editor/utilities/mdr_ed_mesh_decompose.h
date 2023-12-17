#ifndef MDR_ED_MESH_DECOMPOSE_H
#define MDR_ED_MESH_DECOMPOSE_H



#include "core/variant/array.h"
#include "core/containers/pool_vector.h"
#include "core/object/reference.h"
#include "core/variant/variant.h"
#include "core/containers/vector.h"

class MeshDataResource;

class MDREDMeshDecompose {
public:
	struct HandleVertexMapResult {
		Vector<PoolIntArray> handle_to_vertex_map;
		PoolVector3Array handle_points;
	};

	//you can use MeshUtils.merge_mesh_array(arr) to get optimalized handle points. Just get the vertices from it.
	static Vector<PoolIntArray> get_handle_vertex_to_vertex_map(const Array &arrays, const PoolVector3Array &handle_points);

	//returns an array:
	//index 0 is the handle_points
	//index 1 is the handle_to_vertex_map
	static HandleVertexMapResult get_handle_edge_to_vertex_map(const Array &arrays);

	//returns an array:
	//index 0 is the handle_points
	//index 1 is the handle_to_vertex_map
	static HandleVertexMapResult get_handle_face_to_vertex_map(const Array &arrays);
	static PoolVector3Array calculate_map_midpoints(Array mesh, Array vertex_map);
	static bool pool_int_arr_contains(const PoolIntArray &arr, const int val);
	static Vector<PoolIntArray> partition_mesh(Ref<MeshDataResource> mdr);
};

#endif
