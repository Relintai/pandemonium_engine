#ifndef GSAIMATCHORIENTATION_H
#define GSAIMATCHORIENTATION_H

class GSAIMatchOrientation : public GSAISteeringBehavior {
	GDCLASS(GSAIMatchOrientation, GSAISteeringBehavior);

public:
	GSAIAgentLocation get_ *target();
	void set_ *target(const GSAIAgentLocation &val);

	float get_alignment_tolerance() const;
	void set_alignment_tolerance(const float val);

	float get_deceleration_radius() const;
	void set_deceleration_radius(const float val);

	float get_time_to_reach() const;
	void set_time_to_reach(const float val);

	bool get_use_z() const;
	void set_use_z(const bool val);

	void match_orientation(const GSAITargetAcceleration &acceleration, const float desired_orientation);
	void _match_orientation(const GSAITargetAcceleration &acceleration, const float desired_orientation);
	void _calculate_steering(const GSAITargetAcceleration &acceleration);

	GSAIMatchOrientation();
	~GSAIMatchOrientation();

protected:
	static void _bind_methods();

	// Calculates an angular acceleration to match an agent's orientation to that of
	// its target. Attempts to make the agent arrive with zero remaining angular
	// velocity.
	// @category - Individual behaviors
	// The target orientation for the behavior to try and match rotations to.
	GSAIAgentLocation *target;
	// The amount of distance in radians for the behavior to consider itself close
	// enough to be matching the target agent's rotation.
	float alignment_tolerance = 0.0;
	// The amount of distance in radians from the goal to start slowing down.
	float deceleration_radius = 0.0;
	// The amount of time to reach the target velocity
	float time_to_reach = 0.1;
	// Whether to use the X and Z components instead of X and Y components when
	// determining angles. X and Z should be used in 3D.
	bool use_z = false;
};

#endif
