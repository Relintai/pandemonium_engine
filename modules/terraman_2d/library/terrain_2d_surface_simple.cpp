/*************************************************************************/
/*  terrain_2d_surface_simple.cpp                                        */
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

#include "terrain_2d_surface_simple.h"

#include "terrain_2d_library_simple.h"

int Terrain2DSurfaceSimple::get_atlas_x() const {
	return _atlas_position_x;
}
void Terrain2DSurfaceSimple::set_atlas_x(int value) {
	_atlas_position_x = value;
}

int Terrain2DSurfaceSimple::get_atlas_y() const {
	return _atlas_position_y;
}
void Terrain2DSurfaceSimple::set_atlas_y(int value) {
	_atlas_position_y = value;
}

void Terrain2DSurfaceSimple::refresh_rects() {
	Terrain2DLibrarySimple *lib = Object::cast_to<Terrain2DLibrarySimple>(_library);

	ERR_FAIL_COND(lib == NULL);

	float culomn = 1.0 / static_cast<float>(lib->get_atlas_columns());
	float row = 1.0 / static_cast<float>(lib->get_atlas_rows());

	Rect2 r;

	r.position.x = _atlas_position_x * culomn;
	r.position.y = _atlas_position_y * row;

	r.size.x = culomn;
	r.size.y = row;

	_rect = r;
}

Terrain2DSurfaceSimple::Terrain2DSurfaceSimple() {
	_atlas_position_x = 0;
	_atlas_position_y = 0;
}

Terrain2DSurfaceSimple::~Terrain2DSurfaceSimple() {
}

void Terrain2DSurfaceSimple::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_atlas_x"), &Terrain2DSurfaceSimple::get_atlas_x);
	ClassDB::bind_method(D_METHOD("set_atlas_x", "value"), &Terrain2DSurfaceSimple::set_atlas_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_x"), "set_atlas_x", "get_atlas_x");

	ClassDB::bind_method(D_METHOD("get_atlas_y"), &Terrain2DSurfaceSimple::get_atlas_y);
	ClassDB::bind_method(D_METHOD("set_atlas_y", "value"), &Terrain2DSurfaceSimple::set_atlas_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "atlas_y"), "set_atlas_y", "get_atlas_y");
}
