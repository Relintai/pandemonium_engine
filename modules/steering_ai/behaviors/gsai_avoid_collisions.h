#ifndef GSAI_AVOID_COLLISIONS_H
#define GSAI_AVOID_COLLISIONS_H

#include "core/object/reference.h"

#include "../gsai_group_behavior.h"

class GSAITargetAcceleration;
class GSAISteeringAgent;

class GSAIAvoidCollisions : public GSAIGroupBehavior {
	GDCLASS(GSAIAvoidCollisions, GSAIGroupBehavior);

public:
	void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);
	bool _report_neighbor(Ref<GSAISteeringAgent> neighbor);

	GSAIAvoidCollisions();
	~GSAIAvoidCollisions();

protected:
	static void _bind_methods();

	Ref<GSAISteeringAgent> _first_neighbor;
	float _shortest_time;
	float _first_minimum_separation;
	float _first_distance;
	Vector3 _first_relative_position;
	Vector3 _first_relative_velocity;
};

#endif
