/*************************************************************************/
/*  gsai_look_where_you_go.cpp                                           */
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
