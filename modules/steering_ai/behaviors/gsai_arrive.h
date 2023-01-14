#ifndef GSAI_ARRIVE_H
#define GSAI_ARRIVE_H

#include "core/object/reference.h"

#include "../gsai_steering_behavior.h"

class GSAIAgentLocation;

class GSAIArrive : public GSAISteeringBehavior {
	GDCLASS(GSAIArrive, GSAISteeringBehavior);

public:
	Ref<GSAIAgentLocation> get_target();
	void set_target(const Ref<GSAIAgentLocation> &val);

	float get_arrival_tolerance() const;
	void set_arrival_tolerance(const float val);

	float get_deceleration_radius() const;
	void set_deceleration_radius(const float val);

	float get_time_to_reach() const;
	void set_time_to_reach(const float val);

	void arrive(const Ref<GSAITargetAcceleration> &acceleration, const Vector3 &target_position);
	virtual void _arrive(Ref<GSAITargetAcceleration> acceleration, Vector3 target_position);

	void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);

	GSAIArrive();
	~GSAIArrive();

protected:
	static void _bind_methods();

	// Calculates acceleration to take an agent to its target's location. The
	// calculation attempts to arrive with zero remaining velocity.
	// @category - Individual behaviors
	// Target agent to arrive to.
	Ref<GSAIAgentLocation> target;
	// Distance from the target for the agent to be considered successfully
	// arrived.
	float arrival_tolerance;
	// Distance from the target for the agent to begin slowing down.
	float deceleration_radius;
	// Represents the time it takes to change acceleration.
	float time_to_reach;
};

#endif
