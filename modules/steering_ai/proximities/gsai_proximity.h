#ifndef GSAIPROXIMITY_H
#define GSAIPROXIMITY_H

class GSAIProximity : public Reference {
	GDCLASS(GSAIProximity, Reference);

public:
	GSAISteeringAgent get_ *agent();
	void set_ *agent(const GSAISteeringAgent &val);

	Array get_agents();
	void set_agents(const Array &val);

	int find_neighbors(const FuncRef &_callback);
	int _find_neighbors(const FuncRef &_callback);

	GSAIProximity();
	~GSAIProximity();

protected:
	static void _bind_methods();

	// Base container type that stores data to find the neighbors of an agent.
	// @category - Proximities
	// @tags - abstract
	// The owning agent whose neighbors are found in the group
	GSAISteeringAgent *agent;
	// The agents who are part of this group and could be potential neighbors
	Array agents = Array();
	// Returns a number of neighbors based on a `callback` function.
	//
	// `_find_neighbors` calls `callback` for each agent in the `agents` array and
	// adds one to the count if its `callback` returns true.
	// @tags - virtual
};

#endif
