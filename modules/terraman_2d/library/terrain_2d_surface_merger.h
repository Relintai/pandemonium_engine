#ifndef TERRAIN_2D_SURFACE_MERGER_H
#define TERRAIN_2D_SURFACE_MERGER_H


#include "terrain_2d_surface.h"

#include "scene/resources/texture.h"

#include "../../texture_packer/texture_packer.h"

#include "../defines.h"

class Terrain2DSurfaceMerger : public Terrain2DSurface {
	GDCLASS(Terrain2DSurfaceMerger, Terrain2DSurface)

public:
	Ref<AtlasTexture> get_region();
	void set_region(const Ref<AtlasTexture> &texture);

	Ref<Texture> get_texture();
	void set_texture(const Ref<Texture> &texture);

	void refresh_rects();

	Terrain2DSurfaceMerger();
	~Terrain2DSurfaceMerger();

protected:
	static void _bind_methods();

private:
	Ref<AtlasTexture> _region;
	Ref<Texture> _texture;
};

#endif
