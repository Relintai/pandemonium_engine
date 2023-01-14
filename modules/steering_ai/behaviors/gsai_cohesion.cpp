
#include "gsai_cohesion.h"

#include "../gsai_steering_agent.h"
#include "../gsai_target_acceleration.h"
#include "../proximities/gsai_proximity.h"

void GSAICohesion::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	ERR_FAIL_COND(!proximity.is_valid());
	ERR_FAIL_COND(!agent.is_valid());

	acceleration->set_zero();
	_center_of_mass = Vector3();
	int neighbor_count = proximity->find_neighbors(_callback);

	if (neighbor_count > 0) {
		_center_of_mass *= 1.0 / neighbor_count;
		acceleration->set_linear((_center_of_mass - agent->get_position()).normalized() * agent->get_linear_acceleration_max());
	}
}

bool GSAICohesion::_report_neighbor(Ref<GSAISteeringAgent> neighbor) {
	_center_of_mass += neighbor->get_position();
	return true;
}

GSAICohesion::GSAICohesion() {
}

GSAICohesion::~GSAICohesion() {
}

void GSAICohesion::_bind_methods() {
}
