#ifndef GSAI_PATH_H
#define GSAI_PATH_H

/*************************************************************************/
/*  gsai_path.h                                                          */
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
#include "core/variant/array.h"

#include "core/object/reference.h"

class GSAIPath : public Reference {
	GDCLASS(GSAIPath, Reference);

public:
	bool get_is_open() const;
	void set_is_open(const bool val);

	float get_length() const;
	void set_length(const float val);

	Vector3 get_nearest_point_on_segment();
	void set_nearest_point_on_segment(const Vector3 &val);

	void initialize(const PoolVector3Array &waypoints, const bool _is_open = false);
	void create_path(const PoolVector3Array &waypoints);
	float calculate_distance(const Vector3 &agent_current_position);
	Vector3 calculate_target_position(float target_distance);
	Vector3 get_start_point();
	Vector3 get_end_point();
	float _calculate_point_segment_distance_squared(const Vector3 &start, const Vector3 &end, const Vector3 &position);

	GSAIPath();
	~GSAIPath();

protected:
	struct GSAISegment {
		Vector3 begin;
		Vector3 end;
		float length;
		float cumulative_length;

		GSAISegment() {
			length = 0;
			cumulative_length = 0;
		}

		GSAISegment(const Vector3 &p_begin, const Vector3 &p_end) {
			begin = p_begin;
			end = p_end;
			length = p_begin.distance_to(p_end);
			cumulative_length = 0;
		}
	};

protected:
	static void _bind_methods();

	bool is_open;
	float length;

	Vector<GSAISegment> _segments;

	Vector3 _nearest_point_on_segment;
	Vector3 _nearest_point_on_path;
};

#endif
