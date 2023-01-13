
#include "gsai_steering_agent.h"

float GSAISteeringAgent::get_zero_linear_speed_threshold() const {
	return zero_linear_speed_threshold;
}

void GSAISteeringAgent::set_zero_linear_speed_threshold(const float val) {
	zero_linear_speed_threshold = val;
}

float GSAISteeringAgent::get_linear_speed_max() const {
	return linear_speed_max;
}

void GSAISteeringAgent::set_linear_speed_max(const float val) {
	linear_speed_max = val;
}

float GSAISteeringAgent::get_linear_acceleration_max() const {
	return linear_acceleration_max;
}

void GSAISteeringAgent::set_linear_acceleration_max(const float val) {
	linear_acceleration_max = val;
}

float GSAISteeringAgent::get_angular_speed_max() const {
	return angular_speed_max;
}

void GSAISteeringAgent::set_angular_speed_max(const float val) {
	angular_speed_max = val;
}

float GSAISteeringAgent::get_angular_acceleration_max() const {
	return angular_acceleration_max;
}

void GSAISteeringAgent::set_angular_acceleration_max(const float val) {
	angular_acceleration_max = val;
}

Vector3 GSAISteeringAgent::get_linear_velocity() {
	return linear_velocity;
}

void GSAISteeringAgent::set_linear_velocity(const Vector3 &val) {
	linear_velocity = val;
}

float GSAISteeringAgent::get_angular_velocity() const {
	return angular_velocity;
}

void GSAISteeringAgent::set_angular_velocity(const float val) {
	angular_velocity = val;
}

float GSAISteeringAgent::get_bounding_radius() const {
	return bounding_radius;
}

void GSAISteeringAgent::set_bounding_radius(const float val) {
	bounding_radius = val;
}

bool GSAISteeringAgent::get_is_tagged() const {
	return is_tagged;
}

void GSAISteeringAgent::set_is_tagged(const bool val) {
	is_tagged = val;
}

GSAISteeringAgent::GSAISteeringAgent() {
	zero_linear_speed_threshold = 0.01;
	linear_speed_max = 0.0;
	linear_acceleration_max = 0.0;
	angular_speed_max = 0.0;
	angular_acceleration_max = 0.0;
	angular_velocity = 0.0;
	bounding_radius = 0.0;
	is_tagged = false;
}

GSAISteeringAgent::~GSAISteeringAgent() {
}

void GSAISteeringAgent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_zero_linear_speed_threshold"), &GSAISteeringAgent::get_zero_linear_speed_threshold);
	ClassDB::bind_method(D_METHOD("set_zero_linear_speed_threshold", "value"), &GSAISteeringAgent::set_zero_linear_speed_threshold);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "zero_linear_speed_threshold"), "set_zero_linear_speed_threshold", "get_zero_linear_speed_threshold");

	ClassDB::bind_method(D_METHOD("get_linear_speed_max"), &GSAISteeringAgent::get_linear_speed_max);
	ClassDB::bind_method(D_METHOD("set_linear_speed_max", "value"), &GSAISteeringAgent::set_linear_speed_max);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "linear_speed_max"), "set_linear_speed_max", "get_linear_speed_max");

	ClassDB::bind_method(D_METHOD("get_linear_acceleration_max"), &GSAISteeringAgent::get_linear_acceleration_max);
	ClassDB::bind_method(D_METHOD("set_linear_acceleration_max", "value"), &GSAISteeringAgent::set_linear_acceleration_max);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "linear_acceleration_max"), "set_linear_acceleration_max", "get_linear_acceleration_max");

	ClassDB::bind_method(D_METHOD("get_angular_speed_max"), &GSAISteeringAgent::get_angular_speed_max);
	ClassDB::bind_method(D_METHOD("set_angular_speed_max", "value"), &GSAISteeringAgent::set_angular_speed_max);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "angular_speed_max"), "set_angular_speed_max", "get_angular_speed_max");

	ClassDB::bind_method(D_METHOD("get_angular_acceleration_max"), &GSAISteeringAgent::get_angular_acceleration_max);
	ClassDB::bind_method(D_METHOD("set_angular_acceleration_max", "value"), &GSAISteeringAgent::set_angular_acceleration_max);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "angular_acceleration_max"), "set_angular_acceleration_max", "get_angular_acceleration_max");

	ClassDB::bind_method(D_METHOD("get_linear_velocity"), &GSAISteeringAgent::get_linear_velocity);
	ClassDB::bind_method(D_METHOD("set_linear_velocity", "value"), &GSAISteeringAgent::set_linear_velocity);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "linear_velocity"), "set_linear_velocity", "get_linear_velocity");

	ClassDB::bind_method(D_METHOD("get_angular_velocity"), &GSAISteeringAgent::get_angular_velocity);
	ClassDB::bind_method(D_METHOD("set_angular_velocity", "value"), &GSAISteeringAgent::set_angular_velocity);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "angular_velocity"), "set_angular_velocity", "get_angular_velocity");

	ClassDB::bind_method(D_METHOD("get_bounding_radius"), &GSAISteeringAgent::get_bounding_radius);
	ClassDB::bind_method(D_METHOD("set_bounding_radius", "value"), &GSAISteeringAgent::set_bounding_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "bounding_radius"), "set_bounding_radius", "get_bounding_radius");

	ClassDB::bind_method(D_METHOD("get_is_tagged"), &GSAISteeringAgent::get_is_tagged);
	ClassDB::bind_method(D_METHOD("set_is_tagged", "value"), &GSAISteeringAgent::set_is_tagged);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_tagged"), "set_is_tagged", "get_is_tagged");
}
