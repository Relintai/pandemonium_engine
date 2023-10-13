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

#include "prop_data_static_body.h"

#include "prop_data.h"

#include "scene/3d/physics_body.h"
#include "scene/resources/physics_material.h"

Ref<PhysicsMaterial> PropDataStaticBody::get_physics_material_override() const {
	return _physics_material_override;
}
void PropDataStaticBody::set_physics_material_override(const Ref<PhysicsMaterial> &p_material) {
	_physics_material_override = p_material;
}

Vector3 PropDataStaticBody::get_constant_linear_velocity() const {
	return _constant_linear_velocity;
}
void PropDataStaticBody::set_constant_linear_velocity(const Vector3 &p_value) {
	_constant_linear_velocity = p_value;
}

Vector3 PropDataStaticBody::get_constant_angular_velocity() const {
	return _constant_angular_velocity;
}
void PropDataStaticBody::set_constant_angular_velocity(const Vector3 &p_value) {
	_constant_angular_velocity = p_value;
}

bool PropDataStaticBody::_processor_handles(Node *node) {
	return Object::cast_to<StaticBody>(node);
}

void PropDataStaticBody::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	StaticBody *sb = Object::cast_to<StaticBody>(node);

	Ref<PropDataStaticBody> c;
	c.instance();

	c->set_physics_material_override(sb->get_physics_material_override());
	c->set_constant_linear_velocity(sb->get_constant_linear_velocity());
	c->set_constant_angular_velocity(sb->get_constant_angular_velocity());
	c->set_transform(transform);

	c->set_collision_layer(sb->get_collision_layer());
	c->set_collision_mask(sb->get_collision_mask());

	c->processor_process_collision_objects(node, Transform());

	prop_data->add_prop(c);
}

Node *PropDataStaticBody::_processor_get_node_for(const Transform &transform) {
	StaticBody *sb = memnew(StaticBody);
	sb->set_transform(get_transform());
	sb->set_physics_material_override(_physics_material_override);
	sb->set_constant_linear_velocity(_constant_linear_velocity);
	sb->set_constant_angular_velocity(_constant_angular_velocity);

	processor_create_shapes_for(sb);

	return sb;
}

PropDataStaticBody::PropDataStaticBody() {
}
PropDataStaticBody::~PropDataStaticBody() {
}

void PropDataStaticBody::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_physics_material_override"), &PropDataStaticBody::get_physics_material_override);
	ClassDB::bind_method(D_METHOD("set_physics_material_override", "material"), &PropDataStaticBody::set_physics_material_override);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "physics_material_override", PROPERTY_HINT_RESOURCE_TYPE, "PhysicsMaterial"), "set_physics_material_override", "get_physics_material_override");

	ClassDB::bind_method(D_METHOD("get_constant_linear_velocity"), &PropDataStaticBody::get_constant_linear_velocity);
	ClassDB::bind_method(D_METHOD("set_constant_linear_velocity", "layer"), &PropDataStaticBody::set_constant_linear_velocity);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "constant_linear_velocity"), "set_constant_linear_velocity", "get_constant_linear_velocity");

	ClassDB::bind_method(D_METHOD("get_constant_angular_velocity"), &PropDataStaticBody::get_constant_angular_velocity);
	ClassDB::bind_method(D_METHOD("set_constant_angular_velocity", "layer"), &PropDataStaticBody::set_constant_angular_velocity);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "constant_angular_velocity"), "set_constant_angular_velocity", "get_constant_angular_velocity");
}
