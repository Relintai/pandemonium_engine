/*************************************************************************/
/*  greyscale_uniform.cpp                                                */
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

#include "greyscale_uniform.h"

#include "../../editor/mm_graph_node.h"

Ref<MMNodeUniversalProperty> MMGreyscaleUniform::get_uniform() {
	return uniform;
}

void MMGreyscaleUniform::set_uniform(const Ref<MMNodeUniversalProperty> &val) {
	uniform = val;
}

void MMGreyscaleUniform::_init_properties() {
	if (!uniform.is_valid()) {
		uniform.instance();
		uniform->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		uniform->set_default_value(0.5);
		uniform->set_slot_name("Value (Color)");
		uniform->set_value_step(0.01);
		uniform->set_value_range(Vector2(0, 1));
	}

	uniform->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_COLOR);
	register_output_property(uniform);
}

void MMGreyscaleUniform::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_float_universal(uniform);
}

Color MMGreyscaleUniform::_get_value_for(const Vector2 &uv, const int pseed) {
	float f = uniform->get_value(uv);
	return Color(f, f, f, 1);
}

MMGreyscaleUniform::MMGreyscaleUniform() {
}

MMGreyscaleUniform::~MMGreyscaleUniform() {
}

void MMGreyscaleUniform::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_uniform"), &MMGreyscaleUniform::get_uniform);
	ClassDB::bind_method(D_METHOD("set_uniform", "value"), &MMGreyscaleUniform::set_uniform);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "uniform", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_uniform", "get_uniform");
}
