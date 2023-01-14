
#include "gsai_match_orientation.h"

#include "core/math/math_funcs.h"

#include "../gsai_agent_location.h"
#include "../gsai_steering_agent.h"
#include "../gsai_target_acceleration.h"

Ref<GSAIAgentLocation> GSAIMatchOrientation::get_target() {
	return target;
}

void GSAIMatchOrientation::set_target(const Ref<GSAIAgentLocation> &val) {
	target = val;
}

float GSAIMatchOrientation::get_alignment_tolerance() const {
	return alignment_tolerance;
}

void GSAIMatchOrientation::set_alignment_tolerance(const float val) {
	alignment_tolerance = val;
}

float GSAIMatchOrientation::get_deceleration_radius() const {
	return deceleration_radius;
}

void GSAIMatchOrientation::set_deceleration_radius(const float val) {
	deceleration_radius = val;
}

float GSAIMatchOrientation::get_time_to_reach() const {
	return time_to_reach;
}

void GSAIMatchOrientation::set_time_to_reach(const float val) {
	time_to_reach = val;
}

bool GSAIMatchOrientation::get_use_z() const {
	return use_z;
}

void GSAIMatchOrientation::set_use_z(const bool val) {
	use_z = val;
}

void GSAIMatchOrientation::match_orientation(const Ref<GSAITargetAcceleration> &acceleration, const float desired_orientation) {
	call("_match_orientation", acceleration, desired_orientation);
}

void GSAIMatchOrientation::_match_orientation(Ref<GSAITargetAcceleration> acceleration, float desired_orientation) {
	ERR_FAIL_COND(!agent.is_valid());

	float rotation = Math::wrapf(static_cast<double>(desired_orientation - agent->get_orientation()), -Math_PI, Math_PI);
	float rotation_size = abs(rotation);

	if (rotation_size <= alignment_tolerance) {
		acceleration->set_zero();
	} else {
		float desired_rotation = agent->get_angular_speed_max();

		if (rotation_size <= deceleration_radius) {
			desired_rotation *= rotation_size / deceleration_radius;
		}

		desired_rotation *= rotation / rotation_size;
		float angular = ((desired_rotation - agent->get_angular_velocity()) / time_to_reach);
		float limited_acceleration = abs(angular);

		float angular_acceleration_max = agent->get_angular_acceleration_max();

		if (limited_acceleration > angular_acceleration_max) {
			angular *= (angular_acceleration_max / limited_acceleration);
		}

		acceleration->set_angular(angular);
	}

	acceleration->set_linear(Vector3());
}

void GSAIMatchOrientation::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	ERR_FAIL_COND(!target.is_valid());

	match_orientation(acceleration, target->get_orientation());
}

GSAIMatchOrientation::GSAIMatchOrientation() {
	alignment_tolerance = 0.0;
	deceleration_radius = 0.0;
	time_to_reach = 0.1;
	use_z = false;
}

GSAIMatchOrientation::~GSAIMatchOrientation() {
}

void GSAIMatchOrientation::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target"), &GSAIMatchOrientation::get_target);
	ClassDB::bind_method(D_METHOD("set_target", "value"), &GSAIMatchOrientation::set_target);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target", PROPERTY_HINT_RESOURCE_TYPE, "GSAIAgentLocation"), "set_target", "get_target");

	ClassDB::bind_method(D_METHOD("get_alignment_tolerance"), &GSAIMatchOrientation::get_alignment_tolerance);
	ClassDB::bind_method(D_METHOD("set_alignment_tolerance", "value"), &GSAIMatchOrientation::set_alignment_tolerance);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "alignment_tolerance"), "set_alignment_tolerance", "get_alignment_tolerance");

	ClassDB::bind_method(D_METHOD("get_deceleration_radius"), &GSAIMatchOrientation::get_deceleration_radius);
	ClassDB::bind_method(D_METHOD("set_deceleration_radius", "value"), &GSAIMatchOrientation::set_deceleration_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "deceleration_radius"), "set_deceleration_radius", "get_deceleration_radius");

	ClassDB::bind_method(D_METHOD("get_time_to_reach"), &GSAIMatchOrientation::get_time_to_reach);
	ClassDB::bind_method(D_METHOD("set_time_to_reach", "value"), &GSAIMatchOrientation::set_time_to_reach);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "time_to_reach"), "set_time_to_reach", "get_time_to_reach");

	ClassDB::bind_method(D_METHOD("get_use_z"), &GSAIMatchOrientation::get_use_z);
	ClassDB::bind_method(D_METHOD("set_use_z", "value"), &GSAIMatchOrientation::set_use_z);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_z"), "set_use_z", "get_use_z");

	BIND_VMETHOD(MethodInfo("_match_orientation", PropertyInfo(Variant::OBJECT, "acceleration", PROPERTY_HINT_RESOURCE_TYPE, "GSAITargetAcceleration"), PropertyInfo(Variant::REAL, "desired_orientation")));
	ClassDB::bind_method(D_METHOD("match_orientation", "acceleration", "desired_orientation"), &GSAIMatchOrientation::match_orientation);
	ClassDB::bind_method(D_METHOD("_match_orientation", "acceleration", "desired_orientation"), &GSAIMatchOrientation::_match_orientation);
}
