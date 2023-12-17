#ifndef TERRAIN_SURFACE_MERGER_H
#define TERRAIN_SURFACE_MERGER_H


#include "terrain_surface.h"

#include "scene/resources/texture.h"

#include "../../texture_packer/texture_packer.h"

class TerrainSurfaceMerger : public TerrainSurface {
	GDCLASS(TerrainSurfaceMerger, TerrainSurface)

public:
	Ref<AtlasTexture> get_region(const TerrainSurfaceSides side);
	void set_region(const TerrainSurfaceSides side, const Ref<AtlasTexture> &texture);

	Ref<Texture> get_texture(const TerrainSurfaceSides side);
	void set_texture(const TerrainSurfaceSides side, const Ref<Texture> &texture);

	void refresh_rects();

	TerrainSurfaceMerger();
	~TerrainSurfaceMerger();

protected:
	static void _bind_methods();

private:
	Ref<AtlasTexture> _regions[TERRAIN_SIDES_COUNT];
	Ref<Texture> _textures[TERRAIN_SIDES_COUNT];
};

#endif
