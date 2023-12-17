#ifndef GSAI_PRIORITY_H
#define GSAI_PRIORITY_H

/*************************************************************************/
/*  gsai_priority.h                                                      */
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

#include "../gsai_steering_behavior.h"

class GSAISteeringBehavior;
class GSAITargetAcceleration;

class GSAIPriority : public GSAISteeringBehavior {
	GDCLASS(GSAIPriority, GSAISteeringBehavior);

public:
	float get_zero_threshold() const;
	void set_zero_threshold(const float val);

	void add_behavior(const Ref<GSAISteeringBehavior> &behavior);
	Ref<GSAISteeringBehavior> get_behavior(const int index);
	void remove_behavior(const int index);
	int get_behaviour_count();

	void _calculate_steering(Ref<GSAITargetAcceleration> accel);

	GSAIPriority();
	~GSAIPriority();

protected:
	static void _bind_methods();

	float zero_threshold;
	int _last_selected_index;
	Vector<Ref<GSAISteeringBehavior>> _behaviors;
};

#endif
