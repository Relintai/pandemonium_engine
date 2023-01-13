
#include "gsaimatchorientation.h"


GSAIAgentLocation GSAIMatchOrientation::get_*target() {
 return *target;
}

void GSAIMatchOrientation::set_*target(const GSAIAgentLocation &val) {
*target = val;
}


float GSAIMatchOrientation::get_alignment_tolerance() const {
 return alignment_tolerance;
}

void GSAIMatchOrientation::set_alignment_tolerance(const float val) {
alignment_tolerance = val;
}


float GSAIMatchOrientation::get_deceleration_radius() const {
 return deceleration_radius;
}

void GSAIMatchOrientation::set_deceleration_radius(const float val) {
deceleration_radius = val;
}


float GSAIMatchOrientation::get_time_to_reach() const {
 return time_to_reach;
}

void GSAIMatchOrientation::set_time_to_reach(const float val) {
time_to_reach = val;
}


bool GSAIMatchOrientation::get_use_z() const {
 return use_z;
}

void GSAIMatchOrientation::set_use_z(const bool val) {
use_z = val;
}



 // Calculates an angular acceleration to match an agent's orientation to that of;
 // its target. Attempts to make the agent arrive with zero remaining angular;
 // velocity.;
 // @category - Individual behaviors;
 // The target orientation for the behavior to try and match rotations to.;
  GSAIAgentLocation *target;
 // The amount of distance in radians for the behavior to consider itself close;
 // enough to be matching the target agent's rotation.;
  float alignment_tolerance = 0.0;
 // The amount of distance in radians from the goal to start slowing down.;
  float deceleration_radius = 0.0;
 // The amount of time to reach the target velocity;
  float time_to_reach = 0.1;
 // Whether to use the X and Z components instead of X and Y components when;
 // determining angles. X and Z should be used in 3D.;
  bool use_z = false;

 void GSAIMatchOrientation::match_orientation(const GSAITargetAcceleration &acceleration, const float desired_orientation) {
  call("_match_orientation", acceleration, desired_orientation);
}


 void GSAIMatchOrientation::_match_orientation(const GSAITargetAcceleration &acceleration, const float desired_orientation) {
   float rotation = wrapf(desired_orientation - agent.orientation, -PI, PI);
   float rotation_size = abs(rotation);

  if (rotation_size <= alignment_tolerance) {
   acceleration.set_zero();
}


  else {
    float desired_rotation = agent.angular_speed_max;

   if (rotation_size <= deceleration_radius) {
    desired_rotation *= rotation_size / deceleration_radius;
}

   desired_rotation *= rotation / rotation_size;
   acceleration.angular = ((desired_rotation - agent.angular_velocity) / time_to_reach);
    float limited_acceleration = abs(acceleration.angular);

   if (limited_acceleration > agent.angular_acceleration_max) {
    acceleration.angular *= (agent.angular_acceleration_max / limited_acceleration);
}

}

  acceleration.linear = Vector3.ZERO;
}


 void GSAIMatchOrientation::_calculate_steering(const GSAITargetAcceleration &acceleration) {
  match_orientation(acceleration, target.orientation);
}

}

 GSAIMatchOrientation::GSAIMatchOrientation() {
  *target;
  alignment_tolerance = 0.0;
  deceleration_radius = 0.0;
  time_to_reach = 0.1;
  use_z = false;
 }

 GSAIMatchOrientation::~GSAIMatchOrientation() {
 }


 static void GSAIMatchOrientation::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_*target"), &GSAIMatchOrientation::get_*target);
   ClassDB::bind_method(D_METHOD("set_*target", "value"), &GSAIMatchOrientation::set_*target);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*target", PROPERTY_HINT_RESOURCE_TYPE, "GSAIAgentLocation"), "set_*target", "get_*target");


   ClassDB::bind_method(D_METHOD("get_alignment_tolerance"), &GSAIMatchOrientation::get_alignment_tolerance);
   ClassDB::bind_method(D_METHOD("set_alignment_tolerance", "value"), &GSAIMatchOrientation::set_alignment_tolerance);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "alignment_tolerance"), "set_alignment_tolerance", "get_alignment_tolerance");


   ClassDB::bind_method(D_METHOD("get_deceleration_radius"), &GSAIMatchOrientation::get_deceleration_radius);
   ClassDB::bind_method(D_METHOD("set_deceleration_radius", "value"), &GSAIMatchOrientation::set_deceleration_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "deceleration_radius"), "set_deceleration_radius", "get_deceleration_radius");


   ClassDB::bind_method(D_METHOD("get_time_to_reach"), &GSAIMatchOrientation::get_time_to_reach);
   ClassDB::bind_method(D_METHOD("set_time_to_reach", "value"), &GSAIMatchOrientation::set_time_to_reach);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "time_to_reach"), "set_time_to_reach", "get_time_to_reach");


   ClassDB::bind_method(D_METHOD("get_use_z"), &GSAIMatchOrientation::get_use_z);
   ClassDB::bind_method(D_METHOD("set_use_z", "value"), &GSAIMatchOrientation::set_use_z);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_z"), "set_use_z", "get_use_z");


  ClassDB::bind_method(D_METHOD("match_orientation", "acceleration", "desired_orientation"), &GSAIMatchOrientation::match_orientation);
  ClassDB::bind_method(D_METHOD("_match_orientation", "acceleration", "desired_orientation"), &GSAIMatchOrientation::_match_orientation);
  ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAIMatchOrientation::_calculate_steering);

 }



