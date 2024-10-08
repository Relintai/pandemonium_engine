/*************************************************************************/
/*  sdf3d_shape_cone.cpp                                                 */
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

#include "sdf3d_shape_cone.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dShapeCone::get_output() {
	return output;
}

void MMSdf3dShapeCone::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

int MMSdf3dShapeCone::get_axis() const {
	return axis;
}

void MMSdf3dShapeCone::set_axis(const int val) {
	axis = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdf3dShapeCone::get_angle() const {
	return angle;
}

void MMSdf3dShapeCone::set_angle(const float val) {
	angle = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdf3dShapeCone::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_VECTOR2);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);
	register_output_property(output);
}

void MMSdf3dShapeCone::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);

	Array arr;
	arr.push_back("+X");
	arr.push_back("-X");
	arr.push_back("+Y");
	arr.push_back("-Y");
	arr.push_back("+Z");
	arr.push_back("-Z");

	mm_graph_node->add_slot_enum("get_axis", "set_axis", "Axis", arr);
	mm_graph_node->add_slot_float("get_angle", "set_angle", "Angle", 1);
}

Vector2 MMSdf3dShapeCone::_get_property_value_sdf3d(const Vector3 &uv3) {
	if (axis == 0) {
		return MMAlgos::sdf3d_cone_px(uv3, angle);
	} else if (axis == 1) {
		return MMAlgos::sdf3d_cone_nx(uv3, angle);
	} else if (axis == 2) {
		return MMAlgos::sdf3d_cone_py(uv3, angle);
	} else if (axis == 3) {
		return MMAlgos::sdf3d_cone_ny(uv3, angle);
	} else if (axis == 4) {
		return MMAlgos::sdf3d_cone_pz(uv3, angle);
	} else if (axis == 5) {
		return MMAlgos::sdf3d_cone_nz(uv3, angle);
	}

	return Vector2();
}

MMSdf3dShapeCone::MMSdf3dShapeCone() {
	axis = 2;
	angle = 30;
}

MMSdf3dShapeCone::~MMSdf3dShapeCone() {
}

void MMSdf3dShapeCone::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdf3dShapeCone::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdf3dShapeCone::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_axis"), &MMSdf3dShapeCone::get_axis);
	ClassDB::bind_method(D_METHOD("set_axis", "value"), &MMSdf3dShapeCone::set_axis);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "axis"), "set_axis", "get_axis");

	ClassDB::bind_method(D_METHOD("get_angle"), &MMSdf3dShapeCone::get_angle);
	ClassDB::bind_method(D_METHOD("set_angle", "value"), &MMSdf3dShapeCone::set_angle);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "angle"), "set_angle", "get_angle");
}
