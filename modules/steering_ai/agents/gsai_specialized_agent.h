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

	// A base class for a specialized steering agent that updates itself every frame
	// so the user does not have to. All other specialized agents derive from this.
	// @category - Specialized agents
	// @tags - abstract
	// If `true`, calculates linear and angular velocities based on the previous
	// frame. When `false`, the user must keep those values updated.
	bool calculate_velocities;
	// If `true`, interpolates the current linear velocity towards 0 by the
	// `linear_drag_percentage` value.
	// Does not apply to `RigidBody` and `RigidBody2D` nodes.
	bool apply_linear_drag;
	// If `true`, interpolates the current angular velocity towards 0 by the
	// `angular_drag_percentage` value.
	// Does not apply to `RigidBody` and `RigidBody2D` nodes.
	bool apply_angular_drag;
	// The percentage between the current linear velocity and 0 to interpolate by if
	// `apply_linear_drag` is true.
	// Does not apply to `RigidBody` and `RigidBody2D` nodes.
	float linear_drag_percentage;
	// The percentage between the current angular velocity and 0 to interpolate by if
	// `apply_angular_drag` is true.
	// Does not apply to `RigidBody` and `RigidBody2D` nodes.
	float angular_drag_percentage;
	float last_orientation;
	bool applied_steering;
	// Moves the agent's body by target `acceleration`.
	// @tags - virtual
};

#endif
