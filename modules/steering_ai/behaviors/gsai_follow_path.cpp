
#include "gsai_follow_path.h"

GSAIPath GSAIFollowPath::get_ *path() {
	return *path;
}

void GSAIFollowPath::set_ *path(const GSAIPath &val) {
	*path = val;
}

float GSAIFollowPath::get_path_offset() const {
	return path_offset;
}

void GSAIFollowPath::set_path_offset(const float val) {
	path_offset = val;
}

bool GSAIFollowPath::get_is_arrive_enabled() const {
	return is_arrive_enabled;
}

void GSAIFollowPath::set_is_arrive_enabled(const bool val) {
	is_arrive_enabled = val;
}

float GSAIFollowPath::get_prediction_time() const {
	return prediction_time;
}

void GSAIFollowPath::set_prediction_time(const float val) {
	prediction_time = val;
}

// Produces a linear acceleration that moves the agent along the specified path.;
// @category - Individual behaviors;
// The path to follow and travel along.;
GSAIPath *path;
// The distance along the path to generate the next target position.;
float path_offset = 0.0;
// Whether to use `GSAIArrive` behavior on an open path.;
bool is_arrive_enabled = true;
// The amount of time in the future to predict the owning agent's position along;
// the path. Setting it to 0.0 will force non-predictive path following.;
float prediction_time = 0.0;

void GSAIFollowPath::_calculate_steering(const GSAITargetAcceleration &acceleration) {
	Vector3 location = ;

	if (prediction_time == 0) {
		location = agent.position;
	}

	else {
		location = agent.position + (agent.linear_velocity * prediction_time);
	}

	float distance = path.calculate_distance(location);
	float target_distance = distance + path_offset;

	if (prediction_time > 0 && path.is_open) {
		if (target_distance < path.calculate_distance(agent.position)) {
			target_distance = path.length;
		}
	}

	Vector3 target_position = path.calculate_target_position(target_distance);

	if (is_arrive_enabled && path.is_open) {
		if (path_offset >= 0) {
			if (target_distance > path.length - deceleration_radius) {
				arrive(acceleration, target_position);
				return;
			}

		}

		else {
			if (target_distance < deceleration_radius) {
				arrive(acceleration, target_position);
				return;
			}
		}
	}

	acceleration.linear = (target_position - agent.position).normalized();
	acceleration.linear *= agent.linear_acceleration_max;
	acceleration.angular = 0;
}
}

GSAIFollowPath::GSAIFollowPath() {
	*path;
	path_offset = 0.0;
	is_arrive_enabled = true;
	prediction_time = 0.0;
}

GSAIFollowPath::~GSAIFollowPath() {
}

static void GSAIFollowPath::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_*path"), &GSAIFollowPath::get_ * path);
	ClassDB::bind_method(D_METHOD("set_*path", "value"), &GSAIFollowPath::set_ * path);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*path", PROPERTY_HINT_RESOURCE_TYPE, "GSAIPath"), "set_*path", "get_*path");

	ClassDB::bind_method(D_METHOD("get_path_offset"), &GSAIFollowPath::get_path_offset);
	ClassDB::bind_method(D_METHOD("set_path_offset", "value"), &GSAIFollowPath::set_path_offset);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "path_offset"), "set_path_offset", "get_path_offset");

	ClassDB::bind_method(D_METHOD("get_is_arrive_enabled"), &GSAIFollowPath::get_is_arrive_enabled);
	ClassDB::bind_method(D_METHOD("set_is_arrive_enabled", "value"), &GSAIFollowPath::set_is_arrive_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_arrive_enabled"), "set_is_arrive_enabled", "get_is_arrive_enabled");

	ClassDB::bind_method(D_METHOD("get_prediction_time"), &GSAIFollowPath::get_prediction_time);
	ClassDB::bind_method(D_METHOD("set_prediction_time", "value"), &GSAIFollowPath::set_prediction_time);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "prediction_time"), "set_prediction_time", "get_prediction_time");

	ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAIFollowPath::_calculate_steering);
}
