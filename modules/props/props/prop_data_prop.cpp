/*************************************************************************/
/*  prop_data_prop.cpp                                                   */
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

#include "prop_data_prop.h"

#include "../prop_instance.h"
#include "../prop_instance_merger.h"
#include "prop_data.h"

#include "scene/resources/material/material.h"

#include "modules/modules_enabled.gen.h"

Ref<PropData> PropDataProp::get_prop() const {
	return _prop;
}
void PropDataProp::set_prop(const Ref<PropData> value) {
	_prop = value;
}

//Materials
Ref<Material> PropDataProp::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<Material>(NULL));

	return _materials[index];
}

void PropDataProp::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

int PropDataProp::material_get_num() const {
	return _materials.size();
}

void PropDataProp::materials_clear() {
	_materials.clear();
}

Vector<Variant> PropDataProp::materials_get() {
	Vector<Variant> r;
	for (int i = 0; i < _materials.size(); i++) {
		r.push_back(_materials[i].get_ref_ptr());
	}
	return r;
}

void PropDataProp::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

bool PropDataProp::get_is_merger() {
	return _is_merger;
}
void PropDataProp::set_is_merger(bool value) {
	_is_merger = value;
}

bool PropDataProp::get_snap_to_mesh() {
	return _snap_to_mesh;
}
void PropDataProp::set_snap_to_mesh(bool value) {
	_snap_to_mesh = value;
}

Vector3 PropDataProp::get_snap_axis() {
	return _snap_axis;
}
void PropDataProp::set_snap_axis(Vector3 value) {
	_snap_axis = value;
}

#ifdef MODULE_TEXTURE_PACKER_ENABLED
void PropDataProp::_add_textures_into(Ref<TexturePacker> texture_packer) {
	if (get_prop().is_valid()) {
		get_prop()->add_textures_into(texture_packer);
	}
}
#endif

bool PropDataProp::_processor_handles(Node *node) {
	PropInstance *i = Object::cast_to<PropInstance>(node);

	return i;
}

void PropDataProp::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	PropInstance *i = Object::cast_to<PropInstance>(node);

	if (i) {
		Ref<PropDataProp> l;
		l.instance();
		l->set_prop(i->get_prop_data());
		l->set_transform(transform * i->get_transform());

		PropInstanceMerger *im = Object::cast_to<PropInstanceMerger>(node);

		if (im) {
			l->materials_set(im->materials_get());
			l->set_is_merger(true);
		} else {
			Ref<Material> mat = i->get_material();

			if (mat.is_valid()) {
				l->material_add(mat);
			}
		}

		prop_data->add_prop(l);

		return;
	}
}

Node *PropDataProp::_processor_get_node_for(const Transform &transform) {
	if (_is_merger) {
		PropInstanceMerger *i = memnew(PropInstanceMerger);

		i->set_prop_data(get_prop());
		i->set_transform(get_transform());
		i->materials_set(materials_get());

		return i;
	} else {
		PropInstance *i = memnew(PropInstance);

		i->set_prop_data(get_prop());
		i->set_transform(get_transform());

		if (_materials.size() > 0) {
			i->set_material(_materials[0]);
		}

		return i;
	}
}

PropDataProp::PropDataProp() {
	_is_merger = false;
	_snap_to_mesh = false;
	_snap_axis = Vector3(0, 1, 0);
}

PropDataProp::~PropDataProp() {
	if (_prop.is_valid())
		_prop.unref();
}

void PropDataProp::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_prop"), &PropDataProp::get_prop);
	ClassDB::bind_method(D_METHOD("set_prop", "value"), &PropDataProp::set_prop);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "prop", PROPERTY_HINT_RESOURCE_TYPE, "PropData"), "set_prop", "get_prop");

	///Materials
	ClassDB::bind_method(D_METHOD("material_get", "index"), &PropDataProp::material_get);
	ClassDB::bind_method(D_METHOD("material_add", "value"), &PropDataProp::material_add);
	ClassDB::bind_method(D_METHOD("material_get_num"), &PropDataProp::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &PropDataProp::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &PropDataProp::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &PropDataProp::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "23/20:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

	ClassDB::bind_method(D_METHOD("get_is_merger"), &PropDataProp::get_is_merger);
	ClassDB::bind_method(D_METHOD("set_is_merger", "value"), &PropDataProp::set_is_merger);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_merger"), "set_is_merger", "get_is_merger");

	ClassDB::bind_method(D_METHOD("get_snap_to_mesh"), &PropDataProp::get_snap_to_mesh);
	ClassDB::bind_method(D_METHOD("set_snap_to_mesh", "value"), &PropDataProp::set_snap_to_mesh);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "snap_to_mesh"), "set_snap_to_mesh", "get_snap_to_mesh");

	ClassDB::bind_method(D_METHOD("get_snap_axis"), &PropDataProp::get_snap_axis);
	ClassDB::bind_method(D_METHOD("set_snap_axis", "value"), &PropDataProp::set_snap_axis);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "snap_axis"), "set_snap_axis", "get_snap_axis");

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	ClassDB::bind_method(D_METHOD("_add_textures_into", "texture_packer"), &PropDataProp::_add_textures_into);
#endif
}
