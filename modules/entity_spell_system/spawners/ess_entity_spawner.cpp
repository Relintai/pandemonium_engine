

#include "ess_entity_spawner.h"

#include "../singletons/ess.h"

#include "../utility/entity_create_info.h"

_FORCE_INLINE_ void ESSEntitySpawner::request_entity_spawn(Ref<EntityCreateInfo> info) {
	if (has_method("_request_entity_spawn"))
		call("_request_entity_spawn", info);

	emit_signal("on_entity_spawn", info);
}
_FORCE_INLINE_ void ESSEntitySpawner::request_entity_spawn_deferred(Ref<EntityCreateInfo> info) {
	call_deferred("request_entity_spawn", info);
}

ESSEntitySpawner::ESSEntitySpawner() {
	if (ESS::get_singleton()) {
		ESS::get_singleton()->set_entity_spawner(this);
	}
}

ESSEntitySpawner::~ESSEntitySpawner() {
	if (ESS::get_singleton() && ESS::get_singleton()->get_entity_spawner() == this) {
		ESS::get_singleton()->set_entity_spawner(NULL);
	}
}

void ESSEntitySpawner::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_request_entity_spawn", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "EntityCreateInfo")));
	ADD_SIGNAL(MethodInfo("on_entity_spawn", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "EntityCreateInfo")));

	ClassDB::bind_method(D_METHOD("request_entity_spawn", "info"), &ESSEntitySpawner::request_entity_spawn);
	ClassDB::bind_method(D_METHOD("request_entity_spawn_deferred", "info"), &ESSEntitySpawner::request_entity_spawn_deferred);
}
