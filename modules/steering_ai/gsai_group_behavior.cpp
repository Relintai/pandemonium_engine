
#include "gsai_group_behavior.h"

#include "proximities/gsai_proximity.h"
#include "gsai_steering_agent.h"

Ref<GSAIProximity> GSAIGroupBehavior::get_proximity() {
	return proximity;
}

void GSAIGroupBehavior::set_proximity(const Ref<GSAIProximity> &val) {
	proximity = val;
}

Ref<FuncRef> GSAIGroupBehavior::get_callback() {
	return _callback;
}

void GSAIGroupBehavior::set_callback(const Ref<FuncRef> &val) {
	_callback = val;
}

bool GSAIGroupBehavior::_report_neighbor(Ref<GSAISteeringAgent> neighbor) {
	return false;
}

GSAIGroupBehavior::GSAIGroupBehavior() {
	_callback.instance();
	_callback->set_instance(this);
	_callback->set_function("_report_neighbor");
}

GSAIGroupBehavior::~GSAIGroupBehavior() {
}

void GSAIGroupBehavior::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_proximity"), &GSAIGroupBehavior::get_proximity);
	ClassDB::bind_method(D_METHOD("set_proximity", "value"), &GSAIGroupBehavior::set_proximity);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "proximity", PROPERTY_HINT_RESOURCE_TYPE, "GSAIProximity"), "set_proximity", "get_proximity");

	ClassDB::bind_method(D_METHOD("get_callback"), &GSAIGroupBehavior::get_callback);
	ClassDB::bind_method(D_METHOD("set_callback", "value"), &GSAIGroupBehavior::set_callback);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "_callback", PROPERTY_HINT_RESOURCE_TYPE, "FuncRef"), "set_callback", "get_callback");

	BIND_VMETHOD(MethodInfo(Variant::BOOL, "_report_neighbor", PropertyInfo(Variant::OBJECT, "neighbor", PROPERTY_HINT_RESOURCE_TYPE, "GSAISteeringAgent")));
	ClassDB::bind_method(D_METHOD("_report_neighbor", "neighbor"), &GSAIGroupBehavior::_report_neighbor);
}
