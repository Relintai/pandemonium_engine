
#include "greyscale_uniform.h"

#include "../../editor/mm_graph_node.h"

Ref<MMNodeUniversalProperty> MMGreyscaleUniform::get_uniform() {
	return uniform;
}

void MMGreyscaleUniform::set_uniform(const Ref<MMNodeUniversalProperty> &val) {
	uniform = val;
}

void MMGreyscaleUniform::_init_properties() {
	if (!uniform.is_valid()) {
		uniform.instance();
		uniform->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		uniform->set_default_value(0.5);
		uniform->set_slot_name("Value (Color)");
		uniform->set_value_step(0.01);
		uniform->set_value_range(Vector2(0, 1));
	}

	uniform->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_COLOR);
	register_output_property(uniform);
}

void MMGreyscaleUniform::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_float_universal(uniform);
}

Color MMGreyscaleUniform::_get_value_for(const Vector2 &uv, const int pseed) {
	float f = uniform->get_value(uv);
	return Color(f, f, f, 1);
}

MMGreyscaleUniform::MMGreyscaleUniform() {
}

MMGreyscaleUniform::~MMGreyscaleUniform() {
}

void MMGreyscaleUniform::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_uniform"), &MMGreyscaleUniform::get_uniform);
	ClassDB::bind_method(D_METHOD("set_uniform", "value"), &MMGreyscaleUniform::set_uniform);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "uniform", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMNodeUniversalProperty>"), "set_uniform", "get_uniform");

	ClassDB::bind_method(D_METHOD("_init_properties"), &MMGreyscaleUniform::_init_properties);
	ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &MMGreyscaleUniform::_get_value_for);
}
