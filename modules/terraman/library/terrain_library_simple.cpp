/*************************************************************************/
/*  terrain_library_simple.cpp                                           */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "terrain_library_simple.h"

#include "../defines.h"

int TerrainLibrarySimple::get_atlas_columns() const {
	return _atlas_columns;
}

void TerrainLibrarySimple::set_atlas_columns(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_columns = s;
}

int TerrainLibrarySimple::get_atlas_rows() const {
	return _atlas_rows;
}

void TerrainLibrarySimple::set_atlas_rows(int s) {
	ERR_FAIL_COND(s < 0);
	_atlas_rows = s;
}

//Surfaces
Ref<TerrainSurface> TerrainLibrarySimple::terra_surface_get(const int index) {
	ERR_FAIL_INDEX_V(index, _terra_surfaces.size(), Ref<TerrainSurface>(NULL));

	return _terra_surfaces[index];
}

void TerrainLibrarySimple::terra_surface_add(Ref<TerrainSurface> value) {
	ERR_FAIL_COND(!value.is_valid());

	value->set_library(Ref<TerrainLibrarySimple>(this));
	value->set_id(_terra_surfaces.size());

	_terra_surfaces.push_back(value);
}

void TerrainLibrarySimple::terra_surface_set(const int index, Ref<TerrainSurface> value) {
	ERR_FAIL_COND(index < 0);

	if (_terra_surfaces.size() < index) {
		_terra_surfaces.resize(index + 1);
	}

	if (_terra_surfaces[index].is_valid()) {
		_terra_surfaces.get(index)->set_library(Ref<TerrainLibrarySimple>(NULL));
	}

	if (value.is_valid()) {
		value->set_library(Ref<TerrainLibrarySimple>(this));

		_terra_surfaces.set(index, value);
	}
}

void TerrainLibrarySimple::terra_surface_remove(const int index) {
	_terra_surfaces.remove(index);
}

int TerrainLibrarySimple::terra_surface_get_num() const {
	return _terra_surfaces.size();
}

void TerrainLibrarySimple::terra_surfaces_clear() {
	_terra_surfaces.clear();
}

Vector<Variant> TerrainLibrarySimple::get_terra_surfaces() {
	VARIANT_ARRAY_GET(_terra_surfaces);
}

void TerrainLibrarySimple::set_terra_surfaces(const Vector<Variant> &surfaces) {
	_terra_surfaces.clear();

	for (int i = 0; i < surfaces.size(); i++) {
		Ref<TerrainSurfaceSimple> surface = Ref<TerrainSurfaceSimple>(surfaces[i]);

		if (surface.is_valid()) {
			surface->set_library(this);
			surface->refresh_rects();
		}

		_terra_surfaces.push_back(surface);
	}

	set_initialized(true);
}

void TerrainLibrarySimple::refresh_rects() {
	for (int i = 0; i < _terra_surfaces.size(); i++) {
		Ref<TerrainSurfaceSimple> surface = Ref<TerrainSurfaceSimple>(_terra_surfaces[i]);

		if (surface.is_valid()) {
			surface->refresh_rects();
		}
	}
}

TerrainLibrarySimple::TerrainLibrarySimple() {
	_atlas_rows = 8;
	_atlas_columns = 8;
}

TerrainLibrarySimple::~TerrainLibrarySimple() {
	for (int i = 0; i < _terra_surfaces.size(); ++i) {
		Ref<TerrainSurface> surface = _terra_surfaces[i];

		if (surface.is_valid()) {
			surface->set_library(Ref<TerrainLibrarySimple>());
		}
	}

	_terra_surfaces.clear();
}

void TerrainLibrarySimple::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_atlas_columns"), &TerrainLibrarySimple::get_atlas_columns);
	ClassDB::bind_method(D_METHOD("set_atlas_columns", "value"), &TerrainLibrarySimple::set_atlas_columns);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_columns"), "set_atlas_columns", "get_atlas_columns");

	ClassDB::bind_method(D_METHOD("get_atlas_rows"), &TerrainLibrarySimple::get_atlas_rows);
	ClassDB::bind_method(D_METHOD("set_atlas_rows", "value"), &TerrainLibrarySimple::set_atlas_rows);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_rows"), "set_atlas_rows", "get_atlas_rows");

	ClassDB::bind_method(D_METHOD("get_terra_surfaces"), &TerrainLibrarySimple::get_terra_surfaces);
	ClassDB::bind_method(D_METHOD("set_terra_surfaces"), &TerrainLibrarySimple::set_terra_surfaces);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "terra_surfaces", PROPERTY_HINT_NONE, "23/20:TerrainSurfaceSimple", PROPERTY_USAGE_DEFAULT, "TerrainSurfaceSimple"), "set_terra_surfaces", "get_terra_surfaces");
}
