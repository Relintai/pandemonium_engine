
#include "gsai_face.h"

#include "../gsai_steering_agent.h"

#include "../gsai_target_acceleration.h"
#include "../gsai_utils.h"

void GSAIFace::face(const Ref<GSAITargetAcceleration> &acceleration, const Vector3 &target_position) {
	call("_face", acceleration, target_position);
}

void GSAIFace::_face(Ref<GSAITargetAcceleration> acceleration, Vector3 target_position) {
	ERR_FAIL_COND(!agent.is_valid());

	Vector3 to_target = target_position - agent->get_position();
	float distance_squared = to_target.length_squared();

	if (distance_squared < agent->get_zero_linear_speed_threshold()) {
		acceleration->set_zero();
	} else {
		float orientation;

		if (use_z) {
			orientation = GSAIUtils::vector3_to_angle(to_target);
		} else {
			orientation = GSAIUtils::vector2_to_angle(GSAIUtils::to_vector2(to_target));
		}

		match_orientation(acceleration, orientation);
	}
}

void GSAIFace::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	ERR_FAIL_COND(!target.is_valid());

	face(acceleration, target->get_position());
}

GSAIFace::GSAIFace() {
}

GSAIFace::~GSAIFace() {
}

void GSAIFace::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_face", PropertyInfo(Variant::OBJECT, "acceleration", PROPERTY_HINT_RESOURCE_TYPE, "GSAITargetAcceleration"), PropertyInfo(Variant::VECTOR3, "target_position")));
	ClassDB::bind_method(D_METHOD("face", "acceleration", "target_position"), &GSAIFace::face);
	ClassDB::bind_method(D_METHOD("_face", "acceleration", "target_position"), &GSAIFace::_face);
}
