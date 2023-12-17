/*************************************************************************/
/*  gsai_cohesion.cpp                                                    */
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

#include "gsai_cohesion.h"

#include "../gsai_steering_agent.h"
#include "../gsai_target_acceleration.h"
#include "../proximities/gsai_proximity.h"

void GSAICohesion::_calculate_steering(Ref<GSAITargetAcceleration> acceleration) {
	ERR_FAIL_COND(!proximity.is_valid());
	ERR_FAIL_COND(!agent.is_valid());

	acceleration->set_zero();
	_center_of_mass = Vector3();
	int neighbor_count = proximity->find_neighbors(_callback);

	if (neighbor_count > 0) {
		_center_of_mass *= 1.0 / neighbor_count;
		acceleration->set_linear((_center_of_mass - agent->get_position()).normalized() * agent->get_linear_acceleration_max());
	}
}

bool GSAICohesion::_report_neighbor(Ref<GSAISteeringAgent> neighbor) {
	_center_of_mass += neighbor->get_position();
	return true;
}

GSAICohesion::GSAICohesion() {
}

GSAICohesion::~GSAICohesion() {
}

void GSAICohesion::_bind_methods() {
}
