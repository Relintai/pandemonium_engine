/*************************************************************************/
/*  combine.cpp                                                          */
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

#include "combine.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMCombine::get_image() {
	return image;
}

void MMCombine::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMCombine::get_input_r() {
	return input_r;
}

void MMCombine::set_input_r(const Ref<MMNodeUniversalProperty> &val) {
	input_r = val;
}

Ref<MMNodeUniversalProperty> MMCombine::get_input_g() {
	return input_g;
}

void MMCombine::set_input_g(const Ref<MMNodeUniversalProperty> &val) {
	input_g = val;
}

Ref<MMNodeUniversalProperty> MMCombine::get_input_b() {
	return input_b;
}

void MMCombine::set_input_b(const Ref<MMNodeUniversalProperty> &val) {
	input_b = val;
}

Ref<MMNodeUniversalProperty> MMCombine::get_input_a() {
	return input_a;
}

void MMCombine::set_input_a(const Ref<MMNodeUniversalProperty> &val) {
	input_a = val;
}

void MMCombine::_init_properties() {
	if (!input_r.is_valid()) {
		input_r.instance();
		input_r->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		input_r->set_default_value(0);
	}

	input_r->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input_r->set_slot_name(">>>    R    ");

	if (!input_g.is_valid()) {
		input_g.instance();
		input_g->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		input_g->set_default_value(0);
	}

	input_g->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input_g->set_slot_name(">>>    G    ");

	if (!input_b.is_valid()) {
		input_b.instance();
		input_b->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		input_b->set_default_value(0);
	}

	input_b->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input_b->set_slot_name(">>>    B    ");

	if (!input_a.is_valid()) {
		input_a.instance();
		input_a->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		input_a->set_default_value(1);
	}

	input_a->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input_a->set_slot_name(">>>    A    ");

	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	//image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	//image.force_override = true;

	register_input_property(input_r);
	register_input_property(input_g);
	register_input_property(input_b);
	register_input_property(input_a);
	register_output_property(image);
}

void MMCombine::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input_r);
	mm_graph_node->add_slot_label_universal(input_g);
	mm_graph_node->add_slot_label_universal(input_b);
	mm_graph_node->add_slot_label_universal(input_a);
	mm_graph_node->add_slot_texture_universal(image);
}

void MMCombine::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMCombine::_get_value_for(const Vector2 &uv, const int pseed) {
	float r = input_r->get_value(uv);
	float g = input_g->get_value(uv);
	float b = input_b->get_value(uv);
	float a = input_a->get_value(uv);
	return Color(r, g, b, a);
}

MMCombine::MMCombine() {
}

MMCombine::~MMCombine() {
}

void MMCombine::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMCombine::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMCombine::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input_r"), &MMCombine::get_input_r);
	ClassDB::bind_method(D_METHOD("set_input_r", "value"), &MMCombine::set_input_r);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_r", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input_r", "get_input_r");

	ClassDB::bind_method(D_METHOD("get_input_g"), &MMCombine::get_input_g);
	ClassDB::bind_method(D_METHOD("set_input_g", "value"), &MMCombine::set_input_g);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_g", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input_g", "get_input_g");

	ClassDB::bind_method(D_METHOD("get_input_b"), &MMCombine::get_input_b);
	ClassDB::bind_method(D_METHOD("set_input_b", "value"), &MMCombine::set_input_b);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_b", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input_b", "get_input_b");

	ClassDB::bind_method(D_METHOD("get_input_a"), &MMCombine::get_input_a);
	ClassDB::bind_method(D_METHOD("set_input_a", "value"), &MMCombine::set_input_a);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input_a", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input_a", "get_input_a");
}
