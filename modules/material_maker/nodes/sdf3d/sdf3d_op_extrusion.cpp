
#include "sdf3d_op_extrusion.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> Sdf3dOpExtrusion::get_input() {
	return input;
}

void Sdf3dOpExtrusion::set_input(const Ref<MMNodeUniversalProperty> &val) {
	input = val;
}

Ref<MMNodeUniversalProperty> Sdf3dOpExtrusion::get_output() {
	return output;
}

void Sdf3dOpExtrusion::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

float Sdf3dOpExtrusion::get_length() const {
	return length;
}

void Sdf3dOpExtrusion::set_length(const float val) {
	length = val;
	emit_changed();
	output->do_emit_changed();
}

void Sdf3dOpExtrusion::_init_properties() {
	if (!input.is_valid()) {
		input.instance();
		input->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	input->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	//	input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
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

void Sdf3dOpExtrusion::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(input);
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_float("get_length", "set_length", "Length", 0.01);
}

Vector2 Sdf3dOpExtrusion::_get_property_value_sdf3d(const Vector3 &uv3) {
	//vec2 $(name_uv)_w = vec2($in($uv.xz+vec2(0.5)),abs($uv.y)-$d);
	//ret min(max($(name_uv)_w.x,$(name_uv)_w.y),0.0)+length(max($(name_uv)_w,0.0));
	float f = input->get_value(Vector2(uv3.x, uv3.z) + Vector2(0.5, 0.5));
	Vector2 w = Vector2(f, ABS(uv3.y) - length);
	float ff = MIN(MAX(w.x, w.y), 0.0) + MMAlgos::maxv2(w, Vector2()).length();
	return Vector2(ff, 0);
}

void Sdf3dOpExtrusion::on_input_changed() {
	emit_changed();
	output->do_emit_changed();
}

Sdf3dOpExtrusion::Sdf3dOpExtrusion() {
	length = 0.25;
}

Sdf3dOpExtrusion::~Sdf3dOpExtrusion() {
}

void Sdf3dOpExtrusion::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input"), &Sdf3dOpExtrusion::get_input);
	ClassDB::bind_method(D_METHOD("set_input", "value"), &Sdf3dOpExtrusion::set_input);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_input", "get_input");

	ClassDB::bind_method(D_METHOD("get_output"), &Sdf3dOpExtrusion::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &Sdf3dOpExtrusion::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_length"), &Sdf3dOpExtrusion::get_length);
	ClassDB::bind_method(D_METHOD("set_length", "value"), &Sdf3dOpExtrusion::set_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "length"), "set_length", "get_length");

	ClassDB::bind_method(D_METHOD("on_input_changed"), &Sdf3dOpExtrusion::on_input_changed);
}
