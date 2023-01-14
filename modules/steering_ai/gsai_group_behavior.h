#ifndef GSAI_GROUP_BEHAVIOR_H
#define GSAI_GROUP_BEHAVIOR_H

#include "core/int_types.h"
#include "core/math/vector3.h"

#include "core/object/func_ref.h"
#include "core/object/reference.h"

#include "gsai_steering_behavior.h"

class GSAIProximity;

class GSAIGroupBehavior : public GSAISteeringBehavior {
	GDCLASS(GSAIGroupBehavior, GSAISteeringBehavior);

public:
	Ref<GSAIProximity> get_proximity();
	void set_proximity(const Ref<GSAIProximity> &val);

	Ref<FuncRef> get_callback();
	void set_callback(const Ref<FuncRef> &val);

	virtual bool _report_neighbor(Ref<GSAISteeringAgent> neighbor);

	GSAIGroupBehavior();
	~GSAIGroupBehavior();

protected:
	static void _bind_methods();

	// Base type for group-based steering behaviors.
	// @category - Base types
	// Container to find neighbors of the agent and calculate group behavior.
	Ref<GSAIProximity> proximity;
	Ref<FuncRef> _callback;
	// Internal callback for the behavior to define whether or not a member is
	// relevant
	// @tags - virtual
};

#endif
