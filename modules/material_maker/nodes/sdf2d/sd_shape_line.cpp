
#include "sd_shape_line.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdShapeLine::get_output() {
	return output;
}

void MMSdShapeLine::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

Vector2 MMSdShapeLine::get_a() {
	return A;
}

void MMSdShapeLine::set_a(const Vector2 &val) {
	A = val;
	emit_changed();
	output->do_emit_changed();
}

Vector2 MMSdShapeLine::get_b() {
	return B;
}

void MMSdShapeLine::set_b(const Vector2 &val) {
	B = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdShapeLine::get_width() const {
	return width;
}

void MMSdShapeLine::set_width(const float val) {
	width = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdShapeLine::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);

	register_output_property(output);
}

void MMSdShapeLine::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_vector2("get_a", "set_a", "A", 0.01);
	mm_graph_node->add_slot_vector2("get_b", "set_b", "B", 0.01);
	mm_graph_node->add_slot_float("get_width", "set_width", "Width", 0.01);
	mm_graph_node->add_slot_curve();
}

Variant MMSdShapeLine::_get_property_value(const Vector2 &uv) {
	Vector2 line = MMAlgos::sdf_line(uv, A, B, width);
	//$(name_uv)_sdl.x - $r * $profile($(name_uv)_sdl.y);
	return line.x - width * MMAlgos::curve(line.y, points);
}

void MMSdShapeLine::_curve_changed() {
	emit_changed();
	output->do_emit_changed();
}

MMSdShapeLine::MMSdShapeLine() {
	A = Vector2(-0.3, -0.3);
	B = Vector2(0.3, 0.3);
	width = 0.1;
}

MMSdShapeLine::~MMSdShapeLine() {
}

void MMSdShapeLine::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		if (points.size() == 0) {
			init_points_11();
		}
	}
}

void MMSdShapeLine::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdShapeLine::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdShapeLine::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_a"), &MMSdShapeLine::get_a);
	ClassDB::bind_method(D_METHOD("set_a", "value"), &MMSdShapeLine::set_a);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "A"), "set_a", "get_a");

	ClassDB::bind_method(D_METHOD("get_b"), &MMSdShapeLine::get_b);
	ClassDB::bind_method(D_METHOD("set_b", "value"), &MMSdShapeLine::set_b);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "B"), "set_b", "get_b");

	ClassDB::bind_method(D_METHOD("get_width"), &MMSdShapeLine::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "value"), &MMSdShapeLine::set_width);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "width"), "set_width", "get_width");
}
