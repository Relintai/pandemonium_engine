#ifndef MESH_UTILS_H
#define MESH_UTILS_H

/*************************************************************************/
/*  mesh_utils.h                                                         */
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

#include "core/variant/variant.h"

#include "core/object/object.h"

#include "scene/resources/texture.h"

#include "defines.h"

class MeshUtils : public Object {
	GDCLASS(MeshUtils, Object);

public:
	static MeshUtils *get_singleton();

	Array merge_mesh_array(Array arr) const;
	Array bake_mesh_array_uv(Array arr, Ref<Texture> tex, float mul_color = 0.7) const;
	//If normals are present they need to match too to be removed
	Array remove_doubles(Array arr) const;
	//Normals are always interpolated, merged
	Array remove_doubles_interpolate_normals(Array arr) const;

	//Only unwraps, does not create new seams
	PoolVector2Array uv_unwrap(Array arr, bool p_block_align = true, float p_texel_size = 0.05, int p_padding = 1, int p_max_chart_size = 4094) const;

	PoolIntArray delaunay3d_tetrahedralize(const Vector<Vector3> &p_points);

	MeshUtils();
	~MeshUtils();

protected:
	static void _bind_methods();

private:
	static MeshUtils *_instance;
};

#endif
