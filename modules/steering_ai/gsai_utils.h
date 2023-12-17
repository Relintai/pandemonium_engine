#ifndef GSAI_UTILS_H
#define GSAI_UTILS_H

/*************************************************************************/
/*  gsai_utils.h                                                         */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

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
