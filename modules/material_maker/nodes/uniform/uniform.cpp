
#include "uniform.h"

#include "../../editor/mm_graph_node.h"

Ref<MMNodeUniversalProperty> MMUniform::get_uniform() {
	return uniform;
}

void MMUniform::set_uniform(const Ref<MMNodeUniversalProperty> &val) {
	uniform = val;
}

void MMUniform::_init_properties() {
	if (!uniform.is_valid()) {
		uniform.instance();
		uniform->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_COLOR);
		uniform->set_default_value(Color(1, 1, 1, 1));
	}

	uniform->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_COLOR);
	register_output_property(uniform);
}

void MMUniform::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_color_universal(uniform);
}

Color MMUniform::_get_value_for(const Vector2 &uv, const int pseed) {
	return uniform->get_value(uv);
}

MMUniform::MMUniform() {
}

MMUniform::~MMUniform() {
}

void MMUniform::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_uniform"), &MMUniform::get_uniform);
	ClassDB::bind_method(D_METHOD("set_uniform", "value"), &MMUniform::set_uniform);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "uniform", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_uniform", "get_uniform");

	ClassDB::bind_method(D_METHOD("_init_properties"), &MMUniform::_init_properties);
	//ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &MMUniform::_register_methods);
	ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &MMUniform::_get_value_for);
}
