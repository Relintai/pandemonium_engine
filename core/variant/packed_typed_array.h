#ifndef PACKED_TYPED_ARRAY_H
#define PACKED_TYPED_ARRAY_H

/*************************************************************************/
/*  packed_typed_array.h                                                 */
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

#include "core/typedefs.h"

class Variant;
class PackedTypedArrayPrivate;
class Object;
class StringName;
class String;

class PackedTypedArray {
	mutable PackedTypedArrayPrivate *_p;
	void _ref(const PackedTypedArray &p_from) const;
	void _unref() const;

	inline int _clamp_slice_index(int p_index) const;

	void _push_back_unchecked(const Variant &p_value);
	void _set_unchecked(int p_idx, const Variant &p_value);

public:
	enum IntType {
		INT_TYPE_SIGNED_8,
		INT_TYPE_UNSIGNED_8,
		INT_TYPE_SIGNED_16,
		INT_TYPE_UNSIGNED_16,
		INT_TYPE_SIGNED_32,
		INT_TYPE_UNSIGNED_32,
		INT_TYPE_SIGNED_64,
		INT_TYPE_UNSIGNED_64,
	};

	const Variant operator[](int p_idx) const;

	bool set(int p_idx, const Variant &p_value);
	const Variant get(int p_idx) const;

	int size() const;
	bool empty() const;
	void clear();

	bool deep_equal(const PackedTypedArray &p_array, int p_recursion_count = 0) const;
	bool operator==(const PackedTypedArray &p_array) const;

	uint32_t hash() const;
	uint32_t recursive_hash(int p_recursion_count) const;
	void operator=(const PackedTypedArray &p_array);

	bool push_back(const Variant &p_value);
	_FORCE_INLINE_ bool append(const Variant &p_value) { return push_back(p_value); } //for python compatibility
	bool append_array(const PackedTypedArray &p_array);
	bool append_from(const Variant &p_array);
	Error resize(int p_new_size);

	bool insert(int p_pos, const Variant &p_value);
	void remove(int p_pos);
	bool fill(const Variant &p_value);

	Variant front() const;
	Variant back() const;

	PackedTypedArray &sort();
	PackedTypedArray &sort_custom(Object *p_obj, const StringName &p_function);
	void shuffle();
	int bsearch(const Variant &p_value, bool p_before = true);
	int bsearch_custom(const Variant &p_value, Object *p_obj, const StringName &p_function, bool p_before = true);
	PackedTypedArray &invert();

	int find(const Variant &p_value, int p_from = 0) const;
	int rfind(const Variant &p_value, int p_from = -1) const;
	int find_last(const Variant &p_value) const;
	int count(const Variant &p_value) const;
	bool has(const Variant &p_value) const;

	void erase(const Variant &p_value);

	bool push_front(const Variant &p_value);
	Variant pop_back();
	Variant pop_front();
	Variant pop_at(int p_pos);

	PackedTypedArray duplicate(bool p_deep = false) const;

	PackedTypedArray slice(int p_begin, int p_end, int p_step = 1, bool p_deep = false) const;

	Variant min() const;
	Variant max() const;

	bool operator<(const PackedTypedArray &p_array) const;
	bool operator<=(const PackedTypedArray &p_array) const;
	bool operator>(const PackedTypedArray &p_array) const;
	bool operator>=(const PackedTypedArray &p_array) const;

	const void *id() const;

	String get_typename_string() const;

	int get_variant_type() const;
	void set_variant_type(const int p_variant_type);

	IntType get_int_type() const;
	void set_int_type(const IntType p_int_type);

	StringName get_object_class_name() const;
	void set_object_class_name(const StringName &p_object_type_name);

	void set_type_from_name(const StringName &p_type_name);
	void set_type_from_variant(const Variant &p_variant);
	void set_type_from_array_element(const Variant &p_array);
	void set_type_from(const PackedTypedArray &p_array);

	bool validate_type_name(const StringName &p_type_name) const;
	bool validate_object_type_name(const StringName &p_type_name) const;
	bool can_take_variant(const Variant &p_value) const;

	// Returns the internal Vector<>'s pointer.
	// Can be NULL!
	// Look at the .cpp file to know what to cast this to. (ACCESS_DATA define for example.)
	const void *vector_ptr() const;
	void *vector_ptrw();

	// Returns the internal vector<>'s internal array's pointer. Vector<>->ptr().
	// Can be NULL!
	// Look at the .cpp file to know what to cast this to. (ACCESS_DATA define for example.)
	const void *ptr() const;
	void *ptrw();

	PackedTypedArray();
	PackedTypedArray(const PackedTypedArray &p_from);
	PackedTypedArray(const StringName &p_type_name, const Variant &p_from);
	PackedTypedArray(const StringName &p_type_name, const Variant &p_from, const IntType p_int_type);

	// Both this and the TypedArray(const TypedArray &p_from); constructors are useful,
	// but having them both will do lots of clashes when converting from Variant.
	// Maybe static create method should be added or something
	//PackedTypedArray(const StringName &p_type_name);

	~PackedTypedArray();
};

#endif // ARRAY_H
