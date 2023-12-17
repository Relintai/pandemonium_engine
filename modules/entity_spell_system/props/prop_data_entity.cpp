

#include "prop_data_entity.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_PROPS_ENABLED

Ref<EntityData> PropDataEntity::get_entity_data() const {
	return _entity_data;
}
void PropDataEntity::set_entity_data(const Ref<EntityData> &value) {
	_entity_data = value;
}

int PropDataEntity::get_level() const {
	return _level;
}
void PropDataEntity::set_level(const int value) {
	_level = value;
}

PropDataEntity::PropDataEntity() {
	_level = 1;
}
PropDataEntity::~PropDataEntity() {
	_entity_data.unref();
}

void PropDataEntity::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_entity_data"), &PropDataEntity::get_entity_data);
	ClassDB::bind_method(D_METHOD("set_entity_data", "value"), &PropDataEntity::set_entity_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "entity_data", PROPERTY_HINT_RESOURCE_TYPE, "EntityData"), "set_entity_data", "get_entity_data");

	ClassDB::bind_method(D_METHOD("get_level"), &PropDataEntity::get_level);
	ClassDB::bind_method(D_METHOD("set_level", "value"), &PropDataEntity::set_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "level"), "set_level", "get_level");
}

#endif
