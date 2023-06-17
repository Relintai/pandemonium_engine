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

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_FASTNOISE_ENABLED
#include "../../fastnoise/fastnoise_noise_params.h"
#endif

#ifdef MODULE_ENTITY_SPELL_SYSTEM_ENABLED
#include "modules/entity_spell_system/material_cache/ess_material_cache.h"
#endif

const String TiledWallData::BINDING_STRING_TILED_WALL_TILING_TYPE = "None,Horizontal,Vertical,Both";
const String TiledWallData::BINDING_STRING_TILED_WALL_COLLIDER_TYPE = "None,Plane,Box,Convex Mesh,Concave Mesh";

TiledWallData::TiledWallTilingType TiledWallData::get_tiling_type() const {
	return _tiling_type;
}
void TiledWallData::set_tiling_type(const TiledWallData::TiledWallTilingType value) {
	_tiling_type = value;

	emit_changed();
}

TiledWallData::TiledWallColliderType TiledWallData::get_collider_type() const {
	return _collider_type;
}
void TiledWallData::set_collider_type(const TiledWallData::TiledWallColliderType value) {
	_collider_type = value;

	emit_changed();
}

float TiledWallData::get_collider_z_offset() {
	return _collider_z_offset;
}
void TiledWallData::set_collider_z_offset(const float val) {
	_collider_z_offset = val;
}

void TiledWallData::add_tile(const Ref<Texture> &texture, const float y_size, const float z_offset, const int texture_scale) {
	_tiles.push_back(TextureEntry(texture, y_size, z_offset, texture_scale));
}
void TiledWallData::remove_tile(const int index) {
	ERR_FAIL_INDEX(index, _tiles.size());

	_tiles.remove(index);

	emit_changed();
}

TiledWallData::TextureEntry TiledWallData::get_tile(const int index) const {
	ERR_FAIL_INDEX_V(index, _tiles.size(), TextureEntry());

	return _tiles[index];
}

Ref<Texture> TiledWallData::get_tile_texture(const int index) const {
	ERR_FAIL_INDEX_V(index, _tiles.size(), Ref<Texture>());

	return _tiles.get(index).texture;
}
void TiledWallData::set_tile_texture(const int index, const Ref<Texture> &texture) {
	ERR_FAIL_INDEX(index, _tiles.size());

	_tiles.write[index].texture = texture;

	emit_changed();
}

float TiledWallData::get_tile_y_size(const int index) const {
	ERR_FAIL_INDEX_V(index, _tiles.size(), 1);

	return _tiles.get(index).y_size;
}
void TiledWallData::set_tile_y_size(const int index, const float val) {
	ERR_FAIL_INDEX(index, _tiles.size());

	_tiles.write[index].y_size = val;

	emit_changed();
}

float TiledWallData::get_tile_z_offset(const int index) const {
	ERR_FAIL_INDEX_V(index, _tiles.size(), 0);

	return _tiles.get(index).z_offset;
}
void TiledWallData::set_tile_z_offset(const int index, const float val) {
	ERR_FAIL_INDEX(index, _tiles.size());

	_tiles.write[index].z_offset = val;

	emit_changed();
}

int TiledWallData::get_tile_texture_scale(const int index) const {
	ERR_FAIL_INDEX_V(index, _tiles.size(), 0);

	return _tiles.get(index).texture_scale;
}
void TiledWallData::set_tile_texture_scale(const int index, const int val) {
	ERR_FAIL_INDEX(index, _tiles.size());

	_tiles.write[index].texture_scale = val;

	emit_changed();
}

int TiledWallData::get_tile_count() const {
	return _tiles.size();
}

void TiledWallData::set_tile_count(const int count) {
	_tiles.resize(count);
}

//flavour_tiles
void TiledWallData::add_flavour_tile(const Ref<Texture> &texture, const float y_size, const float z_offset, const int texture_scale) {
	_flavour_tiles.push_back(TextureEntry(texture, y_size, z_offset, texture_scale));
}
void TiledWallData::remove_flavour_tile(const int index) {
	ERR_FAIL_INDEX(index, _flavour_tiles.size());

	_flavour_tiles.remove(index);

	emit_changed();
}
TiledWallData::TextureEntry TiledWallData::get_flavour_tile(const int index) const {
	ERR_FAIL_INDEX_V(index, _flavour_tiles.size(), TextureEntry());

	return _flavour_tiles[index];
}

Ref<Texture> TiledWallData::get_flavour_tile_texture(const int index) const {
	ERR_FAIL_INDEX_V(index, _flavour_tiles.size(), Ref<Texture>());

	return _flavour_tiles.get(index).texture;
}
void TiledWallData::set_flavour_tile_texture(const int index, const Ref<Texture> &texture) {
	ERR_FAIL_INDEX(index, _flavour_tiles.size());

	_flavour_tiles.write[index].texture = texture;

	emit_changed();
}

float TiledWallData::get_flavour_tile_y_size(const int index) const {
	ERR_FAIL_INDEX_V(index, _flavour_tiles.size(), 1);

	return _flavour_tiles.get(index).y_size;
}
void TiledWallData::set_flavour_tile_y_size(const int index, const float val) {
	ERR_FAIL_INDEX(index, _flavour_tiles.size());

	_flavour_tiles.write[index].y_size = val;

	emit_changed();
}

float TiledWallData::get_flavour_tile_z_offset(const int index) const {
	ERR_FAIL_INDEX_V(index, _flavour_tiles.size(), 0);

	return _flavour_tiles.get(index).z_offset;
}
void TiledWallData::set_flavour_tile_z_offset(const int index, const float val) {
	ERR_FAIL_INDEX(index, _flavour_tiles.size());

	_flavour_tiles.write[index].z_offset = val;

	emit_changed();
}

int TiledWallData::get_flavour_tile_texture_scale(const int index) const {
	ERR_FAIL_INDEX_V(index, _flavour_tiles.size(), 0);

	return _flavour_tiles.get(index).texture_scale;
}
void TiledWallData::set_flavour_tile_texture_scale(const int index, const int val) {
	ERR_FAIL_INDEX(index, _flavour_tiles.size());

	_flavour_tiles.write[index].texture_scale = val;

	emit_changed();
}

int TiledWallData::get_flavour_tile_count() const {
	return _flavour_tiles.size();
}

void TiledWallData::set_flavour_tile_count(const int count) {
	_flavour_tiles.resize(count);
}

float TiledWallData::get_flavour_tile_chance() const {
	return _flavour_chance;
}
void TiledWallData::set_flavour_tile_chance(const float value) {
	_flavour_chance = value;
}

#ifdef MODULE_FASTNOISE_ENABLED
Ref<FastnoiseNoiseParams> TiledWallData::get_offset_noise() {
	return _offset_noise;
}
void TiledWallData::set_offset_noise(const Ref<FastnoiseNoiseParams> &val) {
	_offset_noise = val;

	emit_changed();
}

float TiledWallData::get_offset_noise_strength() const {
	return _offset_noise_strength;
}
void TiledWallData::set_offset_noise_strength(const float val) {
	_offset_noise_strength = val;

	emit_changed();
}

bool TiledWallData::get_offset_noise_randomize_seed() const {
	return _offset_noise_randomize_seed;
}
void TiledWallData::set_offset_noise_randomize_seed(const bool val) {
	_offset_noise_randomize_seed = val;

	emit_changed();
}

bool TiledWallData::get_offset_noise_skip_edges() const {
	return _offset_noise_skip_edges;
}
void TiledWallData::set_offset_noise_skip_edges(const bool val) {
	_offset_noise_skip_edges = val;

	emit_changed();
}
#endif

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

#ifdef MODULE_TEXTURE_PACKER_ENABLED
void TiledWallData::add_textures_into(Ref<TexturePacker> texture_packer) {
	ERR_FAIL_COND(!texture_packer.is_valid());

	for (int i = 0; i < _tiles.size(); ++i) {
		Ref<Texture> entry = _tiles.get(i).texture;

		if (entry.is_valid()) {
			texture_packer->add_texture(entry);
		}
	}

	for (int i = 0; i < _flavour_tiles.size(); ++i) {
		Ref<Texture> entry = _flavour_tiles.get(i).texture;

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

	for (int i = 0; i < _tiles.size(); ++i) {
		const Ref<Texture> &t = _tiles[i].texture;

		if (t.is_valid()) {
			cache->texture_add(t);
		}
	}

	for (int i = 0; i < _flavour_tiles.size(); ++i) {
		const Ref<Texture> &t = _flavour_tiles[i].texture;

		if (t.is_valid()) {
			cache->texture_add(t);
		}
	}
}

#ifdef MODULE_ENTITY_SPELL_SYSTEM_ENABLED
void TiledWallData::setup_ess_cache(Ref<ESSMaterialCache> cache) {
	//Note: the caller should lock and unlock the cache!

	call("_setup_ess_cache", cache);
}
void TiledWallData::_setup_ess_cache(Ref<ESSMaterialCache> cache) {
	if (cache->material_get_num() == 0) {
		for (int i = 0; i < _materials.size(); ++i) {
			const Ref<Material> &m = _materials[i];

			if (m.is_valid()) {
				Ref<Material> nm = m->duplicate();

				cache->material_add(nm);
			}
		}
	}

	for (int i = 0; i < _tiles.size(); ++i) {
		const Ref<Texture> &t = _tiles[i].texture;

		if (t.is_valid()) {
			cache->texture_add(t);
		}
	}

	for (int i = 0; i < _flavour_tiles.size(); ++i) {
		const Ref<Texture> &t = _flavour_tiles[i].texture;

		if (t.is_valid()) {
			cache->texture_add(t);
		}
	}
}
#endif

void TiledWallData::copy_from(const Ref<TiledWallData> &tiled_wall_data) {
	ERR_FAIL_COND(!tiled_wall_data.is_valid());

	_tiling_type = tiled_wall_data->_tiling_type;

	_tiles.clear();

	for (int i = 0; i < tiled_wall_data->_tiles.size(); ++i) {
		_tiles.push_back(tiled_wall_data->_tiles[i]);
	}

	_flavour_tiles.clear();

	for (int i = 0; i < tiled_wall_data->_flavour_tiles.size(); ++i) {
		_flavour_tiles.push_back(tiled_wall_data->_flavour_tiles[i]);
	}

	emit_changed();
}

TiledWallData::TiledWallData() {
	_tiling_type = TILED_WALL_TILING_TYPE_NONE;
	_collider_type = TILED_WALL_COLLIDER_TYPE_PLANE;
	_flavour_chance = 0.15;
	_collider_z_offset = 0;
#ifdef MODULE_FASTNOISE_ENABLED
	_offset_noise_strength = 0;
	_offset_noise_randomize_seed = true;
	_offset_noise_skip_edges = true;
#endif
}
TiledWallData::~TiledWallData() {
	_tiles.clear();
	_flavour_tiles.clear();
	_materials.clear();
}

bool TiledWallData::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;

	if (name.begins_with("tile_")) {
		int index = name.get_slicec('_', 1).to_int();

		if (index >= _tiles.size()) {
			_tiles.resize(index + 1);
		}

		StringName p = name.get_slicec('/', 1);

		if (p == "texture") {
			_tiles.write[index].texture = p_value;

			return true;
		} else if (p == "y_size") {
			_tiles.write[index].y_size = p_value;

			return true;
		} else if (p == "z_offset") {
			_tiles.write[index].z_offset = p_value;

			return true;
		} else if (p == "texture_scale") {
			_tiles.write[index].texture_scale = p_value;

			return true;
		}
	} else if (name.begins_with("flavour_tile_")) {
		int index = name.get_slicec('_', 2).to_int();

		if (index >= _flavour_tiles.size()) {
			_flavour_tiles.resize(index + 1);
		}

		StringName p = name.get_slicec('/', 1);

		if (p == "texture") {
			_flavour_tiles.write[index].texture = p_value;

			return true;
		} else if (p == "y_size") {
			_flavour_tiles.write[index].y_size = p_value;

			return true;
		} else if (p == "z_offset") {
			_flavour_tiles.write[index].z_offset = p_value;

			return true;
		} else if (p == "texture_scale") {
			_flavour_tiles.write[index].texture_scale = p_value;

			return true;
		}
	}

#ifndef DISABLE_DEPRECATED
	// TODO REMOVE AFTER NEXT RELEASE
	if (p_name == "set_tile_count") {
		set_tile_count(p_value);

		return true;
	}
#endif

	return false;
}

bool TiledWallData::_get(const StringName &p_name, Variant &r_ret) const {
	String name = p_name;

	if (name.begins_with("tile_")) {
		int index = name.get_slicec('_', 1).to_int();

		if (index >= _tiles.size()) {
			return false;
		}

		StringName p = name.get_slicec('/', 1);

		if (p == "texture") {
			r_ret = _tiles[index].texture;

			return true;
		} else if (p == "y_size") {
			r_ret = _tiles[index].y_size;

			return true;
		} else if (p == "z_offset") {
			r_ret = _tiles[index].z_offset;

			return true;
		} else if (p == "texture_scale") {
			r_ret = _tiles[index].texture_scale;

			return true;
		}
	} else if (name.begins_with("flavour_tile_")) {
		int index = name.get_slicec('_', 2).to_int();

		if (index >= _flavour_tiles.size()) {
			return false;
		}

		StringName p = name.get_slicec('/', 1);

		if (p == "texture") {
			r_ret = _flavour_tiles[index].texture;

			return true;
		} else if (p == "y_size") {
			r_ret = _flavour_tiles[index].y_size;

			return true;
		} else if (p == "z_offset") {
			r_ret = _flavour_tiles[index].z_offset;

			return true;
		} else if (p == "texture_scale") {
			r_ret = _flavour_tiles[index].texture_scale;

			return true;
		}
	}

	return false;
}

void TiledWallData::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < _tiles.size(); ++i) {
		p_list->push_back(PropertyInfo(Variant::OBJECT, "tile_" + itos(i) + "/texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture", PROPERTY_USAGE_DEFAULT));
		p_list->push_back(PropertyInfo(Variant::REAL, "tile_" + itos(i) + "/y_size"));
		p_list->push_back(PropertyInfo(Variant::REAL, "tile_" + itos(i) + "/z_offset"));
		p_list->push_back(PropertyInfo(Variant::INT, "tile_" + itos(i) + "/texture_scale"));
	}

	for (int i = 0; i < _flavour_tiles.size(); ++i) {
		p_list->push_back(PropertyInfo(Variant::OBJECT, "flavour_tile_" + itos(i) + "/texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture", PROPERTY_USAGE_DEFAULT));
		p_list->push_back(PropertyInfo(Variant::REAL, "flavour_tile_" + itos(i) + "/y_size"));
		p_list->push_back(PropertyInfo(Variant::REAL, "flavour_tile_" + itos(i) + "/z_offset"));
		p_list->push_back(PropertyInfo(Variant::INT, "flavour_tile_" + itos(i) + "/texture_scale"));
	}
}

void TiledWallData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_tiling_type"), &TiledWallData::get_tiling_type);
	ClassDB::bind_method(D_METHOD("set_tiling_type", "texture"), &TiledWallData::set_tiling_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tiling_type", PROPERTY_HINT_ENUM, TiledWallData::BINDING_STRING_TILED_WALL_TILING_TYPE), "set_tiling_type", "get_tiling_type");

	ClassDB::bind_method(D_METHOD("get_collider_type"), &TiledWallData::get_collider_type);
	ClassDB::bind_method(D_METHOD("set_collider_type", "texture"), &TiledWallData::set_collider_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collider_type", PROPERTY_HINT_ENUM, TiledWallData::BINDING_STRING_TILED_WALL_COLLIDER_TYPE), "set_collider_type", "get_collider_type");

	ClassDB::bind_method(D_METHOD("get_collider_z_offset"), &TiledWallData::get_collider_z_offset);
	ClassDB::bind_method(D_METHOD("set_collider_z_offset", "val"), &TiledWallData::set_collider_z_offset);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "collider_z_offset"), "set_collider_z_offset", "get_collider_z_offset");

	//textures
	ClassDB::bind_method(D_METHOD("add_tile", "texture", "y_size", "z_offset", "texture_scale"), &TiledWallData::add_tile, 1, 0, 1);
	ClassDB::bind_method(D_METHOD("remove_tile", "index"), &TiledWallData::remove_tile);

	ClassDB::bind_method(D_METHOD("get_tile_texture", "index"), &TiledWallData::get_tile_texture);
	ClassDB::bind_method(D_METHOD("set_tile_texture", "index", "texture"), &TiledWallData::set_tile_texture);

	ClassDB::bind_method(D_METHOD("get_tile_y_size", "index"), &TiledWallData::get_tile_y_size);
	ClassDB::bind_method(D_METHOD("set_tile_y_size", "index", "val"), &TiledWallData::set_tile_y_size);

	ClassDB::bind_method(D_METHOD("get_tile_z_offset", "index"), &TiledWallData::get_tile_z_offset);
	ClassDB::bind_method(D_METHOD("set_tile_z_offset", "index", "val"), &TiledWallData::set_tile_z_offset);

	ClassDB::bind_method(D_METHOD("get_tile_texture_scale", "index"), &TiledWallData::get_tile_texture_scale);
	ClassDB::bind_method(D_METHOD("set_tile_texture_scale", "index", "val"), &TiledWallData::set_tile_texture_scale);

	ClassDB::bind_method(D_METHOD("get_tile_count"), &TiledWallData::get_tile_count);
	ClassDB::bind_method(D_METHOD("set_tile_count", "count"), &TiledWallData::set_tile_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tile_count", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_tile_count", "get_tile_count");

	//flavour_tiles
	ClassDB::bind_method(D_METHOD("add_flavour_tile", "texture", "y_size", "z_offset", "texture_scale"), &TiledWallData::add_flavour_tile, 1, 0, 1);
	ClassDB::bind_method(D_METHOD("remove_flavour_tile", "index"), &TiledWallData::remove_flavour_tile);

	ClassDB::bind_method(D_METHOD("get_flavour_tile_texture", "index"), &TiledWallData::get_flavour_tile_texture);
	ClassDB::bind_method(D_METHOD("set_flavour_tile_texture", "index", "texture"), &TiledWallData::set_flavour_tile_texture);

	ClassDB::bind_method(D_METHOD("get_flavour_tile_y_size", "index"), &TiledWallData::get_flavour_tile_y_size);
	ClassDB::bind_method(D_METHOD("set_flavour_tile_y_size", "index", "val"), &TiledWallData::set_flavour_tile_y_size);

	ClassDB::bind_method(D_METHOD("get_flavour_tile_z_offset", "index"), &TiledWallData::get_flavour_tile_z_offset);
	ClassDB::bind_method(D_METHOD("set_flavour_tile_z_offset", "index", "val"), &TiledWallData::set_flavour_tile_z_offset);

	ClassDB::bind_method(D_METHOD("get_flavour_tile_texture_scale", "index"), &TiledWallData::get_flavour_tile_texture_scale);
	ClassDB::bind_method(D_METHOD("set_flavour_tile_texture_scale", "index", "val"), &TiledWallData::set_flavour_tile_texture_scale);

	ClassDB::bind_method(D_METHOD("get_flavour_tile_count"), &TiledWallData::get_flavour_tile_count);
	ClassDB::bind_method(D_METHOD("set_flavour_tile_count", "count"), &TiledWallData::set_flavour_tile_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "flavour_tile_count", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_flavour_tile_count", "get_flavour_tile_count");

	ClassDB::bind_method(D_METHOD("get_flavour_tile_chance"), &TiledWallData::get_flavour_tile_chance);
	ClassDB::bind_method(D_METHOD("set_flavour_tile_chance", "texture"), &TiledWallData::set_flavour_tile_chance);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "flavour_tile_chance"), "set_flavour_tile_chance", "get_flavour_tile_chance");

#ifdef MODULE_FASTNOISE_ENABLED
	ClassDB::bind_method(D_METHOD("get_offset_noise"), &TiledWallData::get_offset_noise);
	ClassDB::bind_method(D_METHOD("set_offset_noise", "texture"), &TiledWallData::set_offset_noise);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "offset_noise", PROPERTY_HINT_RESOURCE_TYPE, "FastnoiseNoiseParams"), "set_offset_noise", "get_offset_noise");

	ClassDB::bind_method(D_METHOD("get_offset_noise_strength"), &TiledWallData::get_offset_noise_strength);
	ClassDB::bind_method(D_METHOD("set_offset_noise_strength", "texture"), &TiledWallData::set_offset_noise_strength);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "offset_noise_strength"), "set_offset_noise_strength", "get_offset_noise_strength");

	ClassDB::bind_method(D_METHOD("get_offset_noise_randomize_seed"), &TiledWallData::get_offset_noise_randomize_seed);
	ClassDB::bind_method(D_METHOD("set_offset_noise_randomize_seed", "texture"), &TiledWallData::set_offset_noise_randomize_seed);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "offset_noise_randomize_seed"), "set_offset_noise_randomize_seed", "get_offset_noise_randomize_seed");

	ClassDB::bind_method(D_METHOD("get_offset_noise_skip_edges"), &TiledWallData::get_offset_noise_skip_edges);
	ClassDB::bind_method(D_METHOD("set_offset_noise_skip_edges", "texture"), &TiledWallData::set_offset_noise_skip_edges);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "offset_noise_skip_edges"), "set_offset_noise_skip_edges", "get_offset_noise_skip_edges");
#endif

	//materials
	ClassDB::bind_method(D_METHOD("material_add", "value"), &TiledWallData::material_add);
	ClassDB::bind_method(D_METHOD("material_set", "index", "value"), &TiledWallData::material_set);
	ClassDB::bind_method(D_METHOD("material_remove", "index"), &TiledWallData::material_remove);
	ClassDB::bind_method(D_METHOD("material_get_num"), &TiledWallData::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &TiledWallData::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &TiledWallData::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &TiledWallData::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "23/20:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	ClassDB::bind_method(D_METHOD("add_textures_into", "texture_packer"), &TiledWallData::add_textures_into);
#endif

	BIND_VMETHOD(MethodInfo("_setup_cache", PropertyInfo(Variant::OBJECT, "cache", PROPERTY_HINT_RESOURCE_TYPE, "PropMaterialCache")));

	ClassDB::bind_method(D_METHOD("setup_cache", "cache"), &TiledWallData::setup_cache);
	ClassDB::bind_method(D_METHOD("_setup_cache", "cache"), &TiledWallData::_setup_cache);

#ifdef MODULE_ENTITY_SPELL_SYSTEM_ENABLED
	BIND_VMETHOD(MethodInfo("_setup_ess_cache", PropertyInfo(Variant::OBJECT, "cache", PROPERTY_HINT_RESOURCE_TYPE, "ESSMaterialCache")));

	ClassDB::bind_method(D_METHOD("setup_ess_cache", "cache"), &TiledWallData::setup_ess_cache);
	ClassDB::bind_method(D_METHOD("_setup_ess_cache", "cache"), &TiledWallData::_setup_ess_cache);
#endif

	ClassDB::bind_method(D_METHOD("copy_from", "prop_data"), &TiledWallData::copy_from);

	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_NONE);
	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_HORIZONTAL);
	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_VERTICAL);
	BIND_ENUM_CONSTANT(TILED_WALL_TILING_TYPE_BOTH);

	BIND_ENUM_CONSTANT(TILED_WALL_COLLIDER_TYPE_NONE);
	BIND_ENUM_CONSTANT(TILED_WALL_COLLIDER_TYPE_PLANE);
	BIND_ENUM_CONSTANT(TILED_WALL_COLLIDER_TYPE_BOX);
	BIND_ENUM_CONSTANT(TILED_WALL_COLLIDER_TYPE_CONVEX_MESH);
	BIND_ENUM_CONSTANT(TILED_WALL_COLLIDER_TYPE_CONCAVE_MESH);
}
