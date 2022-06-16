
#include "translate.h"

#include "../../editor/mm_graph_node.h"

Ref<MMNodeUniversalProperty> MMTranslate::get_image() {
	return image;
}

void MMTranslate::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMTranslate::get_input() {
	return input;
}

void MMTranslate::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Vector2 MMTranslate::get_translation() {
	return translation;
}

void MMTranslate::set_translation(const Vector2 &val) {
	translation = val;
	set_dirty(true);
}

void MMTranslate::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color(0, 0, 0, 1));
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Input1    ");

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

void MMTranslate::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_texture_universal(image);
	mm_graph_node->add_slot_vector2("get_translation", "set_translation", "Translation", 0.01);
}

void MMTranslate::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMTranslate::_get_value_for(const Vector2 &uv, const int pseed) {
	//$i($uv-vec2($translate_x, $translate_y));
	return input->get_value(uv - translation);
}

MMTranslate::MMTranslate() {
}

MMTranslate::~MMTranslate() {
}

void MMTranslate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMTranslate::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMTranslate::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_input"), &MMTranslate::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMTranslate::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_translation"), &MMTranslate::get_translation);
	ClassDB::bind_method(D_METHOD("set_translation", "value"), &MMTranslate::set_translation);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "translation"), "set_translation", "get_translation");
}
