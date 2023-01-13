
#include "gsai_path.h"

bool GSAIPath::get_is_open() const {
	return is_open;
}

void GSAIPath::set_is_open(const bool val) {
	is_open = val;
}

float GSAIPath::get_length() const {
	return length;
}

void GSAIPath::set_length(const float val) {
	length = val;
}

void GSAIPath::initialize(const PoolVector3Array &waypoints, const bool _is_open) {
	is_open = _is_open;

	create_path(waypoints);

	if (waypoints.size() > 0) {
		_nearest_point_on_segment = waypoints[0];
		_nearest_point_on_path = waypoints[0];
	} else {
		_nearest_point_on_segment = Vector3();
		_nearest_point_on_path = Vector3();
	}
}

void GSAIPath::create_path(const PoolVector3Array &waypoints) {
	ERR_FAIL_COND_MSG(waypoints.size() < 2, "Waypoints must contain at least two (2) waypoints.");

	_segments.clear();
	length = 0;

	Vector3 current = waypoints[0];
	Vector3 previous;

	for (int i = 1; i < waypoints.size(); ++i) {
		previous = current;

		if (i < waypoints.size()) {
			current = waypoints[i];
		} else if (is_open) {
			break;
		} else {
			current = waypoints[0];
		}

		GSAISegment segment(previous, current);
		length += segment.length;
		segment.cumulative_length = length;
		_segments.push_back(segment);
	}
}

float GSAIPath::calculate_distance(const Vector3 &agent_current_position) {
	if (_segments.size() == 0) {
		return 0;
	}

	float smallest_distance_squared = Math_INF;
	const GSAISegment *nearest_segment = NULL;

	for (int i = 0; i < _segments.size(); ++i) {
		const GSAISegment &segment = _segments[i];
		float distance_squared = _calculate_point_segment_distance_squared(segment.begin, segment.end, agent_current_position);

		if (distance_squared < smallest_distance_squared) {
			_nearest_point_on_path = _nearest_point_on_segment;
			smallest_distance_squared = distance_squared;
			nearest_segment = &segment;
		}
	}

	float length_on_path = nearest_segment->cumulative_length - _nearest_point_on_path.distance_to(nearest_segment->end);

	return length_on_path;
}

Vector3 GSAIPath::calculate_target_position(float target_distance) {
	if (_segments.size() == 0) {
		return Vector3();
	}

	if (is_open) {
		target_distance = CLAMP(target_distance, 0, length);
	} else {
		if (target_distance < 0) {
			target_distance = length + fmod(target_distance, length);
		} else if (target_distance > length) {
			target_distance = fmod(target_distance, length);
		}
	}

	const GSAISegment *desired_segment = NULL;

	for (int i = 0; i < _segments.size(); ++i) {
		const GSAISegment &segment = _segments[i];

		if (segment.cumulative_length >= target_distance) {
			desired_segment = &segment;
			break;
		}
	}

	if (!desired_segment) {
		desired_segment = &_segments[_segments.size() - 1];
	}

	float distance = desired_segment->cumulative_length - target_distance;
	return (((desired_segment->begin - desired_segment->end) * (distance / desired_segment->length)) + desired_segment->end);
}

Vector3 GSAIPath::get_start_point() {
	if (_segments.size() == 0) {
		return Vector3();
	}

	return _segments[0].begin;
}

Vector3 GSAIPath::get_end_point() {
	if (_segments.size() == 0) {
		return Vector3();
	}

	return _segments[_segments.size() - 1].end;
}

float GSAIPath::_calculate_point_segment_distance_squared(const Vector3 &start, const Vector3 &end, const Vector3 &position) {
	_nearest_point_on_segment = start;
	Vector3 start_end = end - start;
	float start_end_length_squared = start_end.length_squared();

	if (start_end_length_squared != 0) {
		float t = (position - start).dot(start_end) / start_end_length_squared;
		_nearest_point_on_segment += start_end * CLAMP(t, 0, 1);
	}

	return _nearest_point_on_segment.distance_squared_to(position);
}

GSAIPath::GSAIPath() {
	is_open = false;
	length = 0;
}

GSAIPath::~GSAIPath() {
}

void GSAIPath::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_open"), &GSAIPath::get_is_open);
	ClassDB::bind_method(D_METHOD("set_is_open", "value"), &GSAIPath::set_is_open);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_open"), "set_is_open", "get_is_open");

	ClassDB::bind_method(D_METHOD("get_length"), &GSAIPath::get_length);
	ClassDB::bind_method(D_METHOD("set_length", "value"), &GSAIPath::set_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "length"), "set_length", "get_length");

	ClassDB::bind_method(D_METHOD("initialize", "waypoints", "is_open"), &GSAIPath::initialize, false);
	ClassDB::bind_method(D_METHOD("create_path", "waypoints"), &GSAIPath::create_path);
	ClassDB::bind_method(D_METHOD("calculate_distance", "agent_current_position"), &GSAIPath::calculate_distance);
	ClassDB::bind_method(D_METHOD("calculate_target_position", "target_distance"), &GSAIPath::calculate_target_position);
	ClassDB::bind_method(D_METHOD("get_start_point"), &GSAIPath::get_start_point);
	ClassDB::bind_method(D_METHOD("get_end_point"), &GSAIPath::get_end_point);
}
