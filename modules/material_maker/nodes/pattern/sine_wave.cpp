/*************************************************************************/
/*  sine_wave.cpp                                                        */
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

#include "sine_wave.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSineWave::get_image() {
	return image;
}

void MMSineWave::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

float MMSineWave::get_amplitude() const {
	return amplitude;
}

void MMSineWave::set_amplitude(const float val) {
	amplitude = val;
	set_dirty(true);
}

float MMSineWave::get_frequency() const {
	return frequency;
}

void MMSineWave::set_frequency(const float val) {
	frequency = val;
	set_dirty(true);
}

float MMSineWave::get_phase() const {
	return phase;
}

void MMSineWave::set_phase(const float val) {
	phase = val;
	set_dirty(true);
}

void MMSineWave::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	register_output_property(image);
}

void MMSineWave::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_float("get_amplitude", "set_amplitude", "Amplitude", 0.01);
	mm_graph_node->add_slot_float("get_frequency", "set_frequency", "Frequency", 0.1);
	mm_graph_node->add_slot_float("get_phase", "set_phase", "Phase", 0.01);
}

void MMSineWave::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMSineWave::_get_value_for(const Vector2 &uv, const int pseed) {
	float f = 1.0 - abs(2.0 * (uv.y - 0.5) - amplitude * sin((frequency * uv.x + phase) * 6.28318530718));
	return Color(f, f, f, 1);
}

MMSineWave::MMSineWave() {
	amplitude = 0.5;
	frequency = 2;
	phase = 0;
}

MMSineWave::~MMSineWave() {
}

void MMSineWave::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMSineWave::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMSineWave::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_amplitude"), &MMSineWave::get_amplitude);
	ClassDB::bind_method(D_METHOD("set_amplitude", "value"), &MMSineWave::set_amplitude);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "amplitude"), "set_amplitude", "get_amplitude");

	ClassDB::bind_method(D_METHOD("get_frequency"), &MMSineWave::get_frequency);
	ClassDB::bind_method(D_METHOD("set_frequency", "value"), &MMSineWave::set_frequency);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "frequency"), "set_frequency", "get_frequency");

	ClassDB::bind_method(D_METHOD("get_phase"), &MMSineWave::get_phase);
	ClassDB::bind_method(D_METHOD("set_phase", "value"), &MMSineWave::set_phase);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "phase"), "set_phase", "get_phase");
}
