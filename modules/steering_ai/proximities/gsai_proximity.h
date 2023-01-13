#ifndef GSAI_PROXIMITY_H
#define GSAI_PROXIMITY_H

#include "core/int_types.h"
#include "core/math/vector3.h"

#include "core/object/func_ref.h"
#include "core/object/reference.h"

class GSAISteeringAgent;

class GSAIProximity : public Reference {
	GDCLASS(GSAIProximity, Reference);

public:
	Ref<GSAISteeringAgent> get_agent();
	void set_agent(const Ref<GSAISteeringAgent> &val);

	Vector<Variant> get_agents();
	void set_agents(const Vector<Variant> &arr);

	int find_neighbors(const Ref<FuncRef> &callback);
	int _find_neighbors(Ref<FuncRef> callback);

	GSAIProximity();
	~GSAIProximity();

protected:
	static void _bind_methods();

	// Base container type that stores data to find the neighbors of an agent.
	// @category - Proximities
	// @tags - abstract
	// The owning agent whose neighbors are found in the group
	Ref<GSAISteeringAgent> agent;
	// The agents who are part of this group and could be potential neighbors
	Vector<Ref<GSAISteeringAgent>> agents;
	// Returns a number of neighbors based on a `callback` function.
	//
	// `_find_neighbors` calls `callback` for each agent in the `agents` array and
	// adds one to the count if its `callback` returns true.
	// @tags - virtual
};

#endif
