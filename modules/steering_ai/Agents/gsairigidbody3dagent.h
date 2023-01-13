#ifndef GSAIRIGIDBODY3DAGENT_H
#define GSAIRIGIDBODY3DAGENT_H


class GSAIRigidBody3DAgent : public GSAISpecializedAgent {
 GDCLASS(GSAIRigidBody3DAgent, GSAISpecializedAgent);

 public:

 RigidBody get_*body();
 void set_*body(const RigidBody &val);

 Vector3 get__last_position();
 void set__last_position(const Vector3 &val);

 Ref<WeakRef> get__body_ref();
 void set__body_ref(const Ref<WeakRef> &val);

 void _body_ready();
 void _apply_steering(const GSAITargetAcceleration &acceleration, const float _delta);
 void _set_body(const RigidBody &value);
 void _on_SceneTree_frame();

 GSAIRigidBody3DAgent();
 ~GSAIRigidBody3DAgent();

 protected:
 static void _bind_methods();

 // A specialized steering agent that updates itself every frame so the user does
 // not have to using a RigidBody
 // @category - Specialized agents
 // The RigidBody to keep track of
 // setget _set_body
 RigidBody *body;
 Vector3 _last_position = ;
 Ref<WeakRef> _body_ref;
 // Moves the agent's `body` by target `acceleration`.
 // @tags - virtual
};


#endif
