
#include "fill_to_uv.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMFillToUv::get_image() {
	return image;
}

void MMFillToUv::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMFillToUv::get_input() {
	return input;
}

void MMFillToUv::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

int MMFillToUv::get_mode() const {
	return mode;
}

void MMFillToUv::set_mode(const int val) {
	mode = val;
	set_dirty(true);
}

void MMFillToUv::_init_properties() {
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

void MMFillToUv::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("Stretch");
	arr.push_back("Square");

	mm_graph_node->add_slot_enum("get_mode", "set_mode", "Mode", arr);
}

void MMFillToUv::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMFillToUv::_get_value_for(const Vector2 &uv, const int pseed) {
	//vec4 $(name_uv)_bb = $in($uv);
	Color c = input->get_value(uv);
	//fill_to_uv_$mode($uv, $(name_uv)_bb, float($seed));
	Vector3 r = Vector3();

	if (mode == 0) {
		r = MMAlgos::fill_to_uv_stretch(uv, c, float(pseed));
	} else if (mode == 1) {
		r = MMAlgos::fill_to_uv_square(uv, c, float(pseed));
	}

	return Color(r.x, r.y, r.z, 1);
}

MMFillToUv::MMFillToUv() {
	mode = 0;
}

MMFillToUv::~MMFillToUv() {
}

void MMFillToUv::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMFillToUv::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMFillToUv::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMFillToUv::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMFillToUv::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_mode"), &MMFillToUv::get_mode);
	ClassDB::bind_method(D_METHOD("set_mode", "value"), &MMFillToUv::set_mode);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mode"), "set_mode", "get_mode");
}
