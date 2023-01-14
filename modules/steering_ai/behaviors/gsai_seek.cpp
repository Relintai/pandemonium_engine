
#include "gsai_seek.h"

#include "../gsai_agent_location.h"
#include "../gsai_steering_agent.h"
#include "../gsai_target_acceleration.h"

Ref<GSAIAgentLocation> GSAISeek::get_target() {
	return target;
}

void GSAISeek::set_target(const Ref<GSAIAgentLocation> &val) {
	target = val;
}

void GSAISeek::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	ERR_FAIL_COND(!agent.is_valid());
	ERR_FAIL_COND(!target.is_valid());

	acceleration->set_linear(((target->get_position() - agent->get_position()).normalized() * agent->get_linear_acceleration_max()));
	acceleration->set_angular(0);
}

GSAISeek::GSAISeek() {
}

GSAISeek::~GSAISeek() {
}

void GSAISeek::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target"), &GSAISeek::get_target);
	ClassDB::bind_method(D_METHOD("set_target", "value"), &GSAISeek::set_target);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target", PROPERTY_HINT_RESOURCE_TYPE, "GSAIAgentLocation"), "set_target", "get_target");
}
