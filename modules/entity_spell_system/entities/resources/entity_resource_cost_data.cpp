

#include "entity_resource_cost_data.h"

int EntityResourceCostData::get_cost() {
	return _cost;
}
void EntityResourceCostData::set_cost(int value) {
	_cost = value;
}

EntityResourceCostData::EntityResourceCostData() {
	_cost = 0;
}

void EntityResourceCostData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_cost"), &EntityResourceCostData::get_cost);
	ClassDB::bind_method(D_METHOD("set_cost", "value"), &EntityResourceCostData::set_cost);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cost"), "set_cost", "get_cost");
}
