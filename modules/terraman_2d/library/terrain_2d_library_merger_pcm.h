#ifndef TERRAIN_2D_LIBRARY_MERGER_PCM_H
#define TERRAIN_2D_LIBRARY_MERGER_PCM_H
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

#include "core/containers/map.h"
#include "core/resource.h"

#include "terrain_2d_library.h"

#include "scene/resources/material.h"

#include "../data/terrain_2d_light.h"
#include "terrain_2d_surface_merger.h"

#include "core/os/mutex.h"

class Terrain2DSurfaceSimple;
class Terrain2DMesher;
class PackedScene;
class Terrain2DMaterialCachePCM;
class TexturePacker;

//pcm = per chunk material
class Terrain2DLibraryMergerPCM : public Terrain2DLibrary {
	GDCLASS(Terrain2DLibraryMergerPCM, Terrain2DLibrary);

public:
	bool _supports_caching();

	void _material_cache_get_key(Ref<Terrain2DChunk> chunk);
	Ref<Terrain2DMaterialCache> _material_cache_get(const int key);
	void _material_cache_unref(const int key);

	void _prop_material_cache_get_key(Ref<Terrain2DChunk> chunk);
	Ref<Terrain2DMaterialCache> _prop_material_cache_get(const int key);
	void _prop_material_cache_unref(const int key);

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

	Ref<Terrain2DSurface> terra_surface_get(const int index);
	void terra_surface_add(Ref<Terrain2DSurface> value);
	void terra_surface_set(const int index, Ref<Terrain2DSurface> value);
	void terra_surface_remove(const int index);
	int terra_surface_get_num() const;
	void terra_surfaces_clear();

	Vector<Variant> get_terra_surfaces();
	void set_terra_surfaces(const Vector<Variant> &surfaces);

#ifdef PROPS_2D_PRESENT
	Ref<Prop2DData> get_prop(const int index);
	void add_prop(Ref<Prop2DData> value);
	bool has_prop(const Ref<Prop2DData> &value) const;
	void set_prop(const int index, const Ref<Prop2DData> &value);
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

	Terrain2DLibraryMergerPCM();
	~Terrain2DLibraryMergerPCM();

protected:
#ifdef PROPS_2D_PRESENT
	bool process_prop_textures(Ref<Prop2DData> prop);
#endif

	static void _bind_methods();

	Map<int, Ref<Terrain2DMaterialCachePCM>> _material_cache;
	Map<int, Ref<Terrain2DMaterialCachePCM>> _prop_material_cache;

	Vector<Ref<Terrain2DSurfaceMerger>> _terra_surfaces;
#ifdef PROPS_2D_PRESENT
	Vector<Ref<Prop2DData>> _props;
#endif

	//todo remove these
	Ref<TexturePacker> _packer;
	Ref<TexturePacker> _prop_packer;

	Mutex _material_cache_mutex;
	Mutex _prop_material_cache_mutex;
};

#endif
