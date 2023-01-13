
#include "gsai_infinite_proximity.h"

#include "../gsai_steering_agent.h"
#include "scene/main/scene_tree.h"

int GSAIInfiniteProximity::_find_neighbors(Ref<FuncRef> callback) {
	int neighbor_count = 0;
	int agent_count = agents.size();
	Variant arg;
	const Variant *argptr[1];
	argptr[0] = &arg;
	Variant::CallError err;

	for (int i = 0; i < agent_count; ++i) {
		Ref<GSAISteeringAgent> current_agent = agents[i];

		ERR_CONTINUE(!current_agent.is_valid());

		if (current_agent != agent) {
			if (callback->call_func(argptr, 1, err)) {
				neighbor_count += 1;
			}
		}
	}

	return neighbor_count;
}

GSAIInfiniteProximity::GSAIInfiniteProximity() {
}

GSAIInfiniteProximity::~GSAIInfiniteProximity() {
}

void GSAIInfiniteProximity::_bind_methods() {
}
