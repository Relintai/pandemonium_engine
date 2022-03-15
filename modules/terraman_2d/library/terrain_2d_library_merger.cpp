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

#include "terrain_2d_library_merger.h"

#include "scene/resources/packed_scene.h"
#include "scene/resources/texture.h"

#ifdef PROPS_2D_PRESENT
#include "../../props_2d/props/prop_2d_data.h"

#include "../../props_2d/props/prop_2d_data_prop.h"

#if MESH_DATA_RESOURCE_PRESENT
#include "../../mesh_data_resource/props_2d/prop_2d_data_mesh_data.h"
#endif
#endif

#include "../defines.h"

int Terrain2DLibraryMerger::get_texture_flags() const {
	return _packer->get_texture_flags();
}
void Terrain2DLibraryMerger::set_texture_flags(const int flags) {
	_packer->set_texture_flags(flags);
	_prop_packer->set_texture_flags(flags);
}

int Terrain2DLibraryMerger::get_max_atlas_size() const {
	return _packer->get_max_atlas_size();
}
void Terrain2DLibraryMerger::set_max_atlas_size(const int size) {
	_packer->set_max_atlas_size(size);
	_prop_packer->set_max_atlas_size(size);
}

bool Terrain2DLibraryMerger::get_keep_original_atlases() const {
	return _packer->get_keep_original_atlases();
}
void Terrain2DLibraryMerger::set_keep_original_atlases(const bool value) {
	_packer->set_keep_original_atlases(value);
	_prop_packer->set_keep_original_atlases(value);
}

Color Terrain2DLibraryMerger::get_background_color() const {
	return _packer->get_background_color();
}
void Terrain2DLibraryMerger::set_background_color(const Color &color) {
	_packer->set_background_color(color);
	_prop_packer->set_background_color(color);
}

int Terrain2DLibraryMerger::get_margin() const {
	return _packer->get_margin();
}
void Terrain2DLibraryMerger::set_margin(const int margin) {
	_packer->set_margin(margin);
	_prop_packer->set_margin(margin);
}

//Surfaces
Ref<Terrain2DSurface> Terrain2DLibraryMerger::terra_surface_get(const int index) {
	ERR_FAIL_INDEX_V(index, _terra_surfaces.size(), Ref<Terrain2DSurface>(NULL));

	return _terra_surfaces[index];
}

void Terrain2DLibraryMerger::terra_surface_add(Ref<Terrain2DSurface> value) {
	ERR_FAIL_COND(!value.is_valid());

	value->set_library(Ref<Terrain2DLibraryMerger>(this));
	value->set_id(_terra_surfaces.size());

	_terra_surfaces.push_back(value);
}

void Terrain2DLibraryMerger::terra_surface_set(const int index, Ref<Terrain2DSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_terra_surfaces.size() < index) {
		_terra_surfaces.resize(index + 1);
	}

	if (_terra_surfaces[index].is_valid()) {
		_terra_surfaces.get(index)->set_library(Ref<Terrain2DLibraryMerger>(NULL));
	}

	if (value.is_valid()) {
		value->set_library(Ref<Terrain2DLibraryMerger>(this));

		_terra_surfaces.set(index, value);
	}
}

void Terrain2DLibraryMerger::terra_surface_remove(const int index) {
	_terra_surfaces.remove(index);
}

int Terrain2DLibraryMerger::terra_surface_get_num() const {
	return _terra_surfaces.size();
}

void Terrain2DLibraryMerger::terra_surfaces_clear() {
	_packer->clear();

	for (int i = 0; i < _terra_surfaces.size(); i++) {
		Ref<Terrain2DSurfaceMerger> surface = _terra_surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(NULL);
		}
	}

	_terra_surfaces.clear();
}

Vector<Variant> Terrain2DLibraryMerger::get_terra_surfaces() {
	VARIANT_ARRAY_GET(_terra_surfaces);
}

void Terrain2DLibraryMerger::set_terra_surfaces(const Vector<Variant> &surfaces) {
	_terra_surfaces.clear();

	for (int i = 0; i < surfaces.size(); i++) {
		Ref<Terrain2DSurfaceMerger> surface = Ref<Terrain2DSurfaceMerger>(surfaces[i]);

		if (surface.is_valid()) {
			surface->set_library(Ref<Terrain2DLibraryMerger>(this));
		}

		_terra_surfaces.push_back(surface);
	}
}

#ifdef PROPS_2D_PRESENT
Ref<Prop2DData> Terrain2DLibraryMerger::get_prop(const int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<Prop2DData>());

	return _props[index];
}
void Terrain2DLibraryMerger::add_prop(Ref<Prop2DData> value) {
	_props.push_back(value);
}
bool Terrain2DLibraryMerger::has_prop(const Ref<Prop2DData> &value) const {
	return _props.find(value) != -1;
}
void Terrain2DLibraryMerger::set_prop(const int index, const Ref<Prop2DData> &value) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.write[index] = value;
}
void Terrain2DLibraryMerger::remove_prop(const int index) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.remove(index);
}
int Terrain2DLibraryMerger::get_num_props() const {
	return _props.size();
}
void Terrain2DLibraryMerger::clear_props() {
	_props.clear();
}

Vector<Variant> Terrain2DLibraryMerger::get_props() {
	VARIANT_ARRAY_GET(_props);
}

void Terrain2DLibraryMerger::set_props(const Vector<Variant> &props) {
	VARIANT_ARRAY_SET(props, _props, Prop2DData);
}

Rect2 Terrain2DLibraryMerger::get_prop_uv_rect(const Ref<Texture> &texture) {
	if (!texture.is_valid()) {
		return Rect2(0, 0, 1, 1);
	}

	Ref<AtlasTexture> at = _prop_packer->get_texture(texture);

	if (!at.is_valid()) {
		return Rect2(0, 0, 1, 1);
	}

	Rect2 region = at->get_region();

	Ref<Texture> tex = at->get_atlas();

	if (!tex.is_valid()) {
		return Rect2(0, 0, 1, 1);
	}

	Ref<Image> image = tex->get_data();

	if (!image.is_valid()) {
		return Rect2(0, 0, 1, 1);
	}

	float w = tex->get_width();
	float h = tex->get_height();

	region.position = Size2(region.position.x / w, region.position.y / h);
	region.size = Size2(region.size.x / w, region.size.y / h);

	return region;
}

Ref<TexturePacker> Terrain2DLibraryMerger::get_prop_packer() {
	return _prop_packer;
}
#endif

void Terrain2DLibraryMerger::refresh_rects() {
	bool texture_added = false;
	for (int i = 0; i < _terra_surfaces.size(); i++) {
		Ref<Terrain2DSurfaceMerger> surface = Ref<Terrain2DSurfaceMerger>(_terra_surfaces[i]);

		if (surface.is_valid()) {
			Ref<Texture> tex = surface->get_texture();

			if (!tex.is_valid())
				continue;

			if (!_packer->contains_texture(tex)) {
				texture_added = true;
				surface->set_region(_packer->add_texture(tex));
			} else {
				surface->set_region(_packer->get_texture(tex));
			}
		}
	}

	if (texture_added) {
		_packer->merge();

		ERR_FAIL_COND(_packer->get_texture_count() == 0);

		Ref<Texture> tex = _packer->get_generated_texture(0);

		texture_set(tex);
		liquid_texture_set(tex);

		setup_material_albedo(MATERIAL_INDEX_TERRAIN, tex);
		setup_material_albedo(MATERIAL_INDEX_LIQUID, tex);
	}

#ifdef PROPS_2D_PRESENT
	//todo add this back
	//texture_added = false;
	for (int i = 0; i < _props.size(); i++) {
		Ref<Prop2DData> prop = _props.get(i);

		if (prop.is_valid()) {
			if (process_prop_textures(prop))
				texture_added = true;
		}
	}

	//if (texture_added) {
	if (_prop_packer->get_texture_count() > 0) {
		_prop_packer->merge();

		//ERR_FAIL_COND(_prop_packer->get_texture_count() == 0);

		Ref<Texture> tex = _prop_packer->get_generated_texture(0);

		prop_texture_set(tex);

		setup_material_albedo(MATERIAL_INDEX_PROP, tex);
	}
#endif

	for (int i = 0; i < _terra_surfaces.size(); i++) {
		Ref<Terrain2DSurfaceMerger> surface = _terra_surfaces[i];

		if (surface.is_valid()) {
			surface->refresh_rects();
		}
	}

	set_initialized(true);
}

void Terrain2DLibraryMerger::_setup_material_albedo(const int material_index, const Ref<Texture> &texture) {
	Ref<ShaderMaterial> shmat;

	switch (material_index) {
		case MATERIAL_INDEX_TERRAIN:
			shmat = material_get();
			break;
		case MATERIAL_INDEX_LIQUID:
			shmat = liquid_material_get();
			break;
		case MATERIAL_INDEX_PROP:
			shmat = prop_material_get();
			break;
	}

	if (shmat.is_valid()) {
		shmat->set_shader_param("texture_albedo", texture);
	}
}

Terrain2DLibraryMerger::Terrain2DLibraryMerger() {
	_packer.instance();

#if GODOT4
#warning implement
#else
	_packer->set_texture_flags(Texture::FLAG_MIPMAPS | Texture::FLAG_FILTER);
#endif

	_packer->set_max_atlas_size(1024);
	_packer->set_keep_original_atlases(false);
	_packer->set_margin(0);

	_prop_packer.instance();

#if GODOT4
#warning implement
#else
	_prop_packer->set_texture_flags(Texture::FLAG_MIPMAPS | Texture::FLAG_FILTER);
#endif

	_prop_packer->set_max_atlas_size(1024);
	_prop_packer->set_keep_original_atlases(false);
	_prop_packer->set_margin(0);
}

Terrain2DLibraryMerger::~Terrain2DLibraryMerger() {
	for (int i = 0; i < _terra_surfaces.size(); ++i) {
		Ref<Terrain2DSurface> surface = _terra_surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(Ref<Terrain2DLibraryMerger>());
		}
	}

	_terra_surfaces.clear();

	_packer->clear();
	_packer.unref();

	_prop_packer->clear();
	_prop_packer.unref();
}

#ifdef PROPS_2D_PRESENT
bool Terrain2DLibraryMerger::process_prop_textures(Ref<Prop2DData> prop) {
	if (!prop.is_valid()) {
		return false;
	}

	bool texture_added = false;

	for (int i = 0; i < prop->get_prop_count(); ++i) {
#if MESH_DATA_RESOURCE_PRESENT
		Ref<Prop2DDataMeshData> pdm = prop->get_prop(i);

		if (pdm.is_valid()) {
			Ref<Texture> tex = pdm->get_texture();

			if (!tex.is_valid())
				continue;

			if (!_prop_packer->contains_texture(tex)) {
				_prop_packer->add_texture(tex);
				texture_added = true;
			}
		}
#endif

		Ref<Prop2DDataProp2D> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			if (process_prop_textures(pdp))
				texture_added = true;
		}
	}

	return texture_added;
}
#endif

void Terrain2DLibraryMerger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_flags"), &Terrain2DLibraryMerger::get_texture_flags);
	ClassDB::bind_method(D_METHOD("set_texture_flags", "flags"), &Terrain2DLibraryMerger::set_texture_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_flags", PROPERTY_HINT_FLAGS, "Mipmaps,Repeat,Filter,Anisotropic Linear,Convert to Linear,Mirrored Repeat,Video Surface"), "set_texture_flags", "get_texture_flags");

	ClassDB::bind_method(D_METHOD("get_max_atlas_size"), &Terrain2DLibraryMerger::get_max_atlas_size);
	ClassDB::bind_method(D_METHOD("set_max_atlas_size", "size"), &Terrain2DLibraryMerger::set_max_atlas_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_atlas_size"), "set_max_atlas_size", "get_max_atlas_size");

	ClassDB::bind_method(D_METHOD("get_keep_original_atlases"), &Terrain2DLibraryMerger::get_keep_original_atlases);
	ClassDB::bind_method(D_METHOD("set_keep_original_atlases", "value"), &Terrain2DLibraryMerger::set_keep_original_atlases);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_original_atlases"), "set_keep_original_atlases", "get_keep_original_atlases");

	ClassDB::bind_method(D_METHOD("get_background_color"), &Terrain2DLibraryMerger::get_background_color);
	ClassDB::bind_method(D_METHOD("set_background_color", "color"), &Terrain2DLibraryMerger::set_background_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_background_color", "get_background_color");

	ClassDB::bind_method(D_METHOD("get_margin"), &Terrain2DLibraryMerger::get_margin);
	ClassDB::bind_method(D_METHOD("set_margin", "size"), &Terrain2DLibraryMerger::set_margin);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin"), "set_margin", "get_margin");

	ClassDB::bind_method(D_METHOD("get_terra_surfaces"), &Terrain2DLibraryMerger::get_terra_surfaces);
	ClassDB::bind_method(D_METHOD("set_terra_surfaces"), &Terrain2DLibraryMerger::set_terra_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "terra_surfaces", PROPERTY_HINT_NONE, "17/17:Terrain2DSurfaceMerger", PROPERTY_USAGE_DEFAULT, "Terrain2DSurfaceMerger"), "set_terra_surfaces", "get_terra_surfaces");

#ifdef PROPS_2D_PRESENT
	ClassDB::bind_method(D_METHOD("get_props"), &Terrain2DLibraryMerger::get_props);
	ClassDB::bind_method(D_METHOD("set_props"), &Terrain2DLibraryMerger::set_props);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "props", PROPERTY_HINT_NONE, "17/17:Prop2DData", PROPERTY_USAGE_DEFAULT, "Prop2DData"), "set_props", "get_props");

	ClassDB::bind_method(D_METHOD("get_prop_uv_rect", "texture"), &Terrain2DLibraryMerger::get_prop_uv_rect);

	ClassDB::bind_method(D_METHOD("get_prop_packer"), &Terrain2DLibraryMerger::get_prop_packer);
#endif

	ClassDB::bind_method(D_METHOD("_setup_material_albedo", "material_index", "texture"), &Terrain2DLibraryMerger::_setup_material_albedo);
}
