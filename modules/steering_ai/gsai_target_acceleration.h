#ifndef GSAI_TARGET_ACCELERATION_H
#define GSAI_TARGET_ACCELERATION_H

#include "core/int_types.h"
#include "core/math/vector3.h"
#include "core/object/reference.h"

// A desired linear and angular amount of acceleration requested by the steering system.

class GSAITargetAcceleration : public Reference {
	GDCLASS(GSAITargetAcceleration, Reference);

public:
	Vector3 get_linear();
	void set_linear(const Vector3 &val);

	float get_angular() const;
	void set_angular(const float val);

	// Sets the linear and angular components to 0.
	void set_zero();
	// Adds `accel`'s components, multiplied by `scalar`, to this one.
	void add_scaled_accel(const Ref<GSAITargetAcceleration> &accel, const float scalar);
	// Returns the squared magnitude of the linear and angular components.
	float get_magnitude_squared();
	// Returns the magnitude of the linear and angular components.
	float get_magnitude();

	GSAITargetAcceleration();
	~GSAITargetAcceleration();

protected:
	static void _bind_methods();

	// Linear acceleration
	Vector3 linear;

	// Angular acceleration
	float angular;
};

#endif
