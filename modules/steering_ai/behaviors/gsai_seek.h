#ifndef GSAI_SEEK_H
#define GSAI_SEEK_H

#include "core/object/reference.h"

#include "../gsai_steering_behavior.h"

class GSAITargetAcceleration;
class GSAIAgentLocation;

class GSAISeek : public GSAISteeringBehavior {
	GDCLASS(GSAISeek, GSAISteeringBehavior);

public:
	Ref<GSAIAgentLocation> get_target();
	void set_target(const Ref<GSAIAgentLocation> &val);

	void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);

	GSAISeek();
	~GSAISeek();

protected:
	static void _bind_methods();

	// Calculates an acceleration to take an agent to a target agent's position
	// directly.
	// @category - Individual behaviors
	// The target that the behavior aims to move the agent to.
	Ref<GSAIAgentLocation> target;
};

#endif
