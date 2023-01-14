#ifndef GSAI_SEPARATION_H
#define GSAI_SEPARATION_H

#include "core/object/reference.h"

#include "../gsai_group_behavior.h"

class GSAITargetAcceleration;

class GSAISeparation : public GSAIGroupBehavior {
	GDCLASS(GSAISeparation, GSAIGroupBehavior);

public:
	float get_decay_coefficient() const;
	void set_decay_coefficient(const float val);

	Ref<GSAITargetAcceleration> get_acceleration();
	void set_acceleration(const Ref<GSAITargetAcceleration> &val);

	void _calculate_steering(Ref<GSAITargetAcceleration> p_acceleration);
	bool _report_neighbor(Ref<GSAISteeringAgent> p_neighbor);

	GSAISeparation();
	~GSAISeparation();

protected:
	static void _bind_methods();

	// Calculates an acceleration that repels the agent from its neighbors in the
	// given `GSAIProximity`.
	//
	// The acceleration is an average based on all neighbors, multiplied by a
	// strength decreasing by the inverse square law in relation to distance, and it
	// accumulates.
	// @category - Group behaviors
	// The coefficient to calculate how fast the separation strength decays with distance.
	float decay_coefficient;
	Ref<GSAITargetAcceleration> acceleration;
	// Callback for the proximity to call when finding neighbors. Determines the amount of
	// acceleration that `neighbor` imposes based on its distance from the owner agent.
	// @tags - virtual
};

#endif
