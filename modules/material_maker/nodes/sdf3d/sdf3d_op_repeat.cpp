
#include "sdf3d_op_repeat.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dOpRepeat::get_input() {
	return input;
}

void MMSdf3dOpRepeat::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMSdf3dOpRepeat::get_output() {
	return output;
}

void MMSdf3dOpRepeat::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

Vector2 MMSdf3dOpRepeat::get_col_row() {
	return col_row;
}

void MMSdf3dOpRepeat::set_col_row(const Vector2 &val) {
	col_row = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdf3dOpRepeat::get_rotation() const {
	return rotation;
}

void MMSdf3dOpRepeat::set_rotation(const float val) {
	rotation = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdf3dOpRepeat::_init_properties() {
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

void MMSdf3dOpRepeat::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_vector2("get_col_row", "set_col_row", "Col,Row", 1);
	mm_graph_node->add_slot_float("get_rotation", "set_rotation", "Rotation", 0.01);
}

Vector2 MMSdf3dOpRepeat::_get_property_value_sdf3d(const Vector3 &uv3) {
	//todo make seed a class variable probably into MMNode;
	Vector3 new_uv = MMAlgos::sdf3d_repeat(uv3, col_row, rotation, 1);
	return input->get_value_sdf3d(new_uv);
}

void MMSdf3dOpRepeat::on_input_changed() {
	emit_changed();
	output->do_emit_changed();
}

MMSdf3dOpRepeat::MMSdf3dOpRepeat() {
	col_row = Vector2(3, 3);
	rotation = 0.3;
}

MMSdf3dOpRepeat::~MMSdf3dOpRepeat() {
}

void MMSdf3dOpRepeat::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &MMSdf3dOpRepeat::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMSdf3dOpRepeat::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_output"), &MMSdf3dOpRepeat::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdf3dOpRepeat::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_col_row"), &MMSdf3dOpRepeat::get_col_row);
	ClassDB::bind_method(D_METHOD("set_col_row", "value"), &MMSdf3dOpRepeat::set_col_row);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "col_row"), "set_col_row", "get_col_row");

	ClassDB::bind_method(D_METHOD("get_rotation"), &MMSdf3dOpRepeat::get_rotation);
	ClassDB::bind_method(D_METHOD("set_rotation", "value"), &MMSdf3dOpRepeat::set_rotation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "rotation"), "set_rotation", "get_rotation");

	ClassDB::bind_method(D_METHOD("on_input_changed"), &MMSdf3dOpRepeat::on_input_changed);
}
