
#include "gsai_radius_proximity.h"

#include "../gsai_steering_agent.h"
#include "scene/main/scene_tree.h"

float GSAIRadiusProximity::get_radius() const {
	return radius;
}

void GSAIRadiusProximity::set_radius(const float val) {
	radius = val;
}

int GSAIRadiusProximity::_find_neighbors(Ref<FuncRef> callback) {
	ERR_FAIL_COND_V(!agent.is_valid(), 0);

	int agent_count = agents.size();
	int neighbor_count = 0;
	int current_frame;
	Variant arg;
	const Variant *argptr[1];
	argptr[0] = &arg;
	Variant::CallError err;

	SceneTree *scene_tree = SceneTree::get_singleton();

	if (scene_tree) {
		current_frame = scene_tree->get_frame();
	} else {
		current_frame = -_last_frame;
	}

	if (current_frame != _last_frame) {
		_last_frame = current_frame;
		Vector3 owner_position = agent->get_position();

		for (int i = 0; i < agent_count; ++i) {
			Ref<GSAISteeringAgent> current_agent = agents[i];

			ERR_CONTINUE(!current_agent.is_valid());

			if (current_agent != agent) {
				float distance_squared = owner_position.distance_squared_to(current_agent->get_position());
				float range_to = radius + current_agent->get_bounding_radius();

				arg = current_agent.get_ref_ptr();

				if (distance_squared < range_to * range_to) {
					if (callback->call_func(argptr, 1, err)) {
						current_agent->set_is_tagged(true);
						neighbor_count += 1;
						continue;
					}
				}
			}

			current_agent->set_is_tagged(false);
		}

	} else {
		for (int i = 0; i < agent_count; ++i) {
			Ref<GSAISteeringAgent> current_agent = agents[i];

			ERR_CONTINUE(!current_agent.is_valid());

			if (current_agent != agent && current_agent->get_is_tagged()) {
				arg = current_agent.get_ref_ptr();

				if (callback->call_func(argptr, 1, err)) {
					neighbor_count += 1;
				}
			}
		}
	}

	return neighbor_count;
}

GSAIRadiusProximity::GSAIRadiusProximity() {
	radius = 0.0;
	_last_frame = 0;
}

GSAIRadiusProximity::~GSAIRadiusProximity() {
}

void GSAIRadiusProximity::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_radius"), &GSAIRadiusProximity::get_radius);
	ClassDB::bind_method(D_METHOD("set_radius", "value"), &GSAIRadiusProximity::set_radius);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "radius"), "set_radius", "get_radius");
}
