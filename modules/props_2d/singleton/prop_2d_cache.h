#ifndef PROP_2D_CACHE_H
#define PROP_2D_CACHE_H
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

#include "core/bind/core_bind.h"
#include "core/math/color.h"
#include "core/containers/hash_map.h"
#include "core/object/object.h"
#include "core/object/reference.h"
#include "core/containers/vector.h"

#include "scene/resources/material.h"

#include "core/os/mutex.h"

#include "../props/prop_2d_data.h"

#include "modules/modules_enabled.gen.h"

class Prop2DMaterialCache;
class TiledWall2DData;

class Prop2DCache : public Object {
	GDCLASS(Prop2DCache, Object);

public:
	static Prop2DCache *get_singleton();

	float get_default_pixels_per_unit() const;
	void set_default_pixels_per_unit(const float value);

	StringName get_default_prop_material_cache_class();
	void set_default_prop_material_cache_class(const StringName &cls_name);

#ifdef MODULE_TEXTURE_PACKER_ENABLED
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
#endif

	String material_path_get() const;
	void material_path_set(const String &array);

	Ref<Material> material_get();
	void material_set(const Ref<Material> &value);
	void material_load();
	void ensure_material_loaded();

	Ref<Prop2DMaterialCache> material_cache_get(const Ref<Prop2DData> &prop);
	void material_cache_unref(const Ref<Prop2DData> &prop);

	Ref<Prop2DMaterialCache> tiled_wall_material_cache_get(const Ref<TiledWall2DData> &twd);
	void tiled_wall_material_cache_unref(const Ref<TiledWall2DData> &twd);

	Ref<Prop2DMaterialCache> material_cache_custom_key_get(const uint64_t key);
	void material_cache_custom_key_unref(const uint64_t key);

	Ref<Resource> load_resource(const String &path, const String &type_hint = "");

private:
	static Prop2DCache *_instance;

public:
	Prop2DCache();
	~Prop2DCache();

protected:
	static void _bind_methods();

	float _default_pixels_per_unit;

	StringName _default_prop_material_cache_class;

	Map<uint64_t, Ref<Prop2DMaterialCache>> _material_cache;
	Map<uint64_t, Ref<Prop2DMaterialCache>> _tiled_wall_material_cache;
	Map<uint64_t, Ref<Prop2DMaterialCache>> _custom_keyed_material_cache;

	Mutex _material_cache_mutex;
	Mutex _tiled_wall_material_cache_mutex;
	Mutex _custom_keyed_material_cache_mutex;

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	int _texture_flags;
	int _max_atlas_size;
	bool _keep_original_atlases;
	Color _background_color;
	int _margin;
#endif

	String _material_path;
	Ref<Material> _material;
};

#endif
