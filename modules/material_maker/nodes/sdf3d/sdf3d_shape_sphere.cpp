/*************************************************************************/
/*  sdf3d_shape_sphere.cpp                                               */
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

#include "sdf3d_shape_sphere.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dShapeSphere::get_output() {
	return output;
}

void MMSdf3dShapeSphere::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

float MMSdf3dShapeSphere::get_radius() const {
	return radius;
}

void MMSdf3dShapeSphere::set_radius(const float val) {
	radius = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdf3dShapeSphere::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_VECTOR2);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);

	register_output_property(output);
}

void MMSdf3dShapeSphere::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_float("get_radius", "set_radius", "Radius", 0.01);
}

Vector2 MMSdf3dShapeSphere::_get_property_value_sdf3d(const Vector3 &uv3) {
	return MMAlgos::sdf3d_sphere(uv3, radius);
}

MMSdf3dShapeSphere::MMSdf3dShapeSphere() {
	radius = 0.5;
}

MMSdf3dShapeSphere::~MMSdf3dShapeSphere() {
}

void MMSdf3dShapeSphere::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdf3dShapeSphere::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdf3dShapeSphere::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_radius"), &MMSdf3dShapeSphere::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius", "value"), &MMSdf3dShapeSphere::set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");
}
