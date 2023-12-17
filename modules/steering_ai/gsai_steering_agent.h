#ifndef GSAI_STEERING_AGENT_H
#define GSAI_STEERING_AGENT_H

/*************************************************************************/
/*  gsai_steering_agent.h                                                */
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

#include "core/int_types.h"
#include "core/math/vector3.h"

#include "core/object/reference.h"

#include "gsai_agent_location.h"

class GSAISteeringAgent : public GSAIAgentLocation {
	GDCLASS(GSAISteeringAgent, GSAIAgentLocation);

public:
	float get_zero_linear_speed_threshold() const;
	void set_zero_linear_speed_threshold(const float val);

	float get_linear_speed_max() const;
	void set_linear_speed_max(const float val);

	float get_linear_acceleration_max() const;
	void set_linear_acceleration_max(const float val);

	float get_angular_speed_max() const;
	void set_angular_speed_max(const float val);

	float get_angular_acceleration_max() const;
	void set_angular_acceleration_max(const float val);

	Vector3 get_linear_velocity();
	void set_linear_velocity(const Vector3 &val);

	float get_angular_velocity() const;
	void set_angular_velocity(const float val);

	float get_bounding_radius() const;
	void set_bounding_radius(const float val);

	bool get_is_tagged() const;
	void set_is_tagged(const bool val);

	GSAISteeringAgent();
	~GSAISteeringAgent();

protected:
	static void _bind_methods();

	float zero_linear_speed_threshold;
	float linear_speed_max;
	float linear_acceleration_max;
	float angular_speed_max;
	float angular_acceleration_max;
	Vector3 linear_velocity;
	float angular_velocity;
	float bounding_radius;
	bool is_tagged;
};

#endif
