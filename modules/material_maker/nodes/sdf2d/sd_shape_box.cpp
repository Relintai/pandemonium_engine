
#include "sd_shape_box.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdShapeBox::get_output() {
	return output;
}

void MMSdShapeBox::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

Vector2 MMSdShapeBox::get_center() {
	return center;
}

void MMSdShapeBox::set_center(const Vector2 &val) {
	center = val;
	emit_changed();
	output->do_emit_changed();
}

Vector2 MMSdShapeBox::get_size() {
	return size;
}

void MMSdShapeBox::set_size(const Vector2 &val) {
	size = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdShapeBox::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);

	register_output_property(output);
}

void MMSdShapeBox::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_vector2("get_center", "set_center", "Center", 0.01);
	mm_graph_node->add_slot_vector2("get_size", "set_size", "Size", 0.01);
}

Variant MMSdShapeBox::_get_property_value(const Vector2 &uv) {
	return MMAlgos::sdf_box(uv, center, size);
}

MMSdShapeBox::MMSdShapeBox() {
	center = Vector2(0, 0);
	size = Vector2(0.3, 0.2);
}

MMSdShapeBox::~MMSdShapeBox() {
}

void MMSdShapeBox::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdShapeBox::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdShapeBox::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_center"), &MMSdShapeBox::get_center);
	ClassDB::bind_method(D_METHOD("set_center", "value"), &MMSdShapeBox::set_center);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "center"), "set_center", "get_center");

	ClassDB::bind_method(D_METHOD("get_size"), &MMSdShapeBox::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &MMSdShapeBox::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size"), "set_size", "get_size");
}
