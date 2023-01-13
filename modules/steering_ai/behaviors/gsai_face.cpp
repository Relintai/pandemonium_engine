
#include "gsai_face.h"

// Calculates angular acceleration to rotate a target to face its target's;
// position. The behavior attemps to arrive with zero remaining angular velocity.;
// @category - Individual behaviors;

void GSAIFace::face(const GSAITargetAcceleration &acceleration, const Vector3 &target_position) {
	call("_face", acceleration, target_position);
}

void GSAIFace::_face(const GSAITargetAcceleration &acceleration, const Vector3 &target_position) {
	Vector3 to_target = target_position - agent.position;
	float distance_squared = to_target.length_squared();

	if (distance_squared < agent.zero_linear_speed_threshold) {
		acceleration.set_zero();
	}

	else {
		float orientation = ;

		if (use_z) {
			orientation = GSAIUtils.vector3_to_angle(to_target);
		}

		else {
			orientation = GSAIUtils.vector2_to_angle(GSAIUtils.to_vector2(to_target));
		}

		match_orientation(acceleration, orientation);
	}
}

void GSAIFace::_calculate_steering(const GSAITargetAcceleration &acceleration) {
	face(acceleration, target.position);
}
}

GSAIFace::GSAIFace() {
}

GSAIFace::~GSAIFace() {
}

static void GSAIFace::_bind_methods() {
	ClassDB::bind_method(D_METHOD("face", "acceleration", "target_position"), &GSAIFace::face);
	ClassDB::bind_method(D_METHOD("_face", "acceleration", "target_position"), &GSAIFace::_face);
	ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAIFace::_calculate_steering);
}
