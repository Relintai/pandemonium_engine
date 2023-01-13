
#include "gsai_priority.h"


float GSAIPriority::get_zero_threshold() const {
 return zero_threshold;
}

void GSAIPriority::set_zero_threshold(const float val) {
zero_threshold = val;
}


int GSAIPriority::get__last_selected_index() const {
 return _last_selected_index;
}

void GSAIPriority::set__last_selected_index(const int val) {
_last_selected_index = val;
}


Array GSAIPriority::get__behaviors() {
 return _behaviors;
}

void GSAIPriority::set__behaviors(const Array &val) {
_behaviors = val;
}



 // Container for multiple behaviors that returns the result of the first child;
 // behavior with non-zero acceleration.;
 // @category - Combination behaviors;
 // If a behavior's acceleration is lower than this threshold, the container;
 // considers it has an acceleration of zero.;
  float zero_threshold = 0.0;
 // The index of the last behavior the container prioritized.;
  int _last_selected_index = 0;
  Array _behaviors = Array();
 // Appends a steering behavior as a child of this container.;

 void GSAIPriority::add_behavior(const GSAISteeringBehavior &behavior) {
  _behaviors.append(behavior);
}

 // Returns the behavior at the position in the pool referred to by `index`, or;
 // `null` if no behavior was found.;

 GSAISteeringBehavior GSAIPriority::get_behavior(const int index) {

  if (_behaviors.size() > index) {
   return _behaviors[index];
}

  printerr("Tried to get index " + str(index) + " in array of size " + str(_behaviors.size()));
  return null;
}


 void GSAIPriority::remove_behavior(const int index) {

  if (_behaviors.size() > index) {
   _behaviors.remove(index);
   return;
}

  printerr("Tried to get index " + str(index) + " in array of size " + str(_behaviors.size()));
  return;
}


 int GSAIPriority::get_behaviour_count() {
  return _behaviors.size();
}


 void GSAIPriority::_calculate_steering(const GSAITargetAcceleration &accel) {
   float threshold_squared = zero_threshold * zero_threshold;
  _last_selected_index = -1;
   int size = _behaviors.size();

  if (size > 0) {

   for (int i = 0; i < size; ++i) { //i in range(size)
    _last_selected_index = i;
     GSAISteeringBehavior *behavior = _behaviors[i];
    behavior.calculate_steering(accel);

    if (accel.get_magnitude_squared() > threshold_squared) {
     break;
}

}

}


  else {
   accel.set_zero();
}

}

}

 GSAIPriority::GSAIPriority() {
  zero_threshold = 0.0;
  _last_selected_index = 0;
  _behaviors = Array();
 }

 GSAIPriority::~GSAIPriority() {
 }


 static void GSAIPriority::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_zero_threshold"), &GSAIPriority::get_zero_threshold);
   ClassDB::bind_method(D_METHOD("set_zero_threshold", "value"), &GSAIPriority::set_zero_threshold);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "zero_threshold"), "set_zero_threshold", "get_zero_threshold");


   ClassDB::bind_method(D_METHOD("get__last_selected_index"), &GSAIPriority::get__last_selected_index);
   ClassDB::bind_method(D_METHOD("set__last_selected_index", "value"), &GSAIPriority::set__last_selected_index);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "_last_selected_index"), "set__last_selected_index", "get__last_selected_index");


   ClassDB::bind_method(D_METHOD("get__behaviors"), &GSAIPriority::get__behaviors);
   ClassDB::bind_method(D_METHOD("set__behaviors", "value"), &GSAIPriority::set__behaviors);
   ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "_behaviors"), "set__behaviors", "get__behaviors");


  ClassDB::bind_method(D_METHOD("add_behavior", "behavior"), &GSAIPriority::add_behavior);
  ClassDB::bind_method(D_METHOD("get_behavior", "index"), &GSAIPriority::get_behavior);
  ClassDB::bind_method(D_METHOD("remove_behavior", "index"), &GSAIPriority::remove_behavior);
  ClassDB::bind_method(D_METHOD("get_behaviour_count"), &GSAIPriority::get_behaviour_count);
  ClassDB::bind_method(D_METHOD("_calculate_steering", "accel"), &GSAIPriority::_calculate_steering);

 }



