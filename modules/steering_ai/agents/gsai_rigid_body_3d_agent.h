#ifndef GSAI_RIGID_BODY_3D_AGENT_H
#define GSAI_RIGID_BODY_3D_AGENT_H

#include "gsai_specialized_agent.h"

class RigidBody;
class GSAITargetAcceleration;

class GSAIRigidBody3DAgent : public GSAISpecializedAgent {
	GDCLASS(GSAIRigidBody3DAgent, GSAISpecializedAgent);

public:
	RigidBody *get_body();
	void set_body(RigidBody *p_body);
	void set_body_bind(Node *p_body);

	void _physics_process_connect();
	void _physics_process_disconnect();

	void _apply_steering(Ref<GSAITargetAcceleration> acceleration, float delta);

	void _on_SceneTree_frame();

	GSAIRigidBody3DAgent();
	~GSAIRigidBody3DAgent();

protected:
	static void _bind_methods();

	Vector3 _last_position;
	ObjectID _body_ref;
};

#endif
