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

#include "prop_data_collision_object.h"

#include "prop_data.h"

#include "scene/3d/collision_shape.h"
#include "scene/3d/physics_body.h"

uint32_t PropDataCollisionObject::get_collision_layer() const {
	return _collision_layer;
}
void PropDataCollisionObject::set_collision_layer(const uint32_t p_layer) {
	_collision_layer = p_layer;
}

uint32_t PropDataCollisionObject::get_collision_mask() const {
	return _collision_mask;
}
void PropDataCollisionObject::set_collision_mask(const uint32_t p_mask) {
	_collision_mask = p_mask;
}

int PropDataCollisionObject::get_collision_shape_count() const {
	return _shapes.size();
}
void PropDataCollisionObject::set_collision_shape_count(const int p_count) {
	_shapes.resize(p_count);
}

Transform PropDataCollisionObject::get_collision_shape_transform(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, _shapes.size(), Transform());

	return _shapes[p_index].transform;
}
void PropDataCollisionObject::set_collision_shape_transform(const int p_index, const Transform &p_transform) {
	ERR_FAIL_INDEX(p_index, _shapes.size());

	_shapes.write[p_index].transform = p_transform;
}

Ref<Shape> PropDataCollisionObject::get_collision_shape(const int p_index) const {
	ERR_FAIL_INDEX_V(p_index, _shapes.size(), Ref<Shape>());

	return _shapes[p_index].shape;
}
void PropDataCollisionObject::set_collision_shape(const int p_index, const Ref<Shape> &p_shape) {
	ERR_FAIL_INDEX(p_index, _shapes.size());

	_shapes.write[p_index].shape = p_shape;
}

void PropDataCollisionObject::add_collision_shape(const Transform &p_transform, const Ref<Shape> &p_shape) {
	ShapeEntry e;
	e.transform = p_transform;
	e.shape = p_shape;

	_shapes.push_back(e);

	property_list_changed_notify();
}
void PropDataCollisionObject::remove_collision_shape(const int p_index) {
	ERR_FAIL_INDEX(p_index, _shapes.size());

	_shapes.remove(p_index);

	property_list_changed_notify();
}

bool PropDataCollisionObject::_processor_evaluate_children() {
	return false;
}

void PropDataCollisionObject::processor_process_collision_objects(Node *node, const Transform &transform) {
	int cc = node->get_child_count();
	for (int i = 0; i < cc; ++i) {
		Node *c = node->get_child(i);

		CollisionShape *cs = Object::cast_to<CollisionShape>(c);

		if (!cs) {
			continue;
		}

		Transform current_transform = transform * cs->get_transform();
		Ref<Shape> shape = cs->get_shape();

		if (shape.is_valid()) {
			add_collision_shape(current_transform, shape);
		}

		if (c->get_child_count() > 0) {
			processor_process_collision_objects(c, current_transform);
		}
	}
}

void PropDataCollisionObject::processor_create_shapes_for(Node *root) {
	for (int i = 0; i < _shapes.size(); ++i) {
		const ShapeEntry &e = _shapes[i];

		if (!e.shape.is_valid()) {
			continue;
		}

		CollisionShape *cs = memnew(CollisionShape);
		cs->set_transform(e.transform);
		cs->set_shape(e.shape);
		root->add_child(cs);
	}
}

PropDataCollisionObject::PropDataCollisionObject() {
	_collision_layer = 0;
	_collision_mask = 0;
}
PropDataCollisionObject::~PropDataCollisionObject() {
}

bool PropDataCollisionObject::_set(const StringName &p_name, const Variant &p_value) {
	String p = p_name;

	if (!p.begins_with("shapes/")) {
		return false;
	}

	int index = p.get_slicec('/', 1).to_int();

	String prop = p.get_slicec('/', 2);

	if (prop == "transform") {
		_shapes.write[index].transform = p_value;
		return true;
	} else if (prop == "shape") {
		_shapes.write[index].shape = p_value;
		return true;
	}

	return false;
}
bool PropDataCollisionObject::_get(const StringName &p_name, Variant &r_ret) const {
	String p = p_name;

	if (!p.begins_with("shapes/")) {
		return false;
	}

	int index = p.get_slicec('/', 1).to_int();

	String prop = p.get_slicec('/', 2);

	if (prop == "transform") {
		r_ret = _shapes[index].transform;
		return true;
	} else if (prop == "shape") {
		r_ret = _shapes[index].shape;
		return true;
	}

	return false;
}
void PropDataCollisionObject::_get_property_list(List<PropertyInfo> *p_list) const {
	for (int i = 0; i < _shapes.size(); ++i) {
		p_list->push_back(PropertyInfo(Variant::TRANSFORM, "shapes/" + itos(i) + "/transform"));
		p_list->push_back(PropertyInfo(Variant::OBJECT, "shapes/" + itos(i) + "/shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape"));
	}
}

void PropDataCollisionObject::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_collision_layer"), &PropDataCollisionObject::get_collision_layer);
	ClassDB::bind_method(D_METHOD("set_collision_layer", "layer"), &PropDataCollisionObject::set_collision_layer);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_layer", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_layer", "get_collision_layer");

	ClassDB::bind_method(D_METHOD("get_collision_mask"), &PropDataCollisionObject::get_collision_mask);
	ClassDB::bind_method(D_METHOD("set_collision_mask", "value"), &PropDataCollisionObject::set_collision_mask);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_mask", "get_collision_mask");

	ClassDB::bind_method(D_METHOD("get_collision_shape_count"), &PropDataCollisionObject::get_collision_shape_count);
	ClassDB::bind_method(D_METHOD("set_collision_shape_count", "value"), &PropDataCollisionObject::set_collision_shape_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_shape_count", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_collision_shape_count", "get_collision_shape_count");

	ClassDB::bind_method(D_METHOD("get_collision_shape_transform"), &PropDataCollisionObject::get_collision_shape_transform);
	ClassDB::bind_method(D_METHOD("set_collision_shape_transform", "index", "transform"), &PropDataCollisionObject::set_collision_shape_transform);

	ClassDB::bind_method(D_METHOD("get_collision_shape"), &PropDataCollisionObject::get_collision_shape);
	ClassDB::bind_method(D_METHOD("set_collision_shape", "index", "shape"), &PropDataCollisionObject::set_collision_shape);

	ClassDB::bind_method(D_METHOD("add_collision_shape", "transform", "shape"), &PropDataCollisionObject::add_collision_shape);
	ClassDB::bind_method(D_METHOD("remove_collision_shape", "index"), &PropDataCollisionObject::remove_collision_shape);
}
