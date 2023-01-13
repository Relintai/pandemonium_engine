#ifndef GSAIKINEMATICBODY2DAGENT_H
#define GSAIKINEMATICBODY2DAGENT_H

class GSAIKinematicBody2DAgent : public GSAISpecializedAgent {
	GDCLASS(GSAIKinematicBody2DAgent, GSAISpecializedAgent);

public:
	KinematicBody2D get_ *body();
	void set_ *body(const KinematicBody2D &val);

	int get_movement_type() const;
	void set_movement_type(const int val);

	Vector2 get__last_position();
	void set__last_position(const Vector2 &val);

	Ref<WeakRef> get__body_ref();
	void set__body_ref(const Ref<WeakRef> &val);

	enum MovementType {

		SLIDE,
		COLLIDE,
		POSITION
	};

	void _body_ready();
	void _apply_steering(const GSAITargetAcceleration &acceleration, const float delta);
	void _apply_sliding_steering(const Vector3 &accel, const float delta);
	void _apply_collide_steering(const Vector3 &accel, const float delta);
	void _apply_position_steering(const Vector3 &accel, const float delta);
	void _apply_orientation_steering(const float angular_acceleration, const float delta);
	void _set_body(const KinematicBody2D &value);
	void _on_SceneTree_physics_frame();

	GSAIKinematicBody2DAgent();
	~GSAIKinematicBody2DAgent();

protected:
	static void _bind_methods();

	// A specialized steering agent that updates itself every frame so the user does
	// not have to using a KinematicBody2D
	// @category - Specialized agents
	// SLIDE uses `move_and_slide`
	// COLLIDE uses `move_and_collide`
	// POSITION changes the `global_position` directly
};
// The KinematicBody2D to keep track of
// setget _set_body
KinematicBody2D *body;
// The type of movement the body executes
int movement_type = MovementType.SLIDE;
Vector2 _last_position = ;
Ref<WeakRef> _body_ref;
// Moves the agent's `body` by target `acceleration`.
// @tags - virtual
}
;

#endif
