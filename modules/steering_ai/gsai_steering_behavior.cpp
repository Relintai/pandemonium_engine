
#include "gsai_steering_behavior.h"

#include "gsai_target_acceleration.h"
#include "gsai_steering_agent.h"

bool GSAISteeringBehavior::get_is_enabled() const {
	return is_enabled;
}

void GSAISteeringBehavior::set_is_enabled(const bool val) {
	is_enabled = val;
}

Ref<GSAISteeringAgent> GSAISteeringBehavior::get_agent() {
	return agent;
}

void GSAISteeringBehavior::set_agent(const Ref<GSAISteeringAgent> &val) {
	agent = val;
}

void GSAISteeringBehavior::calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	if (is_enabled) {
		call("_calculate_steering", acceleration);
	} else {
		acceleration->set_zero();
	}
}

void GSAISteeringBehavior::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	acceleration->set_zero();
}

GSAISteeringBehavior::GSAISteeringBehavior() {
	is_enabled = true;
}

GSAISteeringBehavior::~GSAISteeringBehavior() {
}

void GSAISteeringBehavior::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_enabled"), &GSAISteeringBehavior::get_is_enabled);
	ClassDB::bind_method(D_METHOD("set_is_enabled", "value"), &GSAISteeringBehavior::set_is_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_enabled"), "set_is_enabled", "get_is_enabled");

	ClassDB::bind_method(D_METHOD("get_agent"), &GSAISteeringBehavior::get_agent);
	ClassDB::bind_method(D_METHOD("set_agent", "value"), &GSAISteeringBehavior::set_agent);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "agent", PROPERTY_HINT_RESOURCE_TYPE, "GSAISteeringAgent"), "set_agent", "get_agent");

	BIND_VMETHOD(MethodInfo("_calculate_steering", PropertyInfo(Variant::OBJECT, "acceleration", PROPERTY_HINT_RESOURCE_TYPE, "GSAITargetAcceleration")));

	ClassDB::bind_method(D_METHOD("calculate_steering", "acceleration"), &GSAISteeringBehavior::calculate_steering);
	ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAISteeringBehavior::_calculate_steering);
}
