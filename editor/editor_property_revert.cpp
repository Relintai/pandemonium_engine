/*************************************************************************/
/*  editor_property_revert.cpp                                           */
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

#include "editor_property_revert.h"

#include "scene/main/property_utils.h"

Variant EditorPropertyRevert::get_property_revert_value(Object *p_object, const StringName &p_property, bool *r_is_valid) {
	if (p_object->has_method("property_can_revert") && p_object->call("property_can_revert", p_property)) {
		if (r_is_valid) {
			*r_is_valid = true;
		}
		return p_object->call("property_get_revert", p_property);
	}

	return PropertyUtils::get_property_default_value(p_object, p_property, r_is_valid);
}

bool EditorPropertyRevert::can_property_revert(Object *p_object, const StringName &p_property) {
	bool is_valid_revert = false;
	Variant revert_value = EditorPropertyRevert::get_property_revert_value(p_object, p_property, &is_valid_revert);
	if (!is_valid_revert) {
		return false;
	}
	Variant current_value = p_object->get(p_property);
	return PropertyUtils::is_property_value_different(current_value, revert_value);
}
