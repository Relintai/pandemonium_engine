
#include "gsai_evade.h"

#include "../gsai_steering_agent.h"

float GSAIEvade::_get_modified_acceleration() {
	ERR_FAIL_COND_V(!agent.is_valid(), 0);

	return -(agent->get_linear_acceleration_max());
}

GSAIEvade::GSAIEvade() {
}

GSAIEvade::~GSAIEvade() {
}

void GSAIEvade::_bind_methods() {
}
