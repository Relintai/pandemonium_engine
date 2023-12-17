#ifndef TERRAIN_2D_LIBRARY_H
#define TERRAIN_2D_LIBRARY_H

/*************************************************************************/
/*  terrain_2d_library.h                                                 */
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

#include "core/object/resource.h"

#include "core/math/rect2.h"
#include "scene/resources/material/material.h"

#include "../data/terrain_2d_light.h"
#include "terrain_2d_surface.h"

#include "../defines.h"

#include "modules/modules_enabled.gen.h"

class Terrain2DMaterialCache;
class Terrain2DSurface;
class Terrain2DMesher;
class PackedScene;
class Terrain2DChunk;
#ifdef MODULE_PROPS_2D_ENABLED
class Prop2DData;
#endif

class Terrain2DLibrary : public Resource {
	GDCLASS(Terrain2DLibrary, Resource)

public:
	enum {
		MATERIAL_INDEX_TERRAIN = 0,
		MATERIAL_INDEX_LIQUID = 1,
		MATERIAL_INDEX_PROP = 2,
	};

public:
	bool get_initialized() const;
	void set_initialized(const bool value);

	bool supports_caching();
	virtual bool _supports_caching();

	Ref<Texture> texture_get();
	void texture_set(const Ref<Texture> &value);

	Ref<Texture> liquid_texture_get();
	void liquid_texture_set(const Ref<Texture> &value);

	Ref<Texture> prop_texture_get();
	void prop_texture_set(const Ref<Texture> &value);

	void material_cache_get_key(Ref<Terrain2DChunk> chunk);
	virtual void _material_cache_get_key(Ref<Terrain2DChunk> chunk);
	Ref<Terrain2DMaterialCache> material_cache_get(const int key);
	virtual Ref<Terrain2DMaterialCache> _material_cache_get(const int key);
	void material_cache_unref(const int key);
	virtual void _material_cache_unref(const int key);

	Ref<Material> material_get();
	void material_set(const Ref<Material> &value);

	void liquid_material_cache_get_key(Ref<Terrain2DChunk> chunk);
	virtual void _liquid_material_cache_get_key(Ref<Terrain2DChunk> chunk);
	Ref<Terrain2DMaterialCache> liquid_material_cache_get(const int key);
	virtual Ref<Terrain2DMaterialCache> _liquid_material_cache_get(const int key);
	void liquid_material_cache_unref(const int key);
	virtual void _liquid_material_cache_unref(const int key);

	Ref<Material> liquid_material_get();
	void liquid_material_set(const Ref<Material> &value);

	void prop_material_cache_get_key(Ref<Terrain2DChunk> chunk);
	virtual void _prop_material_cache_get_key(Ref<Terrain2DChunk> chunk);
	Ref<Terrain2DMaterialCache> prop_material_cache_get(const int key);
	virtual Ref<Terrain2DMaterialCache> _prop_material_cache_get(const int key);
	void prop_material_cache_unref(const int key);
	virtual void _prop_material_cache_unref(const int key);

	Ref<Material> prop_material_get();
	void prop_material_set(const Ref<Material> &value);

	virtual Ref<Terrain2DSurface> terra_surface_get(const int index);
	virtual void terra_surface_add(Ref<Terrain2DSurface> value);
	virtual void terra_surface_set(const int index, Ref<Terrain2DSurface> value);
	virtual void terra_surface_remove(const int index);
	virtual int terra_surface_get_num() const;
	virtual void terra_surfaces_clear();

	virtual Ref<PackedScene> scene_get(const int id);
	virtual void scene_add(Ref<PackedScene> value);
	virtual void scene_set(const int id, Ref<PackedScene> value);
	virtual void scene_remove(const int id);
	virtual int scene_get_num() const;
	virtual void scenes_clear();

#ifdef MODULE_PROPS_2D_ENABLED
	virtual Ref<Prop2DData> prop_get(const int id);
	virtual void prop_add(Ref<Prop2DData> value);
	virtual bool prop_has(const Ref<Prop2DData> &value) const;
	virtual void prop_set(const int id, Ref<Prop2DData> value);
	virtual void prop_remove(const int id);
	virtual int prop_get_num() const;
	virtual void props_clear();

	virtual Rect2 get_prop_uv_rect(const Ref<Texture> &texture);
#endif

	virtual void refresh_rects();

	void setup_material_albedo(int material_index, Ref<Texture> texture);

	Terrain2DLibrary();
	~Terrain2DLibrary();

protected:
	static void _bind_methods();

	bool _initialized;

	Ref<Texture> _texture;
	Ref<Texture> _liquid_texture;
	Ref<Texture> _prop_texture;

	Ref<Material> _material;
	Ref<Material> _liquid_material;
	Ref<Material> _prop_material;
};

#endif // TERRAIN_2D_LIBRARY_H
