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

#include "prop_data_portal.h"

#include "prop_data.h"

#include "scene/3d/portal.h"

bool PropDataPortal::get_is_active() const {
	return _is_active;
}
void PropDataPortal::set_is_active(bool p_active) {
	_is_active = p_active;
}

bool PropDataPortal::get_is_two_way() const {
	return _is_two_way;
}
void PropDataPortal::set_is_two_way(bool p_two_way) {
	_is_two_way = p_two_way;
}

bool PropDataPortal::get_use_default_margin() const {
	return _use_default_margin;
}
void PropDataPortal::set_use_default_margin(bool p_use) {
	_use_default_margin = p_use;
}

real_t PropDataPortal::get_portal_margin() const {
	return _portal_margin;
}
void PropDataPortal::set_portal_margin(real_t p_margin) {
	_portal_margin = p_margin;
}

PoolVector<Vector2> PropDataPortal::get_points() const {
	return _points;
}
void PropDataPortal::set_points(const PoolVector<Vector2> &p_points) {
	_points = p_points;
}

bool PropDataPortal::_processor_handles(Node *node) {
	Portal *p = Object::cast_to<Portal>(node);

	return p;
}

void PropDataPortal::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	Portal *p = Object::cast_to<Portal>(node);

	ERR_FAIL_COND(!p);

	Ref<PropDataPortal> l;
	l.instance();
	l->set_is_active(p->get_portal_active());
	l->set_is_two_way(p->is_two_way());
	l->set_use_default_margin(p->get_use_default_margin());
	l->set_portal_margin(p->get_portal_margin());
	l->set_points(p->get_points());

	l->set_transform(transform * p->get_transform());
	prop_data->add_prop(l);
}

Node *PropDataPortal::_processor_get_node_for(const Transform &transform) {
	Portal *p = memnew(Portal);

	p->set_portal_active(get_is_active());
	p->set_two_way(get_is_two_way());
	p->set_use_default_margin(get_use_default_margin());
	p->set_portal_margin(get_portal_margin());
	p->set_points(get_points());

	p->set_transform(get_transform());

	return p;
}

PropDataPortal::PropDataPortal() {
	_is_active = true;
	_is_two_way = true;
	_use_default_margin = true;
	_portal_margin = 1;
}
PropDataPortal::~PropDataPortal() {
}

void PropDataPortal::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_active"), &PropDataPortal::get_is_active);
	ClassDB::bind_method(D_METHOD("set_is_active", "value"), &PropDataPortal::set_is_active);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_active"), "set_is_active", "get_is_active");

	ClassDB::bind_method(D_METHOD("get_is_two_way"), &PropDataPortal::get_is_two_way);
	ClassDB::bind_method(D_METHOD("set_is_two_way", "value"), &PropDataPortal::set_is_two_way);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_two_way"), "set_is_two_way", "get_is_two_way");

	ClassDB::bind_method(D_METHOD("get_use_default_margin"), &PropDataPortal::get_use_default_margin);
	ClassDB::bind_method(D_METHOD("set_use_default_margin", "value"), &PropDataPortal::set_use_default_margin);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_default_margin"), "set_use_default_margin", "get_use_default_margin");

	ClassDB::bind_method(D_METHOD("get_portal_margin"), &PropDataPortal::get_portal_margin);
	ClassDB::bind_method(D_METHOD("set_portal_margin", "value"), &PropDataPortal::set_portal_margin);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "portal_margin"), "set_portal_margin", "get_portal_margin");

	ClassDB::bind_method(D_METHOD("get_points"), &PropDataPortal::get_points);
	ClassDB::bind_method(D_METHOD("set_points", "value"), &PropDataPortal::set_points);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "points"), "set_points", "get_points");
}
