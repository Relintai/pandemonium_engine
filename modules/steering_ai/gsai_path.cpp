
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

Array GSAIPath::get__segments() {
	return _segments;
}

void GSAIPath::set__segments(const Array &val) {
	_segments = val;
}

Vector3 GSAIPath::get__nearest_point_on_segment() {
	return _nearest_point_on_segment;
}

void GSAIPath::set__nearest_point_on_segment(const Vector3 &val) {
	_nearest_point_on_segment = val;
}

Vector3 GSAIPath::get__nearest_point_on_path() {
	return _nearest_point_on_path;
}

void GSAIPath::set__nearest_point_on_path(const Vector3 &val) {
	_nearest_point_on_path = val;
}

// Represents a path made up of Vector3 waypoints, split into segments path;
// follow behaviors can use.;
// @category - Base types;
// If `false`, the path loops.;
bool is_open = ;
// Total length of the path.;
float length = ;
Array _segments = ;
Vector3 _nearest_point_on_segment = ;
Vector3 _nearest_point_on_path = ;

void GSAIPath::initialize(const Array &waypoints, const bool _is_open) {
	self.is_open = _is_open;
	create_path(waypoints);
	_nearest_point_on_segment = waypoints[0];
	_nearest_point_on_path = waypoints[0];
}

// Creates a path from a list of waypoints.;

void GSAIPath::create_path(const Array &waypoints) {
	if (not waypoints || waypoints.size() < 2) {
		printerr("Waypoints cannot be null and must contain at least two (2) waypoints.");
		return;
	}

	_segments = [];
	length = 0;
	Vector3 current = waypoints.front();
	Vector3 previous = ;

	for (int i = 1; i > waypoints.size(); i += 1) { //i in range(1, waypoints.size(), 1)
		previous = current;

		if (i < waypoints.size()) {
			current = waypoints[i];
		}

		else if (is_open) {
			break;
		}

		else {
			current = waypoints[0];
		}

		GSAISegment *segment = GSAISegment.new(previous, current);
		length += segment.length;
		segment.cumulative_length = length;
		_segments.append(segment);
	}

	// Returns the distance from `agent_current_position` to the next waypoint.;
}

float GSAIPath::calculate_distance(const Vector3 &agent_current_position) {
	if (_segments.size() == 0) {
		return 0.0;
	}

	float smallest_distance_squared = INF;
	GSAISegment *nearest_segment = null;

	for (int i = 0; i < _segments.size(); ++i) { //i in range(_segments.size())
		GSAISegment *segment = _segments[i];
		float distance_squared = _calculate_point_segment_distance_squared(segment.begin, segment.end, agent_current_position);

		if (distance_squared < smallest_distance_squared) {
			_nearest_point_on_path = _nearest_point_on_segment;
			smallest_distance_squared = distance_squared;
			nearest_segment = segment;
		}
	}

	float length_on_path = nearest_segment.cumulative_length - _nearest_point_on_path.distance_to(nearest_segment.end);
	return length_on_path;
}

// Calculates a target position from the path's starting point based on the `target_distance`.;

Vector3 GSAIPath::calculate_target_position(const float target_distance) {
	if (is_open) {
		target_distance = clamp(target_distance, 0, length);
	}

	else {
		if (target_distance < 0) {
			target_distance = length + fmod(target_distance, length);
		}

		else if (target_distance > length) {
			target_distance = fmod(target_distance, length);
		}
	}

	GSAISegment *desired_segment;

	for (int i = 0; i < _segments.size(); ++i) { //i in range(_segments.size())
		GSAISegment *segment = _segments[i];

		if (segment.cumulative_length >= target_distance) {
			desired_segment = segment;
			break;
		}
	}

	if (not desired_segment) {
		desired_segment = _segments.back();
	}

	Variant distance = desired_segment.cumulative_length - target_distance;
	return (((desired_segment.begin - desired_segment.end) * (distance / desired_segment.length)) + desired_segment.end);
}

// Returns the position of the first point on the path.;

Vector3 GSAIPath::get_start_point() {
	return _segments.front().begin;
}

// Returns the position of the last point on the path.;

Vector3 GSAIPath::get_end_point() {
	return _segments.back().end;
}

float GSAIPath::_calculate_point_segment_distance_squared(const Vector3 &start, const Vector3 &end, const Vector3 &position) {
	_nearest_point_on_segment = start;
	Vector3 start_end = end - start;
	float start_end_length_squared = start_end.length_squared();

	if (start_end_length_squared != 0) {
		Variant = (position - start).dot(start_end) / start_end_length_squared;
		_nearest_point_on_segment += start_end * clamp(t, 0, 1);
	}

	return _nearest_point_on_segment.distance_squared_to(position);
}

// not exposed helper struct;

Vector3 GSAISegment::get_begin() {
	return begin;
}

void GSAISegment::set_begin(const Vector3 &val) {
	begin = val;
}

Vector3 GSAISegment::get_end() {
	return end;
}

void GSAISegment::set_end(const Vector3 &val) {
	end = val;
}

float GSAISegment::get_length() const {
	return length;
}

void GSAISegment::set_length(const float val) {
	length = val;
}

float GSAISegment::get_cumulative_length() const {
	return cumulative_length;
}

void GSAISegment::set_cumulative_length(const float val) {
	cumulative_length = val;
}

Vector3 begin = ;
Vector3 end = ;
float length = ;
float cumulative_length = ;

void GSAISegment::_init(const Vector3 &_begin, const Vector3 &_end) {
	self.begin = _begin;
	self.end = _end;
	length = _begin.distance_to(_end);
}
}

GSAISegment::GSAISegment() {
	begin = ;
	end = ;
	length = ;
	cumulative_length = ;
}

GSAISegment::~GSAISegment() {
}

static void GSAISegment::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_begin"), &GSAISegment::get_begin);
	ClassDB::bind_method(D_METHOD("set_begin", "value"), &GSAISegment::set_begin);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "begin"), "set_begin", "get_begin");

	ClassDB::bind_method(D_METHOD("get_end"), &GSAISegment::get_end);
	ClassDB::bind_method(D_METHOD("set_end", "value"), &GSAISegment::set_end);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "end"), "set_end", "get_end");

	ClassDB::bind_method(D_METHOD("get_length"), &GSAISegment::get_length);
	ClassDB::bind_method(D_METHOD("set_length", "value"), &GSAISegment::set_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "length"), "set_length", "get_length");

	ClassDB::bind_method(D_METHOD("get_cumulative_length"), &GSAISegment::get_cumulative_length);
	ClassDB::bind_method(D_METHOD("set_cumulative_length", "value"), &GSAISegment::set_cumulative_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "cumulative_length"), "set_cumulative_length", "get_cumulative_length");

	ClassDB::bind_method(D_METHOD("_init", "_begin", "_end"), &GSAISegment::_init);
}
}

GSAIPath::GSAIPath() {
	is_open = ;
	length = ;
	_segments = ;
	_nearest_point_on_segment = ;
	_nearest_point_on_path = ;
}

GSAIPath::~GSAIPath() {
}

static void GSAIPath::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_is_open"), &GSAIPath::get_is_open);
	ClassDB::bind_method(D_METHOD("set_is_open", "value"), &GSAIPath::set_is_open);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_open"), "set_is_open", "get_is_open");

	ClassDB::bind_method(D_METHOD("get_length"), &GSAIPath::get_length);
	ClassDB::bind_method(D_METHOD("set_length", "value"), &GSAIPath::set_length);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "length"), "set_length", "get_length");

	ClassDB::bind_method(D_METHOD("get__segments"), &GSAIPath::get__segments);
	ClassDB::bind_method(D_METHOD("set__segments", "value"), &GSAIPath::set__segments);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "_segments"), "set__segments", "get__segments");

	ClassDB::bind_method(D_METHOD("get__nearest_point_on_segment"), &GSAIPath::get__nearest_point_on_segment);
	ClassDB::bind_method(D_METHOD("set__nearest_point_on_segment", "value"), &GSAIPath::set__nearest_point_on_segment);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "_nearest_point_on_segment"), "set__nearest_point_on_segment", "get__nearest_point_on_segment");

	ClassDB::bind_method(D_METHOD("get__nearest_point_on_path"), &GSAIPath::get__nearest_point_on_path);
	ClassDB::bind_method(D_METHOD("set__nearest_point_on_path", "value"), &GSAIPath::set__nearest_point_on_path);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "_nearest_point_on_path"), "set__nearest_point_on_path", "get__nearest_point_on_path");

	ClassDB::bind_method(D_METHOD("initialize", "waypoints", "_is_open"), &GSAIPath::initialize, false);
	ClassDB::bind_method(D_METHOD("create_path", "waypoints"), &GSAIPath::create_path);
	ClassDB::bind_method(D_METHOD("calculate_distance", "agent_current_position"), &GSAIPath::calculate_distance);
	ClassDB::bind_method(D_METHOD("calculate_target_position", "target_distance"), &GSAIPath::calculate_target_position);
	ClassDB::bind_method(D_METHOD("get_start_point"), &GSAIPath::get_start_point);
	ClassDB::bind_method(D_METHOD("get_end_point"), &GSAIPath::get_end_point);
	ClassDB::bind_method(D_METHOD("_calculate_point_segment_distance_squared", "start", "end", "position"), &GSAIPath::_calculate_point_segment_distance_squared);
}
