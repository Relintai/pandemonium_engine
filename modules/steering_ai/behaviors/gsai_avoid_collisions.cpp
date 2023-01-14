
#include "gsai_avoid_collisions.h"

#include "../gsai_steering_agent.h"
#include "../gsai_target_acceleration.h"
#include "../proximities/gsai_proximity.h"

#include "core/math/math_defs.h"
#include "core/math/math_funcs.h"

void GSAIAvoidCollisions::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	ERR_FAIL_COND(!proximity.is_valid());
	ERR_FAIL_COND(!agent.is_valid());

	_shortest_time = Math_INF;
	_first_neighbor.unref();
	_first_minimum_separation = 0;
	_first_distance = 0;
	int neighbor_count = proximity->find_neighbors(_callback);

	Vector3 linear = acceleration->get_linear();

	if (neighbor_count == 0 || !_first_neighbor.is_valid()) {
		acceleration->set_zero();
		linear = Vector3();
	} else {
		if ((_first_minimum_separation <= 0 || _first_distance < agent->get_bounding_radius() + _first_neighbor->get_bounding_radius())) {
			linear = _first_neighbor->get_position() - agent->get_position();
		} else {
			linear = (_first_relative_position + (_first_relative_velocity * _shortest_time));
		}
	}

	linear = (linear.normalized() * -agent->get_linear_acceleration_max());
	acceleration->set_linear(linear);
	acceleration->set_angular(0);
}

bool GSAIAvoidCollisions::_report_neighbor(Ref<GSAISteeringAgent> neighbor) {
	ERR_FAIL_COND_V(!agent.is_valid(), false);

	Vector3 relative_position = neighbor->get_position() - agent->get_position();
	Vector3 relative_velocity = neighbor->get_linear_velocity() - agent->get_linear_velocity();
	float relative_speed_squared = relative_velocity.length_squared();

	if (relative_speed_squared == 0) {
		return false;
	} else {
		float time_to_collision = -relative_position.dot(relative_velocity) / relative_speed_squared;

		if (time_to_collision <= 0 || time_to_collision >= _shortest_time) {
			return false;
		} else {
			float distance = relative_position.length();
			float minimum_separation = (distance - Math::sqrt(relative_speed_squared) * time_to_collision);

			if (minimum_separation > agent->get_bounding_radius() + neighbor->get_bounding_radius()) {
				return false;
			} else {
				_shortest_time = time_to_collision;
				_first_neighbor = neighbor;
				_first_minimum_separation = minimum_separation;
				_first_distance = distance;
				_first_relative_position = relative_position;
				_first_relative_velocity = relative_velocity;
				return true;
			}
		}
	}
}

GSAIAvoidCollisions::GSAIAvoidCollisions() {
	_shortest_time = 0.0;
	_first_minimum_separation = 0.0;
	_first_distance = 0.0;
}

GSAIAvoidCollisions::~GSAIAvoidCollisions() {
}

void GSAIAvoidCollisions::_bind_methods() {
}
