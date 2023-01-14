
#include "gsai_priority.h"

#include "../gsai_target_acceleration.h"

float GSAIPriority::get_zero_threshold() const {
	return zero_threshold;
}

void GSAIPriority::set_zero_threshold(const float val) {
	zero_threshold = val;
}

void GSAIPriority::add_behavior(const Ref<GSAISteeringBehavior> &behavior) {
	_behaviors.push_back(behavior);
}

Ref<GSAISteeringBehavior> GSAIPriority::get_behavior(const int index) {
	ERR_FAIL_INDEX_V(index, _behaviors.size(), Ref<GSAISteeringBehavior>());

	return _behaviors[index];
}

void GSAIPriority::remove_behavior(const int index) {
	ERR_FAIL_INDEX(index, _behaviors.size());

	_behaviors.remove(index);
}

int GSAIPriority::get_behaviour_count() {
	return _behaviors.size();
}

void GSAIPriority::_calculate_steering(Ref<GSAITargetAcceleration> accel) {
	float threshold_squared = zero_threshold * zero_threshold;
	_last_selected_index = -1;
	int size = _behaviors.size();

	if (size > 0) {
		for (int i = 0; i < size; ++i) {
			Ref<GSAISteeringBehavior> behavior = _behaviors[i];

			ERR_CONTINUE(!behavior.is_valid());

			_last_selected_index = i;

			behavior->calculate_steering(accel);

			if (accel->get_magnitude_squared() > threshold_squared) {
				break;
			}
		}
	} else {
		accel->set_zero();
	}
}

GSAIPriority::GSAIPriority() {
	zero_threshold = 0.0;
	_last_selected_index = -1;
}

GSAIPriority::~GSAIPriority() {
}

void GSAIPriority::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_zero_threshold"), &GSAIPriority::get_zero_threshold);
	ClassDB::bind_method(D_METHOD("set_zero_threshold", "value"), &GSAIPriority::set_zero_threshold);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "zero_threshold"), "set_zero_threshold", "get_zero_threshold");

	ClassDB::bind_method(D_METHOD("add_behavior", "behavior"), &GSAIPriority::add_behavior);
	ClassDB::bind_method(D_METHOD("get_behavior", "index"), &GSAIPriority::get_behavior);
	ClassDB::bind_method(D_METHOD("remove_behavior", "index"), &GSAIPriority::remove_behavior);
	ClassDB::bind_method(D_METHOD("get_behaviour_count"), &GSAIPriority::get_behaviour_count);
}
