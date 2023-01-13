#ifndef GSAISPECIALIZEDAGENT_H
#define GSAISPECIALIZEDAGENT_H

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

	void apply_steering(const GSAITargetAcceleration &_acceleration, const float _delta);
	void _apply_steering(const GSAITargetAcceleration &_acceleration, const float _delta);

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
	bool calculate_velocities = true;
	// If `true`, interpolates the current linear velocity towards 0 by the
	// `linear_drag_percentage` value.
	// Does not apply to `RigidBody` and `RigidBody2D` nodes.
	bool apply_linear_drag = true;
	// If `true`, interpolates the current angular velocity towards 0 by the
	// `angular_drag_percentage` value.
	// Does not apply to `RigidBody` and `RigidBody2D` nodes.
	bool apply_angular_drag = true;
	// The percentage between the current linear velocity and 0 to interpolate by if
	// `apply_linear_drag` is true.
	// Does not apply to `RigidBody` and `RigidBody2D` nodes.
	float linear_drag_percentage = 0.0;
	// The percentage between the current angular velocity and 0 to interpolate by if
	// `apply_angular_drag` is true.
	// Does not apply to `RigidBody` and `RigidBody2D` nodes.
	float angular_drag_percentage = 0.0;
	float last_orientation = 0.0;
	bool applied_steering = false;
	// Moves the agent's body by target `acceleration`.
	// @tags - virtual
};

#endif
