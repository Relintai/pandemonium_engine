/*************************************************************************/
/*  mdr_ed_mesh_decompose.cpp                                            */
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

#include "mdr_ed_mesh_decompose.h"

#include "../../mesh_data_resource.h"
#include "modules/mesh_utils/mesh_utils.h"
#include "scene/resources/mesh/mesh.h"

//you can use MeshUtils.merge_mesh_array(arr) to get optimalized handle points. Just get the vertices from it.
Vector<PoolIntArray> MDREDMeshDecompose::get_handle_vertex_to_vertex_map(const Array &arrays, const PoolVector3Array &handle_points) {
	Vector<PoolIntArray> handle_to_vertex_map;
	handle_to_vertex_map.resize(handle_points.size());

	if (handle_points.size() == 0) {
		return handle_to_vertex_map;
	}

	if (arrays.size() != ArrayMesh::ARRAY_MAX || arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return handle_to_vertex_map;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];

	if (vertices.size() == 0) {
		return handle_to_vertex_map;
	}

	for (int i = 0; i < handle_points.size(); ++i) {
		Vector3 hv = handle_points[i];
		PoolIntArray iarr;

		//find all verts that have the same position as the handle
		for (int j = 0; j < vertices.size(); ++j) {
			Vector3 vn = vertices[j];

			if (Math::is_equal_approx(hv.x, vn.x) && Math::is_equal_approx(hv.y, vn.y) && Math::is_equal_approx(hv.z, vn.z)) {
				iarr.append(j);
			}
		}

		handle_to_vertex_map.write[i] = iarr;
	}

	return handle_to_vertex_map;
}

//returns an array:
//index 0 is the handle_points
//index 1 is the handle_to_vertex_map
MDREDMeshDecompose::HandleVertexMapResult MDREDMeshDecompose::get_handle_edge_to_vertex_map(const Array &arrays) {
	HandleVertexMapResult ret;

	if (arrays.size() != ArrayMesh::ARRAY_MAX || arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return ret;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];

	if (vertices.size() == 0) {
		return ret;
	}

	Array arr;
	arr.resize(ArrayMesh::ARRAY_MAX);
	arr[ArrayMesh::ARRAY_VERTEX] = arrays[ArrayMesh::ARRAY_VERTEX];
	arr[ArrayMesh::ARRAY_INDEX] = arrays[ArrayMesh::ARRAY_INDEX];

	Array optimized_arrays = MeshUtils::get_singleton()->merge_mesh_array(arr);
	PoolVector3Array optimized_verts = optimized_arrays[ArrayMesh::ARRAY_VERTEX];
	PoolIntArray optimized_indices = optimized_arrays[ArrayMesh::ARRAY_INDEX];

	Vector<PoolIntArray> vert_to_optimized_vert_map = get_handle_vertex_to_vertex_map(arrays, optimized_verts);

	Dictionary edge_map;

	for (int i = 0; i < optimized_indices.size(); i += 3) {
		for (int j = 0; j < 3; ++j) {
			int i0 = optimized_indices[i + j];
			int i1 = optimized_indices[i + ((j + 1) % 3)];

			int ei0 = MIN(i0, i1);
			int ei1 = MAX(i0, i1);

			if (!edge_map.has(ei0)) {
				edge_map[ei0] = PoolIntArray();
			}

			PoolIntArray etm = edge_map[ei0];

			PoolIntArray::Read r = etm.read();

			bool found = false;
			for (int ii = 0; ii < etm.size(); ++ii) {
				if (r[ii] == ei1) {
					found = true;
					break;
				}
			}

			r.release();

			if (!found) {
				etm.append(ei1);
				edge_map[ei0] = etm;
			}
		}
	}

	Array keys = edge_map.keys();

	for (int k = 0; k < keys.size(); ++k) {
		int key = keys[k];
		PoolIntArray indices = edge_map[key];

		PoolIntArray::Read r = indices.read();

		for (int i = 0; i < indices.size(); ++i) {
			int indx = r[i];
			int ei0 = key;
			int ei1 = indx;

			Vector3 v0 = optimized_verts[ei0];
			Vector3 v1 = optimized_verts[ei1];

			Vector3 emid = v0.linear_interpolate(v1, 0.5);
			ret.handle_points.append(emid);
			//int hindx = handle_points.size() - 1;

			PoolIntArray vm0 = vert_to_optimized_vert_map[ei0];
			PoolIntArray vm1 = vert_to_optimized_vert_map[ei1];

			PoolIntArray vm;
			vm.append_array(vm0);

			PoolIntArray::Read vm1r = vm1.read();

			for (int v = 0; v < vm1.size(); ++v) {
				int vi = vm1r[v];
				bool found = false;

				PoolIntArray::Read vmr = vm.read();

				for (int vrind = 0; vrind < vm.size(); ++vrind) {
					int vit = vmr[vrind];
					if (vi == vit) {
						found = true;
						break;
					}
				}

				vmr.release();

				if (!found) {
					vm.append(vi);
				}
			}

			vm1r.release();

			ret.handle_to_vertex_map.push_back(vm);
		}

		r.release();
	}

	return ret;
}

//returns an array:
//index 0 is the handle_points
//index 1 is the handle_to_vertex_map
MDREDMeshDecompose::HandleVertexMapResult MDREDMeshDecompose::get_handle_face_to_vertex_map(const Array &arrays) {
	HandleVertexMapResult ret;

	if (arrays.size() != ArrayMesh::ARRAY_MAX || arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return ret;
	}

	PoolVector3Array vertices = arrays[ArrayMesh::ARRAY_VERTEX];

	if (vertices.size() == 0) {
		return ret;
	}

	Array arr;
	arr.resize(ArrayMesh::ARRAY_MAX);
	arr[ArrayMesh::ARRAY_VERTEX] = arrays[ArrayMesh::ARRAY_VERTEX];
	arr[ArrayMesh::ARRAY_INDEX] = arrays[ArrayMesh::ARRAY_INDEX];

	Array optimized_arrays = MeshUtils::get_singleton()->merge_mesh_array(arr);
	PoolVector3Array optimized_verts = optimized_arrays[ArrayMesh::ARRAY_VERTEX];
	PoolIntArray optimized_indices = optimized_arrays[ArrayMesh::ARRAY_INDEX];

	Vector<PoolIntArray> vert_to_optimized_vert_map = get_handle_vertex_to_vertex_map(arrays, optimized_verts);

	for (int i = 0; i < optimized_indices.size(); i += 3) {
		int i0 = optimized_indices[i + 0];
		int i1 = optimized_indices[i + 1];
		int i2 = optimized_indices[i + 2];

		Vector3 v0 = optimized_verts[i0];
		Vector3 v1 = optimized_verts[i1];
		Vector3 v2 = optimized_verts[i2];

		Vector3 pmid = v0 + v1 + v2;
		pmid /= 3;
		ret.handle_points.append(pmid);

		PoolIntArray vm0 = vert_to_optimized_vert_map[i0];
		PoolIntArray vm1 = vert_to_optimized_vert_map[i1];
		PoolIntArray vm2 = vert_to_optimized_vert_map[i2];

		PoolIntArray vm = PoolIntArray();
		vm.append_array(vm0);

		PoolIntArray::Read vm1r = vm1.read();

		for (int v = 0; v < vm1.size(); ++v) {
			int vi = vm1r[v];
			bool found = false;

			PoolIntArray::Read vmr = vm.read();

			for (int vrind = 0; vrind < vm.size(); ++vrind) {
				int vit = vmr[vrind];
				if (vi == vit) {
					found = true;
					break;
				}
			}

			vmr.release();

			if (!found) {
				vm.append(vi);
			}
		}

		vm1r.release();

		PoolIntArray::Read vm2r = vm2.read();

		for (int v = 0; v < vm2.size(); ++v) {
			int vi = vm2r[v];
			bool found = false;

			PoolIntArray::Read vmr = vm.read();

			for (int vrind = 0; vrind < vm.size(); ++vrind) {
				int vit = vmr[vrind];
				if (vi == vit) {
					found = true;
					break;
				}
			}

			vmr.release();

			if (!found) {
				vm.append(vi);
			}
		}

		vm2r.release();

		ret.handle_to_vertex_map.push_back(vm);
	}

	return ret;
}

PoolVector3Array MDREDMeshDecompose::calculate_map_midpoints(Array mesh, Array vertex_map) {
	return PoolVector3Array();
}

bool MDREDMeshDecompose::pool_int_arr_contains(const PoolIntArray &arr, const int val) {
	PoolIntArray::Read r = arr.read();

	for (int i = 0; i < arr.size(); ++i) {
		if (r[i] == val) {
			return true;
		}
	}

	return false;
}

Vector<PoolIntArray> MDREDMeshDecompose::partition_mesh(Ref<MeshDataResource> mdr) {
	Vector<PoolIntArray> partitions;

	Array arrays = mdr->get_array();

	if (arrays.size() != ArrayMesh::ARRAY_MAX) {
		return partitions;
	}

	if (arrays[ArrayMesh::ARRAY_INDEX].is_null()) {
		return partitions;
	}

	PoolIntArray indices = arrays[ArrayMesh::ARRAY_INDEX];

	int triangle_count = indices.size() / 3;
	PoolIntArray processed_triangles = PoolIntArray();

	while (triangle_count != processed_triangles.size()) {
		PoolIntArray partition = PoolIntArray();

		bool first = true;
		bool triangle_added = true;
		while (triangle_added) {
			triangle_added = false;
			for (int i = 0; i < indices.size(); ++i) {
				int triangle_index = i / 3;

				if (pool_int_arr_contains(processed_triangles, triangle_index)) {
					continue;
				}

				if (first) {
					first = false;

					// We have to be at the 0th index of a triangle
					partition.append(indices[i]);
					partition.append(indices[i + 1]);
					partition.append(indices[i + 2]);

					triangle_added = true;
					break;
				}

				int index = indices[i];

				if (pool_int_arr_contains(partition, index)) {
					processed_triangles.append(triangle_index);

					int tri_start_index = i - (i % 3);

					int i0 = indices[tri_start_index];
					int i1 = indices[tri_start_index + 1];
					int i2 = indices[tri_start_index + 2];

					partition.append(i0);
					partition.append(i1);
					partition.append(i2);

					triangle_added = true;
					break;
				}
			}
		}

		partitions.push_back(partition);
	}

	return partitions;
}
