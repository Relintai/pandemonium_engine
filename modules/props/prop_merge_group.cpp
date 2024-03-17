/*************************************************************************/
/*  prop_merge_group.cpp                                                 */
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

#include "prop_merge_group.h"

#include "scene/resources/material/material.h"

//Materials
Ref<Material> PropMergeGroup::material_get(const int index) {
	ERR_FAIL_INDEX_V(index, _materials.size(), Ref<Material>(NULL));

	return _materials[index];
}

void PropMergeGroup::material_add(const Ref<Material> &value) {
	ERR_FAIL_COND(!value.is_valid());

	_materials.push_back(value);
}

int PropMergeGroup::material_get_num() const {
	return _materials.size();
}

void PropMergeGroup::materials_clear() {
	_materials.clear();
}

Vector<Variant> PropMergeGroup::materials_get() {
	Vector<Variant> r;
	for (int i = 0; i < _materials.size(); i++) {
		r.push_back(_materials[i].get_ref_ptr());
	}
	return r;
}

void PropMergeGroup::materials_set(const Vector<Variant> &materials) {
	_materials.clear();

	for (int i = 0; i < materials.size(); i++) {
		Ref<Material> material = Ref<Material>(materials[i]);

		_materials.push_back(material);
	}
}

bool PropMergeGroup::get_is_merger() {
	return _is_merger;
}
void PropMergeGroup::set_is_merger(bool value) {
	_is_merger = value;
}

PropMergeGroup::PropMergeGroup() {
	_is_merger = false;
}
PropMergeGroup::~PropMergeGroup() {
}

void PropMergeGroup::_bind_methods() {
	///Materials
	ClassDB::bind_method(D_METHOD("material_get", "index"), &PropMergeGroup::material_get);
	ClassDB::bind_method(D_METHOD("material_add", "value"), &PropMergeGroup::material_add);
	ClassDB::bind_method(D_METHOD("material_get_num"), &PropMergeGroup::material_get_num);
	ClassDB::bind_method(D_METHOD("materials_clear"), &PropMergeGroup::materials_clear);

	ClassDB::bind_method(D_METHOD("materials_get"), &PropMergeGroup::materials_get);
	ClassDB::bind_method(D_METHOD("materials_set"), &PropMergeGroup::materials_set);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "materials", PROPERTY_HINT_NONE, "23/20:Material", PROPERTY_USAGE_DEFAULT, "Material"), "materials_set", "materials_get");

	ClassDB::bind_method(D_METHOD("get_is_merger"), &PropMergeGroup::get_is_merger);
	ClassDB::bind_method(D_METHOD("set_is_merger", "value"), &PropMergeGroup::set_is_merger);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_merger"), "set_is_merger", "get_is_merger");
}
