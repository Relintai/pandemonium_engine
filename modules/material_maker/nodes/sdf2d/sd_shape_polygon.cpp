
#include "sd_shape_polygon.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMSdShapePolygon::get_output() {
	return output;
}

void MMSdShapePolygon::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

void MMSdShapePolygon::_init_properties() {
	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
	output->set_slot_name(">>>   Output    >>>");
	output->set_get_value_from_owner(true);

	register_output_property(output);
}

void MMSdShapePolygon::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_polygon();
}

Variant MMSdShapePolygon::_get_property_value(const Vector2 &uv) {
	return MMAlgos::sdPolygon(uv, points);
}

void MMSdShapePolygon::_polygon_changed() {
	emit_changed();
	output->do_emit_changed();
}

MMSdShapePolygon::MMSdShapePolygon() {
}

MMSdShapePolygon::~MMSdShapePolygon() {
}

void MMSdShapePolygon::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_output"), &MMSdShapePolygon::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMSdShapePolygon::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");
}
