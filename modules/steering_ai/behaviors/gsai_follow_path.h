#ifndef GSAI_FOLLOW_PATH_H
#define GSAI_FOLLOW_PATH_H

/*************************************************************************/
/*  gsai_follow_path.h                                                   */
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

#include "gsai_arrive.h"

class GSAIPath;

class GSAIFollowPath : public GSAIArrive {
	GDCLASS(GSAIFollowPath, GSAIArrive);

public:
	Ref<GSAIPath> get_path();
	void set_path(const Ref<GSAIPath> &val);

	float get_path_offset() const;
	void set_path_offset(const float val);

	bool get_is_arrive_enabled() const;
	void set_is_arrive_enabled(const bool val);

	float get_prediction_time() const;
	void set_prediction_time(const float val);

	void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);

	GSAIFollowPath();
	~GSAIFollowPath();

protected:
	static void _bind_methods();

	Ref<GSAIPath> path;
	float path_offset;
	bool is_arrive_enabled;
	float prediction_time;
};

#endif
