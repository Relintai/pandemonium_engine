
#include "sd_op_rounded_shape.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdOpRoundedShape::get_output() {
	return output;
}

void MMSdOpRoundedShape::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

float MMSdOpRoundedShape::get_radius() const {
	return radius;
}

void MMSdOpRoundedShape::set_radius(const float val) {
	radius = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdOpRoundedShape::_init_properties() {
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

void MMSdOpRoundedShape::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_float("get_radius", "set_radius", "Radius", 0.01);
}

Variant MMSdOpRoundedShape::_get_property_value(const Vector2 &uv) {
	float val = output->get_value(uv, true);
	return MMAlgos::sdf_rounded_shape(val, radius);
}

MMSdOpRoundedShape::MMSdOpRoundedShape() {
	radius = 0;
}

MMSdOpRoundedShape::~MMSdOpRoundedShape() {
}

void MMSdOpRoundedShape::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdOpRoundedShape::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdOpRoundedShape::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_radius"), &MMSdOpRoundedShape::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius", "value"), &MMSdOpRoundedShape::set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");
}
