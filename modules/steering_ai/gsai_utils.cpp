
#include "gsai_utils.h"

GSAIUtils *GSAIUtils::_singleton = NULL;

GSAIUtils::GSAIUtils() {
	_singleton = this;
}

GSAIUtils::~GSAIUtils() {
  _singleton = NULL;
}

void GSAIUtils::_bind_methods() {
	ClassDB::bind_method(D_METHOD("clampedv3", "vector", "limit"), &GSAIUtils::bclampedv3);
	ClassDB::bind_method(D_METHOD("vector3_to_angle", "vector"), &GSAIUtils::bvector3_to_angle);
	ClassDB::bind_method(D_METHOD("vector2_to_angle", "vector"), &GSAIUtils::bvector2_to_angle);
	ClassDB::bind_method(D_METHOD("angle_to_vector2", "angle"), &GSAIUtils::bangle_to_vector2);
	ClassDB::bind_method(D_METHOD("to_vector2", "vector"), &GSAIUtils::bto_vector2);
	ClassDB::bind_method(D_METHOD("to_vector3", "vector"), &GSAIUtils::bto_vector3);
}
