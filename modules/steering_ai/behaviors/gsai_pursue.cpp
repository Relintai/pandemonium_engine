
#include "gsai_pursue.h"


GSAISteeringAgent GSAIPursue::get_*target() {
 return *target;
}

void GSAIPursue::set_*target(const GSAISteeringAgent &val) {
*target = val;
}


float GSAIPursue::get_predict_time_max() const {
 return predict_time_max;
}

void GSAIPursue::set_predict_time_max(const float val) {
predict_time_max = val;
}



 // Calculates an acceleration to make an agent intercept another based on the;
 // target agent's movement.;
 // @category - Individual behaviors;
 // The target agent that the behavior is trying to intercept.;
  GSAISteeringAgent *target;
 // The maximum amount of time in the future the behavior predicts the target's;
 // location.;
  float predict_time_max = 1.0;

 void GSAIPursue::_calculate_steering(const GSAITargetAcceleration &acceleration) {
   Vector3 target_position = target.position;
   float distance_squared = (target_position - agent.position).length_squared();
   float speed_squared = agent.linear_velocity.length_squared();
   float predict_time = predict_time_max;

  if (speed_squared > 0) {
    Variant predict_time_squared = distance_squared / speed_squared;

   if (predict_time_squared < predict_time_max * predict_time_max) {
    predict_time = sqrt(predict_time_squared);
}

}

  acceleration.linear = ((target_position + (target.linear_velocity * predict_time)) - agent.position).normalized();
  acceleration.linear *= get_modified_acceleration();
  acceleration.angular = 0;
}


 float GSAIPursue::get_modified_acceleration() {
  return call("_get_modified_acceleration");
}


 float GSAIPursue::_get_modified_acceleration() {
  return agent.linear_acceleration_max;
}

}

 GSAIPursue::GSAIPursue() {
  *target;
  predict_time_max = 1.0;
 }

 GSAIPursue::~GSAIPursue() {
 }


 static void GSAIPursue::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_*target"), &GSAIPursue::get_*target);
   ClassDB::bind_method(D_METHOD("set_*target", "value"), &GSAIPursue::set_*target);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*target", PROPERTY_HINT_RESOURCE_TYPE, "GSAISteeringAgent"), "set_*target", "get_*target");


   ClassDB::bind_method(D_METHOD("get_predict_time_max"), &GSAIPursue::get_predict_time_max);
   ClassDB::bind_method(D_METHOD("set_predict_time_max", "value"), &GSAIPursue::set_predict_time_max);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "predict_time_max"), "set_predict_time_max", "get_predict_time_max");


  ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAIPursue::_calculate_steering);
  ClassDB::bind_method(D_METHOD("get_modified_acceleration"), &GSAIPursue::get_modified_acceleration);
  ClassDB::bind_method(D_METHOD("_get_modified_acceleration"), &GSAIPursue::_get_modified_acceleration);

 }



