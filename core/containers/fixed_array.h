#ifndef FIXED_ARRAY_H
#define FIXED_ARRAY_H

/*************************************************************************/
/*  fixed_array.h                                                        */
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

#include "core/containers/local_vector.h"

// High performance fixed size array, single threaded.
// Especially useful if you need to create an array on the stack, to
// prevent dynamic allocations (especially in bottleneck code).

template <class T, uint32_t CAPACITY = 8, bool force_trivial = false, uint32_t ALIGN = 1>
class FixedArray {
	static_assert(ALIGN > 0, "ALIGN must be at least 1.");
	static_assert(CAPACITY > 0, "CAPACITY must be at least 1.");
	const static uint32_t UNIT_SIZE = ((sizeof(T) + ALIGN - 1) / ALIGN * ALIGN);
	const static bool CONSTRUCT = !HAS_TRIVIAL_CONSTRUCTOR(T) && !force_trivial;
	const static bool DESTRUCT = !HAS_TRIVIAL_DESTRUCTOR(T) && !force_trivial;

	uint32_t _size = 0;
	uint8_t _data[CAPACITY * UNIT_SIZE];

	const T &get(uint32_t p_index) const {
		return *(const T *)&_data[p_index * UNIT_SIZE];
	}

	T &get(uint32_t p_index) {
		return *(T *)&_data[p_index * UNIT_SIZE];
	}

public:
	uint32_t size() const { return _size; }
	bool is_empty() const { return !_size; }
	bool is_full() const { return _size == CAPACITY; }
	uint32_t capacity() const { return CAPACITY; }

	T *request(bool p_construct = true) {
		if (size() < CAPACITY) {
			T *ele = &get(_size++);
			if (CONSTRUCT && p_construct) {
				memnew_placement(ele, T);
			}
			return ele;
		}
		return nullptr;
	}

	void push_back(const T &p_val) {
		T *mem = request(false);
		ERR_FAIL_NULL(mem);
		*mem = p_val;
	}

	void clear() {
		resize(0);
	}

	void remove_unordered(uint32_t p_index) {
		ERR_FAIL_UNSIGNED_INDEX(p_index, _size);

		_size--;
		if (_size > p_index) {
			get(p_index) = get(_size);
		}

		if (DESTRUCT) {
			get(_size).~T();
		}
	}

	void resize(uint32_t p_size) {
		ERR_FAIL_COND(p_size > CAPACITY);

		if (DESTRUCT && (p_size < _size)) {
			for (uint32_t i = p_size; i < _size; i++) {
				get(i).~T();
			}
		}

		if (CONSTRUCT && (p_size > _size)) {
			for (uint32_t i = _size; i < p_size; i++) {
				memnew_placement(&get(i), T);
			}
		}

		_size = p_size;
	}

	bool pop() {
		if (!size()) {
			return false;
		}
		resize(size() - 1);
		return true;
	}

	void insert(const T &p_val, uint32_t p_index, uint32_t p_max_size = CAPACITY) {
		// Insert will drop the last item if the array is already full,
		// as the fixed array is not growable.
		// We can also optionally set p_max_size to grow to a smaller size than CAPACITY.
		DEV_ASSERT(p_max_size <= CAPACITY);
		//DEV_ASSERT(p_index < CAPACITY);

		// We can only insert to size()+1 when the array is not full already.
		ERR_FAIL_UNSIGNED_INDEX(p_index, MIN(p_max_size, size() + 1));

		int32_t move_end = int32_t(size()) - 1;

		// Two possibles, either we are adding to the list or lopping one off.
		if (size() < p_max_size) {
			resize(size() + 1);
		}

		move_end = MIN(move_end, (int32_t)p_max_size - 2);

		for (int32_t n = move_end; n >= (int32_t)p_index; n--) {
			get(n + 1) = get(n);
		}

		// Save the new value.
		get(p_index) = p_val;
	}

	const T &operator[](uint32_t p_index) const {
		DEV_ASSERT(p_index < size());
		return get(p_index);
	}

	T &operator[](uint32_t p_index) {
		DEV_ASSERT(p_index < size());
		return get(p_index);
	}

	const T &last() const {
		DEV_ASSERT(size());
		return (*this)[size() - 1];
	}
	T &last() {
		DEV_ASSERT(size());
		return (*this)[size() - 1];
	}
	const T &first() const {
		DEV_ASSERT(size());
		return (*this)[0];
	}
	T &first() {
		DEV_ASSERT(size());
		return (*this)[0];
	}

	operator Vector<T>() const {
		Vector<T> ret;
		if (size()) {
			ret.resize(size());
			T *dest = ret.ptrw();
			if (ALIGN <= 1) {
				memcpy(dest, _data, sizeof(T) * _size);
			} else {
				for (uint32_t n = 0; n < _size; n++) {
					dest[n] = get(n);
				}
			}
		}
		return ret;
	}
};

#endif // FIXED_ARRAY_H
