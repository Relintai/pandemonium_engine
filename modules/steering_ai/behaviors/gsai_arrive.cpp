/*************************************************************************/
/*  gsai_arrive.cpp                                                      */
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

#include "gsai_arrive.h"

#include "../gsai_steering_agent.h"
#include "../gsai_target_acceleration.h"
#include "../gsai_utils.h"

Ref<GSAIAgentLocation> GSAIArrive::get_target() {
	return target;
}

void GSAIArrive::set_target(const Ref<GSAIAgentLocation> &val) {
	target = val;
}

float GSAIArrive::get_arrival_tolerance() const {
	return arrival_tolerance;
}

void GSAIArrive::set_arrival_tolerance(const float val) {
	arrival_tolerance = val;
}

float GSAIArrive::get_deceleration_radius() const {
	return deceleration_radius;
}

void GSAIArrive::set_deceleration_radius(const float val) {
	deceleration_radius = val;
}

float GSAIArrive::get_time_to_reach() const {
	return time_to_reach;
}

void GSAIArrive::set_time_to_reach(const float val) {
	time_to_reach = val;
}

void GSAIArrive::arrive(const Ref<GSAITargetAcceleration> &acceleration, const Vector3 &target_position) {
	call("_arrive", acceleration, target_position);
}

void GSAIArrive::_arrive(Ref<GSAITargetAcceleration> acceleration, Vector3 target_position) {
	ERR_FAIL_COND(!agent.is_valid());

	Vector3 to_target = target_position - agent->get_position();
	float distance = to_target.length();

	if (distance <= arrival_tolerance) {
		acceleration->set_zero();
	} else {
		float desired_speed = agent->get_linear_speed_max();

		if (distance <= deceleration_radius) {
			desired_speed *= distance / deceleration_radius;
		}

		Vector3 desired_velocity = to_target * desired_speed / distance;
		desired_velocity = ((desired_velocity - agent->get_linear_velocity()) * 1.0 / time_to_reach);
		acceleration->set_linear(GSAIUtils::clampedv3(desired_velocity, agent->get_linear_acceleration_max()));
		acceleration->set_angular(0);
	}
}

void GSAIArrive::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	ERR_FAIL_COND(!target.is_valid());

	arrive(acceleration, target->get_position());
}

GSAIArrive::GSAIArrive() {
	arrival_tolerance = 0.0;
	deceleration_radius = 0.0;
	time_to_reach = 0.1;
}

GSAIArrive::~GSAIArrive() {
}

void GSAIArrive::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target"), &GSAIArrive::get_target);
	ClassDB::bind_method(D_METHOD("set_target", "value"), &GSAIArrive::set_target);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target", PROPERTY_HINT_RESOURCE_TYPE, "GSAIAgentLocation"), "set_target", "get_target");

	ClassDB::bind_method(D_METHOD("get_arrival_tolerance"), &GSAIArrive::get_arrival_tolerance);
	ClassDB::bind_method(D_METHOD("set_arrival_tolerance", "value"), &GSAIArrive::set_arrival_tolerance);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "arrival_tolerance"), "set_arrival_tolerance", "get_arrival_tolerance");

	ClassDB::bind_method(D_METHOD("get_deceleration_radius"), &GSAIArrive::get_deceleration_radius);
	ClassDB::bind_method(D_METHOD("set_deceleration_radius", "value"), &GSAIArrive::set_deceleration_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "deceleration_radius"), "set_deceleration_radius", "get_deceleration_radius");

	ClassDB::bind_method(D_METHOD("get_time_to_reach"), &GSAIArrive::get_time_to_reach);
	ClassDB::bind_method(D_METHOD("set_time_to_reach", "value"), &GSAIArrive::set_time_to_reach);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "time_to_reach"), "set_time_to_reach", "get_time_to_reach");

	BIND_VMETHOD(MethodInfo("_arrive", PropertyInfo(Variant::OBJECT, "acceleration", PROPERTY_HINT_RESOURCE_TYPE, "GSAITargetAcceleration"), PropertyInfo(Variant::VECTOR3, "target_position")));
	ClassDB::bind_method(D_METHOD("arrive", "acceleration", "target_position"), &GSAIArrive::arrive);
	ClassDB::bind_method(D_METHOD("_arrive", "acceleration", "target_position"), &GSAIArrive::_arrive);
}
