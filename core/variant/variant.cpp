/*************************************************************************/
/*  variant.cpp                                                          */
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

#include "variant.h"

#include "core/core_string_names.h"
#include "core/io/marshalls.h"
#include "core/math/math_funcs.h"
#include "core/object/object_rc.h"
#include "core/object/resource.h"
#include "core/string/print_string.h"
#include "core/variant/variant_parser.h"
#include "scene/main/control.h"
#include "scene/main/node.h"

String Variant::get_type_name(Variant::Type p_type) {
	switch (p_type) {
		case NIL: {
			return "Nil";
		} break;

		// atomic types
		case BOOL: {
			return "bool";
		} break;
		case INT: {
			return "int";

		} break;
		case REAL: {
			return "float";

		} break;
		case STRING: {
			return "String";
		} break;

		// math types
		case RECT2: {
			return "Rect2";
		} break;
		case RECT2I: {
			return "Rect2i";
		} break;
		case VECTOR2: {
			return "Vector2";
		} break;
		case VECTOR2I: {
			return "Vector2i";
		} break;
		case VECTOR3: {
			return "Vector3";
		} break;
		case VECTOR3I: {
			return "Vector3i";
		} break;
		case VECTOR4: {
			return "Vector4";
		} break;
		case VECTOR4I: {
			return "Vector4i";
		} break;

		case PLANE: {
			return "Plane";
		} break;
		case QUATERNION: {
			return "Quaternion";
		} break;
		case AABB: {
			return "AABB";
		} break;
		case BASIS: {
			return "Basis";
		} break;
		case TRANSFORM: {
			return "Transform";
		} break;
		case TRANSFORM2D: {
			return "Transform2D";
		} break;
		case PROJECTION: {
			return "Projection";
		} break;

		// misc types
		case COLOR: {
			return "Color";
		} break;
		case NODE_PATH: {
			return "NodePath";
		} break;
		case RID: {
			return "RID";
		} break;
		case OBJECT: {
			return "Object";
		} break;
		case STRING_NAME: {
			return "StringName";
		} break;
		case DICTIONARY: {
			return "Dictionary";
		} break;
		case ARRAY: {
			return "Array";
		} break;

		// arrays
		case POOL_BYTE_ARRAY: {
			return "PoolByteArray";
		} break;
		case POOL_INT_ARRAY: {
			return "PoolIntArray";
		} break;
		case POOL_REAL_ARRAY: {
			return "PoolRealArray";
		} break;
		case POOL_STRING_ARRAY: {
			return "PoolStringArray";
		} break;
		case POOL_VECTOR2_ARRAY: {
			return "PoolVector2Array";
		} break;
		case POOL_VECTOR2I_ARRAY: {
			return "PoolVector2iArray";
		} break;
		case POOL_VECTOR3_ARRAY: {
			return "PoolVector3Array";
		} break;
		case POOL_VECTOR3I_ARRAY: {
			return "PoolVector3iArray";
		} break;
		case POOL_VECTOR4_ARRAY: {
			return "PoolVector4Array";
		} break;
		case POOL_VECTOR4I_ARRAY: {
			return "PoolVector4iArray";
		} break;
		case POOL_COLOR_ARRAY: {
			return "PoolColorArray";
		} break;
		default: {
		}
	}

	return "";
}

bool Variant::can_convert(Variant::Type p_type_from, Variant::Type p_type_to) {
	if (p_type_from == p_type_to) {
		return true;
	}
	if (p_type_to == NIL && p_type_from != NIL) { //nil can convert to anything
		return true;
	}

	if (p_type_from == NIL) {
		return (p_type_to == OBJECT);
	};

	const Type *valid_types = nullptr;
	const Type *invalid_types = nullptr;

	switch (p_type_to) {
		case NIL: {
			//can't
		} break;
		case BOOL: {
			static const Type valid[] = {
				INT,
				REAL,
				STRING,
				NIL,
			};

			valid_types = valid;
		} break;
		case INT: {
			static const Type valid[] = {
				BOOL,
				REAL,
				STRING,
				NIL,
			};

			valid_types = valid;
		} break;
		case REAL: {
			static const Type valid[] = {
				BOOL,
				INT,
				STRING,
				NIL,
			};

			valid_types = valid;
		} break;
		case STRING: {
			static const Type invalid[] = {
				OBJECT,
				NIL
			};

			invalid_types = invalid;
		} break;
		case RECT2: {
			static const Type valid[] = {
				RECT2I,
				NIL
			};

			valid_types = valid;
		} break;
		case RECT2I: {
			static const Type valid[] = {
				RECT2,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR2: {
			static const Type valid[] = {
				VECTOR2I,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR2I: {
			static const Type valid[] = {
				VECTOR2,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR3: {
			static const Type valid[] = {
				VECTOR3I,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR3I: {
			static const Type valid[] = {
				VECTOR3,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR4: {
			static const Type valid[] = {
				VECTOR4I,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR4I: {
			static const Type valid[] = {
				VECTOR4,
				NIL
			};

			valid_types = valid;
		} break;
		case PLANE: {
			//can't
		} break;
		case QUATERNION: {
			static const Type valid[] = {
				BASIS,
				NIL
			};

			valid_types = valid;
		} break;
		case AABB: {
			//can't
		} break;
		case BASIS: {
			static const Type valid[] = {
				QUATERNION,
				VECTOR3,
				NIL
			};

			valid_types = valid;
		} break;
		case TRANSFORM: {
			static const Type valid[] = {
				TRANSFORM2D,
				QUATERNION,
				BASIS,
				NIL
			};

			valid_types = valid;
		} break;
		case TRANSFORM2D: {
			static const Type valid[] = {
				TRANSFORM,
				NIL
			};

			valid_types = valid;
		} break;
		case PROJECTION: {
			static const Type valid[] = {
				TRANSFORM,
				NIL
			};

			valid_types = valid;
		} break;
		// misc types
		case COLOR: {
			static const Type valid[] = {
				STRING,
				INT,
				NIL,
			};

			valid_types = valid;
		} break;
		case NODE_PATH: {
			static const Type valid[] = {
				STRING,
				NIL
			};

			valid_types = valid;
		} break;
		case RID: {
			static const Type valid[] = {
				OBJECT,
				NIL
			};

			valid_types = valid;
		} break;
		case OBJECT: {
			static const Type valid[] = {
				NIL
			};

			valid_types = valid;
		} break;
		case STRING_NAME: {
			static const Type valid[] = {
				STRING,
				NIL
			};

			valid_types = valid;
		} break;
		case DICTIONARY: {
			//can't
		} break;
		case ARRAY: {
			static const Type valid[] = {
				POOL_BYTE_ARRAY,
				POOL_INT_ARRAY,
				POOL_STRING_ARRAY,
				POOL_REAL_ARRAY,
				POOL_COLOR_ARRAY,
				POOL_VECTOR2_ARRAY,
				POOL_VECTOR2I_ARRAY,
				POOL_VECTOR3_ARRAY,
				POOL_VECTOR3I_ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		// arrays
		case POOL_BYTE_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		case POOL_INT_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};
			valid_types = valid;
		} break;
		case POOL_REAL_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		case POOL_STRING_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};
			valid_types = valid;
		} break;
		case POOL_VECTOR2_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};
			valid_types = valid;
		} break;
		case POOL_VECTOR2I_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};
			valid_types = valid;
		} break;
		case POOL_VECTOR3_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};
			valid_types = valid;
		} break;
		case POOL_VECTOR3I_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};
			valid_types = valid;
		} break;
		case POOL_VECTOR4_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};
			valid_types = valid;
		} break;
		case POOL_VECTOR4I_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};
			valid_types = valid;
		} break;
		case POOL_COLOR_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		default: {
		}
	}

	if (valid_types) {
		int i = 0;
		while (valid_types[i] != NIL) {
			if (p_type_from == valid_types[i]) {
				return true;
			}
			i++;
		}

	} else if (invalid_types) {
		int i = 0;
		while (invalid_types[i] != NIL) {
			if (p_type_from == invalid_types[i]) {
				return false;
			}
			i++;
		}

		return true;
	}

	return false;
}

bool Variant::can_convert_strict(Variant::Type p_type_from, Variant::Type p_type_to) {
	if (p_type_from == p_type_to) {
		return true;
	}
	if (p_type_to == NIL && p_type_from != NIL) { //nil can convert to anything
		return true;
	}

	if (p_type_from == NIL) {
		return (p_type_to == OBJECT);
	};

	const Type *valid_types = nullptr;

	switch (p_type_to) {
		case NIL: {
			//can't, also already handled
		} break;
		case BOOL: {
			static const Type valid[] = {
				INT,
				REAL,
				//STRING,
				NIL,
			};

			valid_types = valid;
		} break;
		case INT: {
			static const Type valid[] = {
				BOOL,
				REAL,
				//STRING,
				NIL,
			};

			valid_types = valid;

		} break;
		case REAL: {
			static const Type valid[] = {
				BOOL,
				INT,
				//STRING,
				NIL,
			};

			valid_types = valid;

		} break;
		case STRING: {
			static const Type valid[] = {
				NODE_PATH,
				STRING_NAME,
				NIL
			};

			valid_types = valid;
		} break;
		case RECT2: {
			static const Type valid[] = {
				RECT2I,
				NIL
			};

			valid_types = valid;
		} break;
		case RECT2I: {
			static const Type valid[] = {
				RECT2,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR2: {
			static const Type valid[] = {
				VECTOR2I,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR2I: {
			static const Type valid[] = {
				VECTOR2,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR3: {
			static const Type valid[] = {
				VECTOR3I,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR3I: {
			static const Type valid[] = {
				VECTOR3,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR4: {
			static const Type valid[] = {
				VECTOR4I,
				NIL
			};

			valid_types = valid;
		} break;
		case VECTOR4I: {
			static const Type valid[] = {
				VECTOR4,
				NIL
			};

			valid_types = valid;
		} break;
		case PLANE: {
			//Can't
		} break;
		case QUATERNION: {
			static const Type valid[] = {
				BASIS,
				NIL
			};

			valid_types = valid;
		} break;
		case AABB: {
			//Can't
		} break;
		case BASIS: {
			static const Type valid[] = {
				QUATERNION,
				VECTOR3,
				NIL
			};

			valid_types = valid;
		} break;
		case TRANSFORM: {
			static const Type valid[] = {
				TRANSFORM2D,
				QUATERNION,
				BASIS,
				NIL
			};

			valid_types = valid;
		} break;
		case TRANSFORM2D: {
			static const Type valid[] = {
				TRANSFORM,
				NIL
			};

			valid_types = valid;
		} break;
		case PROJECTION: {
			static const Type valid[] = {
				TRANSFORM,
				NIL
			};

			valid_types = valid;
		} break;
		case COLOR: {
			static const Type valid[] = {
				STRING,
				INT,
				NIL,
			};

			valid_types = valid;
		} break;
		case NODE_PATH: {
			static const Type valid[] = {
				STRING,
				NIL
			};

			valid_types = valid;
		} break;
		case RID: {
			static const Type valid[] = {
				OBJECT,
				NIL
			};

			valid_types = valid;
		} break;
		case OBJECT: {
			static const Type valid[] = {
				NIL
			};

			valid_types = valid;
		} break;
		case STRING_NAME: {
			static const Type valid[] = {
				STRING,
				NIL
			};

			valid_types = valid;
		} break;
		case DICTIONARY: {
			//Can't
		} break;
		case ARRAY: {
			static const Type valid[] = {
				POOL_BYTE_ARRAY,
				POOL_INT_ARRAY,
				POOL_STRING_ARRAY,
				POOL_REAL_ARRAY,
				POOL_COLOR_ARRAY,
				POOL_VECTOR2_ARRAY,
				POOL_VECTOR2I_ARRAY,
				POOL_VECTOR3_ARRAY,
				POOL_VECTOR3I_ARRAY,
				POOL_VECTOR4_ARRAY,
				POOL_VECTOR4I_ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		// arrays
		case POOL_BYTE_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		case POOL_INT_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};
			valid_types = valid;
		} break;
		case POOL_REAL_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		case POOL_STRING_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		case POOL_VECTOR2_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		case POOL_VECTOR2I_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		case POOL_VECTOR3_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		case POOL_VECTOR3I_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		case POOL_VECTOR4_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		case POOL_VECTOR4I_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		case POOL_COLOR_ARRAY: {
			static const Type valid[] = {
				ARRAY,
				NIL
			};

			valid_types = valid;
		} break;
		default: {
		}
	}

	if (valid_types) {
		int i = 0;
		while (valid_types[i] != NIL) {
			if (p_type_from == valid_types[i]) {
				return true;
			}
			i++;
		}
	}

	return false;
}

bool Variant::deep_equal(const Variant &p_variant, int p_recursion_count) const {
	ERR_FAIL_COND_V_MSG(p_recursion_count > MAX_RECURSION, true, "Max recursion reached");

	// Containers must be handled with recursivity checks
	switch (type) {
		case Variant::Type::DICTIONARY: {
			if (p_variant.type != Variant::Type::DICTIONARY) {
				return false;
			}

			const Dictionary v1_as_d = Dictionary(*this);
			const Dictionary v2_as_d = Dictionary(p_variant);

			return v1_as_d.deep_equal(v2_as_d, p_recursion_count + 1);
		} break;
		case Variant::Type::ARRAY: {
			if (p_variant.type != Variant::Type::ARRAY) {
				return false;
			}

			const Array v1_as_a = Array(*this);
			const Array v2_as_a = Array(p_variant);

			return v1_as_a.deep_equal(v2_as_a, p_recursion_count + 1);
		} break;
		default: {
			return *this == p_variant;
		} break;
	}
}

bool Variant::operator==(const Variant &p_variant) const {
	if (type != p_variant.type) { //evaluation of operator== needs to be more strict
		return false;
	}
	bool v;
	Variant r;
	evaluate(OP_EQUAL, *this, p_variant, r, v);
	return r;
}

bool Variant::operator!=(const Variant &p_variant) const {
	if (type != p_variant.type) { //evaluation of operator== needs to be more strict
		return true;
	}
	bool v;
	Variant r;
	evaluate(OP_NOT_EQUAL, *this, p_variant, r, v);
	return r;
}

bool Variant::operator<(const Variant &p_variant) const {
	if (type != p_variant.type) { //if types differ, then order by type first
		return type < p_variant.type;
	}
	bool v;
	Variant r;
	evaluate(OP_LESS, *this, p_variant, r, v);
	return r;
}

bool Variant::is_zero() const {
	switch (type) {
		case NIL: {
			return true;
		} break;

		// atomic types
		case BOOL: {
			return !(_data._bool);
		} break;
		case INT: {
			return _data._int == 0;
		} break;
		case REAL: {
			return _data._real == 0;
		} break;
		case STRING: {
			return *reinterpret_cast<const String *>(_data._mem) == String();
		} break;

		// math types
		case RECT2: {
			return *reinterpret_cast<const Rect2 *>(_data._mem) == Rect2();
		} break;
		case RECT2I: {
			return *reinterpret_cast<const Rect2i *>(_data._mem) == Rect2i();
		} break;
		case VECTOR2: {
			return *reinterpret_cast<const Vector2 *>(_data._mem) == Vector2();
		} break;
		case VECTOR2I: {
			return *reinterpret_cast<const Vector2i *>(_data._mem) == Vector2i();
		} break;
		case VECTOR3: {
			return *reinterpret_cast<const Vector3 *>(_data._mem) == Vector3();
		} break;
		case VECTOR3I: {
			return *reinterpret_cast<const Vector3i *>(_data._mem) == Vector3i();
		} break;
		case VECTOR4: {
			return *reinterpret_cast<const Vector4 *>(_data._mem) == Vector4();
		} break;
		case VECTOR4I: {
			return *reinterpret_cast<const Vector4i *>(_data._mem) == Vector4i();
		} break;

		case PLANE: {
			return *reinterpret_cast<const Plane *>(_data._mem) == Plane();
		} break;
		case QUATERNION: {
			return *reinterpret_cast<const Quaternion *>(_data._mem) == Quaternion();
		} break;
		case AABB: {
			return *_data._aabb == ::AABB();
		} break;
		case BASIS: {
			return *_data._basis == Basis();
		} break;
		case TRANSFORM: {
			return *_data._transform == Transform();
		} break;
		case TRANSFORM2D: {
			return *_data._transform2d == Transform2D();
		} break;
		case PROJECTION: {
			return *_data._projection == Projection();
		} break;

		// misc types
		case COLOR: {
			return *reinterpret_cast<const Color *>(_data._mem) == Color();
		} break;
		case NODE_PATH: {
			return reinterpret_cast<const NodePath *>(_data._mem)->is_empty();
		} break;
		case RID: {
			return *reinterpret_cast<const ::RID *>(_data._mem) == ::RID();
		} break;
		case OBJECT: {
			return _UNSAFE_OBJ_PROXY_PTR(*this) == nullptr;
		} break;
		case STRING_NAME: {
			return *reinterpret_cast<const StringName *>(_data._mem) != StringName();
		} break;
		case DICTIONARY: {
			return reinterpret_cast<const Dictionary *>(_data._mem)->empty();
		} break;
		case ARRAY: {
			return reinterpret_cast<const Array *>(_data._mem)->empty();
		} break;

		// arrays
		case POOL_BYTE_ARRAY: {
			return reinterpret_cast<const PoolVector<uint8_t> *>(_data._mem)->size() == 0;
		} break;
		case POOL_INT_ARRAY: {
			return reinterpret_cast<const PoolVector<int> *>(_data._mem)->size() == 0;
		} break;
		case POOL_REAL_ARRAY: {
			return reinterpret_cast<const PoolVector<real_t> *>(_data._mem)->size() == 0;
		} break;
		case POOL_STRING_ARRAY: {
			return reinterpret_cast<const PoolVector<String> *>(_data._mem)->size() == 0;
		} break;
		case POOL_VECTOR2_ARRAY: {
			return reinterpret_cast<const PoolVector<Vector2> *>(_data._mem)->size() == 0;
		} break;
		case POOL_VECTOR2I_ARRAY: {
			return reinterpret_cast<const PoolVector<Vector2i> *>(_data._mem)->size() == 0;
		} break;
		case POOL_VECTOR3_ARRAY: {
			return reinterpret_cast<const PoolVector<Vector3> *>(_data._mem)->size() == 0;
		} break;
		case POOL_VECTOR3I_ARRAY: {
			return reinterpret_cast<const PoolVector<Vector3i> *>(_data._mem)->size() == 0;
		} break;
		case POOL_VECTOR4_ARRAY: {
			return reinterpret_cast<const PoolVector<Vector4> *>(_data._mem)->size() == 0;
		} break;
		case POOL_VECTOR4I_ARRAY: {
			return reinterpret_cast<const PoolVector<Vector4i> *>(_data._mem)->size() == 0;
		} break;
		case POOL_COLOR_ARRAY: {
			return reinterpret_cast<const PoolVector<Color> *>(_data._mem)->size() == 0;
		} break;
		default: {
		}
	}

	return false;
}

bool Variant::is_one() const {
	switch (type) {
		case NIL: {
			return true;
		} break;

		// atomic types
		case BOOL: {
			return _data._bool;
		} break;
		case INT: {
			return _data._int == 1;
		} break;
		case REAL: {
			return _data._real == 1;
		} break;
		case RECT2: {
			return *reinterpret_cast<const Rect2 *>(_data._mem) == Rect2(1, 1, 1, 1);
		} break;
		case RECT2I: {
			return *reinterpret_cast<const Rect2i *>(_data._mem) == Rect2i(1, 1, 1, 1);
		} break;
		case VECTOR2: {
			return *reinterpret_cast<const Vector2 *>(_data._mem) == Vector2(1, 1);
		} break;
		case VECTOR2I: {
			return *reinterpret_cast<const Vector2i *>(_data._mem) == Vector2i(1, 1);
		} break;
		case VECTOR3: {
			return *reinterpret_cast<const Vector3 *>(_data._mem) == Vector3(1, 1, 1);
		} break;
		case VECTOR3I: {
			return *reinterpret_cast<const Vector3i *>(_data._mem) == Vector3i(1, 1, 1);
		} break;
		case VECTOR4: {
			return *reinterpret_cast<const Vector4 *>(_data._mem) == Vector4(1, 1, 1, 1);
		} break;
		case VECTOR4I: {
			return *reinterpret_cast<const Vector4i *>(_data._mem) == Vector4i(1, 1, 1, 1);
		} break;
		case PLANE: {
			return *reinterpret_cast<const Plane *>(_data._mem) == Plane(1, 1, 1, 1);
		} break;
		case COLOR: {
			return *reinterpret_cast<const Color *>(_data._mem) == Color(1, 1, 1, 1);
		} break;

		default: {
			return !is_zero();
		}
	}

	return false;
}

ObjectID Variant::get_object_instance_id() const {
	if (unlikely(type != OBJECT)) {
		return 0;
	} else if (likely(_get_obj().rc)) {
		return _get_obj().rc->instance_id;
	} else if (likely(!_get_obj().ref.is_null())) {
		return _REF_OBJ_PTR(*this)->get_instance_id();
	} else {
		return 0;
	}
}

bool Variant::is_invalid_object() const {
	return type == OBJECT && _get_obj().rc && !_get_obj().rc->get_ptr();
}

void Variant::reference(const Variant &p_variant) {
	switch (type) {
		case NIL:
		case BOOL:
		case INT:
		case REAL:
			break;
		default:
			clear();
	}

	type = p_variant.type;

	switch (p_variant.type) {
		case NIL: {
			// none
		} break;

		// atomic types
		case BOOL: {
			_data._bool = p_variant._data._bool;
		} break;
		case INT: {
			_data._int = p_variant._data._int;
		} break;
		case REAL: {
			_data._real = p_variant._data._real;
		} break;
		case STRING: {
			memnew_placement(_data._mem, String(*reinterpret_cast<const String *>(p_variant._data._mem)));
		} break;

		// math types
		case RECT2: {
			memnew_placement(_data._mem, Rect2(*reinterpret_cast<const Rect2 *>(p_variant._data._mem)));
		} break;
		case RECT2I: {
			memnew_placement(_data._mem, Rect2i(*reinterpret_cast<const Rect2i *>(p_variant._data._mem)));
		} break;
		case VECTOR2: {
			memnew_placement(_data._mem, Vector2(*reinterpret_cast<const Vector2 *>(p_variant._data._mem)));
		} break;
		case VECTOR2I: {
			memnew_placement(_data._mem, Vector2i(*reinterpret_cast<const Vector2i *>(p_variant._data._mem)));
		} break;
		case VECTOR3: {
			memnew_placement(_data._mem, Vector3(*reinterpret_cast<const Vector3 *>(p_variant._data._mem)));
		} break;
		case VECTOR3I: {
			memnew_placement(_data._mem, Vector3i(*reinterpret_cast<const Vector3i *>(p_variant._data._mem)));
		} break;
		case VECTOR4: {
			memnew_placement(_data._mem, Vector4(*reinterpret_cast<const Vector4 *>(p_variant._data._mem)));
		} break;
		case VECTOR4I: {
			memnew_placement(_data._mem, Vector4i(*reinterpret_cast<const Vector4i *>(p_variant._data._mem)));
		} break;

		case PLANE: {
			memnew_placement(_data._mem, Plane(*reinterpret_cast<const Plane *>(p_variant._data._mem)));
		} break;
		case QUATERNION: {
			memnew_placement(_data._mem, Quaternion(*reinterpret_cast<const Quaternion *>(p_variant._data._mem)));
		} break;
		case AABB: {
			_data._aabb = memnew(::AABB(*p_variant._data._aabb));
		} break;
		case BASIS: {
			_data._basis = memnew(Basis(*p_variant._data._basis));
		} break;
		case TRANSFORM: {
			_data._transform = memnew(Transform(*p_variant._data._transform));
		} break;
		case TRANSFORM2D: {
			_data._transform2d = memnew(Transform2D(*p_variant._data._transform2d));
		} break;
		case PROJECTION: {
			_data._projection = memnew(Projection(*p_variant._data._projection));
		} break;

		// misc types
		case COLOR: {
			memnew_placement(_data._mem, Color(*reinterpret_cast<const Color *>(p_variant._data._mem)));
		} break;
		case NODE_PATH: {
			memnew_placement(_data._mem, NodePath(*reinterpret_cast<const NodePath *>(p_variant._data._mem)));
		} break;
		case RID: {
			memnew_placement(_data._mem, ::RID(*reinterpret_cast<const ::RID *>(p_variant._data._mem)));
		} break;
		case OBJECT: {
			memnew_placement(_data._mem, ObjData(p_variant._get_obj()));
			if (likely(_get_obj().rc)) {
				_get_obj().rc->increment();
			}
		} break;
		case STRING_NAME: {
			memnew_placement(_data._mem, StringName(*reinterpret_cast<const StringName *>(p_variant._data._mem)));
		} break;
		case DICTIONARY: {
			memnew_placement(_data._mem, Dictionary(*reinterpret_cast<const Dictionary *>(p_variant._data._mem)));
		} break;
		case ARRAY: {
			memnew_placement(_data._mem, Array(*reinterpret_cast<const Array *>(p_variant._data._mem)));
		} break;

		// arrays
		case POOL_BYTE_ARRAY: {
			memnew_placement(_data._mem, PoolVector<uint8_t>(*reinterpret_cast<const PoolVector<uint8_t> *>(p_variant._data._mem)));
		} break;
		case POOL_INT_ARRAY: {
			memnew_placement(_data._mem, PoolVector<int>(*reinterpret_cast<const PoolVector<int> *>(p_variant._data._mem)));
		} break;
		case POOL_REAL_ARRAY: {
			memnew_placement(_data._mem, PoolVector<real_t>(*reinterpret_cast<const PoolVector<real_t> *>(p_variant._data._mem)));
		} break;
		case POOL_STRING_ARRAY: {
			memnew_placement(_data._mem, PoolVector<String>(*reinterpret_cast<const PoolVector<String> *>(p_variant._data._mem)));
		} break;
		case POOL_VECTOR2_ARRAY: {
			memnew_placement(_data._mem, PoolVector<Vector2>(*reinterpret_cast<const PoolVector<Vector2> *>(p_variant._data._mem)));
		} break;
		case POOL_VECTOR2I_ARRAY: {
			memnew_placement(_data._mem, PoolVector<Vector2i>(*reinterpret_cast<const PoolVector<Vector2i> *>(p_variant._data._mem)));
		} break;
		case POOL_VECTOR3_ARRAY: {
			memnew_placement(_data._mem, PoolVector<Vector3>(*reinterpret_cast<const PoolVector<Vector3> *>(p_variant._data._mem)));
		} break;
		case POOL_VECTOR3I_ARRAY: {
			memnew_placement(_data._mem, PoolVector<Vector3i>(*reinterpret_cast<const PoolVector<Vector3i> *>(p_variant._data._mem)));
		} break;
		case POOL_VECTOR4_ARRAY: {
			memnew_placement(_data._mem, PoolVector<Vector4>(*reinterpret_cast<const PoolVector<Vector4> *>(p_variant._data._mem)));
		} break;
		case POOL_VECTOR4I_ARRAY: {
			memnew_placement(_data._mem, PoolVector<Vector4i>(*reinterpret_cast<const PoolVector<Vector4i> *>(p_variant._data._mem)));
		} break;
		case POOL_COLOR_ARRAY: {
			memnew_placement(_data._mem, PoolVector<Color>(*reinterpret_cast<const PoolVector<Color> *>(p_variant._data._mem)));
		} break;
		default: {
		}
	}
}

void Variant::zero() {
	switch (type) {
		case NIL:
			break;
		case BOOL:
			this->_data._bool = false;
			break;
		case INT:
			this->_data._int = 0;
			break;
		case REAL:
			this->_data._real = 0;
			break;
		case RECT2:
			*reinterpret_cast<Rect2 *>(this->_data._mem) = Rect2();
			break;
		case RECT2I:
			*reinterpret_cast<Rect2i *>(this->_data._mem) = Rect2i();
			break;
		case VECTOR2:
			*reinterpret_cast<Vector2 *>(this->_data._mem) = Vector2();
			break;
		case VECTOR2I:
			*reinterpret_cast<Vector2i *>(this->_data._mem) = Vector2i();
			break;
		case VECTOR3:
			*reinterpret_cast<Vector3 *>(this->_data._mem) = Vector3();
			break;
		case VECTOR3I:
			*reinterpret_cast<Vector3i *>(this->_data._mem) = Vector3i();
			break;
		case VECTOR4:
			*reinterpret_cast<Vector4 *>(this->_data._mem) = Vector4();
			break;
		case VECTOR4I:
			*reinterpret_cast<Vector4i *>(this->_data._mem) = Vector4i();
			break;
		case PLANE:
			*reinterpret_cast<Plane *>(this->_data._mem) = Plane();
			break;
		case QUATERNION:
			*reinterpret_cast<Quaternion *>(this->_data._mem) = Quaternion();
			break;
		case AABB:
			*reinterpret_cast<::AABB *>(this->_data._mem) = ::AABB();
			break;
		case COLOR:
			*reinterpret_cast<Color *>(this->_data._mem) = Color();
			break;
		case PROJECTION:
			*reinterpret_cast<Projection *>(this->_data._mem) = Projection();
			break;
		default:
			this->clear();
			break;
	}
}

void Variant::clear() {
	switch (type) {
		/*
		// no point, they don't allocate memory
		BOOL,
		INT,
		REAL,
		*/
		case STRING: {
			reinterpret_cast<String *>(_data._mem)->~String();
		} break;
		/*
		RECT2,
		RECT2I
		VECTOR2,
		VECTOR2I,
		VECTOR3,
		VECTOR3i,
		VECTOR4,
		VECTOR4i,

		PLANE,
		QUATERNION,
		*/
		case AABB: {
			memdelete(_data._aabb);
		} break;
		case BASIS: {
			memdelete(_data._basis);
		} break;
		case TRANSFORM: {
			memdelete(_data._transform);
		} break;
		case TRANSFORM2D: {
			memdelete(_data._transform2d);
		} break;
		case PROJECTION: {
			memdelete(_data._projection);
		} break;
		//COLOR

		// misc types
		case NODE_PATH: {
			reinterpret_cast<NodePath *>(_data._mem)->~NodePath();
		} break;
		case RID: {
			// not much need probably
			reinterpret_cast<::RID *>(_data._mem)->~RID();
		} break;
		case OBJECT: {
			if (likely(_get_obj().rc)) {
				if (unlikely(_get_obj().rc->decrement())) {
					memdelete(_get_obj().rc);
				}
			} else {
				_get_obj().ref.unref();
			}
		} break;
		case STRING_NAME: {
			reinterpret_cast<StringName *>(_data._mem)->~StringName();
		} break;
		case DICTIONARY: {
			reinterpret_cast<Dictionary *>(_data._mem)->~Dictionary();
		} break;
		case ARRAY: {
			reinterpret_cast<Array *>(_data._mem)->~Array();
		} break;

		// arrays
		case POOL_BYTE_ARRAY: {
			reinterpret_cast<PoolVector<uint8_t> *>(_data._mem)->~PoolVector<uint8_t>();
		} break;
		case POOL_INT_ARRAY: {
			reinterpret_cast<PoolVector<int> *>(_data._mem)->~PoolVector<int>();
		} break;
		case POOL_REAL_ARRAY: {
			reinterpret_cast<PoolVector<real_t> *>(_data._mem)->~PoolVector<real_t>();
		} break;
		case POOL_STRING_ARRAY: {
			reinterpret_cast<PoolVector<String> *>(_data._mem)->~PoolVector<String>();
		} break;
		case POOL_VECTOR2_ARRAY: {
			reinterpret_cast<PoolVector<Vector2> *>(_data._mem)->~PoolVector<Vector2>();
		} break;
		case POOL_VECTOR2I_ARRAY: {
			reinterpret_cast<PoolVector<Vector2i> *>(_data._mem)->~PoolVector<Vector2i>();
		} break;
		case POOL_VECTOR3_ARRAY: {
			reinterpret_cast<PoolVector<Vector3> *>(_data._mem)->~PoolVector<Vector3>();
		} break;
		case POOL_VECTOR3I_ARRAY: {
			reinterpret_cast<PoolVector<Vector3i> *>(_data._mem)->~PoolVector<Vector3i>();
		} break;
		case POOL_VECTOR4_ARRAY: {
			reinterpret_cast<PoolVector<Vector4> *>(_data._mem)->~PoolVector<Vector4>();
		} break;
		case POOL_VECTOR4I_ARRAY: {
			reinterpret_cast<PoolVector<Vector4i> *>(_data._mem)->~PoolVector<Vector4i>();
		} break;
		case POOL_COLOR_ARRAY: {
			reinterpret_cast<PoolVector<Color> *>(_data._mem)->~PoolVector<Color>();
		} break;
		default: {
		} /* not needed */
	}

	type = NIL;
}

Variant::operator signed int() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1 : 0;
		case INT:
			return _data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_int();
		default: {
			return 0;
		}
	}
}
Variant::operator unsigned int() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1 : 0;
		case INT:
			return _data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_int();
		default: {
			return 0;
		}
	}
}

Variant::operator int64_t() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1 : 0;
		case INT:
			return _data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_int64();
		default: {
			return 0;
		}
	}
}

/*
Variant::operator long unsigned int() const {

	switch( type ) {

		case NIL: return 0;
		case BOOL: return _data._bool ? 1 : 0;
		case INT: return _data._int;
		case REAL: return _data._real;
		case STRING: return operator String().to_int();
		default: {

			return 0;
		}
	}

	return 0;
};
*/

Variant::operator uint64_t() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1 : 0;
		case INT:
			return _data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_int();
		default: {
			return 0;
		}
	}
}

#ifdef NEED_LONG_INT
Variant::operator signed long() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1 : 0;
		case INT:
			return _data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_int();
		default: {
			return 0;
		}
	}

	return 0;
};

Variant::operator unsigned long() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1 : 0;
		case INT:
			return _data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_int();
		default: {
			return 0;
		}
	}

	return 0;
};
#endif

Variant::operator signed short() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1 : 0;
		case INT:
			return _data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_int();
		default: {
			return 0;
		}
	}
}
Variant::operator unsigned short() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1 : 0;
		case INT:
			return _data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_int();
		default: {
			return 0;
		}
	}
}
Variant::operator signed char() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1 : 0;
		case INT:
			return _data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_int();
		default: {
			return 0;
		}
	}
}
Variant::operator unsigned char() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1 : 0;
		case INT:
			return _data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_int();
		default: {
			return 0;
		}
	}
}

Variant::operator CharType() const {
	return operator unsigned int();
}

Variant::operator float() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1.0 : 0.0;
		case INT:
			return (float)_data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_double();
		default: {
			return 0;
		}
	}
}
Variant::operator double() const {
	switch (type) {
		case NIL:
			return 0;
		case BOOL:
			return _data._bool ? 1.0 : 0.0;
		case INT:
			return (double)_data._int;
		case REAL:
			return _data._real;
		case STRING:
			return operator String().to_double();
		default: {
			return 0;
		}
	}
}

Variant::operator StringName() const {
	if (type == STRING_NAME) {
		return *reinterpret_cast<const StringName *>(_data._mem);
	} else if (type == NODE_PATH) {
		return reinterpret_cast<const NodePath *>(_data._mem)->get_sname();
	}

	return StringName(operator String());
}

struct _VariantStrPair {
	String key;
	String value;

	bool operator<(const _VariantStrPair &p) const {
		return key < p.key;
	}
};

Variant::operator String() const {
	List<const void *> stack;

	return stringify(stack);
}

template <class T>
String stringify_vector(const T &vec, List<const void *> &stack) {
	String str("[");
	for (int i = 0; i < vec.size(); i++) {
		if (i > 0) {
			str += ", ";
		}
		str = str + Variant(vec[i]).stringify(stack);
	}
	str += "]";
	return str;
}

String Variant::stringify(List<const void *> &stack) const {
	switch (type) {
		case NIL:
			return "Null";
		case BOOL:
			return _data._bool ? "True" : "False";
		case INT:
			return itos(_data._int);
		case REAL:
			return rtos(_data._real);
		case STRING:
			return *reinterpret_cast<const String *>(_data._mem);
		case RECT2:
			return operator Rect2();
		case RECT2I:
			return operator Rect2i();
		case VECTOR2:
			return operator Vector2();
		case VECTOR2I:
			return operator Vector2i();
		case VECTOR3:
			return operator Vector3();
		case VECTOR3I:
			return operator Vector3i();
		case VECTOR4:
			return operator Vector4();
		case VECTOR4I:
			return operator Vector4i();
		case PLANE:
			return operator Plane();
		case QUATERNION:
			return operator Quaternion();
		case AABB:
			return operator ::AABB();
		case BASIS: {
			return operator Basis();
		} break;
		case TRANSFORM:
			return operator Transform();
		case TRANSFORM2D: {
			return operator Transform2D();
		} break;
		case PROJECTION:
			return operator Projection();
		case COLOR:
			return operator Color();
		case NODE_PATH:
			return operator NodePath();
		//RID
		case OBJECT: {
			Object *obj = _OBJ_PTR(*this);
			if (likely(obj)) {
				return obj->to_string();
			} else {
				if (_get_obj().rc) {
					return "[Deleted Object]";
				}
				return "[Object:null]";
			}
		} break;
		case STRING_NAME:
			return operator StringName();
		case DICTIONARY: {
			const Dictionary &d = *reinterpret_cast<const Dictionary *>(_data._mem);
			if (stack.find(d.id())) {
				return "{...}";
			}

			stack.push_back(d.id());

			//const String *K=NULL;
			String str("{");
			List<Variant> keys;
			d.get_key_list(&keys);

			Vector<_VariantStrPair> pairs;

			for (List<Variant>::Element *E = keys.front(); E; E = E->next()) {
				_VariantStrPair sp;
				sp.key = E->get().stringify(stack);
				sp.value = d[E->get()].stringify(stack);

				pairs.push_back(sp);
			}

			pairs.sort();

			for (int i = 0; i < pairs.size(); i++) {
				if (i > 0) {
					str += ", ";
				}
				str += pairs[i].key + ":" + pairs[i].value;
			}
			str += "}";

			stack.erase(d.id());
			return str;
		} break;
		case ARRAY: {
			Array arr = operator Array();
			if (stack.find(arr.id())) {
				return "[...]";
			}
			stack.push_back(arr.id());
			String str = stringify_vector(arr, stack);
			stack.erase(arr.id());
			return str;

		} break;

		case POOL_BYTE_ARRAY: {
			return stringify_vector(operator PoolVector<uint8_t>(), stack);
		} break;
		case POOL_INT_ARRAY: {
			return stringify_vector(operator PoolVector<int>(), stack);
		} break;
		case POOL_REAL_ARRAY: {
			return stringify_vector(operator PoolVector<real_t>(), stack);
		} break;
		case POOL_STRING_ARRAY: {
			return stringify_vector(operator PoolVector<String>(), stack);
		} break;
		case POOL_VECTOR2_ARRAY: {
			return stringify_vector(operator PoolVector<Vector2>(), stack);
		} break;
		case POOL_VECTOR2I_ARRAY: {
			return stringify_vector(operator PoolVector<Vector2i>(), stack);
		} break;
		case POOL_VECTOR3_ARRAY: {
			return stringify_vector(operator PoolVector<Vector3>(), stack);
		} break;
		case POOL_VECTOR3I_ARRAY: {
			return stringify_vector(operator PoolVector<Vector3i>(), stack);
		} break;
		case POOL_VECTOR4_ARRAY: {
			return stringify_vector(operator PoolVector<Vector4>(), stack);
		} break;
		case POOL_VECTOR4I_ARRAY: {
			return stringify_vector(operator PoolVector<Vector4i>(), stack);
		} break;
		case POOL_COLOR_ARRAY: {
			return stringify_vector(operator PoolVector<Color>(), stack);
		} break;

		default: {
			return "[" + get_type_name(type) + "]";
		}
	}

	return "";
}

Variant::operator Rect2() const {
	if (type == RECT2) {
		return *reinterpret_cast<const Rect2 *>(_data._mem);
	} else if (type == RECT2I) {
		return Rect2(*reinterpret_cast<const Rect2i *>(_data._mem));
	} else {
		return Rect2();
	}
}
Variant::operator Rect2i() const {
	if (type == RECT2I) {
		return *reinterpret_cast<const Rect2i *>(_data._mem);
	} else if (type == RECT2) {
		return Rect2i(*reinterpret_cast<const Rect2 *>(_data._mem));
	} else {
		return Rect2i();
	}
}

Variant::operator Vector2() const {
	if (type == VECTOR2) {
		return *reinterpret_cast<const Vector2 *>(_data._mem);
	} else if (type == VECTOR2I) {
		return Vector2(reinterpret_cast<const Vector2i *>(_data._mem)->x, reinterpret_cast<const Vector2i *>(_data._mem)->y);
	} else if (type == VECTOR3) {
		return Vector2(reinterpret_cast<const Vector3 *>(_data._mem)->x, reinterpret_cast<const Vector3 *>(_data._mem)->y);
	} else if (type == VECTOR3I) {
		return Vector2(reinterpret_cast<const Vector3i *>(_data._mem)->x, reinterpret_cast<const Vector3i *>(_data._mem)->y);
	} else if (type == VECTOR4) {
		return Vector2(reinterpret_cast<const Vector4 *>(_data._mem)->x, reinterpret_cast<const Vector4 *>(_data._mem)->y);
	} else if (type == VECTOR4I) {
		return Vector2(reinterpret_cast<const Vector4i *>(_data._mem)->x, reinterpret_cast<const Vector4i *>(_data._mem)->y);
	} else {
		return Vector2();
	}
}
Variant::operator Vector2i() const {
	if (type == VECTOR2I) {
		return *reinterpret_cast<const Vector2i *>(_data._mem);
	} else if (type == VECTOR2) {
		return Vector2i(reinterpret_cast<const Vector2 *>(_data._mem)->x, reinterpret_cast<const Vector2 *>(_data._mem)->y);
	} else if (type == VECTOR3) {
		return Vector2i(reinterpret_cast<const Vector3 *>(_data._mem)->x, reinterpret_cast<const Vector3 *>(_data._mem)->y);
	} else if (type == VECTOR3I) {
		return Vector2i(reinterpret_cast<const Vector3i *>(_data._mem)->x, reinterpret_cast<const Vector3i *>(_data._mem)->y);
	} else if (type == VECTOR4) {
		return Vector2i(reinterpret_cast<const Vector4 *>(_data._mem)->x, reinterpret_cast<const Vector4 *>(_data._mem)->y);
	} else if (type == VECTOR4I) {
		return Vector2i(reinterpret_cast<const Vector4i *>(_data._mem)->x, reinterpret_cast<const Vector4i *>(_data._mem)->y);
	} else {
		return Vector2i();
	}
}

Variant::operator Vector3() const {
	if (type == VECTOR3) {
		return *reinterpret_cast<const Vector3 *>(_data._mem);
	} else if (type == VECTOR3I) {
		return Vector3(*reinterpret_cast<const Vector3i *>(_data._mem));
	} else if (type == VECTOR2) {
		return Vector3(reinterpret_cast<const Vector2 *>(_data._mem)->x, reinterpret_cast<const Vector2 *>(_data._mem)->y, 0.0);
	} else if (type == VECTOR2I) {
		return Vector3(reinterpret_cast<const Vector2i *>(_data._mem)->x, reinterpret_cast<const Vector2i *>(_data._mem)->y, 0.0);
	} else if (type == VECTOR4) {
		return Vector3(reinterpret_cast<const Vector4 *>(_data._mem)->x, reinterpret_cast<const Vector4 *>(_data._mem)->y, reinterpret_cast<const Vector4 *>(_data._mem)->z);
	} else if (type == VECTOR4I) {
		return Vector3(reinterpret_cast<const Vector4i *>(_data._mem)->x, reinterpret_cast<const Vector4i *>(_data._mem)->y, reinterpret_cast<const Vector4i *>(_data._mem)->z);
	} else {
		return Vector3();
	}
}
Variant::operator Vector3i() const {
	if (type == VECTOR3I) {
		return *reinterpret_cast<const Vector3i *>(_data._mem);
	} else if (type == VECTOR3) {
		return Vector3i(reinterpret_cast<const Vector3 *>(_data._mem)->x, reinterpret_cast<const Vector3 *>(_data._mem)->y, reinterpret_cast<const Vector3 *>(_data._mem)->z);
	} else if (type == VECTOR2) {
		return Vector3i(reinterpret_cast<const Vector2 *>(_data._mem)->x, reinterpret_cast<const Vector2 *>(_data._mem)->y, 0.0);
	} else if (type == VECTOR2I) {
		return Vector3i(reinterpret_cast<const Vector2i *>(_data._mem)->x, reinterpret_cast<const Vector2i *>(_data._mem)->y, 0.0);
	} else if (type == VECTOR4) {
		return Vector3i(reinterpret_cast<const Vector4 *>(_data._mem)->x, reinterpret_cast<const Vector4 *>(_data._mem)->y, reinterpret_cast<const Vector4 *>(_data._mem)->z);
	} else if (type == VECTOR4I) {
		return Vector3i(reinterpret_cast<const Vector4i *>(_data._mem)->x, reinterpret_cast<const Vector4i *>(_data._mem)->y, reinterpret_cast<const Vector4i *>(_data._mem)->z);
	} else {
		return Vector3i();
	}
}

Variant::operator Vector4() const {
	if (type == VECTOR4) {
		return *reinterpret_cast<const Vector4 *>(_data._mem);
	} else if (type == VECTOR4I) {
		return *reinterpret_cast<const Vector4i *>(_data._mem);
	} else if (type == VECTOR2) {
		return Vector4(reinterpret_cast<const Vector2 *>(_data._mem)->x, reinterpret_cast<const Vector2 *>(_data._mem)->y, 0.0, 0.0);
	} else if (type == VECTOR2I) {
		return Vector4(reinterpret_cast<const Vector2i *>(_data._mem)->x, reinterpret_cast<const Vector2i *>(_data._mem)->y, 0.0, 0.0);
	} else if (type == VECTOR3) {
		return Vector4(reinterpret_cast<const Vector3 *>(_data._mem)->x, reinterpret_cast<const Vector3 *>(_data._mem)->y, reinterpret_cast<const Vector3 *>(_data._mem)->z, 0.0);
	} else if (type == VECTOR3I) {
		return Vector4(reinterpret_cast<const Vector3i *>(_data._mem)->x, reinterpret_cast<const Vector3i *>(_data._mem)->y, reinterpret_cast<const Vector3i *>(_data._mem)->z, 0.0);
	} else {
		return Vector4();
	}
}

Variant::operator Vector4i() const {
	if (type == VECTOR4I) {
		return *reinterpret_cast<const Vector4i *>(_data._mem);
	} else if (type == VECTOR4) {
		const Vector4 &v4 = *reinterpret_cast<const Vector4 *>(_data._mem);
		return Vector4i(v4.x, v4.y, v4.z, v4.w);
	} else if (type == VECTOR2) {
		return Vector4i(reinterpret_cast<const Vector2 *>(_data._mem)->x, reinterpret_cast<const Vector2 *>(_data._mem)->y, 0.0, 0.0);
	} else if (type == VECTOR2I) {
		return Vector4i(reinterpret_cast<const Vector2i *>(_data._mem)->x, reinterpret_cast<const Vector2i *>(_data._mem)->y, 0.0, 0.0);
	} else if (type == VECTOR3) {
		return Vector4i(reinterpret_cast<const Vector3 *>(_data._mem)->x, reinterpret_cast<const Vector3 *>(_data._mem)->y, reinterpret_cast<const Vector3 *>(_data._mem)->z, 0.0);
	} else if (type == VECTOR3I) {
		return Vector4i(reinterpret_cast<const Vector3i *>(_data._mem)->x, reinterpret_cast<const Vector3i *>(_data._mem)->y, reinterpret_cast<const Vector3i *>(_data._mem)->z, 0.0);
	} else {
		return Vector4i();
	}
}

Variant::operator Plane() const {
	if (type == PLANE) {
		return *reinterpret_cast<const Plane *>(_data._mem);
	} else {
		return Plane();
	}
}
Variant::operator ::AABB() const {
	if (type == AABB) {
		return *_data._aabb;
	} else {
		return ::AABB();
	}
}

Variant::operator Basis() const {
	if (type == BASIS) {
		return *_data._basis;
	} else if (type == QUATERNION) {
		return *reinterpret_cast<const Quaternion *>(_data._mem);
	} else if (type == VECTOR3) {
		return Basis(*reinterpret_cast<const Vector3 *>(_data._mem));
	} else if (type == TRANSFORM) { // unexposed in Variant::can_convert?
		return _data._transform->basis;
	} else {
		return Basis();
	}
}

Variant::operator Quaternion() const {
	if (type == QUATERNION) {
		return *reinterpret_cast<const Quaternion *>(_data._mem);
	} else if (type == BASIS) {
		return *_data._basis;
	} else if (type == TRANSFORM) {
		return _data._transform->basis;
	} else {
		return Quaternion();
	}
}

Variant::operator Transform2D() const {
	if (type == TRANSFORM2D) {
		return *_data._transform2d;
	} else if (type == TRANSFORM) {
		const Transform &t = *_data._transform;
		Transform2D m;
		m.columns[0][0] = t.basis.rows[0][0];
		m.columns[0][1] = t.basis.rows[1][0];
		m.columns[1][0] = t.basis.rows[0][1];
		m.columns[1][1] = t.basis.rows[1][1];
		m.columns[2][0] = t.origin[0];
		m.columns[2][1] = t.origin[1];
		return m;
	} else {
		return Transform2D();
	}
}

Variant::operator Transform() const {
	if (type == TRANSFORM) {
		return *_data._transform;
	} else if (type == BASIS) {
		return Transform(*_data._basis, Vector3());
	} else if (type == QUATERNION) {
		return Transform(Basis(*reinterpret_cast<const Quaternion *>(_data._mem)), Vector3());
	} else if (type == TRANSFORM2D) {
		const Transform2D &t = *_data._transform2d;
		Transform m;
		m.basis.rows[0][0] = t.columns[0][0];
		m.basis.rows[1][0] = t.columns[0][1];
		m.basis.rows[0][1] = t.columns[1][0];
		m.basis.rows[1][1] = t.columns[1][1];
		m.origin[0] = t.columns[2][0];
		m.origin[1] = t.columns[2][1];
		return m;
	} else if (type == PROJECTION) {
		return *_data._projection;
	} else {
		return Transform();
	}
}

Variant::operator Projection() const {
	if (type == TRANSFORM) {
		return *_data._transform;
	} else if (type == BASIS) {
		return Transform(*_data._basis, Vector3());
	} else if (type == QUATERNION) {
		return Transform(Basis(*reinterpret_cast<const Quaternion *>(_data._mem)), Vector3());
	} else if (type == TRANSFORM2D) {
		const Transform2D &t = *_data._transform2d;
		Transform m;
		m.basis.rows[0][0] = t.columns[0][0];
		m.basis.rows[1][0] = t.columns[0][1];
		m.basis.rows[0][1] = t.columns[1][0];
		m.basis.rows[1][1] = t.columns[1][1];
		m.origin[0] = t.columns[2][0];
		m.origin[1] = t.columns[2][1];
		return m;
	} else if (type == PROJECTION) {
		return *_data._projection;
	} else {
		return Projection();
	}
}

Variant::operator Color() const {
	if (type == COLOR) {
		return *reinterpret_cast<const Color *>(_data._mem);
	} else if (type == STRING) {
		return Color::html(operator String());
	} else if (type == INT) {
		return Color::hex(operator int());
	} else {
		return Color();
	}
}

Variant::operator NodePath() const {
	if (type == NODE_PATH) {
		return *reinterpret_cast<const NodePath *>(_data._mem);
	} else if (type == STRING) {
		return NodePath(operator String());
	} else {
		return NodePath();
	}
}

Variant::operator RefPtr() const {
	if (type == OBJECT) {
		return _get_obj().ref;
	} else {
		return RefPtr();
	}
}

Variant::operator ::RID() const {
	if (type == RID) {
		return *reinterpret_cast<const ::RID *>(_data._mem);
	} else if (type == OBJECT) {
		if (!_get_obj().ref.is_null()) {
			return _get_obj().ref.get_rid();
		} else {
			Object *obj = likely(_get_obj().rc) ? _get_obj().rc->get_ptr() : nullptr;
			if (unlikely(!obj)) {
				if (_get_obj().rc) {
					ERR_PRINT("Attempted get RID on a deleted object.");
				}
				return ::RID();
			}
			Variant::CallError ce;
			Variant ret = obj->call(CoreStringNames::get_singleton()->get_rid, nullptr, 0, ce);
			if (ce.error == Variant::CallError::CALL_OK && ret.get_type() == Variant::RID) {
				return ret;
			} else {
				return ::RID();
			}
		}
	} else {
		return ::RID();
	}
}

Variant::operator Object *() const {
	if (type == OBJECT) {
		return _OBJ_PTR(*this);
	} else {
		return nullptr;
	}
}
Variant::operator Node *() const {
	if (type == OBJECT) {
		Object *obj = _get_obj().rc ? _get_obj().rc->get_ptr() : nullptr;
		return Object::cast_to<Node>(obj);
	}
	return nullptr;
}
Variant::operator Control *() const {
	if (type == OBJECT) {
		Object *obj = _get_obj().rc ? _get_obj().rc->get_ptr() : nullptr;
		return Object::cast_to<Control>(obj);
	}
	return nullptr;
}

Variant::operator Dictionary() const {
	if (type == DICTIONARY) {
		return *reinterpret_cast<const Dictionary *>(_data._mem);
	} else {
		return Dictionary();
	}
}

template <class DA, class SA>
inline DA _convert_array(const SA &p_array) {
	DA da;
	da.resize(p_array.size());

	for (int i = 0; i < p_array.size(); i++) {
		da.set(i, Variant(p_array.get(i)));
	}

	return da;
}

template <class DA>
inline DA _convert_array_from_variant(const Variant &p_variant) {
	switch (p_variant.get_type()) {
		case Variant::ARRAY: {
			return _convert_array<DA, Array>(p_variant.operator Array());
		}
		case Variant::POOL_BYTE_ARRAY: {
			return _convert_array<DA, PoolVector<uint8_t>>(p_variant.operator PoolVector<uint8_t>());
		}
		case Variant::POOL_INT_ARRAY: {
			return _convert_array<DA, PoolVector<int>>(p_variant.operator PoolVector<int>());
		}
		case Variant::POOL_REAL_ARRAY: {
			return _convert_array<DA, PoolVector<real_t>>(p_variant.operator PoolVector<real_t>());
		}
		case Variant::POOL_STRING_ARRAY: {
			return _convert_array<DA, PoolVector<String>>(p_variant.operator PoolVector<String>());
		}
		case Variant::POOL_VECTOR2_ARRAY: {
			return _convert_array<DA, PoolVector<Vector2>>(p_variant.operator PoolVector<Vector2>());
		}
		case Variant::POOL_VECTOR2I_ARRAY: {
			return _convert_array<DA, PoolVector<Vector2i>>(p_variant.operator PoolVector<Vector2i>());
		}
		case Variant::POOL_VECTOR3_ARRAY: {
			return _convert_array<DA, PoolVector<Vector3>>(p_variant.operator PoolVector<Vector3>());
		}
		case Variant::POOL_VECTOR3I_ARRAY: {
			return _convert_array<DA, PoolVector<Vector3i>>(p_variant.operator PoolVector<Vector3i>());
		}
		case Variant::POOL_VECTOR4_ARRAY: {
			return _convert_array<DA, PoolVector<Vector4>>(p_variant.operator PoolVector<Vector4>());
		}
		case Variant::POOL_VECTOR4I_ARRAY: {
			return _convert_array<DA, PoolVector<Vector4i>>(p_variant.operator PoolVector<Vector4i>());
		}
		case Variant::POOL_COLOR_ARRAY: {
			return _convert_array<DA, PoolVector<Color>>(p_variant.operator PoolVector<Color>());
		}
		default: {
			return DA();
		}
	}
}

Variant::operator Array() const {
	if (type == ARRAY) {
		return *reinterpret_cast<const Array *>(_data._mem);
	} else {
		return _convert_array_from_variant<Array>(*this);
	}
}

Variant::operator PoolVector<uint8_t>() const {
	if (type == POOL_BYTE_ARRAY) {
		return *reinterpret_cast<const PoolVector<uint8_t> *>(_data._mem);
	} else {
		return _convert_array_from_variant<PoolVector<uint8_t>>(*this);
	}
}
Variant::operator PoolVector<int>() const {
	if (type == POOL_INT_ARRAY) {
		return *reinterpret_cast<const PoolVector<int> *>(_data._mem);
	} else {
		return _convert_array_from_variant<PoolVector<int>>(*this);
	}
}
Variant::operator PoolVector<real_t>() const {
	if (type == POOL_REAL_ARRAY) {
		return *reinterpret_cast<const PoolVector<real_t> *>(_data._mem);
	} else {
		return _convert_array_from_variant<PoolVector<real_t>>(*this);
	}
}

Variant::operator PoolVector<String>() const {
	if (type == POOL_STRING_ARRAY) {
		return *reinterpret_cast<const PoolVector<String> *>(_data._mem);
	} else {
		return _convert_array_from_variant<PoolVector<String>>(*this);
	}
}
Variant::operator PoolVector<Vector2>() const {
	if (type == POOL_VECTOR2_ARRAY) {
		return *reinterpret_cast<const PoolVector<Vector2> *>(_data._mem);
	} else {
		return _convert_array_from_variant<PoolVector<Vector2>>(*this);
	}
}
Variant::operator PoolVector<Vector2i>() const {
	if (type == POOL_VECTOR2I_ARRAY) {
		return *reinterpret_cast<const PoolVector<Vector2i> *>(_data._mem);
	} else {
		return _convert_array_from_variant<PoolVector<Vector2i>>(*this);
	}
}
Variant::operator PoolVector<Vector3>() const {
	if (type == POOL_VECTOR3_ARRAY) {
		return *reinterpret_cast<const PoolVector<Vector3> *>(_data._mem);
	} else {
		return _convert_array_from_variant<PoolVector<Vector3>>(*this);
	}
}
Variant::operator PoolVector<Vector3i>() const {
	if (type == POOL_VECTOR3I_ARRAY) {
		return *reinterpret_cast<const PoolVector<Vector3i> *>(_data._mem);
	} else {
		return _convert_array_from_variant<PoolVector<Vector3i>>(*this);
	}
}
Variant::operator PoolVector<Vector4>() const {
	if (type == POOL_VECTOR4_ARRAY) {
		return *reinterpret_cast<const PoolVector<Vector4> *>(_data._mem);
	} else {
		return _convert_array_from_variant<PoolVector<Vector4>>(*this);
	}
}
Variant::operator PoolVector<Vector4i>() const {
	if (type == POOL_VECTOR4I_ARRAY) {
		return *reinterpret_cast<const PoolVector<Vector4i> *>(_data._mem);
	} else {
		return _convert_array_from_variant<PoolVector<Vector4i>>(*this);
	}
}
Variant::operator PoolVector<Color>() const {
	if (type == POOL_COLOR_ARRAY) {
		return *reinterpret_cast<const PoolVector<Color> *>(_data._mem);
	} else {
		return _convert_array_from_variant<PoolVector<Color>>(*this);
	}
}

/* helpers */

Variant::operator Vector<::RID>() const {
	Array va = operator Array();
	Vector<::RID> rids;
	rids.resize(va.size());
	for (int i = 0; i < rids.size(); i++) {
		rids.write[i] = va[i];
	}
	return rids;
}

Variant::operator PoolVector<Plane>() const {
	Array va = operator Array();
	PoolVector<Plane> planes;
	int va_size = va.size();
	if (va_size == 0) {
		return planes;
	}

	planes.resize(va_size);
	PoolVector<Plane>::Write w = planes.write();

	for (int i = 0; i < va_size; i++) {
		w[i] = va[i];
	}

	return planes;
}

Variant::operator PoolVector<Face3>() const {
	PoolVector<Vector3> va = operator PoolVector<Vector3>();
	PoolVector<Face3> faces;
	int va_size = va.size();
	if (va_size == 0) {
		return faces;
	}

	faces.resize(va_size / 3);
	PoolVector<Face3>::Write w = faces.write();
	PoolVector<Vector3>::Read r = va.read();

	for (int i = 0; i < va_size; i++) {
		w[i / 3].vertex[i % 3] = r[i];
	}

	return faces;
}

Variant::operator Vector<Plane>() const {
	Array va = operator Array();
	Vector<Plane> planes;
	int va_size = va.size();
	if (va_size == 0) {
		return planes;
	}

	planes.resize(va_size);

	for (int i = 0; i < va_size; i++) {
		planes.write[i] = va[i];
	}

	return planes;
}

Variant::operator Vector<Variant>() const {
	Array from = operator Array();
	Vector<Variant> to;
	int len = from.size();
	to.resize(len);
	for (int i = 0; i < len; i++) {
		to.write[i] = from[i];
	}
	return to;
}

Variant::operator Vector<uint8_t>() const {
	PoolVector<uint8_t> from = operator PoolVector<uint8_t>();
	Vector<uint8_t> to;
	int len = from.size();
	to.resize(len);
	for (int i = 0; i < len; i++) {
		to.write[i] = from[i];
	}
	return to;
}
Variant::operator Vector<int>() const {
	PoolVector<int> from = operator PoolVector<int>();
	Vector<int> to;
	int len = from.size();
	to.resize(len);
	for (int i = 0; i < len; i++) {
		to.write[i] = from[i];
	}
	return to;
}
Variant::operator Vector<real_t>() const {
	PoolVector<real_t> from = operator PoolVector<real_t>();
	Vector<real_t> to;
	int len = from.size();
	to.resize(len);
	for (int i = 0; i < len; i++) {
		to.write[i] = from[i];
	}
	return to;
}

Variant::operator Vector<String>() const {
	PoolVector<String> from = operator PoolVector<String>();
	Vector<String> to;
	int len = from.size();
	to.resize(len);
	for (int i = 0; i < len; i++) {
		to.write[i] = from[i];
	}
	return to;
}
Variant::operator Vector<StringName>() const {
	PoolVector<String> from = operator PoolVector<String>();
	Vector<StringName> to;
	int len = from.size();
	to.resize(len);
	for (int i = 0; i < len; i++) {
		to.write[i] = from[i];
	}
	return to;
}

Variant::operator Vector<Vector2>() const {
	PoolVector<Vector2> from = operator PoolVector<Vector2>();
	Vector<Vector2> to;
	int len = from.size();
	if (len == 0) {
		return Vector<Vector2>();
	}
	to.resize(len);
	PoolVector<Vector2>::Read r = from.read();
	Vector2 *w = to.ptrw();
	for (int i = 0; i < len; i++) {
		w[i] = r[i];
	}
	return to;
}
Variant::operator Vector<Vector2i>() const {
	PoolVector<Vector2i> from = operator PoolVector<Vector2i>();
	Vector<Vector2i> to;
	int len = from.size();
	if (len == 0) {
		return Vector<Vector2i>();
	}
	to.resize(len);
	PoolVector<Vector2i>::Read r = from.read();
	Vector2i *w = to.ptrw();
	for (int i = 0; i < len; i++) {
		w[i] = r[i];
	}
	return to;
}

Variant::operator Vector<Vector3>() const {
	PoolVector<Vector3> from = operator PoolVector<Vector3>();
	Vector<Vector3> to;
	int len = from.size();
	if (len == 0) {
		return Vector<Vector3>();
	}
	to.resize(len);
	PoolVector<Vector3>::Read r = from.read();
	Vector3 *w = to.ptrw();
	for (int i = 0; i < len; i++) {
		w[i] = r[i];
	}
	return to;
}
Variant::operator Vector<Vector3i>() const {
	PoolVector<Vector3i> from = operator PoolVector<Vector3i>();
	Vector<Vector3i> to;
	int len = from.size();
	if (len == 0) {
		return Vector<Vector3i>();
	}
	to.resize(len);
	PoolVector<Vector3i>::Read r = from.read();
	Vector3i *w = to.ptrw();
	for (int i = 0; i < len; i++) {
		w[i] = r[i];
	}
	return to;
}

Variant::operator Vector<Vector4>() const {
	PoolVector<Vector4> from = operator PoolVector<Vector4>();
	Vector<Vector4> to;
	int len = from.size();
	if (len == 0) {
		return Vector<Vector4>();
	}
	to.resize(len);
	PoolVector<Vector4>::Read r = from.read();
	Vector4 *w = to.ptrw();
	for (int i = 0; i < len; i++) {
		w[i] = r[i];
	}
	return to;
}
Variant::operator Vector<Vector4i>() const {
	PoolVector<Vector4i> from = operator PoolVector<Vector4i>();
	Vector<Vector4i> to;
	int len = from.size();
	if (len == 0) {
		return Vector<Vector4i>();
	}
	to.resize(len);
	PoolVector<Vector4i>::Read r = from.read();
	Vector4i *w = to.ptrw();
	for (int i = 0; i < len; i++) {
		w[i] = r[i];
	}
	return to;
}

Variant::operator Vector<Color>() const {
	PoolVector<Color> from = operator PoolVector<Color>();
	Vector<Color> to;
	int len = from.size();
	if (len == 0) {
		return Vector<Color>();
	}
	to.resize(len);
	PoolVector<Color>::Read r = from.read();
	Color *w = to.ptrw();
	for (int i = 0; i < len; i++) {
		w[i] = r[i];
	}
	return to;
}

Variant::operator Margin() const {
	return (Margin) operator int();
}
Variant::operator Side() const {
	return (Side) operator int();
}
Variant::operator Orientation() const {
	return (Orientation) operator int();
}

Variant::operator IP_Address() const {
	if (type == POOL_REAL_ARRAY || type == POOL_INT_ARRAY || type == POOL_BYTE_ARRAY) {
		PoolVector<int> addr = operator PoolVector<int>();
		if (addr.size() == 4) {
			return IP_Address(addr.get(0), addr.get(1), addr.get(2), addr.get(3));
		}
	}

	return IP_Address(operator String());
}

Variant::Variant(bool p_bool) {
	type = BOOL;
	_data._bool = p_bool;
}

/*
Variant::Variant(long unsigned int p_long) {

	type=INT;
	_data._int=p_long;
};
*/

Variant::Variant(signed int p_int) {
	type = INT;
	_data._int = p_int;
}
Variant::Variant(unsigned int p_int) {
	type = INT;
	_data._int = p_int;
}

#ifdef NEED_LONG_INT

Variant::Variant(signed long p_int) {
	type = INT;
	_data._int = p_int;
}
Variant::Variant(unsigned long p_int) {
	type = INT;
	_data._int = p_int;
}
#endif

Variant::Variant(int64_t p_int) {
	type = INT;
	_data._int = p_int;
}

Variant::Variant(uint64_t p_int) {
	type = INT;
	_data._int = p_int;
}

Variant::Variant(signed short p_short) {
	type = INT;
	_data._int = p_short;
}
Variant::Variant(unsigned short p_short) {
	type = INT;
	_data._int = p_short;
}
Variant::Variant(signed char p_char) {
	type = INT;
	_data._int = p_char;
}
Variant::Variant(unsigned char p_char) {
	type = INT;
	_data._int = p_char;
}
Variant::Variant(float p_float) {
	type = REAL;
	_data._real = p_float;
}
Variant::Variant(double p_double) {
	type = REAL;
	_data._real = p_double;
}

Variant::Variant(const StringName &p_string) {
	type = STRING_NAME;
	memnew_placement(_data._mem, StringName(p_string));
}
Variant::Variant(const String &p_string) {
	type = STRING;
	memnew_placement(_data._mem, String(p_string));
}

Variant::Variant(const char *const p_cstring) {
	type = STRING;
	memnew_placement(_data._mem, String((const char *)p_cstring));
}

Variant::Variant(const CharType *p_wstring) {
	type = STRING;
	memnew_placement(_data._mem, String(p_wstring));
}

Variant::Variant(const Rect2 &p_rect2) {
	type = RECT2;
	memnew_placement(_data._mem, Rect2(p_rect2));
}
Variant::Variant(const Rect2i &p_rect2) {
	type = RECT2I;
	memnew_placement(_data._mem, Rect2i(p_rect2));
}

Variant::Variant(const Vector2 &p_vector2) {
	type = VECTOR2;
	memnew_placement(_data._mem, Vector2(p_vector2));
}
Variant::Variant(const Vector2i &p_vector2) {
	type = VECTOR2I;
	memnew_placement(_data._mem, Vector2i(p_vector2));
}

Variant::Variant(const Vector3 &p_vector3) {
	type = VECTOR3;
	memnew_placement(_data._mem, Vector3(p_vector3));
}
Variant::Variant(const Vector3i &p_vector3) {
	type = VECTOR3I;
	memnew_placement(_data._mem, Vector3i(p_vector3));
}

Variant::Variant(const Vector4 &p_vector4) {
	type = VECTOR4;
	memnew_placement(_data._mem, Vector4(p_vector4));
}
Variant::Variant(const Vector4i &p_vector4) {
	type = VECTOR4I;
	memnew_placement(_data._mem, Vector4i(p_vector4));
}

Variant::Variant(const Plane &p_plane) {
	type = PLANE;
	memnew_placement(_data._mem, Plane(p_plane));
}
Variant::Variant(const ::AABB &p_aabb) {
	type = AABB;
	_data._aabb = memnew(::AABB(p_aabb));
}

Variant::Variant(const Basis &p_matrix) {
	type = BASIS;
	_data._basis = memnew(Basis(p_matrix));
}

Variant::Variant(const Quaternion &p_quat) {
	type = QUATERNION;
	memnew_placement(_data._mem, Quaternion(p_quat));
}
Variant::Variant(const Transform &p_transform) {
	type = TRANSFORM;
	_data._transform = memnew(Transform(p_transform));
}

Variant::Variant(const Transform2D &p_transform) {
	type = TRANSFORM2D;
	_data._transform2d = memnew(Transform2D(p_transform));
}

Variant::Variant(const Projection &p_projection) {
	type = PROJECTION;
	_data._projection = memnew(Projection(p_projection));
}

Variant::Variant(const Color &p_color) {
	type = COLOR;
	memnew_placement(_data._mem, Color(p_color));
}

Variant::Variant(const NodePath &p_node_path) {
	type = NODE_PATH;
	memnew_placement(_data._mem, NodePath(p_node_path));
}

Variant::Variant(const RefPtr &p_resource) {
	type = OBJECT;
	memnew_placement(_data._mem, ObjData);
	_get_obj().rc = nullptr;
	_get_obj().ref = p_resource;
}

Variant::Variant(const ::RID &p_rid) {
	type = RID;
	memnew_placement(_data._mem, ::RID(p_rid));
}

Variant::Variant(const Object *p_object) {
	type = OBJECT;
	Object *obj = const_cast<Object *>(p_object);

	memnew_placement(_data._mem, ObjData);
	Reference *ref = Object::cast_to<Reference>(obj);
	if (unlikely(ref)) {
		*reinterpret_cast<Ref<Reference> *>(_get_obj().ref.get_data()) = Ref<Reference>(ref);
		_get_obj().rc = nullptr;
	} else {
		_get_obj().rc = likely(obj) ? obj->_use_rc() : nullptr;
	}
}

Variant::Variant(const Dictionary &p_dictionary) {
	type = DICTIONARY;
	memnew_placement(_data._mem, Dictionary(p_dictionary));
}

Variant::Variant(const Array &p_array) {
	type = ARRAY;
	memnew_placement(_data._mem, Array(p_array));
}

Variant::Variant(const PoolVector<Plane> &p_array) {
	type = ARRAY;

	Array *plane_array = memnew_placement(_data._mem, Array);

	plane_array->resize(p_array.size());

	for (int i = 0; i < p_array.size(); i++) {
		plane_array->operator[](i) = Variant(p_array[i]);
	}
}

Variant::Variant(const Vector<Plane> &p_array) {
	type = ARRAY;

	Array *plane_array = memnew_placement(_data._mem, Array);

	plane_array->resize(p_array.size());

	for (int i = 0; i < p_array.size(); i++) {
		plane_array->operator[](i) = Variant(p_array[i]);
	}
}

Variant::Variant(const Vector<::RID> &p_array) {
	type = ARRAY;

	Array *rid_array = memnew_placement(_data._mem, Array);

	rid_array->resize(p_array.size());

	for (int i = 0; i < p_array.size(); i++) {
		rid_array->set(i, Variant(p_array[i]));
	}
}

Variant::Variant(const PoolVector<uint8_t> &p_raw_array) {
	type = POOL_BYTE_ARRAY;
	memnew_placement(_data._mem, PoolVector<uint8_t>(p_raw_array));
}
Variant::Variant(const PoolVector<int> &p_int_array) {
	type = POOL_INT_ARRAY;
	memnew_placement(_data._mem, PoolVector<int>(p_int_array));
}
Variant::Variant(const PoolVector<real_t> &p_real_array) {
	type = POOL_REAL_ARRAY;
	memnew_placement(_data._mem, PoolVector<real_t>(p_real_array));
}
Variant::Variant(const PoolVector<String> &p_string_array) {
	type = POOL_STRING_ARRAY;
	memnew_placement(_data._mem, PoolVector<String>(p_string_array));
}
Variant::Variant(const PoolVector<Vector2> &p_vector2_array) {
	type = POOL_VECTOR2_ARRAY;
	memnew_placement(_data._mem, PoolVector<Vector2>(p_vector2_array));
}
Variant::Variant(const PoolVector<Vector2i> &p_vector2_array) {
	type = POOL_VECTOR2I_ARRAY;
	memnew_placement(_data._mem, PoolVector<Vector2i>(p_vector2_array));
}
Variant::Variant(const PoolVector<Vector3> &p_vector3_array) {
	type = POOL_VECTOR3_ARRAY;
	memnew_placement(_data._mem, PoolVector<Vector3>(p_vector3_array));
}
Variant::Variant(const PoolVector<Vector3i> &p_vector3_array) {
	type = POOL_VECTOR3I_ARRAY;
	memnew_placement(_data._mem, PoolVector<Vector3i>(p_vector3_array));
}

Variant::Variant(const PoolVector<Vector4> &p_vector4_array) {
	type = POOL_VECTOR4_ARRAY;
	memnew_placement(_data._mem, PoolVector<Vector4>(p_vector4_array));
}
Variant::Variant(const PoolVector<Vector4i> &p_vector4_array) {
	type = POOL_VECTOR4I_ARRAY;
	memnew_placement(_data._mem, PoolVector<Vector4i>(p_vector4_array));
}

Variant::Variant(const PoolVector<Color> &p_color_array) {
	type = POOL_COLOR_ARRAY;
	memnew_placement(_data._mem, PoolVector<Color>(p_color_array));
}

Variant::Variant(const PoolVector<Face3> &p_face_array) {
	PoolVector<Vector3> vertices;
	int face_count = p_face_array.size();
	vertices.resize(face_count * 3);

	if (face_count) {
		PoolVector<Face3>::Read r = p_face_array.read();
		PoolVector<Vector3>::Write w = vertices.write();

		for (int i = 0; i < face_count; i++) {
			for (int j = 0; j < 3; j++) {
				w[i * 3 + j] = r[i].vertex[j];
			}
		}
	}

	type = NIL;

	*this = vertices;
}

/* helpers */

Variant::Variant(const Vector<Variant> &p_array) {
	type = NIL;
	Array v;
	int len = p_array.size();
	v.resize(len);
	for (int i = 0; i < len; i++) {
		v.set(i, p_array[i]);
	}
	*this = v;
}

Variant::Variant(const Vector<uint8_t> &p_array) {
	type = NIL;
	PoolVector<uint8_t> v;
	int len = p_array.size();
	v.resize(len);
	for (int i = 0; i < len; i++) {
		v.set(i, p_array[i]);
	}
	*this = v;
}

Variant::Variant(const Vector<int> &p_array) {
	type = NIL;
	PoolVector<int> v;
	int len = p_array.size();
	v.resize(len);
	for (int i = 0; i < len; i++) {
		v.set(i, p_array[i]);
	}
	*this = v;
}

Variant::Variant(const Vector<real_t> &p_array) {
	type = NIL;
	PoolVector<real_t> v;
	int len = p_array.size();
	v.resize(len);
	for (int i = 0; i < len; i++) {
		v.set(i, p_array[i]);
	}
	*this = v;
}

Variant::Variant(const Vector<String> &p_array) {
	type = NIL;
	PoolVector<String> v;
	int len = p_array.size();
	v.resize(len);
	for (int i = 0; i < len; i++) {
		v.set(i, p_array[i]);
	}
	*this = v;
}

Variant::Variant(const Vector<StringName> &p_array) {
	type = NIL;
	PoolVector<String> v;
	int len = p_array.size();
	v.resize(len);
	for (int i = 0; i < len; i++) {
		v.set(i, p_array[i]);
	}
	*this = v;
}

Variant::Variant(const Vector<Vector2> &p_array) {
	type = NIL;
	PoolVector<Vector2> v;
	int len = p_array.size();
	if (len > 0) {
		v.resize(len);
		PoolVector<Vector2>::Write w = v.write();
		const Vector2 *r = p_array.ptr();

		for (int i = 0; i < len; i++) {
			w[i] = r[i];
		}
	}
	*this = v;
}
Variant::Variant(const Vector<Vector2i> &p_array) {
	type = NIL;
	PoolVector<Vector2i> v;
	int len = p_array.size();
	if (len > 0) {
		v.resize(len);
		PoolVector<Vector2i>::Write w = v.write();
		const Vector2i *r = p_array.ptr();

		for (int i = 0; i < len; i++) {
			w[i] = r[i];
		}
	}
	*this = v;
}

Variant::Variant(const Vector<Vector3> &p_array) {
	type = NIL;
	PoolVector<Vector3> v;
	int len = p_array.size();
	if (len > 0) {
		v.resize(len);
		PoolVector<Vector3>::Write w = v.write();
		const Vector3 *r = p_array.ptr();

		for (int i = 0; i < len; i++) {
			w[i] = r[i];
		}
	}
	*this = v;
}
Variant::Variant(const Vector<Vector3i> &p_array) {
	type = NIL;
	PoolVector<Vector3i> v;
	int len = p_array.size();
	if (len > 0) {
		v.resize(len);
		PoolVector<Vector3i>::Write w = v.write();
		const Vector3i *r = p_array.ptr();

		for (int i = 0; i < len; i++) {
			w[i] = r[i];
		}
	}
	*this = v;
}

Variant::Variant(const Vector<Vector4> &p_array) {
	type = NIL;
	PoolVector<Vector4> v;
	int len = p_array.size();
	if (len > 0) {
		v.resize(len);
		PoolVector<Vector4>::Write w = v.write();
		const Vector4 *r = p_array.ptr();

		for (int i = 0; i < len; i++) {
			w[i] = r[i];
		}
	}
	*this = v;
}
Variant::Variant(const Vector<Vector4i> &p_array) {
	type = NIL;
	PoolVector<Vector4i> v;
	int len = p_array.size();
	if (len > 0) {
		v.resize(len);
		PoolVector<Vector4i>::Write w = v.write();
		const Vector4i *r = p_array.ptr();

		for (int i = 0; i < len; i++) {
			w[i] = r[i];
		}
	}
	*this = v;
}

Variant::Variant(const Vector<Color> &p_array) {
	type = NIL;
	PoolVector<Color> v;
	int len = p_array.size();
	v.resize(len);
	for (int i = 0; i < len; i++) {
		v.set(i, p_array[i]);
	}
	*this = v;
}

void Variant::operator=(const Variant &p_variant) {
	if (unlikely(this == &p_variant)) {
		return;
	}

	if (unlikely(type != p_variant.type)) {
		reference(p_variant);
		return;
	}

	switch (p_variant.type) {
		case NIL: {
			// none
		} break;

		// atomic types
		case BOOL: {
			_data._bool = p_variant._data._bool;
		} break;
		case INT: {
			_data._int = p_variant._data._int;
		} break;
		case REAL: {
			_data._real = p_variant._data._real;
		} break;
		case STRING: {
			*reinterpret_cast<String *>(_data._mem) = *reinterpret_cast<const String *>(p_variant._data._mem);
		} break;

			// math types
		case RECT2: {
			*reinterpret_cast<Rect2 *>(_data._mem) = *reinterpret_cast<const Rect2 *>(p_variant._data._mem);
		} break;
		case RECT2I: {
			*reinterpret_cast<Rect2i *>(_data._mem) = *reinterpret_cast<const Rect2i *>(p_variant._data._mem);
		} break;
		case VECTOR2: {
			*reinterpret_cast<Vector2 *>(_data._mem) = *reinterpret_cast<const Vector2 *>(p_variant._data._mem);
		} break;
		case VECTOR2I: {
			*reinterpret_cast<Vector2i *>(_data._mem) = *reinterpret_cast<const Vector2i *>(p_variant._data._mem);
		} break;
		case VECTOR3: {
			*reinterpret_cast<Vector3 *>(_data._mem) = *reinterpret_cast<const Vector3 *>(p_variant._data._mem);
		} break;
		case VECTOR3I: {
			*reinterpret_cast<Vector3i *>(_data._mem) = *reinterpret_cast<const Vector3i *>(p_variant._data._mem);
		} break;
		case VECTOR4: {
			*reinterpret_cast<Vector4 *>(_data._mem) = *reinterpret_cast<const Vector4 *>(p_variant._data._mem);
		} break;
		case VECTOR4I: {
			*reinterpret_cast<Vector4i *>(_data._mem) = *reinterpret_cast<const Vector4i *>(p_variant._data._mem);
		} break;

		case PLANE: {
			*reinterpret_cast<Plane *>(_data._mem) = *reinterpret_cast<const Plane *>(p_variant._data._mem);
		} break;
		case QUATERNION: {
			*reinterpret_cast<Quaternion *>(_data._mem) = *reinterpret_cast<const Quaternion *>(p_variant._data._mem);
		} break;
		case AABB: {
			*_data._aabb = *(p_variant._data._aabb);
		} break;
		case BASIS: {
			*_data._basis = *(p_variant._data._basis);
		} break;
		case TRANSFORM: {
			*_data._transform = *(p_variant._data._transform);
		} break;
		case TRANSFORM2D: {
			*_data._transform2d = *(p_variant._data._transform2d);
		} break;
		case PROJECTION: {
			*_data._projection = *(p_variant._data._projection);
		} break;

		// misc types
		case COLOR: {
			*reinterpret_cast<Color *>(_data._mem) = *reinterpret_cast<const Color *>(p_variant._data._mem);
		} break;
		case NODE_PATH: {
			*reinterpret_cast<NodePath *>(_data._mem) = *reinterpret_cast<const NodePath *>(p_variant._data._mem);
		} break;
		case RID: {
			*reinterpret_cast<::RID *>(_data._mem) = *reinterpret_cast<const ::RID *>(p_variant._data._mem);
		} break;
		case OBJECT: {
			if (likely(_get_obj().rc)) {
				if (unlikely(_get_obj().rc->decrement())) {
					memdelete(_get_obj().rc);
				}
			}
			*reinterpret_cast<ObjData *>(_data._mem) = p_variant._get_obj();
			if (likely(_get_obj().rc)) {
				_get_obj().rc->increment();
			}
		} break;
		case STRING_NAME: {
			*reinterpret_cast<StringName *>(_data._mem) = *reinterpret_cast<const StringName *>(p_variant._data._mem);
		} break;
		case DICTIONARY: {
			*reinterpret_cast<Dictionary *>(_data._mem) = *reinterpret_cast<const Dictionary *>(p_variant._data._mem);
		} break;
		case ARRAY: {
			*reinterpret_cast<Array *>(_data._mem) = *reinterpret_cast<const Array *>(p_variant._data._mem);
		} break;

		// arrays
		case POOL_BYTE_ARRAY: {
			*reinterpret_cast<PoolVector<uint8_t> *>(_data._mem) = *reinterpret_cast<const PoolVector<uint8_t> *>(p_variant._data._mem);
		} break;
		case POOL_INT_ARRAY: {
			*reinterpret_cast<PoolVector<int> *>(_data._mem) = *reinterpret_cast<const PoolVector<int> *>(p_variant._data._mem);
		} break;
		case POOL_REAL_ARRAY: {
			*reinterpret_cast<PoolVector<real_t> *>(_data._mem) = *reinterpret_cast<const PoolVector<real_t> *>(p_variant._data._mem);
		} break;
		case POOL_STRING_ARRAY: {
			*reinterpret_cast<PoolVector<String> *>(_data._mem) = *reinterpret_cast<const PoolVector<String> *>(p_variant._data._mem);
		} break;
		case POOL_VECTOR2_ARRAY: {
			*reinterpret_cast<PoolVector<Vector2> *>(_data._mem) = *reinterpret_cast<const PoolVector<Vector2> *>(p_variant._data._mem);
		} break;
		case POOL_VECTOR2I_ARRAY: {
			*reinterpret_cast<PoolVector<Vector2i> *>(_data._mem) = *reinterpret_cast<const PoolVector<Vector2i> *>(p_variant._data._mem);
		} break;
		case POOL_VECTOR3_ARRAY: {
			*reinterpret_cast<PoolVector<Vector3> *>(_data._mem) = *reinterpret_cast<const PoolVector<Vector3> *>(p_variant._data._mem);
		} break;
		case POOL_VECTOR3I_ARRAY: {
			*reinterpret_cast<PoolVector<Vector3i> *>(_data._mem) = *reinterpret_cast<const PoolVector<Vector3i> *>(p_variant._data._mem);
		} break;
		case POOL_VECTOR4_ARRAY: {
			*reinterpret_cast<PoolVector<Vector4> *>(_data._mem) = *reinterpret_cast<const PoolVector<Vector4> *>(p_variant._data._mem);
		} break;
		case POOL_VECTOR4I_ARRAY: {
			*reinterpret_cast<PoolVector<Vector4i> *>(_data._mem) = *reinterpret_cast<const PoolVector<Vector4i> *>(p_variant._data._mem);
		} break;
		case POOL_COLOR_ARRAY: {
			*reinterpret_cast<PoolVector<Color> *>(_data._mem) = *reinterpret_cast<const PoolVector<Color> *>(p_variant._data._mem);
		} break;
		default: {
		}
	}
}

Variant::Variant(const IP_Address &p_address) {
	type = STRING;
	memnew_placement(_data._mem, String(p_address));
}

Variant::Variant(const Variant &p_variant) {
	type = NIL;
	reference(p_variant);
}

/*
Variant::~Variant() {

	clear();
}*/

uint32_t Variant::hash() const {
	return recursive_hash(0);
}

uint32_t Variant::recursive_hash(int p_recursion_count) const {
	switch (type) {
		case NIL: {
			return 0;
		} break;

		case BOOL: {
			return _data._bool ? 1 : 0;
		} break;
		case INT: {
			return hash_one_uint64((uint64_t)_data._int);
		} break;
		case REAL: {
			return hash_murmur3_one_float(_data._real);
		} break;
		case STRING: {
			return reinterpret_cast<const String *>(_data._mem)->hash();
		} break;

			// math types
		case RECT2: {
			return HashMapHasherDefault::hash(*reinterpret_cast<const Rect2 *>(_data._mem));
		} break;
		case RECT2I: {
			return HashMapHasherDefault::hash(*reinterpret_cast<const Rect2i *>(_data._mem));
		} break;
		case VECTOR2: {
			return HashMapHasherDefault::hash(*reinterpret_cast<const Vector2 *>(_data._mem));
		} break;
		case VECTOR2I: {
			return HashMapHasherDefault::hash(*reinterpret_cast<const Vector2i *>(_data._mem));
		} break;
		case VECTOR3: {
			return HashMapHasherDefault::hash(*reinterpret_cast<const Vector3 *>(_data._mem));
		} break;
		case VECTOR3I: {
			return HashMapHasherDefault::hash(*reinterpret_cast<const Vector3i *>(_data._mem));
		} break;
		case VECTOR4: {
			return HashMapHasherDefault::hash(*reinterpret_cast<const Vector4 *>(_data._mem));
		} break;
		case VECTOR4I: {
			return HashMapHasherDefault::hash(*reinterpret_cast<const Vector4i *>(_data._mem));
		} break;
		case PLANE: {
			uint32_t h = HASH_MURMUR3_SEED;
			const Plane &p = *reinterpret_cast<const Plane *>(_data._mem);
			h = hash_murmur3_one_real(p.normal.x, h);
			h = hash_murmur3_one_real(p.normal.y, h);
			h = hash_murmur3_one_real(p.normal.z, h);
			h = hash_murmur3_one_real(p.d, h);
			return hash_fmix32(h);
		} break;
		case QUATERNION: {
			uint32_t h = HASH_MURMUR3_SEED;
			const Quaternion &q = *reinterpret_cast<const Quaternion *>(_data._mem);
			h = hash_murmur3_one_real(q.x, h);
			h = hash_murmur3_one_real(q.y, h);
			h = hash_murmur3_one_real(q.z, h);
			h = hash_murmur3_one_real(q.w, h);
			return hash_fmix32(h);
		} break;
		case AABB: {
			return HashMapHasherDefault::hash(*_data._aabb);
		} break;
		case BASIS: {
			uint32_t h = HASH_MURMUR3_SEED;
			const Basis &b = *_data._basis;
			h = hash_murmur3_one_real(b[0].x, h);
			h = hash_murmur3_one_real(b[0].y, h);
			h = hash_murmur3_one_real(b[0].z, h);
			h = hash_murmur3_one_real(b[1].x, h);
			h = hash_murmur3_one_real(b[1].y, h);
			h = hash_murmur3_one_real(b[1].z, h);
			h = hash_murmur3_one_real(b[2].x, h);
			h = hash_murmur3_one_real(b[2].y, h);
			h = hash_murmur3_one_real(b[2].z, h);
			return hash_fmix32(h);
		} break;
		case TRANSFORM: {
			uint32_t h = HASH_MURMUR3_SEED;
			const Transform &t = *_data._transform;
			h = hash_murmur3_one_real(t.basis[0].x, h);
			h = hash_murmur3_one_real(t.basis[0].y, h);
			h = hash_murmur3_one_real(t.basis[0].z, h);
			h = hash_murmur3_one_real(t.basis[1].x, h);
			h = hash_murmur3_one_real(t.basis[1].y, h);
			h = hash_murmur3_one_real(t.basis[1].z, h);
			h = hash_murmur3_one_real(t.basis[2].x, h);
			h = hash_murmur3_one_real(t.basis[2].y, h);
			h = hash_murmur3_one_real(t.basis[2].z, h);
			h = hash_murmur3_one_real(t.origin.x, h);
			h = hash_murmur3_one_real(t.origin.y, h);
			h = hash_murmur3_one_real(t.origin.z, h);
			return hash_fmix32(h);
		} break;
		case TRANSFORM2D: {
			uint32_t h = HASH_MURMUR3_SEED;
			const Transform2D &t = *_data._transform2d;
			h = hash_murmur3_one_real(t[0].x, h);
			h = hash_murmur3_one_real(t[0].y, h);
			h = hash_murmur3_one_real(t[1].x, h);
			h = hash_murmur3_one_real(t[1].y, h);
			h = hash_murmur3_one_real(t[2].x, h);
			h = hash_murmur3_one_real(t[2].y, h);

			return hash_fmix32(h);
		} break;
		case PROJECTION: {
			uint32_t h = HASH_MURMUR3_SEED;
			const Projection &t = *_data._projection;
			h = hash_murmur3_one_real(t.matrix[0].x, h);
			h = hash_murmur3_one_real(t.matrix[0].y, h);
			h = hash_murmur3_one_real(t.matrix[0].z, h);
			h = hash_murmur3_one_real(t.matrix[0].w, h);
			h = hash_murmur3_one_real(t.matrix[1].x, h);
			h = hash_murmur3_one_real(t.matrix[1].y, h);
			h = hash_murmur3_one_real(t.matrix[1].z, h);
			h = hash_murmur3_one_real(t.matrix[1].w, h);
			h = hash_murmur3_one_real(t.matrix[2].x, h);
			h = hash_murmur3_one_real(t.matrix[2].y, h);
			h = hash_murmur3_one_real(t.matrix[2].z, h);
			h = hash_murmur3_one_real(t.matrix[2].w, h);
			h = hash_murmur3_one_real(t.matrix[3].x, h);
			h = hash_murmur3_one_real(t.matrix[3].y, h);
			h = hash_murmur3_one_real(t.matrix[3].z, h);
			h = hash_murmur3_one_real(t.matrix[3].w, h);
			return hash_fmix32(h);
		} break;

		// misc types
		case COLOR: {
			uint32_t h = HASH_MURMUR3_SEED;
			const Color &c = *reinterpret_cast<const Color *>(_data._mem);
			h = hash_murmur3_one_float(c.r, h);
			h = hash_murmur3_one_float(c.g, h);
			h = hash_murmur3_one_float(c.b, h);
			h = hash_murmur3_one_float(c.a, h);
			return hash_fmix32(h);
		} break;
		case NODE_PATH: {
			return reinterpret_cast<const NodePath *>(_data._mem)->hash();
		} break;
		case RID: {
			return hash_one_uint64(reinterpret_cast<const ::RID *>(_data._mem)->get_id());
		} break;
		case OBJECT: {
			return hash_one_uint64(hash_make_uint64_t(_UNSAFE_OBJ_PROXY_PTR(*this)));
		} break;
		case STRING_NAME: {
			return reinterpret_cast<const StringName *>(_data._mem)->hash();
		} break;
		case DICTIONARY: {
			return reinterpret_cast<const Dictionary *>(_data._mem)->recursive_hash(p_recursion_count);
		} break;
		case ARRAY: {
			const Array &arr = *reinterpret_cast<const Array *>(_data._mem);
			return arr.recursive_hash(p_recursion_count);
		} break;

		case POOL_BYTE_ARRAY: {
			const PoolVector<uint8_t> &arr = *reinterpret_cast<const PoolVector<uint8_t> *>(_data._mem);
			int len = arr.size();
			if (likely(len)) {
				PoolVector<uint8_t>::Read r = arr.read();
				return hash_murmur3_buffer((uint8_t *)&r[0], len);
			} else {
				return hash_murmur3_one_64(0);
			}

		} break;
		case POOL_INT_ARRAY: {
			const PoolVector<int> &arr = *reinterpret_cast<const PoolVector<int> *>(_data._mem);
			int len = arr.size();
			if (likely(len)) {
				PoolVector<int>::Read r = arr.read();
				return hash_murmur3_buffer((uint8_t *)&r[0], len * sizeof(int));
			} else {
				return hash_murmur3_one_64(0);
			}

		} break;
		case POOL_REAL_ARRAY: {
			const PoolVector<real_t> &arr = *reinterpret_cast<const PoolVector<real_t> *>(_data._mem);
			int len = arr.size();

			if (likely(len)) {
				PoolVector<real_t>::Read r = arr.read();
				uint32_t h = HASH_MURMUR3_SEED;

				for (int i = 0; i < len; i++) {
					h = hash_murmur3_one_real(r[i], h);
				}

				return hash_fmix32(h);
			} else {
				return hash_murmur3_one_real(0.0);
			}

		} break;
		case POOL_STRING_ARRAY: {
			uint32_t hash = HASH_MURMUR3_SEED;
			const PoolVector<String> &arr = *reinterpret_cast<const PoolVector<String> *>(_data._mem);
			int len = arr.size();

			if (likely(len)) {
				PoolVector<String>::Read r = arr.read();

				for (int i = 0; i < len; i++) {
					hash = hash_murmur3_one_32(r[i].hash(), hash);
				}

				hash = hash_fmix32(hash);
			}

			return hash;
		} break;
		case POOL_VECTOR2_ARRAY: {
			uint32_t hash = HASH_MURMUR3_SEED;
			const PoolVector<Vector2> &arr = *reinterpret_cast<const PoolVector<Vector2> *>(_data._mem);
			int len = arr.size();

			if (likely(len)) {
				PoolVector<Vector2>::Read r = arr.read();

				for (int i = 0; i < len; i++) {
					hash = hash_murmur3_one_real(r[i].x, hash);
					hash = hash_murmur3_one_real(r[i].y, hash);
				}

				hash = hash_fmix32(hash);
			}

			return hash;
		} break;
		case POOL_VECTOR2I_ARRAY: {
			uint32_t hash = HASH_MURMUR3_SEED;
			const PoolVector<Vector2i> &arr = *reinterpret_cast<const PoolVector<Vector2i> *>(_data._mem);
			int len = arr.size();

			if (likely(len)) {
				PoolVector<Vector2i>::Read r = arr.read();

				for (int i = 0; i < len; i++) {
					hash = hash_murmur3_one_32(r[i].x, hash);
					hash = hash_murmur3_one_32(r[i].y, hash);
				}

				hash = hash_fmix32(hash);
			}

			return hash;
		} break;
		case POOL_VECTOR3_ARRAY: {
			uint32_t hash = HASH_MURMUR3_SEED;
			const PoolVector<Vector3> &arr = *reinterpret_cast<const PoolVector<Vector3> *>(_data._mem);
			int len = arr.size();

			if (likely(len)) {
				PoolVector<Vector3>::Read r = arr.read();

				for (int i = 0; i < len; i++) {
					hash = hash_murmur3_one_real(r[i].x, hash);
					hash = hash_murmur3_one_real(r[i].y, hash);
					hash = hash_murmur3_one_real(r[i].z, hash);
				}

				hash = hash_fmix32(hash);
			}

			return hash;
		} break;
		case POOL_VECTOR3I_ARRAY: {
			uint32_t hash = HASH_MURMUR3_SEED;
			const PoolVector<Vector3i> &arr = *reinterpret_cast<const PoolVector<Vector3i> *>(_data._mem);
			int len = arr.size();

			if (likely(len)) {
				PoolVector<Vector3i>::Read r = arr.read();

				for (int i = 0; i < len; i++) {
					hash = hash_murmur3_one_32(r[i].x, hash);
					hash = hash_murmur3_one_32(r[i].y, hash);
					hash = hash_murmur3_one_32(r[i].z, hash);
				}

				hash = hash_fmix32(hash);
			}

			return hash;
		} break;
		case POOL_VECTOR4_ARRAY: {
			uint32_t hash = HASH_MURMUR3_SEED;
			const PoolVector<Vector4> &arr = *reinterpret_cast<const PoolVector<Vector4> *>(_data._mem);
			int len = arr.size();

			if (likely(len)) {
				PoolVector<Vector4>::Read r = arr.read();

				for (int i = 0; i < len; i++) {
					hash = hash_murmur3_one_real(r[i].x, hash);
					hash = hash_murmur3_one_real(r[i].y, hash);
					hash = hash_murmur3_one_real(r[i].z, hash);
					hash = hash_murmur3_one_real(r[i].w, hash);
				}

				hash = hash_fmix32(hash);
			}

			return hash;
		} break;
		case POOL_VECTOR4I_ARRAY: {
			uint32_t hash = HASH_MURMUR3_SEED;
			const PoolVector<Vector4i> &arr = *reinterpret_cast<const PoolVector<Vector4i> *>(_data._mem);
			int len = arr.size();

			if (likely(len)) {
				PoolVector<Vector4i>::Read r = arr.read();

				for (int i = 0; i < len; i++) {
					hash = hash_murmur3_one_32(r[i].x, hash);
					hash = hash_murmur3_one_32(r[i].y, hash);
					hash = hash_murmur3_one_32(r[i].z, hash);
					hash = hash_murmur3_one_32(r[i].w, hash);
				}

				hash = hash_fmix32(hash);
			}

			return hash;
		} break;
		case POOL_COLOR_ARRAY: {
			uint32_t hash = HASH_MURMUR3_SEED;
			const PoolVector<Color> &arr = *reinterpret_cast<const PoolVector<Color> *>(_data._mem);
			int len = arr.size();

			if (likely(len)) {
				PoolVector<Color>::Read r = arr.read();

				for (int i = 0; i < len; i++) {
					hash = hash_murmur3_one_real(r[i].r, hash);
					hash = hash_murmur3_one_real(r[i].g, hash);
					hash = hash_murmur3_one_real(r[i].b, hash);
					hash = hash_murmur3_one_real(r[i].a, hash);
				}

				hash = hash_fmix32(hash);
			}

			return hash;
		} break;
		default: {
		}
	}

	return 0;
}

#define hash_compare_scalar(p_lhs, p_rhs) \
	(((p_lhs) == (p_rhs)) || (Math::is_nan(p_lhs) && Math::is_nan(p_rhs)))

#define hash_compare_vector2(p_lhs, p_rhs)        \
	(hash_compare_scalar((p_lhs).x, (p_rhs).x) && \
			hash_compare_scalar((p_lhs).y, (p_rhs).y))

#define hash_compare_vector2i(p_lhs, p_rhs) \
	(((p_lhs).x == (p_rhs).x) &&            \
			((p_lhs).y == (p_rhs).y))

#define hash_compare_vector3(p_lhs, p_rhs)               \
	(hash_compare_scalar((p_lhs).x, (p_rhs).x) &&        \
			hash_compare_scalar((p_lhs).y, (p_rhs).y) && \
			hash_compare_scalar((p_lhs).z, (p_rhs).z))

#define hash_compare_vector3i(p_lhs, p_rhs) \
	(((p_lhs).x == (p_rhs).x) &&            \
			((p_lhs).y == (p_rhs).y) &&     \
			((p_lhs).z == (p_rhs).z))

#define hash_compare_vector4(p_lhs, p_rhs)               \
	(hash_compare_scalar((p_lhs).x, (p_rhs).x) &&        \
			hash_compare_scalar((p_lhs).y, (p_rhs).y) && \
			hash_compare_scalar((p_lhs).z, (p_rhs).z) && \
			hash_compare_scalar((p_lhs).w, (p_rhs).w))

#define hash_compare_vector4i(p_lhs, p_rhs) \
	(((p_lhs).x == (p_rhs).x) &&            \
			((p_lhs).y == (p_rhs).y) &&     \
			((p_lhs).z == (p_rhs).z) &&     \
			((p_lhs).w == (p_rhs).w))

#define hash_compare_quat(p_lhs, p_rhs)                  \
	(hash_compare_scalar((p_lhs).x, (p_rhs).x) &&        \
			hash_compare_scalar((p_lhs).y, (p_rhs).y) && \
			hash_compare_scalar((p_lhs).z, (p_rhs).z) && \
			hash_compare_scalar((p_lhs).w, (p_rhs).w))

#define hash_compare_color(p_lhs, p_rhs)                 \
	(hash_compare_scalar((p_lhs).r, (p_rhs).r) &&        \
			hash_compare_scalar((p_lhs).g, (p_rhs).g) && \
			hash_compare_scalar((p_lhs).b, (p_rhs).b) && \
			hash_compare_scalar((p_lhs).a, (p_rhs).a))

#define hash_compare_pool_array(p_lhs, p_rhs, p_type, p_compare_func)                   \
	const PoolVector<p_type> &l = *reinterpret_cast<const PoolVector<p_type> *>(p_lhs); \
	const PoolVector<p_type> &r = *reinterpret_cast<const PoolVector<p_type> *>(p_rhs); \
                                                                                        \
	if (l.size() != r.size())                                                           \
		return false;                                                                   \
                                                                                        \
	PoolVector<p_type>::Read lr = l.read();                                             \
	PoolVector<p_type>::Read rr = r.read();                                             \
                                                                                        \
	for (int i = 0; i < l.size(); ++i) {                                                \
		if (!p_compare_func((lr[i]), (rr[i])))                                          \
			return false;                                                               \
	}                                                                                   \
                                                                                        \
	return true

bool Variant::hash_compare(const Variant &p_variant) const {
	if (type != p_variant.type) {
		return false;
	}

	switch (type) {
		//BOOL
		case INT: {
			return _data._int == p_variant._data._int;
		} break;
		case REAL: {
			return hash_compare_scalar(_data._real, p_variant._data._real);
		} break;
		case STRING: {
			return *reinterpret_cast<const String *>(_data._mem) == *reinterpret_cast<const String *>(p_variant._data._mem);
		} break;

		case RECT2: {
			const Rect2 *l = reinterpret_cast<const Rect2 *>(_data._mem);
			const Rect2 *r = reinterpret_cast<const Rect2 *>(p_variant._data._mem);

			return hash_compare_vector2(l->position, r->position) &&
					hash_compare_vector2(l->size, r->size);
		} break;
		case RECT2I: {
			const Rect2i *l = reinterpret_cast<const Rect2i *>(_data._mem);
			const Rect2i *r = reinterpret_cast<const Rect2i *>(p_variant._data._mem);

			return hash_compare_vector2i(l->position, r->position) &&
					hash_compare_vector2i(l->size, r->size);
		} break;
		case VECTOR2: {
			const Vector2 *l = reinterpret_cast<const Vector2 *>(_data._mem);
			const Vector2 *r = reinterpret_cast<const Vector2 *>(p_variant._data._mem);

			return hash_compare_vector2(*l, *r);
		} break;
		case VECTOR2I: {
			const Vector2i *l = reinterpret_cast<const Vector2i *>(_data._mem);
			const Vector2i *r = reinterpret_cast<const Vector2i *>(p_variant._data._mem);

			return hash_compare_vector2i(*l, *r);
		} break;
		case VECTOR3: {
			const Vector3 *l = reinterpret_cast<const Vector3 *>(_data._mem);
			const Vector3 *r = reinterpret_cast<const Vector3 *>(p_variant._data._mem);

			return hash_compare_vector3(*l, *r);
		} break;
		case VECTOR3I: {
			const Vector3i *l = reinterpret_cast<const Vector3i *>(_data._mem);
			const Vector3i *r = reinterpret_cast<const Vector3i *>(p_variant._data._mem);

			return hash_compare_vector3i(*l, *r);
		} break;
		case VECTOR4: {
			const Vector4 *l = reinterpret_cast<const Vector4 *>(_data._mem);
			const Vector4 *r = reinterpret_cast<const Vector4 *>(p_variant._data._mem);

			return hash_compare_vector4(*l, *r);
		} break;
		case VECTOR4I: {
			const Vector4i *l = reinterpret_cast<const Vector4i *>(_data._mem);
			const Vector4i *r = reinterpret_cast<const Vector4i *>(p_variant._data._mem);

			return hash_compare_vector4i(*l, *r);
		} break;

		case PLANE: {
			const Plane *l = reinterpret_cast<const Plane *>(_data._mem);
			const Plane *r = reinterpret_cast<const Plane *>(p_variant._data._mem);

			return hash_compare_vector3(l->normal, r->normal) &&
					hash_compare_scalar(l->d, r->d);
		} break;
		case QUATERNION: {
			const Quaternion *l = reinterpret_cast<const Quaternion *>(_data._mem);
			const Quaternion *r = reinterpret_cast<const Quaternion *>(p_variant._data._mem);

			return hash_compare_quat(*l, *r);
		} break;
		case AABB: {
			const ::AABB *l = _data._aabb;
			const ::AABB *r = p_variant._data._aabb;

			return hash_compare_vector3(l->position, r->position) &&
					hash_compare_vector3(l->size, r->size);

		} break;
		case BASIS: {
			const Basis *l = _data._basis;
			const Basis *r = p_variant._data._basis;

			for (int i = 0; i < 3; i++) {
				if (!hash_compare_vector3(l->rows[i], r->rows[i])) {
					return false;
				}
			}

			return true;
		} break;
		case TRANSFORM: {
			const Transform *l = _data._transform;
			const Transform *r = p_variant._data._transform;

			for (int i = 0; i < 3; i++) {
				if (!hash_compare_vector3(l->basis.rows[i], r->basis.rows[i])) {
					return false;
				}
			}

			return hash_compare_vector3(l->origin, r->origin);
		} break;
		case TRANSFORM2D: {
			Transform2D *l = _data._transform2d;
			Transform2D *r = p_variant._data._transform2d;

			for (int i = 0; i < 3; i++) {
				if (!hash_compare_vector2(l->columns[i], r->columns[i])) {
					return false;
				}
			}

			return true;
		} break;
		case PROJECTION: {
			const Projection *l = _data._projection;
			const Projection *r = p_variant._data._projection;

			for (int i = 0; i < 4; i++) {
				if (!hash_compare_vector4(l->matrix[i], r->matrix[i])) {
					return false;
				}
			}

			return true;
		} break;

		case COLOR: {
			const Color *l = reinterpret_cast<const Color *>(_data._mem);
			const Color *r = reinterpret_cast<const Color *>(p_variant._data._mem);

			return hash_compare_color(*l, *r);
		} break;
		case ARRAY: {
			const Array &l = *(reinterpret_cast<const Array *>(_data._mem));
			const Array &r = *(reinterpret_cast<const Array *>(p_variant._data._mem));

			if (l.size() != r.size()) {
				return false;
			}

			for (int i = 0; i < l.size(); ++i) {
				if (!l[i].hash_compare(r[i])) {
					return false;
				}
			}

			return true;
		} break;

		case POOL_REAL_ARRAY: {
			hash_compare_pool_array(_data._mem, p_variant._data._mem, real_t, hash_compare_scalar);
		} break;
		case POOL_VECTOR2_ARRAY: {
			hash_compare_pool_array(_data._mem, p_variant._data._mem, Vector2, hash_compare_vector2);
		} break;
		case POOL_VECTOR2I_ARRAY: {
			hash_compare_pool_array(_data._mem, p_variant._data._mem, Vector2i, hash_compare_vector2i);
		} break;
		case POOL_VECTOR3_ARRAY: {
			hash_compare_pool_array(_data._mem, p_variant._data._mem, Vector3, hash_compare_vector3);
		} break;
		case POOL_VECTOR3I_ARRAY: {
			hash_compare_pool_array(_data._mem, p_variant._data._mem, Vector3i, hash_compare_vector3i);
		} break;
		case POOL_VECTOR4_ARRAY: {
			hash_compare_pool_array(_data._mem, p_variant._data._mem, Vector4, hash_compare_vector4);
		} break;
		case POOL_VECTOR4I_ARRAY: {
			hash_compare_pool_array(_data._mem, p_variant._data._mem, Vector4i, hash_compare_vector4i);
		} break;
		case POOL_COLOR_ARRAY: {
			hash_compare_pool_array(_data._mem, p_variant._data._mem, Color, hash_compare_color);
		} break;

		default:
			bool v;
			Variant r;
			evaluate(OP_EQUAL, *this, p_variant, r, v);
			return r;
	}

	return false;
}

bool Variant::is_ref() const {
	return type == OBJECT && !_get_obj().ref.is_null();
}

Vector<Variant> varray() {
	return Vector<Variant>();
}

Vector<Variant> varray(const Variant &p_arg1) {
	Vector<Variant> v;
	v.push_back(p_arg1);
	return v;
}
Vector<Variant> varray(const Variant &p_arg1, const Variant &p_arg2) {
	Vector<Variant> v;
	v.push_back(p_arg1);
	v.push_back(p_arg2);
	return v;
}
Vector<Variant> varray(const Variant &p_arg1, const Variant &p_arg2, const Variant &p_arg3) {
	Vector<Variant> v;
	v.push_back(p_arg1);
	v.push_back(p_arg2);
	v.push_back(p_arg3);
	return v;
}
Vector<Variant> varray(const Variant &p_arg1, const Variant &p_arg2, const Variant &p_arg3, const Variant &p_arg4) {
	Vector<Variant> v;
	v.push_back(p_arg1);
	v.push_back(p_arg2);
	v.push_back(p_arg3);
	v.push_back(p_arg4);
	return v;
}

Vector<Variant> varray(const Variant &p_arg1, const Variant &p_arg2, const Variant &p_arg3, const Variant &p_arg4, const Variant &p_arg5) {
	Vector<Variant> v;
	v.push_back(p_arg1);
	v.push_back(p_arg2);
	v.push_back(p_arg3);
	v.push_back(p_arg4);
	v.push_back(p_arg5);
	return v;
}

void Variant::static_assign(const Variant &p_variant) {
}

bool Variant::is_shared() const {
	switch (type) {
		case OBJECT:
			return true;
		case ARRAY:
			return true;
		case DICTIONARY:
			return true;
		default: {
		}
	}

	return false;
}

Variant Variant::call(const StringName &p_method, VARIANT_ARG_DECLARE) {
	VARIANT_ARGPTRS;
	int argc = 0;
	for (int i = 0; i < VARIANT_ARG_MAX; i++) {
		if (argptr[i]->get_type() == Variant::NIL) {
			break;
		}
		argc++;
	}

	CallError error;

	Variant ret = call(p_method, argptr, argc, error);

	switch (error.error) {
		case CallError::CALL_ERROR_INVALID_ARGUMENT: {
			String err = "Invalid type for argument #" + itos(error.argument) + ", expected '" + Variant::get_type_name(error.expected) + "'.";
			ERR_PRINT(err.utf8().get_data());

		} break;
		case CallError::CALL_ERROR_INVALID_METHOD: {
			String err = "Invalid method '" + p_method + "' for type '" + Variant::get_type_name(type) + "'.";
			ERR_PRINT(err.utf8().get_data());
		} break;
		case CallError::CALL_ERROR_TOO_MANY_ARGUMENTS: {
			String err = "Too many arguments for method '" + p_method + "'";
			ERR_PRINT(err.utf8().get_data());
		} break;
		default: {
		}
	}

	return ret;
}

void Variant::construct_from_string(const String &p_string, Variant &r_value, ObjectConstruct p_obj_construct, void *p_construct_ud) {
	r_value = Variant();
}

String Variant::get_construct_string() const {
	String vars;
	VariantWriter::write_to_string(*this, vars);

	return vars;
}

String Variant::get_call_error_text(Object *p_base, const StringName &p_method, const Variant **p_argptrs, int p_argcount, const Variant::CallError &ce) {
	String err_text;

	if (ce.error == Variant::CallError::CALL_ERROR_INVALID_ARGUMENT) {
		int errorarg = ce.argument;
		if (p_argptrs) {
			err_text = "Cannot convert argument " + itos(errorarg + 1) + " from " + Variant::get_type_name(p_argptrs[errorarg]->get_type()) + " to " + Variant::get_type_name(ce.expected) + ".";
		} else {
			err_text = "Cannot convert argument " + itos(errorarg + 1) + " from [missing argptr, type unknown] to " + Variant::get_type_name(ce.expected) + ".";
		}
	} else if (ce.error == Variant::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS) {
		err_text = "Method expected " + itos(ce.argument) + " arguments, but called with " + itos(p_argcount) + ".";
	} else if (ce.error == Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS) {
		err_text = "Method expected " + itos(ce.argument) + " arguments, but called with " + itos(p_argcount) + ".";
	} else if (ce.error == Variant::CallError::CALL_ERROR_INVALID_METHOD) {
		err_text = "Method not found.";
	} else if (ce.error == Variant::CallError::CALL_ERROR_INSTANCE_IS_NULL) {
		err_text = "Instance is null";
	} else if (ce.error == Variant::CallError::CALL_OK) {
		return "Call OK";
	}

	String class_name = p_base->get_class();
	Ref<Script> script = p_base->get_script();
	if (script.is_valid() && script->get_path().is_resource_file()) {
		class_name += "(" + script->get_path().get_file() + ")";
	}
	return "'" + class_name + "::" + String(p_method) + "': " + err_text;
}

String vformat(const String &p_text, const Variant &p1, const Variant &p2, const Variant &p3, const Variant &p4, const Variant &p5) {
	Array args;
	if (p1.get_type() != Variant::NIL) {
		args.push_back(p1);

		if (p2.get_type() != Variant::NIL) {
			args.push_back(p2);

			if (p3.get_type() != Variant::NIL) {
				args.push_back(p3);

				if (p4.get_type() != Variant::NIL) {
					args.push_back(p4);

					if (p5.get_type() != Variant::NIL) {
						args.push_back(p5);
					}
				}
			}
		}
	}

	bool error = false;
	String fmt = p_text.sprintf(args, &error);

	ERR_FAIL_COND_V_MSG(error, String(), fmt);

	return fmt;
}
