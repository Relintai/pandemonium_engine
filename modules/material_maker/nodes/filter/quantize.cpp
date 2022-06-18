
#include "quantize.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMQuantize::get_image() {
	return image;
}

void MMQuantize::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMQuantize::get_input() {
	return input;
}

void MMQuantize::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

int MMQuantize::get_steps() const {
	return steps;
}

void MMQuantize::set_steps(const int val) {
	steps = val;
	set_dirty(true);
}

void MMQuantize::_init_properties() {
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

void MMQuantize::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_int("get_steps", "set_steps", "Steps");
}

void MMQuantize::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMQuantize::_get_value_for(const Vector2 &uv, const int pseed) {
	Color c = input->get_value(uv);
	//vec4(floor($in($uv).rgb*$steps)/$steps, $in($uv).a);
	Vector3 v = MMAlgos::floorv3(Vector3(c.r, c.g, c.b) * steps) / float(steps);
	return Color(v.x, v.y, v.z, c.a);
}

MMQuantize::MMQuantize() {
	steps = 4;
}

MMQuantize::~MMQuantize() {
}

void MMQuantize::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMQuantize::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMQuantize::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMQuantize::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMQuantize::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_steps"), &MMQuantize::get_steps);
	ClassDB::bind_method(D_METHOD("set_steps", "value"), &MMQuantize::set_steps);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "steps"), "set_steps", "get_steps");
}
