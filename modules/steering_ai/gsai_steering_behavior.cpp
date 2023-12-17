/*************************************************************************/
/*  gsai_steering_behavior.cpp                                           */
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

#include "gsai_steering_behavior.h"

#include "gsai_steering_agent.h"
#include "gsai_target_acceleration.h"

bool GSAISteeringBehavior::get_is_enabled() const {
	return is_enabled;
}

void GSAISteeringBehavior::set_is_enabled(const bool val) {
	is_enabled = val;
}

Ref<GSAISteeringAgent> GSAISteeringBehavior::get_agent() {
	return agent;
}

void GSAISteeringBehavior::set_agent(const Ref<GSAISteeringAgent> &val) {
	agent = val;
}

void GSAISteeringBehavior::calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	if (is_enabled) {
		call("_calculate_steering", acceleration);
	} else {
		acceleration->set_zero();
	}
}

void GSAISteeringBehavior::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	acceleration->set_zero();
}

GSAISteeringBehavior::GSAISteeringBehavior() {
	is_enabled = true;
}

GSAISteeringBehavior::~GSAISteeringBehavior() {
}

void GSAISteeringBehavior::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_enabled"), &GSAISteeringBehavior::get_is_enabled);
	ClassDB::bind_method(D_METHOD("set_is_enabled", "value"), &GSAISteeringBehavior::set_is_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_enabled"), "set_is_enabled", "get_is_enabled");

	ClassDB::bind_method(D_METHOD("get_agent"), &GSAISteeringBehavior::get_agent);
	ClassDB::bind_method(D_METHOD("set_agent", "value"), &GSAISteeringBehavior::set_agent);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "agent", PROPERTY_HINT_RESOURCE_TYPE, "GSAISteeringAgent"), "set_agent", "get_agent");

	BIND_VMETHOD(MethodInfo("_calculate_steering", PropertyInfo(Variant::OBJECT, "acceleration", PROPERTY_HINT_RESOURCE_TYPE, "GSAITargetAcceleration")));

	ClassDB::bind_method(D_METHOD("calculate_steering", "acceleration"), &GSAISteeringBehavior::calculate_steering);
	ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAISteeringBehavior::_calculate_steering);
}
