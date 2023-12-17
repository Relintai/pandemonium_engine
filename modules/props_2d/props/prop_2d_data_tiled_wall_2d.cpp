/*************************************************************************/
/*  prop_2d_data_tiled_wall_2d.cpp                                       */
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

#include "prop_2d_data_tiled_wall_2d.h"

#include "../tiled_wall/tiled_wall_2d.h"
#include "../tiled_wall/tiled_wall_2d_data.h"

#include "prop_2d_data.h"

int Prop2DDataTiledWall2D::get_width() const {
	return _width;
}
void Prop2DDataTiledWall2D::set_width(const int value) {
	_width = value;
}

int Prop2DDataTiledWall2D::get_heigth() const {
	return _height;
}
void Prop2DDataTiledWall2D::set_heigth(const int value) {
	_height = value;
}

Ref<TiledWall2DData> Prop2DDataTiledWall2D::get_data() {
	return _data;
}
void Prop2DDataTiledWall2D::set_data(const Ref<TiledWall2DData> &data) {
	_data = data;
}

bool Prop2DDataTiledWall2D::get_collision() const {
	return _collision;
}
void Prop2DDataTiledWall2D::set_collision(const bool value) {
	_collision = value;
}

bool Prop2DDataTiledWall2D::_processor_handles(Node *node) {
	TiledWall2D *t = Object::cast_to<TiledWall2D>(node);

	return t;
}

void Prop2DDataTiledWall2D::_processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry) {
	TiledWall2D *t = Object::cast_to<TiledWall2D>(node);

	ERR_FAIL_COND(!t);

	Ref<Prop2DDataTiledWall2D> tw;

	if (entry.is_valid()) {
		tw = entry;
	} else {
		tw.instance();
	}

	tw->set_width(t->get_width());
	tw->set_heigth(t->get_heigth());
	tw->set_data(t->get_data());
	//tw->set_collision(t->get_collision());

	Prop2DDataEntry::_processor_process(prop_data, node, transform, tw);
}

Node *Prop2DDataTiledWall2D::_processor_get_node_for(const Transform2D &transform, Node *node) {
	TiledWall2D *t;

	if (node) {
		t = Object::cast_to<TiledWall2D>(node);
	} else {
		t = memnew(TiledWall2D());
	}

	t->set_width(get_width());
	t->set_heigth(get_heigth());
	//t->set_collision(get_collision());
	t->set_data(get_data());
	//t->set_transform(get_transform());

	return Prop2DDataEntry::_processor_get_node_for(transform, t);
}

Prop2DDataTiledWall2D::Prop2DDataTiledWall2D() {
	_width = 1;
	_height = 1;
	_collision = true;
}
Prop2DDataTiledWall2D::~Prop2DDataTiledWall2D() {
	_data.unref();
}

void Prop2DDataTiledWall2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_width"), &Prop2DDataTiledWall2D::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "value"), &Prop2DDataTiledWall2D::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "width"), "set_width", "get_width");

	ClassDB::bind_method(D_METHOD("get_heigth"), &Prop2DDataTiledWall2D::get_heigth);
	ClassDB::bind_method(D_METHOD("set_heigth", "value"), &Prop2DDataTiledWall2D::set_heigth);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "height"), "set_heigth", "get_heigth");

	ClassDB::bind_method(D_METHOD("get_data"), &Prop2DDataTiledWall2D::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "value"), &Prop2DDataTiledWall2D::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "TiledWall2DData"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_collision"), &Prop2DDataTiledWall2D::get_collision);
	ClassDB::bind_method(D_METHOD("set_collision", "value"), &Prop2DDataTiledWall2D::set_collision);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "collision"), "set_collision", "get_collision");
}
