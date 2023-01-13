
#include "gsai_look_where_you_go.h"



 // Calculates an angular acceleration to match an agent's orientation to its;
 // direction of travel.;
 // @category - Individual behaviors;

 void GSAILookWhereYouGo::_calculate_steering(const GSAITargetAcceleration &accel) {

  if (agent.linear_velocity.length_squared() < agent.zero_linear_speed_threshold) {
   accel.set_zero();
}


  else {
    float orientation = ;

   if (use_z) {
    orientation = GSAIUtils.vector3_to_angle(agent.linear_velocity);
}


   else {
    orientation = GSAIUtils.vector2_to_angle(GSAIUtils.to_vector2(agent.linear_velocity));
}

   match_orientation(accel, orientation);
}

}

}

 GSAILookWhereYouGo::GSAILookWhereYouGo() {
 }

 GSAILookWhereYouGo::~GSAILookWhereYouGo() {
 }


 static void GSAILookWhereYouGo::_bind_methods() {
  ClassDB::bind_method(D_METHOD("_calculate_steering", "accel"), &GSAILookWhereYouGo::_calculate_steering);

 }



