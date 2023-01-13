#ifndef GSAI_SEEK_H
#define GSAI_SEEK_H

class GSAISeek : public GSAISteeringBehavior {
	GDCLASS(GSAISeek, GSAISteeringBehavior);

public:
	GSAIAgentLocation get_ *target();
	void set_ *target(const GSAIAgentLocation &val);

	void _calculate_steering(const GSAITargetAcceleration &acceleration);

	GSAISeek();
	~GSAISeek();

protected:
	static void _bind_methods();

	// Calculates an acceleration to take an agent to a target agent's position
	// directly.
	// @category - Individual behaviors
	// The target that the behavior aims to move the agent to.
	GSAIAgentLocation *target;
};

#endif
