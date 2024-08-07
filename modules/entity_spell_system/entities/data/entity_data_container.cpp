/*************************************************************************/
/*  entity_data_container.cpp                                            */
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

#include "entity_data_container.h"

Dictionary EntityDataContainer::to_dict() {
	return call("_to_dict");
}
void EntityDataContainer::from_dict(const Dictionary &dict) {
	call("_from_dict", dict);
}

Dictionary EntityDataContainer::_to_dict() {
	Dictionary dict;

	dict["class_name"] = get_class_static();

	return dict;
}
void EntityDataContainer::_from_dict(const Dictionary &dict) {
	ERR_FAIL_COND(dict.empty());
}

EntityDataContainer::EntityDataContainer() {
}
EntityDataContainer::~EntityDataContainer() {
}

void EntityDataContainer::_bind_methods() {
	//Serialization
	BIND_VMETHOD(MethodInfo("_from_dict", PropertyInfo(Variant::DICTIONARY, "dict")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::DICTIONARY, "dict"), "_to_dict"));

	ClassDB::bind_method(D_METHOD("from_dict", "dict"), &EntityDataContainer::from_dict);
	ClassDB::bind_method(D_METHOD("to_dict"), &EntityDataContainer::to_dict);

	ClassDB::bind_method(D_METHOD("_from_dict", "dict"), &EntityDataContainer::_from_dict);
	ClassDB::bind_method(D_METHOD("_to_dict"), &EntityDataContainer::_to_dict);
}
