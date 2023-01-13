
#include "gsai_seek.h"

GSAIAgentLocation GSAISeek::get_ *target() {
	return *target;
}

void GSAISeek::set_ *target(const GSAIAgentLocation &val) {
	*target = val;
}

// Calculates an acceleration to take an agent to a target agent's position;
// directly.;
// @category - Individual behaviors;
// The target that the behavior aims to move the agent to.;
GSAIAgentLocation *target;

void GSAISeek::_calculate_steering(const GSAITargetAcceleration &acceleration) {
	acceleration.linear = ((target.position - agent.position).normalized() * agent.linear_acceleration_max);
	acceleration.angular = 0;
}
}

GSAISeek::GSAISeek() {
	*target;
}

GSAISeek::~GSAISeek() {
}

static void GSAISeek::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_*target"), &GSAISeek::get_ * target);
	ClassDB::bind_method(D_METHOD("set_*target", "value"), &GSAISeek::set_ * target);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*target", PROPERTY_HINT_RESOURCE_TYPE, "GSAIAgentLocation"), "set_*target", "get_*target");

	ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAISeek::_calculate_steering);
}
