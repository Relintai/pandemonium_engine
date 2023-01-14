
#include "gsai_look_where_you_go.h"

#include "../gsai_steering_agent.h"
#include "../gsai_target_acceleration.h"
#include "../gsai_utils.h"

void GSAILookWhereYouGo::_calculate_steering(Ref<GSAITargetAcceleration> accel) {
	ERR_FAIL_COND(!agent.is_valid());

	if (agent->get_linear_velocity().length_squared() < agent->get_zero_linear_speed_threshold()) {
		accel->set_zero();
	} else {
		float orientation;

		if (use_z) {
			orientation = GSAIUtils::vector3_to_angle(agent->get_linear_velocity());
		} else {
			orientation = GSAIUtils::vector2_to_angle(GSAIUtils::to_vector2(agent->get_linear_velocity()));
		}

		match_orientation(accel, orientation);
	}
}

GSAILookWhereYouGo::GSAILookWhereYouGo() {
}

GSAILookWhereYouGo::~GSAILookWhereYouGo() {
}

void GSAILookWhereYouGo::_bind_methods() {
}
