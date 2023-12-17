/*************************************************************************/
/*  output_image.cpp                                                     */
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

#include "output_image.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMOutputImage::get_input() {
	return input;
}

void MMOutputImage::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMOutputImage::get_image() {
	return image;
}

void MMOutputImage::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

String MMOutputImage::get_postfix() {
	return postfix;
}

void MMOutputImage::set_postfix(const String &val) {
	postfix = val;
	set_dirty(true);
}

void MMOutputImage::_init_properties() {
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

	image->set_slot_name("image");

	register_input_property(input);
}

void MMOutputImage::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_line_edit("get_postfix", "set_postfix", "postfix");
	mm_graph_node->add_slot_button("save", "Save");
}

void MMOutputImage::_render(const Ref<MMMaterial> &material) {
	_material_path = material->get_path();

	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMOutputImage::_get_value_for(const Vector2 &uv, const int pseed) {
	Color c = input->get_value(uv);
	return c;
}

void MMOutputImage::save() {
	if (_material_path == "") {
		return;
	}

	if (!image.is_valid()) {
		return;
	}

	Ref<Image> img = image->get_active_image();

	if (!img.is_valid()) {
		return;
	}

	String matbn = _material_path.get_basename();
	String final_file_name = matbn + postfix + ".png";
	img->save_png(final_file_name);
}

MMOutputImage::MMOutputImage() {
}

MMOutputImage::~MMOutputImage() {
}

void MMOutputImage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &MMOutputImage::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMOutputImage::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_image"), &MMOutputImage::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMOutputImage::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_postfix"), &MMOutputImage::get_postfix);
	ClassDB::bind_method(D_METHOD("set_postfix", "value"), &MMOutputImage::set_postfix);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "postfix"), "set_postfix", "get_postfix");

	ClassDB::bind_method(D_METHOD("save"), &MMOutputImage::save);
}
