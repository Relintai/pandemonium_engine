#ifndef GSAIPATH_H
#define GSAIPATH_H

class GSAIPath : public Reference {
	GDCLASS(GSAIPath, Reference);

public:
	bool get_is_open() const;
	void set_is_open(const bool val);

	float get_length() const;
	void set_length(const float val);

	Array get__segments();
	void set__segments(const Array &val);

	Vector3 get__nearest_point_on_segment();
	void set__nearest_point_on_segment(const Vector3 &val);

	Vector3 get__nearest_point_on_path();
	void set__nearest_point_on_path(const Vector3 &val);

	void initialize(const Array &waypoints, const bool _is_open = false);
	void create_path(const Array &waypoints);
	float calculate_distance(const Vector3 &agent_current_position);
	Vector3 calculate_target_position(const float target_distance);
	Vector3 get_start_point();
	Vector3 get_end_point();
	float _calculate_point_segment_distance_squared(const Vector3 &start, const Vector3 &end, const Vector3 &position);
	class GSAISegment {
	public:
		Vector3 get_begin();
		void set_begin(const Vector3 &val);

		Vector3 get_end();
		void set_end(const Vector3 &val);

		float get_length() const;
		void set_length(const float val);

		float get_cumulative_length() const;
		void set_cumulative_length(const float val);

		void _init(const Vector3 &_begin, const Vector3 &_end);

		GSAISegment();
		~GSAISegment();

	protected:
		static void _bind_methods();

		Vector3 begin = ;
		Vector3 end = ;
		float length = ;
		float cumulative_length = ;
	};

	GSAIPath();
	~GSAIPath();

protected:
	static void _bind_methods();

	// Represents a path made up of Vector3 waypoints, split into segments path
	// follow behaviors can use.
	// @category - Base types
	// If `false`, the path loops.
	bool is_open = ;
	// Total length of the path.
	float length = ;
	Array _segments = ;
	Vector3 _nearest_point_on_segment = ;
	Vector3 _nearest_point_on_path = ;
	// Creates a path from a list of waypoints.
	// Calculates a target position from the path's starting point based on the `target_distance`.
	// Returns the position of the first point on the path.
	// Returns the position of the last point on the path.
	// not exposed helper struct
};

#endif
