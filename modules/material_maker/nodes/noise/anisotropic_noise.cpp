/*************************************************************************/
/*  anisotropic_noise.cpp                                                */
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

#include "anisotropic_noise.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMAnisotropicNoise::get_image() {
	return image;
}

void MMAnisotropicNoise::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Vector2 MMAnisotropicNoise::get_scale() {
	return scale;
}

void MMAnisotropicNoise::set_scale(const Vector2 &val) {
	scale = val;
	set_dirty(true);
}

float MMAnisotropicNoise::get_smoothness() const {
	return smoothness;
}

void MMAnisotropicNoise::set_smoothness(const float val) {
	smoothness = val;
	set_dirty(true);
}

float MMAnisotropicNoise::get_interpolation() const {
	return interpolation;
}

void MMAnisotropicNoise::set_interpolation(const float val) {
	interpolation = val;
	set_dirty(true);
}

void MMAnisotropicNoise::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	register_output_property(image);
}

void MMAnisotropicNoise::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	//, Vector2(1, 10));
	mm_graph_node->add_slot_vector2("get_scale", "set_scale", "Scale", 1);
	//, Vector2(0, 1));
	mm_graph_node->add_slot_float("get_smoothness", "set_smoothness", "Smoothness", 0.01);
	//, Vector2(0, 1));
	mm_graph_node->add_slot_float("get_interpolation", "set_interpolation", "Interpolation", 0.01);
}

Color MMAnisotropicNoise::_get_value_for(const Vector2 &uv, const int pseed) {
	float ps = 1.0 / float(pseed);
	//anisotropic($(uv), vec2($(scale_x), $(scale_y)), $(seed), $(smoothness), $(interpolation));
	return MMAlgos::anisotropicc(uv, scale, ps, smoothness, interpolation);
}

void MMAnisotropicNoise::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

MMAnisotropicNoise::MMAnisotropicNoise() {
	scale = Vector2(4, 256);
	smoothness = 1;
	interpolation = 1;
}

MMAnisotropicNoise::~MMAnisotropicNoise() {
}

void MMAnisotropicNoise::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMAnisotropicNoise::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMAnisotropicNoise::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_scale"), &MMAnisotropicNoise::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "value"), &MMAnisotropicNoise::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");

	ClassDB::bind_method(D_METHOD("get_smoothness"), &MMAnisotropicNoise::get_smoothness);
	ClassDB::bind_method(D_METHOD("set_smoothness", "value"), &MMAnisotropicNoise::set_smoothness);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "smoothness"), "set_smoothness", "get_smoothness");

	ClassDB::bind_method(D_METHOD("get_interpolation"), &MMAnisotropicNoise::get_interpolation);
	ClassDB::bind_method(D_METHOD("set_interpolation", "value"), &MMAnisotropicNoise::set_interpolation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "interpolation"), "set_interpolation", "get_interpolation");
}
