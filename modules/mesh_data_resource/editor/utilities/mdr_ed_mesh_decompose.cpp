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

#include "mdr_ed_mesh_decompose.h"

//you can use MeshUtils.merge_mesh_array(arr) to get optimalized handle points. Just get the vertices from it.
Array MDREDMeshDecompose::get_handle_vertex_to_vertex_map(Array arrays, PoolVector3Array handle_points) {
	/*
	var handle_to_vertex_map : Array
	handle_to_vertex_map.resize(handle_points.size())

	if handle_points.size() == 0:
		return handle_to_vertex_map

	if arrays.size() != ArrayMesh.ARRAY_MAX || arrays[ArrayMesh.ARRAY_INDEX] == null:
		return handle_to_vertex_map

	var vertices : PoolVector3Array = arrays[ArrayMesh.ARRAY_VERTEX]

	if vertices.size() == 0:
		return handle_to_vertex_map

	for i in range(handle_points.size()):
		var hv : Vector3 = handle_points[i]
		var iarr : PoolIntArray = PoolIntArray()

		#find all verts that have the same position as the handle
		for j in range(vertices.size()):
			var vn : Vector3 = vertices[j]

			if is_equal_approx(hv.x, vn.x) && is_equal_approx(hv.y, vn.y) && is_equal_approx(hv.z, vn.z):
				iarr.append(j)

		handle_to_vertex_map[i] = iarr

	return handle_to_vertex_map

	*/
}

//returns an array:
//index 0 is the handle_points
//index 1 is the handle_to_vertex_map
Array MDREDMeshDecompose::get_handle_edge_to_vertex_map(Array arrays) {
	/*

	*/
}

//returns an array:
//index 0 is the handle_points
//index 1 is the handle_to_vertex_map
Array MDREDMeshDecompose::get_handle_face_to_vertex_map(Array arrays) {
	/*
	var handle_to_vertex_map : Array
	var handle_points : PoolVector3Array

	if arrays.size() != ArrayMesh.ARRAY_MAX || arrays[ArrayMesh.ARRAY_INDEX] == null:
		return [ handle_points, handle_to_vertex_map ]

	var vertices : PoolVector3Array = arrays[ArrayMesh.ARRAY_VERTEX]

	if vertices.size() == 0:
		return [ handle_points, handle_to_vertex_map ]

	var arr : Array = Array()
	arr.resize(ArrayMesh.ARRAY_MAX)
	arr[ArrayMesh.ARRAY_VERTEX] = arrays[ArrayMesh.ARRAY_VERTEX]
	arr[ArrayMesh.ARRAY_INDEX] = arrays[ArrayMesh.ARRAY_INDEX]

	var optimized_arrays : Array = MeshUtils.merge_mesh_array(arr)
	var optimized_verts : PoolVector3Array = optimized_arrays[ArrayMesh.ARRAY_VERTEX]
	var optimized_indices : PoolIntArray = optimized_arrays[ArrayMesh.ARRAY_INDEX]

	var vert_to_optimized_vert_map : Array = get_handle_vertex_to_vertex_map(arrays, optimized_verts)

	for i in range(0, optimized_indices.size(), 3):
		var i0 : int = optimized_indices[i + 0]
		var i1 : int = optimized_indices[i + 1]
		var i2 : int = optimized_indices[i + 2]

		var v0 : Vector3 = optimized_verts[i0]
		var v1 : Vector3 = optimized_verts[i1]
		var v2 : Vector3 = optimized_verts[i2]

		var pmid : Vector3 = v0 + v1 + v2
		pmid /= 3
		handle_points.append(pmid)

		var vm0 : PoolIntArray = vert_to_optimized_vert_map[i0]
		var vm1 : PoolIntArray = vert_to_optimized_vert_map[i1]
		var vm2 : PoolIntArray = vert_to_optimized_vert_map[i2]

		var vm : PoolIntArray = PoolIntArray()
		vm.append_array(vm0)

		for vi in vm1:
			var found : bool = false
			for vit in vm:
				if vi == vit:
					found = true
					break

			if !found:
				vm.append(vi)

		for vi in vm2:
			var found : bool = false
			for vit in vm:
				if vi == vit:
					found = true
					break

			if !found:
				vm.append(vi)

		handle_to_vertex_map.append(vm)

	return [ handle_points, handle_to_vertex_map ]
	*/
}
PoolVector3Array MDREDMeshDecompose::calculate_map_midpoints(Array mesh, Array vertex_map) {
	/*
	return PoolVector3Array()
	*/
}
bool MDREDMeshDecompose::pool_int_arr_contains(PoolIntArray arr, int val) {
	/*
	for a in arr:
		if a == val:
			return true

	return false
	*/
}
Array MDREDMeshDecompose::partition_mesh(Ref<MeshDataResource> mdr) {
	/*
	var partitions : Array = Array()

	var arrays : Array = mdr.get_array()

	if arrays.size() != ArrayMesh.ARRAY_MAX:
		return partitions

	if arrays[ArrayMesh.ARRAY_INDEX] == null:
		return partitions

	var indices : PoolIntArray = arrays[ArrayMesh.ARRAY_INDEX]

	var triangle_count : int = indices.size() / 3
	var processed_triangles : PoolIntArray = PoolIntArray()

	while triangle_count != processed_triangles.size():
		var partition : PoolIntArray = PoolIntArray()

		var first : bool = true
		var triangle_added : bool = true
		while triangle_added:
			triangle_added = false
			for i in range(indices.size()):
				var triangle_index : int = i / 3

				if pool_int_arr_contains(processed_triangles, triangle_index):
					continue

				if first:
					first = false

					# We have to be at the 0th index of a triangle
					partition.append(indices[i])
					partition.append(indices[i + 1])
					partition.append(indices[i + 2])

					triangle_added = true
					break

				var index : int = indices[i]

				if pool_int_arr_contains(partition, index):
					processed_triangles.append(triangle_index)

					var tri_start_index : int = i - (i % 3)

					var i0 : int = indices[tri_start_index]
					var i1 : int = indices[tri_start_index + 1]
					var i2 : int = indices[tri_start_index + 2]

					partition.append(i0)
					partition.append(i1)
					partition.append(i2)

					triangle_added = true
					break


		partitions.append(partition)


	return partitions
	*/
}
