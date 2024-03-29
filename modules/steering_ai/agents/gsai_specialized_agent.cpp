/*************************************************************************/
/*  gsai_specialized_agent.cpp                                           */
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

#include "gsai_specialized_agent.h"

#include "../gsai_target_acceleration.h"

bool GSAISpecializedAgent::get_calculate_velocities() const {
	return calculate_velocities;
}

void GSAISpecializedAgent::set_calculate_velocities(const bool val) {
	calculate_velocities = val;
}

bool GSAISpecializedAgent::get_apply_linear_drag() const {
	return apply_linear_drag;
}

void GSAISpecializedAgent::set_apply_linear_drag(const bool val) {
	apply_linear_drag = val;
}

bool GSAISpecializedAgent::get_apply_angular_drag() const {
	return apply_angular_drag;
}

void GSAISpecializedAgent::set_apply_angular_drag(const bool val) {
	apply_angular_drag = val;
}

float GSAISpecializedAgent::get_linear_drag_percentage() const {
	return linear_drag_percentage;
}

void GSAISpecializedAgent::set_linear_drag_percentage(const float val) {
	linear_drag_percentage = val;
}

float GSAISpecializedAgent::get_angular_drag_percentage() const {
	return angular_drag_percentage;
}

void GSAISpecializedAgent::set_angular_drag_percentage(const float val) {
	angular_drag_percentage = val;
}

float GSAISpecializedAgent::get_last_orientation() const {
	return last_orientation;
}

void GSAISpecializedAgent::set_last_orientation(const float val) {
	last_orientation = val;
}

bool GSAISpecializedAgent::get_applied_steering() const {
	return applied_steering;
}

void GSAISpecializedAgent::set_applied_steering(const bool val) {
	applied_steering = val;
}

void GSAISpecializedAgent::apply_steering(const Ref<GSAITargetAcceleration> &acceleration, const float delta) {
	call("_apply_steering", acceleration, delta);
}

void GSAISpecializedAgent::_apply_steering(Ref<GSAITargetAcceleration> acceleration, float delta) {
}

GSAISpecializedAgent::GSAISpecializedAgent() {
	calculate_velocities = true;
	apply_linear_drag = true;
	apply_angular_drag = true;
	linear_drag_percentage = 0.0;
	angular_drag_percentage = 0.0;
	last_orientation = 0.0;
	applied_steering = false;
}

GSAISpecializedAgent::~GSAISpecializedAgent() {
}

void GSAISpecializedAgent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_calculate_velocities"), &GSAISpecializedAgent::get_calculate_velocities);
	ClassDB::bind_method(D_METHOD("set_calculate_velocities", "value"), &GSAISpecializedAgent::set_calculate_velocities);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "calculate_velocities"), "set_calculate_velocities", "get_calculate_velocities");

	ClassDB::bind_method(D_METHOD("get_apply_linear_drag"), &GSAISpecializedAgent::get_apply_linear_drag);
	ClassDB::bind_method(D_METHOD("set_apply_linear_drag", "value"), &GSAISpecializedAgent::set_apply_linear_drag);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "apply_linear_drag"), "set_apply_linear_drag", "get_apply_linear_drag");

	ClassDB::bind_method(D_METHOD("get_apply_angular_drag"), &GSAISpecializedAgent::get_apply_angular_drag);
	ClassDB::bind_method(D_METHOD("set_apply_angular_drag", "value"), &GSAISpecializedAgent::set_apply_angular_drag);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "apply_angular_drag"), "set_apply_angular_drag", "get_apply_angular_drag");

	ClassDB::bind_method(D_METHOD("get_linear_drag_percentage"), &GSAISpecializedAgent::get_linear_drag_percentage);
	ClassDB::bind_method(D_METHOD("set_linear_drag_percentage", "value"), &GSAISpecializedAgent::set_linear_drag_percentage);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "linear_drag_percentage"), "set_linear_drag_percentage", "get_linear_drag_percentage");

	ClassDB::bind_method(D_METHOD("get_angular_drag_percentage"), &GSAISpecializedAgent::get_angular_drag_percentage);
	ClassDB::bind_method(D_METHOD("set_angular_drag_percentage", "value"), &GSAISpecializedAgent::set_angular_drag_percentage);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "angular_drag_percentage"), "set_angular_drag_percentage", "get_angular_drag_percentage");

	ClassDB::bind_method(D_METHOD("get_last_orientation"), &GSAISpecializedAgent::get_last_orientation);
	ClassDB::bind_method(D_METHOD("set_last_orientation", "value"), &GSAISpecializedAgent::set_last_orientation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "last_orientation"), "set_last_orientation", "get_last_orientation");

	ClassDB::bind_method(D_METHOD("get_applied_steering"), &GSAISpecializedAgent::get_applied_steering);
	ClassDB::bind_method(D_METHOD("set_applied_steering", "value"), &GSAISpecializedAgent::set_applied_steering);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "applied_steering"), "set_applied_steering", "get_applied_steering");

	BIND_VMETHOD(MethodInfo("_apply_steering", PropertyInfo(Variant::OBJECT, "acceleration", PROPERTY_HINT_RESOURCE_TYPE, "GSAITargetAcceleration"), PropertyInfo(Variant::REAL, "delta")));
	ClassDB::bind_method(D_METHOD("apply_steering", "acceleration", "delta"), &GSAISpecializedAgent::apply_steering);
	ClassDB::bind_method(D_METHOD("_apply_steering", "acceleration", "delta"), &GSAISpecializedAgent::_apply_steering);
}
