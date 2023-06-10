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

#include "terrain_2d_library_simple.h"

#include "../defines.h"

int Terrain2DLibrarySimple::get_atlas_columns() const {
	return _atlas_columns;
}

void Terrain2DLibrarySimple::set_atlas_columns(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_columns = s;
}

int Terrain2DLibrarySimple::get_atlas_rows() const {
	return _atlas_rows;
}

void Terrain2DLibrarySimple::set_atlas_rows(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_rows = s;
}

//Surfaces
Ref<Terrain2DSurface> Terrain2DLibrarySimple::terra_surface_get(const int index) {
	ERR_FAIL_INDEX_V(index, _terra_surfaces.size(), Ref<Terrain2DSurface>(NULL));

	return _terra_surfaces[index];
}

void Terrain2DLibrarySimple::terra_surface_add(Ref<Terrain2DSurface> value) {
	ERR_FAIL_COND(!value.is_valid());

	value->set_library(Ref<Terrain2DLibrarySimple>(this));
	value->set_id(_terra_surfaces.size());

	_terra_surfaces.push_back(value);
}

void Terrain2DLibrarySimple::terra_surface_set(const int index, Ref<Terrain2DSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_terra_surfaces.size() < index) {
		_terra_surfaces.resize(index + 1);
	}

	if (_terra_surfaces[index].is_valid()) {
		_terra_surfaces.get(index)->set_library(Ref<Terrain2DLibrarySimple>(NULL));
	}

	if (value.is_valid()) {
		value->set_library(Ref<Terrain2DLibrarySimple>(this));

		_terra_surfaces.set(index, value);
	}
}

void Terrain2DLibrarySimple::terra_surface_remove(const int index) {
	_terra_surfaces.remove(index);
}

int Terrain2DLibrarySimple::terra_surface_get_num() const {
	return _terra_surfaces.size();
}

void Terrain2DLibrarySimple::terra_surfaces_clear() {
	_terra_surfaces.clear();
}

Vector<Variant> Terrain2DLibrarySimple::get_terra_surfaces() {
	VARIANT_ARRAY_GET(_terra_surfaces);
}

void Terrain2DLibrarySimple::set_terra_surfaces(const Vector<Variant> &surfaces) {
	_terra_surfaces.clear();

	for (int i = 0; i < surfaces.size(); i++) {
		Ref<Terrain2DSurfaceSimple> surface = Ref<Terrain2DSurfaceSimple>(surfaces[i]);

		if (surface.is_valid()) {
			surface->set_library(this);
			surface->refresh_rects();
		}

		_terra_surfaces.push_back(surface);
	}

	set_initialized(true);
}

void Terrain2DLibrarySimple::refresh_rects() {
	for (int i = 0; i < _terra_surfaces.size(); i++) {
		Ref<Terrain2DSurfaceSimple> surface = Ref<Terrain2DSurfaceSimple>(_terra_surfaces[i]);

		if (surface.is_valid()) {
			surface->refresh_rects();
		}
	}
}

Terrain2DLibrarySimple::Terrain2DLibrarySimple() {
	_atlas_rows = 8;
	_atlas_columns = 8;
}

Terrain2DLibrarySimple::~Terrain2DLibrarySimple() {
	for (int i = 0; i < _terra_surfaces.size(); ++i) {
		Ref<Terrain2DSurface> surface = _terra_surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(Ref<Terrain2DLibrarySimple>());
		}
	}

	_terra_surfaces.clear();
}

void Terrain2DLibrarySimple::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_atlas_columns"), &Terrain2DLibrarySimple::get_atlas_columns);
	ClassDB::bind_method(D_METHOD("set_atlas_columns", "value"), &Terrain2DLibrarySimple::set_atlas_columns);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_columns"), "set_atlas_columns", "get_atlas_columns");

	ClassDB::bind_method(D_METHOD("get_atlas_rows"), &Terrain2DLibrarySimple::get_atlas_rows);
	ClassDB::bind_method(D_METHOD("set_atlas_rows", "value"), &Terrain2DLibrarySimple::set_atlas_rows);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_rows"), "set_atlas_rows", "get_atlas_rows");

	ClassDB::bind_method(D_METHOD("get_terra_surfaces"), &Terrain2DLibrarySimple::get_terra_surfaces);
	ClassDB::bind_method(D_METHOD("set_terra_surfaces"), &Terrain2DLibrarySimple::set_terra_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "terra_surfaces", PROPERTY_HINT_NONE, "23/20:Terrain2DSurfaceSimple", PROPERTY_USAGE_DEFAULT, "Terrain2DSurfaceSimple"), "set_terra_surfaces", "get_terra_surfaces");
}
