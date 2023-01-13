
#include "gsai_evade.h"

// Calculates acceleration to take an agent away from where a target agent is;
// moving.;
// @category - Individual behaviors;

float GSAIEvade::_get_modified_acceleration() {
	return -agent.linear_acceleration_max;
}
}

GSAIEvade::GSAIEvade() {
}

GSAIEvade::~GSAIEvade() {
}

static void GSAIEvade::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_get_modified_acceleration"), &GSAIEvade::_get_modified_acceleration);
}
