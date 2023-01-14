#ifndef GSAI_MATCH_ORIENTATION_H
#define GSAI_MATCH_ORIENTATION_H

#include "core/object/reference.h"

#include "../gsai_steering_behavior.h"

class GSAIAgentLocation;
class GSAITargetAcceleration;

class GSAIMatchOrientation : public GSAISteeringBehavior {
	GDCLASS(GSAIMatchOrientation, GSAISteeringBehavior);

public:
	Ref<GSAIAgentLocation> get_target();
	void set_target(const Ref<GSAIAgentLocation> &val);

	float get_alignment_tolerance() const;
	void set_alignment_tolerance(const float val);

	float get_deceleration_radius() const;
	void set_deceleration_radius(const float val);

	float get_time_to_reach() const;
	void set_time_to_reach(const float val);

	bool get_use_z() const;
	void set_use_z(const bool val);

	void match_orientation(const Ref<GSAITargetAcceleration> &acceleration, const float desired_orientation);
	virtual void _match_orientation(Ref<GSAITargetAcceleration> acceleration, float desired_orientation);

	void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);

	GSAIMatchOrientation();
	~GSAIMatchOrientation();

protected:
	static void _bind_methods();

	Ref<GSAIAgentLocation> target;
	float alignment_tolerance;
	float deceleration_radius;
	float time_to_reach;
	bool use_z;
};

#endif
