/*************************************************************************/
/*  sd_shape_box.cpp                                                     */
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

#include "sd_shape_box.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdShapeBox::get_output() {
	return output;
}

void MMSdShapeBox::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

Vector2 MMSdShapeBox::get_center() {
	return center;
}

void MMSdShapeBox::set_center(const Vector2 &val) {
	center = val;
	emit_changed();
	output->do_emit_changed();
}

Vector2 MMSdShapeBox::get_size() {
	return size;
}

void MMSdShapeBox::set_size(const Vector2 &val) {
	size = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdShapeBox::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);

	register_output_property(output);
}

void MMSdShapeBox::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_vector2("get_center", "set_center", "Center", 0.01);
	mm_graph_node->add_slot_vector2("get_size", "set_size", "Size", 0.01);
}

Variant MMSdShapeBox::_get_property_value(const Vector2 &uv) {
	return MMAlgos::sdf_box(uv, center, size);
}

MMSdShapeBox::MMSdShapeBox() {
	center = Vector2(0, 0);
	size = Vector2(0.3, 0.2);
}

MMSdShapeBox::~MMSdShapeBox() {
}

void MMSdShapeBox::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdShapeBox::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdShapeBox::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_center"), &MMSdShapeBox::get_center);
	ClassDB::bind_method(D_METHOD("set_center", "value"), &MMSdShapeBox::set_center);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "center"), "set_center", "get_center");

	ClassDB::bind_method(D_METHOD("get_size"), &MMSdShapeBox::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &MMSdShapeBox::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");
}
