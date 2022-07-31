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

#include "prop_data_tiled_wall.h"

#include "../tiled_wall/tiled_wall.h"
#include "../tiled_wall/tiled_wall_data.h"

#include "prop_data.h"

int PropDataTiledWall::get_width() const {
	return _width;
}
void PropDataTiledWall::set_width(const int value) {
	_width = value;
}

int PropDataTiledWall::get_heigth() const {
	return _height;
}
void PropDataTiledWall::set_heigth(const int value) {
	_height = value;
}

Ref<TiledWallData> PropDataTiledWall::get_data() {
	return _data;
}
void PropDataTiledWall::set_data(const Ref<TiledWallData> &data) {
	_data = data;
}

bool PropDataTiledWall::get_collision() const {
	return _collision;
}
void PropDataTiledWall::set_collision(const bool value) {
	_collision = value;
}

bool PropDataTiledWall::_processor_handles(Node *node) {
	TiledWall *t = Object::cast_to<TiledWall>(node);

	return t;
}

void PropDataTiledWall::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	TiledWall *t = Object::cast_to<TiledWall>(node);

	ERR_FAIL_COND(!t);

	Ref<PropDataTiledWall> tw;
	tw.instance();

	tw->set_width(t->get_width());
	tw->set_heigth(t->get_heigth());
	tw->set_data(t->get_data());
	tw->set_collision(t->get_collision());
	tw->set_transform(transform * t->get_transform());
	prop_data->add_prop(tw);
}

Node *PropDataTiledWall::_processor_get_node_for(const Transform &transform) {
	TiledWall *t = memnew(TiledWall);

	t->set_width(get_width());
	t->set_heigth(get_heigth());
	t->set_collision(get_collision());
	t->set_data(get_data());
	t->set_transform(get_transform());

	return t;
}

PropDataTiledWall::PropDataTiledWall() {
	_width = 1;
	_height = 1;
	_collision = true;
}
PropDataTiledWall::~PropDataTiledWall() {
	_data.unref();
}

void PropDataTiledWall::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_width"), &PropDataTiledWall::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "value"), &PropDataTiledWall::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "width"), "set_width", "get_width");

	ClassDB::bind_method(D_METHOD("get_heigth"), &PropDataTiledWall::get_heigth);
	ClassDB::bind_method(D_METHOD("set_heigth", "value"), &PropDataTiledWall::set_heigth);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "heigth"), "set_heigth", "get_heigth");

	ClassDB::bind_method(D_METHOD("get_data"), &PropDataTiledWall::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "value"), &PropDataTiledWall::set_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "TiledWallData"), "set_data", "get_data");

	ClassDB::bind_method(D_METHOD("get_collision"), &PropDataTiledWall::get_collision);
	ClassDB::bind_method(D_METHOD("set_collision", "value"), &PropDataTiledWall::set_collision);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "collision"), "set_collision", "get_collision");
}
