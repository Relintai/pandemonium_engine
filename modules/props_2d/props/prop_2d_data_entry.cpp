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

#include "prop_2d_data_entry.h"

#include "prop_2d_data.h"

#include "scene/main/node_2d.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_packer.h"
#endif

#include "../prop_2d_mesher.h"

Transform Prop2DDataEntry::get_transform() const {
	return Transform();
}
void Prop2DDataEntry::set_transform(const Transform &value) {
}

Transform2D Prop2DDataEntry::get_transform_2d() const {
	return _transform;
}
void Prop2DDataEntry::set_transform_2d(const Transform2D &value) {
	_transform = value;
}

int Prop2DDataEntry::get_z_index() const {
	return _z_index;
}
void Prop2DDataEntry::set_z_index(const int value) {
	_z_index = value;
}

bool Prop2DDataEntry::get_z_as_relative() const {
	return _z_as_relative;
}
void Prop2DDataEntry::set_z_as_relative(const bool value) {
	_z_as_relative = value;
}

bool Prop2DDataEntry::get_visible() const {
	return _visible;
}
void Prop2DDataEntry::set_visible(const bool value) {
	_visible = value;
}

Color Prop2DDataEntry::get_modulate() const {
	return _modulate;
}
void Prop2DDataEntry::set_modulate(const Color &value) {
	_modulate = value;
}

Color Prop2DDataEntry::get_self_modulate() const {
	return _self_modulate;
}
void Prop2DDataEntry::set_self_modulate(const Color &value) {
	_self_modulate = value;
}

bool Prop2DDataEntry::get_show_behind_parent() const {
	return _show_behind_parent;
}
void Prop2DDataEntry::set_show_behind_parent(const bool value) {
	_show_behind_parent = value;
}

int Prop2DDataEntry::get_light_mask() const {
	return _light_mask;
}
void Prop2DDataEntry::set_light_mask(const int value) {
	_light_mask = value;
}

Ref<Material> Prop2DDataEntry::get_material() const {
	return _material;
}
void Prop2DDataEntry::set_material(const Ref<Material> &value) {
	_material = value;
}

bool Prop2DDataEntry::get_use_parent_material() const {
	return _use_parent_material;
}
void Prop2DDataEntry::set_use_parent_material(const bool value) {
	_use_parent_material = value;
}

#ifdef MODULE_TEXTURE_PACKER_ENABLED
void Prop2DDataEntry::add_textures_into(Ref<TexturePacker> texture_packer) {
	if (has_method("_add_textures_into"))
		call("_add_textures_into", texture_packer);
}
#endif

bool Prop2DDataEntry::processor_handles(Node *node) {
	return call("_processor_handles", node);
}
void Prop2DDataEntry::processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform) {
	call("_processor_process", prop_data, node, transform);
}
Node *Prop2DDataEntry::processor_get_node_for(const Transform2D &transform) {
	return call("_processor_get_node_for", transform);
}
bool Prop2DDataEntry::processor_evaluate_children() {
	return call("_processor_evaluate_children");
}

bool Prop2DDataEntry::_processor_handles(Node *node) {
	return false;
}
void Prop2DDataEntry::_processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry) {
	Node2D *n = Object::cast_to<Node2D>(node);

	ERR_FAIL_COND(!n);

	Ref<Prop2DDataEntry> e;

	if (entry.is_valid()) {
		e = entry;
	} else {
		e.instance();
	}

	e->set_transform_2d(transform * n->get_transform());
	e->set_z_index(n->get_z_index());
	e->set_z_as_relative(n->is_z_relative());
	e->set_visible(n->is_visible());
	e->set_modulate(n->get_modulate());
	e->set_self_modulate(n->get_self_modulate());
	e->set_show_behind_parent(n->is_draw_behind_parent_enabled());
	e->set_light_mask(n->get_light_mask());
	e->set_material(n->get_material());
	e->set_use_parent_material(n->get_use_parent_material());

	prop_data->add_prop(e);
}
Node *Prop2DDataEntry::_processor_get_node_for(const Transform2D &transform, Node *node) {
	Node2D *n;

	if (node) {
		n = Object::cast_to<Node2D>(node);
	} else {
		n = memnew(Node2D());
	}

	ERR_FAIL_COND_V(!n, nullptr);

	n->set_transform(transform * get_transform_2d());

	n->set_z_index(get_z_index());
	n->set_z_as_relative(get_z_as_relative());
	n->set_visible(get_visible());
	n->set_modulate(get_modulate());
	n->set_self_modulate(get_self_modulate());
	n->set_draw_behind_parent(get_show_behind_parent());

	n->set_light_mask(get_light_mask());
	n->set_material(get_material());
	n->set_use_parent_material(get_use_parent_material());

	return n;
}
bool Prop2DDataEntry::_processor_evaluate_children() {
	return true;
}

Prop2DDataEntry::Prop2DDataEntry() {
	_z_index = 0;
	_z_as_relative = true;

	_visible = true;
	_modulate = Color(1, 1, 1, 1);
	_self_modulate = Color(1, 1, 1, 1);
	_show_behind_parent = false;

	_light_mask = 1;

	_use_parent_material = false;
}
Prop2DDataEntry::~Prop2DDataEntry() {
}

void Prop2DDataEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_transform"), &Prop2DDataEntry::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &Prop2DDataEntry::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "transform"), "set_transform", "get_transform");

	ClassDB::bind_method(D_METHOD("get_transform_2d"), &Prop2DDataEntry::get_transform_2d);
	ClassDB::bind_method(D_METHOD("set_transform_2d", "value"), &Prop2DDataEntry::set_transform_2d);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "transform_2d"), "set_transform_2d", "get_transform_2d");

	ClassDB::bind_method(D_METHOD("get_z_index"), &Prop2DDataEntry::get_z_index);
	ClassDB::bind_method(D_METHOD("set_z_index", "value"), &Prop2DDataEntry::set_z_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "z_index"), "set_z_index", "get_z_index");

	ClassDB::bind_method(D_METHOD("get_z_as_relative"), &Prop2DDataEntry::get_z_as_relative);
	ClassDB::bind_method(D_METHOD("set_z_as_relative", "value"), &Prop2DDataEntry::set_z_as_relative);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "z_as_relative"), "set_z_as_relative", "get_z_as_relative");

	ClassDB::bind_method(D_METHOD("get_visible"), &Prop2DDataEntry::get_visible);
	ClassDB::bind_method(D_METHOD("set_visible", "value"), &Prop2DDataEntry::set_visible);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "visible"), "set_visible", "get_visible");

	ClassDB::bind_method(D_METHOD("get_modulate"), &Prop2DDataEntry::get_modulate);
	ClassDB::bind_method(D_METHOD("set_modulate", "value"), &Prop2DDataEntry::set_modulate);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate"), "set_modulate", "get_modulate");

	ClassDB::bind_method(D_METHOD("get_self_modulate"), &Prop2DDataEntry::get_self_modulate);
	ClassDB::bind_method(D_METHOD("set_self_modulate", "value"), &Prop2DDataEntry::set_self_modulate);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "self_modulate"), "set_self_modulate", "get_self_modulate");

	ClassDB::bind_method(D_METHOD("get_show_behind_parent"), &Prop2DDataEntry::get_show_behind_parent);
	ClassDB::bind_method(D_METHOD("set_show_behind_parent", "value"), &Prop2DDataEntry::set_show_behind_parent);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "show_behind_parent"), "set_show_behind_parent", "get_show_behind_parent");

	ClassDB::bind_method(D_METHOD("get_light_mask"), &Prop2DDataEntry::get_light_mask);
	ClassDB::bind_method(D_METHOD("set_light_mask", "value"), &Prop2DDataEntry::set_light_mask);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "light_mask"), "set_light_mask", "get_light_mask");

	ClassDB::bind_method(D_METHOD("get_material"), &Prop2DDataEntry::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &Prop2DDataEntry::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "material"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_use_parent_material"), &Prop2DDataEntry::get_use_parent_material);
	ClassDB::bind_method(D_METHOD("set_use_parent_material", "value"), &Prop2DDataEntry::set_use_parent_material);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_parent_material"), "set_use_parent_material", "get_use_parent_material");

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	BIND_VMETHOD(MethodInfo("_add_textures_into", PropertyInfo(Variant::OBJECT, "texture_packer", PROPERTY_HINT_RESOURCE_TYPE, "TexturePacker")));

	ClassDB::bind_method(D_METHOD("add_textures_into", "texture_packer"), &Prop2DDataEntry::add_textures_into);
#endif

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "handles"), "_processor_handles"));
	BIND_VMETHOD(MethodInfo("_processor_process",
			PropertyInfo(Variant::OBJECT, "prop_data", PROPERTY_HINT_RESOURCE_TYPE, "Prop2DData"),
			PropertyInfo(Variant::OBJECT, "node", PROPERTY_HINT_RESOURCE_TYPE, "Node"),
			PropertyInfo(Variant::TRANSFORM, "transform"),
			PropertyInfo(Variant::OBJECT, "entry", PROPERTY_HINT_RESOURCE_TYPE, "Prop2DDataEntry")));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "node", PROPERTY_HINT_RESOURCE_TYPE, "Node"), "_processor_get_node_for",
			PropertyInfo(Variant::TRANSFORM, "transform"),
			PropertyInfo(Variant::OBJECT, "node", PROPERTY_HINT_RESOURCE_TYPE, "Node")));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "evaluate"), "_processor_evaluate_children"));

	ClassDB::bind_method(D_METHOD("processor_handles", "node"), &Prop2DDataEntry::processor_handles);
	ClassDB::bind_method(D_METHOD("processor_process", "prop_data", "node", "transform"), &Prop2DDataEntry::processor_process);
	ClassDB::bind_method(D_METHOD("processor_get_node_for", "prop_data"), &Prop2DDataEntry::processor_get_node_for);
	ClassDB::bind_method(D_METHOD("processor_evaluate_children"), &Prop2DDataEntry::processor_evaluate_children);

	ClassDB::bind_method(D_METHOD("_processor_handles", "node"), &Prop2DDataEntry::_processor_handles);
	ClassDB::bind_method(D_METHOD("_processor_process", "prop_data", "node", "transform", "entry"), &Prop2DDataEntry::_processor_process, Ref<Prop2DDataEntry>());
	ClassDB::bind_method(D_METHOD("_processor_get_node_for", "transform", "node"), &Prop2DDataEntry::_processor_get_node_for, Variant());
	ClassDB::bind_method(D_METHOD("_processor_evaluate_children"), &Prop2DDataEntry::_processor_evaluate_children);
}
