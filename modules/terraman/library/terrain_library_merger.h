#ifndef TERRAIN_LIBRARY_MERGER_H
#define TERRAIN_LIBRARY_MERGER_H


#include "core/containers/rb_map.h"
#include "core/object/resource.h"

#include "terrain_library.h"

#include "scene/resources/material/material.h"

#include "../data/terrain_light.h"
#include "terrain_surface_merger.h"

#include "modules/modules_enabled.gen.h"

class TerrainSurfaceSimple;
class TerrainMesher;
class PackedScene;

class TerrainLibraryMerger : public TerrainLibrary {
	GDCLASS(TerrainLibraryMerger, TerrainLibrary)

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

	Ref<TerrainSurface> terra_surface_get(const int index);
	void terra_surface_add(Ref<TerrainSurface> value);
	void terra_surface_set(const int index, Ref<TerrainSurface> value);
	void terra_surface_remove(const int index);
	int terra_surface_get_num() const;
	void terra_surfaces_clear();

	Vector<Variant> get_terra_surfaces();
	void set_terra_surfaces(const Vector<Variant> &surfaces);

#ifdef MODULE_PROPS_ENABLED
	Ref<PropData> get_prop(const int index);
	void add_prop(Ref<PropData> value);
	bool has_prop(const Ref<PropData> &value) const;
	void set_prop(const int index, const Ref<PropData> &value);
	void remove_prop(const int index);
	int get_num_props() const;
	void clear_props();

	Vector<Variant> get_props();
	void set_props(const Vector<Variant> &props);

	Rect2 get_prop_uv_rect(const Ref<Texture> &texture);

	Ref<TexturePacker> get_prop_packer();
#endif

	void refresh_rects();

	void _setup_material_albedo(const int material_index, const Ref<Texture> &texture);

	TerrainLibraryMerger();
	~TerrainLibraryMerger();

protected:
#ifdef MODULE_PROPS_ENABLED
	bool process_prop_textures(Ref<PropData> prop);
#endif

	static void _bind_methods();

	Vector<Ref<TerrainSurfaceMerger>> _terra_surfaces;
#ifdef MODULE_PROPS_ENABLED
	Vector<Ref<PropData>> _props;
#endif

	Ref<TexturePacker> _packer;
	Ref<TexturePacker> _prop_packer;
};

#endif
