/*************************************************************************/
/*  sd_op_repeat.cpp                                                     */
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

#include "sd_op_repeat.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdOpRepeat::get_output() {
	return output;
}

void MMSdOpRepeat::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

int MMSdOpRepeat::get_x() const {
	return x;
}

void MMSdOpRepeat::set_x(const int val) {
	x = val;
	emit_changed();
	output->do_emit_changed();
}

int MMSdOpRepeat::get_y() const {
	return y;
}

void MMSdOpRepeat::set_y(const int val) {
	y = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdOpRepeat::get_random_rotation() const {
	return random_rotation;
}

void MMSdOpRepeat::set_random_rotation(const float val) {
	random_rotation = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdOpRepeat::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	output->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	//output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	output->set_slot_name(">>>    Apply    >>>");
	output->set_get_value_from_owner(true);

	register_input_property(output);
	register_output_property(output);
}

void MMSdOpRepeat::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_int("get_x", "set_x", "X");
	mm_graph_node->add_slot_int("get_y", "set_y", "Y");
	mm_graph_node->add_slot_float("get_random_rotation", "set_random_rotation", "Random rotation", 0.01);
}

Variant MMSdOpRepeat::_get_property_value(const Vector2 &uv) {
	//todo add this as a parameter;
	int pseed = 123123;
	//$in(repeat_2d($uv, vec2(1.0/$rx, 1.0/$ry), float($seed), $r));
	Vector2 new_uv = MMAlgos::repeat_2d(uv, Vector2(1.0 / float(x), 1.0 / float(y)), 1.0 / float(pseed), random_rotation);
	return output->get_value(new_uv, true);
}

MMSdOpRepeat::MMSdOpRepeat() {
	x = 3;
	y = 3;
	random_rotation = 0.5;
}

MMSdOpRepeat::~MMSdOpRepeat() {
}

void MMSdOpRepeat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdOpRepeat::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdOpRepeat::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_x"), &MMSdOpRepeat::get_x);
	ClassDB::bind_method(D_METHOD("set_x", "value"), &MMSdOpRepeat::set_x);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "x"), "set_x", "get_x");

	ClassDB::bind_method(D_METHOD("get_y"), &MMSdOpRepeat::get_y);
	ClassDB::bind_method(D_METHOD("set_y", "value"), &MMSdOpRepeat::set_y);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "y"), "set_y", "get_y");

	ClassDB::bind_method(D_METHOD("get_random_rotation"), &MMSdOpRepeat::get_random_rotation);
	ClassDB::bind_method(D_METHOD("set_random_rotation", "value"), &MMSdOpRepeat::set_random_rotation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "random_rotation"), "set_random_rotation", "get_random_rotation");
}
