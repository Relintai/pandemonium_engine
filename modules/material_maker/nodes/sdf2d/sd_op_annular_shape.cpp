
#include "sd_op_annular_shape.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdOpAnnularShape::get_output() {
	return output;
}

void MMSdOpAnnularShape::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

float MMSdOpAnnularShape::get_width() const {
	return width;
}

void MMSdOpAnnularShape::set_width(const float val) {
	width = val;
	emit_changed();
	output->do_emit_changed();
}

int MMSdOpAnnularShape::get_ripples() const {
	return ripples;
}

void MMSdOpAnnularShape::set_ripples(const int val) {
	ripples = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdOpAnnularShape::_init_properties() {
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

void MMSdOpAnnularShape::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_float("get_width", "set_width", "Width", 0.01);
	mm_graph_node->add_slot_int("get_ripples", "set_ripples", "Ripples");
}

Variant MMSdOpAnnularShape::_get_property_value(const Vector2 &uv) {
	float val = output->get_value(uv, true);
	return MMAlgos::sdRipples(val, width, ripples);
}

MMSdOpAnnularShape::MMSdOpAnnularShape() {
	width = 0.1;
	ripples = 1;
}

MMSdOpAnnularShape::~MMSdOpAnnularShape() {
}

void MMSdOpAnnularShape::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdOpAnnularShape::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdOpAnnularShape::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_width"), &MMSdOpAnnularShape::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "value"), &MMSdOpAnnularShape::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");

	ClassDB::bind_method(D_METHOD("get_ripples"), &MMSdOpAnnularShape::get_ripples);
	ClassDB::bind_method(D_METHOD("set_ripples", "value"), &MMSdOpAnnularShape::set_ripples);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ripples"), "set_ripples", "get_ripples");
}
