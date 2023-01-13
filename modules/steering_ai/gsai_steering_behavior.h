#ifndef GSAI_STEERING_BEHAVIOR_H
#define GSAI_STEERING_BEHAVIOR_H

class GSAISteeringBehavior : public Reference {
	GDCLASS(GSAISteeringBehavior, Reference);

public:
	bool get_is_enabled() const;
	void set_is_enabled(const bool val);

	GSAISteeringAgent get_ *agent();
	void set_ *agent(const GSAISteeringAgent &val);

	void calculate_steering(const GSAITargetAcceleration &acceleration);
	void _calculate_steering(const GSAITargetAcceleration &acceleration);

	GSAISteeringBehavior();
	~GSAISteeringBehavior();

protected:
	static void _bind_methods();

	// Base class for all steering behaviors.
	//
	// Steering behaviors calculate the linear and the angular acceleration to be
	// to the agent that owns them.
	//
	// The `calculate_steering` function is the entry point for all behaviors.
	// Individual steering behaviors encapsulate the steering logic.
	// @category - Base types
	// If `false`, all calculations return zero amounts of acceleration.
	bool is_enabled = true;
	// The AI agent on which the steering behavior bases its calculations.
	GSAISteeringAgent *agent;
	// Sets the `acceleration` with the behavior's desired amount of acceleration.
};

#endif
