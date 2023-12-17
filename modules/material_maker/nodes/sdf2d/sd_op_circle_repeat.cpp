/*************************************************************************/
/*  sd_op_circle_repeat.cpp                                              */
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

#include "sd_op_circle_repeat.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdOpCircleRepeat::get_output() {
	return output;
}

void MMSdOpCircleRepeat::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

int MMSdOpCircleRepeat::get_count() const {
	return count;
}

void MMSdOpCircleRepeat::set_count(const int val) {
	count = val;

	emit_changed();
	output->do_emit_changed();
}

void MMSdOpCircleRepeat::_init_properties() {
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

void MMSdOpCircleRepeat::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_int("get_count", "set_count", "Count");
}

Variant MMSdOpCircleRepeat::_get_property_value(const Vector2 &uv) {
	//$in(circle_repeat_transform_2d($uv-vec2(0.5), $c)+vec2(0.5));
	Vector2 new_uv = MMAlgos::circle_repeat_transform_2d(uv - Vector2(0.5, 0.5), count) + Vector2(0.5, 0.5);
	return output->get_value(new_uv, true);
}

MMSdOpCircleRepeat::MMSdOpCircleRepeat() {
	count = 6;
}

MMSdOpCircleRepeat::~MMSdOpCircleRepeat() {
}

void MMSdOpCircleRepeat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdOpCircleRepeat::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdOpCircleRepeat::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_count"), &MMSdOpCircleRepeat::get_count);
	ClassDB::bind_method(D_METHOD("set_count", "value"), &MMSdOpCircleRepeat::set_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "count"), "set_count", "get_count");
}
