#ifndef GSAI_RIGID_BODY_2D_AGENT_H
#define GSAI_RIGID_BODY_2D_AGENT_H

#include "gsai_specialized_agent.h"

class RigidBody2D;
class GSAITargetAcceleration;

class GSAIRigidBody2DAgent : public GSAISpecializedAgent {
	GDCLASS(GSAIRigidBody2DAgent, GSAISpecializedAgent);

public:
	RigidBody2D *get_body();
	void set_body(RigidBody2D *p_body);
	void set_body_bind(Node *p_body);

	void _physics_process_connect();
	void _physics_process_disconnect();

	void _apply_steering(Ref<GSAITargetAcceleration> acceleration, float delta);

	void _on_SceneTree_frame();

	GSAIRigidBody2DAgent();
	~GSAIRigidBody2DAgent();

protected:
	static void _bind_methods();

	// A specialized steering agent that updates itself every frame so the user does
	// not have to using a RigidBody2D
	// @category - Specialized agents
	// The RigidBody2D to keep track of
	// setget _set_body
	Vector2 _last_position;
	ObjectID _body_ref;
	// Moves the agent's `body` by target `acceleration`.
	// @tags - virtual
};

#endif
