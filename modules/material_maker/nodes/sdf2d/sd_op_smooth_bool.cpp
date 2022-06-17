
#include "sd_op_smooth_bool.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdOpSmoothBool::get_input1() {
	return input1;
}

void MMSdOpSmoothBool::set_input1(const Ref<MMNodeUniversalProperty> &val) {
	input1 = val;
}

Ref<MMNodeUniversalProperty> MMSdOpSmoothBool::get_input2() {
	return input2;
}

void MMSdOpSmoothBool::set_input2(const Ref<MMNodeUniversalProperty> &val) {
	input2 = val;
}

Ref<MMNodeUniversalProperty> MMSdOpSmoothBool::get_output() {
	return output;
}

void MMSdOpSmoothBool::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

int MMSdOpSmoothBool::get_operation() const {
	return operation;
}

void MMSdOpSmoothBool::set_operation(const int val) {
	operation = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdOpSmoothBool::get_smoothness() const {
	return smoothness;
}

void MMSdOpSmoothBool::set_smoothness(const float val) {
	smoothness = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdOpSmoothBool::_init_properties() {
	if (!input1.is_valid()) {
		input1.instance();
		input1->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	input1->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	//	input1.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	input1->set_slot_name(">>>   Input 1       ");

	if (!input1->is_connected("changed", this, "on_input_changed")) {
		input1->connect("changed", this, "on_input_changed");
	}

	if (!input2.is_valid()) {
		input2.instance();
		input2->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	input2->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	//	input2.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	input2->set_slot_name(">>>   Input 2       ");

	if (!input2->is_connected("changed", this, "on_input_changed")) {
		input2->connect("changed", this, "on_input_changed");
	}

	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	//	output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	output->set_slot_name("       Output    >>>");
	output->set_get_value_from_owner(true);

	register_input_property(input1);
	register_input_property(input2);
	register_output_property(output);
}

void MMSdOpSmoothBool::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input1);
	mm_graph_node->add_slot_label_universal(input2);
	mm_graph_node->add_slot_label_universal(output);

	Array arr;
	arr.push_back("Union");
	arr.push_back("Substraction");
	arr.push_back("Intersection");

	mm_graph_node->add_slot_enum("get_operation", "set_operation", "Operation", arr);
	mm_graph_node->add_slot_float("get_smoothness", "set_smoothness", "Smoothness", 0.01);
}

Variant MMSdOpSmoothBool::_get_property_value(const Vector2 &uv) {
	if (operation == 0) {
		return MMAlgos::sdf_smooth_boolean_union(input1->get_value(uv), input2->get_value(uv), smoothness);
	} else if (operation == 1) {
		return MMAlgos::sdf_smooth_boolean_substraction(input1->get_value(uv), input2->get_value(uv), smoothness);
	} else if (operation == 2) {
		return MMAlgos::sdf_smooth_boolean_intersection(input1->get_value(uv), input2->get_value(uv), smoothness);
	}

	return 0.0;
}

void MMSdOpSmoothBool::on_input_changed() {
	emit_changed();
	output->do_emit_changed();
}

MMSdOpSmoothBool::MMSdOpSmoothBool() {
	operation = 0;
	smoothness = 0.15;
}

MMSdOpSmoothBool::~MMSdOpSmoothBool() {
}

void MMSdOpSmoothBool::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input1"), &MMSdOpSmoothBool::get_input1);
	ClassDB::bind_method(D_METHOD("set_input1", "value"), &MMSdOpSmoothBool::set_input1);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input1", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input1", "get_input1");

	ClassDB::bind_method(D_METHOD("get_input2"), &MMSdOpSmoothBool::get_input2);
	ClassDB::bind_method(D_METHOD("set_input2", "value"), &MMSdOpSmoothBool::set_input2);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input2", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input2", "get_input2");

	ClassDB::bind_method(D_METHOD("get_output"), &MMSdOpSmoothBool::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdOpSmoothBool::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_operation"), &MMSdOpSmoothBool::get_operation);
	ClassDB::bind_method(D_METHOD("set_operation", "value"), &MMSdOpSmoothBool::set_operation);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "operation"), "set_operation", "get_operation");

	ClassDB::bind_method(D_METHOD("get_smoothness"), &MMSdOpSmoothBool::get_smoothness);
	ClassDB::bind_method(D_METHOD("set_smoothness", "value"), &MMSdOpSmoothBool::set_smoothness);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "smoothness"), "set_smoothness", "get_smoothness");

	ClassDB::bind_method(D_METHOD("on_input_changed"), &MMSdOpSmoothBool::on_input_changed);
}
