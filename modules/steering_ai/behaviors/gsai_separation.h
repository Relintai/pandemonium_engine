#ifndef GSAISEPARATION_H
#define GSAISEPARATION_H

class GSAISeparation : public GSAIGroupBehavior {
	GDCLASS(GSAISeparation, GSAIGroupBehavior);

public:
	float get_decay_coefficient() const;
	void set_decay_coefficient(const float val);

	GSAITargetAcceleration get_ *acceleration();
	void set_ *acceleration(const GSAITargetAcceleration &val);

	void _calculate_steering(const GSAITargetAcceleration &_acceleration);
	bool _report_neighbor(const GSAISteeringAgent &neighbor);

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
	float decay_coefficient = 1.0;
	GSAITargetAcceleration *acceleration;
	// Callback for the proximity to call when finding neighbors. Determines the amount of
	// acceleration that `neighbor` imposes based on its distance from the owner agent.
	// @tags - virtual
};

#endif
