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

#include "terrain_library_merger_pcm.h"

#include "../../texture_packer/texture_packer.h"
#include "scene/resources/packed_scene.h"
#include "scene/resources/texture.h"

#ifdef PROPS_PRESENT
#include "../../props/props/prop_data.h"

#include "../../props/props/prop_data_prop.h"

#if MESH_DATA_RESOURCE_PRESENT
#include "../../mesh_data_resource/props/prop_data_mesh_data.h"
#endif
#endif

#include "../defines.h"
#include "../world/default/terrain_chunk_default.h"
#include "core/containers/hashfuncs.h"
#include "core/object/message_queue.h"
#include "terrain_material_cache_pcm.h"

bool TerrainLibraryMergerPCM::_supports_caching() {
	return true;
}

void TerrainLibraryMergerPCM::_material_cache_get_key(Ref<TerrainChunk> chunk) {
	uint8_t *ch = chunk->channel_get(TerrainChunkDefault::DEFAULT_CHANNEL_TYPE);

	if (!ch) {
		chunk->material_cache_key_set(0);
		chunk->material_cache_key_has_set(false);

		return;
	}

	Vector<uint8_t> surfaces;

	uint32_t size = chunk->get_data_size();

	for (uint32_t i = 0; i < size; ++i) {
		uint8_t v = ch[i];

		if (v == 0) {
			continue;
		}

		int ssize = surfaces.size();
		bool found = false;
		for (uint8_t j = 0; j < ssize; ++j) {
			if (surfaces[j] == v) {
				found = true;
				break;
			}
		}

		if (!found) {
			surfaces.push_back(v);
		}
	}

	if (surfaces.size() == 0) {
		chunk->material_cache_key_set(0);
		chunk->material_cache_key_has_set(false);

		return;
	}

	surfaces.sort();

	String hstr;

	for (int i = 0; i < surfaces.size(); ++i) {
		hstr += String::num(surfaces[i]) + "|";
	}

	int hash = static_cast<int>(hstr.hash());

	chunk->material_cache_key_set(hash);
	chunk->material_cache_key_has_set(true);

	_material_cache_mutex.lock();

	if (_material_cache.has(hash)) {
		Ref<TerrainMaterialCachePCM> cc = _material_cache[hash];

		if (cc.is_valid()) {
			cc->inc_ref_count();
		}

		_material_cache_mutex.unlock();

		return;
	}

	//print_error("New cache: " + hstr);

	Ref<TerrainMaterialCachePCM> cache;
	cache.instance();
	cache->inc_ref_count();

	cache->set_texture_flags(get_texture_flags());
	cache->set_max_atlas_size(get_max_atlas_size());
	cache->set_keep_original_atlases(get_keep_original_atlases());
	cache->set_background_color(get_background_color());
	cache->set_margin(get_margin());

	for (int i = 0; i < surfaces.size(); ++i) {
		int s = surfaces[i] - 1;

		if (_terra_surfaces.size() <= s) {
			continue;
		}

		Ref<TerrainSurfaceMerger> ms = _terra_surfaces[s];

		if (!ms.is_valid()) {
			continue;
		}

		Ref<TerrainSurfaceMerger> nms = ms->duplicate();
		nms->set_library(Ref<TerrainLibraryMergerPCM>(this));
		nms->set_id(s);

		cache->surface_add(nms);
	}

	for (int i = 0; i < _materials.size(); ++i) {
		Ref<Material> m = _materials[i];

		if (!m.is_valid()) {
			continue;
		}

		Ref<Material> nm = m->duplicate();

		cache->material_add(nm);
	}

	_material_cache[hash] = cache;

	//unlock here, so if a different thread need the cache it will be able to immediately access the materials and surfaces when it gets it.
	_material_cache_mutex.unlock();

	//this will generate the atlases
	cache->refresh_rects();
}

Ref<TerrainMaterialCache> TerrainLibraryMergerPCM::_material_cache_get(const int key) {
	Ref<TerrainMaterialCache> c;

	_material_cache_mutex.lock();
	c = _material_cache[key];
	_material_cache_mutex.unlock();

	ERR_FAIL_COND_V(!c.is_valid(), Ref<TerrainMaterialCache>());

	return c;
}

void TerrainLibraryMergerPCM::_material_cache_unref(const int key) {
	if (_material_cache_mutex.try_lock() != OK) {
		// If we don't get the lock try again later
		// This is needed, because when duplicating materials the RenderingServer apparently
		// needs synchronization with the main thread. So if _material_cache_unref holds the mutex
		// and is duplicating the materials, trying to get the lock from the main thread will deadlock
		// the game. This can happen when chungs are spawned and despawned really fast.
		// E.g. when flying around in the editor.
		MessageQueue::get_singleton()->push_call(this, "_material_cache_unref", key, 1);
		return;
	}

	//_material_cache_mutex.lock();

	if (!_material_cache.has(key)) {
		return;
	}

	Ref<TerrainMaterialCachePCM> cc = _material_cache[key];

	if (!cc.is_valid()) {
		_material_cache_mutex.unlock();
		return;
	}

	cc->dec_ref_count();

	if (cc->get_ref_count() <= 0) {
		_material_cache.erase(key);
	}

	_material_cache_mutex.unlock();
}

//Liquids
void TerrainLibraryMergerPCM::_liquid_material_cache_get_key(Ref<TerrainChunk> chunk) {
	uint8_t *ch = chunk->channel_get(TerrainChunkDefault::DEFAULT_CHANNEL_LIQUID_TYPE);

	if (!ch) {
		chunk->liquid_material_cache_key_set(0);
		chunk->liquid_material_cache_key_has_set(false);

		return;
	}

	Vector<uint8_t> surfaces;

	uint32_t size = chunk->get_data_size();

	for (uint32_t i = 0; i < size; ++i) {
		uint8_t v = ch[i];

		if (v == 0) {
			continue;
		}

		int ssize = surfaces.size();
		bool found = false;
		for (uint8_t j = 0; j < ssize; ++j) {
			if (surfaces[j] == v) {
				found = true;
				break;
			}
		}

		if (!found) {
			surfaces.push_back(v);
		}
	}

	if (surfaces.size() == 0) {
		chunk->liquid_material_cache_key_set(0);
		chunk->liquid_material_cache_key_has_set(false);

		return;
	}

	surfaces.sort();

	String hstr;

	for (int i = 0; i < surfaces.size(); ++i) {
		hstr += String::num(surfaces[i]) + "|";
	}

	int hash = static_cast<int>(hstr.hash());

	chunk->liquid_material_cache_key_set(hash);
	chunk->liquid_material_cache_key_has_set(true);

	_liquid_material_cache_mutex.lock();

	if (_liquid_material_cache.has(hash)) {
		Ref<TerrainMaterialCachePCM> cc = _liquid_material_cache[hash];

		if (cc.is_valid()) {
			cc->inc_ref_count();
		}

		_liquid_material_cache_mutex.unlock();

		return;
	}

	//print_error("New cache: " + hstr);

	Ref<TerrainMaterialCachePCM> cache;
	cache.instance();
	cache->inc_ref_count();

	cache->set_texture_flags(get_texture_flags());
	cache->set_max_atlas_size(get_max_atlas_size());
	cache->set_keep_original_atlases(get_keep_original_atlases());
	cache->set_background_color(get_background_color());
	cache->set_margin(get_margin());

	for (int i = 0; i < surfaces.size(); ++i) {
		int s = surfaces[i] - 1;

		if (_terra_surfaces.size() <= s) {
			continue;
		}

		Ref<TerrainSurfaceMerger> ms = _terra_surfaces[s];

		if (!ms.is_valid()) {
			continue;
		}

		Ref<TerrainSurfaceMerger> nms = ms->duplicate();
		nms->set_library(Ref<TerrainLibraryMergerPCM>(this));
		nms->set_id(s);

		cache->surface_add(nms);
	}

	for (int i = 0; i < _liquid_materials.size(); ++i) {
		Ref<Material> m = _liquid_materials[i];

		if (!m.is_valid()) {
			continue;
		}

		Ref<Material> nm = m->duplicate();

		cache->material_add(nm);
	}

	_liquid_material_cache[hash] = cache;

	//unlock here, so if a different thread need the cache it will be able to immediately access the materials and surfaces when it gets it.
	_liquid_material_cache_mutex.unlock();

	//this will generate the atlases
	cache->refresh_rects();
}

Ref<TerrainMaterialCache> TerrainLibraryMergerPCM::_liquid_material_cache_get(const int key) {
	Ref<TerrainMaterialCache> c;

	_liquid_material_cache_mutex.lock();
	c = _liquid_material_cache[key];
	_liquid_material_cache_mutex.unlock();

	ERR_FAIL_COND_V(!c.is_valid(), Ref<TerrainMaterialCache>());

	return c;
}

void TerrainLibraryMergerPCM::_liquid_material_cache_unref(const int key) {
	if (_liquid_material_cache_mutex.try_lock() != OK) {
		// If we don't get the lock try again later
		// This is needed, because when duplicating materials the RenderingServer apparently
		// needs synchronization with the main thread. So if _material_cache_unref holds the mutex
		// and is duplicating the materials, trying to get the lock from the main thread will deadlock
		// the game. This can happen when chungs are spawned and despawned really fast.
		// E.g. when flying around in the editor.
		MessageQueue::get_singleton()->push_call(this, "_liquid_material_cache_unref", key, 1);
		return;
	}

	//_material_cache_mutex.lock();

	if (!_liquid_material_cache.has(key)) {
		return;
	}

	Ref<TerrainMaterialCachePCM> cc = _liquid_material_cache[key];

	if (!cc.is_valid()) {
		_liquid_material_cache_mutex.unlock();
		return;
	}

	cc->dec_ref_count();

	if (cc->get_ref_count() <= 0) {
		_liquid_material_cache.erase(key);
	}

	_liquid_material_cache_mutex.unlock();
}

//Props
void TerrainLibraryMergerPCM::_prop_material_cache_get_key(Ref<TerrainChunk> chunk) {
	Vector<uint64_t> props;

	/*
	#ifdef PROPS_PRESENT
		for (int i = 0; i < chunk->prop_get_count(); ++i) {
			Ref<PropData> prop = chunk->prop_get(i);

			ERR_CONTINUE(!prop.is_valid());

			//get pointer's value as uint64
			uint64_t v = make_uint64_t<PropData *>(*prop);

			int psize = props.size();
			bool found = false;
			for (int j = 0; j < psize; ++j) {
				if (props[j] == v) {
					found = true;
					break;
				}
			}

			if (!found) {
				props.push_back(v);
			}
		}
	#endif
	*/

#if MESH_DATA_RESOURCE_PRESENT
	for (int i = 0; i < chunk->mesh_data_resource_get_count(); ++i) {
		Ref<Texture> tex = chunk->mesh_data_resource_get_texture(i);

		if (!tex.is_valid())
			continue;

		//get pointer's value as uint64
		uint64_t v = make_uint64_t<Texture *>(*tex);

		int psize = props.size();
		bool found = false;
		for (int j = 0; j < psize; ++j) {
			if (props[j] == v) {
				found = true;
				break;
			}
		}

		if (!found) {
			props.push_back(v);
		}
	}
#endif

	if (props.size() == 0) {
		chunk->prop_material_cache_key_set(0);
		chunk->prop_material_cache_key_has_set(false);

		return;
	}

	props.sort();

	String hstr;

	for (int i = 0; i < props.size(); ++i) {
		hstr += String::num_uint64(props[i]) + "|";
	}

	int hash = static_cast<int>(hstr.hash());

	chunk->prop_material_cache_key_set(hash);
	chunk->prop_material_cache_key_has_set(true);

	_prop_material_cache_mutex.lock();

	if (_prop_material_cache.has(hash)) {
		Ref<TerrainMaterialCachePCM> cc = _prop_material_cache[hash];

		if (cc.is_valid()) {
			cc->inc_ref_count();
		}

		_prop_material_cache_mutex.unlock();

		return;
	}

	//print_error("New prop cache: " + hstr);

	Ref<TerrainMaterialCachePCM> cache;
	cache.instance();
	cache->inc_ref_count();

	cache->set_texture_flags(get_texture_flags());
	cache->set_max_atlas_size(get_max_atlas_size());
	cache->set_keep_original_atlases(get_keep_original_atlases());
	cache->set_background_color(get_background_color());
	cache->set_margin(get_margin());

	for (int i = 0; i < _prop_materials.size(); ++i) {
		Ref<Material> m = _prop_materials[i];

		if (!m.is_valid()) {
			continue;
		}

		Ref<Material> nm = m->duplicate();

		cache->material_add(nm);
	}

	/*
	#ifdef PROPS_PRESENT
		for (int i = 0; i < chunk->prop_get_count(); ++i) {
			Ref<PropData> prop = chunk->prop_get(i);

			ERR_CONTINUE(!prop.is_valid());

			cache->prop_add_textures(prop);
		}
	#endif
	*/

#if MESH_DATA_RESOURCE_PRESENT
	for (int i = 0; i < chunk->mesh_data_resource_get_count(); ++i) {
		Ref<Texture> tex = chunk->mesh_data_resource_get_texture(i);

		if (!tex.is_valid())
			continue;

		cache->additional_texture_add(tex);
	}
#endif

	_prop_material_cache[hash] = cache;

	//unlock here, so if a different thread need the cache it will be able to immediately access the materials and surfaces when it gets it.
	_prop_material_cache_mutex.unlock();

	//this will generate the atlases
	cache->refresh_rects();
}
Ref<TerrainMaterialCache> TerrainLibraryMergerPCM::_prop_material_cache_get(const int key) {
	Ref<TerrainMaterialCache> c;

	_prop_material_cache_mutex.lock();
	c = _prop_material_cache[key];
	_prop_material_cache_mutex.unlock();

	ERR_FAIL_COND_V(!c.is_valid(), Ref<TerrainMaterialCache>());

	return c;
}
void TerrainLibraryMergerPCM::_prop_material_cache_unref(const int key) {
	if (_prop_material_cache_mutex.try_lock() != OK) {
		// If we don't get the lock try again later
		// This is needed, because when duplicating materials the RenderingServer apparently
		// needs synchronization with the main thread. So if _prop_material_cache_get_key holds the mutex
		// and is duplicating the materials, trying to get the lock from the main thread will deadlock
		// the game. This can happen when chungs are spawned and despawned really fast.
		// E.g. when flying around in the editor.
		MessageQueue::get_singleton()->push_call(this, "_prop_material_cache_unref", key, 1);
		return;
	}

	//_prop_material_cache_mutex.lock();

	if (!_prop_material_cache.has(key)) {
		return;
	}

	Ref<TerrainMaterialCachePCM> cc = _prop_material_cache[key];

	if (!cc.is_valid()) {
		_prop_material_cache_mutex.unlock();
		return;
	}

	cc->dec_ref_count();

	if (cc->get_ref_count() <= 0) {
		_prop_material_cache.erase(key);
	}

	_prop_material_cache_mutex.unlock();
}

int TerrainLibraryMergerPCM::get_texture_flags() const {
	return _packer->get_texture_flags();
}
void TerrainLibraryMergerPCM::set_texture_flags(const int flags) {
	_packer->set_texture_flags(flags);
	_prop_packer->set_texture_flags(flags);
}

int TerrainLibraryMergerPCM::get_max_atlas_size() const {
	return _packer->get_max_atlas_size();
}
void TerrainLibraryMergerPCM::set_max_atlas_size(const int size) {
	_packer->set_max_atlas_size(size);
	_prop_packer->set_max_atlas_size(size);
}

bool TerrainLibraryMergerPCM::get_keep_original_atlases() const {
	return _packer->get_keep_original_atlases();
}
void TerrainLibraryMergerPCM::set_keep_original_atlases(const bool value) {
	_packer->set_keep_original_atlases(value);
	_prop_packer->set_keep_original_atlases(value);
}

Color TerrainLibraryMergerPCM::get_background_color() const {
	return _packer->get_background_color();
}
void TerrainLibraryMergerPCM::set_background_color(const Color &color) {
	_packer->set_background_color(color);
	_prop_packer->set_background_color(color);
}

int TerrainLibraryMergerPCM::get_margin() const {
	return _packer->get_margin();
}
void TerrainLibraryMergerPCM::set_margin(const int margin) {
	_packer->set_margin(margin);
	_prop_packer->set_margin(margin);
}

//Surfaces
Ref<TerrainSurface> TerrainLibraryMergerPCM::terra_surface_get(const int index) {
	ERR_FAIL_INDEX_V(index, _terra_surfaces.size(), Ref<TerrainSurface>(NULL));

	return _terra_surfaces[index];
}

void TerrainLibraryMergerPCM::terra_surface_add(Ref<TerrainSurface> value) {
	ERR_FAIL_COND(!value.is_valid());

	value->set_library(Ref<TerrainLibraryMergerPCM>(this));
	value->set_id(_terra_surfaces.size());

	_terra_surfaces.push_back(value);
}

void TerrainLibraryMergerPCM::terra_surface_set(const int index, Ref<TerrainSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_terra_surfaces.size() < index) {
		_terra_surfaces.resize(index + 1);
	}

	if (_terra_surfaces[index].is_valid()) {
		_terra_surfaces.get(index)->set_library(Ref<TerrainLibraryMergerPCM>(NULL));
	}

	if (value.is_valid()) {
		value->set_library(Ref<TerrainLibraryMergerPCM>(this));

		_terra_surfaces.set(index, value);
	}
}

void TerrainLibraryMergerPCM::terra_surface_remove(const int index) {
	_terra_surfaces.remove(index);
}

int TerrainLibraryMergerPCM::terra_surface_get_num() const {
	return _terra_surfaces.size();
}

void TerrainLibraryMergerPCM::terra_surfaces_clear() {
	_packer->clear();

	for (int i = 0; i < _terra_surfaces.size(); i++) {
		Ref<TerrainSurfaceMerger> surface = _terra_surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(NULL);
		}
	}

	_terra_surfaces.clear();
}

Vector<Variant> TerrainLibraryMergerPCM::get_terra_surfaces() {
	VARIANT_ARRAY_GET(_terra_surfaces);
}

void TerrainLibraryMergerPCM::set_terra_surfaces(const Vector<Variant> &surfaces) {
	_terra_surfaces.clear();

	for (int i = 0; i < surfaces.size(); i++) {
		Ref<TerrainSurfaceMerger> surface = Ref<TerrainSurfaceMerger>(surfaces[i]);

		if (surface.is_valid()) {
			surface->set_library(Ref<TerrainLibraryMergerPCM>(this));
		}

		_terra_surfaces.push_back(surface);
	}
}

#ifdef PROPS_PRESENT
Ref<PropData> TerrainLibraryMergerPCM::get_prop(const int index) {
	ERR_FAIL_INDEX_V(index, _props.size(), Ref<PropData>());

	return _props[index];
}
void TerrainLibraryMergerPCM::add_prop(Ref<PropData> value) {
	_props.push_back(value);
}
bool TerrainLibraryMergerPCM::has_prop(const Ref<PropData> &value) const {
	return _props.find(value) != -1;
}
void TerrainLibraryMergerPCM::set_prop(const int index, const Ref<PropData> &value) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.write[index] = value;
}
void TerrainLibraryMergerPCM::remove_prop(const int index) {
	ERR_FAIL_INDEX(index, _props.size());

	_props.remove(index);
}
int TerrainLibraryMergerPCM::get_num_props() const {
	return _props.size();
}
void TerrainLibraryMergerPCM::clear_props() {
	_props.clear();
}

Vector<Variant> TerrainLibraryMergerPCM::get_props() {
	VARIANT_ARRAY_GET(_props);
}

void TerrainLibraryMergerPCM::set_props(const Vector<Variant> &props) {
	VARIANT_ARRAY_SET(props, _props, PropData);
}

Rect2 TerrainLibraryMergerPCM::get_prop_uv_rect(const Ref<Texture> &texture) {
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

Ref<TexturePacker> TerrainLibraryMergerPCM::get_prop_packer() {
	return _prop_packer;
}
#endif

void TerrainLibraryMergerPCM::refresh_rects() {
	/*
	bool texture_added = false;
	for (int i = 0; i < _terra_surfaces.size(); i++) {
		Ref<TerrainSurfaceMerger> surface = Ref<TerrainSurfaceMerger>(_terra_surfaces[i]);

		if (surface.is_valid()) {
			for (int j = 0; j < TerrainSurface::TERRAIN_SIDES_COUNT; ++j) {
				Ref<Texture> tex = surface->get_texture(static_cast<TerrainSurface::TerrainSurfaceSides>(j));

				if (!tex.is_valid())
					continue;

				if (!_packer->contains_texture(tex)) {
					texture_added = true;
					surface->set_region(static_cast<TerrainSurface::TerrainSurfaceSides>(j), _packer->add_texture(tex));
				} else {
					surface->set_region(static_cast<TerrainSurface::TerrainSurfaceSides>(j), _packer->get_texture(tex));
				}
			}
		}
	}

	if (texture_added) {
		_packer->merge();

		ERR_FAIL_COND(_packer->get_texture_count() == 0);

		Ref<Texture> tex = _packer->get_generated_texture(0);

		setup_material_albedo(MATERIAL_INDEX_TERRAIN, tex);
		setup_material_albedo(MATERIAL_INDEX_LIQUID, tex);
	}

#ifdef PROPS_PRESENT
	//todo add this back
	//texture_added = false;
	for (int i = 0; i < _props.size(); i++) {
		Ref<PropData> prop = _props.get(i);

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

		setup_material_albedo(MATERIAL_INDEX_PROP, tex);
	}
#endif

	for (int i = 0; i < _terra_surfaces.size(); i++) {
		Ref<TerrainSurfaceMerger> surface = _terra_surfaces[i];

		if (surface.is_valid()) {
			surface->refresh_rects();
		}
	}
*/
	set_initialized(true);
}

void TerrainLibraryMergerPCM::_setup_material_albedo(const int material_index, const Ref<Texture> &texture) {
	/*
	Ref<SpatialMaterial> mat;

	int count = 0;

	switch (material_index) {
		case MATERIAL_INDEX_TERRAIN:
			count = material_get_num();
			break;
		case MATERIAL_INDEX_LIQUID:
			count = liquid_material_get_num();
			break;
		case MATERIAL_INDEX_PROP:
			count = prop_material_get_num();
			break;
	}

	for (int i = 0; i < count; ++i) {
		switch (material_index) {
			case MATERIAL_INDEX_TERRAIN:
				mat = material_get(i);
				break;
			case MATERIAL_INDEX_LIQUID:
				mat = liquid_material_get(i);
				break;
			case MATERIAL_INDEX_PROP:
				mat = prop_material_get(i);
				break;
		}

		Ref<SpatialMaterial> spmat;

		switch (material_index) {
			case MATERIAL_INDEX_TERRAIN:
				spmat = material_get(i);
				break;
			case MATERIAL_INDEX_LIQUID:
				spmat = liquid_material_get(i);
				break;
			case MATERIAL_INDEX_PROP:
				spmat = prop_material_get(i);
				break;
		}

		if (spmat.is_valid()) {
			spmat->set_texture(SpatialMaterial::TEXTURE_ALBEDO, texture);
			return;
		}

		Ref<ShaderMaterial> shmat;

		switch (material_index) {
			case MATERIAL_INDEX_TERRAIN:
				shmat = material_get(i);
				break;
			case MATERIAL_INDEX_LIQUID:
				shmat = liquid_material_get(i);
				break;
			case MATERIAL_INDEX_PROP:
				shmat = prop_material_get(i);
				break;
		}

		if (shmat.is_valid()) {
			shmat->set_shader_param("texture_albedo", texture);
		}
	}
	*/
}

TerrainLibraryMergerPCM::TerrainLibraryMergerPCM() {
	_packer.instance();

	_packer->set_texture_flags(Texture::FLAG_MIPMAPS | Texture::FLAG_FILTER);

	_packer->set_max_atlas_size(1024);
	_packer->set_keep_original_atlases(false);
	_packer->set_margin(0);

	_prop_packer.instance();

	_prop_packer->set_texture_flags(Texture::FLAG_MIPMAPS | Texture::FLAG_FILTER);

	_prop_packer->set_max_atlas_size(1024);
	_prop_packer->set_keep_original_atlases(false);
	_prop_packer->set_margin(0);
}

TerrainLibraryMergerPCM::~TerrainLibraryMergerPCM() {
	for (int i = 0; i < _terra_surfaces.size(); ++i) {
		Ref<TerrainSurface> surface = _terra_surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(Ref<TerrainLibraryMergerPCM>());
		}
	}

	_terra_surfaces.clear();

	_packer->clear();
	_packer.unref();

	_prop_packer->clear();
	_prop_packer.unref();
}

#ifdef PROPS_PRESENT
bool TerrainLibraryMergerPCM::process_prop_textures(Ref<PropData> prop) {
	/*
	if (!prop.is_valid()) {
		return false;
	}

	bool texture_added = false;

	for (int i = 0; i < prop->get_prop_count(); ++i) {
		Ref<PropDataMeshData> pdm = prop->get_prop(i);

		if (pdm.is_valid()) {
			Ref<Texture> tex = pdm->get_texture();

			if (!tex.is_valid())
				continue;

			if (!_prop_packer->contains_texture(tex)) {
				_prop_packer->add_texture(tex);
				texture_added = true;
			}
		}

		Ref<PropDataProp> pdp = prop->get_prop(i);

		if (pdp.is_valid()) {
			if (process_prop_textures(pdp))
				texture_added = true;
		}
	}

	return texture_added;
	*/

	return false;
}
#endif

void TerrainLibraryMergerPCM::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_texture_flags"), &TerrainLibraryMergerPCM::get_texture_flags);
	ClassDB::bind_method(D_METHOD("set_texture_flags", "flags"), &TerrainLibraryMergerPCM::set_texture_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "texture_flags", PROPERTY_HINT_FLAGS, "Mipmaps,Repeat,Filter,Anisotropic Linear,Convert to Linear,Mirrored Repeat,Video Surface"), "set_texture_flags", "get_texture_flags");

	ClassDB::bind_method(D_METHOD("get_max_atlas_size"), &TerrainLibraryMergerPCM::get_max_atlas_size);
	ClassDB::bind_method(D_METHOD("set_max_atlas_size", "size"), &TerrainLibraryMergerPCM::set_max_atlas_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_atlas_size"), "set_max_atlas_size", "get_max_atlas_size");

	ClassDB::bind_method(D_METHOD("get_keep_original_atlases"), &TerrainLibraryMergerPCM::get_keep_original_atlases);
	ClassDB::bind_method(D_METHOD("set_keep_original_atlases", "value"), &TerrainLibraryMergerPCM::set_keep_original_atlases);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_original_atlases"), "set_keep_original_atlases", "get_keep_original_atlases");

	ClassDB::bind_method(D_METHOD("get_background_color"), &TerrainLibraryMergerPCM::get_background_color);
	ClassDB::bind_method(D_METHOD("set_background_color", "color"), &TerrainLibraryMergerPCM::set_background_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_background_color", "get_background_color");

	ClassDB::bind_method(D_METHOD("get_margin"), &TerrainLibraryMergerPCM::get_margin);
	ClassDB::bind_method(D_METHOD("set_margin", "size"), &TerrainLibraryMergerPCM::set_margin);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "margin"), "set_margin", "get_margin");

	ClassDB::bind_method(D_METHOD("get_terra_surfaces"), &TerrainLibraryMergerPCM::get_terra_surfaces);
	ClassDB::bind_method(D_METHOD("set_terra_surfaces"), &TerrainLibraryMergerPCM::set_terra_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "terra_surfaces", PROPERTY_HINT_NONE, "23/19:TerrainSurfaceMerger", PROPERTY_USAGE_DEFAULT, "TerrainSurfaceMerger"), "set_terra_surfaces", "get_terra_surfaces");

#ifdef PROPS_PRESENT
	ClassDB::bind_method(D_METHOD("get_props"), &TerrainLibraryMergerPCM::get_props);
	ClassDB::bind_method(D_METHOD("set_props"), &TerrainLibraryMergerPCM::set_props);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "props", PROPERTY_HINT_NONE, "23/19:PropData", PROPERTY_USAGE_DEFAULT, "PropData"), "set_props", "get_props");

	ClassDB::bind_method(D_METHOD("get_prop_uv_rect", "texture"), &TerrainLibraryMergerPCM::get_prop_uv_rect);

	ClassDB::bind_method(D_METHOD("get_prop_packer"), &TerrainLibraryMergerPCM::get_prop_packer);
#endif

	ClassDB::bind_method(D_METHOD("_setup_material_albedo", "material_index", "texture"), &TerrainLibraryMergerPCM::_setup_material_albedo);
}
