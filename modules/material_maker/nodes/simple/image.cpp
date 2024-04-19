/*************************************************************************/
/*  image.cpp                                                            */
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

#include "image.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"
#include "core/io/image_loader.h"
#include "core/io/resource_loader.h"

Ref<MMNodeUniversalProperty> MMImage::get_image() {
	return image;
}

void MMImage::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

String MMImage::get_image_path() {
	return image_path;
}

void MMImage::set_image_path(const String &val) {
	image_path = val;
	Ref<Image> img;
	
	if (image_path != "") {
		img.instance();
		ImageLoader::load_image(image_path, img);
	}

	image->set_default_value(img);
	set_dirty(true);
}

void MMImage::_init_properties() {
	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);

	register_output_property(image);
}

void MMImage::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_image_path_universal(image, "get_image_path", "set_image_path");
}

//func _render(material) -> void:;
//	var img : MMImage = render_image(material);
//;
//	image.set_value(img);

Color MMImage::_get_value_for(const Vector2 &uv, const int pseed) {
	return image->get_value(uv);
}

MMImage::MMImage() {
}

MMImage::~MMImage() {
}

void MMImage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMImage::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMImage::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_image_path"), &MMImage::get_image_path);
	ClassDB::bind_method(D_METHOD("set_image_path", "value"), &MMImage::set_image_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "image_path"), "set_image_path", "get_image_path");
}
