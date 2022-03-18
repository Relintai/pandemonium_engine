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

#include "terrain_2d_surface_merger.h"

#include "terrain_2d_library_merger.h"

#include "../defines.h"

Ref<AtlasTexture> Terrain2DSurfaceMerger::get_region() {
	return _region;
}
void Terrain2DSurfaceMerger::set_region(const Ref<AtlasTexture> &texture) {
	_region = texture;
}

Ref<Texture> Terrain2DSurfaceMerger::get_texture() {
	return _texture;
}
void Terrain2DSurfaceMerger::set_texture(const Ref<Texture> &texture) {
	_texture = texture;
}

void Terrain2DSurfaceMerger::refresh_rects() {
	if (!_region.is_valid()) {
		_rect = Rect2();
		return;
	}

	Ref<AtlasTexture> at = _region;
	Ref<Texture> tex = at->get_atlas();

	if (!tex.is_valid()) {
		_rect = Rect2();
		return;
	}

	Rect2 region = at->get_region();
	float w = tex->get_width();
	float h = tex->get_height();

	Rect2 r;

	r.position.x = region.position.x / w;
	r.position.y = region.position.y / h;
	r.size.x = region.size.x / w;
	r.size.y = region.size.y / h;

	_rect = r;
}

Terrain2DSurfaceMerger::Terrain2DSurfaceMerger() {
}

Terrain2DSurfaceMerger::~Terrain2DSurfaceMerger() {
	_region.unref();
	_texture.unref();
}

void Terrain2DSurfaceMerger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_region"), &Terrain2DSurfaceMerger::get_region);
	ClassDB::bind_method(D_METHOD("set_region", "texture"), &Terrain2DSurfaceMerger::set_region);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "region", PROPERTY_HINT_RESOURCE_TYPE, "AtlasTexture", 0), "set_region", "get_region");

	ClassDB::bind_method(D_METHOD("get_texture"), &Terrain2DSurfaceMerger::get_texture);
	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &Terrain2DSurfaceMerger::set_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
}
