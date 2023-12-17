

#include "terrain_surface_merger.h"

#include "terrain_library_merger.h"

Ref<AtlasTexture> TerrainSurfaceMerger::get_region(const TerrainSurfaceSides side) {
	return _regions[side];
}
void TerrainSurfaceMerger::set_region(const TerrainSurfaceSides side, const Ref<AtlasTexture> &texture) {
	_regions[side] = texture;
}

Ref<Texture> TerrainSurfaceMerger::get_texture(const TerrainSurfaceSides side) {
	return _textures[side];
}
void TerrainSurfaceMerger::set_texture(const TerrainSurfaceSides side, const Ref<Texture> &texture) {
	_textures[side] = texture;
}

void TerrainSurfaceMerger::refresh_rects() {
	for (int i = 0; i < TERRAIN_SIDES_COUNT; ++i) {
		if (!_regions[i].is_valid()) {
			_rects[i] = Rect2();
			continue;
		}

		Ref<AtlasTexture> at = _regions[i];
		Ref<Texture> tex = at->get_atlas();

		if (!tex.is_valid()) {
			_rects[i] = Rect2();
			continue;
		}

		Rect2 region = at->get_region();
		float w = tex->get_width();
		float h = tex->get_height();

		Rect2 r;

		r.position.x = region.position.x / w;
		r.position.y = region.position.y / h;
		r.size.x = region.size.x / w;
		r.size.y = region.size.y / h;

		_rects[i] = r;
	}
}

TerrainSurfaceMerger::TerrainSurfaceMerger() {
	for (int i = 0; i < TERRAIN_SIDES_COUNT; ++i) {
		_regions[i].unref();
		_textures[i].unref();
	}
}

TerrainSurfaceMerger::~TerrainSurfaceMerger() {
}

void TerrainSurfaceMerger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_region", "side"), &TerrainSurfaceMerger::get_region);
	ClassDB::bind_method(D_METHOD("set_region", "side", "texture"), &TerrainSurfaceMerger::set_region);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "region_top", PROPERTY_HINT_RESOURCE_TYPE, "AtlasTexture", 0), "set_region", "get_region", TERRAIN_SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "region_bottom", PROPERTY_HINT_RESOURCE_TYPE, "AtlasTexture", 0), "set_region", "get_region", TERRAIN_SIDE_BOTTOM);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "region_side", PROPERTY_HINT_RESOURCE_TYPE, "AtlasTexture", 0), "set_region", "get_region", TERRAIN_SIDE_SIDE);

	ClassDB::bind_method(D_METHOD("get_texture", "side"), &TerrainSurfaceMerger::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "side", "texture"), &TerrainSurfaceMerger::set_texture);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "texture_top", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture", TERRAIN_SIDE_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "texture_bottom", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture", TERRAIN_SIDE_BOTTOM);
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "texture_side", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture", TERRAIN_SIDE_SIDE);
}
