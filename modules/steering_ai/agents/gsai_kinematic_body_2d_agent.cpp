
#include "gsai_kinematic_body_2d_agent.h"


KinematicBody2D GSAIKinematicBody2DAgent::get_*body() {
 return *body;
}

void GSAIKinematicBody2DAgent::set_*body(const KinematicBody2D &val) {
*body = val;
}


int GSAIKinematicBody2DAgent::get_movement_type() const {
 return movement_type;
}

void GSAIKinematicBody2DAgent::set_movement_type(const int val) {
movement_type = val;
}


Vector2 GSAIKinematicBody2DAgent::get__last_position() {
 return _last_position;
}

void GSAIKinematicBody2DAgent::set__last_position(const Vector2 &val) {
_last_position = val;
}


Ref<WeakRef> GSAIKinematicBody2DAgent::get__body_ref() {
 return _body_ref;
}

void GSAIKinematicBody2DAgent::set__body_ref(const Ref<WeakRef> &val) {
_body_ref = val;
}



 // A specialized steering agent that updates itself every frame so the user does;
 // not have to using a KinematicBody2D;
 // @category - Specialized agents;
 // SLIDE uses `move_and_slide`;
 // COLLIDE uses `move_and_collide`;
 // POSITION changes the `global_position` directly;
 };
 // The KinematicBody2D to keep track of;
 // setget _set_body;
  KinematicBody2D *body;
 // The type of movement the body executes;
  int movement_type = MovementType.SLIDE;
  Vector2 _last_position = ;
  Ref<WeakRef> _body_ref;

 void GSAIKinematicBody2DAgent::_body_ready() {
  // warning-ignore:return_value_discarded;
  body.get_tree().connect("physics_frame", self, "_on_SceneTree_physics_frame");
}

 // Moves the agent's `body` by target `acceleration`.;
 // @tags - virtual;

 void GSAIKinematicBody2DAgent::_apply_steering(const GSAITargetAcceleration &acceleration, const float delta) {
  applied_steering = true;

  if (movement_type == MovementType.COLLIDE) {
   _apply_collide_steering(acceleration.linear, delta);
}


  else if (movement_type == MovementType.SLIDE) {
   _apply_sliding_steering(acceleration.linear, delta);
}


  else {
   _apply_position_steering(acceleration.linear, delta);
}

  _apply_orientation_steering(acceleration.angular, delta);
}


 void GSAIKinematicBody2DAgent::_apply_sliding_steering(const Vector3 &accel, const float delta) {
   KinematicBody2D *_body = _body_ref.get_ref();

  if (!_body) {
   return;
}


  if (!_body.is_inside_tree() || _body.get_tree().paused) {
   return;
}

   Vector2 velocity = GSAIUtils.to_vector2(linear_velocity + accel * delta).clamped(linear_speed_max);

  if (apply_linear_drag) {
   velocity = velocity.linear_interpolate(Vector2.ZERO, linear_drag_percentage);
}

  velocity = _body.move_and_slide(velocity);

  if (calculate_velocities) {
   linear_velocity = GSAIUtils.to_vector3(velocity);
}

}


 void GSAIKinematicBody2DAgent::_apply_collide_steering(const Vector3 &accel, const float delta) {
   KinematicBody2D *_body = _body_ref.get_ref();

  if (!_body) {
   return;
}

   Vector3 velocity = GSAIUtils.clampedv3(linear_velocity + accel * delta, linear_speed_max);

  if (apply_linear_drag) {
   velocity = velocity.linear_interpolate(Vector3.ZERO, linear_drag_percentage);
}

  // warning-ignore:return_value_discarded;
  _body.move_and_collide(GSAIUtils.to_vector2(velocity) * delta);

  if (calculate_velocities) {
   linear_velocity = velocity;
}

}


 void GSAIKinematicBody2DAgent::_apply_position_steering(const Vector3 &accel, const float delta) {
   KinematicBody2D *_body = _body_ref.get_ref();

  if (!_body) {
   return;
}

   Vector3 velocity = GSAIUtils.clampedv3(linear_velocity + accel * delta, linear_speed_max);

  if (apply_linear_drag) {
   velocity = velocity.linear_interpolate(Vector3.ZERO, linear_drag_percentage);
}

  _body.global_position += GSAIUtils.to_vector2(velocity) * delta;

  if (calculate_velocities) {
   linear_velocity = velocity;
}

}


 void GSAIKinematicBody2DAgent::_apply_orientation_steering(const float angular_acceleration, const float delta) {
   KinematicBody2D *_body = _body_ref.get_ref();

  if (!_body) {
   return;
}

   Variant  = clamp(angular_velocity + angular_acceleration * delta, -angular_speed_max, angular_speed_max);

  if (apply_angular_drag) {
   velocity = lerp(velocity, 0, angular_drag_percentage);
}

  _body.rotation += velocity * delta;

  if (calculate_velocities) {
   angular_velocity = velocity;
}

}


 void GSAIKinematicBody2DAgent::_set_body(const KinematicBody2D &value) {
   bool had_body = false;

  if (body) {
   had_body = true;
}

  body = value;
  _body_ref = weakref(body);
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


 void GSAIKinematicBody2DAgent::_on_SceneTree_physics_frame() {
   KinematicBody2D *_body = _body_ref.get_ref();

  if (!_body) {
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
    linear_velocity = GSAIUtils.clampedv3(GSAIUtils.to_vector3(current_position - _last_position), linear_speed_max);

    if (apply_linear_drag) {
     linear_velocity = linear_velocity.linear_interpolate(Vector3.ZERO, linear_drag_percentage);
}

    angular_velocity = clamp(last_orientation - current_orientation, -angular_speed_max, angular_speed_max);

    if (apply_angular_drag) {
     angular_velocity = lerp(angular_velocity, 0, angular_drag_percentage);
}

    _last_position = current_position;
    last_orientation = current_orientation;
}

}

}

}

 GSAIKinematicBody2DAgent::GSAIKinematicBody2DAgent() {
  *body;
  movement_type = MovementType.SLIDE;
  _last_position = ;
  _body_ref;
 }

 GSAIKinematicBody2DAgent::~GSAIKinematicBody2DAgent() {
 }


 static void GSAIKinematicBody2DAgent::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_*body"), &GSAIKinematicBody2DAgent::get_*body);
   ClassDB::bind_method(D_METHOD("set_*body", "value"), &GSAIKinematicBody2DAgent::set_*body);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*body", PROPERTY_HINT_RESOURCE_TYPE, "KinematicBody2D"), "set_*body", "get_*body");


   ClassDB::bind_method(D_METHOD("get_movement_type"), &GSAIKinematicBody2DAgent::get_movement_type);
   ClassDB::bind_method(D_METHOD("set_movement_type", "value"), &GSAIKinematicBody2DAgent::set_movement_type);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "movement_type"), "set_movement_type", "get_movement_type");


   ClassDB::bind_method(D_METHOD("get__last_position"), &GSAIKinematicBody2DAgent::get__last_position);
   ClassDB::bind_method(D_METHOD("set__last_position", "value"), &GSAIKinematicBody2DAgent::set__last_position);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "_last_position"), "set__last_position", "get__last_position");


   ClassDB::bind_method(D_METHOD("get__body_ref"), &GSAIKinematicBody2DAgent::get__body_ref);
   ClassDB::bind_method(D_METHOD("set__body_ref", "value"), &GSAIKinematicBody2DAgent::set__body_ref);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "_body_ref", PROPERTY_HINT_RESOURCE_TYPE, "Ref<WeakRef>"), "set__body_ref", "get__body_ref");


  ClassDB::bind_method(D_METHOD("_body_ready"), &GSAIKinematicBody2DAgent::_body_ready);
  ClassDB::bind_method(D_METHOD("_apply_steering", "acceleration", "delta"), &GSAIKinematicBody2DAgent::_apply_steering);
  ClassDB::bind_method(D_METHOD("_apply_sliding_steering", "accel", "delta"), &GSAIKinematicBody2DAgent::_apply_sliding_steering);
  ClassDB::bind_method(D_METHOD("_apply_collide_steering", "accel", "delta"), &GSAIKinematicBody2DAgent::_apply_collide_steering);
  ClassDB::bind_method(D_METHOD("_apply_position_steering", "accel", "delta"), &GSAIKinematicBody2DAgent::_apply_position_steering);
  ClassDB::bind_method(D_METHOD("_apply_orientation_steering", "angular_acceleration", "delta"), &GSAIKinematicBody2DAgent::_apply_orientation_steering);
  ClassDB::bind_method(D_METHOD("_set_body", "value"), &GSAIKinematicBody2DAgent::_set_body);
  ClassDB::bind_method(D_METHOD("_on_SceneTree_physics_frame"), &GSAIKinematicBody2DAgent::_on_SceneTree_physics_frame);

 }



