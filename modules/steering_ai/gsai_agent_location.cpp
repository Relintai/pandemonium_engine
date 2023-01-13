
#include "gsai_agent_location.h"

Vector3 GSAIAgentLocation::get_position() {
	return position;
}

void GSAIAgentLocation::set_position(const Vector3 &val) {
	position = val;
}

float GSAIAgentLocation::get_orientation() const {
	return orientation;
}

void GSAIAgentLocation::set_orientation(const float val) {
	orientation = val;
}

GSAIAgentLocation::GSAIAgentLocation() {
	orientation = 0.0;
}

GSAIAgentLocation::~GSAIAgentLocation() {
}

void GSAIAgentLocation::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_position"), &GSAIAgentLocation::get_position);
	ClassDB::bind_method(D_METHOD("set_position", "value"), &GSAIAgentLocation::set_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "position"), "set_position", "get_position");

	ClassDB::bind_method(D_METHOD("get_orientation"), &GSAIAgentLocation::get_orientation);
	ClassDB::bind_method(D_METHOD("set_orientation", "value"), &GSAIAgentLocation::set_orientation);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "orientation"), "set_orientation", "get_orientation");
}
