#ifndef GROUND_CLUTTER_2D_H
#define GROUND_CLUTTER_2D_H


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
