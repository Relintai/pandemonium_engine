
#include "gsai_utils.h"



 // Math and vector utility functions.;
 // @Category - Utilities;
 // Returns the `vector` with its length capped to `limit`.;

 Vector3 GSAIUtils::clampedv3(const Vector3 &vector, const float limit) {
   float length_squared = vector.length_squared();
   float limit_squared = limit * limit;

  if (length_squared > limit_squared) {
   vector *= sqrt(limit_squared / length_squared);
}

  return vector;
}

 // Returns an angle in radians between the positive X axis and the `vector`.;
 //;
 // This assumes orientation for 3D agents that are upright and rotate;
 // around the Y axis.;

 float GSAIUtils::vector3_to_angle(const Vector3 &vector) {
  return atan2(vector.x, vector.z);
}

 // Returns an angle in radians between the positive X axis and the `vector`.;

 float GSAIUtils::vector2_to_angle(const Vector2 &vector) {
  return atan2(vector.x, -vector.y);
}

 // Returns a directional vector from the given orientation angle.;
 //;
 // This assumes orientation for 2D agents or 3D agents that are upright and;
 // rotate around the Y axis.;

 Vector2 GSAIUtils::angle_to_vector2(const float angle) {
  return Vector2(sin(-angle), cos(angle));
}

 // Returns a vector2 with `vector`'s x and y components.;

 Vector2 GSAIUtils::to_vector2(const Vector3 &vector) {
  return Vector2(vector.x, vector.y);
}

 // Returns a vector3 with `vector`'s x and y components and 0 in z.;

 Vector3 GSAIUtils::to_vector3(const Vector2 &vector) {
  return Vector3(vector.x, vector.y, 0);
}

}

 GSAIUtils::GSAIUtils() {
 }

 GSAIUtils::~GSAIUtils() {
 }


 static void GSAIUtils::_bind_methods() {

 }



