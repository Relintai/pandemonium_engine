
#include "sd_op_morph.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdOpMorph::get_input1() {
	return input1;
}

void MMSdOpMorph::set_input1(const Ref<MMNodeUniversalProperty> &val) {
	input1 = val;
}

Ref<MMNodeUniversalProperty> MMSdOpMorph::get_input2() {
	return input2;
}

void MMSdOpMorph::set_input2(const Ref<MMNodeUniversalProperty> &val) {
	input2 = val;
}

Ref<MMNodeUniversalProperty> MMSdOpMorph::get_output() {
	return output;
}

void MMSdOpMorph::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

float MMSdOpMorph::get_amount() const {
	return amount;
}

void MMSdOpMorph::set_amount(const float val) {
	amount = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdOpMorph::_init_properties() {
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

void MMSdOpMorph::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input1);
	mm_graph_node->add_slot_label_universal(input2);
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_float("get_amount", "set_amount", "Amount", 0.01);
}

Variant MMSdOpMorph::_get_property_value(const Vector2 &uv) {
	return MMAlgos::sdf_morph(input1->get_value(uv), input2->get_value(uv), amount);
}

void MMSdOpMorph::on_input_changed() {
	emit_changed();
	output->do_emit_changed();
}

MMSdOpMorph::MMSdOpMorph() {
	amount = 0.5;
}

MMSdOpMorph::~MMSdOpMorph() {
}

void MMSdOpMorph::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input1"), &MMSdOpMorph::get_input1);
	ClassDB::bind_method(D_METHOD("set_input1", "value"), &MMSdOpMorph::set_input1);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input1", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input1", "get_input1");

	ClassDB::bind_method(D_METHOD("get_input2"), &MMSdOpMorph::get_input2);
	ClassDB::bind_method(D_METHOD("set_input2", "value"), &MMSdOpMorph::set_input2);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input2", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input2", "get_input2");

	ClassDB::bind_method(D_METHOD("get_output"), &MMSdOpMorph::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdOpMorph::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_amount"), &MMSdOpMorph::get_amount);
	ClassDB::bind_method(D_METHOD("set_amount", "value"), &MMSdOpMorph::set_amount);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "amount"), "set_amount", "get_amount");

	ClassDB::bind_method(D_METHOD("on_input_changed"), &MMSdOpMorph::on_input_changed);
}
