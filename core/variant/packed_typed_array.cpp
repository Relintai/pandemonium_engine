/*************************************************************************/
/*  packed_typed_array.cpp                                               */
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

#include "packed_typed_array.h"

#include "core/containers/hashfuncs.h"
#include "core/containers/local_vector.h"
#include "core/containers/vector.h"
#include "core/object/object.h"
#include "core/object/script_language.h"
#include "core/variant/variant.h"

class PackedTypedArrayPrivate {
public:
	_FORCE_INLINE_ void ensure_data() {
		if (data) {
			return;
		}

		_ensure_data();
	}

	void _ensure_data() {
		switch (type) {
			case Variant::NIL: {
				data = memnew(Vector<bool>());
			} break;
			case Variant::BOOL: {
				data = memnew(Vector<bool>());
			} break;
			case Variant::INT: {
				switch (int_type) {
					case PackedTypedArray::INT_TYPE_SIGNED_8: {
						data = memnew(Vector<int8_t>());
					} break;
					case PackedTypedArray::INT_TYPE_UNSIGNED_8: {
						data = memnew(Vector<uint8_t>());
					} break;
					case PackedTypedArray::INT_TYPE_SIGNED_16: {
						data = memnew(Vector<int16_t>());
					} break;
					case PackedTypedArray::INT_TYPE_UNSIGNED_16: {
						data = memnew(Vector<uint16_t>());
					} break;
					case PackedTypedArray::INT_TYPE_SIGNED_32: {
						data = memnew(Vector<int32_t>());
					} break;
					case PackedTypedArray::INT_TYPE_UNSIGNED_32: {
						data = memnew(Vector<uint32_t>());
					} break;
					case PackedTypedArray::INT_TYPE_SIGNED_64: {
						data = memnew(Vector<int64_t>());
					} break;
					case PackedTypedArray::INT_TYPE_UNSIGNED_64: {
						data = memnew(Vector<uint64_t>());
					} break;
				}
			} break;
			case Variant::REAL: {
				data = memnew(Vector<real_t>());
			} break;
			case Variant::STRING: {
				data = memnew(Vector<String>());
			} break;
			case Variant::RECT2: {
				data = memnew(Vector<Rect2>());
			} break;
			case Variant::RECT2I: {
				data = memnew(Vector<Rect2i>());
			} break;
			case Variant::VECTOR2: {
				data = memnew(Vector<Vector2>());
			} break;
			case Variant::VECTOR2I: {
				data = memnew(Vector<Vector2i>());
			} break;
			case Variant::VECTOR3: {
				data = memnew(Vector<Vector3>());
			} break;
			case Variant::VECTOR3I: {
				data = memnew(Vector<Vector3i>());
			} break;
			case Variant::VECTOR4: {
				data = memnew(Vector<Vector4>());
			} break;
			case Variant::VECTOR4I: {
				data = memnew(Vector<Vector4i>());
			} break;
			case Variant::PLANE: {
				data = memnew(Vector<Plane>());
			} break;
			case Variant::QUATERNION: {
				data = memnew(Vector<Quaternion>());
			} break;
			case Variant::AABB: {
				data = memnew(Vector<AABB>());
			} break;
			case Variant::BASIS: {
				data = memnew(Vector<Basis>());
			} break;
			case Variant::TRANSFORM: {
				data = memnew(Vector<Transform>());
			} break;
			case Variant::TRANSFORM2D: {
				data = memnew(Vector<Transform2D>());
			} break;
			case Variant::PROJECTION: {
				data = memnew(Vector<Projection>());
			} break;
			case Variant::COLOR: {
				data = memnew(Vector<Color>());
			} break;
			case Variant::NODE_PATH: {
				data = memnew(Vector<NodePath>());
			} break;
			case Variant::RID: {
				data = memnew(Vector<RID>());
			} break;
			case Variant::OBJECT: {
				data = memnew(Vector<Variant>());
			} break;
			case Variant::STRING_NAME: {
				data = memnew(Vector<StringName>());
			} break;
			case Variant::DICTIONARY: {
				data = memnew(Vector<Dictionary>());
			} break;
			case Variant::ARRAY: {
				data = memnew(Vector<Array>());
			} break;
			case Variant::TYPED_ARRAY: {
				data = memnew(Vector<TypedArray>());
			} break;
			case Variant::PACKED_TYPED_ARRAY: {
				data = memnew(Vector<PackedTypedArray>());
			} break;
			case Variant::POOL_BYTE_ARRAY: {
				data = memnew(Vector<Variant>());
			} break;
			case Variant::POOL_INT_ARRAY: {
				data = memnew(Vector<Variant>());
			} break;
			case Variant::POOL_REAL_ARRAY: {
				data = memnew(Vector<Variant>());
			} break;
			case Variant::POOL_STRING_ARRAY: {
				data = memnew(Vector<Variant>());
			} break;
			case Variant::POOL_VECTOR2_ARRAY: {
				data = memnew(Vector<Variant>());
			} break;
			case Variant::POOL_VECTOR2I_ARRAY: {
				data = memnew(Vector<Variant>());
			} break;
			case Variant::POOL_VECTOR3_ARRAY: {
				data = memnew(Vector<Variant>());
			} break;
			case Variant::POOL_VECTOR3I_ARRAY: {
				data = memnew(Vector<Variant>());
			} break;
			case Variant::POOL_VECTOR4_ARRAY: {
				data = memnew(Vector<Variant>());
			} break;
			case Variant::POOL_VECTOR4I_ARRAY: {
				data = memnew(Vector<Variant>());
			} break;
			case Variant::POOL_COLOR_ARRAY: {
				data = memnew(Vector<Variant>());
			} break;
			default: {
			} break;
		}
	}

	void clear() {
		if (!data) {
			return;
		}

		switch (type) {
			case Variant::NIL: {
				memdelete(reinterpret_cast<Vector<bool> *>(data));
			} break;
			case Variant::BOOL: {
				memdelete(reinterpret_cast<Vector<bool> *>(data));
			} break;
			case Variant::INT: {
				switch (int_type) {
					case PackedTypedArray::INT_TYPE_SIGNED_8: {
						memdelete(reinterpret_cast<Vector<int8_t> *>(data));
					} break;
					case PackedTypedArray::INT_TYPE_UNSIGNED_8: {
						memdelete(reinterpret_cast<Vector<uint8_t> *>(data));
					} break;
					case PackedTypedArray::INT_TYPE_SIGNED_16: {
						memdelete(reinterpret_cast<Vector<int16_t> *>(data));
					} break;
					case PackedTypedArray::INT_TYPE_UNSIGNED_16: {
						memdelete(reinterpret_cast<Vector<uint16_t> *>(data));
					} break;
					case PackedTypedArray::INT_TYPE_SIGNED_32: {
						memdelete(reinterpret_cast<Vector<int32_t> *>(data));
					} break;
					case PackedTypedArray::INT_TYPE_UNSIGNED_32: {
						memdelete(reinterpret_cast<Vector<uint32_t> *>(data));
					} break;
					case PackedTypedArray::INT_TYPE_SIGNED_64: {
						memdelete(reinterpret_cast<Vector<int64_t> *>(data));
					} break;
					case PackedTypedArray::INT_TYPE_UNSIGNED_64: {
						memdelete(reinterpret_cast<Vector<uint64_t> *>(data));
					} break;
				}
			} break;
			case Variant::REAL: {
				memdelete(reinterpret_cast<Vector<real_t> *>(data));
			} break;
			case Variant::STRING: {
				memdelete(reinterpret_cast<Vector<String> *>(data));
			} break;
			case Variant::RECT2: {
				memdelete(reinterpret_cast<Vector<Rect2> *>(data));
			} break;
			case Variant::RECT2I: {
				memdelete(reinterpret_cast<Vector<Rect2i> *>(data));
			} break;
			case Variant::VECTOR2: {
				memdelete(reinterpret_cast<Vector<Vector2> *>(data));
			} break;
			case Variant::VECTOR2I: {
				memdelete(reinterpret_cast<Vector<Vector2i> *>(data));
			} break;
			case Variant::VECTOR3: {
				memdelete(reinterpret_cast<Vector<Vector3> *>(data));
			} break;
			case Variant::VECTOR3I: {
				memdelete(reinterpret_cast<Vector<Vector3i> *>(data));
			} break;
			case Variant::VECTOR4: {
				memdelete(reinterpret_cast<Vector<Vector4> *>(data));
			} break;
			case Variant::VECTOR4I: {
				memdelete(reinterpret_cast<Vector<Vector4i> *>(data));
			} break;
			case Variant::PLANE: {
				memdelete(reinterpret_cast<Vector<Plane> *>(data));
			} break;
			case Variant::QUATERNION: {
				memdelete(reinterpret_cast<Vector<Quaternion> *>(data));
			} break;
			case Variant::AABB: {
				memdelete(reinterpret_cast<Vector<AABB> *>(data));
			} break;
			case Variant::BASIS: {
				memdelete(reinterpret_cast<Vector<Basis> *>(data));
			} break;
			case Variant::TRANSFORM: {
				memdelete(reinterpret_cast<Vector<Transform> *>(data));
			} break;
			case Variant::TRANSFORM2D: {
				memdelete(reinterpret_cast<Vector<Transform2D> *>(data));
			} break;
			case Variant::PROJECTION: {
				memdelete(reinterpret_cast<Vector<Projection> *>(data));
			} break;
			case Variant::COLOR: {
				memdelete(reinterpret_cast<Vector<Color> *>(data));
			} break;
			case Variant::NODE_PATH: {
				memdelete(reinterpret_cast<Vector<NodePath> *>(data));
			} break;
			case Variant::RID: {
				memdelete(reinterpret_cast<Vector<RID> *>(data));
			} break;
			case Variant::OBJECT: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			case Variant::STRING_NAME: {
				memdelete(reinterpret_cast<Vector<StringName> *>(data));
			} break;
			case Variant::DICTIONARY: {
				memdelete(reinterpret_cast<Vector<Dictionary> *>(data));
			} break;
			case Variant::ARRAY: {
				memdelete(reinterpret_cast<Vector<Array> *>(data));
			} break;
			case Variant::TYPED_ARRAY: {
				memdelete(reinterpret_cast<Vector<TypedArray> *>(data));
			} break;
			case Variant::PACKED_TYPED_ARRAY: {
				memdelete(reinterpret_cast<Vector<PackedTypedArray> *>(data));
			} break;
			case Variant::POOL_BYTE_ARRAY: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			case Variant::POOL_INT_ARRAY: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			case Variant::POOL_REAL_ARRAY: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			case Variant::POOL_STRING_ARRAY: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			case Variant::POOL_VECTOR2_ARRAY: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			case Variant::POOL_VECTOR2I_ARRAY: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			case Variant::POOL_VECTOR3_ARRAY: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			case Variant::POOL_VECTOR3I_ARRAY: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			case Variant::POOL_VECTOR4_ARRAY: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			case Variant::POOL_VECTOR4I_ARRAY: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			case Variant::POOL_COLOR_ARRAY: {
				memdelete(reinterpret_cast<Vector<Variant> *>(data));
			} break;
			default: {
			} break;
		}

		data = NULL;
	}

	PackedTypedArrayPrivate() {
		type = Variant::NIL;
		int_type = PackedTypedArray::INT_TYPE_SIGNED_64;
		is_global_class = false;
		data = NULL;
	}
	~PackedTypedArrayPrivate() {
		clear();
	}

	SafeRefCount refcount;

	Variant::Type type;
	StringName object_class_name;
	PackedTypedArray::IntType int_type;
	bool is_global_class;

	void *data;
};

#define ACCESS_DATA(command)                                                                        \
	switch (_p->type) {                                                                             \
		case Variant::NIL: {                                                                        \
			Vector<bool> *vec = reinterpret_cast<Vector<bool> *>(_p->data);                         \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::BOOL: {                                                                       \
			Vector<bool> *vec = reinterpret_cast<Vector<bool> *>(_p->data);                         \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::INT: {                                                                        \
			switch (_p->int_type) {                                                                 \
				case PackedTypedArray::INT_TYPE_SIGNED_8: {                                         \
					Vector<int8_t> *vec = reinterpret_cast<Vector<int8_t> *>(_p->data);             \
					command;                                                                        \
				} break;                                                                            \
				case PackedTypedArray::INT_TYPE_UNSIGNED_8: {                                       \
					Vector<uint8_t> *vec = reinterpret_cast<Vector<uint8_t> *>(_p->data);           \
					command;                                                                        \
				} break;                                                                            \
				case PackedTypedArray::INT_TYPE_SIGNED_16: {                                        \
					Vector<int16_t> *vec = reinterpret_cast<Vector<int16_t> *>(_p->data);           \
					command;                                                                        \
				} break;                                                                            \
				case PackedTypedArray::INT_TYPE_UNSIGNED_16: {                                      \
					Vector<uint16_t> *vec = reinterpret_cast<Vector<uint16_t> *>(_p->data);         \
					command;                                                                        \
				} break;                                                                            \
				case PackedTypedArray::INT_TYPE_SIGNED_32: {                                        \
					Vector<int32_t> *vec = reinterpret_cast<Vector<int32_t> *>(_p->data);           \
					command;                                                                        \
				} break;                                                                            \
				case PackedTypedArray::INT_TYPE_UNSIGNED_32: {                                      \
					Vector<uint32_t> *vec = reinterpret_cast<Vector<uint32_t> *>(_p->data);         \
					command;                                                                        \
				} break;                                                                            \
				case PackedTypedArray::INT_TYPE_SIGNED_64: {                                        \
					Vector<int64_t> *vec = reinterpret_cast<Vector<int64_t> *>(_p->data);           \
					command;                                                                        \
				} break;                                                                            \
				case PackedTypedArray::INT_TYPE_UNSIGNED_64: {                                      \
					Vector<uint64_t> *vec = reinterpret_cast<Vector<uint64_t> *>(_p->data);         \
					command;                                                                        \
				} break;                                                                            \
			}                                                                                       \
		} break;                                                                                    \
		case Variant::REAL: {                                                                       \
			Vector<real_t> *vec = reinterpret_cast<Vector<real_t> *>(_p->data);                     \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::STRING: {                                                                     \
			Vector<String> *vec = reinterpret_cast<Vector<String> *>(_p->data);                     \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::RECT2: {                                                                      \
			Vector<Rect2> *vec = reinterpret_cast<Vector<Rect2> *>(_p->data);                       \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::RECT2I: {                                                                     \
			Vector<Rect2i> *vec = reinterpret_cast<Vector<Rect2i> *>(_p->data);                     \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::VECTOR2: {                                                                    \
			Vector<Vector2> *vec = reinterpret_cast<Vector<Vector2> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::VECTOR2I: {                                                                   \
			Vector<Vector2i> *vec = reinterpret_cast<Vector<Vector2i> *>(_p->data);                 \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::VECTOR3: {                                                                    \
			Vector<Vector3> *vec = reinterpret_cast<Vector<Vector3> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::VECTOR3I: {                                                                   \
			Vector<Vector3i> *vec = reinterpret_cast<Vector<Vector3i> *>(_p->data);                 \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::VECTOR4: {                                                                    \
			Vector<Vector4> *vec = reinterpret_cast<Vector<Vector4> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::VECTOR4I: {                                                                   \
			Vector<Vector4i> *vec = reinterpret_cast<Vector<Vector4i> *>(_p->data);                 \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::PLANE: {                                                                      \
			Vector<Plane> *vec = reinterpret_cast<Vector<Plane> *>(_p->data);                       \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::QUATERNION: {                                                                 \
			Vector<Quaternion> *vec = reinterpret_cast<Vector<Quaternion> *>(_p->data);             \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::AABB: {                                                                       \
			Vector<AABB> *vec = reinterpret_cast<Vector<AABB> *>(_p->data);                         \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::BASIS: {                                                                      \
			Vector<Basis> *vec = reinterpret_cast<Vector<Basis> *>(_p->data);                       \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::TRANSFORM: {                                                                  \
			Vector<Transform> *vec = reinterpret_cast<Vector<Transform> *>(_p->data);               \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::TRANSFORM2D: {                                                                \
			Vector<Transform2D> *vec = reinterpret_cast<Vector<Transform2D> *>(_p->data);           \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::PROJECTION: {                                                                 \
			Vector<Projection> *vec = reinterpret_cast<Vector<Projection> *>(_p->data);             \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::COLOR: {                                                                      \
			Vector<Color> *vec = reinterpret_cast<Vector<Color> *>(_p->data);                       \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::NODE_PATH: {                                                                  \
			Vector<NodePath> *vec = reinterpret_cast<Vector<NodePath> *>(_p->data);                 \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::RID: {                                                                        \
			Vector<RID> *vec = reinterpret_cast<Vector<RID> *>(_p->data);                           \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::OBJECT: {                                                                     \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::STRING_NAME: {                                                                \
			Vector<StringName> *vec = reinterpret_cast<Vector<StringName> *>(_p->data);             \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::DICTIONARY: {                                                                 \
			Vector<Dictionary> *vec = reinterpret_cast<Vector<Dictionary> *>(_p->data);             \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::ARRAY: {                                                                      \
			Vector<Array> *vec = reinterpret_cast<Vector<Array> *>(_p->data);                       \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::TYPED_ARRAY: {                                                                \
			Vector<TypedArray> *vec = reinterpret_cast<Vector<TypedArray> *>(_p->data);             \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::PACKED_TYPED_ARRAY: {                                                         \
			Vector<PackedTypedArray> *vec = reinterpret_cast<Vector<PackedTypedArray> *>(_p->data); \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::POOL_BYTE_ARRAY: {                                                            \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::POOL_INT_ARRAY: {                                                             \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::POOL_REAL_ARRAY: {                                                            \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::POOL_STRING_ARRAY: {                                                          \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::POOL_VECTOR2_ARRAY: {                                                         \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::POOL_VECTOR2I_ARRAY: {                                                        \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::POOL_VECTOR3_ARRAY: {                                                         \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::POOL_VECTOR3I_ARRAY: {                                                        \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::POOL_VECTOR4_ARRAY: {                                                         \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::POOL_VECTOR4I_ARRAY: {                                                        \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		case Variant::POOL_COLOR_ARRAY: {                                                           \
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);                   \
			command;                                                                                \
		} break;                                                                                    \
		default: {                                                                                  \
		} break;                                                                                    \
	}

void PackedTypedArray::_ref(const PackedTypedArray &p_from) const {
	PackedTypedArrayPrivate *_fp = p_from._p;

	ERR_FAIL_COND(!_fp); // should NOT happen.

	if (_fp == _p) {
		return; // whatever it is, nothing to do here move along
	}

	bool success = _fp->refcount.ref();

	ERR_FAIL_COND(!success); // should really not happen either

	_unref();

	_p = p_from._p;
}

void PackedTypedArray::_unref() const {
	if (!_p) {
		return;
	}

	if (_p->refcount.unref()) {
		memdelete(_p);
	}
	_p = nullptr;
}

const Variant PackedTypedArray::operator[](int p_idx) const {
	ERR_FAIL_COND_V(!_p->data, Variant());

	ACCESS_DATA(return vec->operator[](p_idx));

	return Variant();
}

int PackedTypedArray::size() const {
	if (!_p->data) {
		return 0;
	}

	ACCESS_DATA(return vec->size());

	return 0;
}
bool PackedTypedArray::empty() const {
	if (!_p->data) {
		return true;
	}

	ACCESS_DATA(return vec->empty());

	return 0;
}
void PackedTypedArray::clear() {
	if (!_p->data) {
		return;
	}

	ACCESS_DATA(return vec->clear());
}

template <typename T>
bool deep_compare_vectors(const Vector<T> &a1, const Vector<T> &a2) {
	const int size = a1.size();
	if (size != a2.size()) {
		return false;
	}

	for (int i = 0; i < size; i++) {
		if (a1[i] != a2[i]) {
			return false;
		}
	}

	return true;
}

template <typename T>
bool deep_compare_vectors_approx_simple(const Vector<T> &a1, const Vector<T> &a2) {
	const int size = a1.size();
	if (size != a2.size()) {
		return false;
	}

	for (int i = 0; i < size; i++) {
		if (!Math::is_equal_approx(a1[i], a2[i])) {
			return false;
		}
	}

	return true;
}

template <typename T>
bool deep_compare_vectors_approx_struct(const Vector<T> &a1, const Vector<T> &a2) {
	const int size = a1.size();
	if (size != a2.size()) {
		return false;
	}

	for (int i = 0; i < size; i++) {
		if (!a1[i].is_equal_approx(a2[i])) {
			return false;
		}
	}

	return true;
}

template <typename T>
bool deep_compare_variant_based_vectors(const Vector<T> &a1, const Vector<T> &a2, int p_recursion_count) {
	const int size = a1.size();
	if (size != a2.size()) {
		return false;
	}

	// Heavy O(n) check
	p_recursion_count++;
	for (int i = 0; i < size; i++) {
		if (!a1[i].deep_equal(a2[i], p_recursion_count)) {
			return false;
		}
	}

	return true;
}

template <typename T>
bool deep_compare_variant_based_vectors_approx(const Vector<T> &a1, const Vector<T> &a2, int p_recursion_count) {
	const int size = a1.size();
	if (size != a2.size()) {
		return false;
	}

	// Heavy O(n) check
	p_recursion_count++;
	for (int i = 0; i < size; i++) {
		if (!a1[i].deep_equal_approx(a2[i], p_recursion_count)) {
			return false;
		}
	}

	return true;
}

#define DO_DEEP_COMPARE_VECTORS(m_type)                                        \
	Vector<m_type> *a1 = reinterpret_cast<Vector<m_type> *>(_p->data);         \
	Vector<m_type> *a2 = reinterpret_cast<Vector<m_type> *>(p_array._p->data); \
	return deep_compare_vectors(*a1, *a2);

#define DO_DEEP_COMPARE_VECTORS_APPROX_SIMPLE(m_type)                          \
	Vector<m_type> *a1 = reinterpret_cast<Vector<m_type> *>(_p->data);         \
	Vector<m_type> *a2 = reinterpret_cast<Vector<m_type> *>(p_array._p->data); \
	return deep_compare_vectors_approx_simple(*a1, *a2);

#define DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(m_type)                          \
	Vector<m_type> *a1 = reinterpret_cast<Vector<m_type> *>(_p->data);         \
	Vector<m_type> *a2 = reinterpret_cast<Vector<m_type> *>(p_array._p->data); \
	return deep_compare_vectors_approx_struct(*a1, *a2);

#define DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(m_type)                          \
	Vector<m_type> *a1 = reinterpret_cast<Vector<m_type> *>(_p->data);         \
	Vector<m_type> *a2 = reinterpret_cast<Vector<m_type> *>(p_array._p->data); \
	return deep_compare_variant_based_vectors(*a1, *a2, p_recursion_count);

#define DO_DEEP_COMPARE_VARIANT_BASED_VECTORS_APPROX(m_type)                   \
	Vector<m_type> *a1 = reinterpret_cast<Vector<m_type> *>(_p->data);         \
	Vector<m_type> *a2 = reinterpret_cast<Vector<m_type> *>(p_array._p->data); \
	return deep_compare_variant_based_vectors_approx(*a1, *a2, p_recursion_count);

bool PackedTypedArray::deep_equal(const PackedTypedArray &p_array, int p_recursion_count) const {
	// Cheap checks
	ERR_FAIL_COND_V_MSG(p_recursion_count > MAX_RECURSION, true, "Max recursion reached");

	if (_p == p_array._p) {
		return true;
	}

	if (!_p->data && !p_array._p->data) {
		return true;
	}

	if (!_p->data || !p_array._p->data) {
		return false;
	}

	if (_p->type != p_array._p->type) {
		return false;
	}

	if (_p->type == Variant::OBJECT) {
		if (_p->object_class_name != p_array._p->object_class_name) {
			return false;
		}
	}

	if (_p->type == Variant::INT) {
		if (_p->int_type != p_array._p->int_type) {
			return false;
		}
	}

	switch (_p->type) {
		case Variant::NIL: {
			DO_DEEP_COMPARE_VECTORS(bool);
		} break;
		case Variant::BOOL: {
			DO_DEEP_COMPARE_VECTORS(bool);
		} break;
		case Variant::INT: {
			switch (_p->int_type) {
				case PackedTypedArray::INT_TYPE_SIGNED_8: {
					DO_DEEP_COMPARE_VECTORS(int8_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_8: {
					DO_DEEP_COMPARE_VECTORS(uint8_t);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_16: {
					DO_DEEP_COMPARE_VECTORS(int16_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_16: {
					DO_DEEP_COMPARE_VECTORS(uint16_t);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_32: {
					DO_DEEP_COMPARE_VECTORS(int32_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_32: {
					DO_DEEP_COMPARE_VECTORS(uint32_t);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_64: {
					DO_DEEP_COMPARE_VECTORS(int64_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_64: {
					DO_DEEP_COMPARE_VECTORS(uint64_t);
				} break;
			}
		} break;
		case Variant::REAL: {
			DO_DEEP_COMPARE_VECTORS(real_t);
		} break;
		case Variant::STRING: {
			DO_DEEP_COMPARE_VECTORS(String);
		} break;
		case Variant::RECT2: {
			DO_DEEP_COMPARE_VECTORS(Rect2);
		} break;
		case Variant::RECT2I: {
			DO_DEEP_COMPARE_VECTORS(Rect2i);
		} break;
		case Variant::VECTOR2: {
			DO_DEEP_COMPARE_VECTORS(Vector2);
		} break;
		case Variant::VECTOR2I: {
			DO_DEEP_COMPARE_VECTORS(Vector2i);
		} break;
		case Variant::VECTOR3: {
			DO_DEEP_COMPARE_VECTORS(Vector3);
		} break;
		case Variant::VECTOR3I: {
			DO_DEEP_COMPARE_VECTORS(Vector3i);
		} break;
		case Variant::VECTOR4: {
			DO_DEEP_COMPARE_VECTORS(Vector4);
		} break;
		case Variant::VECTOR4I: {
			DO_DEEP_COMPARE_VECTORS(Vector4i);
		} break;
		case Variant::PLANE: {
			DO_DEEP_COMPARE_VECTORS(Plane);
		} break;
		case Variant::QUATERNION: {
			DO_DEEP_COMPARE_VECTORS(Quaternion);
		} break;
		case Variant::AABB: {
			DO_DEEP_COMPARE_VECTORS(AABB);
		} break;
		case Variant::BASIS: {
			DO_DEEP_COMPARE_VECTORS(Basis);
		} break;
		case Variant::TRANSFORM: {
			DO_DEEP_COMPARE_VECTORS(Transform);
		} break;
		case Variant::TRANSFORM2D: {
			DO_DEEP_COMPARE_VECTORS(Transform2D);
		} break;
		case Variant::PROJECTION: {
			DO_DEEP_COMPARE_VECTORS(Projection);
		} break;
		case Variant::COLOR: {
			DO_DEEP_COMPARE_VECTORS(Color);
		} break;
		case Variant::NODE_PATH: {
			DO_DEEP_COMPARE_VECTORS(NodePath);
		} break;
		case Variant::RID: {
			DO_DEEP_COMPARE_VECTORS(RID);
		} break;
		case Variant::OBJECT: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::STRING_NAME: {
			DO_DEEP_COMPARE_VECTORS(StringName);
		} break;
		case Variant::DICTIONARY: {
			DO_DEEP_COMPARE_VECTORS(Dictionary);
		} break;
		case Variant::ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Array);

		} break;
		case Variant::TYPED_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(TypedArray);
		} break;
		case Variant::PACKED_TYPED_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(PackedTypedArray);
		} break;
		case Variant::POOL_BYTE_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_INT_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_REAL_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_STRING_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR2_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR2I_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR3_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR3I_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR4_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR4I_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_COLOR_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		default: {
		} break;
	}

	return false;
}

bool PackedTypedArray::deep_equal_approx(const PackedTypedArray &p_array, int p_recursion_count) const {
	// Cheap checks
	ERR_FAIL_COND_V_MSG(p_recursion_count > MAX_RECURSION, true, "Max recursion reached");

	if (_p == p_array._p) {
		return true;
	}

	if (!_p->data && !p_array._p->data) {
		return true;
	}

	if (!_p->data || !p_array._p->data) {
		return false;
	}

	if (_p->type != p_array._p->type) {
		return false;
	}

	if (_p->type == Variant::OBJECT) {
		if (_p->object_class_name != p_array._p->object_class_name) {
			return false;
		}
	}

	if (_p->type == Variant::INT) {
		if (_p->int_type != p_array._p->int_type) {
			return false;
		}
	}

	switch (_p->type) {
		case Variant::NIL: {
			DO_DEEP_COMPARE_VECTORS(bool);
		} break;
		case Variant::BOOL: {
			DO_DEEP_COMPARE_VECTORS(bool);
		} break;
		case Variant::INT: {
			switch (_p->int_type) {
				case PackedTypedArray::INT_TYPE_SIGNED_8: {
					DO_DEEP_COMPARE_VECTORS(int8_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_8: {
					DO_DEEP_COMPARE_VECTORS(uint8_t);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_16: {
					DO_DEEP_COMPARE_VECTORS(int16_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_16: {
					DO_DEEP_COMPARE_VECTORS(uint16_t);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_32: {
					DO_DEEP_COMPARE_VECTORS(int32_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_32: {
					DO_DEEP_COMPARE_VECTORS(uint32_t);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_64: {
					DO_DEEP_COMPARE_VECTORS(int64_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_64: {
					DO_DEEP_COMPARE_VECTORS(uint64_t);
				} break;
			}
		} break;
		case Variant::REAL: {
			DO_DEEP_COMPARE_VECTORS_APPROX_SIMPLE(real_t);
		} break;
		case Variant::STRING: {
			DO_DEEP_COMPARE_VECTORS(String);
		} break;
		case Variant::RECT2: {
			DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(Rect2);
		} break;
		case Variant::RECT2I: {
			DO_DEEP_COMPARE_VECTORS(Rect2i);
		} break;
		case Variant::VECTOR2: {
			DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(Vector2);
		} break;
		case Variant::VECTOR2I: {
			DO_DEEP_COMPARE_VECTORS(Vector2i);
		} break;
		case Variant::VECTOR3: {
			DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(Vector3);
		} break;
		case Variant::VECTOR3I: {
			DO_DEEP_COMPARE_VECTORS(Vector3i);
		} break;
		case Variant::VECTOR4: {
			DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(Vector4);
		} break;
		case Variant::VECTOR4I: {
			DO_DEEP_COMPARE_VECTORS(Vector4i);
		} break;
		case Variant::PLANE: {
			DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(Plane);
		} break;
		case Variant::QUATERNION: {
			DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(Quaternion);
		} break;
		case Variant::AABB: {
			DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(AABB);
		} break;
		case Variant::BASIS: {
			DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(Basis);
		} break;
		case Variant::TRANSFORM: {
			DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(Transform);
		} break;
		case Variant::TRANSFORM2D: {
			DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(Transform2D);
		} break;
		case Variant::PROJECTION: {
			DO_DEEP_COMPARE_VECTORS(Projection);
		} break;
		case Variant::COLOR: {
			DO_DEEP_COMPARE_VECTORS_APPROX_STRUCT(Color);
		} break;
		case Variant::NODE_PATH: {
			DO_DEEP_COMPARE_VECTORS(NodePath);
		} break;
		case Variant::RID: {
			DO_DEEP_COMPARE_VECTORS(RID);
		} break;
		case Variant::OBJECT: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::STRING_NAME: {
			DO_DEEP_COMPARE_VECTORS(StringName);
		} break;
		case Variant::DICTIONARY: {
			DO_DEEP_COMPARE_VECTORS(Dictionary);
		} break;
		case Variant::ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS_APPROX(Array);
		} break;
		case Variant::TYPED_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS_APPROX(TypedArray);
		} break;
		case Variant::PACKED_TYPED_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS_APPROX(PackedTypedArray);
		} break;
		case Variant::POOL_BYTE_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_INT_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_REAL_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS_APPROX(Variant);
		} break;
		case Variant::POOL_STRING_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR2_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS_APPROX(Variant);
		} break;
		case Variant::POOL_VECTOR2I_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR3_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS_APPROX(Variant);
		} break;
		case Variant::POOL_VECTOR3I_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR4_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS_APPROX(Variant);
		} break;
		case Variant::POOL_VECTOR4I_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_COLOR_ARRAY: {
			DO_DEEP_COMPARE_VARIANT_BASED_VECTORS_APPROX(Variant);
		} break;
		default: {
		} break;
	}

	return false;
}

bool PackedTypedArray::operator==(const PackedTypedArray &p_array) const {
	return _p == p_array._p;
}

uint32_t PackedTypedArray::hash() const {
	return recursive_hash(0);
}

#define RECURSIVE_HASH_VECTORS(m_type, m_hash_func)                   \
	Vector<m_type> *a = reinterpret_cast<Vector<m_type> *>(_p->data); \
                                                                      \
	const int s = a->size();                                          \
                                                                      \
	for (int i = 0; i < s; i++) {                                     \
		h = m_hash_func(a->operator[](i), h);                         \
	}

#define RECURSIVE_HASH_VECTORS_HMHD(m_type, m_hash_func)                          \
	Vector<m_type> *a = reinterpret_cast<Vector<m_type> *>(_p->data);             \
                                                                                  \
	const int s = a->size();                                                      \
                                                                                  \
	for (int i = 0; i < s; i++) {                                                 \
		h = m_hash_func(HashMapHasherDefault::hash((m_type)a->operator[](i)), h); \
	}

#define RECURSIVE_HASH_VECTORS_CAST(m_type, m_hash_func, m_cast_to)   \
	Vector<m_type> *a = reinterpret_cast<Vector<m_type> *>(_p->data); \
                                                                      \
	const int s = a->size();                                          \
                                                                      \
	for (int i = 0; i < s; i++) {                                     \
		h = m_hash_func(static_cast<m_cast_to>(a->operator[](i)), h); \
	}

#define RECURSIVE_HASH_VARIANT_BASED_VECTORS(m_type)                                    \
	Vector<m_type> *a = reinterpret_cast<Vector<m_type> *>(_p->data);                   \
                                                                                        \
	const int s = a->size();                                                            \
                                                                                        \
	for (int i = 0; i < s; i++) {                                                       \
		h = hash_murmur3_one_32(a->operator[](i).recursive_hash(p_recursion_count), h); \
	}

uint32_t PackedTypedArray::recursive_hash(int p_recursion_count) const {
	ERR_FAIL_COND_V_MSG(p_recursion_count > MAX_RECURSION, 0, "Max recursion reached");
	p_recursion_count++;

	uint32_t h = hash_murmur3_one_32(0);

	h = hash_murmur3_one_32((int)_p->type, h);

	if (_p->type == Variant::INT) {
		h = hash_murmur3_one_32((int)_p->int_type, h);
	} else if (_p->type == Variant::OBJECT) {
		h = hash_murmur3_one_32(HashMapHasherDefault::hash(_p->object_class_name), h);
	}

	if (!_p->data) {
		return hash_fmix32(h);
	}

	switch (_p->type) {
		case Variant::NIL: {
			RECURSIVE_HASH_VECTORS_CAST(bool, hash_murmur3_one_32, uint8_t);
		} break;
		case Variant::BOOL: {
			RECURSIVE_HASH_VECTORS_CAST(bool, hash_murmur3_one_32, uint8_t);
		} break;
		case Variant::INT: {
			switch (_p->int_type) {
				case PackedTypedArray::INT_TYPE_SIGNED_8: {
					RECURSIVE_HASH_VECTORS(int8_t, hash_murmur3_one_32);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_8: {
					RECURSIVE_HASH_VECTORS(uint8_t, hash_murmur3_one_32);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_16: {
					RECURSIVE_HASH_VECTORS(int16_t, hash_murmur3_one_32);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_16: {
					RECURSIVE_HASH_VECTORS(uint16_t, hash_murmur3_one_32);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_32: {
					RECURSIVE_HASH_VECTORS(int32_t, hash_murmur3_one_32);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_32: {
					RECURSIVE_HASH_VECTORS(uint32_t, hash_murmur3_one_32);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_64: {
					RECURSIVE_HASH_VECTORS(int64_t, hash_murmur3_one_64);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_64: {
					RECURSIVE_HASH_VECTORS(uint64_t, hash_murmur3_one_64);
				} break;
			}
		} break;
		case Variant::REAL: {
			RECURSIVE_HASH_VECTORS(real_t, hash_murmur3_one_real);
		} break;
		case Variant::STRING: {
			RECURSIVE_HASH_VECTORS_HMHD(String, hash_murmur3_one_32);
		} break;
		case Variant::RECT2: {
			RECURSIVE_HASH_VECTORS_HMHD(Rect2, hash_murmur3_one_32);
		} break;
		case Variant::RECT2I: {
			RECURSIVE_HASH_VECTORS_HMHD(Rect2i, hash_murmur3_one_32);
		} break;
		case Variant::VECTOR2: {
			RECURSIVE_HASH_VECTORS_HMHD(Vector2, hash_murmur3_one_32);
		} break;
		case Variant::VECTOR2I: {
			RECURSIVE_HASH_VECTORS_HMHD(Vector2i, hash_murmur3_one_32);
		} break;
		case Variant::VECTOR3: {
			RECURSIVE_HASH_VECTORS_HMHD(Vector3, hash_murmur3_one_32);
		} break;
		case Variant::VECTOR3I: {
			RECURSIVE_HASH_VECTORS_HMHD(Vector3i, hash_murmur3_one_32);
		} break;
		case Variant::VECTOR4: {
			RECURSIVE_HASH_VECTORS_HMHD(Vector4, hash_murmur3_one_32);
		} break;
		case Variant::VECTOR4I: {
			RECURSIVE_HASH_VECTORS_HMHD(Vector4i, hash_murmur3_one_32);
		} break;
		case Variant::PLANE: {
			RECURSIVE_HASH_VECTORS_HMHD(Plane, hash_murmur3_one_32);
		} break;
		case Variant::QUATERNION: {
			RECURSIVE_HASH_VECTORS_HMHD(Quaternion, hash_murmur3_one_32);
		} break;
		case Variant::AABB: {
			RECURSIVE_HASH_VECTORS_HMHD(AABB, hash_murmur3_one_32);
		} break;
		case Variant::BASIS: {
			RECURSIVE_HASH_VECTORS_HMHD(Basis, hash_murmur3_one_32);
		} break;
		case Variant::TRANSFORM: {
			RECURSIVE_HASH_VECTORS_HMHD(Transform, hash_murmur3_one_32);
		} break;
		case Variant::TRANSFORM2D: {
			RECURSIVE_HASH_VECTORS_HMHD(Transform2D, hash_murmur3_one_32);
		} break;
		case Variant::PROJECTION: {
			RECURSIVE_HASH_VECTORS_HMHD(Projection, hash_murmur3_one_32);
		} break;
		case Variant::COLOR: {
			RECURSIVE_HASH_VECTORS_HMHD(Color, hash_murmur3_one_32);
		} break;
		case Variant::NODE_PATH: {
			RECURSIVE_HASH_VECTORS_HMHD(NodePath, hash_murmur3_one_32);
		} break;
		case Variant::RID: {
			RECURSIVE_HASH_VECTORS_HMHD(RID, hash_murmur3_one_32);
		} break;
		case Variant::OBJECT: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::STRING_NAME: {
			RECURSIVE_HASH_VECTORS_HMHD(StringName, hash_murmur3_one_32);
		} break;
		case Variant::DICTIONARY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Dictionary);
		} break;
		case Variant::ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Array);
		} break;
		case Variant::TYPED_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(TypedArray);
		} break;
		case Variant::PACKED_TYPED_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(PackedTypedArray);
		} break;
		case Variant::POOL_BYTE_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_INT_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_REAL_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_STRING_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR2_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR2I_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR3_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR3I_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR4_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_VECTOR4I_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		case Variant::POOL_COLOR_ARRAY: {
			RECURSIVE_HASH_VARIANT_BASED_VECTORS(Variant);
		} break;
		default: {
		} break;
	}

	return hash_fmix32(h);
}

#undef RECURSIVE_HASH_VECTORS
#undef RECURSIVE_HASH_VECTORS_HMHD
#undef RECURSIVE_HASH_VECTORS_CAST
#undef RECURSIVE_HASH_VARIANT_BASED_VECTORS

void PackedTypedArray::operator=(const PackedTypedArray &p_array) {
	_ref(p_array);
}

bool PackedTypedArray::push_back(const Variant &p_value) {
	if (!can_take_variant(p_value)) {
		return false;
	}

	_p->ensure_data();
	ACCESS_DATA(vec->push_back(p_value));

	return true;
}

void PackedTypedArray::_push_back_unchecked(const Variant &p_value) {
	_p->ensure_data();
	ACCESS_DATA(vec->push_back(p_value));
}

#define APPEND_ARRAYS(m_type)                                            \
	Vector<m_type> *veca = reinterpret_cast<Vector<m_type> *>(_p->data); \
	Vector<m_type> *vecb = reinterpret_cast<Vector<m_type> *>(_p->data); \
	veca->append_array(*vecb);

bool PackedTypedArray::append_array(const PackedTypedArray &p_array) {
	if (!p_array._p->data) {
		return false;
	}

	if (p_array.size() == 0) {
		return false;
	}

	if (_p->type != p_array._p->type) {
		return false;
	}

	if (_p->type == Variant::OBJECT) {
		if (!can_take_variant(p_array[0])) {
			return false;
		}
	}

	if (_p->type == Variant::INT) {
		if (_p->int_type != p_array._p->int_type) {
			return false;
		}
	}

	_p->ensure_data();

	switch (_p->type) {
		case Variant::NIL: {
			APPEND_ARRAYS(bool);
		} break;
		case Variant::BOOL: {
			APPEND_ARRAYS(bool);
		} break;
		case Variant::INT: {
			switch (_p->int_type) {
				case PackedTypedArray::INT_TYPE_SIGNED_8: {
					APPEND_ARRAYS(int8_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_8: {
					APPEND_ARRAYS(uint8_t);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_16: {
					APPEND_ARRAYS(int16_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_16: {
					APPEND_ARRAYS(uint16_t);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_32: {
					APPEND_ARRAYS(int32_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_32: {
					APPEND_ARRAYS(uint32_t);
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_64: {
					APPEND_ARRAYS(int64_t);
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_64: {
					APPEND_ARRAYS(uint64_t);
				} break;
			}
		} break;
		case Variant::REAL: {
			APPEND_ARRAYS(real_t);
		} break;
		case Variant::STRING: {
			APPEND_ARRAYS(String);
		} break;
		case Variant::RECT2: {
			APPEND_ARRAYS(Rect2);
		} break;
		case Variant::RECT2I: {
			APPEND_ARRAYS(Rect2i);
		} break;
		case Variant::VECTOR2: {
			APPEND_ARRAYS(Vector2);
		} break;
		case Variant::VECTOR2I: {
			APPEND_ARRAYS(Vector2i);
		} break;
		case Variant::VECTOR3: {
			APPEND_ARRAYS(Vector3);
		} break;
		case Variant::VECTOR3I: {
			APPEND_ARRAYS(Vector3i);
		} break;
		case Variant::VECTOR4: {
			APPEND_ARRAYS(Vector4);
		} break;
		case Variant::VECTOR4I: {
			APPEND_ARRAYS(Vector4i);
		} break;
		case Variant::PLANE: {
			APPEND_ARRAYS(Plane);
		} break;
		case Variant::QUATERNION: {
			APPEND_ARRAYS(Quaternion);
		} break;
		case Variant::AABB: {
			APPEND_ARRAYS(AABB);
		} break;
		case Variant::BASIS: {
			APPEND_ARRAYS(Basis);
		} break;
		case Variant::TRANSFORM: {
			APPEND_ARRAYS(Transform);
		} break;
		case Variant::TRANSFORM2D: {
			APPEND_ARRAYS(Transform2D);
		} break;
		case Variant::PROJECTION: {
			APPEND_ARRAYS(Projection);
		} break;
		case Variant::COLOR: {
			APPEND_ARRAYS(Color);
		} break;
		case Variant::NODE_PATH: {
			APPEND_ARRAYS(NodePath);
		} break;
		case Variant::RID: {
			APPEND_ARRAYS(RID);
		} break;
		case Variant::OBJECT: {
			APPEND_ARRAYS(Variant);
		} break;
		case Variant::STRING_NAME: {
			APPEND_ARRAYS(StringName);
		} break;
		case Variant::DICTIONARY: {
			APPEND_ARRAYS(Dictionary);
		} break;
		case Variant::ARRAY: {
			APPEND_ARRAYS(Array);

		} break;
		case Variant::TYPED_ARRAY: {
			APPEND_ARRAYS(TypedArray);
		} break;
		case Variant::PACKED_TYPED_ARRAY: {
			APPEND_ARRAYS(PackedTypedArray);
		} break;
		case Variant::POOL_BYTE_ARRAY: {
			APPEND_ARRAYS(Variant);
		} break;
		case Variant::POOL_INT_ARRAY: {
			APPEND_ARRAYS(Variant);
		} break;
		case Variant::POOL_REAL_ARRAY: {
			APPEND_ARRAYS(Variant);
		} break;
		case Variant::POOL_STRING_ARRAY: {
			APPEND_ARRAYS(Variant);
		} break;
		case Variant::POOL_VECTOR2_ARRAY: {
			APPEND_ARRAYS(Variant);
		} break;
		case Variant::POOL_VECTOR2I_ARRAY: {
			APPEND_ARRAYS(Variant);
		} break;
		case Variant::POOL_VECTOR3_ARRAY: {
			APPEND_ARRAYS(Variant);
		} break;
		case Variant::POOL_VECTOR3I_ARRAY: {
			APPEND_ARRAYS(Variant);
		} break;
		case Variant::POOL_VECTOR4_ARRAY: {
			APPEND_ARRAYS(Variant);
		} break;
		case Variant::POOL_VECTOR4I_ARRAY: {
			APPEND_ARRAYS(Variant);
		} break;
		case Variant::POOL_COLOR_ARRAY: {
			APPEND_ARRAYS(Variant);
		} break;
		default: {
		} break;
	}

	return true;
}

#undef APPEND_ARRAYS

bool PackedTypedArray::append_from(const Variant &p_array) {
	_p->ensure_data();

	switch (p_array.get_type()) {
		case Variant::ARRAY: {
			Array arr;
			bool element_added = false;
			for (int i = 0; i < arr.size(); ++i) {
				if (push_back(arr[i])) {
					element_added = true;
				}
			}
			return element_added;
		} break;
		case Variant::TYPED_ARRAY: {
			TypedArray arr;

			if (arr.size() == 0) {
				return false;
			}

			if (arr.get_variant_type() != _p->type) {
				return false;
			}

			if (_p->type == Variant::OBJECT) {
				bool element_added = false;
				for (int i = 0; i < arr.size(); ++i) {
					if (push_back(arr[i])) {
						element_added = true;
					}
				}
				return element_added;
			} else {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(arr[i]);
				}
				return true;
			}
		} break;
		case Variant::PACKED_TYPED_ARRAY: {
			append_array(p_array);
		} break;
		case Variant::POOL_BYTE_ARRAY: {
			PoolByteArray arr = p_array;

			if (arr.size() == 0) {
				return false;
			}

			if (_p->type != Variant::INT && _p->type != Variant::REAL) {
				return false;
			}

			for (int i = 0; i < arr.size(); ++i) {
				_push_back_unchecked(arr[i]);
			}
			return true;
		} break;
		case Variant::POOL_INT_ARRAY: {
			PoolIntArray arr = p_array;

			if (arr.size() == 0) {
				return false;
			}

			if (_p->type == Variant::INT) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(arr[i]);
				}
				return true;
			} else if (_p->type == Variant::REAL) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(static_cast<int>(arr[i]));
				}
				return true;
			}

			return false;

		} break;
		case Variant::POOL_REAL_ARRAY: {
			PoolRealArray arr = p_array;

			if (arr.size() == 0) {
				return false;
			}

			if (_p->type == Variant::INT) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(static_cast<int>(arr[i]));
				}
				return true;
			} else if (_p->type == Variant::REAL) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(arr[i]);
				}
				return true;
			}

			return false;

		} break;
		case Variant::POOL_STRING_ARRAY: {
			PoolStringArray arr = p_array;

			if (arr.size() == 0) {
				return false;
			}

			if (_p->type == Variant::STRING) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(arr[i]);
				}
				return true;
			} else if (_p->type == Variant::STRING_NAME) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(String(arr[i]));
				}
				return true;
			}

			return false;
		} break;
		case Variant::POOL_VECTOR2_ARRAY: {
			PoolVector2Array arr = p_array;

			if (arr.size() == 0) {
				return false;
			}

			if (_p->type == Variant::VECTOR2) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(arr[i]);
				}
				return true;
			} else if (_p->type == Variant::VECTOR2I) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(Vector2(arr[i]));
				}
				return true;
			}

			return false;
		} break;
		case Variant::POOL_VECTOR2I_ARRAY: {
			PoolVector2iArray arr = p_array;

			if (arr.size() == 0) {
				return false;
			}

			if (_p->type == Variant::VECTOR2I) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(arr[i]);
				}
				return true;
			} else if (_p->type == Variant::VECTOR2) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(Vector2i(arr[i]));
				}
				return true;
			}

			return false;
		} break;
		case Variant::POOL_VECTOR3_ARRAY: {
			PoolVector3Array arr = p_array;

			if (arr.size() == 0) {
				return false;
			}

			if (_p->type == Variant::VECTOR3) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(arr[i]);
				}
				return true;
			} else if (_p->type == Variant::VECTOR3I) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(Vector3(arr[i]));
				}
				return true;
			}

			return false;
		} break;
		case Variant::POOL_VECTOR3I_ARRAY: {
			PoolVector3iArray arr = p_array;

			if (arr.size() == 0) {
				return false;
			}

			if (_p->type == Variant::VECTOR3I) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(arr[i]);
				}
				return true;
			} else if (_p->type == Variant::VECTOR3) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(Vector3i(arr[i]));
				}
				return true;
			}

			return false;
		} break;
		case Variant::POOL_VECTOR4_ARRAY: {
			PoolVector4Array arr = p_array;

			if (arr.size() == 0) {
				return false;
			}

			if (_p->type == Variant::VECTOR4) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(arr[i]);
				}
				return true;
			} else if (_p->type == Variant::VECTOR4I) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(Vector4(arr[i]));
				}
				return true;
			}

			return false;
		} break;
		case Variant::POOL_VECTOR4I_ARRAY: {
			PoolVector4iArray arr = p_array;

			if (arr.size() == 0) {
				return false;
			}

			if (_p->type == Variant::VECTOR4I) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(arr[i]);
				}
				return true;
			} else if (_p->type == Variant::VECTOR4) {
				for (int i = 0; i < arr.size(); ++i) {
					_push_back_unchecked(Vector4i(arr[i]));
				}
				return true;
			}

			return false;
		} break;
		case Variant::POOL_COLOR_ARRAY: {
			PoolColorArray arr = p_array;

			if (arr.size() == 0) {
				return false;
			}

			if (_p->type != Variant::COLOR) {
				return false;
			}

			for (int i = 0; i < arr.size(); ++i) {
				_push_back_unchecked(arr[i]);
			}
			return true;
		} break;
		default: {
			return false;
		} break;
	}

	return false;
}

Error PackedTypedArray::resize(int p_new_size) {
	_p->ensure_data();

	ACCESS_DATA(return vec->resize(p_new_size));

	return OK;
}

bool PackedTypedArray::insert(int p_pos, const Variant &p_value) {
	if (!can_take_variant(p_value)) {
		return false;
	}

	if (!_p->data) {
		return false;
	}

	ACCESS_DATA(vec->insert(p_pos, p_value));

	return true;
}

bool PackedTypedArray::fill(const Variant &p_value) {
	if (!can_take_variant(p_value)) {
		return false;
	}

	if (!_p->data) {
		return true;
	}

	ACCESS_DATA(vec->fill(p_value));

	return true;
}

void PackedTypedArray::erase(const Variant &p_value) {
	if (!_p->data) {
		return;
	}

	ACCESS_DATA(vec->erase(p_value));
}

Variant PackedTypedArray::front() const {
	ERR_FAIL_COND_V_MSG(!_p->data, Variant(), "Can't take value from empty array.");
	int size = 0;
	ACCESS_DATA(size = vec->size());
	ERR_FAIL_COND_V_MSG(size == 0, Variant(), "Can't take value from empty array.");
	return operator[](0);
}

Variant PackedTypedArray::back() const {
	ERR_FAIL_COND_V_MSG(!_p->data, Variant(), "Can't take value from empty array.");
	int size = 0;
	ACCESS_DATA(size = vec->size());
	ERR_FAIL_COND_V_MSG(size == 0, Variant(), "Can't take value from empty array.");
	return operator[](size - 1);
}

int PackedTypedArray::find(const Variant &p_value, int p_from) const {
	if (!_p->data) {
		return -1;
	}

	ACCESS_DATA(return vec->find(p_value, p_from));

	return -1;
}

int PackedTypedArray::rfind(const Variant &p_value, int p_from) const {
	if (!_p->data) {
		return -1;
	}

	int size = 0;
	ACCESS_DATA(size = vec->size());

	if (size == 0) {
		return -1;
	}

	if (p_from < 0) {
		// Relative offset from the end
		p_from = size + p_from;
	}
	if (p_from < 0 || p_from >= size) {
		// Limit to array boundaries
		p_from = size - 1;
	}

	for (int i = p_from; i >= 0; i--) {
		if (operator[](i) == p_value) {
			return i;
		}
	}

	return -1;
}

int PackedTypedArray::find_last(const Variant &p_value) const {
	return rfind(p_value);
}

int PackedTypedArray::count(const Variant &p_value) const {
	if (!_p->data) {
		return 0;
	}

	int size = 0;
	ACCESS_DATA(size = vec->size());

	if (size == 0) {
		return 0;
	}

	int amount = 0;
	for (int i = 0; i < size; i++) {
		if (operator[](i) == p_value) {
			amount++;
		}
	}

	return amount;
}

bool PackedTypedArray::has(const Variant &p_value) const {
	return find(p_value, 0) != -1;
}

void PackedTypedArray::remove(int p_pos) {
	ERR_FAIL_COND(!_p->data);

	ACCESS_DATA(return vec->remove(p_pos));
}

bool PackedTypedArray::set(int p_idx, const Variant &p_value) {
	ERR_FAIL_COND_V(!_p->data, false);

	if (!can_take_variant(p_value)) {
		return false;
	}

	ACCESS_DATA(vec->write[p_idx] = p_value);

	return true;
}

void PackedTypedArray::_set_unchecked(int p_idx, const Variant &p_value) {
	ERR_FAIL_COND(!_p->data);
	ACCESS_DATA(vec->write[p_idx] = p_value);
}

const Variant PackedTypedArray::get(int p_idx) const {
	return operator[](p_idx);
}

PackedTypedArray PackedTypedArray::duplicate(bool p_deep) const {
	PackedTypedArray new_arr;
	new_arr.set_type_from(*this);

	if (!_p->data) {
		return new_arr;
	}

	int element_count = size();
	new_arr.resize(element_count);
	for (int i = 0; i < element_count; i++) {
		new_arr.set(i, p_deep ? get(i).duplicate(p_deep) : get(i));
	}

	return new_arr;
}

int PackedTypedArray::_clamp_slice_index(int p_index) const {
	int arr_size = size();
	int fixed_index = CLAMP(p_index, -arr_size, arr_size - 1);
	if (fixed_index < 0) {
		fixed_index = arr_size + fixed_index;
	}
	return fixed_index;
}

PackedTypedArray PackedTypedArray::slice(int p_begin, int p_end, int p_step, bool p_deep) const { // like python, but inclusive on upper bound

	PackedTypedArray new_arr;

	ERR_FAIL_COND_V_MSG(p_step == 0, new_arr, "PackedTypedArray slice step size cannot be zero.");

	if (empty()) { // Don't try to slice empty arrays.
		return new_arr;
	}
	if (p_step > 0) {
		if (p_begin >= size() || p_end < -size()) {
			return new_arr;
		}
	} else { // p_step < 0
		if (p_begin < -size() || p_end >= size()) {
			return new_arr;
		}
	}

	int begin = _clamp_slice_index(p_begin);
	int end = _clamp_slice_index(p_end);

	int new_arr_size = MAX(((end - begin + p_step) / p_step), 0);
	new_arr.resize(new_arr_size);

	if (p_step > 0) {
		int dest_idx = 0;
		for (int idx = begin; idx <= end; idx += p_step) {
			ERR_FAIL_COND_V_MSG(dest_idx < 0 || dest_idx >= new_arr_size, PackedTypedArray(), "Bug in PackedTypedArray slice()");
			new_arr._set_unchecked(dest_idx++, p_deep ? get(idx).duplicate(p_deep) : get(idx));
		}
	} else { // p_step < 0
		int dest_idx = 0;
		for (int idx = begin; idx >= end; idx += p_step) {
			ERR_FAIL_COND_V_MSG(dest_idx < 0 || dest_idx >= new_arr_size, PackedTypedArray(), "Bug in PackedTypedArray slice()");
			new_arr._set_unchecked(dest_idx++, p_deep ? get(idx).duplicate(p_deep) : get(idx));
		}
	}

	return new_arr;
}

struct _PackedTypedArrayVariantSort {
	_FORCE_INLINE_ bool operator()(const Variant &p_l, const Variant &p_r) const {
		bool valid = false;
		Variant res;
		Variant::evaluate(Variant::OP_LESS, p_l, p_r, res, valid);
		if (!valid) {
			res = false;
		}
		return res;
	}
};

PackedTypedArray &PackedTypedArray::sort() {
	if (!_p->data) {
		return *this;
	}

	switch (_p->type) {
		case Variant::NIL: {
			Vector<bool> *vec = reinterpret_cast<Vector<bool> *>(_p->data);
			vec->sort();
		} break;
		case Variant::BOOL: {
			Vector<bool> *vec = reinterpret_cast<Vector<bool> *>(_p->data);
			vec->sort();
		} break;
		case Variant::INT: {
			switch (_p->int_type) {
				case PackedTypedArray::INT_TYPE_SIGNED_8: {
					Vector<int8_t> *vec = reinterpret_cast<Vector<int8_t> *>(_p->data);
					vec->sort();
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_8: {
					Vector<uint8_t> *vec = reinterpret_cast<Vector<uint8_t> *>(_p->data);
					vec->sort();
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_16: {
					Vector<int16_t> *vec = reinterpret_cast<Vector<int16_t> *>(_p->data);
					vec->sort();
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_16: {
					Vector<uint16_t> *vec = reinterpret_cast<Vector<uint16_t> *>(_p->data);
					vec->sort();
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_32: {
					Vector<int32_t> *vec = reinterpret_cast<Vector<int32_t> *>(_p->data);
					vec->sort();
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_32: {
					Vector<uint32_t> *vec = reinterpret_cast<Vector<uint32_t> *>(_p->data);
					vec->sort();
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_64: {
					Vector<int64_t> *vec = reinterpret_cast<Vector<int64_t> *>(_p->data);
					vec->sort();
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_64: {
					Vector<uint64_t> *vec = reinterpret_cast<Vector<uint64_t> *>(_p->data);
					vec->sort();
				} break;
			}
		} break;
		case Variant::REAL: {
			Vector<real_t> *vec = reinterpret_cast<Vector<real_t> *>(_p->data);
			vec->sort();
		} break;
		case Variant::STRING: {
			Vector<String> *vec = reinterpret_cast<Vector<String> *>(_p->data);
			vec->sort();
		} break;
		case Variant::RECT2: {
			//Vector<Rect2> *vec = reinterpret_cast<Vector<Rect2> *>(_p->data);
			//vec->sort();
			ERR_FAIL_V_MSG(*this, "Rect2s cannot be sorted directly.");
		} break;
		case Variant::RECT2I: {
			//Vector<Rect2i> *vec = reinterpret_cast<Vector<Rect2i> *>(_p->data);
			//vec->sort();
			ERR_FAIL_V_MSG(*this, "Rect2is cannot be sorted directly.");
		} break;
		case Variant::VECTOR2: {
			Vector<Vector2> *vec = reinterpret_cast<Vector<Vector2> *>(_p->data);
			vec->sort();
		} break;
		case Variant::VECTOR2I: {
			Vector<Vector2i> *vec = reinterpret_cast<Vector<Vector2i> *>(_p->data);
			vec->sort();
		} break;
		case Variant::VECTOR3: {
			Vector<Vector3> *vec = reinterpret_cast<Vector<Vector3> *>(_p->data);
			vec->sort();
		} break;
		case Variant::VECTOR3I: {
			Vector<Vector3i> *vec = reinterpret_cast<Vector<Vector3i> *>(_p->data);
			vec->sort();
		} break;
		case Variant::VECTOR4: {
			Vector<Vector4> *vec = reinterpret_cast<Vector<Vector4> *>(_p->data);
			vec->sort();
		} break;
		case Variant::VECTOR4I: {
			Vector<Vector4i> *vec = reinterpret_cast<Vector<Vector4i> *>(_p->data);
			vec->sort();
		} break;
		case Variant::PLANE: {
			//Vector<Plane> *vec = reinterpret_cast<Vector<Plane> *>(_p->data);
			//vec->sort();
			ERR_FAIL_V_MSG(*this, "Planes cannot be sorted directly.");
		} break;
		case Variant::QUATERNION: {
			//Vector<Quaternion> *vec = reinterpret_cast<Vector<Quaternion> *>(_p->data);
			//vec->sort();
			ERR_FAIL_V_MSG(*this, "Quaternions cannot be sorted directly.");
		} break;
		case Variant::AABB: {
			//Vector<AABB> *vec = reinterpret_cast<Vector<AABB> *>(_p->data);
			//vec->sort();
			ERR_FAIL_V_MSG(*this, "AABBs cannot be sorted directly.");
		} break;
		case Variant::BASIS: {
			//Vector<Basis> *vec = reinterpret_cast<Vector<Basis> *>(_p->data);
			//vec->sort();
			ERR_FAIL_V_MSG(*this, "Basises cannot be sorted directly.");
		} break;
		case Variant::TRANSFORM: {
			//Vector<Transform> *vec = reinterpret_cast<Vector<Transform> *>(_p->data);
			//vec->sort();
			ERR_FAIL_V_MSG(*this, "Transforms cannot be sorted directly.");
		} break;
		case Variant::TRANSFORM2D: {
			//Vector<Transform2D> *vec = reinterpret_cast<Vector<Transform2D> *>(_p->data);
			//vec->sort();
			ERR_FAIL_V_MSG(*this, "Transform2Ds cannot be sorted directly.");
		} break;
		case Variant::PROJECTION: {
			//Vector<Projection> *vec = reinterpret_cast<Vector<Projection> *>(_p->data);
			//vec->sort();
			ERR_FAIL_V_MSG(*this, "Projections cannot be sorted directly.");
		} break;
		case Variant::COLOR: {
			Vector<Color> *vec = reinterpret_cast<Vector<Color> *>(_p->data);
			vec->sort();
		} break;
		case Variant::NODE_PATH: {
			//Vector<NodePath> *vec = reinterpret_cast<Vector<NodePath> *>(_p->data);
			//vec->sort();
			ERR_FAIL_V_MSG(*this, "NodePaths cannot be sorted directly.");
		} break;
		case Variant::RID: {
			Vector<RID> *vec = reinterpret_cast<Vector<RID> *>(_p->data);
			vec->sort();
		} break;
		case Variant::OBJECT: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		case Variant::STRING_NAME: {
			Vector<StringName> *vec = reinterpret_cast<Vector<StringName> *>(_p->data);
			vec->sort();
		} break;
		case Variant::DICTIONARY: {
			//Vector<Dictionary> *vec = reinterpret_cast<Vector<Dictionary> *>(_p->data);
			//vec->sort();
			ERR_FAIL_V_MSG(*this, "Dictionaries cannot be sorted directly.");
		} break;
		case Variant::ARRAY: {
			Vector<Array> *vec = reinterpret_cast<Vector<Array> *>(_p->data);
			vec->sort();

		} break;
		case Variant::TYPED_ARRAY: {
			Vector<TypedArray> *vec = reinterpret_cast<Vector<TypedArray> *>(_p->data);
			vec->sort();
		} break;
		case Variant::PACKED_TYPED_ARRAY: {
			Vector<PackedTypedArray> *vec = reinterpret_cast<Vector<PackedTypedArray> *>(_p->data);
			vec->sort();
		} break;
		case Variant::POOL_BYTE_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		case Variant::POOL_INT_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		case Variant::POOL_REAL_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		case Variant::POOL_STRING_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		case Variant::POOL_VECTOR2_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		case Variant::POOL_VECTOR2I_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		case Variant::POOL_VECTOR3_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		case Variant::POOL_VECTOR3I_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		case Variant::POOL_VECTOR4_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		case Variant::POOL_VECTOR4I_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		case Variant::POOL_COLOR_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			vec->sort_custom<_PackedTypedArrayVariantSort>();
		} break;
		default: {
		} break;
	}

	return *this;
}

struct _PackedTypedArrayVariantSortCustom {
	Object *obj;
	StringName func;

	_FORCE_INLINE_ bool operator()(const Variant &p_l, const Variant &p_r) const {
		const Variant *args[2] = { &p_l, &p_r };
		Variant::CallError err;
		bool res = obj->call(func, args, 2, err);
		if (err.error != Variant::CallError::CALL_OK) {
			res = false;
		}
		return res;
	}
};

PackedTypedArray &PackedTypedArray::sort_custom(Object *p_obj, const StringName &p_function) {
	ERR_FAIL_NULL_V(p_obj, *this);

	if (!_p->data) {
		return *this;
	}

	int dsize = size();

	if (dsize == 0) {
		return *this;
	}

	Vector<Variant> sort_array;
	sort_array.resize(dsize);

	for (int i = 0; i < dsize; ++i) {
		sort_array.write[i] = get(i);
	}

	SortArray<Variant, _PackedTypedArrayVariantSortCustom, true> avs;
	avs.compare.obj = p_obj;
	avs.compare.func = p_function;
	avs.sort(sort_array.ptrw(), sort_array.size());

	for (int i = 0; i < dsize; ++i) {
		_set_unchecked(i, sort_array[i]);
	}

	return *this;
}

void PackedTypedArray::shuffle() {
	if (!_p->data) {
		return;
	}

	const int n = size();

	if (n < 2) {
		return;
	}

	for (int i = n - 1; i >= 1; i--) {
		const int j = Math::rand() % (i + 1);
		const Variant tmp = get(j);

		_set_unchecked(j, get(i));
		_set_unchecked(i, tmp);
	}
}

template <typename Less, typename VT>
_FORCE_INLINE_ int bisect(const Vector<VT> &p_array, const Variant &p_value, bool p_before, const Less &p_less) {
	int lo = 0;
	int hi = p_array.size();
	if (p_before) {
		while (lo < hi) {
			const int mid = (lo + hi) / 2;
			if (p_less(p_array.get(mid), p_value)) {
				lo = mid + 1;
			} else {
				hi = mid;
			}
		}
	} else {
		while (lo < hi) {
			const int mid = (lo + hi) / 2;
			if (p_less(p_value, p_array.get(mid))) {
				hi = mid;
			} else {
				lo = mid + 1;
			}
		}
	}
	return lo;
}

int PackedTypedArray::bsearch(const Variant &p_value, bool p_before) {
	if (!_p->data) {
		return -1;
	}

	if (size() == 0) {
		return -1;
	}

	switch (_p->type) {
		case Variant::NIL: {
			Vector<bool> *vec = reinterpret_cast<Vector<bool> *>(_p->data);
			return bisect<_DefaultComparator<bool>, bool>(*vec, p_value, p_before, _DefaultComparator<bool>());
		} break;
		case Variant::BOOL: {
			Vector<bool> *vec = reinterpret_cast<Vector<bool> *>(_p->data);
			return bisect<_DefaultComparator<bool>, bool>(*vec, p_value, p_before, _DefaultComparator<bool>());
		} break;
		case Variant::INT: {
			switch (_p->int_type) {
				case PackedTypedArray::INT_TYPE_SIGNED_8: {
					Vector<int8_t> *vec = reinterpret_cast<Vector<int8_t> *>(_p->data);
					return bisect<_DefaultComparator<int8_t>, int8_t>(*vec, p_value, p_before, _DefaultComparator<int8_t>());
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_8: {
					Vector<uint8_t> *vec = reinterpret_cast<Vector<uint8_t> *>(_p->data);
					return bisect<_DefaultComparator<uint8_t>, uint8_t>(*vec, p_value, p_before, _DefaultComparator<uint8_t>());
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_16: {
					Vector<int16_t> *vec = reinterpret_cast<Vector<int16_t> *>(_p->data);
					return bisect<_DefaultComparator<int16_t>, int16_t>(*vec, p_value, p_before, _DefaultComparator<int16_t>());
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_16: {
					Vector<uint16_t> *vec = reinterpret_cast<Vector<uint16_t> *>(_p->data);
					return bisect<_DefaultComparator<uint16_t>, uint16_t>(*vec, p_value, p_before, _DefaultComparator<uint16_t>());
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_32: {
					Vector<int32_t> *vec = reinterpret_cast<Vector<int32_t> *>(_p->data);
					return bisect<_DefaultComparator<int32_t>, int32_t>(*vec, p_value, p_before, _DefaultComparator<int32_t>());
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_32: {
					Vector<uint32_t> *vec = reinterpret_cast<Vector<uint32_t> *>(_p->data);
					return bisect<_DefaultComparator<uint32_t>, uint32_t>(*vec, p_value, p_before, _DefaultComparator<uint32_t>());
				} break;
				case PackedTypedArray::INT_TYPE_SIGNED_64: {
					Vector<int64_t> *vec = reinterpret_cast<Vector<int64_t> *>(_p->data);
					return bisect<_DefaultComparator<int64_t>, int64_t>(*vec, p_value, p_before, _DefaultComparator<int64_t>());
				} break;
				case PackedTypedArray::INT_TYPE_UNSIGNED_64: {
					Vector<uint64_t> *vec = reinterpret_cast<Vector<uint64_t> *>(_p->data);
					return bisect<_DefaultComparator<uint64_t>, uint64_t>(*vec, p_value, p_before, _DefaultComparator<uint64_t>());
				} break;
			}
		} break;
		case Variant::REAL: {
			Vector<real_t> *vec = reinterpret_cast<Vector<real_t> *>(_p->data);
			return bisect<_DefaultComparator<real_t>, real_t>(*vec, p_value, p_before, _DefaultComparator<real_t>());
		} break;
		case Variant::STRING: {
			Vector<String> *vec = reinterpret_cast<Vector<String> *>(_p->data);
			return bisect<_DefaultComparator<String>, String>(*vec, p_value, p_before, _DefaultComparator<String>());
		} break;
		case Variant::RECT2: {
			//Vector<Rect2> *vec = reinterpret_cast<Vector<Rect2> *>(_p->data);
			ERR_FAIL_V_MSG(-1, "Rect2s cannot be bisected directly.");
		} break;
		case Variant::RECT2I: {
			//Vector<Rect2i> *vec = reinterpret_cast<Vector<Rect2i> *>(_p->data);
			ERR_FAIL_V_MSG(-1, "Rect2is cannot be bisected directly.");
		} break;
		case Variant::VECTOR2: {
			Vector<Vector2> *vec = reinterpret_cast<Vector<Vector2> *>(_p->data);
			return bisect<_DefaultComparator<Vector2>, Vector2>(*vec, p_value, p_before, _DefaultComparator<Vector2>());
		} break;
		case Variant::VECTOR2I: {
			Vector<Vector2i> *vec = reinterpret_cast<Vector<Vector2i> *>(_p->data);
			return bisect<_DefaultComparator<Vector2i>, Vector2i>(*vec, p_value, p_before, _DefaultComparator<Vector2i>());
		} break;
		case Variant::VECTOR3: {
			Vector<Vector3> *vec = reinterpret_cast<Vector<Vector3> *>(_p->data);
			return bisect<_DefaultComparator<Vector3>, Vector3>(*vec, p_value, p_before, _DefaultComparator<Vector3>());
		} break;
		case Variant::VECTOR3I: {
			Vector<Vector3i> *vec = reinterpret_cast<Vector<Vector3i> *>(_p->data);
			return bisect<_DefaultComparator<Vector3i>, Vector3i>(*vec, p_value, p_before, _DefaultComparator<Vector3i>());
		} break;
		case Variant::VECTOR4: {
			Vector<Vector4> *vec = reinterpret_cast<Vector<Vector4> *>(_p->data);
			return bisect<_DefaultComparator<Vector4>, Vector4>(*vec, p_value, p_before, _DefaultComparator<Vector4>());
		} break;
		case Variant::VECTOR4I: {
			Vector<Vector4i> *vec = reinterpret_cast<Vector<Vector4i> *>(_p->data);
			return bisect<_DefaultComparator<Vector4i>, Vector4i>(*vec, p_value, p_before, _DefaultComparator<Vector4i>());
		} break;
		case Variant::PLANE: {
			//Vector<Plane> *vec = reinterpret_cast<Vector<Plane> *>(_p->data);
			ERR_FAIL_V_MSG(-1, "Planes cannot be bisected directly.");
		} break;
		case Variant::QUATERNION: {
			//Vector<Quaternion> *vec = reinterpret_cast<Vector<Quaternion> *>(_p->data);
			ERR_FAIL_V_MSG(-1, "Qaternions cannot be bisected directly.");
		} break;
		case Variant::AABB: {
			//Vector<AABB> *vec = reinterpret_cast<Vector<AABB> *>(_p->data);
			ERR_FAIL_V_MSG(-1, "AABBs cannot be bisected directly.");
		} break;
		case Variant::BASIS: {
			//Vector<Basis> *vec = reinterpret_cast<Vector<Basis> *>(_p->data);
			ERR_FAIL_V_MSG(-1, "Basises cannot be bisected directly.");
		} break;
		case Variant::TRANSFORM: {
			//Vector<Transform> *vec = reinterpret_cast<Vector<Transform> *>(_p->data);
			ERR_FAIL_V_MSG(-1, "Transforms cannot be bisected directly.");
		} break;
		case Variant::TRANSFORM2D: {
			//Vector<Transform2D> *vec = reinterpret_cast<Vector<Transform2D> *>(_p->data);
			ERR_FAIL_V_MSG(-1, "ransform2Ds cannot be bisected directly.");
		} break;
		case Variant::PROJECTION: {
			//Vector<Projection> *vec = reinterpret_cast<Vector<Projection> *>(_p->data);
			ERR_FAIL_V_MSG(-1, "Projections cannot be bisected directly.");
		} break;
		case Variant::COLOR: {
			Vector<Color> *vec = reinterpret_cast<Vector<Color> *>(_p->data);
			return bisect<_DefaultComparator<Color>, Color>(*vec, p_value, p_before, _DefaultComparator<Color>());
		} break;
		case Variant::NODE_PATH: {
			//Vector<NodePath> *vec = reinterpret_cast<Vector<NodePath> *>(_p->data);
			ERR_FAIL_V_MSG(-1, "NodePaths cannot be bisected directly.");
		} break;
		case Variant::RID: {
			Vector<RID> *vec = reinterpret_cast<Vector<RID> *>(_p->data);
			return bisect<_DefaultComparator<RID>, RID>(*vec, p_value, p_before, _DefaultComparator<RID>());
		} break;
		case Variant::OBJECT: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		case Variant::STRING_NAME: {
			Vector<StringName> *vec = reinterpret_cast<Vector<StringName> *>(_p->data);
			return bisect<_DefaultComparator<StringName>, StringName>(*vec, p_value, p_before, _DefaultComparator<StringName>());
		} break;
		case Variant::DICTIONARY: {
			//Vector<Dictionary> *vec = reinterpret_cast<Vector<Dictionary> *>(_p->data);
			ERR_FAIL_V_MSG(-1, "Dictionaries cannot be bisected directly.");
		} break;
		case Variant::ARRAY: {
			Vector<Array> *vec = reinterpret_cast<Vector<Array> *>(_p->data);
			return bisect<_DefaultComparator<Array>, Array>(*vec, p_value, p_before, _DefaultComparator<Array>());
		} break;
		case Variant::TYPED_ARRAY: {
			Vector<TypedArray> *vec = reinterpret_cast<Vector<TypedArray> *>(_p->data);
			return bisect<_DefaultComparator<TypedArray>, TypedArray>(*vec, p_value, p_before, _DefaultComparator<TypedArray>());
		} break;
		case Variant::PACKED_TYPED_ARRAY: {
			Vector<PackedTypedArray> *vec = reinterpret_cast<Vector<PackedTypedArray> *>(_p->data);
			return bisect<_DefaultComparator<PackedTypedArray>, PackedTypedArray>(*vec, p_value, p_before, _DefaultComparator<PackedTypedArray>());
		} break;
		case Variant::POOL_BYTE_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		case Variant::POOL_INT_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		case Variant::POOL_REAL_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		case Variant::POOL_STRING_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		case Variant::POOL_VECTOR2_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		case Variant::POOL_VECTOR2I_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		case Variant::POOL_VECTOR3_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		case Variant::POOL_VECTOR3I_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		case Variant::POOL_VECTOR4_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		case Variant::POOL_VECTOR4I_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		case Variant::POOL_COLOR_ARRAY: {
			Vector<Variant> *vec = reinterpret_cast<Vector<Variant> *>(_p->data);
			return bisect<_PackedTypedArrayVariantSort, Variant>(*vec, p_value, p_before, _PackedTypedArrayVariantSort());
		} break;
		default: {
		} break;
	}

	return -1;
}

int PackedTypedArray::bsearch_custom(const Variant &p_value, Object *p_obj, const StringName &p_function, bool p_before) {
	ERR_FAIL_NULL_V(p_obj, 0);

	if (!_p->data) {
		return -1;
	}

	int dsize = size();

	if (dsize == 0) {
		return -1;
	}

	_PackedTypedArrayVariantSortCustom less;
	less.obj = p_obj;
	less.func = p_function;

	Vector<Variant> sarray;
	sarray.resize(dsize);

	for (int i = 0; i < dsize; ++i) {
		sarray.write[i] = get(i);
	}

	return bisect(sarray, p_value, p_before, less);
}

PackedTypedArray &PackedTypedArray::invert() {
	if (!_p->data) {
		return *this;
	}

	ACCESS_DATA(vec->invert());

	return *this;
}

bool PackedTypedArray::push_front(const Variant &p_value) {
	if (!can_take_variant(p_value)) {
		return false;
	}

	_p->ensure_data();

	ACCESS_DATA(vec->insert(0, p_value));

	return true;
}

Variant PackedTypedArray::pop_back() {
	if (!_p->data) {
		return Variant();
	}
	int s = size();

	if (s == 0) {
		return Variant();
	}

	const int n = s - 1;
	const Variant ret = get(n);
	resize(n);
	return ret;
}

Variant PackedTypedArray::pop_front() {
	if (!_p->data) {
		return Variant();
	}

	int s = size();

	if (s == 0) {
		return Variant();
	}

	const Variant ret = get(0);
	remove(0);
	return ret;
}

Variant PackedTypedArray::pop_at(int p_pos) {
	if (!_p->data) {
		return Variant();
	}

	int s = size();

	if (s == 0) {
		// Return `null` without printing an error to mimic `pop_back()` and `pop_front()` behavior.
		return Variant();
	}

	if (p_pos < 0) {
		// Relative offset from the end
		p_pos = s + p_pos;
	}

	ERR_FAIL_INDEX_V_MSG(p_pos, s, Variant(),
			vformat("The calculated index %s is out of bounds (the array has %s elements). Leaving the array untouched and returning `null`.", p_pos, s));

	const Variant ret = get(p_pos);
	remove(p_pos);
	return ret;
}

Variant PackedTypedArray::min() const {
	Variant minval;
	for (int i = 0; i < size(); i++) {
		if (i == 0) {
			minval = get(i);
		} else {
			bool valid;
			Variant ret;
			Variant test = get(i);
			Variant::evaluate(Variant::OP_LESS, test, minval, ret, valid);
			if (!valid) {
				return Variant(); //not a valid comparison
			}
			if (bool(ret)) {
				//is less
				minval = test;
			}
		}
	}
	return minval;
}

Variant PackedTypedArray::max() const {
	Variant maxval;
	for (int i = 0; i < size(); i++) {
		if (i == 0) {
			maxval = get(i);
		} else {
			bool valid;
			Variant ret;
			Variant test = get(i);
			Variant::evaluate(Variant::OP_GREATER, test, maxval, ret, valid);
			if (!valid) {
				return Variant(); //not a valid comparison
			}
			if (bool(ret)) {
				//is less
				maxval = test;
			}
		}
	}
	return maxval;
}

bool PackedTypedArray::operator<(const PackedTypedArray &p_array) const {
	int a_len = size();
	int b_len = p_array.size();

	int min_cmp = MIN(a_len, b_len);

	for (int i = 0; i < min_cmp; i++) {
		if (operator[](i) < p_array[i]) {
			return true;
		} else if (p_array[i] < operator[](i)) {
			return false;
		}
	}

	return a_len < b_len;
}

bool PackedTypedArray::operator<=(const PackedTypedArray &p_array) const {
	return !operator>(p_array);
}
bool PackedTypedArray::operator>(const PackedTypedArray &p_array) const {
	return p_array < *this;
}
bool PackedTypedArray::operator>=(const PackedTypedArray &p_array) const {
	return !operator<(p_array);
}

const void *PackedTypedArray::id() const {
	return _p;
}

String PackedTypedArray::get_typename_string() const {
	if (_p->type == Variant::OBJECT) {
		return _p->object_class_name;
	}

	return Variant::get_type_name(_p->type);
}

int PackedTypedArray::get_variant_type() const {
	return _p->type;
}
void PackedTypedArray::set_variant_type(const int p_variant_type) {
	if (_p->data) {
		ERR_FAIL_COND(size() > 0);
		_p->clear();
	}

	_p->type = static_cast<Variant::Type>(p_variant_type);
}

PackedTypedArray::IntType PackedTypedArray::get_int_type() const {
	return _p->int_type;
}
void PackedTypedArray::set_int_type(const IntType p_int_type) {
	if (_p->data) {
		ERR_FAIL_COND(size() > 0);
		_p->clear();
	}

	_p->int_type = p_int_type;
}

StringName PackedTypedArray::get_object_class_name() const {
	return _p->object_class_name;
}
void PackedTypedArray::set_object_class_name(const StringName &p_object_type_name) {
	if (_p->data) {
		ERR_FAIL_COND(size() > 0);
		_p->clear();
	}

	_p->object_class_name = p_object_type_name;
	_p->is_global_class = ScriptServer::is_global_class(p_object_type_name);

	// Just warn the user.
	ERR_FAIL_COND(!validate_object_type_name(_p->object_class_name));
}

void PackedTypedArray::set_type_from_name(const StringName &p_type_name) {
	if (_p->data) {
		ERR_FAIL_COND(size() > 0);
		_p->clear();
	}

	String type_name = p_type_name;
	Variant::Type variant_type = Variant::VARIANT_MAX;

	for (int i = 0; i < Variant::VARIANT_MAX; i++) {
		if (type_name == Variant::get_type_name(Variant::Type(i))) {
			variant_type = Variant::Type(i);
			break;
		}
	}

	if (variant_type == Variant::VARIANT_MAX) {
		variant_type = Variant::OBJECT;
	}

	_p->type = variant_type;
	_p->int_type = INT_TYPE_SIGNED_64;

	if (variant_type == Variant::OBJECT) {
		_p->object_class_name = p_type_name;
		_p->is_global_class = ScriptServer::is_global_class(p_type_name);

		// Just warn the user.
		ERR_FAIL_COND(!validate_object_type_name(_p->object_class_name));
	} else {
		_p->object_class_name = StringName();
	}
}

void PackedTypedArray::set_type_from_variant(const Variant &p_variant) {
	if (_p->data) {
		ERR_FAIL_COND(size() > 0);
		_p->clear();
	}

	_p->type = p_variant.get_type();
	_p->int_type = INT_TYPE_SIGNED_64;

	if (_p->type == Variant::OBJECT) {
		Object *obj = ObjectDB::get_instance(p_variant.get_object_instance_id());

		if (!obj) {
			_p->object_class_name = StringName();
			_p->is_global_class = false;
			return;
		}

		_p->object_class_name = obj->get_class_name();
		_p->is_global_class = false;

		Ref<Script> script = obj->get_script();

		while (script.is_valid()) {
			StringName global_class_name = script->get_global_class_name();

			if (global_class_name == StringName()) {
				script = script->get_base_script();
				continue;
			}

			_p->object_class_name = global_class_name;
			_p->is_global_class = true;

			break;
		}
	} else {
		_p->object_class_name = StringName();
		_p->is_global_class = false;
	}
}

void PackedTypedArray::set_type_from_array_element(const Variant &p_array) {
	if (_p->data) {
		ERR_FAIL_COND(size() > 0);
		_p->clear();
	}

	switch (p_array.get_type()) {
		case Variant::NIL:
		case Variant::BOOL:
		case Variant::INT:
		case Variant::REAL:
		case Variant::STRING:
		case Variant::RECT2:
		case Variant::RECT2I:
		case Variant::VECTOR2:
		case Variant::VECTOR2I:
		case Variant::VECTOR3:
		case Variant::VECTOR3I:
		case Variant::VECTOR4:
		case Variant::VECTOR4I:
		case Variant::PLANE:
		case Variant::QUATERNION:
		case Variant::AABB:
		case Variant::BASIS:
		case Variant::TRANSFORM:
		case Variant::TRANSFORM2D:
		case Variant::PROJECTION:
		case Variant::COLOR:
		case Variant::NODE_PATH:
		case Variant::RID:
		case Variant::STRING_NAME:
		case Variant::DICTIONARY: {
			ERR_FAIL_MSG("Given type is not an array!");
		} break;
		case Variant::OBJECT: {
			bool valid;
			Variant element = p_array.get(0, &valid);

			ERR_FAIL_COND_MSG(!valid, "Given type is not an array!");

			set_type_from_variant(element);
		} break;
		case Variant::ARRAY: {
			Array arr = p_array;

			ERR_FAIL_COND_MSG(arr.size() == 0, "Cannot set type from empty array!");

			// Try to find the first non null element.
			// If can't be found we just use the null type
			Variant element;
			for (int i = 0; i < arr.size(); ++i) {
				element = arr[i];

				if (element.get_type() != Variant::NIL) {
					break;
				}
			}
			set_type_from_variant(element);
		} break;
		case Variant::TYPED_ARRAY: {
			TypedArray arr = p_array;
			set_type_from_name(arr.get_typename_string());
		} break;
		case Variant::PACKED_TYPED_ARRAY: {
			PackedTypedArray arr = p_array;
			set_type_from(arr);
		} break;
		case Variant::POOL_BYTE_ARRAY: {
			_p->type = Variant::INT;
			_p->int_type = INT_TYPE_UNSIGNED_8;
			_p->object_class_name = StringName();
			_p->is_global_class = false;
		} break;
		case Variant::POOL_INT_ARRAY: {
			_p->type = Variant::INT;
			_p->int_type = INT_TYPE_SIGNED_64;
			_p->object_class_name = StringName();
			_p->is_global_class = false;
		} break;
		case Variant::POOL_REAL_ARRAY: {
			_p->type = Variant::REAL;
			_p->int_type = INT_TYPE_SIGNED_64;
			_p->object_class_name = StringName();
			_p->is_global_class = false;
		} break;
		case Variant::POOL_STRING_ARRAY: {
			_p->type = Variant::STRING;
			_p->int_type = INT_TYPE_SIGNED_64;
			_p->object_class_name = StringName();
			_p->is_global_class = false;
		} break;
		case Variant::POOL_VECTOR2_ARRAY: {
			_p->type = Variant::VECTOR2;
			_p->int_type = INT_TYPE_SIGNED_64;
			_p->object_class_name = StringName();
			_p->is_global_class = false;
		} break;
		case Variant::POOL_VECTOR2I_ARRAY: {
			_p->type = Variant::VECTOR2I;
			_p->int_type = INT_TYPE_SIGNED_64;
			_p->object_class_name = StringName();
			_p->is_global_class = false;
		} break;
		case Variant::POOL_VECTOR3_ARRAY: {
			_p->type = Variant::VECTOR3;
			_p->int_type = INT_TYPE_SIGNED_64;
			_p->object_class_name = StringName();
			_p->is_global_class = false;
		} break;
		case Variant::POOL_VECTOR3I_ARRAY: {
			_p->type = Variant::VECTOR3I;
			_p->int_type = INT_TYPE_SIGNED_64;
			_p->object_class_name = StringName();
			_p->is_global_class = false;
		} break;
		case Variant::POOL_VECTOR4_ARRAY: {
			_p->type = Variant::VECTOR4;
			_p->int_type = INT_TYPE_SIGNED_64;
			_p->object_class_name = StringName();
			_p->is_global_class = false;
		} break;
		case Variant::POOL_VECTOR4I_ARRAY: {
			_p->type = Variant::VECTOR4I;
			_p->int_type = INT_TYPE_SIGNED_64;
			_p->object_class_name = StringName();
			_p->is_global_class = false;
		} break;
		case Variant::POOL_COLOR_ARRAY: {
			_p->type = Variant::COLOR;
			_p->int_type = INT_TYPE_SIGNED_64;
			_p->object_class_name = StringName();
			_p->is_global_class = false;
		} break;
		default: {
		} break;
	}
}

void PackedTypedArray::set_type_from(const PackedTypedArray &p_array) {
	if (_p->data) {
		ERR_FAIL_COND(size() > 0);
		_p->clear();
	}

	_p->type = p_array._p->type;
	_p->object_class_name = p_array._p->object_class_name;
	_p->int_type = p_array._p->int_type;
	_p->is_global_class = p_array._p->is_global_class;

	// Just warn the user.
	if (_p->type == Variant::OBJECT) {
		ERR_FAIL_COND(!validate_object_type_name(_p->object_class_name));
	}
}

bool PackedTypedArray::validate_type_name(const StringName &p_type_name) const {
	if (p_type_name == StringName()) {
		return false;
	}

	String type_name = p_type_name;

	for (int i = 0; i < Variant::VARIANT_MAX; i++) {
		if (type_name == Variant::get_type_name(Variant::Type(i))) {
			return true;
		}
	}

	if (ScriptServer::is_global_class(p_type_name)) {
		return true;
	}

	return ClassDB::class_exists(p_type_name);
}

bool PackedTypedArray::validate_object_type_name(const StringName &p_type_name) const {
	if (p_type_name == StringName()) {
		return false;
	}

	if (ScriptServer::is_global_class(p_type_name)) {
		return true;
	}

	return ClassDB::class_exists(p_type_name);
}

bool PackedTypedArray::can_take_variant(const Variant &p_value) const {
	if (p_value.get_type() == Variant::NIL) {
		if (_p->type == Variant::OBJECT) {
			return true;
		}
	}

	if (_p->type != p_value.get_type()) {
		return false;
	}

	if (_p->type == Variant::OBJECT) {
		Object *obj = ObjectDB::get_instance(p_value.get_object_instance_id());

		if (!obj) {
			// can this happen?
			return true;
		}

		// The set object_class_name is a registered script global class
		// We don't need to care about engine side classes anymore.
		if (_p->is_global_class) {
			Ref<Script> script = obj->get_script();

			while (script.is_valid()) {
				StringName global_class_name = script->get_global_class_name();

				if (global_class_name == StringName()) {
					continue;
				}

				// There is no multiple inheritance, so the first global name we find should suffice
				if (_p->object_class_name == global_class_name) {
					return true;
				}

				script = script->get_base_script();
			}

			return false;
		}

		return obj->is_class(_p->object_class_name);
	}

	return true;
}

// Returns the internal Vector<>'s pointer
const void *PackedTypedArray::vector_ptr() const {
	return _p->data;
}
void *PackedTypedArray::vector_ptrw() {
	return _p->data;
}

// Returns the internal vector<>'s internal aiiay's pointer. Vector<>.ptr()
const void *PackedTypedArray::ptr() const {
	if (!_p->data) {
		return NULL;
	}

	ACCESS_DATA(return vec->ptr());

	return NULL;
}
void *PackedTypedArray::ptrw() {
	if (!_p->data) {
		return NULL;
	}

	ACCESS_DATA(return vec->ptrw());

	return NULL;
}

PackedTypedArray::PackedTypedArray() {
	_p = memnew(PackedTypedArrayPrivate);
	_p->refcount.init();
}

PackedTypedArray::PackedTypedArray(const PackedTypedArray &p_from) {
	_p = nullptr;
	_ref(p_from);
}

PackedTypedArray::PackedTypedArray(const StringName &p_type_name, const Variant &p_from) {
	_p = memnew(PackedTypedArrayPrivate);
	_p->refcount.init();
	append_from(p_from);
}

PackedTypedArray::PackedTypedArray(const StringName &p_type_name, const Variant &p_from, const IntType p_int_type) {
	_p = memnew(PackedTypedArrayPrivate);
	_p->refcount.init();
	set_type_from_name(p_type_name);
	_p->int_type = p_int_type;
	append_from(p_from);
}

/*
PackedTypedArray::PackedTypedArray(const StringName &p_type_name) {
	_p = memnew(PackedTypedArrayPrivate);
	_p->refcount.init();
	set_type_from_name(p_type_name);
}
*/

PackedTypedArray::~PackedTypedArray() {
	_unref();
}

#undef ACCESS_DATA
#undef DO_DEEP_COMPARE_VECTORS
#undef DO_DEEP_COMPARE_VARIANT_BASED_VECTORS
