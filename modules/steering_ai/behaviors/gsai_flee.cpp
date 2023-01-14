
#include "gsai_flee.h"

#include "../gsai_agent_location.h"
#include "../gsai_steering_agent.h"
#include "../gsai_target_acceleration.h"

void GSAIFlee::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	ERR_FAIL_COND(!agent.is_valid());
	ERR_FAIL_COND(!target.is_valid());

	acceleration->set_linear((agent->get_position() - target->get_position()).normalized() * agent->get_linear_acceleration_max());
	acceleration->set_angular(0);
}

GSAIFlee::GSAIFlee() {
}

GSAIFlee::~GSAIFlee() {
}

void GSAIFlee::_bind_methods() {
}
