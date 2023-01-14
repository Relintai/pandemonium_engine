#ifndef GSAI_KINEMATIC_BODY_3D_AGENT_H
#define GSAI_KINEMATIC_BODY_3D_AGENT_H

#include "gsai_specialized_agent.h"

class KinematicBody;
class GSAITargetAcceleration;

class GSAIKinematicBody3DAgent : public GSAISpecializedAgent {
	GDCLASS(GSAIKinematicBody3DAgent, GSAISpecializedAgent);

public:
	KinematicBody *get_body();
	void set_body(KinematicBody *p_body);
	void set_body_bind(Node *p_body);

	int get_movement_type() const;
	void set_movement_type(const int val);

	enum MovementType {
		SLIDE,
		COLLIDE,
		POSITION
	};

	void _physics_process_connect();
	void _physics_process_disconnect();

	void _apply_steering(Ref<GSAITargetAcceleration> acceleration, float delta);

	void _apply_sliding_steering(KinematicBody *body, const Vector3 &accel, const float delta);
	void _apply_collide_steering(KinematicBody *body, const Vector3 &accel, const float delta);
	void _apply_position_steering(KinematicBody *body, const Vector3 &accel, const float delta);
	void _apply_orientation_steering(KinematicBody *body, const float angular_acceleration, const float delta);

	void _on_SceneTree_physics_frame();

	GSAIKinematicBody3DAgent();
	~GSAIKinematicBody3DAgent();

protected:
	static void _bind_methods();

	// A specialized steering agent that updates itself every frame so the user does
	// not have to using a KinematicBody
	// @category - Specialized agents
	// SLIDE uses `move_and_slide`
	// COLLIDE uses `move_and_collide`
	// POSITION changes the global_position directly

	// The KinematicBody to keep track of
	// setget _set_body
	// The type of movement the body executes
	int movement_type;
	Vector3 _last_position;
	ObjectID _body_ref;
	// Moves the agent's `body` by target `acceleration`.
	// @tags - virtual
};

#endif
