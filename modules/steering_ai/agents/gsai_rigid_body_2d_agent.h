#ifndef GSAI_RIGID_BODY_2D_AGENT_H
#define GSAI_RIGID_BODY_2D_AGENT_H

class GSAIRigidBody2DAgent : public GSAISpecializedAgent {
	GDCLASS(GSAIRigidBody2DAgent, GSAISpecializedAgent);

public:
	RigidBody2D get_ *body();
	void set_ *body(const RigidBody2D &val);

	Vector2 get__last_position();
	void set__last_position(const Vector2 &val);

	Ref<WeakRef> get__body_ref();
	void set__body_ref(const Ref<WeakRef> &val);

	void _body_ready();
	void _apply_steering(const GSAITargetAcceleration &acceleration, const float _delta);
	void _set_body(const RigidBody2D &value);
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
	RigidBody2D *body;
	Vector2 _last_position = ;
	Ref<WeakRef> _body_ref;
	// Moves the agent's `body` by target `acceleration`.
	// @tags - virtual
};

#endif
