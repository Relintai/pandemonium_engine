
#include "sdf3d_shape_box.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdf3dShapeBox::get_output() {
	return output;
}

void MMSdf3dShapeBox::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

Vector3 MMSdf3dShapeBox::get_size() {
	return size;
}

void MMSdf3dShapeBox::set_size(const Vector3 &val) {
	size = val;
	emit_changed();
	output->do_emit_changed();
}

float MMSdf3dShapeBox::get_radius() const {
	return radius;
}

void MMSdf3dShapeBox::set_radius(const float val) {
	radius = val;
	emit_changed();
	output->do_emit_changed();
}

void MMSdf3dShapeBox::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_VECTOR2);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);

	register_output_property(output);
}

void MMSdf3dShapeBox::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_vector3("get_size", "set_size", "Size", 0.01);
	mm_graph_node->add_slot_float("get_radius", "set_radius", "Radius", 0.01);
}

Vector2 MMSdf3dShapeBox::_get_property_value_sdf3d(const Vector3 &uv3) {
	return MMAlgos::sdf3d_box(uv3, size.x, size.y, size.z, radius);
}

MMSdf3dShapeBox::MMSdf3dShapeBox() {
	size = Vector3(0.3, 0.25, 0.25);
	radius = 0.01;
}

MMSdf3dShapeBox::~MMSdf3dShapeBox() {
}

void MMSdf3dShapeBox::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdf3dShapeBox::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdf3dShapeBox::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_size"), &MMSdf3dShapeBox::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "value"), &MMSdf3dShapeBox::set_size);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "size"), "set_size", "get_size");

	ClassDB::bind_method(D_METHOD("get_radius"), &MMSdf3dShapeBox::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius", "value"), &MMSdf3dShapeBox::set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");
}
