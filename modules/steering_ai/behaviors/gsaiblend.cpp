
#include "gsaiblend.h"


Array GSAIBlend::get__behaviors() {
 return _behaviors;
}

void GSAIBlend::set__behaviors(const Array &val) {
_behaviors = val;
}


GSAITargetAcceleration GSAIBlend::get_*_accel() {
 return *_accel;
}

void GSAIBlend::set_*_accel(const GSAITargetAcceleration &val) {
*_accel = val;
}



 // Blends multiple steering behaviors into one, and returns a weighted;
 // acceleration from their calculations.;
 //;
 // Stores the behaviors internally as dictionaries of the form;
 // {;
 // 	behavior : GSAISteeringBehavior,;
 // 	weight : float;
 // };
 // @category - Combination behaviors;
  Array _behaviors = Array();
  GSAITargetAcceleration *_accel = GSAITargetAcceleration.new();
 // Appends a behavior to the internal array along with its `weight`.;

 void GSAIBlend::add_behavior(const GSAISteeringBehavior &behavior, const float weight) {
  behavior.agent = agent;
   Dictionary dict = Dictionary();
  dict["behavior"] = behavior;
  dict["weight"] = weight;
  _behaviors.append(dict);
}

 // Returns the behavior at the specified `index`, or an empty `Dictionary` if;
 // none was found.;

 Dictionary GSAIBlend::get_behavior(const int index) {

  if (_behaviors.size() > index) {
   return _behaviors[index];
}

  printerr("Tried to get index " + str(index) + " in array of size " + str(_behaviors.size()));
  return Dictionary();
}


 void GSAIBlend::remove_behavior(const int index) {

  if (_behaviors.size() > index) {
   _behaviors.remove(index);
   return;
}

  printerr("Tried to get index " + str(index) + " in array of size " + str(_behaviors.size()));
  return;
}


 int GSAIBlend::get_behaviour_count() {
  return _behaviors.size();
}


 GSAITargetAcceleration GSAIBlend::get_accel() {
  return _accel;
}


 void GSAIBlend::_calculate_steering(const GSAITargetAcceleration &blended_accel) {
  blended_accel.set_zero();

  for (int i = 0; i < _behaviors.size(); ++i) { //i in range(_behaviors.size())
    Dictionary bw = _behaviors[i];
   bw.behavior.calculate_steering(_accel);
   blended_accel.add_scaled_accel(_accel, bw.weight);
}

  blended_accel.linear = GSAIUtils.clampedv3(blended_accel.linear, agent.linear_acceleration_max);
  blended_accel.angular = clamp(blended_accel.angular, -agent.angular_acceleration_max, agent.angular_acceleration_max);
}

}

 GSAIBlend::GSAIBlend() {
  _behaviors = Array();
  *_accel = GSAITargetAcceleration.new();
 }

 GSAIBlend::~GSAIBlend() {
 }


 static void GSAIBlend::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get__behaviors"), &GSAIBlend::get__behaviors);
   ClassDB::bind_method(D_METHOD("set__behaviors", "value"), &GSAIBlend::set__behaviors);
   ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "_behaviors"), "set__behaviors", "get__behaviors");


   ClassDB::bind_method(D_METHOD("get_*_accel"), &GSAIBlend::get_*_accel);
   ClassDB::bind_method(D_METHOD("set_*_accel", "value"), &GSAIBlend::set_*_accel);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_accel", PROPERTY_HINT_RESOURCE_TYPE, "GSAITargetAcceleration"), "set_*_accel", "get_*_accel");


  ClassDB::bind_method(D_METHOD("add_behavior", "behavior", "weight"), &GSAIBlend::add_behavior);
  ClassDB::bind_method(D_METHOD("get_behavior", "index"), &GSAIBlend::get_behavior);
  ClassDB::bind_method(D_METHOD("remove_behavior", "index"), &GSAIBlend::remove_behavior);
  ClassDB::bind_method(D_METHOD("get_behaviour_count"), &GSAIBlend::get_behaviour_count);
  ClassDB::bind_method(D_METHOD("get_accel"), &GSAIBlend::get_accel);
  ClassDB::bind_method(D_METHOD("_calculate_steering", "blended_accel"), &GSAIBlend::_calculate_steering);

 }



