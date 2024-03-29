/*************************************************************************/
/*  fill_to_size.cpp                                                     */
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

#include "fill_to_size.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMFillToSize::get_image() {
	return image;
}

void MMFillToSize::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMFillToSize::get_input() {
	return input;
}

void MMFillToSize::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

int MMFillToSize::get_formula() const {
	return formula;
}

void MMFillToSize::set_formula(const int val) {
	formula = val;
	set_dirty(true);
}

void MMFillToSize::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color(0, 0, 0, 1));
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input    ");

	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	//image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	//image.force_override = true;
	register_input_property(input);
	register_output_property(image);
}

void MMFillToSize::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("Area");
	arr.push_back("Width");
	arr.push_back("Height");
	arr.push_back("Max(W,H)");

	mm_graph_node->add_slot_enum("get_formula", "set_formula", "Formula", arr);
}

void MMFillToSize::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMFillToSize::_get_value_for(const Vector2 &uv, const int pseed) {
	//vec4 $(name_uv)_bb = $in($uv);
	Color c = input->get_value(uv);
	float f = 0;
	//"Area" sqrt($(name_uv)_bb.z*$(name_uv)_bb.w);
	//"Width" $(name_uv)_bb.z;
	//"Height" $(name_uv)_bb.w;
	//"max(W, H)" max($(name_uv)_bb.z, $(name_uv)_bb.w);

	if (formula == 0) {
		f = sqrt(c.b * c.a);
	} else if (formula == 1) {
		f = c.b;
	} else if (formula == 2) {
		f = c.a;
	} else if (formula == 3) {
		f = MAX(c.b, c.a);
	}

	return Color(f, f, f, 1);
}

MMFillToSize::MMFillToSize() {
	formula = 0;
}

MMFillToSize::~MMFillToSize() {
}

void MMFillToSize::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMFillToSize::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMFillToSize::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMFillToSize::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMFillToSize::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_formula"), &MMFillToSize::get_formula);
	ClassDB::bind_method(D_METHOD("set_formula", "value"), &MMFillToSize::set_formula);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "formula"), "set_formula", "get_formula");
}
