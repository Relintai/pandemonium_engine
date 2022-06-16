
#include "kaleidoscope.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMKaleidoscope::get_image() {
	return image;
}

void MMKaleidoscope::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMKaleidoscope::get_input() {
	return input;
}

void MMKaleidoscope::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

int MMKaleidoscope::get_count() const {
	return count;
}

void MMKaleidoscope::set_count(const int val) {
	count = val;
	set_dirty(true);
}

float MMKaleidoscope::get_offset() const {
	return offset;
}

void MMKaleidoscope::set_offset(const float val) {
	offset = val;
	set_dirty(true);
}

void MMKaleidoscope::_init_properties() {
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

void MMKaleidoscope::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_int("get_count", "set_count", "Count");
	mm_graph_node->add_slot_float("get_offset", "set_offset", "Offset", 0.5);
}

void MMKaleidoscope::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMKaleidoscope::_get_value_for(const Vector2 &uv, const int pseed) {
	//$i(kal_rotate($uv, $count, $offset));
	return input->get_value(MMAlgos::kal_rotate(uv, count, offset));
}

MMKaleidoscope::MMKaleidoscope() {
	count = 5;
	offset = 0;
}

MMKaleidoscope::~MMKaleidoscope() {
}

void MMKaleidoscope::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMKaleidoscope::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMKaleidoscope::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMKaleidoscope::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMKaleidoscope::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_count"), &MMKaleidoscope::get_count);
	ClassDB::bind_method(D_METHOD("set_count", "value"), &MMKaleidoscope::set_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "count"), "set_count", "get_count");

	ClassDB::bind_method(D_METHOD("get_offset"), &MMKaleidoscope::get_offset);
	ClassDB::bind_method(D_METHOD("set_offset", "value"), &MMKaleidoscope::set_offset);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "offset"), "set_offset", "get_offset");
}
