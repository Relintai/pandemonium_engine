
#include "sdf3d_op_elongation.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dOpElongation::get_input() {
	return input;
}

void MMSdf3dOpElongation::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMSdf3dOpElongation::get_output() {
	return output;
}

void MMSdf3dOpElongation::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

Vector3 MMSdf3dOpElongation::get_length() {
	return length;
}

void MMSdf3dOpElongation::set_length(const Vector3 &val) {
	length = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdf3dOpElongation::_init_properties() {
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

void MMSdf3dOpElongation::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_vector3("get_length", "set_length", "Length", 0.01);
}

Vector2 MMSdf3dOpElongation::_get_property_value_sdf3d(const Vector3 &uv3) {
	//$in($uv - clamp($uv, -abs(vec3($x, $y, $z)), abs(vec3($x, $y, $z))));
	Vector3 new_uv = uv3 - MMAlgos::clampv3(uv3, -MMAlgos::absv3(length), MMAlgos::absv3(length));
	return input->get_value_sdf3d(new_uv);
}

void MMSdf3dOpElongation::on_input_changed() {
	emit_changed();
	output->do_emit_changed();
}

MMSdf3dOpElongation::MMSdf3dOpElongation() {
	length = Vector3(0.2, 0, 0);
}

MMSdf3dOpElongation::~MMSdf3dOpElongation() {
}

void MMSdf3dOpElongation::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &MMSdf3dOpElongation::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMSdf3dOpElongation::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_output"), &MMSdf3dOpElongation::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdf3dOpElongation::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_length"), &MMSdf3dOpElongation::get_length);
	ClassDB::bind_method(D_METHOD("set_length", "value"), &MMSdf3dOpElongation::set_length);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "length"), "set_length", "get_length");

	ClassDB::bind_method(D_METHOD("on_input_changed"), &MMSdf3dOpElongation::on_input_changed);
}
