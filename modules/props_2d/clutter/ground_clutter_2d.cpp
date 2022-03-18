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

#include "ground_clutter_2d.h"

#ifdef VOXELMAN_PRESENT
#include "../../voxelman/world/voxel_chunk.h"

bool GroundClutter2D::should_spawn(Ref<VoxelChunk> chunk, int x, int y, int z) {
	if (has_method("_should_spawn"))
		return call("_should_spawn", chunk, x, y, z);

	return false;
}

void GroundClutter2D::add_meshes_to(Ref<VoxelMesher> mesher, Ref<VoxelChunk> chunk, int x, int y, int z) {
	if (has_method("_add_meshes_to"))
		call("_add_meshes_to", mesher, chunk, x, y, z);
}
#endif

#ifdef TEXTURE_PACKER_PRESENT
void GroundClutter2D::add_textures_to(Ref<TexturePacker> packer) {
	if (has_method("_add_textures_to"))
		call("_add_textures_to", packer);
}
#endif

GroundClutter2D::GroundClutter2D() {
}

GroundClutter2D::~GroundClutter2D() {
}

void GroundClutter2D::_bind_methods() {
#ifdef TEXTURE_PACKER_PRESENT
	BIND_VMETHOD(MethodInfo("_add_textures_to", PropertyInfo(Variant::OBJECT, "packer", PROPERTY_HINT_RESOURCE_TYPE, "TexturePacker")));

	ClassDB::bind_method(D_METHOD("add_textures_to", "packer"), &GroundClutter2D::add_textures_to);
#endif

#ifdef VOXELMAN_PRESENT
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "should"), "_should_spawn", PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk"), PropertyInfo(Variant::INT, "x"), PropertyInfo(Variant::INT, "y"), PropertyInfo(Variant::INT, "z")));
	BIND_VMETHOD(MethodInfo("_add_meshes_to", PropertyInfo(Variant::OBJECT, "mesher", PROPERTY_HINT_RESOURCE_TYPE, "VoxelMesher"), PropertyInfo(Variant::OBJECT, "chunk", PROPERTY_HINT_RESOURCE_TYPE, "VoxelChunk"), PropertyInfo(Variant::INT, "x"), PropertyInfo(Variant::INT, "y"), PropertyInfo(Variant::INT, "z")));

	ClassDB::bind_method(D_METHOD("should_spawn", "chunk", "x", "y", "z"), &GroundClutter2D::should_spawn);
	ClassDB::bind_method(D_METHOD("add_meshes_to", "mesher", "chunk", "x", "y", "z"), &GroundClutter2D::add_meshes_to);
#endif
}
