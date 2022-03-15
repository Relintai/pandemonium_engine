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

#include "terrain_2d_material_cache.h"

#include "../defines.h"

#ifdef PROPS_2D_PRESENT
#include "../../props_2d/props/prop_2d_data.h"
#include "../../props_2d/props/prop_2d_data_prop.h"

#if MESH_DATA_RESOURCE_PRESENT
#include "../../mesh_data_resource/props_2d/prop_2d_data_mesh_data.h"
#endif
#endif

bool Terrain2DMaterialCache::get_initialized() {
	return _initialized;
}
void Terrain2DMaterialCache::set_initialized(const bool value) {
	_initialized = value;
}

int Terrain2DMaterialCache::get_ref_count() {
	return _ref_count;
}
void Terrain2DMaterialCache::set_ref_count(const int value) {
	_ref_count = value;
}
void Terrain2DMaterialCache::inc_ref_count() {
	_ref_count += 1;
}
void Terrain2DMaterialCache::dec_ref_count() {
	_ref_count -= 1;
}

//Materials
Ref<Material> Terrain2DMaterialCache::material_get() {
	return _material;
}

void Terrain2DMaterialCache::material_set(const Ref<Material> &value) {
	_material = value;
}

//Surfaces
Ref<Terrain2DSurface> Terrain2DMaterialCache::surface_get(const int index) {
	ERR_FAIL_INDEX_V(index, _surfaces.size(), Ref<Terrain2DSurface>());

	return _surfaces[index];
}
Ref<Terrain2DSurface> Terrain2DMaterialCache::surface_id_get(const int id) {
	Ref<Terrain2DSurface> surface;

	for (int i = 0; i < _surfaces.size(); ++i) {
		surface = _surfaces[i];

		if (surface.is_valid()) {
			if (surface->get_id() == id) {
				break;
			}
		}
	}

	return surface;
}
void Terrain2DMaterialCache::surface_add(Ref<Terrain2DSurface> value) {
	ERR_FAIL_COND(!value.is_valid());

	_surfaces.push_back(value);
}
void Terrain2DMaterialCache::surface_set(int index, Ref<Terrain2DSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_surfaces.size() < index) {
		_surfaces.resize(index + 1);
	}

	_surfaces.set(index, value);
}
void Terrain2DMaterialCache::surface_remove(const int index) {
	_surfaces.VREMOVE(index);
}
int Terrain2DMaterialCache::surface_get_num() const {
	return _surfaces.size();
}
void Terrain2DMaterialCache::surfaces_clear() {
	_surfaces.clear();
}

void Terrain2DMaterialCache::additional_texture_add(const Ref<Texture> &texture) {
	_additional_textures.push_back(texture);
}
void Terrain2DMaterialCache::additional_texture_remove(const Ref<Texture> &texture) {
	for (int i = 0; i < _additional_textures.size(); ++i) {
		if (_additional_textures[i] == texture) {
			_additional_textures.VREMOVE(i);
			return;
		}
	}
}
void Terrain2DMaterialCache::additional_texture_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _additional_textures.size());

	_additional_textures.VREMOVE(index);
}
void Terrain2DMaterialCache::additional_textures_clear() {
	_additional_textures.clear();
}
int Terrain2DMaterialCache::additional_texture_count() {
	return _additional_textures.size();
}
Ref<Texture> Terrain2DMaterialCache::additional_texture_get(const int index) {
	ERR_FAIL_INDEX_V(index, _additional_textures.size(), Ref<Texture>());

	return _additional_textures[index];
}
Ref<AtlasTexture> Terrain2DMaterialCache::additional_texture_get_atlas(const int index) {
	ERR_FAIL_INDEX_V(index, _additional_textures.size(), Ref<AtlasTexture>());

	return additional_texture_get_atlas_tex(_additional_textures[index]);
}
Ref<AtlasTexture> Terrain2DMaterialCache::additional_texture_get_atlas_tex(const Ref<Texture> &texture) {
	return Ref<AtlasTexture>();
}
Rect2 Terrain2DMaterialCache::additional_texture_get_rect(const Ref<Texture> &texture) {
	Ref<AtlasTexture> at = additional_texture_get_atlas_tex(texture);

	if (!at.is_valid()) {
		return Rect2();
	}

	return at->get_region();
}
Rect2 Terrain2DMaterialCache::additional_texture_get_uv_rect(const Ref<Texture> &texture) {
	return Rect2(0, 0, 1, 1);
}

Ref<Texture> Terrain2DMaterialCache::texture_get_merged() {
	return Ref<Texture>();
}

#ifdef PROPS_2D_PRESENT
void Terrain2DMaterialCache::prop_add_textures(const Ref<Prop2DData> &prop) {
	if (!prop.is_valid()) {
		return;
	}

	for (int i = 0; i < prop->get_prop_count(); ++i) {
#if MESH_DATA_RESOURCE_PRESENT
		Ref<Prop2DDataMeshData> pdm = prop->get_prop(i);

		if (pdm.is_valid()) {
			Ref<Texture> tex = pdm->get_texture();

			if (!tex.is_valid())
				continue;

			additional_texture_add(tex);
		}
#endif

		Ref<Prop2DDataProp2D> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			prop_add_textures(pdp);
		}
	}
}
void Terrain2DMaterialCache::prop_remove_textures(const Ref<Prop2DData> &prop) {
	if (!prop.is_valid()) {
		return;
	}

	for (int i = 0; i < prop->get_prop_count(); ++i) {
#if MESH_DATA_RESOURCE_PRESENT
		Ref<Prop2DDataMeshData> pdm = prop->get_prop(i);

		if (pdm.is_valid()) {
			Ref<Texture> tex = pdm->get_texture();

			if (!tex.is_valid())
				continue;

			additional_texture_remove(tex);
		}
#endif

		Ref<Prop2DDataProp2D> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			prop_remove_textures(pdp);
		}
	}
}
#endif

void Terrain2DMaterialCache::refresh_rects() {
	_initialized = true;
}

void Terrain2DMaterialCache::setup_material_albedo(Ref<Texture> texture) {
#if VERSION_MAJOR < 4
	if (has_method("_setup_material_albedo")) {
		call("_setup_material_albedo", texture);
	}
#else
	GDVIRTUAL_CALL(_setup_material_albedo, texture);
#endif
}

Terrain2DMaterialCache::Terrain2DMaterialCache() {
	_ref_count = 0;
	_initialized = false;
}

Terrain2DMaterialCache::~Terrain2DMaterialCache() {
	_material.unref();
	_surfaces.clear();
}

void Terrain2DMaterialCache::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &Terrain2DMaterialCache::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &Terrain2DMaterialCache::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized"), "set_initialized", "get_initialized");

	ClassDB::bind_method(D_METHOD("get_ref_count"), &Terrain2DMaterialCache::get_ref_count);
	ClassDB::bind_method(D_METHOD("set_ref_count", "value"), &Terrain2DMaterialCache::set_ref_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mat_ref_count"), "set_ref_count", "get_ref_count");
	ClassDB::bind_method(D_METHOD("inc_ref_count"), &Terrain2DMaterialCache::inc_ref_count);
	ClassDB::bind_method(D_METHOD("dec_ref_count"), &Terrain2DMaterialCache::dec_ref_count);

#if VERSION_MAJOR < 4
	BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));
#else
	GDVIRTUAL_BIND(_setup_material_albedo, "texture");
#endif

	ClassDB::bind_method(D_METHOD("material_get"), &Terrain2DMaterialCache::material_get);
	ClassDB::bind_method(D_METHOD("material_set", "value"), &Terrain2DMaterialCache::material_set);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "material_set", "material_get");

	ClassDB::bind_method(D_METHOD("surface_get", "index"), &Terrain2DMaterialCache::surface_get);
	ClassDB::bind_method(D_METHOD("surface_id_get", "index"), &Terrain2DMaterialCache::surface_id_get);

	ClassDB::bind_method(D_METHOD("surface_add", "value"), &Terrain2DMaterialCache::surface_add);
	ClassDB::bind_method(D_METHOD("surface_set", "index", "surface"), &Terrain2DMaterialCache::surface_set);
	ClassDB::bind_method(D_METHOD("surface_remove", "index"), &Terrain2DMaterialCache::surface_remove);
	ClassDB::bind_method(D_METHOD("surface_get_num"), &Terrain2DMaterialCache::surface_get_num);
	ClassDB::bind_method(D_METHOD("surfaces_clear"), &Terrain2DMaterialCache::surfaces_clear);

	ClassDB::bind_method(D_METHOD("additional_texture_add", "texture"), &Terrain2DMaterialCache::additional_texture_add);
	ClassDB::bind_method(D_METHOD("additional_texture_remove", "texture"), &Terrain2DMaterialCache::additional_texture_remove);
	ClassDB::bind_method(D_METHOD("additional_texture_remove_index", "index"), &Terrain2DMaterialCache::additional_texture_remove_index);
	ClassDB::bind_method(D_METHOD("additional_textures_clear"), &Terrain2DMaterialCache::additional_textures_clear);
	ClassDB::bind_method(D_METHOD("additional_texture_count"), &Terrain2DMaterialCache::additional_texture_count);
	ClassDB::bind_method(D_METHOD("additional_texture_get", "index"), &Terrain2DMaterialCache::additional_texture_get);
	ClassDB::bind_method(D_METHOD("additional_texture_get_atlas", "index"), &Terrain2DMaterialCache::additional_texture_get_atlas);
	ClassDB::bind_method(D_METHOD("additional_texture_get_atlas_tex", "index"), &Terrain2DMaterialCache::additional_texture_get_atlas_tex);
	ClassDB::bind_method(D_METHOD("additional_texture_get_rect", "texture"), &Terrain2DMaterialCache::additional_texture_get_rect);
	ClassDB::bind_method(D_METHOD("additional_texture_get_uv_rect", "texture"), &Terrain2DMaterialCache::additional_texture_get_uv_rect);

	ClassDB::bind_method(D_METHOD("texture_get_merged"), &Terrain2DMaterialCache::texture_get_merged);

#ifdef PROPS_2D_PRESENT
	ClassDB::bind_method(D_METHOD("prop_add_textures", "prop"), &Terrain2DMaterialCache::prop_add_textures);
	ClassDB::bind_method(D_METHOD("prop_remove_textures", "prop"), &Terrain2DMaterialCache::prop_remove_textures);
#endif

	ClassDB::bind_method(D_METHOD("refresh_rects"), &Terrain2DMaterialCache::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "texture"), &Terrain2DMaterialCache::setup_material_albedo);
}
