/*************************************************************************/
/*  gsai_seek.cpp                                                        */
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

#include "gsai_seek.h"

#include "../gsai_agent_location.h"
#include "../gsai_steering_agent.h"
#include "../gsai_target_acceleration.h"

Ref<GSAIAgentLocation> GSAISeek::get_target() {
	return target;
}

void GSAISeek::set_target(const Ref<GSAIAgentLocation> &val) {
	target = val;
}

void GSAISeek::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	ERR_FAIL_COND(!agent.is_valid());
	ERR_FAIL_COND(!target.is_valid());

	acceleration->set_linear(((target->get_position() - agent->get_position()).normalized() * agent->get_linear_acceleration_max()));
	acceleration->set_angular(0);
}

GSAISeek::GSAISeek() {
}

GSAISeek::~GSAISeek() {
}

void GSAISeek::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target"), &GSAISeek::get_target);
	ClassDB::bind_method(D_METHOD("set_target", "value"), &GSAISeek::set_target);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target", PROPERTY_HINT_RESOURCE_TYPE, "GSAIAgentLocation"), "set_target", "get_target");
}
