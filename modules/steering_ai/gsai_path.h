#ifndef GSAI_PATH_H
#define GSAI_PATH_H

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

	// Represents a path made up of Vector3 waypoints, split into segments path
	// follow behaviors can use.
	// @category - Base types
	// If `false`, the path loops.
	bool is_open;
	// Total length of the path.
	float length;

	Vector<GSAISegment> _segments;

	Vector3 _nearest_point_on_segment;
	Vector3 _nearest_point_on_path;
	// Creates a path from a list of waypoints.
	// Calculates a target position from the path's starting point based on the `target_distance`.
	// Returns the position of the first point on the path.
	// Returns the position of the last point on the path.
	// not exposed helper struct
};

#endif
