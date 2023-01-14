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

	int movement_type;
	Vector3 _last_position;
	ObjectID _body_ref;
};

#endif
