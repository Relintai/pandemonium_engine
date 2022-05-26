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

#include "tiled_wall_data.h"

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

#include "../material_cache/prop_material_cache.h"

const String TiledWallData::BINDING_STRING_TILED_WALL_TILING_TYPE = "None,Horizontal,Vertical,Both";

TiledWallData::TiledWallTilingType TiledWallData::get_tiling_type() const {
	return _tiling_type;
}
void TiledWallData::set_tiling_type(const TiledWallData::TiledWallTilingType value) {
	_tiling_type = value;

	emit_changed();
}

void TiledWallData::add_tile(const Ref<Texture> &texture, const Vector2 &val, const float z_offset) {
	_textures.push_back(texture);
}
void TiledWallData::remove_tile(const int index) {
	ERR_FAIL_INDEX(index, _textures.size());

	_textures.remove(index);

	emit_changed();
}

Ref<Texture> TiledWallData::get_tile_texture(const int index) const {
	ERR_FAIL_INDEX_V(index, _textures.size(), Ref<Texture>());

	return _textures.get(index).texture;
}
void TiledWallData::set_tile_texture(const int index, const Ref<Texture> &texture) {
	ERR_FAIL_INDEX(index, _textures.size());

	_textures.write[index].texture = texture;

	emit_changed();
}

Vector2 TiledWallData::get_tile_texture_size(const int index) const {
	ERR_FAIL_INDEX_V(index, _textures.size(), Vector2(1, 1));

	return _textures.get(index).size;
}
void TiledWallData::set_tile_texture_size(const int index, const Vector2 &val) {
	ERR_FAIL_INDEX(index, _textures.size());

	_textures.write[index].size = val;

	emit_changed();
}

float TiledWallData::get_tile_texture_z_offset(const int index) const {
	ERR_FAIL_INDEX_V(index, _textures.size(), 0);

	return _textures.get(index).z_offset;
}
void TiledWallData::set_tile_texture_z_offset(const int index, const float val) {
	ERR_FAIL_INDEX(index, _textures.size());

	_textures.write[index].z_offset = val;

	emit_changed();
}

int TiledWallData::get_tile_count() const {
	return _textures.size();
}

void TiledWallData::set_tile_count(const int count) {
	_textures.resize(count);
}

//flavour_textures
void TiledWallData::add_flavour_tile(const Ref<Texture> &texture, const Vector2 &val, const float z_offset) {
	_flavour_textures.push_back(texture);
}
void TiledWallData::remove_flavour_tile(const int index) {
	ERR_FAIL_INDEX(index, _flavour_textures.size());

	_flavour_textures.remove(index);

	emit_changed();
}

Ref<Texture> TiledWallData::get_flavour_tile_texture(const int index) const {
	ERR_FAIL_INDEX_V(index, _flavour_textures.size(), Ref<Texture>());

	return _flavour_textures.get(index).texture;
}
void TiledWallData::set_flavour_tile_texture(const int index, const Ref<Texture> &texture) {
	ERR_FAIL_INDEX(index, _flavour_textures.size());

	_flavour_textures.write[index].texture = texture;

	emit_changed();
}

Vector2 TiledWallData::get_flavour_tile_texture_size(const int index) const {
	ERR_FAIL_INDEX_V(index, _flavour_textures.size(), Vector2(1, 1));

	return _flavour_textures.get(index).size;
}
void TiledWallData::set_flavour_tile_texture_size(const int index, const Vector2 &val) {
	ERR_FAIL_INDEX(index, _flavour_textures.size());

	_flavour_textures.write[index].size = val;

	emit_changed();
}

float TiledWallData::get_flavour_tile_texture_z_offset(const int index) const {
	ERR_FAIL_INDEX_V(index, _flavour_textures.size(), 0);

	return _flavour_textures.get(index).z_offset;
}
void TiledWallData::set_flavour_tile_texture_z_offset(const int index, const float val) {
	ERR_FAIL_INDEX(index, _flavour_textures.size());

	_flavour_textures.write[index].z_offset = val;

	emit_changed();
}

int TiledWallData::get_flavour_tile_count() const {
	return _flavour_textures.size();
}

void TiledWallData::set_flavour_tile_count(const int count) {
	_flavour_textures.resize(count);
}

float TiledWallData::get_flavour_tile_chance() const {
	return _flavour_chance;
}
void TiledWallData::set_flavour_tile_chance(const float value) {
	_flavour_chance = value;
}

//materials
void TiledWallData::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

void TiledWallData::material_set(const int index, const Ref<Material> &value) {
	ERR_FAIL_INDEX(index, _materials.size());

	_materials.set(index, value);

	emit_changed();
}

void TiledWallData::material_remove(const int index) {
	_materials.remove(index);

	emit_changed();
}

int TiledWallData::material_get_num() const {
	return _materials.size();
}

void TiledWallData::materials_clear() {
	_materials.clear();
}

Vector<Variant> TiledWallData::materials_get() {
	VARIANT_ARRAY_GET(_materials);
}

void TiledWallData::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}

	emit_changed();
}

#if TEXTURE_PACKER_PRESENT
void TiledWallData::add_textures_into(Ref<TexturePacker> texture_packer) {
	ERR_FAIL_COND(!texture_packer.is_valid());

	for (int i = 0; i < _textures.size(); ++i) {
		Ref<Texture> entry = _textures.get(i).texture;

		if (entry.is_valid()) {
			texture_packer->add_texture(entry);
		}
	}

	for (int i = 0; i < _flavour_textures.size(); ++i) {
		Ref<Texture> entry = _flavour_textures.get(i).texture;

		if (entry.is_valid()) {
			texture_packer->add_texture(entry);
		}
	}
}
#endif

void TiledWallData::setup_cache(Ref<PropMaterialCache> cache) {
	//Note: the caller should lock and unlock the cache!

	call("_setup_cache", cache);
}
void TiledWallData::_setup_cache(Ref<PropMaterialCache> cache) {
	if (cache->material_get_num() == 0) {
		for (int i = 0; i < _materials.size(); ++i) {
			const Ref<Material> &m = _materials[i];

			if (m.is_valid()) {
				Ref<Material> nm = m->duplicate();

				cache->material_add(nm);
			}
		}
	}

	for (int i = 0; i < _textures.size(); ++i) {
		const Ref<Texture> &t = _textures[i].texture;

		if (t.is_valid()) {
			cache->texture_add(t);
		}
	}

	for (int i = 0; i < _flavour_textures.size(); ++i) {
		const Ref<Texture> &t = _flavour_textures[i].texture;

		if (t.is_valid()) {
			cache->texture_add(t);
		}
	}
}

void TiledWallData::copy_from(const Ref<TiledWallData> &tiled_wall_data) {
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

TiledWallData::TiledWallData() {
	_tiling_type = TILED_WALL_TILING_TYPE_NONE;
	_flavour_chance = 0.15;
}
TiledWallData::~TiledWallData() {
	_textures.clear();
	_flavour_textures.clear();
	_materials.clear();
}

bool TiledWallData::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name.begins_with("tile_")) {
		int index = name.get_slicec('_', 1).to_int();

		if (index >= _textures.size()) {
			_textures.resize(index + 1);
		}

		StringName p = name.get_slicec('/', 1);

		if (p == "texture") {
			_textures.write[index].texture = p_value;

			return true;
		} else if (p == "size") {
			_textures.write[index].size = p_value;

			return true;
		} else if (p == "z_offset") {
			_textures.write[index].z_offset = p_value;

			return true;
		}
	} else if (name.begins_with("flavour_tile_")) {
		int index = name.get_slicec('_', 1).to_int();

		if (index >= _flavour_textures.size()) {
			_flavour_textures.resize(index + 1);
		}

		StringName p = name.get_slicec('/', 1);

		if (p == "texture") {
			_flavour_textures.write[index].texture = p_value;

			return true;
		} else if (p == "size") {
			_flavour_textures.write[index].size = p_value;

			return true;
		} else if (p == "z_offset") {
			_flavour_textures.write[index].z_offset = p_value;

			return true;
		}
	}

	return false;
}

bool TiledWallData::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name.begins_with("tile_")) {
		int index = name.get_slicec('_', 1).to_int();

		if (index >= _textures.size()) {
			return false;
		}

		StringName p = name.get_slicec('/', 1);

		if (p == "texture") {
			r_ret = _textures[index].texture;

			return true;
		} else if (p == "size") {
			r_ret = _textures[index].size;

			return true;
		} else if (p == "z_offset") {
			r_ret = _textures[index].z_offset;

			return true;
		}
	} else if (name.begins_with("flavour_tile_")) {
		int index = name.get_slicec('_', 1).to_int();

		if (index >= _flavour_textures.size()) {
			return false;
		}

		StringName p = name.get_slicec('/', 1);

		if (p == "texture") {
			r_ret = _flavour_textures[index].texture;

			return true;
		} else if (p == "size") {
			r_ret = _flavour_textures[index].size;

			return true;
		} else if (p == "z_offset") {
			r_ret = _flavour_textures[index].z_offset;

			return true;
		}
	}

	return false;
}

void TiledWallData::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < _textures.size(); ++i) {
		p_list->push_back(PropertyInfo(Variant::OBJECT, "tile_" + itos(i) + "/texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture", PROPERTY_USAGE_DEFAULT));
		p_list->push_back(PropertyInfo(Variant::VECTOR2, "tile_" + itos(i) + "/size", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT));
		p_list->push_back(PropertyInfo(Variant::REAL, "tile_" + itos(i) + "/z_offset", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT));
	}

	for (int i = 0; i < _flavour_textures.size(); ++i) {
		p_list->push_back(PropertyInfo(Variant::OBJECT, "flavour_tile_" + itos(i) + "/texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture", PROPERTY_USAGE_DEFAULT));
		p_list->push_back(PropertyInfo(Variant::VECTOR2, "flavour_tile_" + itos(i) + "/size", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT));
		p_list->push_back(PropertyInfo(Variant::REAL, "flavour_tile_" + itos(i) + "/z_offset", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT));
	}
}

void TiledWallData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_tiling_type"), &TiledWallData::get_tiling_type);
	ClassDB::bind_method(D_METHOD("set_tiling_type", "texture"), &TiledWallData::set_tiling_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tiling_type", PROPERTY_HINT_ENUM, TiledWallData::BINDING_STRING_TILED_WALL_TILING_TYPE), "set_tiling_type", "get_tiling_type");

	//textures
	ClassDB::bind_method(D_METHOD("add_tile", "texture", "size", "z_offset"), &TiledWallData::add_tile, Vector2(1, 1), 0);
	ClassDB::bind_method(D_METHOD("remove_tile", "index"), &TiledWallData::remove_tile);

	ClassDB::bind_method(D_METHOD("get_tile_texture", "index"), &TiledWallData::get_tile_texture);
	ClassDB::bind_method(D_METHOD("set_tile_texture", "index", "texture"), &TiledWallData::set_tile_texture);

	ClassDB::bind_method(D_METHOD("get_tile_texture_size", "index"), &TiledWallData::get_tile_texture_size);
	ClassDB::bind_method(D_METHOD("set_tile_texture_size", "index", "val"), &TiledWallData::set_tile_texture_size);

	ClassDB::bind_method(D_METHOD("get_tile_texture_z_offset", "index"), &TiledWallData::get_tile_texture_z_offset);
	ClassDB::bind_method(D_METHOD("set_tile_texture_z_offset", "index", "val"), &TiledWallData::set_tile_texture_z_offset);

	ClassDB::bind_method(D_METHOD("get_tile_count"), &TiledWallData::get_tile_count);
	ClassDB::bind_method(D_METHOD("set_tile_count", "count"), &TiledWallData::set_tile_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "set_tile_count", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_tile_count", "get_tile_count");

	//flavour_textures
	ClassDB::bind_method(D_METHOD("add_flavour_tile", "texture", "size", "z_offset"), &TiledWallData::add_flavour_tile, Vector2(1, 1), 0);
	ClassDB::bind_method(D_METHOD("remove_flavour_tile", "index"), &TiledWallData::remove_flavour_tile);

	ClassDB::bind_method(D_METHOD("get_flavour_tile_texture", "index"), &TiledWallData::get_flavour_tile_texture);
	ClassDB::bind_method(D_METHOD("set_flavour_tile_texture", "index", "texture"), &TiledWallData::set_flavour_tile_texture);

	ClassDB::bind_method(D_METHOD("get_flavour_tile_texture_size", "index"), &TiledWallData::get_flavour_tile_texture_size);
	ClassDB::bind_method(D_METHOD("set_flavour_tile_texture_size", "index", "val"), &TiledWallData::set_flavour_tile_texture_size);

	ClassDB::bind_method(D_METHOD("get_flavour_tile_texture_z_offset", "index"), &TiledWallData::get_flavour_tile_texture_z_offset);
	ClassDB::bind_method(D_METHOD("set_flavour_tile_texture_z_offset", "index", "val"), &TiledWallData::set_flavour_tile_texture_z_offset);

	ClassDB::bind_method(D_METHOD("get_flavour_tile_count"), &TiledWallData::get_flavour_tile_count);
	ClassDB::bind_method(D_METHOD("set_flavour_tile_count", "count"), &TiledWallData::set_flavour_tile_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "flavour_tile_count", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_flavour_tile_count", "get_flavour_tile_count");

	ClassDB::bind_method(D_METHOD("get_flavour_tile_chance"), &TiledWallData::get_flavour_tile_chance);
	ClassDB::bind_method(D_METHOD("set_flavour_tile_chance", "texture"), &TiledWallData::set_flavour_tile_chance);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "flavour_tile_chance"), "set_flavour_tile_chance", "get_flavour_tile_chance");

	//materials
	ClassDB::bind_method(D_METHOD("material_add", "value"), &TiledWallData::material_add);
	ClassDB::bind_method(D_METHOD("material_set", "index", "value"), &TiledWallData::material_set);
	ClassDB::bind_method(D_METHOD("material_remove", "index"), &TiledWallData::material_remove);
	ClassDB::bind_method(D_METHOD("material_get_num"), &TiledWallData::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &TiledWallData::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &TiledWallData::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &TiledWallData::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "17/17:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

#if TEXTURE_PACKER_PRESENT
	ClassDB::bind_method(D_METHOD("add_textures_into", "texture_packer"), &TiledWallData::add_textures_into);
#endif

	BIND_VMETHOD(MethodInfo("_setup_cache", PropertyInfo(Variant::OBJECT, "cache", PROPERTY_HINT_RESOURCE_TYPE, "PropMaterialCache")));

	ClassDB::bind_method(D_METHOD("setup_cache", "cache"), &TiledWallData::setup_cache);
	ClassDB::bind_method(D_METHOD("_setup_cache", "cache"), &TiledWallData::_setup_cache);

	ClassDB::bind_method(D_METHOD("copy_from", "prop_data"), &TiledWallData::copy_from);

	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_NONE);
	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_HORIZONTAL);
	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_VERTICAL);
	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_BOTH);
}
