#ifndef GSAI_UTILS_H
#define GSAI_UTILS_H

#include "core/int_types.h"
#include "core/math/math_funcs.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "core/object/object.h"

class GSAIUtils : public Object {
	GDCLASS(GSAIUtils, Object);

public:
	static _ALWAYS_INLINE_ Vector3 clampedv3(Vector3 vector, const float limit) {
		float length_squared = vector.length_squared();
		float limit_squared = limit * limit;

		if (length_squared > limit_squared) {
			vector *= Math::sqrt(limit_squared / length_squared);
		}

		return vector;
	}

	static _ALWAYS_INLINE_ float vector3_to_angle(const Vector3 &vector) {
		return atan2(vector.x, vector.z);
	}

	static _ALWAYS_INLINE_ float vector2_to_angle(const Vector2 &vector) {
		return atan2(vector.x, -vector.y);
	}

	static _ALWAYS_INLINE_ Vector2 angle_to_vector2(const float angle) {
		return Vector2(sin(-angle), cos(angle));
	}

	static _ALWAYS_INLINE_ Vector2 to_vector2(const Vector3 &vector) {
		return Vector2(vector.x, vector.y);
	}

	static _ALWAYS_INLINE_ Vector3 to_vector3(const Vector2 &vector) {
		return Vector3(vector.x, vector.y, 0);
	}

	// Binds for scripting
	_FORCE_INLINE_ Vector3 bclampedv3(const Vector3 &vector, const float limit) {
		return GSAIUtils::clampedv3(vector, limit);
	}
	_FORCE_INLINE_ float bvector3_to_angle(const Vector3 &vector) {
		return GSAIUtils::vector3_to_angle(vector);
	}
	_FORCE_INLINE_ float bvector2_to_angle(const Vector2 &vector) {
		return GSAIUtils::vector2_to_angle(vector);
	}
	_FORCE_INLINE_ Vector2 bangle_to_vector2(const float angle) {
		return GSAIUtils::angle_to_vector2(angle);
	}
	_FORCE_INLINE_ Vector2 bto_vector2(const Vector3 &vector) {
		return GSAIUtils::to_vector2(vector);
	}
	_FORCE_INLINE_ Vector3 bto_vector3(const Vector2 &vector) {
		return GSAIUtils::to_vector3(vector);
	}

	static _FORCE_INLINE_ GSAIUtils *get_singleton() {
		return _singleton;
	}

	GSAIUtils();
	~GSAIUtils();

protected:
	static void _bind_methods();

	static GSAIUtils *_singleton;
};

#endif
