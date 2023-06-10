
#include "gsai_proximity.h"

#include "../gsai_steering_agent.h"

Ref<GSAISteeringAgent> GSAIProximity::get_agent() {
	return agent;
}

void GSAIProximity::set_agent(const Ref<GSAISteeringAgent> &val) {
	agent = val;
}

Vector<Ref<GSAISteeringAgent>> GSAIProximity::get_agents() {
	return agents;
}

void GSAIProximity::set_agents(const Vector<Ref<GSAISteeringAgent>> &arr) {
	agents = arr;
}

Array GSAIProximity::get_agents_arr() {
	Array r;
	for (int i = 0; i < agents.size(); i++) {
		r.push_back(agents[i].get_ref_ptr());
	}
	return r;
}

void GSAIProximity::set_agents_arr(const Array &arr) {
	agents.clear();
	for (int i = 0; i < arr.size(); i++) {
		Ref<GSAISteeringAgent> e = Ref<GSAISteeringAgent>(arr[i]);
		agents.push_back(e);
	}
}

int GSAIProximity::find_neighbors(const Ref<FuncRef> &callback) {
	return call("_find_neighbors", callback);
}

int GSAIProximity::_find_neighbors(Ref<FuncRef> callback) {
	return 0;
}

GSAIProximity::GSAIProximity() {
}

GSAIProximity::~GSAIProximity() {
}

void GSAIProximity::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_agent"), &GSAIProximity::get_agent);
	ClassDB::bind_method(D_METHOD("set_agent", "value"), &GSAIProximity::set_agent);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "agent", PROPERTY_HINT_RESOURCE_TYPE, "GSAISteeringAgent"), "set_agent", "get_agent");

	ClassDB::bind_method(D_METHOD("get_agents"), &GSAIProximity::get_agents_arr);
	ClassDB::bind_method(D_METHOD("set_agents", "arr"), &GSAIProximity::set_agents_arr);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "agents", PROPERTY_HINT_NONE, "23/20:GSAISteeringAgent", PROPERTY_USAGE_DEFAULT, "GSAISteeringAgent"), "set_agents", "get_agents");

	BIND_VMETHOD(MethodInfo(Variant::INT, "_find_neighbors", PropertyInfo(Variant::OBJECT, "callback", PROPERTY_HINT_RESOURCE_TYPE, "FuncRef")));

	ClassDB::bind_method(D_METHOD("find_neighbors", "callback"), &GSAIProximity::find_neighbors);
	ClassDB::bind_method(D_METHOD("_find_neighbors", "callback"), &GSAIProximity::_find_neighbors);
}
