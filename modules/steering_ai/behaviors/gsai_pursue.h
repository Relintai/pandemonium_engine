#ifndef GSAI_PURSUE_H
#define GSAI_PURSUE_H

#include "core/object/reference.h"

#include "../gsai_steering_behavior.h"

class GSAISteeringAgent;
class GSAITargetAcceleration;

class GSAIPursue : public GSAISteeringBehavior {
	GDCLASS(GSAIPursue, GSAISteeringBehavior);

public:
	Ref<GSAISteeringAgent> get_target();
	void set_target(const Ref<GSAISteeringAgent> &val);

	float get_predict_time_max() const;
	void set_predict_time_max(const float val);

	void _calculate_steering(Ref<GSAITargetAcceleration> acceleration);
	
	float get_modified_acceleration();
	virtual float _get_modified_acceleration();

	GSAIPursue();
	~GSAIPursue();

protected:
	static void _bind_methods();

	// Calculates an acceleration to make an agent intercept another based on the
	// target agent's movement.
	// @category - Individual behaviors
	// The target agent that the behavior is trying to intercept.
	Ref<GSAISteeringAgent> target;
	// The maximum amount of time in the future the behavior predicts the target's
	// location.
	float predict_time_max;
};

#endif
