
#include "sdf3d_tf_scale.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dTfScale::get_input() {
	return input;
}

void MMSdf3dTfScale::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> MMSdf3dTfScale::get_output() {
	return output;
}

void MMSdf3dTfScale::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

float MMSdf3dTfScale::get_scale() const {
	return scale;
}

void MMSdf3dTfScale::set_scale(const float val) {
	scale = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdf3dTfScale::_init_properties() {
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

void MMSdf3dTfScale::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_float("get_scale", "set_scale", "Scale", 0.01);
}

Vector2 MMSdf3dTfScale::_get_property_value_sdf3d(const Vector3 &uv3) {
	//vec2 $(name_uv)_in = $in(($uv)/$s);
	return input->get_value_sdf3d(uv3 / scale);
}

void MMSdf3dTfScale::on_input_changed() {
	emit_changed();
	output->do_emit_changed();
}

MMSdf3dTfScale::MMSdf3dTfScale() {
	scale = 1;
}

MMSdf3dTfScale::~MMSdf3dTfScale() {
}

void MMSdf3dTfScale::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &MMSdf3dTfScale::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &MMSdf3dTfScale::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_output"), &MMSdf3dTfScale::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdf3dTfScale::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_scale"), &MMSdf3dTfScale::get_scale);
	ClassDB::bind_method(D_METHOD("set_scale", "value"), &MMSdf3dTfScale::set_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "scale"), "set_scale", "get_scale");

	ClassDB::bind_method(D_METHOD("on_input_changed"), &MMSdf3dTfScale::on_input_changed);
}
