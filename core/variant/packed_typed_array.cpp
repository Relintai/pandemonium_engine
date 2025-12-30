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
#include "core/variant/variant.h"

class PackedTypedArrayPrivate {
public:
	struct ObjData {
		// Will be null for every type deriving from Reference as they have their
		// own reference count mechanism
		ObjectRC *rc;
		// Always initialized, but will be null if the Ref<> assigned was null
		// or this Variant is not even holding a Reference-derived object
		RefPtr ref;
	};

	_FORCE_INLINE_ ObjData &_get_obj(const int p_index) {
		return *reinterpret_cast<ObjData *>(&data[p_index]->_mem[0]);
	}

	_FORCE_INLINE_ const ObjData &_get_obj(const int p_index) const {
		return *reinterpret_cast<const ObjData *>(&data[p_index]->_mem[0]);
	}

	union PackedTypedArrayPrivateEntry {
		bool _bool;
		int64_t _int;
		double _real;
		Transform2D *_transform2d;
		::AABB *_aabb;
		Basis *_basis;
		Transform *_transform;
		Projection *_projection;
		void *_ptr; //generic pointer
		uint8_t _mem[sizeof(ObjData) > (sizeof(real_t) * 4) ? sizeof(ObjData) : (sizeof(real_t) * 4)]{ 0 };
	} GCC_ALIGNED_8;

	PackedTypedArrayPrivate() {
	}
	~PackedTypedArrayPrivate() {
	}

	SafeRefCount refcount;

	Variant::Type type;
	StringName object_class_name;

	// Maybe this should just be a straight up void* that would achieve tighter packing in some cases.
	LocalVector<PackedTypedArrayPrivateEntry *> data;

	// Temporary
	Vector<Variant> array;
};

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

Variant PackedTypedArray::operator[](int p_idx) {
	return _p->array.write[p_idx];
}

const Variant PackedTypedArray::operator[](int p_idx) const {
	return _p->array[p_idx];
}

int PackedTypedArray::size() const {
	return _p->array.size();
}
bool PackedTypedArray::empty() const {
	return _p->array.empty();
}
void PackedTypedArray::clear() {
	_p->array.clear();
}

bool PackedTypedArray::deep_equal(const PackedTypedArray &p_array, int p_recursion_count) const {
	// Cheap checks
	ERR_FAIL_COND_V_MSG(p_recursion_count > MAX_RECURSION, true, "Max recursion reached");
	if (_p == p_array._p) {
		return true;
	}
	const Vector<Variant> &a1 = _p->array;
	const Vector<Variant> &a2 = p_array._p->array;
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

bool PackedTypedArray::operator==(const PackedTypedArray &p_array) const {
	return _p == p_array._p;
}

uint32_t PackedTypedArray::hash() const {
	return recursive_hash(0);
}

uint32_t PackedTypedArray::recursive_hash(int p_recursion_count) const {
	ERR_FAIL_COND_V_MSG(p_recursion_count > MAX_RECURSION, 0, "Max recursion reached");
	p_recursion_count++;

	uint32_t h = hash_murmur3_one_32(0);

	for (int i = 0; i < _p->array.size(); i++) {
		h = hash_murmur3_one_32(_p->array[i].recursive_hash(p_recursion_count), h);
	}
	return hash_fmix32(h);
}

void PackedTypedArray::operator=(const PackedTypedArray &p_array) {
	_ref(p_array);
}

void PackedTypedArray::push_back(const Variant &p_value) {
	_p->array.push_back(p_value);
}

void PackedTypedArray::append_array(const PackedTypedArray &p_array) {
	_p->array.append_array(p_array._p->array);
}

Error PackedTypedArray::resize(int p_new_size) {
	return _p->array.resize(p_new_size);
}

void PackedTypedArray::insert(int p_pos, const Variant &p_value) {
	_p->array.insert(p_pos, p_value);
}

void PackedTypedArray::fill(const Variant &p_value) {
	_p->array.fill(p_value);
}

void PackedTypedArray::erase(const Variant &p_value) {
	_p->array.erase(p_value);
}

Variant PackedTypedArray::front() const {
	ERR_FAIL_COND_V_MSG(_p->array.size() == 0, Variant(), "Can't take value from empty array.");
	return operator[](0);
}

Variant PackedTypedArray::back() const {
	ERR_FAIL_COND_V_MSG(_p->array.size() == 0, Variant(), "Can't take value from empty array.");
	return operator[](_p->array.size() - 1);
}

int PackedTypedArray::find(const Variant &p_value, int p_from) const {
	return _p->array.find(p_value, p_from);
}

int PackedTypedArray::rfind(const Variant &p_value, int p_from) const {
	if (_p->array.size() == 0) {
		return -1;
	}

	if (p_from < 0) {
		// Relative offset from the end
		p_from = _p->array.size() + p_from;
	}
	if (p_from < 0 || p_from >= _p->array.size()) {
		// Limit to array boundaries
		p_from = _p->array.size() - 1;
	}

	for (int i = p_from; i >= 0; i--) {
		if (_p->array[i] == p_value) {
			return i;
		}
	}

	return -1;
}

int PackedTypedArray::find_last(const Variant &p_value) const {
	return rfind(p_value);
}

int PackedTypedArray::count(const Variant &p_value) const {
	if (_p->array.size() == 0) {
		return 0;
	}

	int amount = 0;
	for (int i = 0; i < _p->array.size(); i++) {
		if (_p->array[i] == p_value) {
			amount++;
		}
	}

	return amount;
}

bool PackedTypedArray::has(const Variant &p_value) const {
	return _p->array.find(p_value, 0) != -1;
}

void PackedTypedArray::remove(int p_pos) {
	_p->array.remove(p_pos);
}

void PackedTypedArray::set(int p_idx, const Variant &p_value) {
	operator[](p_idx) = p_value;
}

const Variant PackedTypedArray::get(int p_idx) const {
	return operator[](p_idx);
}

PackedTypedArray PackedTypedArray::duplicate(bool p_deep) const {
	PackedTypedArray new_arr;
	int element_count = size();
	new_arr.resize(element_count);
	for (int i = 0; i < element_count; i++) {
		new_arr[i] = p_deep ? get(i).duplicate(p_deep) : get(i);
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
			new_arr[dest_idx++] = p_deep ? get(idx).duplicate(p_deep) : get(idx);
		}
	} else { // p_step < 0
		int dest_idx = 0;
		for (int idx = begin; idx >= end; idx += p_step) {
			ERR_FAIL_COND_V_MSG(dest_idx < 0 || dest_idx >= new_arr_size, PackedTypedArray(), "Bug in PackedTypedArray slice()");
			new_arr[dest_idx++] = p_deep ? get(idx).duplicate(p_deep) : get(idx);
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
	_p->array.sort_custom<_PackedTypedArrayVariantSort>();
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

	SortArray<Variant, _PackedTypedArrayVariantSortCustom, true> avs;
	avs.compare.obj = p_obj;
	avs.compare.func = p_function;
	avs.sort(_p->array.ptrw(), _p->array.size());
	return *this;
}

void PackedTypedArray::shuffle() {
	const int n = _p->array.size();
	if (n < 2) {
		return;
	}
	Variant *data = _p->array.ptrw();
	for (int i = n - 1; i >= 1; i--) {
		const int j = Math::rand() % (i + 1);
		const Variant tmp = data[j];
		data[j] = data[i];
		data[i] = tmp;
	}
}

template <typename Less>
_FORCE_INLINE_ int bisect(const Vector<Variant> &p_array, const Variant &p_value, bool p_before, const Less &p_less) {
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
	return bisect(_p->array, p_value, p_before, _PackedTypedArrayVariantSort());
}

int PackedTypedArray::bsearch_custom(const Variant &p_value, Object *p_obj, const StringName &p_function, bool p_before) {
	ERR_FAIL_NULL_V(p_obj, 0);

	_PackedTypedArrayVariantSortCustom less;
	less.obj = p_obj;
	less.func = p_function;

	return bisect(_p->array, p_value, p_before, less);
}

PackedTypedArray &PackedTypedArray::invert() {
	_p->array.invert();
	return *this;
}

void PackedTypedArray::push_front(const Variant &p_value) {
	_p->array.insert(0, p_value);
}

Variant PackedTypedArray::pop_back() {
	if (!_p->array.empty()) {
		const int n = _p->array.size() - 1;
		const Variant ret = _p->array.get(n);
		_p->array.resize(n);
		return ret;
	}
	return Variant();
}

Variant PackedTypedArray::pop_front() {
	if (!_p->array.empty()) {
		const Variant ret = _p->array.get(0);
		_p->array.remove(0);
		return ret;
	}
	return Variant();
}

Variant PackedTypedArray::pop_at(int p_pos) {
	if (_p->array.empty()) {
		// Return `null` without printing an error to mimic `pop_back()` and `pop_front()` behavior.
		return Variant();
	}

	if (p_pos < 0) {
		// Relative offset from the end
		p_pos = _p->array.size() + p_pos;
	}

	ERR_FAIL_INDEX_V_MSG(
			p_pos,
			_p->array.size(),
			Variant(),
			vformat(
					"The calculated index %s is out of bounds (the array has %s elements). Leaving the array untouched and returning `null`.",
					p_pos,
					_p->array.size()));

	const Variant ret = _p->array.get(p_pos);
	_p->array.remove(p_pos);
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
	_p->type = static_cast<Variant::Type>(p_variant_type);
}

StringName PackedTypedArray::get_object_class_name() const {
	return _p->object_class_name;
}
void PackedTypedArray::set_object_class_name(const StringName &p_object_type_name) {
	_p->object_class_name = p_object_type_name;
}

void PackedTypedArray::set_type_from_name(const StringName &p_type_name) {
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
	if (variant_type == Variant::OBJECT) {
		_p->object_class_name = p_type_name;
	} else {
		_p->object_class_name = StringName();
	}
}

void PackedTypedArray::set_type_from(const PackedTypedArray &p_array) {
	ERR_FAIL_COND(_p->array.size() > 0);

	_p->type = p_array._p->type;
	_p->object_class_name = p_array._p->object_class_name;
}

bool PackedTypedArray::can_take_variant(const Variant &p_value) {
	if (_p->type != p_value.get_type()) {
		return false;
	}

	if (_p->type == Variant::OBJECT) {
		//TODO check type
	}

	return true;
}

PackedTypedArray::PackedTypedArray(const PackedTypedArray &p_from) {
	_p = nullptr;
	_ref(p_from);
}

PackedTypedArray::PackedTypedArray() {
	_p = memnew(PackedTypedArrayPrivate);
	_p->refcount.init();
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
