#ifndef GSAI_SPECIALIZED_AGENT_H
#define GSAI_SPECIALIZED_AGENT_H

/*************************************************************************/
/*  gsai_specialized_agent.h                                             */
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

#include "core/object/reference.h"

#include "../gsai_steering_agent.h"

class GSAITargetAcceleration;

class GSAISpecializedAgent : public GSAISteeringAgent {
	GDCLASS(GSAISpecializedAgent, GSAISteeringAgent);

public:
	bool get_calculate_velocities() const;
	void set_calculate_velocities(const bool val);

	bool get_apply_linear_drag() const;
	void set_apply_linear_drag(const bool val);

	bool get_apply_angular_drag() const;
	void set_apply_angular_drag(const bool val);

	float get_linear_drag_percentage() const;
	void set_linear_drag_percentage(const float val);

	float get_angular_drag_percentage() const;
	void set_angular_drag_percentage(const float val);

	float get_last_orientation() const;
	void set_last_orientation(const float val);

	bool get_applied_steering() const;
	void set_applied_steering(const bool val);

	void apply_steering(const Ref<GSAITargetAcceleration> &acceleration, const float delta);
	virtual void _apply_steering(Ref<GSAITargetAcceleration> acceleration, float delta);

	GSAISpecializedAgent();
	~GSAISpecializedAgent();

protected:
	static void _bind_methods();

	bool calculate_velocities;
	bool apply_linear_drag;
	bool apply_angular_drag;
	float linear_drag_percentage;
	float angular_drag_percentage;
	float last_orientation;
	bool applied_steering;
};

#endif
