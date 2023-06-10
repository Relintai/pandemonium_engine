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

#include "prop_material_cache.h"

#include "../../props/props/prop_data.h"
#include "../../props/props/prop_data_prop.h"
#include "../../props/props/prop_data_tiled_wall.h"
#include "../singleton/prop_cache.h"
#include "../tiled_wall/tiled_wall_data.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../../mesh_data_resource/props/prop_data_mesh_data.h"
#endif

#define VARIANT_ARRAY_GET(arr)             \
	Vector<Variant> r;                     \
	for (int i = 0; i < arr.size(); i++) { \
		r.push_back(arr[i].get_ref_ptr()); \
	}                                      \
	return r;

bool PropMaterialCache::get_initialized() {
	return _initialized;
}
void PropMaterialCache::set_initialized(const bool value) {
	_initialized = value;
}

bool PropMaterialCache::mutex_locked() {
	return _locked;
}
void PropMaterialCache::mutex_lock() {
	_mutex.lock();
}
void PropMaterialCache::mutex_unlock() {
	_mutex.unlock();
}

int PropMaterialCache::get_ref_count() {
	return _ref_count;
}
void PropMaterialCache::set_ref_count(const int value) {
	_ref_count = value;
}
void PropMaterialCache::inc_ref_count() {
	_ref_count += 1;
}
void PropMaterialCache::dec_ref_count() {
	_ref_count -= 1;
}

//Materials
Ref<Material> PropMaterialCache::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<Material>(NULL));

	return _materials[index];
}

Ref<Material> PropMaterialCache::material_lod_get(const int index) {
	ERR_FAIL_COND_V(_materials.size() == 0, Ref<Material>(NULL));

	if (index < 0) {
		return _materials[0];
	}

	if (index >= _materials.size()) {
		return _materials[_materials.size() - 1];
	}

	return _materials[index];
}

void PropMaterialCache::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

void PropMaterialCache::material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _materials.size());

	_materials.set(index, value);
}

void PropMaterialCache::material_remove(const int index) {
	_materials.remove(index);
}

int PropMaterialCache::material_get_num() const {
	return _materials.size();
}

void PropMaterialCache::materials_clear() {
	_materials.clear();
}

Vector<Variant> PropMaterialCache::materials_get() {
	VARIANT_ARRAY_GET(_materials);
}

void PropMaterialCache::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

void PropMaterialCache::texture_add(const Ref<Texture> &texture) {
	_textures.push_back(texture);
}
void PropMaterialCache::texture_remove(const Ref<Texture> &texture) {
	for (int i = 0; i < _textures.size(); ++i) {
		if (_textures[i] == texture) {
			_textures.remove(i);
			return;
		}
	}
}
void PropMaterialCache::texture_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _textures.size());

	_textures.remove(index);
}
void PropMaterialCache::textures_clear() {
	_textures.clear();
}
int PropMaterialCache::texture_count() {
	return _textures.size();
}
Ref<Texture> PropMaterialCache::texture_get(const int index) {
	ERR_FAIL_INDEX_V(index, _textures.size(), Ref<Texture>());

	return _textures[index];
}
Ref<AtlasTexture> PropMaterialCache::texture_get_atlas(const int index) {
	ERR_FAIL_INDEX_V(index, _textures.size(), Ref<AtlasTexture>());

	return texture_get_atlas_tex(_textures[index]);
}
Ref<AtlasTexture> PropMaterialCache::texture_get_atlas_tex(const Ref<Texture> &texture) {
	return Ref<AtlasTexture>();
}
Rect2 PropMaterialCache::texture_get_uv_rect(const Ref<Texture> &texture) {
	return Rect2(0, 0, 1, 1);
}

void PropMaterialCache::prop_add_textures(const Ref<PropData> &prop) {
	if (!prop.is_valid()) {
		return;
	}

	for (int i = 0; i < prop->get_prop_count(); ++i) {
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		Ref<PropDataMeshData> pdm = prop->get_prop(i);

		if (pdm.is_valid()) {
			Ref<Texture> tex = pdm->get_texture();

			if (!tex.is_valid())
				continue;

			texture_add(tex);

			continue;
		}
#endif

		Ref<PropDataTiledWall> pdtw = prop->get_prop(i);

		if (pdtw.is_valid()) {
			Ref<TiledWallData> twd = pdtw->get_data();

			if (!twd.is_valid())
				continue;

			twd->setup_cache(Ref<PropMaterialCache>(this));

			continue;
		}

		Ref<PropDataProp> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			prop_add_textures(pdp->get_prop());
		}
	}
}
void PropMaterialCache::prop_remove_textures(const Ref<PropData> &prop) {
	if (!prop.is_valid()) {
		return;
	}

	for (int i = 0; i < prop->get_prop_count(); ++i) {
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		Ref<PropDataMeshData> pdm = prop->get_prop(i);

		if (pdm.is_valid()) {
			Ref<Texture> tex = pdm->get_texture();

			if (!tex.is_valid())
				continue;

			texture_remove(tex);
		}
#endif

		Ref<PropDataTiledWall> pdtw = prop->get_prop(i);

		if (pdtw.is_valid()) {
			Ref<TiledWallData> twd = pdtw->get_data();

			if (!twd.is_valid())
				continue;

			for (int j = 0; j < twd->get_tile_count(); ++j) {
				const Ref<Texture> &tex = twd->get_tile_texture(j);

				if (tex.is_valid()) {
					texture_remove(tex);
				}
			}

			for (int j = 0; j < twd->get_flavour_tile_count(); ++j) {
				const Ref<Texture> &tex = twd->get_flavour_tile_texture(j);

				if (tex.is_valid()) {
					texture_remove(tex);
				}
			}

			continue;
		}

		Ref<PropDataProp> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			prop_remove_textures(pdp);
		}
	}
}

void PropMaterialCache::refresh_rects() {
	_initialized = true;
}

void PropMaterialCache::initial_setup_default() {
	//Note: call only on the main thread! Shader->duplicate() can crash if done from an another thread!

	PropCache *pc = PropCache::get_singleton();

	pc->ensure_materials_loaded();

	int matc = pc->material_get_num();
	for (int i = 0; i < matc; ++i) {
		Ref<Material> m = pc->material_get(i);

		ERR_CONTINUE(!m.is_valid());

		Ref<Material> md = m->duplicate();

		_materials.push_back(md);
	}
}

void PropMaterialCache::setup_material_albedo(Ref<Texture> texture) {
	if (has_method("_setup_material_albedo"))
		call("_setup_material_albedo", texture);
}

PropMaterialCache::PropMaterialCache() {
	_ref_count = 0;
	_initialized = false;
	_locked = false;
}

PropMaterialCache::~PropMaterialCache() {
	_materials.clear();
}

void PropMaterialCache::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &PropMaterialCache::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &PropMaterialCache::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized"), "set_initialized", "get_initialized");

	ClassDB::bind_method(D_METHOD("mutex_locked"), &PropMaterialCache::mutex_locked);
	ClassDB::bind_method(D_METHOD("mutex_lock"), &PropMaterialCache::mutex_lock);
	ClassDB::bind_method(D_METHOD("mutex_unlock"), &PropMaterialCache::mutex_unlock);

	ClassDB::bind_method(D_METHOD("get_ref_count"), &PropMaterialCache::get_ref_count);
	ClassDB::bind_method(D_METHOD("set_ref_count", "value"), &PropMaterialCache::set_ref_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mat_ref_count"), "set_ref_count", "get_ref_count");
	ClassDB::bind_method(D_METHOD("inc_ref_count"), &PropMaterialCache::inc_ref_count);
	ClassDB::bind_method(D_METHOD("dec_ref_count"), &PropMaterialCache::dec_ref_count);

	BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));

	ClassDB::bind_method(D_METHOD("material_get", "index"), &PropMaterialCache::material_get);
	ClassDB::bind_method(D_METHOD("material_lod_get", "index"), &PropMaterialCache::material_lod_get);
	ClassDB::bind_method(D_METHOD("material_add", "value"), &PropMaterialCache::material_add);
	ClassDB::bind_method(D_METHOD("material_set", "index", "value"), &PropMaterialCache::material_set);
	ClassDB::bind_method(D_METHOD("material_remove", "index"), &PropMaterialCache::material_remove);
	ClassDB::bind_method(D_METHOD("material_get_num"), &PropMaterialCache::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &PropMaterialCache::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &PropMaterialCache::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &PropMaterialCache::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "23/20:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

	ClassDB::bind_method(D_METHOD("texture_add", "texture"), &PropMaterialCache::texture_add);
	ClassDB::bind_method(D_METHOD("texture_remove", "texture"), &PropMaterialCache::texture_remove);
	ClassDB::bind_method(D_METHOD("texture_remove_index", "index"), &PropMaterialCache::texture_remove_index);
	ClassDB::bind_method(D_METHOD("textures_clear"), &PropMaterialCache::textures_clear);
	ClassDB::bind_method(D_METHOD("texture_count"), &PropMaterialCache::texture_count);
	ClassDB::bind_method(D_METHOD("texture_get", "index"), &PropMaterialCache::texture_get);
	ClassDB::bind_method(D_METHOD("texture_get_atlas", "index"), &PropMaterialCache::texture_get_atlas);
	ClassDB::bind_method(D_METHOD("texture_get_atlas_tex", "index"), &PropMaterialCache::texture_get_atlas_tex);
	ClassDB::bind_method(D_METHOD("texture_get_uv_rect", "texture"), &PropMaterialCache::texture_get_uv_rect);

	ClassDB::bind_method(D_METHOD("prop_add_textures", "prop"), &PropMaterialCache::prop_add_textures);
	ClassDB::bind_method(D_METHOD("prop_remove_textures", "prop"), &PropMaterialCache::prop_remove_textures);

	ClassDB::bind_method(D_METHOD("refresh_rects"), &PropMaterialCache::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "texture"), &PropMaterialCache::setup_material_albedo);
}
