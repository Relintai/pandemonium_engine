
#include "gsai_separation.h"

#include "../gsai_steering_agent.h"
#include "../gsai_target_acceleration.h"
#include "../proximities/gsai_proximity.h"

float GSAISeparation::get_decay_coefficient() const {
	return decay_coefficient;
}

void GSAISeparation::set_decay_coefficient(const float val) {
	decay_coefficient = val;
}

Ref<GSAITargetAcceleration> GSAISeparation::get_acceleration() {
	return acceleration;
}

void GSAISeparation::set_acceleration(const Ref<GSAITargetAcceleration> &val) {
	acceleration = val;
}

void GSAISeparation::_calculate_steering(Ref<GSAITargetAcceleration> p_acceleration) {
	acceleration = p_acceleration;

	acceleration->set_zero();

	ERR_FAIL_COND(!proximity.is_valid());

	proximity->find_neighbors(_callback);
}

bool GSAISeparation::_report_neighbor(Ref<GSAISteeringAgent> p_neighbor) {
	ERR_FAIL_COND_V(!agent.is_valid(), false);

	Vector3 to_agent = agent->get_position() - p_neighbor->get_position();
	float distance_squared = to_agent.length_squared();
	float acceleration_max = agent->get_linear_acceleration_max();
	float strength = decay_coefficient / distance_squared;

	if (strength > acceleration_max) {
		strength = acceleration_max;
	}

	acceleration->set_linear(acceleration->get_linear() + to_agent * (strength / sqrt(distance_squared)));

	return true;
}

GSAISeparation::GSAISeparation() {
	decay_coefficient = 1.0;
}

GSAISeparation::~GSAISeparation() {
}

void GSAISeparation::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_decay_coefficient"), &GSAISeparation::get_decay_coefficient);
	ClassDB::bind_method(D_METHOD("set_decay_coefficient", "value"), &GSAISeparation::set_decay_coefficient);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "decay_coefficient"), "set_decay_coefficient", "get_decay_coefficient");

	ClassDB::bind_method(D_METHOD("get_acceleration"), &GSAISeparation::get_acceleration);
	ClassDB::bind_method(D_METHOD("set_acceleration", "value"), &GSAISeparation::set_acceleration);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "acceleration", PROPERTY_HINT_RESOURCE_TYPE, "GSAITargetAcceleration"), "set_acceleration", "get_acceleration");
}
