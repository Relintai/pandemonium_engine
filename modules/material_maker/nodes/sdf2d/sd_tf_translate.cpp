
#include "sd_tf_translate.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdTfTranslate::get_output() {
	return output;
}

void MMSdTfTranslate::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

Vector2 MMSdTfTranslate::get_translation() {
	return translation;
}

void MMSdTfTranslate::set_translation(const Vector2 &val) {
	translation = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdTfTranslate::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	output->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	//output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	output->set_slot_name(">>>    Apply    >>>");
	output->set_get_value_from_owner(true);

	register_input_property(output);
	register_output_property(output);
}

void MMSdTfTranslate::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_vector2("get_translation", "set_translation", "Translation", 0.01);
}

Variant MMSdTfTranslate::_get_property_value(const Vector2 &uv) {
	return output->get_value(uv - translation, true);
}

MMSdTfTranslate::MMSdTfTranslate() {
	translation = Vector2(0, 0);
}

MMSdTfTranslate::~MMSdTfTranslate() {
}

void MMSdTfTranslate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdTfTranslate::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdTfTranslate::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_translation"), &MMSdTfTranslate::get_translation);
	ClassDB::bind_method(D_METHOD("set_translation", "value"), &MMSdTfTranslate::set_translation);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "translation"), "set_translation", "get_translation");
}
