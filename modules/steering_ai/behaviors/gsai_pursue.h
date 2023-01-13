#ifndef GSAIPURSUE_H
#define GSAIPURSUE_H

class GSAIPursue : public GSAISteeringBehavior {
	GDCLASS(GSAIPursue, GSAISteeringBehavior);

public:
	GSAISteeringAgent get_ *target();
	void set_ *target(const GSAISteeringAgent &val);

	float get_predict_time_max() const;
	void set_predict_time_max(const float val);

	void _calculate_steering(const GSAITargetAcceleration &acceleration);
	float get_modified_acceleration();
	float _get_modified_acceleration();

	GSAIPursue();
	~GSAIPursue();

protected:
	static void _bind_methods();

	// Calculates an acceleration to make an agent intercept another based on the
	// target agent's movement.
	// @category - Individual behaviors
	// The target agent that the behavior is trying to intercept.
	GSAISteeringAgent *target;
	// The maximum amount of time in the future the behavior predicts the target's
	// location.
	float predict_time_max = 1.0;
};

#endif
