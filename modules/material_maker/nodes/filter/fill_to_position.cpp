
#include "fill_to_position.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMFillToPosition::get_image() {
	return image;
}

void MMFillToPosition::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMFillToPosition::get_input() {
	return input;
}

void MMFillToPosition::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

int MMFillToPosition::get_axis() const {
	return axis;
}

void MMFillToPosition::set_axis(const int val) {
	axis = val;
	set_dirty(true);
}

void MMFillToPosition::_init_properties() {
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

void MMFillToPosition::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("X");
	arr.push_back("Y");
	arr.push_back("Radial");

	mm_graph_node->add_slot_enum("get_axis", "set_axis", "Axis", arr);
}

void MMFillToPosition::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMFillToPosition::_get_value_for(const Vector2 &uv, const int pseed) {
	Color c = input->get_value(uv);
	//vec2 $(name_uv)_c = fract($in($uv).xy+0.5*$in($uv).zw);
	Vector2 cnv = MMAlgos::fractv2(Vector2(c.r, c.g) + 0.5 * Vector2(c.b, c.a));
	//X, $(name_uv)_c.x;
	//Y, $(name_uv)_c.y;
	//Radial, length($(name_uv)_c-vec2(0.5));

	if (axis == 0) {
		return Color(cnv.x, cnv.x, cnv.x, 1);
	} else if (axis == 1) {
		return Color(cnv.y, cnv.y, cnv.y, 1);
	} else if (axis == 2) {
		float f = (cnv - Vector2(0.5, 0.5)).length();
		return Color(f, f, f, 1);
	}

	return Color(0, 0, 0, 1);
}

MMFillToPosition::MMFillToPosition() {
	axis = 2;
}

MMFillToPosition::~MMFillToPosition() {
}

void MMFillToPosition::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMFillToPosition::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMFillToPosition::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMFillToPosition::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMFillToPosition::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_axis"), &MMFillToPosition::get_axis);
	ClassDB::bind_method(D_METHOD("set_axis", "value"), &MMFillToPosition::set_axis);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "axis"), "set_axis", "get_axis");
}
