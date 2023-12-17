#ifndef NAVIGATION_MESH_SOURCE_GEOMETRY_DATA_3D_H
#define NAVIGATION_MESH_SOURCE_GEOMETRY_DATA_3D_H

/*************************************************************************/
/*  navigation_mesh_source_geometry_data_3d.h                            */
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

#include "core/object/resource.h"

#include "core/math/transform.h"

#include "scene/resources/mesh/mesh.h"

class NavigationMeshSourceGeometryData3D : public Resource {
	GDCLASS(NavigationMeshSourceGeometryData3D, Resource);

	PoolRealArray vertices;
	PoolIntArray indices;

protected:
	static void _bind_methods();

private:
	void _add_vertex(const Vector3 &p_vec3);
	void _add_mesh(const Ref<Mesh> &p_mesh, const Transform &p_xform);
	void _add_mesh_array(const Array &p_array, const Transform &p_xform);
	void _add_faces(const PoolVector3Array &p_faces, const Transform &p_xform);

public:
	// kept root node transform here on the geometry data
	// if we add this transform to all exposed functions we need to break comp on all functions later
	// when navigation_mesh changes from global transfrom to relative to navregion
	// but if it stays here we can just remove it and change the internal functions only
	Transform root_node_transform;

	void set_vertices(const PoolRealArray &p_vertices);
	PoolRealArray get_vertices() const { return vertices; }

	void set_indices(const PoolIntArray &p_indices);
	PoolIntArray get_indices() const { return indices; }

	bool has_data() { return vertices.size() && indices.size(); };
	void clear();

	void add_mesh(const Ref<Mesh> &p_mesh, const Transform &p_xform);
	void add_mesh_array(const Array &p_mesh_array, const Transform &p_xform);
	void add_faces(const PoolVector3Array &p_faces, const Transform &p_xform);

	NavigationMeshSourceGeometryData3D();
	~NavigationMeshSourceGeometryData3D();
};

#endif // NAVIGATION_MESH_SOURCE_GEOMETRY_DATA_3D_H
