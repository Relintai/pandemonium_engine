#ifndef GSAI_COHESION_H
#define GSAI_COHESION_H

#include "core/object/reference.h"

#include "../gsai_group_behavior.h"

class GSAITargetAcceleration;
class GSAISteeringAgent;

class GSAICohesion : public GSAIGroupBehavior {
	GDCLASS(GSAICohesion, GSAIGroupBehavior);

public:
	void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);
	bool _report_neighbor(Ref<GSAISteeringAgent> neighbor);

	GSAICohesion();
	~GSAICohesion();

protected:
	static void _bind_methods();

	// Calculates an acceleration that attempts to move the agent towards the center
	// of mass of the agents in the area defined by the `GSAIProximity`.
	// @category - Group behaviors
	Vector3 _center_of_mass;
};

#endif
