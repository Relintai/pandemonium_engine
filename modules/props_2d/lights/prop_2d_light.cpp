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

#include "prop_2d_light.h"

Vector2 Prop2DLight::get_position() {
	return _position;
}
void Prop2DLight::set_position(const Vector2 &pos) {
	_position = pos;
}

Color Prop2DLight::get_color() const {
	return _color;
}
void Prop2DLight::set_color(const Color &color) {
	_color = color;
}

float Prop2DLight::get_size() const {
	return _size;
}
void Prop2DLight::set_size(const float size) {
	_size = size;
}

Prop2DLight::Prop2DLight() {
	_size = 0;
}

Prop2DLight::~Prop2DLight() {
}

void Prop2DLight::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_position"), &Prop2DLight::get_position);
	ClassDB::bind_method(D_METHOD("set_position"), &Prop2DLight::set_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "world_position"), "set_position", "get_position");

	ClassDB::bind_method(D_METHOD("get_color"), &Prop2DLight::get_color);
	ClassDB::bind_method(D_METHOD("set_color"), &Prop2DLight::set_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("get_size"), &Prop2DLight::get_size);
	ClassDB::bind_method(D_METHOD("set_size"), &Prop2DLight::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");
}
