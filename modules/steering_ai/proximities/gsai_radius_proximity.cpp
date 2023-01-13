
#include "gsai_radius_proximity.h"

float GSAIRadiusProximity::get_radius() const {
	return radius;
}

void GSAIRadiusProximity::set_radius(const float val) {
	radius = val;
}

int GSAIRadiusProximity::get__last_frame() const {
	return _last_frame;
}

void GSAIRadiusProximity::set__last_frame(const int val) {
	_last_frame = val;
}

SceneTree GSAIRadiusProximity::get_ *_scene_tree() {
	return *_scene_tree;
}

void GSAIRadiusProximity::set_ *_scene_tree(const SceneTree &val) {
	*_scene_tree = val;
}

// Determines any agent that is in the specified list as being neighbors with the owner agent if;
// they lie within the specified radius.;
// @category - Proximities;
// The radius around the owning agent to find neighbors in;
float radius = 0.0;
int _last_frame = 0;
SceneTree *_scene_tree;

void GSAIRadiusProximity::_init() {
	_scene_tree = Engine.get_main_loop();
}

// Returns a number of neighbors based on a `callback` function.;
//;
// `_find_neighbors` calls `callback` for each agent in the `agents` array that lie within;
// the radius around the owning agent and adds one to the count if its `callback` returns true.;
// @tags - virtual;

int GSAIRadiusProximity::_find_neighbors(const FuncRef &callback) {
	int agent_count = agents.size();
	int neighbor_count = 0;
	int current_frame = ;

	if (_scene_tree) {
		current_frame = _scene_tree.get_frame();
	}

	else {
		current_frame = -_last_frame;
	}

	if (current_frame != _last_frame) {
		_last_frame = current_frame;
		Vector3 owner_position = agent.position;

		for (int i = 0; i < agent_count; ++i) { //i in range(agent_count)
			GSAISteeringAgent *current_agent = agents[i] as GSAISteeringAgent;

			if (current_agent != agent) {
				float distance_squared = owner_position.distance_squared_to(current_agent.position);
				float range_to = radius + current_agent.bounding_radius;

				if (distance_squared < range_to * range_to) {
					if (callback.call_func(current_agent)) {
						current_agent.is_tagged = true;
						neighbor_count += 1;
						continue;
					}
				}
			}

			current_agent.is_tagged = false;
		}

	}

	else {
		for (int i = 0; i < agent_count; ++i) { //i in range(agent_count)
			GSAISteeringAgent *current_agent = agents[i] as GSAISteeringAgent;

			if (current_agent != agent && current_agent.is_tagged) {
				if (callback.call_func(current_agent)) {
					neighbor_count += 1;
				}
			}
		}
	}

	return neighbor_count;
}
}

GSAIRadiusProximity::GSAIRadiusProximity() {
	radius = 0.0;
	_last_frame = 0;
	*_scene_tree;
}

GSAIRadiusProximity::~GSAIRadiusProximity() {
}

static void GSAIRadiusProximity::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_radius"), &GSAIRadiusProximity::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius", "value"), &GSAIRadiusProximity::set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");

	ClassDB::bind_method(D_METHOD("get__last_frame"), &GSAIRadiusProximity::get__last_frame);
	ClassDB::bind_method(D_METHOD("set__last_frame", "value"), &GSAIRadiusProximity::set__last_frame);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "_last_frame"), "set__last_frame", "get__last_frame");

	ClassDB::bind_method(D_METHOD("get_*_scene_tree"), &GSAIRadiusProximity::get_ * _scene_tree);
	ClassDB::bind_method(D_METHOD("set_*_scene_tree", "value"), &GSAIRadiusProximity::set_ * _scene_tree);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_scene_tree", PROPERTY_HINT_RESOURCE_TYPE, "SceneTree"), "set_*_scene_tree", "get_*_scene_tree");

	ClassDB::bind_method(D_METHOD("_init"), &GSAIRadiusProximity::_init);
	ClassDB::bind_method(D_METHOD("_find_neighbors", "callback"), &GSAIRadiusProximity::_find_neighbors);
}
