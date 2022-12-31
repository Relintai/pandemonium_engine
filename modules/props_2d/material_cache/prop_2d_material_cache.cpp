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

#include "prop_2d_material_cache.h"

#include "../props/prop_2d_data.h"
#include "../props/prop_2d_data_prop.h"
#include "../props/prop_2d_data_tiled_wall_2d.h"
#include "../singleton/prop_2d_cache.h"
#include "../tiled_wall/tiled_wall_2d_data.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../../mesh_data_resource/props_2d/prop_2d_data_mesh_data.h"
#endif

#define VARIANT_ARRAY_GET(arr)             \
	Vector<Variant> r;                     \
	for (int i = 0; i < arr.size(); i++) { \
		r.push_back(arr[i].get_ref_ptr()); \
	}                                      \
	return r;

bool Prop2DMaterialCache::get_initialized() {
	return _initialized;
}
void Prop2DMaterialCache::set_initialized(const bool value) {
	_initialized = value;
}

bool Prop2DMaterialCache::mutex_locked() {
	return _locked;
}
void Prop2DMaterialCache::mutex_lock() {
	_mutex.lock();
}
void Prop2DMaterialCache::mutex_unlock() {
	_mutex.unlock();
}

int Prop2DMaterialCache::get_ref_count() {
	return _ref_count;
}
void Prop2DMaterialCache::set_ref_count(const int value) {
	_ref_count = value;
}
void Prop2DMaterialCache::inc_ref_count() {
	_ref_count += 1;
}
void Prop2DMaterialCache::dec_ref_count() {
	_ref_count -= 1;
}

//Materials
Ref<Material> Prop2DMaterialCache::material_get() {
	return _material;
}

void Prop2DMaterialCache::material_set(const Ref<Material> &value) {
	_material = value;
}

void Prop2DMaterialCache::texture_add(const Ref<Texture> &texture) {
	_textures.push_back(texture);
}
void Prop2DMaterialCache::texture_remove(const Ref<Texture> &texture) {
	for (int i = 0; i < _textures.size(); ++i) {
		if (_textures[i] == texture) {
			_textures.remove(i);
			return;
		}
	}
}
void Prop2DMaterialCache::texture_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _textures.size());

	_textures.remove(index);
}
void Prop2DMaterialCache::textures_clear() {
	_textures.clear();
}
int Prop2DMaterialCache::texture_count() {
	return _textures.size();
}
Ref<Texture> Prop2DMaterialCache::texture_get(const int index) {
	ERR_FAIL_INDEX_V(index, _textures.size(), Ref<Texture>());

	return _textures[index];
}
Ref<AtlasTexture> Prop2DMaterialCache::texture_get_atlas(const int index) {
	ERR_FAIL_INDEX_V(index, _textures.size(), Ref<AtlasTexture>());

	return texture_get_atlas_tex(_textures[index]);
}
Ref<AtlasTexture> Prop2DMaterialCache::texture_get_atlas_tex(const Ref<Texture> &texture) {
	return Ref<AtlasTexture>();
}
Rect2 Prop2DMaterialCache::texture_get_rect(const Ref<Texture> &texture) {
	Ref<AtlasTexture> at = texture_get_atlas_tex(texture);

	if (!at.is_valid()) {
		return Rect2();
	}

	return at->get_region();
}
Rect2 Prop2DMaterialCache::texture_get_uv_rect(const Ref<Texture> &texture) {
	return Rect2(0, 0, 1, 1);
}

Ref<Texture> Prop2DMaterialCache::texture_get_merged() {
	return Ref<Texture>();
}

void Prop2DMaterialCache::prop_add_textures(const Ref<Prop2DData> &prop) {
	if (!prop.is_valid()) {
		return;
	}

	for (int i = 0; i < prop->get_prop_count(); ++i) {
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		Ref<Prop2DDataMeshData> pdm = prop->get_prop(i);

		if (pdm.is_valid()) {
			Ref<Texture> tex = pdm->get_texture();

			if (!tex.is_valid())
				continue;

			texture_add(tex);

			continue;
		}
#endif

		Ref<Prop2DDataTiledWall2D> pdtw = prop->get_prop(i);

		if (pdtw.is_valid()) {
			Ref<TiledWall2DData> twd = pdtw->get_data();

			if (!twd.is_valid())
				continue;

			twd->setup_cache(Ref<Prop2DMaterialCache>(this));

			continue;
		}

		Ref<Prop2DDataProp2D> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			prop_add_textures(pdp->get_prop());
		}
	}
}
void Prop2DMaterialCache::prop_remove_textures(const Ref<Prop2DData> &prop) {
	if (!prop.is_valid()) {
		return;
	}

	for (int i = 0; i < prop->get_prop_count(); ++i) {
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		Ref<Prop2DDataMeshData> pdm = prop->get_prop(i);

		if (pdm.is_valid()) {
			Ref<Texture> tex = pdm->get_texture();

			if (!tex.is_valid())
				continue;

			texture_remove(tex);
		}
#endif

		Ref<Prop2DDataTiledWall2D> pdtw = prop->get_prop(i);

		if (pdtw.is_valid()) {
			Ref<TiledWall2DData> twd = pdtw->get_data();

			if (!twd.is_valid())
				continue;

			for (int j = 0; j < twd->get_texture_count(); ++j) {
				const Ref<Texture> &tex = twd->get_texture(j);

				if (tex.is_valid()) {
					texture_remove(tex);
				}
			}

			for (int j = 0; j < twd->get_flavour_texture_count(); ++j) {
				const Ref<Texture> &tex = twd->get_flavour_texture(j);

				if (tex.is_valid()) {
					texture_remove(tex);
				}
			}

			continue;
		}

		Ref<Prop2DDataProp2D> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			prop_remove_textures(pdp);
		}
	}
}

void Prop2DMaterialCache::refresh_rects() {
	_initialized = true;
}

void Prop2DMaterialCache::initial_setup_default() {
	//Note: call only on the main thread! Shader->duplicate() can crash if done from an another thread!
	//Also shader duplication is synchronized with the main thread. So you can cause daedlocks if you hold up the main thread
	//Somewhere else.

	Prop2DCache *pc = Prop2DCache::get_singleton();

	pc->ensure_material_loaded();

	Ref<Material> m = pc->material_get();

	if (m.is_valid()) {
		Ref<Material> md = m->duplicate();

		_material = md;
	}
}

void Prop2DMaterialCache::setup_material_albedo(Ref<Texture> texture) {
	if (has_method("_setup_material_albedo"))
		call("_setup_material_albedo", texture);
}

Prop2DMaterialCache::Prop2DMaterialCache() {
	_ref_count = 0;
	_initialized = false;
	_locked = false;
}

Prop2DMaterialCache::~Prop2DMaterialCache() {
}

void Prop2DMaterialCache::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &Prop2DMaterialCache::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &Prop2DMaterialCache::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized"), "set_initialized", "get_initialized");

	ClassDB::bind_method(D_METHOD("mutex_locked"), &Prop2DMaterialCache::mutex_locked);
	ClassDB::bind_method(D_METHOD("mutex_lock"), &Prop2DMaterialCache::mutex_lock);
	ClassDB::bind_method(D_METHOD("mutex_unlock"), &Prop2DMaterialCache::mutex_unlock);

	ClassDB::bind_method(D_METHOD("get_ref_count"), &Prop2DMaterialCache::get_ref_count);
	ClassDB::bind_method(D_METHOD("set_ref_count", "value"), &Prop2DMaterialCache::set_ref_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mat_ref_count"), "set_ref_count", "get_ref_count");
	ClassDB::bind_method(D_METHOD("inc_ref_count"), &Prop2DMaterialCache::inc_ref_count);
	ClassDB::bind_method(D_METHOD("dec_ref_count"), &Prop2DMaterialCache::dec_ref_count);

	BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));

	ClassDB::bind_method(D_METHOD("material_get"), &Prop2DMaterialCache::material_get);
	ClassDB::bind_method(D_METHOD("material_set", "value"), &Prop2DMaterialCache::material_set);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "material_set", "material_get");

	ClassDB::bind_method(D_METHOD("texture_add", "texture"), &Prop2DMaterialCache::texture_add);
	ClassDB::bind_method(D_METHOD("texture_remove", "texture"), &Prop2DMaterialCache::texture_remove);
	ClassDB::bind_method(D_METHOD("texture_remove_index", "index"), &Prop2DMaterialCache::texture_remove_index);
	ClassDB::bind_method(D_METHOD("textures_clear"), &Prop2DMaterialCache::textures_clear);
	ClassDB::bind_method(D_METHOD("texture_count"), &Prop2DMaterialCache::texture_count);
	ClassDB::bind_method(D_METHOD("texture_get", "index"), &Prop2DMaterialCache::texture_get);
	ClassDB::bind_method(D_METHOD("texture_get_atlas", "index"), &Prop2DMaterialCache::texture_get_atlas);
	ClassDB::bind_method(D_METHOD("texture_get_atlas_tex", "index"), &Prop2DMaterialCache::texture_get_atlas_tex);
	ClassDB::bind_method(D_METHOD("texture_get_uv_rect", "texture"), &Prop2DMaterialCache::texture_get_uv_rect);
	ClassDB::bind_method(D_METHOD("texture_get_merged"), &Prop2DMaterialCache::texture_get_merged);

	ClassDB::bind_method(D_METHOD("prop_add_textures", "prop"), &Prop2DMaterialCache::prop_add_textures);
	ClassDB::bind_method(D_METHOD("prop_remove_textures", "prop"), &Prop2DMaterialCache::prop_remove_textures);

	ClassDB::bind_method(D_METHOD("refresh_rects"), &Prop2DMaterialCache::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "texture"), &Prop2DMaterialCache::setup_material_albedo);
}
