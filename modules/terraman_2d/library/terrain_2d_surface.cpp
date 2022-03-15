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

#include "terrain_2d_surface.h"

int Terrain2DSurface::get_id() const {
	return _id;
}
void Terrain2DSurface::set_id(const int value) {
	_id = value;
}

Rect2 Terrain2DSurface::get_rect() const {
	return _rect;
}
void Terrain2DSurface::set_rect(const Rect2 &rect) {
	_rect = rect;
}

Ref<Terrain2DLibrary> Terrain2DSurface::get_library() const {
	return Ref<Terrain2DLibrary>(_library);
}

void Terrain2DSurface::set_library(Ref<Terrain2DLibrary> library) {
	_library = (*library);
}

_FORCE_INLINE_ Vector2 Terrain2DSurface::transform_uv(const Vector2 &p_uv) const {
	Vector2 uv = p_uv;

	Rect2 r = _rect;

	uv.x *= r.size.x;
	uv.y *= r.size.y;
	uv.x += r.position.x;
	uv.y += r.position.y;

	return uv;
}

_FORCE_INLINE_ Vector2 Terrain2DSurface::transform_uv_scaled(const Vector2 &p_uv, const int p_current_x, const int p_current_y, const int p_max) const {
	Vector2 uv = p_uv;

	Rect2 r = _rect;

	float sizex = r.size.x / static_cast<float>(p_max);
	float sizey = r.size.y / static_cast<float>(p_max);

	uv.x *= sizex;
	uv.y *= sizey;

	uv.x += r.position.x + sizex * p_current_x;
	uv.y += r.position.y + sizey * p_current_y;

	return uv;
}

void Terrain2DSurface::refresh_rects() {
}

Terrain2DSurface::Terrain2DSurface() {
	_id = 0;
	_mesher_index = 0;
	_transparent = false;
	_liquid = false;
	_library = NULL;
}

Terrain2DSurface::~Terrain2DSurface() {
	_library = NULL;
}

void Terrain2DSurface::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &Terrain2DSurface::get_id);
	ClassDB::bind_method(D_METHOD("set_id", "value"), &Terrain2DSurface::set_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "voxel_name"), "set_name", "get_name");

	ClassDB::bind_method(D_METHOD("get_rect"), &Terrain2DSurface::get_rect);
	ClassDB::bind_method(D_METHOD("set_rect", "rect"), &Terrain2DSurface::set_rect);

	ClassDB::bind_method(D_METHOD("transform_uv", "uv"), &Terrain2DSurface::transform_uv);
	ClassDB::bind_method(D_METHOD("transform_uv_scaled", "uv", "p_current_x", "p_current_y", "max"), &Terrain2DSurface::transform_uv_scaled);

	ClassDB::bind_method(D_METHOD("refresh_rects"), &Terrain2DSurface::refresh_rects);
}
