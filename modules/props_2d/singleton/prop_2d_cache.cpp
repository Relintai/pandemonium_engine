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

#include "prop_2d_cache.h"

#include "../props/prop_2d_data.h"
#include "../props/prop_2d_data_entry.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"

#include "../jobs/prop_2d_texture_job.h"

#include "core/os/thread_pool.h"

#include "../material_cache/prop_2d_material_cache.h"
#include "../tiled_wall/tiled_wall_2d_data.h"

#include "core/containers/hashfuncs.h"

#define VARIANT_ARRAY_GET(arr)             \
	Vector<Variant> r;                     \
	for (int i = 0; i < arr.size(); i++) { \
		r.push_back(arr[i].get_ref_ptr()); \
	}                                      \
	return r;

Prop2DCache *Prop2DCache::_instance;

Prop2DCache *Prop2DCache::get_singleton() {
	return _instance;
}

float Prop2DCache::get_default_pixels_per_unit() const {
	return _default_pixels_per_unit;
}
void Prop2DCache::set_default_pixels_per_unit(const float value) {
	_default_pixels_per_unit = value;
}

StringName Prop2DCache::get_default_prop_material_cache_class() {
	return _default_prop_material_cache_class;
}
void Prop2DCache::set_default_prop_material_cache_class(const StringName &cls_name) {
	_default_prop_material_cache_class = cls_name;
}

#ifdef TEXTURE_PACKER_PRESENT
int Prop2DCache::get_texture_flags() const {
	return _texture_flags;
}
void Prop2DCache::set_texture_flags(const int flags) {
	_texture_flags = flags;
}

int Prop2DCache::get_max_atlas_size() const {
	return _max_atlas_size;
}
void Prop2DCache::set_max_atlas_size(const int size) {
	_max_atlas_size = size;
}

bool Prop2DCache::get_keep_original_atlases() const {
	return _keep_original_atlases;
}
void Prop2DCache::set_keep_original_atlases(const bool value) {
	_keep_original_atlases = value;
}

Color Prop2DCache::get_background_color() const {
	return _background_color;
}
void Prop2DCache::set_background_color(const Color &color) {
	_background_color = color;
}

int Prop2DCache::get_margin() const {
	return _margin;
}
void Prop2DCache::set_margin(const int margin) {
	_margin = margin;
}
#endif

String Prop2DCache::material_path_get() const {
	return _material_path;
}
void Prop2DCache::material_path_set(const String &value) {
	_material_path = value;
}

Ref<Material> Prop2DCache::material_get() {
	return _material;
}

void Prop2DCache::material_set(const Ref<Material> &value) {
	_material = value;
}

void Prop2DCache::material_load() {
	_material.unref();

	if (_material_path == "") {
		return;
	}

	Ref<Material> d = load_resource(_material_path, "Material");

	ERR_FAIL_COND(!d.is_valid());

	_material = d;
}

void Prop2DCache::ensure_material_loaded() {
	if (_material_path == "" || _material.is_valid()) {
		return;
	}

	material_load();
}

Ref<Prop2DMaterialCache> Prop2DCache::material_cache_get(const Ref<Prop2DData> &prop) {
	ERR_FAIL_COND_V(!prop.is_valid(), Ref<Prop2DMaterialCache>());

	//get pointer's value as uint64
	uint64_t k = make_uint64_t<const Prop2DData *>(*prop);

	_material_cache_mutex.lock();

	if (_material_cache.has(k)) {
		Ref<Prop2DMaterialCache> m = _material_cache[k];

		m->inc_ref_count();

		_material_cache_mutex.unlock();

		return m;
	}

	Prop2DMaterialCache *p = Object::cast_to<Prop2DMaterialCache>(ClassDB::instance(_default_prop_material_cache_class));

	if (!p) {
		ERR_PRINT("Can't instance the given Prop2DMaterialCache! class_name: " + String(_default_prop_material_cache_class));
	}

	Ref<Prop2DMaterialCache> m(p);

	_material_cache[k] = m;

	_material_cache_mutex.unlock();

	return m;
}
void Prop2DCache::material_cache_unref(const Ref<Prop2DData> &prop) {
	//get pointer's value as uint64
	uint64_t k = make_uint64_t<const Prop2DData *>(*prop);

	_material_cache_mutex.lock();

	if (!_material_cache.has(k)) {
		_material_cache_mutex.unlock();

		ERR_PRINT("Prop2DCache::material_cache_unref: can't find cache!");

		return;
	}

	Ref<Prop2DMaterialCache> m = _material_cache[k];

	m->dec_ref_count();
	if (m->get_ref_count() <= 0) {
		_material_cache.erase(k);
	}

	_material_cache_mutex.unlock();
}

Ref<Prop2DMaterialCache> Prop2DCache::tiled_wall_material_cache_get(const Ref<TiledWall2DData> &twd) {
	ERR_FAIL_COND_V(!twd.is_valid(), Ref<Prop2DMaterialCache>());

	//get pointer's value as uint64
	uint64_t k = make_uint64_t<const TiledWall2DData *>(*twd);

	_tiled_wall_material_cache_mutex.lock();

	if (_tiled_wall_material_cache.has(k)) {
		Ref<Prop2DMaterialCache> m = _tiled_wall_material_cache[k];

		m->inc_ref_count();

		_tiled_wall_material_cache_mutex.unlock();

		return m;
	}

	Prop2DMaterialCache *p = Object::cast_to<Prop2DMaterialCache>(ClassDB::instance(_default_prop_material_cache_class));

	if (!p) {
		ERR_PRINT("Can't instance the given Prop2DMaterialCache! class_name: " + String(_default_prop_material_cache_class));
	}

	Ref<Prop2DMaterialCache> m(p);

	_tiled_wall_material_cache[k] = m;

	_tiled_wall_material_cache_mutex.unlock();

	return m;
}
void Prop2DCache::tiled_wall_material_cache_unref(const Ref<TiledWall2DData> &twd) {
	//get pointer's value as uint64
	uint64_t k = make_uint64_t<const TiledWall2DData *>(*twd);

	_tiled_wall_material_cache_mutex.lock();

	if (!_tiled_wall_material_cache.has(k)) {
		_tiled_wall_material_cache_mutex.unlock();

		ERR_PRINT("Prop2DCache::material_cache_unref: can't find cache!");

		return;
	}

	Ref<Prop2DMaterialCache> m = _tiled_wall_material_cache[k];

	m->dec_ref_count();
	if (m->get_ref_count() <= 0) {
		_tiled_wall_material_cache.erase(k);
	}

	_tiled_wall_material_cache_mutex.unlock();
}

Ref<Prop2DMaterialCache> Prop2DCache::material_cache_custom_key_get(const uint64_t key) {
	_custom_keyed_material_cache_mutex.lock();

	if (_custom_keyed_material_cache.has(key)) {
		Ref<Prop2DMaterialCache> m = _custom_keyed_material_cache[key];

		m->inc_ref_count();

		_custom_keyed_material_cache_mutex.unlock();

		return m;
	}

	Prop2DMaterialCache *p = Object::cast_to<Prop2DMaterialCache>(ClassDB::instance(_default_prop_material_cache_class));

	if (!p) {
		ERR_PRINT("Can't instance the given Prop2DMaterialCache! class_name: " + String(_default_prop_material_cache_class));
	}

	Ref<Prop2DMaterialCache> m(p);

	_custom_keyed_material_cache[key] = m;

	_custom_keyed_material_cache_mutex.unlock();

	return m;
}
void Prop2DCache::material_cache_custom_key_unref(const uint64_t key) {
	_custom_keyed_material_cache_mutex.lock();

	if (!_material_cache.has(key)) {
		_custom_keyed_material_cache_mutex.unlock();

		ERR_PRINT("Prop2DCache::material_cache_custom_key_unref: can't find cache!");

		return;
	}

	Ref<Prop2DMaterialCache> m = _custom_keyed_material_cache[key];

	m->dec_ref_count();
	if (m->get_ref_count() <= 0) {
		_custom_keyed_material_cache.erase(key);
	}

	_custom_keyed_material_cache_mutex.unlock();
}

Ref<Resource> Prop2DCache::load_resource(const String &path, const String &type_hint) {
	_ResourceLoader *rl = _ResourceLoader::get_singleton();

	Ref<ResourceInteractiveLoader> resl = rl->load_interactive(path, type_hint);

	ERR_FAIL_COND_V(!resl.is_valid(), Ref<Resource>());

	resl->wait();

	return resl->get_resource();
}

Prop2DCache::Prop2DCache() {
	_instance = this;

	_default_pixels_per_unit = GLOBAL_DEF("props_2d/default_pixels_per_unit", 64);

#if TEXTURE_PACKER_PRESENT
	_default_prop_material_cache_class = GLOBAL_DEF("props_2d/default_prop_material_cache_class", "Prop2DMaterialCachePCM");
#else
	_default_prop_material_cache_class = GLOBAL_DEF("props_2d/default_prop_material_cache_class", "Prop2DMaterialCache");
#endif

#ifdef TEXTURE_PACKER_PRESENT
	_texture_flags = GLOBAL_DEF("props_2d/texture_flags", Texture::FLAG_MIPMAPS | Texture::FLAG_FILTER);

	_max_atlas_size = GLOBAL_DEF("props_2d/max_atlas_size", 1024);
	_keep_original_atlases = GLOBAL_DEF("props_2d/keep_original_atlases", false);
	_background_color = GLOBAL_DEF("props_2d/background_color", Color());
	_margin = GLOBAL_DEF("props_2d/margin", 0);
#endif

	_material_path = GLOBAL_DEF("props_2d/material_path", "");
}

Prop2DCache::~Prop2DCache() {
	_instance = NULL;
}

void Prop2DCache::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_default_pixels_per_unit"), &Prop2DCache::get_default_pixels_per_unit);
	ClassDB::bind_method(D_METHOD("set_default_pixels_per_unit", "value"), &Prop2DCache::set_default_pixels_per_unit);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "default_pixels_per_unit"), "set_default_pixels_per_unit", "get_default_pixels_per_unit");

	ClassDB::bind_method(D_METHOD("get_default_prop_material_cache_class"), &Prop2DCache::get_default_prop_material_cache_class);
	ClassDB::bind_method(D_METHOD("set_default_prop_material_cache_class", "cls_name"), &Prop2DCache::set_default_prop_material_cache_class);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "default_prop_material_cache_class"), "set_default_prop_material_cache_class", "get_default_prop_material_cache_class");

#ifdef TEXTURE_PACKER_PRESENT
	ClassDB::bind_method(D_METHOD("get_texture_flags"), &Prop2DCache::get_texture_flags);
	ClassDB::bind_method(D_METHOD("set_texture_flags", "flags"), &Prop2DCache::set_texture_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_flags", PROPERTY_HINT_FLAGS, "Mipmaps,Repeat,Filter,Anisotropic Linear,Convert to Linear,Mirrored Repeat,Video Surface"), "set_texture_flags", "get_texture_flags");

	ClassDB::bind_method(D_METHOD("get_max_atlas_size"), &Prop2DCache::get_max_atlas_size);
	ClassDB::bind_method(D_METHOD("set_max_atlas_size", "size"), &Prop2DCache::set_max_atlas_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_atlas_size"), "set_max_atlas_size", "get_max_atlas_size");

	ClassDB::bind_method(D_METHOD("get_keep_original_atlases"), &Prop2DCache::get_keep_original_atlases);
	ClassDB::bind_method(D_METHOD("set_keep_original_atlases", "value"), &Prop2DCache::set_keep_original_atlases);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_original_atlases"), "set_keep_original_atlases", "get_keep_original_atlases");

	ClassDB::bind_method(D_METHOD("get_background_color"), &Prop2DCache::get_background_color);
	ClassDB::bind_method(D_METHOD("set_background_color", "color"), &Prop2DCache::set_background_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_background_color", "get_background_color");

	ClassDB::bind_method(D_METHOD("get_margin"), &Prop2DCache::get_margin);
	ClassDB::bind_method(D_METHOD("set_margin", "size"), &Prop2DCache::set_margin);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin"), "set_margin", "get_margin");
#endif

	ClassDB::bind_method(D_METHOD("material_path_get"), &Prop2DCache::material_path_get);
	ClassDB::bind_method(D_METHOD("material_path_set", "value"), &Prop2DCache::material_path_set);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "material_path"), "material_path_set", "material_path_get");

	ClassDB::bind_method(D_METHOD("material_get"), &Prop2DCache::material_get);
	ClassDB::bind_method(D_METHOD("material_set", "value"), &Prop2DCache::material_set);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "material_set", "material_get");

	ClassDB::bind_method(D_METHOD("material_load"), &Prop2DCache::material_load);
	ClassDB::bind_method(D_METHOD("ensure_material_loaded"), &Prop2DCache::ensure_material_loaded);

	ClassDB::bind_method(D_METHOD("material_cache_get", "prop"), &Prop2DCache::material_cache_get);
	ClassDB::bind_method(D_METHOD("material_cache_unref", "prop"), &Prop2DCache::material_cache_unref);

	ClassDB::bind_method(D_METHOD("tiled_wall_material_cache_get", "twd"), &Prop2DCache::tiled_wall_material_cache_get);
	ClassDB::bind_method(D_METHOD("tiled_wall_material_cache_unref", "twd"), &Prop2DCache::tiled_wall_material_cache_unref);

	ClassDB::bind_method(D_METHOD("material_cache_custom_key_get", "key"), &Prop2DCache::material_cache_custom_key_get);
	ClassDB::bind_method(D_METHOD("material_cache_custom_key_unref", "key"), &Prop2DCache::material_cache_custom_key_unref);

	ClassDB::bind_method(D_METHOD("load_resource", "path", "type_hint"), &Prop2DCache::load_resource, "");
}
