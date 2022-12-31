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

#include "tiled_wall_2d_data.h"

#include "servers/physics_server.h"

#define VARIANT_ARRAY_GET(arr)             \
	Vector<Variant> r;                     \
	for (int i = 0; i < arr.size(); i++) { \
		r.push_back(arr[i]);               \
	}                                      \
	return r;

#define VARIANT_ARRAY_SET(arr, arr_into, type) \
	arr_into.clear();                          \
	for (int i = 0; i < arr.size(); i++) {     \
		Ref<type> e = Ref<type>(arr[i]);       \
		arr_into.push_back(e);                 \
	}

#include "../material_cache/prop_2d_material_cache.h"

#include "modules/modules_enabled.gen.h"

const String TiledWall2DData::BINDING_STRING_TILED_WALL_TILING_TYPE = "None,Horizontal,Vertical,Both";

TiledWall2DData::TiledWall2DTilingType TiledWall2DData::get_tiling_type() const {
	return _tiling_type;
}
void TiledWall2DData::set_tiling_type(const TiledWall2DData::TiledWall2DTilingType value) {
	_tiling_type = value;

	emit_changed();
}

Ref<Texture> TiledWall2DData::get_texture(const int index) const {
	ERR_FAIL_INDEX_V(index, _textures.size(), Ref<Texture>());

	return _textures.get(index);
}
void TiledWall2DData::set_texture(const int index, const Ref<Texture> &texture) {
	ERR_FAIL_INDEX(index, _textures.size());

	_textures.set(index, texture);

	emit_changed();
}
void TiledWall2DData::add_texture(const Ref<Texture> &texture) {
	_textures.push_back(texture);
}
void TiledWall2DData::remove_texture(const int index) {
	ERR_FAIL_INDEX(index, _textures.size());

	_textures.remove(index);

	emit_changed();
}

int TiledWall2DData::get_texture_count() const {
	return _textures.size();
}

Vector<Variant> TiledWall2DData::get_textures() {
	Vector<Variant> r;
	for (int i = 0; i < _textures.size(); i++) {
		r.push_back(_textures[i].get_ref_ptr());
	}
	return r;
}
void TiledWall2DData::set_textures(const Vector<Variant> &textures) {
	_textures.clear();
	for (int i = 0; i < textures.size(); i++) {
		Ref<Texture> tex = Ref<Texture>(textures[i]);

		_textures.push_back(tex);
	}
}

//flavour_textures
Ref<Texture> TiledWall2DData::get_flavour_texture(const int index) const {
	ERR_FAIL_INDEX_V(index, _flavour_textures.size(), Ref<Texture>());

	return _flavour_textures.get(index);
}
void TiledWall2DData::set_flavour_texture(const int index, const Ref<Texture> &texture) {
	ERR_FAIL_INDEX(index, _flavour_textures.size());

	_flavour_textures.set(index, texture);

	emit_changed();
}
void TiledWall2DData::add_flavour_texture(const Ref<Texture> &texture) {
	_flavour_textures.push_back(texture);
}
void TiledWall2DData::remove_flavour_texture(const int index) {
	ERR_FAIL_INDEX(index, _flavour_textures.size());

	_flavour_textures.remove(index);

	emit_changed();
}

int TiledWall2DData::get_flavour_texture_count() const {
	return _flavour_textures.size();
}

Vector<Variant> TiledWall2DData::get_flavour_textures() {
	Vector<Variant> r;
	for (int i = 0; i < _flavour_textures.size(); i++) {
		r.push_back(_flavour_textures[i].get_ref_ptr());
	}
	return r;
}
void TiledWall2DData::set_flavour_textures(const Vector<Variant> &textures) {
	_flavour_textures.clear();
	for (int i = 0; i < textures.size(); i++) {
		Ref<Texture> tex = Ref<Texture>(textures[i]);

		_flavour_textures.push_back(tex);
	}

	emit_changed();
}

float TiledWall2DData::get_flavour_chance() const {
	return _flavour_chance;
}
void TiledWall2DData::set_flavour_chance(const float value) {
	_flavour_chance = value;
}

//materials

Ref<Material> TiledWall2DData::material_get() {
	return _material;
}

void TiledWall2DData::material_set(const Ref<Material> &value) {
	_material = value;

	emit_changed();
}

#ifdef MODULE_TEXTURE_PACKER_ENABLED
void TiledWall2DData::add_textures_into(Ref<TexturePacker> texture_packer) {
	ERR_FAIL_COND(!texture_packer.is_valid());

	for (int i = 0; i < _textures.size(); ++i) {
		Ref<Texture> entry = _textures.get(i);

		if (entry.is_valid()) {
			texture_packer->add_texture(entry);
		}
	}

	for (int i = 0; i < _flavour_textures.size(); ++i) {
		Ref<Texture> entry = _flavour_textures.get(i);

		if (entry.is_valid()) {
			texture_packer->add_texture(entry);
		}
	}
}
#endif

void TiledWall2DData::setup_cache(Ref<Prop2DMaterialCache> cache) {
	//Note: the caller should lock and unlock the cache!

	call("_setup_cache", cache);
}
void TiledWall2DData::_setup_cache(Ref<Prop2DMaterialCache> cache) {
	if (_material.is_valid() && !cache->material_get().is_valid()) {
		Ref<Material> nm = _material->duplicate();

		cache->material_set(nm);
	}

	for (int i = 0; i < _textures.size(); ++i) {
		const Ref<Texture> &t = _textures[i];

		if (t.is_valid()) {
			cache->texture_add(t);
		}
	}

	for (int i = 0; i < _flavour_textures.size(); ++i) {
		const Ref<Texture> &t = _flavour_textures[i];

		if (t.is_valid()) {
			cache->texture_add(t);
		}
	}
}

void TiledWall2DData::copy_from(const Ref<TiledWall2DData> &tiled_wall_data) {
	ERR_FAIL_COND(!tiled_wall_data.is_valid());

	_tiling_type = tiled_wall_data->_tiling_type;

	_textures.clear();

	for (int i = 0; i < tiled_wall_data->_textures.size(); ++i) {
		_textures.push_back(tiled_wall_data->_textures[i]);
	}

	_flavour_textures.clear();

	for (int i = 0; i < tiled_wall_data->_flavour_textures.size(); ++i) {
		_flavour_textures.push_back(tiled_wall_data->_flavour_textures[i]);
	}

	emit_changed();
}

TiledWall2DData::TiledWall2DData() {
	_tiling_type = TILED_WALL_TILING_TYPE_NONE;
	_flavour_chance = 0.15;
}
TiledWall2DData::~TiledWall2DData() {
	_textures.clear();
	_flavour_textures.clear();
	_material.unref();
}

void TiledWall2DData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_tiling_type"), &TiledWall2DData::get_tiling_type);
	ClassDB::bind_method(D_METHOD("set_tiling_type", "texture"), &TiledWall2DData::set_tiling_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tiling_type", PROPERTY_HINT_ENUM, TiledWall2DData::BINDING_STRING_TILED_WALL_TILING_TYPE), "set_tiling_type", "get_tiling_type");

	//textures
	ClassDB::bind_method(D_METHOD("get_texture", "index"), &TiledWall2DData::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "index", "texture"), &TiledWall2DData::set_texture);
	ClassDB::bind_method(D_METHOD("add_texture", "texture"), &TiledWall2DData::add_texture);
	ClassDB::bind_method(D_METHOD("remove_texture", "index"), &TiledWall2DData::remove_texture);

	ClassDB::bind_method(D_METHOD("get_texture_count"), &TiledWall2DData::get_texture_count);

	ClassDB::bind_method(D_METHOD("get_textures"), &TiledWall2DData::get_textures);
	ClassDB::bind_method(D_METHOD("set_textures", "textures"), &TiledWall2DData::set_textures);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "textures", PROPERTY_HINT_NONE, "23/19:Texture", PROPERTY_USAGE_DEFAULT, "Texture"), "set_textures", "get_textures");

	//flavour_textures
	ClassDB::bind_method(D_METHOD("get_flavour_texture", "index"), &TiledWall2DData::get_flavour_texture);
	ClassDB::bind_method(D_METHOD("set_flavour_texture", "index", "texture"), &TiledWall2DData::set_flavour_texture);
	ClassDB::bind_method(D_METHOD("add_tflavour_exture", "texture"), &TiledWall2DData::add_flavour_texture);
	ClassDB::bind_method(D_METHOD("remove_flavour_texture", "index"), &TiledWall2DData::remove_flavour_texture);

	ClassDB::bind_method(D_METHOD("get_flavour_texture_count"), &TiledWall2DData::get_flavour_texture_count);

	ClassDB::bind_method(D_METHOD("get_flavour_textures"), &TiledWall2DData::get_flavour_textures);
	ClassDB::bind_method(D_METHOD("set_flavour_textures", "textures"), &TiledWall2DData::set_flavour_textures);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "flavour_textures", PROPERTY_HINT_NONE, "23/19:Texture", PROPERTY_USAGE_DEFAULT, "Texture"), "set_flavour_textures", "get_flavour_textures");

	ClassDB::bind_method(D_METHOD("get_flavour_chance"), &TiledWall2DData::get_flavour_chance);
	ClassDB::bind_method(D_METHOD("set_flavour_chance", "texture"), &TiledWall2DData::set_flavour_chance);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "flavour_chance"), "set_flavour_chance", "get_flavour_chance");

	//materials
	ClassDB::bind_method(D_METHOD("material_get"), &TiledWall2DData::material_get);
	ClassDB::bind_method(D_METHOD("material_set", "value"), &TiledWall2DData::material_set);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "material_set", "material_get");

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	ClassDB::bind_method(D_METHOD("add_textures_into", "texture_packer"), &TiledWall2DData::add_textures_into);
#endif

	BIND_VMETHOD(MethodInfo("_setup_cache", PropertyInfo(Variant::OBJECT, "cache", PROPERTY_HINT_RESOURCE_TYPE, "Prop2DMaterialCache")));

	ClassDB::bind_method(D_METHOD("setup_cache", "cache"), &TiledWall2DData::setup_cache);
	ClassDB::bind_method(D_METHOD("_setup_cache", "cache"), &TiledWall2DData::_setup_cache);

	ClassDB::bind_method(D_METHOD("copy_from", "prop_data"), &TiledWall2DData::copy_from);

	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_NONE);
	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_HORIZONTAL);
	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_VERTICAL);
	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_BOTH);
}
