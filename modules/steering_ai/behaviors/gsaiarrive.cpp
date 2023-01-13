
#include "gsaiarrive.h"


GSAIAgentLocation GSAIArrive::get_*target() {
 return *target;
}

void GSAIArrive::set_*target(const GSAIAgentLocation &val) {
*target = val;
}


float GSAIArrive::get_arrival_tolerance() const {
 return arrival_tolerance;
}

void GSAIArrive::set_arrival_tolerance(const float val) {
arrival_tolerance = val;
}


float GSAIArrive::get_deceleration_radius() const {
 return deceleration_radius;
}

void GSAIArrive::set_deceleration_radius(const float val) {
deceleration_radius = val;
}


float GSAIArrive::get_time_to_reach() const {
 return time_to_reach;
}

void GSAIArrive::set_time_to_reach(const float val) {
time_to_reach = val;
}



 // Calculates acceleration to take an agent to its target's location. The;
 // calculation attempts to arrive with zero remaining velocity.;
 // @category - Individual behaviors;
 // Target agent to arrive to.;
  GSAIAgentLocation *target;
 // Distance from the target for the agent to be considered successfully;
 // arrived.;
  float arrival_tolerance = 0.0;
 // Distance from the target for the agent to begin slowing down.;
  float deceleration_radius = 0.0;
 // Represents the time it takes to change acceleration.;
  float time_to_reach = 0.1;

 void GSAIArrive::arrive(const GSAITargetAcceleration &acceleration, const Vector3 &target_position) {
  call("_arrive", acceleration, target_position);
}


 void GSAIArrive::_arrive(const GSAITargetAcceleration &acceleration, const Vector3 &target_position) {
   Vector3 to_target = target_position - agent.position;
   float distance = to_target.length();

  if (distance <= arrival_tolerance) {
   acceleration.set_zero();
}


  else {
    float desired_speed = agent.linear_speed_max;

   if (distance <= deceleration_radius) {
    desired_speed *= distance / deceleration_radius;
}

    Vector3 desired_velocity = to_target * desired_speed / distance;
   desired_velocity = ((desired_velocity - agent.linear_velocity) * 1.0 / time_to_reach);
   acceleration.linear = GSAIUtils.clampedv3(desired_velocity, agent.linear_acceleration_max);
   acceleration.angular = 0;
}

}


 void GSAIArrive::_calculate_steering(const GSAITargetAcceleration &acceleration) {
  arrive(acceleration, target.position);
}

}

 GSAIArrive::GSAIArrive() {
  *target;
  arrival_tolerance = 0.0;
  deceleration_radius = 0.0;
  time_to_reach = 0.1;
 }

 GSAIArrive::~GSAIArrive() {
 }


 static void GSAIArrive::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_*target"), &GSAIArrive::get_*target);
   ClassDB::bind_method(D_METHOD("set_*target", "value"), &GSAIArrive::set_*target);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*target", PROPERTY_HINT_RESOURCE_TYPE, "GSAIAgentLocation"), "set_*target", "get_*target");


   ClassDB::bind_method(D_METHOD("get_arrival_tolerance"), &GSAIArrive::get_arrival_tolerance);
   ClassDB::bind_method(D_METHOD("set_arrival_tolerance", "value"), &GSAIArrive::set_arrival_tolerance);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "arrival_tolerance"), "set_arrival_tolerance", "get_arrival_tolerance");


   ClassDB::bind_method(D_METHOD("get_deceleration_radius"), &GSAIArrive::get_deceleration_radius);
   ClassDB::bind_method(D_METHOD("set_deceleration_radius", "value"), &GSAIArrive::set_deceleration_radius);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "deceleration_radius"), "set_deceleration_radius", "get_deceleration_radius");


   ClassDB::bind_method(D_METHOD("get_time_to_reach"), &GSAIArrive::get_time_to_reach);
   ClassDB::bind_method(D_METHOD("set_time_to_reach", "value"), &GSAIArrive::set_time_to_reach);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "time_to_reach"), "set_time_to_reach", "get_time_to_reach");


  ClassDB::bind_method(D_METHOD("arrive", "acceleration", "target_position"), &GSAIArrive::arrive);
  ClassDB::bind_method(D_METHOD("_arrive", "acceleration", "target_position"), &GSAIArrive::_arrive);
  ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAIArrive::_calculate_steering);

 }



