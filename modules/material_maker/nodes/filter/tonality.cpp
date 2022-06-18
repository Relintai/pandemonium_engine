
#include "tonality.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMTonality::get_image() {
	return image;
}

void MMTonality::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMTonality::get_input() {
	return input;
}

void MMTonality::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

void MMTonality::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		input->set_default_value(0);
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

void MMTonality::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_curve();
}

void MMTonality::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMTonality::_get_value_for(const Vector2 &uv, const int pseed) {
	float f = input->get_value(uv);
	float cf = MMAlgos::curve(f, points);
	return Color(cf, cf, cf, 1);
}

void MMTonality::_curve_changed() {
	set_dirty(true);
}

MMTonality::MMTonality() {
}

MMTonality::~MMTonality() {
}

void MMTonality::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		if (points.size() == 0) {
			init_points_01();
		}
	}
}

void MMTonality::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMTonality::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMTonality::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMTonality::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMTonality::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");
}
