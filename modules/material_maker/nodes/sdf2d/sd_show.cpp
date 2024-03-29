/*************************************************************************/
/*  sd_show.cpp                                                          */
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

#include "sd_show.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> SSSdShow::get_image() {
	return image;
}

void SSSdShow::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> SSSdShow::get_input() {
	return input;
}

void SSSdShow::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

float SSSdShow::get_bevel() const {
	return bevel;
}

void SSSdShow::set_bevel(const float val) {
	bevel = val;
	set_dirty(true);
}

float SSSdShow::get_base() const {
	return base;
}

void SSSdShow::set_base(const float val) {
	base = val;
	set_dirty(true);
}

void SSSdShow::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	//for some reason this doesn't work, todo check;
	//	input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name("Input");

	register_output_property(image);
	register_input_property(input);
}

void SSSdShow::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_float("get_bevel", "set_bevel", "Bevel", 0.01);
	mm_graph_node->add_slot_float("get_base", "set_base", "Base", 0.01);
}

void SSSdShow::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color SSSdShow::_get_value_for(const Vector2 &uv, const int pseed) {
	float f = input->get_value(uv);
	//clamp($base-$in($uv)/max($bevel, 0.00001), 0.0, 1.0);
	float cf = CLAMP(base - f / MAX(bevel, 0.00001), 0.0, 1.0);
	return Color(cf, cf, cf, 1);
}

SSSdShow::SSSdShow() {
	bevel = 0;
	base = 0;
}

SSSdShow::~SSSdShow() {
}

void SSSdShow::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &SSSdShow::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &SSSdShow::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &SSSdShow::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &SSSdShow::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_bevel"), &SSSdShow::get_bevel);
	ClassDB::bind_method(D_METHOD("set_bevel", "value"), &SSSdShow::set_bevel);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "bevel"), "set_bevel", "get_bevel");

	ClassDB::bind_method(D_METHOD("get_base"), &SSSdShow::get_base);
	ClassDB::bind_method(D_METHOD("set_base", "value"), &SSSdShow::set_base);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "base"), "set_base", "get_base");
}
