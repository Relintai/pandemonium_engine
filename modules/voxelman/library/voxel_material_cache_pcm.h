#ifndef VOXEL_MATERIAL_CACHE_PCM_H
#define VOXEL_MATERIAL_CACHE_PCM_H


#include "voxel_material_cache.h"

#include "core/math/color.h"
#include "core/object/resource.h"
#include "core/containers/vector.h"

#include "core/math/rect2.h"
#include "scene/resources/material/material.h"

#include "../defines.h"

class VoxelSurface;
class TexturePacker;
class PropData;

class VoxelMaterialCachePCM : public VoxelMaterialCache {
	GDCLASS(VoxelMaterialCachePCM, VoxelMaterialCache);

public:
	int get_texture_flags() const;
	void set_texture_flags(const int flags);

	int get_max_atlas_size() const;
	void set_max_atlas_size(const int size);

	bool get_keep_original_atlases() const;
	void set_keep_original_atlases(const bool value);

	Color get_background_color() const;
	void set_background_color(const Color &color);

	int get_margin() const;
	void set_margin(const int margin);

	Ref<AtlasTexture> additional_texture_get_atlas_tex(const Ref<Texture> &texture);
	Rect2 additional_texture_get_uv_rect(const Ref<Texture> &texture);

	void refresh_rects();

	void _setup_material_albedo(Ref<Texture> texture);

	VoxelMaterialCachePCM();
	~VoxelMaterialCachePCM();

protected:
	static void _bind_methods();

	Ref<TexturePacker> _packer;
};

#endif
