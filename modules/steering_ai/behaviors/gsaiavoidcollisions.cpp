
#include "gsaiavoidcollisions.h"


GSAISteeringAgent GSAIAvoidCollisions::get_*_first_neighbor() {
 return *_first_neighbor;
}

void GSAIAvoidCollisions::set_*_first_neighbor(const GSAISteeringAgent &val) {
*_first_neighbor = val;
}


float GSAIAvoidCollisions::get__shortest_time() const {
 return _shortest_time;
}

void GSAIAvoidCollisions::set__shortest_time(const float val) {
_shortest_time = val;
}


float GSAIAvoidCollisions::get__first_minimum_separation() const {
 return _first_minimum_separation;
}

void GSAIAvoidCollisions::set__first_minimum_separation(const float val) {
_first_minimum_separation = val;
}


float GSAIAvoidCollisions::get__first_distance() const {
 return _first_distance;
}

void GSAIAvoidCollisions::set__first_distance(const float val) {
_first_distance = val;
}


Vector3 GSAIAvoidCollisions::get__first_relative_position() {
 return _first_relative_position;
}

void GSAIAvoidCollisions::set__first_relative_position(const Vector3 &val) {
_first_relative_position = val;
}


Vector3 GSAIAvoidCollisions::get__first_relative_velocity() {
 return _first_relative_velocity;
}

void GSAIAvoidCollisions::set__first_relative_velocity(const Vector3 &val) {
_first_relative_velocity = val;
}



 // Steers the agent to avoid obstacles in its path. Approximates obstacles as;
 // spheres.;
 // @category - Group behaviors;
  GSAISteeringAgent *_first_neighbor;
  float _shortest_time = 0.0;
  float _first_minimum_separation = 0.0;
  float _first_distance = 0.0;
  Vector3 _first_relative_position = ;
  Vector3 _first_relative_velocity = ;

 void GSAIAvoidCollisions::_calculate_steering(const GSAITargetAcceleration &acceleration) {
  _shortest_time = INF;
  _first_neighbor = null;
  _first_minimum_separation = 0;
  _first_distance = 0;
   int neighbor_count = proximity.find_neighbors(_callback);

  if (neighbor_count == 0 || not _first_neighbor) {
   acceleration.set_zero();
}


  else {

   if ((_first_minimum_separation <= 0 || _first_distance < agent.bounding_radius + _first_neighbor.bounding_radius)) {
    acceleration.linear = _first_neighbor.position - agent.position;
}


   else {
    acceleration.linear = (_first_relative_position+ (_first_relative_velocity * _shortest_time));
}

}

  acceleration.linear = (acceleration.linear.normalized() * -agent.linear_acceleration_max);
  acceleration.angular = 0;
}

 // Callback for the proximity to call when finding neighbors. Keeps track of every `neighbor`;
 // that was found but only keeps the one the owning agent will most likely collide with.;
 // @tags - virtual;

 bool GSAIAvoidCollisions::_report_neighbor(const GSAISteeringAgent &neighbor) {
   Vector3 relative_position = neighbor.position - agent.position;
   Vector3 relative_velocity = neighbor.linear_velocity - agent.linear_velocity;
   float relative_speed_squared = relative_velocity.length_squared();

  if (relative_speed_squared == 0) {
   return false;
}


  else {
    float time_to_collision = -relative_position.dot(relative_velocity) / relative_speed_squared;

   if (time_to_collision <= 0 || time_to_collision >= _shortest_time) {
    return false;
}


   else {
     Variant  = relative_position.length();
     float minimum_separation = (distance - sqrt(relative_speed_squared) * time_to_collision);

    if (minimum_separation > agent.bounding_radius + neighbor.bounding_radius) {
     return false;
}


    else {
     _shortest_time = time_to_collision;
     _first_neighbor = neighbor;
     _first_minimum_separation = minimum_separation;
     _first_distance = distance;
     _first_relative_position = relative_position;
     _first_relative_velocity = relative_velocity;
     return true;
}

}

}

}

}

 GSAIAvoidCollisions::GSAIAvoidCollisions() {
  *_first_neighbor;
  _shortest_time = 0.0;
  _first_minimum_separation = 0.0;
  _first_distance = 0.0;
  _first_relative_position = ;
  _first_relative_velocity = ;
 }

 GSAIAvoidCollisions::~GSAIAvoidCollisions() {
 }


 static void GSAIAvoidCollisions::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_*_first_neighbor"), &GSAIAvoidCollisions::get_*_first_neighbor);
   ClassDB::bind_method(D_METHOD("set_*_first_neighbor", "value"), &GSAIAvoidCollisions::set_*_first_neighbor);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_first_neighbor", PROPERTY_HINT_RESOURCE_TYPE, "GSAISteeringAgent"), "set_*_first_neighbor", "get_*_first_neighbor");


   ClassDB::bind_method(D_METHOD("get__shortest_time"), &GSAIAvoidCollisions::get__shortest_time);
   ClassDB::bind_method(D_METHOD("set__shortest_time", "value"), &GSAIAvoidCollisions::set__shortest_time);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "_shortest_time"), "set__shortest_time", "get__shortest_time");


   ClassDB::bind_method(D_METHOD("get__first_minimum_separation"), &GSAIAvoidCollisions::get__first_minimum_separation);
   ClassDB::bind_method(D_METHOD("set__first_minimum_separation", "value"), &GSAIAvoidCollisions::set__first_minimum_separation);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "_first_minimum_separation"), "set__first_minimum_separation", "get__first_minimum_separation");


   ClassDB::bind_method(D_METHOD("get__first_distance"), &GSAIAvoidCollisions::get__first_distance);
   ClassDB::bind_method(D_METHOD("set__first_distance", "value"), &GSAIAvoidCollisions::set__first_distance);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "_first_distance"), "set__first_distance", "get__first_distance");


   ClassDB::bind_method(D_METHOD("get__first_relative_position"), &GSAIAvoidCollisions::get__first_relative_position);
   ClassDB::bind_method(D_METHOD("set__first_relative_position", "value"), &GSAIAvoidCollisions::set__first_relative_position);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "_first_relative_position"), "set__first_relative_position", "get__first_relative_position");


   ClassDB::bind_method(D_METHOD("get__first_relative_velocity"), &GSAIAvoidCollisions::get__first_relative_velocity);
   ClassDB::bind_method(D_METHOD("set__first_relative_velocity", "value"), &GSAIAvoidCollisions::set__first_relative_velocity);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "_first_relative_velocity"), "set__first_relative_velocity", "get__first_relative_velocity");


  ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAIAvoidCollisions::_calculate_steering);
  ClassDB::bind_method(D_METHOD("_report_neighbor", "neighbor"), &GSAIAvoidCollisions::_report_neighbor);

 }



