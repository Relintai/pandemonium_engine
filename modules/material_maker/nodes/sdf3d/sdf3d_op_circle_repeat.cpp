
#include "sdf3d_op_circle_repeat.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dOpCircleRepeat::get_input() {
	return input;
}

void MMSdf3dOpCircleRepeat::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMSdf3dOpCircleRepeat::get_output() {
	return output;
}

void MMSdf3dOpCircleRepeat::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

int MMSdf3dOpCircleRepeat::get_count() const {
	return count;
}

void MMSdf3dOpCircleRepeat::set_count(const int val) {
	count = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdf3dOpCircleRepeat::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_VECTOR2);
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	//	input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_VECTOR2;
	input->set_slot_name(">>>   Input        ");

	if (!input->is_connected("changed", this, "on_input_changed")) {
		input->connect("changed", this, "on_input_changed");
	}

	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_VECTOR2);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);

	register_input_property(input);
	register_output_property(output);
}

void MMSdf3dOpCircleRepeat::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_int("get_count", "set_count", "Count");
}

Vector2 MMSdf3dOpCircleRepeat::_get_property_value_sdf3d(const Vector3 &uv3) {
	//todo make seed a class variable probably into MMNode;
	Vector3 new_uv = MMAlgos::circle_repeat_transform(uv3, count);
	return input->get_value_sdf3d(new_uv);
}

void MMSdf3dOpCircleRepeat::on_input_changed() {
	emit_changed();
	output->do_emit_changed();
}

MMSdf3dOpCircleRepeat::MMSdf3dOpCircleRepeat() {
	count = 5;
}

MMSdf3dOpCircleRepeat::~MMSdf3dOpCircleRepeat() {
}

void MMSdf3dOpCircleRepeat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &MMSdf3dOpCircleRepeat::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMSdf3dOpCircleRepeat::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_output"), &MMSdf3dOpCircleRepeat::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdf3dOpCircleRepeat::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_count"), &MMSdf3dOpCircleRepeat::get_count);
	ClassDB::bind_method(D_METHOD("set_count", "value"), &MMSdf3dOpCircleRepeat::set_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "count"), "set_count", "get_count");

	ClassDB::bind_method(D_METHOD("on_input_changed"), &MMSdf3dOpCircleRepeat::on_input_changed);
}
