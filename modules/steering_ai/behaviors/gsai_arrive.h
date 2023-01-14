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

	Ref<GSAIAgentLocation> target;
	float arrival_tolerance;
	float deceleration_radius;
	float time_to_reach;
};

#endif
