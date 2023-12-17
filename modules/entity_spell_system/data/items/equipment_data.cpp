/*************************************************************************/
/*  equipment_data.cpp                                                   */
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

#include "equipment_data.h"

#include "item_instance.h"
#include "item_template.h"

#include "../../singletons/ess.h"

Ref<ItemTemplate> EquipmentData::get_slot(int index) {
	ERR_FAIL_INDEX_V(index, _entries.size(), Ref<ItemTemplate>());

	return _entries[index];
}
void EquipmentData::set_slot(int index, Ref<ItemTemplate> entry) {
	ERR_FAIL_INDEX(index, _entries.size());

	_entries.write[index] = entry;
}

Ref<ItemInstance> EquipmentData::get_item(int index) {
	ERR_FAIL_INDEX_V(index, _entries.size(), Ref<ItemInstance>());

	Ref<ItemTemplate> ede = _entries[index];

	if (!ede.is_valid())
		return Ref<ItemInstance>();

	return ede->create_item_instance();
}

EquipmentData::EquipmentData() {
	if (ESS::get_singleton()) {
		_entries.resize(ESS::get_singleton()->equip_slot_get_count());
	}
}

EquipmentData::~EquipmentData() {
	_entries.clear();
}

bool EquipmentData::_set(const StringName &p_name, const Variant &p_value) {
	ERR_FAIL_COND_V(!ESS::get_singleton(), false);

	String name = p_name;

	if (name.get_slicec('/', 0) == "slot") {
		StringName prop = name.get_slicec('/', 1);

		if (ESS::get_singleton()->equip_slot_is_property(prop)) {
			int id = ESS::get_singleton()->equip_slot_get_property_id(prop);

			if (_entries.size() < id) {
				return false;
			}

			_entries.set(id, p_value);

			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}

	return true;
}

bool EquipmentData::_get(const StringName &p_name, Variant &r_ret) const {
	ERR_FAIL_COND_V(!ESS::get_singleton(), false);

	String name = p_name;

	if (name.get_slicec('/', 0) == "slot") {
		StringName prop = name.get_slicec('/', 1);

		if (ESS::get_singleton()->equip_slot_is_property(prop)) {
			int id = ESS::get_singleton()->equip_slot_get_property_id(prop);

			if (_entries.size() < id) {
				return false;
			}

			r_ret = _entries[id];

			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}

	return true;
}

void EquipmentData::_get_property_list(List<PropertyInfo> *p_list) const {
	ERR_FAIL_COND(!ESS::get_singleton());

	for (int i = 0; i < ESS::get_singleton()->equip_slot_get_count(); ++i) {
		p_list->push_back(PropertyInfo(Variant::OBJECT, "slot/" + ESS::get_singleton()->equip_slot_get_property_name(i), PROPERTY_HINT_RESOURCE_TYPE, "ItemTemplate"));
	}
}

void EquipmentData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_slot", "index"), &EquipmentData::get_slot);
	ClassDB::bind_method(D_METHOD("set_slot", "index", "entry"), &EquipmentData::set_slot);
}
