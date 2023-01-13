#ifndef GSAI_AGENT_LOCATION_H
#define GSAI_AGENT_LOCATION_H

#include "core/int_types.h"
#include "core/math/vector3.h"
#include "core/object/reference.h"

class GSAIAgentLocation : public Reference {
	GDCLASS(GSAIAgentLocation, Reference);

public:
	Vector3 get_position();
	void set_position(const Vector3 &val);

	float get_orientation() const;
	void set_orientation(const float val);

	GSAIAgentLocation();
	~GSAIAgentLocation();

protected:
	static void _bind_methods();

	// Represents an agent with only a location and an orientation.
	// @category - Base types
	// The agent's position in space.
	Vector3 position;
	// The agent's orientation on its Y axis rotation.
	float orientation;
};

#endif
