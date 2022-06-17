
#include "sd_shape_arc.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdShapeArc::get_output() {
	return output;
}

void MMSdShapeArc::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

Vector2 MMSdShapeArc::get_angle() {
	return angle;
}

void MMSdShapeArc::set_angle(const Vector2 &val) {
	angle = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdShapeArc::get_radius() const {
	return radius;
}

void MMSdShapeArc::set_radius(const float val) {
	radius = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdShapeArc::get_width() const {
	return width;
}

void MMSdShapeArc::set_width(const float val) {
	width = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdShapeArc::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);

	register_output_property(output);
}

void MMSdShapeArc::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_vector2("get_angle", "set_angle", "Angle", 1);
	mm_graph_node->add_slot_float("get_radius", "set_radius", "Radius", 0.01);
	mm_graph_node->add_slot_float("get_width", "set_width", "Width", 0.01);
}

Variant MMSdShapeArc::_get_property_value(const Vector2 &uv) {
	return MMAlgos::sdf_arc(uv, angle, Vector2(radius, width));
}

MMSdShapeArc::MMSdShapeArc() {
	angle = Vector2(30, 150);
	radius = 0.3;
	width = 0.1;
}

MMSdShapeArc::~MMSdShapeArc() {
}

void MMSdShapeArc::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdShapeArc::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdShapeArc::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_angle"), &MMSdShapeArc::get_angle);
	ClassDB::bind_method(D_METHOD("set_angle", "value"), &MMSdShapeArc::set_angle);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "angle"), "set_angle", "get_angle");

	ClassDB::bind_method(D_METHOD("get_radius"), &MMSdShapeArc::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius", "value"), &MMSdShapeArc::set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");

	ClassDB::bind_method(D_METHOD("get_width"), &MMSdShapeArc::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "value"), &MMSdShapeArc::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");
}
