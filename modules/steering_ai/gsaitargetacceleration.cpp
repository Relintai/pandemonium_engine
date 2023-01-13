
#include "gsaitargetacceleration.h"


Vector3 GSAITargetAcceleration::get_linear() {
 return linear;
}

void GSAITargetAcceleration::set_linear(const Vector3 &val) {
linear = val;
}


float GSAITargetAcceleration::get_angular() const {
 return angular;
}

void GSAITargetAcceleration::set_angular(const float val) {
angular = val;
}



 // A desired linear and angular amount of acceleration requested by the steering;
 // system.;
 // @category - Base types;
 // Linear acceleration;
  Vector3 linear = Vector3.ZERO;
 // Angular acceleration;
  float angular = 0.0;
 // Sets the linear and angular components to 0.;

 void GSAITargetAcceleration::set_zero() {
  linear.x = 0.0;
  linear.y = 0.0;
  linear.z = 0.0;
  angular = 0.0;
}

 // Adds `accel`'s components, multiplied by `scalar`, to this one.;

 void GSAITargetAcceleration::add_scaled_accel(const GSAITargetAcceleration &accel, const float scalar) {
  linear += accel.linear * scalar;
  angular += accel.angular * scalar;
}

 // Returns the squared magnitude of the linear and angular components.;

 float GSAITargetAcceleration::get_magnitude_squared() {
  return linear.length_squared() + angular * angular;
}

 // Returns the magnitude of the linear and angular components.;

 float GSAITargetAcceleration::get_magnitude() {
  return sqrt(get_magnitude_squared());
}

}

 GSAITargetAcceleration::GSAITargetAcceleration() {
  linear = Vector3.ZERO;
  angular = 0.0;
 }

 GSAITargetAcceleration::~GSAITargetAcceleration() {
 }


 static void GSAITargetAcceleration::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_linear"), &GSAITargetAcceleration::get_linear);
   ClassDB::bind_method(D_METHOD("set_linear", "value"), &GSAITargetAcceleration::set_linear);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "linear"), "set_linear", "get_linear");


   ClassDB::bind_method(D_METHOD("get_angular"), &GSAITargetAcceleration::get_angular);
   ClassDB::bind_method(D_METHOD("set_angular", "value"), &GSAITargetAcceleration::set_angular);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "angular"), "set_angular", "get_angular");


  ClassDB::bind_method(D_METHOD("set_zero"), &GSAITargetAcceleration::set_zero);
  ClassDB::bind_method(D_METHOD("add_scaled_accel", "accel", "scalar"), &GSAITargetAcceleration::add_scaled_accel);
  ClassDB::bind_method(D_METHOD("get_magnitude_squared"), &GSAITargetAcceleration::get_magnitude_squared);
  ClassDB::bind_method(D_METHOD("get_magnitude"), &GSAITargetAcceleration::get_magnitude);

 }



