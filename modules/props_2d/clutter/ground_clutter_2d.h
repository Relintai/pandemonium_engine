#ifndef GROUND_CLUTTER_2D_H
#define GROUND_CLUTTER_2D_H
/*
Copyright (c) 2019-2023 Péter Magyar

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

#include "core/object/resource.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_packer.h"
#endif

#ifdef MODULE_VOXELMAN_ENABLED
#include "../../voxelman/meshers/voxel_mesher.h"
#endif

class VoxelChunk;
class VoxelMesher;

class GroundClutter2D : public Resource {
	GDCLASS(GroundClutter2D, Resource);

public:
#ifdef MODULE_VOXELMAN_ENABLED
	bool should_spawn(Ref<VoxelChunk> chunk, int x, int y, int z);

	void add_meshes_to(Ref<VoxelMesher> mesher, Ref<VoxelChunk> chunk, int x, int y, int z);
#endif

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	void add_textures_to(Ref<TexturePacker> packer);
#endif

	GroundClutter2D();
	~GroundClutter2D();

private:
	static void _bind_methods();

private:
};

#endif
