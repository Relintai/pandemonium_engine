
#include "repeat.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMRepeat::get_input() {
	return input;
}

void MMRepeat::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

void MMRepeat::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		input->set_default_value(Color(0, 0, 0, 1));
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_slot_name(">>>    Apply    >>>");
	//input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_COLOR;
	input->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	input->set_get_value_from_owner(true);

	register_input_property(input);
	register_output_property(input);
}

void MMRepeat::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
}

Variant MMRepeat::_get_property_value(const Vector2 &uv) {
	return input->get_value(MMAlgos::fractv2(uv), true);
}

MMRepeat::MMRepeat() {
}

MMRepeat::~MMRepeat() {
}

void MMRepeat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &MMRepeat::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMRepeat::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");
}
