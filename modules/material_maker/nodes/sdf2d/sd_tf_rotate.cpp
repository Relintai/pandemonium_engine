/*************************************************************************/
/*  sd_tf_rotate.cpp                                                     */
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

#include "sd_tf_rotate.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdTfRotate::get_output() {
	return output;
}

void MMSdTfRotate::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

float MMSdTfRotate::get_angle() const {
	return angle;
}

void MMSdTfRotate::set_angle(const float val) {
	angle = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdTfRotate::_init_properties() {
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

void MMSdTfRotate::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_float("get_angle", "set_angle", "Angle", 1);
}

Variant MMSdTfRotate::_get_property_value(const Vector2 &uv) {
	//$in(sdf2d_rotate($uv, $a*0.01745329251))",;
	return output->get_value(MMAlgos::sdf2d_rotate(uv, angle * 0.01745329251), true);
}

MMSdTfRotate::MMSdTfRotate() {
	angle = 0;
}

MMSdTfRotate::~MMSdTfRotate() {
}

void MMSdTfRotate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdTfRotate::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdTfRotate::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_angle"), &MMSdTfRotate::get_angle);
	ClassDB::bind_method(D_METHOD("set_angle", "value"), &MMSdTfRotate::set_angle);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "angle"), "set_angle", "get_angle");
}
