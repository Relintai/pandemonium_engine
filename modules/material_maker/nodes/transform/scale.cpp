/*************************************************************************/
/*  scale.cpp                                                            */
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

#include "scale.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMScale::get_image() {
	return image;
}

void MMScale::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMScale::get_input() {
	return input;
}

void MMScale::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Vector2 MMScale::get_center() {
	return center;
}

void MMScale::set_center(const Vector2 &val) {
	center = val;
	set_dirty(true);
}

Vector2 MMScale::get_scale() {
	return scale;
}

void MMScale::set_scale(const Vector2 &val) {
	scale = val;
	set_dirty(true);
}

void MMScale::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color(0, 0, 0, 1));
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input1    ");

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

void MMScale::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_vector2("get_center", "set_center", "Center", 0.01);
	mm_graph_node->add_slot_vector2("get_scale", "set_scale", "MMScale", 0.01);
}

void MMScale::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMScale::_get_value_for(const Vector2 &uv, const int pseed) {
	//$i(scale($uv, vec2(0.5+$cx, 0.5+$cy), vec2($scale_x, $scale_y)));
	return input->get_value(MMAlgos::scale(uv, center + Vector2(0.5, 0.5), scale));
}

MMScale::MMScale() {
	scale = Vector2(1, 1);
}

MMScale::~MMScale() {
}

void MMScale::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMScale::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMScale::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMScale::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMScale::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_center"), &MMScale::get_center);
	ClassDB::bind_method(D_METHOD("set_center", "value"), &MMScale::set_center);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "center"), "set_center", "get_center");

	ClassDB::bind_method(D_METHOD("get_scale"), &MMScale::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "value"), &MMScale::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");
}
