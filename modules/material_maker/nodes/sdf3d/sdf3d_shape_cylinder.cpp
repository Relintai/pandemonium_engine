/*************************************************************************/
/*  sdf3d_shape_cylinder.cpp                                             */
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

#include "sdf3d_shape_cylinder.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dShapeCylinder::get_output() {
	return output;
}

void MMSdf3dShapeCylinder::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

int MMSdf3dShapeCylinder::get_axis() const {
	return axis;
}

void MMSdf3dShapeCylinder::set_axis(const int val) {
	axis = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdf3dShapeCylinder::get_length() const {
	return length;
}

void MMSdf3dShapeCylinder::set_length(const float val) {
	length = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdf3dShapeCylinder::get_radius() const {
	return radius;
}

void MMSdf3dShapeCylinder::set_radius(const float val) {
	radius = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdf3dShapeCylinder::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_VECTOR2);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);
	register_output_property(output);
}

void MMSdf3dShapeCylinder::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);

	Array arr;
	arr.push_back("X");
	arr.push_back("Y");
	arr.push_back("Z");

	mm_graph_node->add_slot_enum("get_axis", "set_axis", "Axis", arr);
	mm_graph_node->add_slot_float("get_length", "set_length", "Length", 0.01);
	mm_graph_node->add_slot_float("get_radius", "set_radius", "Radius", 0.01);
}

Vector2 MMSdf3dShapeCylinder::_get_property_value_sdf3d(const Vector3 &uv3) {
	if (axis == 0) {
		return MMAlgos::sdf3d_cylinder_x(uv3, radius, length);
	}

	else if (axis == 1) {
		return MMAlgos::sdf3d_cylinder_y(uv3, radius, length);
	}

	else if (axis == 2) {
		return MMAlgos::sdf3d_cylinder_z(uv3, radius, length);
	}

	return Vector2();
}

MMSdf3dShapeCylinder::MMSdf3dShapeCylinder() {
	axis = 1;
	length = 0.25;
	radius = 0.25;
}

MMSdf3dShapeCylinder::~MMSdf3dShapeCylinder() {
}

void MMSdf3dShapeCylinder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdf3dShapeCylinder::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdf3dShapeCylinder::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_axis"), &MMSdf3dShapeCylinder::get_axis);
	ClassDB::bind_method(D_METHOD("set_axis", "value"), &MMSdf3dShapeCylinder::set_axis);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "axis"), "set_axis", "get_axis");

	ClassDB::bind_method(D_METHOD("get_length"), &MMSdf3dShapeCylinder::get_length);
	ClassDB::bind_method(D_METHOD("set_length", "value"), &MMSdf3dShapeCylinder::set_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "length"), "set_length", "get_length");

	ClassDB::bind_method(D_METHOD("get_radius"), &MMSdf3dShapeCylinder::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius", "value"), &MMSdf3dShapeCylinder::set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");
}
