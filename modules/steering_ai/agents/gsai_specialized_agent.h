#ifndef GSAI_SPECIALIZED_AGENT_H
#define GSAI_SPECIALIZED_AGENT_H

#include "core/object/reference.h"

#include "../gsai_steering_agent.h"

class GSAITargetAcceleration;

class GSAISpecializedAgent : public GSAISteeringAgent {
	GDCLASS(GSAISpecializedAgent, GSAISteeringAgent);

public:
	bool get_calculate_velocities() const;
	void set_calculate_velocities(const bool val);

	bool get_apply_linear_drag() const;
	void set_apply_linear_drag(const bool val);

	bool get_apply_angular_drag() const;
	void set_apply_angular_drag(const bool val);

	float get_linear_drag_percentage() const;
	void set_linear_drag_percentage(const float val);

	float get_angular_drag_percentage() const;
	void set_angular_drag_percentage(const float val);

	float get_last_orientation() const;
	void set_last_orientation(const float val);

	bool get_applied_steering() const;
	void set_applied_steering(const bool val);

	void apply_steering(const Ref<GSAITargetAcceleration> &acceleration, const float delta);
	virtual void _apply_steering(Ref<GSAITargetAcceleration> acceleration, float delta);

	GSAISpecializedAgent();
	~GSAISpecializedAgent();

protected:
	static void _bind_methods();

	bool calculate_velocities;
	bool apply_linear_drag;
	bool apply_angular_drag;
	float linear_drag_percentage;
	float angular_drag_percentage;
	float last_orientation;
	bool applied_steering;
};

#endif
