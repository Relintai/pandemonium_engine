
#include "gsai_rigid_body_2d_agent.h"


RigidBody2D GSAIRigidBody2DAgent::get_*body() {
 return *body;
}

void GSAIRigidBody2DAgent::set_*body(const RigidBody2D &val) {
*body = val;
}


Vector2 GSAIRigidBody2DAgent::get__last_position() {
 return _last_position;
}

void GSAIRigidBody2DAgent::set__last_position(const Vector2 &val) {
_last_position = val;
}


Ref<WeakRef> GSAIRigidBody2DAgent::get__body_ref() {
 return _body_ref;
}

void GSAIRigidBody2DAgent::set__body_ref(const Ref<WeakRef> &val) {
_body_ref = val;
}



 // A specialized steering agent that updates itself every frame so the user does;
 // not have to using a RigidBody2D;
 // @category - Specialized agents;
 // The RigidBody2D to keep track of;
 // setget _set_body;
  RigidBody2D *body;
  Vector2 _last_position = ;
  Ref<WeakRef> _body_ref;

 void GSAIRigidBody2DAgent::_body_ready() {
  // warning-ignore:return_value_discarded;
  body.get_tree().connect("physics_frame", self, "_on_SceneTree_frame");
}

 // Moves the agent's `body` by target `acceleration`.;
 // @tags - virtual;

 void GSAIRigidBody2DAgent::_apply_steering(const GSAITargetAcceleration &acceleration, const float _delta) {
   RigidBody2D *_body = _body_ref.get_ref();

  if (not _body) {
   return;
}

  applied_steering = true;
  _body.apply_central_impulse(GSAIUtils.to_vector2(acceleration.linear));
  _body.apply_torque_impulse(acceleration.angular);

  if (calculate_velocities) {
   linear_velocity = GSAIUtils.to_vector3(_body.linear_velocity);
   angular_velocity = _body.angular_velocity;
}

}


 void GSAIRigidBody2DAgent::_set_body(const RigidBody2D &value) {
   bool had_body = false;

  if (body) {
   had_body = true;
}

  body = value;
  _body_ref = weakref(value);
  _last_position = value.global_position;
  last_orientation = value.rotation;
  position = GSAIUtils.to_vector3(_last_position);
  orientation = last_orientation;

  if (!had_body) {

   if (!body.is_inside_tree()) {
    body.connect("ready", self, "_body_ready");
}


   else {
    _body_ready();
}

}

}


 void GSAIRigidBody2DAgent::_on_SceneTree_frame() {
   RigidBody2D *_body = _body_ref.get_ref();

  if (!_body) {
   return;
}


  if (!_body.is_inside_tree() || _body.get_tree().paused) {
   return;
}

   Vector2 current_position = _body.global_position;
   float current_orientation = _body.rotation;
  position = GSAIUtils.to_vector3(current_position);
  orientation = current_orientation;

  if (calculate_velocities) {

   if (applied_steering) {
    applied_steering = false;
}


   else {
    linear_velocity = GSAIUtils.to_vector3(_body.linear_velocity);
    angular_velocity = _body.angular_velocity;
}

}

}

}

 GSAIRigidBody2DAgent::GSAIRigidBody2DAgent() {
  *body;
  _last_position = ;
  _body_ref;
 }

 GSAIRigidBody2DAgent::~GSAIRigidBody2DAgent() {
 }


 static void GSAIRigidBody2DAgent::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_*body"), &GSAIRigidBody2DAgent::get_*body);
   ClassDB::bind_method(D_METHOD("set_*body", "value"), &GSAIRigidBody2DAgent::set_*body);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*body", PROPERTY_HINT_RESOURCE_TYPE, "RigidBody2D"), "set_*body", "get_*body");


   ClassDB::bind_method(D_METHOD("get__last_position"), &GSAIRigidBody2DAgent::get__last_position);
   ClassDB::bind_method(D_METHOD("set__last_position", "value"), &GSAIRigidBody2DAgent::set__last_position);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "_last_position"), "set__last_position", "get__last_position");


   ClassDB::bind_method(D_METHOD("get__body_ref"), &GSAIRigidBody2DAgent::get__body_ref);
   ClassDB::bind_method(D_METHOD("set__body_ref", "value"), &GSAIRigidBody2DAgent::set__body_ref);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "_body_ref", PROPERTY_HINT_RESOURCE_TYPE, "Ref<WeakRef>"), "set__body_ref", "get__body_ref");


  ClassDB::bind_method(D_METHOD("_body_ready"), &GSAIRigidBody2DAgent::_body_ready);
  ClassDB::bind_method(D_METHOD("_apply_steering", "acceleration", "_delta"), &GSAIRigidBody2DAgent::_apply_steering);
  ClassDB::bind_method(D_METHOD("_set_body", "value"), &GSAIRigidBody2DAgent::_set_body);
  ClassDB::bind_method(D_METHOD("_on_SceneTree_frame"), &GSAIRigidBody2DAgent::_on_SceneTree_frame);

 }



