#ifndef GSAI_STEERING_BEHAVIOR_H
#define GSAI_STEERING_BEHAVIOR_H

#include "core/int_types.h"
#include "core/math/vector3.h"

#include "core/object/reference.h"

class GSAISteeringAgent;
class GSAITargetAcceleration;

// Base class for all steering behaviors.
// Steering behaviors calculate the linear and the angular acceleration to be
// to the agent that owns them.
// Individual steering behaviors encapsulate the steering logic.

class GSAISteeringBehavior : public Reference {
	GDCLASS(GSAISteeringBehavior, Reference);

public:
	// If `false`, all calculations return zero amounts of acceleration.
	bool get_is_enabled() const;
	void set_is_enabled(const bool val);

	// The AI agent on which the steering behavior bases its calculations.
	Ref<GSAISteeringAgent> get_agent();
	void set_agent(const Ref<GSAISteeringAgent> &val);

	// The `calculate_steering` function is the entry point for all behaviors.
	// Sets the `acceleration` with the behavior's desired amount of acceleration.
	void calculate_steering(Ref<GSAITargetAcceleration> acceleration);
	void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);

	GSAISteeringBehavior();
	~GSAISteeringBehavior();

protected:
	static void _bind_methods();

	bool is_enabled;

	Ref<GSAISteeringAgent> agent;
};

#endif
