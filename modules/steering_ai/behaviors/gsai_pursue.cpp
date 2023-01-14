
#include "gsai_pursue.h"

#include "core/math/math_funcs.h"

#include "../gsai_steering_agent.h"
#include "../gsai_target_acceleration.h"

Ref<GSAISteeringAgent> GSAIPursue::get_target() {
	return target;
}

void GSAIPursue::set_target(const Ref<GSAISteeringAgent> &val) {
	target = val;
}

float GSAIPursue::get_predict_time_max() const {
	return predict_time_max;
}

void GSAIPursue::set_predict_time_max(const float val) {
	predict_time_max = val;
}

void GSAIPursue::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	ERR_FAIL_COND(!target.is_valid());
	ERR_FAIL_COND(!agent.is_valid());

	Vector3 target_position = target->get_position();
	float distance_squared = (target_position - agent->get_position()).length_squared();
	float speed_squared = agent->get_linear_velocity().length_squared();
	float predict_time = predict_time_max;

	if (speed_squared > 0) {
		float predict_time_squared = distance_squared / speed_squared;

		if (predict_time_squared < predict_time_max * predict_time_max) {
			predict_time = Math::sqrt(predict_time_squared);
		}
	}

	Vector3 linear = ((target_position + (target->get_linear_velocity() * predict_time)) - agent->get_position()).normalized();
	linear *= get_modified_acceleration();

	acceleration->set_linear(linear);
	acceleration->set_angular(0);
}

float GSAIPursue::get_modified_acceleration() {
	return call("_get_modified_acceleration");
}

float GSAIPursue::_get_modified_acceleration() {
	ERR_FAIL_COND_V(!agent.is_valid(), 0);

	return agent->get_linear_acceleration_max();
}

GSAIPursue::GSAIPursue() {
	predict_time_max = 1.0;
}

GSAIPursue::~GSAIPursue() {
}

void GSAIPursue::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target"), &GSAIPursue::get_target);
	ClassDB::bind_method(D_METHOD("set_target", "value"), &GSAIPursue::set_target);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target", PROPERTY_HINT_RESOURCE_TYPE, "GSAISteeringAgent"), "set_target", "get_target");

	ClassDB::bind_method(D_METHOD("get_predict_time_max"), &GSAIPursue::get_predict_time_max);
	ClassDB::bind_method(D_METHOD("set_predict_time_max", "value"), &GSAIPursue::set_predict_time_max);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "predict_time_max"), "set_predict_time_max", "get_predict_time_max");

	BIND_VMETHOD(MethodInfo(Variant::REAL, "_get_modified_acceleration"));
	ClassDB::bind_method(D_METHOD("get_modified_acceleration"), &GSAIPursue::get_modified_acceleration);
	ClassDB::bind_method(D_METHOD("_get_modified_acceleration"), &GSAIPursue::_get_modified_acceleration);
}
