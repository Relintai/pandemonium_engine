
#include "gsai_flee.h"



 // Calculates acceleration to take an agent directly away from a target agent.;
 // @category - Individual behaviors;

 void GSAIFlee::_calculate_steering(const GSAITargetAcceleration &acceleration) {
  acceleration.linear = ((agent.position - target.position).normalized() * agent.linear_acceleration_max);
  acceleration.angular = 0;
}

}

 GSAIFlee::GSAIFlee() {
 }

 GSAIFlee::~GSAIFlee() {
 }


 static void GSAIFlee::_bind_methods() {
  ClassDB::bind_method(D_METHOD("_calculate_steering", "acceleration"), &GSAIFlee::_calculate_steering);

 }



