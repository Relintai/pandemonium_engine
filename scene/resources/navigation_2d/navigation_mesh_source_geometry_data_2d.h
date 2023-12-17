#ifndef NAVIGATION_MESH_SOURCE_GEOMETRY_DATA_2D_H
#define NAVIGATION_MESH_SOURCE_GEOMETRY_DATA_2D_H

/*************************************************************************/
/*  navigation_mesh_source_geometry_data_2d.h                            */
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

#include "scene/main/node_2d.h"
#include "scene/resources/navigation_2d/navigation_polygon.h"

class NavigationMeshSourceGeometryData2D : public Resource {
	GDCLASS(NavigationMeshSourceGeometryData2D, Resource);

	Vector<Vector<Vector2>> traversable_outlines;
	Vector<Vector<Vector2>> obstruction_outlines;

protected:
	static void _bind_methods();

public:
	void _set_traversable_outlines(const Vector<Vector<Vector2>> &p_traversable_outlines);
	const Vector<Vector<Vector2>> &_get_traversable_outlines() const { return traversable_outlines; }

	void _set_obstruction_outlines(const Vector<Vector<Vector2>> &p_obstruction_outlines);
	const Vector<Vector<Vector2>> &_get_obstruction_outlines() const { return obstruction_outlines; }

	void _add_traversable_outline(const Vector<Vector2> &p_shape_outline);
	void _add_obstruction_outline(const Vector<Vector2> &p_shape_outline);

	// kept root node transform here on the geometry data
	// if we add this transform to all exposed functions we need to break comp on all functions later
	// when navigation_mesh changes from global transfrom to relative to navregion
	// but if it stays here we can just remove it and change the internal functions only
	Transform2D root_node_transform;

	void set_traversable_outlines(const Array &p_traversable_outlines);
	Array get_traversable_outlines() const;

	void set_obstruction_outlines(const Array &p_obstruction_outlines);
	Array get_obstruction_outlines() const;

	void add_traversable_outline(const PoolVector2Array &p_shape_outline);
	void add_obstruction_outline(const PoolVector2Array &p_shape_outline);

	bool has_data() { return traversable_outlines.size(); };
	void clear();

	NavigationMeshSourceGeometryData2D();
	~NavigationMeshSourceGeometryData2D();
};

#endif // NAVIGATION_MESH_SOURCE_GEOMETRY_DATA_2D_H
