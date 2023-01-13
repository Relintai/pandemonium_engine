
#include "gsai_cohesion.h"


Vector3 GSAICohesion::get__center_of_mass() {
 return _center_of_mass;
}

void GSAICohesion::set__center_of_mass(const Vector3 &val) {
_center_of_mass = val;
}



 // Calculates an acceleration that attempts to move the agent towards the center;
 // of mass of the agents in the area defined by the `GSAIProximity`.;
 // @category - Group behaviors;
  Vector3 _center_of_mass = ;

 void GSAICohesion::_calculate_steering(const GSAITargetAcceleration &acceleration) {
  acceleration.set_zero();
  _center_of_mass = Vector3.ZERO;
   Variant  = proximity.find_neighbors(_callback);

  if (neighbor_count > 0) {
   _center_of_mass *= 1.0 / neighbor_count;
   acceleration.linear = ((_center_of_mass - agent.position).normalized() * agent.linear_acceleration_max);
}

  // Callback for the proximity to call when finding neighbors. Adds `neighbor`'s position;
  // to the center of mass of the group.;
  // @tags - virtual;
}


 bool GSAICohesion::_report_neighbor(const GSAISteeringAgent &neighbor) {
  _center_of_mass += neighbor.position;
  return true;
}

}

 GSAICohesion::GSAICohesion() {
  _center_of_mass = ;
 }

 GSAICohesion::~GSAICohesion() {
 }


 static void GSAICohesion::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get__center_of_mass"), &GSAICohesion::get__center_of_mass);
   ClassDB::bind_method(D_METHOD("set__center_of_mass", "value"), &GSAICohesion::set__center_of_mass);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "_center_of_mass"), "set__center_of_mass", "get__center_of_mass");


  ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAICohesion::_calculate_steering);
  ClassDB::bind_method(D_METHOD("_report_neighbor", "neighbor"), &GSAICohesion::_report_neighbor);

 }



