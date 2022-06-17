
#include "sd_shape_circle.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdShapeCircle::get_output() {
	return output;
}

void MMSdShapeCircle::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

Vector2 MMSdShapeCircle::get_center() {
	return center;
}

void MMSdShapeCircle::set_center(const Vector2 &val) {
	center = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdShapeCircle::get_radius() const {
	return radius;
}

void MMSdShapeCircle::set_radius(const float val) {
	radius = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdShapeCircle::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);

	register_output_property(output);
}

void MMSdShapeCircle::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_vector2("get_center", "set_center", "Center", 0.01);
	mm_graph_node->add_slot_float("get_radius", "set_radius", "Radius", 0.01);
}

Variant MMSdShapeCircle::_get_property_value(const Vector2 &uv) {
	return MMAlgos::sdf_circle(uv, center, radius);
}

MMSdShapeCircle::MMSdShapeCircle() {
	center = Vector2(0, 0);
	radius = 0.4;
}

MMSdShapeCircle::~MMSdShapeCircle() {
}

void MMSdShapeCircle::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdShapeCircle::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdShapeCircle::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_center"), &MMSdShapeCircle::get_center);
	ClassDB::bind_method(D_METHOD("set_center", "value"), &MMSdShapeCircle::set_center);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "center"), "set_center", "get_center");

	ClassDB::bind_method(D_METHOD("get_radius"), &MMSdShapeCircle::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius", "value"), &MMSdShapeCircle::set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");
}
