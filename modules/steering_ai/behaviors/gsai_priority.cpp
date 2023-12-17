/*************************************************************************/
/*  gsai_priority.cpp                                                    */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

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
