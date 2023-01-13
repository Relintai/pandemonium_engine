
#include "gsai_steering_behavior.h"


bool GSAISteeringBehavior::get_is_enabled() const {
 return is_enabled;
}

void GSAISteeringBehavior::set_is_enabled(const bool val) {
is_enabled = val;
}


GSAISteeringAgent GSAISteeringBehavior::get_*agent() {
 return *agent;
}

void GSAISteeringBehavior::set_*agent(const GSAISteeringAgent &val) {
*agent = val;
}



 // Base class for all steering behaviors.;
 //;
 // Steering behaviors calculate the linear and the angular acceleration to be;
 // to the agent that owns them.;
 //;
 // The `calculate_steering` function is the entry point for all behaviors.;
 // Individual steering behaviors encapsulate the steering logic.;
 // @category - Base types;
 // If `false`, all calculations return zero amounts of acceleration.;
  bool is_enabled = true;
 // The AI agent on which the steering behavior bases its calculations.;
  GSAISteeringAgent *agent;
 // Sets the `acceleration` with the behavior's desired amount of acceleration.;

 void GSAISteeringBehavior::calculate_steering(const GSAITargetAcceleration &acceleration) {

  if (is_enabled) {
   call("_calculate_steering", acceleration);
}


  else {
   acceleration.set_zero();
}

}


 void GSAISteeringBehavior::_calculate_steering(const GSAITargetAcceleration &acceleration) {
  acceleration.set_zero();
}

}

 GSAISteeringBehavior::GSAISteeringBehavior() {
  is_enabled = true;
  *agent;
 }

 GSAISteeringBehavior::~GSAISteeringBehavior() {
 }


 static void GSAISteeringBehavior::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_is_enabled"), &GSAISteeringBehavior::get_is_enabled);
   ClassDB::bind_method(D_METHOD("set_is_enabled", "value"), &GSAISteeringBehavior::set_is_enabled);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_enabled"), "set_is_enabled", "get_is_enabled");


   ClassDB::bind_method(D_METHOD("get_*agent"), &GSAISteeringBehavior::get_*agent);
   ClassDB::bind_method(D_METHOD("set_*agent", "value"), &GSAISteeringBehavior::set_*agent);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*agent", PROPERTY_HINT_RESOURCE_TYPE, "GSAISteeringAgent"), "set_*agent", "get_*agent");


  ClassDB::bind_method(D_METHOD("calculate_steering", "acceleration"), &GSAISteeringBehavior::calculate_steering);
  ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAISteeringBehavior::_calculate_steering);

 }



