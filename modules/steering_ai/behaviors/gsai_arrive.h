#ifndef GSAI_ARRIVE_H
#define GSAI_ARRIVE_H

class GSAIArrive : public GSAISteeringBehavior {
	GDCLASS(GSAIArrive, GSAISteeringBehavior);

public:
	GSAIAgentLocation get_ *target();
	void set_ *target(const GSAIAgentLocation &val);

	float get_arrival_tolerance() const;
	void set_arrival_tolerance(const float val);

	float get_deceleration_radius() const;
	void set_deceleration_radius(const float val);

	float get_time_to_reach() const;
	void set_time_to_reach(const float val);

	void arrive(const GSAITargetAcceleration &acceleration, const Vector3 &target_position);
	void _arrive(const GSAITargetAcceleration &acceleration, const Vector3 &target_position);
	void _calculate_steering(const GSAITargetAcceleration &acceleration);

	GSAIArrive();
	~GSAIArrive();

protected:
	static void _bind_methods();

	// Calculates acceleration to take an agent to its target's location. The
	// calculation attempts to arrive with zero remaining velocity.
	// @category - Individual behaviors
	// Target agent to arrive to.
	GSAIAgentLocation *target;
	// Distance from the target for the agent to be considered successfully
	// arrived.
	float arrival_tolerance = 0.0;
	// Distance from the target for the agent to begin slowing down.
	float deceleration_radius = 0.0;
	// Represents the time it takes to change acceleration.
	float time_to_reach = 0.1;
};

#endif
