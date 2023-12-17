

#include "entity_resource_cost_data_resource.h"

Ref<EntityResource> EntityResourceCostDataResource::get_entity_resource_data() {
	return _entity_resource_data;
}
void EntityResourceCostDataResource::set_entity_resource_data(Ref<EntityResource> data) {
	_entity_resource_data = data;
}

EntityResourceCostDataResource::EntityResourceCostDataResource() {
}

void EntityResourceCostDataResource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_entity_resource_data"), &EntityResourceCostDataResource::get_entity_resource_data);
	ClassDB::bind_method(D_METHOD("set_entity_resource_data", "value"), &EntityResourceCostDataResource::set_entity_resource_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "entity_resource_data", PROPERTY_HINT_RESOURCE_TYPE, "EntityResource"), "set_entity_resource_data", "get_entity_resource_data");
}
