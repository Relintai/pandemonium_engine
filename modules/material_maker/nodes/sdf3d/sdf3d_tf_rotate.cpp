
#include "sdf3d_tf_rotate.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dTfRotate::get_input() {
	return input;
}

void MMSdf3dTfRotate::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMSdf3dTfRotate::get_output() {
	return output;
}

void MMSdf3dTfRotate::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

Vector3 MMSdf3dTfRotate::get_rotation() {
	return rotation;
}

void MMSdf3dTfRotate::set_rotation(const Vector3 &val) {
	rotation = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdf3dTfRotate::_init_properties() {
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

void MMSdf3dTfRotate::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_vector3("get_rotation", "set_rotation", "Rotation", 0.01);
}

Vector2 MMSdf3dTfRotate::_get_property_value_sdf3d(const Vector3 &uv3) {
	//$in(rotate3d($uv, -vec3($ax, $ay, $az)*0.01745329251));
	return input->get_value_sdf3d(MMAlgos::rotate3d(uv3, -rotation * 0.01745329251));
}

void MMSdf3dTfRotate::on_input_changed() {
	emit_changed();
	output->do_emit_changed();
}

MMSdf3dTfRotate::MMSdf3dTfRotate() {
}

MMSdf3dTfRotate::~MMSdf3dTfRotate() {
}

void MMSdf3dTfRotate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &MMSdf3dTfRotate::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMSdf3dTfRotate::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_output"), &MMSdf3dTfRotate::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdf3dTfRotate::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_rotation"), &MMSdf3dTfRotate::get_rotation);
	ClassDB::bind_method(D_METHOD("set_rotation", "value"), &MMSdf3dTfRotate::set_rotation);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "rotation"), "set_rotation", "get_rotation");

	ClassDB::bind_method(D_METHOD("on_input_changed"), &MMSdf3dTfRotate::on_input_changed);
}
