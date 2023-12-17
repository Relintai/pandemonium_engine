#ifndef MDR_ED_MESH_DECOMPOSE_H
#define MDR_ED_MESH_DECOMPOSE_H

/*************************************************************************/
/*  mdr_ed_mesh_decompose.h                                              */
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

#include "core/containers/pool_vector.h"
#include "core/containers/vector.h"
#include "core/object/reference.h"
#include "core/variant/array.h"
#include "core/variant/variant.h"

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
