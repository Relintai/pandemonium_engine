/*************************************************************************/
/*  terrain_material_cache.cpp                                           */
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

#include "terrain_material_cache.h"

#include "../defines.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_PROPS_ENABLED
#include "../../props/props/prop_data.h"
#include "../../props/props/prop_data_prop.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../../mesh_data_resource/props/prop_data_mesh_data.h"
#endif
#endif

bool TerrainMaterialCache::get_initialized() {
	return _initialized;
}
void TerrainMaterialCache::set_initialized(const bool value) {
	_initialized = value;
}

int TerrainMaterialCache::get_ref_count() {
	return _ref_count;
}
void TerrainMaterialCache::set_ref_count(const int value) {
	_ref_count = value;
}
void TerrainMaterialCache::inc_ref_count() {
	_ref_count += 1;
}
void TerrainMaterialCache::dec_ref_count() {
	_ref_count -= 1;
}

//Materials
Ref<Material> TerrainMaterialCache::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<Material>(NULL));

	return _materials[index];
}

Ref<Material> TerrainMaterialCache::material_lod_get(const int index) {
	ERR_FAIL_COND_V(_materials.size() == 0, Ref<Material>(NULL));

	if (index < 0) {
		return _materials[0];
	}

	if (index >= _materials.size()) {
		return _materials[_materials.size() - 1];
	}

	return _materials[index];
}

void TerrainMaterialCache::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

void TerrainMaterialCache::material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _materials.size());

	_materials.set(index, value);
}

void TerrainMaterialCache::material_remove(const int index) {
	_materials.remove(index);
}

int TerrainMaterialCache::material_get_num() const {
	return _materials.size();
}

void TerrainMaterialCache::materials_clear() {
	_materials.clear();
}

Vector<Variant> TerrainMaterialCache::materials_get() {
	VARIANT_ARRAY_GET(_materials);
}

void TerrainMaterialCache::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

//Surfaces
Ref<TerrainSurface> TerrainMaterialCache::surface_get(const int index) {
	ERR_FAIL_INDEX_V(index, _surfaces.size(), Ref<TerrainSurface>());

	return _surfaces[index];
}
Ref<TerrainSurface> TerrainMaterialCache::surface_id_get(const int id) {
	Ref<TerrainSurface> surface;

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
void TerrainMaterialCache::surface_add(Ref<TerrainSurface> value) {
	ERR_FAIL_COND(!value.is_valid());

	_surfaces.push_back(value);
}
void TerrainMaterialCache::surface_set(int index, Ref<TerrainSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_surfaces.size() < index) {
		_surfaces.resize(index + 1);
	}

	_surfaces.set(index, value);
}
void TerrainMaterialCache::surface_remove(const int index) {
	_surfaces.remove(index);
}
int TerrainMaterialCache::surface_get_num() const {
	return _surfaces.size();
}
void TerrainMaterialCache::surfaces_clear() {
	_surfaces.clear();
}

void TerrainMaterialCache::additional_texture_add(const Ref<Texture> &texture) {
	_additional_textures.push_back(texture);
}
void TerrainMaterialCache::additional_texture_remove(const Ref<Texture> &texture) {
	for (int i = 0; i < _additional_textures.size(); ++i) {
		if (_additional_textures[i] == texture) {
			_additional_textures.remove(i);
			return;
		}
	}
}
void TerrainMaterialCache::additional_texture_remove_index(const int index) {
	ERR_FAIL_INDEX(index, _additional_textures.size());

	_additional_textures.remove(index);
}
void TerrainMaterialCache::additional_textures_clear() {
	_additional_textures.clear();
}
int TerrainMaterialCache::additional_texture_count() {
	return _additional_textures.size();
}
Ref<Texture> TerrainMaterialCache::additional_texture_get(const int index) {
	ERR_FAIL_INDEX_V(index, _additional_textures.size(), Ref<Texture>());

	return _additional_textures[index];
}
Ref<AtlasTexture> TerrainMaterialCache::additional_texture_get_atlas(const int index) {
	ERR_FAIL_INDEX_V(index, _additional_textures.size(), Ref<AtlasTexture>());

	return additional_texture_get_atlas_tex(_additional_textures[index]);
}
Ref<AtlasTexture> TerrainMaterialCache::additional_texture_get_atlas_tex(const Ref<Texture> &texture) {
	return Ref<AtlasTexture>();
}
Rect2 TerrainMaterialCache::additional_texture_get_uv_rect(const Ref<Texture> &texture) {
	return Rect2(0, 0, 1, 1);
}

#ifdef MODULE_PROPS_ENABLED
void TerrainMaterialCache::prop_add_textures(const Ref<PropData> &prop) {
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

			additional_texture_add(tex);
		}
#endif

		Ref<PropDataProp> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			prop_add_textures(pdp);
		}
	}
}
void TerrainMaterialCache::prop_remove_textures(const Ref<PropData> &prop) {
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

			additional_texture_remove(tex);
		}
#endif

		Ref<PropDataProp> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			prop_remove_textures(pdp);
		}
	}
}
#endif

void TerrainMaterialCache::refresh_rects() {
	_initialized = true;
}

void TerrainMaterialCache::setup_material_albedo(Ref<Texture> texture) {
	if (has_method("_setup_material_albedo")) {
		call("_setup_material_albedo", texture);
	}
}

TerrainMaterialCache::TerrainMaterialCache() {
	_ref_count = 0;
	_initialized = false;
}

TerrainMaterialCache::~TerrainMaterialCache() {
	_materials.clear();
	_surfaces.clear();
}

void TerrainMaterialCache::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_initialized"), &TerrainMaterialCache::get_initialized);
	ClassDB::bind_method(D_METHOD("set_initialized", "value"), &TerrainMaterialCache::set_initialized);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "initialized"), "set_initialized", "get_initialized");

	ClassDB::bind_method(D_METHOD("get_ref_count"), &TerrainMaterialCache::get_ref_count);
	ClassDB::bind_method(D_METHOD("set_ref_count", "value"), &TerrainMaterialCache::set_ref_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mat_ref_count"), "set_ref_count", "get_ref_count");
	ClassDB::bind_method(D_METHOD("inc_ref_count"), &TerrainMaterialCache::inc_ref_count);
	ClassDB::bind_method(D_METHOD("dec_ref_count"), &TerrainMaterialCache::dec_ref_count);

	BIND_VMETHOD(MethodInfo("_setup_material_albedo", PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture")));

	ClassDB::bind_method(D_METHOD("material_get", "index"), &TerrainMaterialCache::material_get);
	ClassDB::bind_method(D_METHOD("material_lod_get", "index"), &TerrainMaterialCache::material_lod_get);
	ClassDB::bind_method(D_METHOD("material_add", "value"), &TerrainMaterialCache::material_add);
	ClassDB::bind_method(D_METHOD("material_set", "index", "value"), &TerrainMaterialCache::material_set);
	ClassDB::bind_method(D_METHOD("material_remove", "index"), &TerrainMaterialCache::material_remove);
	ClassDB::bind_method(D_METHOD("material_get_num"), &TerrainMaterialCache::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &TerrainMaterialCache::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &TerrainMaterialCache::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &TerrainMaterialCache::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "23/20:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

	ClassDB::bind_method(D_METHOD("surface_get", "index"), &TerrainMaterialCache::surface_get);
	ClassDB::bind_method(D_METHOD("surface_id_get", "index"), &TerrainMaterialCache::surface_id_get);

	ClassDB::bind_method(D_METHOD("surface_add", "value"), &TerrainMaterialCache::surface_add);
	ClassDB::bind_method(D_METHOD("surface_set", "index", "surface"), &TerrainMaterialCache::surface_set);
	ClassDB::bind_method(D_METHOD("surface_remove", "index"), &TerrainMaterialCache::surface_remove);
	ClassDB::bind_method(D_METHOD("surface_get_num"), &TerrainMaterialCache::surface_get_num);
	ClassDB::bind_method(D_METHOD("surfaces_clear"), &TerrainMaterialCache::surfaces_clear);

	ClassDB::bind_method(D_METHOD("additional_texture_add", "texture"), &TerrainMaterialCache::additional_texture_add);
	ClassDB::bind_method(D_METHOD("additional_texture_remove", "texture"), &TerrainMaterialCache::additional_texture_remove);
	ClassDB::bind_method(D_METHOD("additional_texture_remove_index", "index"), &TerrainMaterialCache::additional_texture_remove_index);
	ClassDB::bind_method(D_METHOD("additional_textures_clear"), &TerrainMaterialCache::additional_textures_clear);
	ClassDB::bind_method(D_METHOD("additional_texture_count"), &TerrainMaterialCache::additional_texture_count);
	ClassDB::bind_method(D_METHOD("additional_texture_get", "index"), &TerrainMaterialCache::additional_texture_get);
	ClassDB::bind_method(D_METHOD("additional_texture_get_atlas", "index"), &TerrainMaterialCache::additional_texture_get_atlas);
	ClassDB::bind_method(D_METHOD("additional_texture_get_atlas_tex", "index"), &TerrainMaterialCache::additional_texture_get_atlas_tex);
	ClassDB::bind_method(D_METHOD("additional_texture_get_uv_rect", "texture"), &TerrainMaterialCache::additional_texture_get_uv_rect);

#ifdef MODULE_PROPS_ENABLED
	ClassDB::bind_method(D_METHOD("prop_add_textures", "prop"), &TerrainMaterialCache::prop_add_textures);
	ClassDB::bind_method(D_METHOD("prop_remove_textures", "prop"), &TerrainMaterialCache::prop_remove_textures);
#endif

	ClassDB::bind_method(D_METHOD("refresh_rects"), &TerrainMaterialCache::refresh_rects);

	ClassDB::bind_method(D_METHOD("setup_material_albedo", "texture"), &TerrainMaterialCache::setup_material_albedo);
}
