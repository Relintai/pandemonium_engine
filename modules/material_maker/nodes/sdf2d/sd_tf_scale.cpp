/*************************************************************************/
/*  sd_tf_scale.cpp                                                      */
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

#include "sd_tf_scale.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdTfScale::get_output() {
	return output;
}

void MMSdTfScale::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

float MMSdTfScale::get_scale() const {
	return scale;
}

void MMSdTfScale::set_scale(const float val) {
	scale = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdTfScale::_init_properties() {
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

void MMSdTfScale::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_float("get_scale", "set_scale", "Scale", 0.01);
}

Variant MMSdTfScale::_get_property_value(const Vector2 &uv) {
	//$in(($uv-vec2(0.5))/$s+vec2(0.5))*$s;
	return output->get_value(((uv - Vector2(0.5, 0.5)) / scale + Vector2(0.5, 0.5)), true);
}

MMSdTfScale::MMSdTfScale() {
	scale = 1;
}

MMSdTfScale::~MMSdTfScale() {
}

void MMSdTfScale::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdTfScale::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdTfScale::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_scale"), &MMSdTfScale::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "value"), &MMSdTfScale::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "scale"), "set_scale", "get_scale");
}
