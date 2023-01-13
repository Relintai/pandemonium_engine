
#include "gsaiseparation.h"


float GSAISeparation::get_decay_coefficient() const {
 return decay_coefficient;
}

void GSAISeparation::set_decay_coefficient(const float val) {
decay_coefficient = val;
}


GSAITargetAcceleration GSAISeparation::get_*acceleration() {
 return *acceleration;
}

void GSAISeparation::set_*acceleration(const GSAITargetAcceleration &val) {
*acceleration = val;
}



 // Calculates an acceleration that repels the agent from its neighbors in the;
 // given `GSAIProximity`.;
 //;
 // The acceleration is an average based on all neighbors, multiplied by a;
 // strength decreasing by the inverse square law in relation to distance, and it;
 // accumulates.;
 // @category - Group behaviors;
 // The coefficient to calculate how fast the separation strength decays with distance.;
  float decay_coefficient = 1.0;
  GSAITargetAcceleration *acceleration;

 void GSAISeparation::_calculate_steering(const GSAITargetAcceleration &_acceleration) {
  self.acceleration = _acceleration;
  acceleration.set_zero();
  // warning-ignore:return_value_discarded;
  proximity.find_neighbors(_callback);
}

 // Callback for the proximity to call when finding neighbors. Determines the amount of;
 // acceleration that `neighbor` imposes based on its distance from the owner agent.;
 // @tags - virtual;

 bool GSAISeparation::_report_neighbor(const GSAISteeringAgent &neighbor) {
   Vector3 to_agent = agent.position - neighbor.position;
   float distance_squared = to_agent.length_squared();
   float acceleration_max = agent.linear_acceleration_max;
   Variant strength = decay_coefficient / distance_squared;

  if (strength > acceleration_max) {
   strength = acceleration_max;
}

  acceleration.linear += to_agent * (strength / sqrt(distance_squared));
  return true;
}

}

 GSAISeparation::GSAISeparation() {
  decay_coefficient = 1.0;
  *acceleration;
 }

 GSAISeparation::~GSAISeparation() {
 }


 static void GSAISeparation::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_decay_coefficient"), &GSAISeparation::get_decay_coefficient);
   ClassDB::bind_method(D_METHOD("set_decay_coefficient", "value"), &GSAISeparation::set_decay_coefficient);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "decay_coefficient"), "set_decay_coefficient", "get_decay_coefficient");


   ClassDB::bind_method(D_METHOD("get_*acceleration"), &GSAISeparation::get_*acceleration);
   ClassDB::bind_method(D_METHOD("set_*acceleration", "value"), &GSAISeparation::set_*acceleration);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*acceleration", PROPERTY_HINT_RESOURCE_TYPE, "GSAITargetAcceleration"), "set_*acceleration", "get_*acceleration");


  ClassDB::bind_method(D_METHOD("_calculate_steering", "_acceleration"), &GSAISeparation::_calculate_steering);
  ClassDB::bind_method(D_METHOD("_report_neighbor", "neighbor"), &GSAISeparation::_report_neighbor);

 }



