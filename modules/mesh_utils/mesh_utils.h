#ifndef MESH_UTILS_H
#define MESH_UTILS_H
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




#include "core/variant.h"
#include "core/version.h"

#if VERSION_MAJOR > 3
#include "core/object/object.h"
#else
#include "core/object.h"
#endif

#include "scene/resources/texture.h"

#include "defines.h"

#if GODOT4
#define Texture Texture2D
#endif

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

#if GODOT4
#undef Texture
#endif

#endif
