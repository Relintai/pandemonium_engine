/*************************************************************************/
/*  sdf3d_shape_torus.cpp                                                */
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

#include "sdf3d_shape_torus.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dShapeTorus::get_output() {
	return output;
}

void MMSdf3dShapeTorus::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

int MMSdf3dShapeTorus::get_axis() const {
	return axis;
}

void MMSdf3dShapeTorus::set_axis(const int val) {
	axis = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdf3dShapeTorus::get_major_radius() const {
	return major_radius;
}

void MMSdf3dShapeTorus::set_major_radius(const float val) {
	major_radius = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdf3dShapeTorus::get_minor_radius() const {
	return minor_radius;
}

void MMSdf3dShapeTorus::set_minor_radius(const float val) {
	minor_radius = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdf3dShapeTorus::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_VECTOR2);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);
	register_output_property(output);
}

void MMSdf3dShapeTorus::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);

	Array arr;
	arr.push_back("X");
	arr.push_back("Y");
	arr.push_back("Z");

	mm_graph_node->add_slot_enum("get_axis", "set_axis", "Axis", arr);
	mm_graph_node->add_slot_float("get_major_radius", "set_major_radius", "Major_radius", 0.01);
	mm_graph_node->add_slot_float("get_minor_radius", "set_minor_radius", "Minor_radius", 0.01);
}

Vector2 MMSdf3dShapeTorus::_get_property_value_sdf3d(const Vector3 &uv3) {
	if (axis == 0) {
		return MMAlgos::sdf3d_torus_x(uv3, major_radius, minor_radius);
	}

	else if (axis == 1) {
		return MMAlgos::sdf3d_torus_y(uv3, major_radius, minor_radius);
	}

	else if (axis == 2) {
		return MMAlgos::sdf3d_torus_z(uv3, major_radius, minor_radius);
	}

	return Vector2();
}

MMSdf3dShapeTorus::MMSdf3dShapeTorus() {
	axis = 2;
	major_radius = 0.3;
	minor_radius = 0.15;
}

MMSdf3dShapeTorus::~MMSdf3dShapeTorus() {
}

void MMSdf3dShapeTorus::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdf3dShapeTorus::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdf3dShapeTorus::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_axis"), &MMSdf3dShapeTorus::get_axis);
	ClassDB::bind_method(D_METHOD("set_axis", "value"), &MMSdf3dShapeTorus::set_axis);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "axis"), "set_axis", "get_axis");

	ClassDB::bind_method(D_METHOD("get_major_radius"), &MMSdf3dShapeTorus::get_major_radius);
	ClassDB::bind_method(D_METHOD("set_major_radius", "value"), &MMSdf3dShapeTorus::set_major_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "major_radius"), "set_major_radius", "get_major_radius");

	ClassDB::bind_method(D_METHOD("get_minor_radius"), &MMSdf3dShapeTorus::get_minor_radius);
	ClassDB::bind_method(D_METHOD("set_minor_radius", "value"), &MMSdf3dShapeTorus::set_minor_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "minor_radius"), "set_minor_radius", "get_minor_radius");
}
