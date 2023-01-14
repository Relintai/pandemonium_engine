#ifndef GSAI_TARGET_ACCELERATION_H
#define GSAI_TARGET_ACCELERATION_H

#include "core/int_types.h"
#include "core/math/vector3.h"
#include "core/object/reference.h"

class GSAITargetAcceleration : public Reference {
	GDCLASS(GSAITargetAcceleration, Reference);

public:
	Vector3 get_linear();
	void set_linear(const Vector3 &val);

	float get_angular() const;
	void set_angular(const float val);

	void set_zero();
	void add_scaled_accel(const Ref<GSAITargetAcceleration> &accel, const float scalar);
	
	float get_magnitude_squared();
	float get_magnitude();

	GSAITargetAcceleration();
	~GSAITargetAcceleration();

protected:
	static void _bind_methods();

	Vector3 linear;
	float angular;
};

#endif
