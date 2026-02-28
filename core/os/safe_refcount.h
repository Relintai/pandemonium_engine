#ifndef SAFE_REFCOUNT_H
#define SAFE_REFCOUNT_H

/*************************************************************************/
/*  safe_refcount.h                                                      */
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

#ifdef NO_THREADS

template <class T>
class SafeNumeric {
protected:
	T value;

public:
	_ALWAYS_INLINE_ void set(T p_value) {
		value = p_value;
	}

	_ALWAYS_INLINE_ T get() const {
		return value;
	}

	_ALWAYS_INLINE_ T increment() {
		return ++value;
	}

	_ALWAYS_INLINE_ T postincrement() {
		return value++;
	}

	_ALWAYS_INLINE_ T decrement() {
		return --value;
	}

	_ALWAYS_INLINE_ T postdecrement() {
		return value--;
	}

	_ALWAYS_INLINE_ T add(T p_value) {
		return value += p_value;
	}

	_ALWAYS_INLINE_ T postadd(T p_value) {
		T old = value;
		value += p_value;
		return old;
	}

	_ALWAYS_INLINE_ T sub(T p_value) {
		return value -= p_value;
	}

	_ALWAYS_INLINE_ T postsub(T p_value) {
		T old = value;
		value -= p_value;
		return old;
	}

	_ALWAYS_INLINE_ T exchange_if_greater(T p_value) {
		if (value < p_value) {
			value = p_value;
		}
		return value;
	}

	_ALWAYS_INLINE_ T conditional_increment() {
		if (value == 0) {
			return 0;
		} else {
			return ++value;
		}
	}

	_ALWAYS_INLINE_ bool compare_exchange_weak(T &p_expected, T p_desired) {
		if (value == p_expected) {
			value = p_desired;
			return true;
		} else {
			return false;
		}
	}

	_ALWAYS_INLINE_ bool compare_exchange_strong(T &p_expected, T p_desired) {
		if (value == p_expected) {
			value = p_desired;
			return true;
		} else {
			return false;
		}
	}

	_ALWAYS_INLINE_ explicit SafeNumeric(T p_value = static_cast<T>(0)) :
			value(p_value) {
	}
};

// Template for atomic pointer types
template <class T>
class SafePointer {
	T _value;

public:
	_ALWAYS_INLINE_ void set(T p_value) {
		_value = p_value;
	}

	_ALWAYS_INLINE_ T get() const {
		return _value;
	}

	_ALWAYS_INLINE_ bool compare_exchange_weak(T &p_expected, T p_desired) {
		if (_value == p_expected) {
			_value = p_desired;
			return true;
		} else {
			return false;
		}
	}

	_ALWAYS_INLINE_ bool compare_exchange_strong(T &p_expected, T p_desired) {
		if (_value == p_expected) {
			_value = p_desired;
			return true;
		} else {
			return false;
		}
	}

	_ALWAYS_INLINE_ explicit SafePointer(T p_value = nullptr) {
		_value = p_value;
	}
};

class SafeFlag {
protected:
	bool flag;

public:
	_ALWAYS_INLINE_ bool is_set() const {
		return flag;
	}

	_ALWAYS_INLINE_ void set() {
		flag = true;
	}

	_ALWAYS_INLINE_ void clear() {
		flag = false;
	}

	_ALWAYS_INLINE_ void set_to(bool p_value) {
		flag = p_value;
	}

	_ALWAYS_INLINE_ bool test_and_set() {
		if (flag) {
			return false;
		}

		flag = true;

		return true;
	}

	_ALWAYS_INLINE_ explicit SafeFlag(bool p_value = false) :
			flag(p_value) {}
};

class SafeRefCount {
	uint32_t count;

public:
	_ALWAYS_INLINE_ bool ref() { // true on success
		if (count != 0) {
			++count;
			return true;
		} else {
			return false;
		}
	}

	_ALWAYS_INLINE_ uint32_t refval() { // none-zero on success
		if (count != 0) {
			return ++count;
		} else {
			return 0;
		}
	}

	_ALWAYS_INLINE_ bool unref() { // true if must be disposed of
		return --count == 0;
	}

	_ALWAYS_INLINE_ uint32_t unrefval() { // 0 if must be disposed of
		return --count;
	}

	_ALWAYS_INLINE_ uint32_t get() const {
		return count;
	}

	_ALWAYS_INLINE_ void init(uint32_t p_value = 1) {
		count = p_value;
	}

	SafeRefCount() :
			count(0) {}
};

#elif defined(__GNUC__)

template <class T>
class SafeNumeric {
	T _value;

public:
	_ALWAYS_INLINE_ void set(T p_value) {
		// Hack for gcc having no plain store with the old syncs.
		// If this ends up having issues on some platforms, a spinlock can be implemented.
		// But people seems to use this as a set operation, so it's linkely not going to be an issue.
		// Gcc docs:
		// Many targets have only minimal support for such locks, and do not support a full exchange operation. In this case, a target may
		// support reduced functionality here by which the only valid value to store is the immediate constant 1.
		// The exact value actually stored in *ptr is implementation defined.
		__sync_lock_test_and_set(&_value, p_value);
	}

	_ALWAYS_INLINE_ T get() const {
		// Hack for gcc having no plain load with the old syncs.
		return __sync_fetch_and_add((volatile T *)&_value, 0);
	}

	_ALWAYS_INLINE_ T increment() {
		return __sync_add_and_fetch(&_value, 1);
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postincrement() {
		return __sync_fetch_and_add(&_value, 1);
	}

	_ALWAYS_INLINE_ T decrement() {
		return __sync_sub_and_fetch(&_value, 1);
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postdecrement() {
		return __sync_fetch_and_sub(&_value, 1);
	}

	_ALWAYS_INLINE_ T add(T p_value) {
		return __sync_add_and_fetch(&_value, p_value);
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postadd(T p_value) {
		return __sync_fetch_and_add(&_value, p_value);
	}

	_ALWAYS_INLINE_ T sub(T p_value) {
		return __sync_sub_and_fetch(&_value, p_value);
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postsub(T p_value) {
		return __sync_fetch_and_sub(&_value, p_value);
	}

	_ALWAYS_INLINE_ T exchange_if_greater(T p_value) {
		while (true) {
			T tmp = get();

			if (tmp >= _value) {
				return tmp; // already greater, or equal
			}

			if (__sync_bool_compare_and_swap(&_value, tmp, _value)) {
				return _value;
			}
		}
	}

	_ALWAYS_INLINE_ T conditional_increment() {
		while (true) {
			T tmp = get();

			if (tmp == 0) {
				return 0;
			}

			if (__sync_bool_compare_and_swap(&_value, tmp, tmp + 1)) {
				return tmp + 1;
			}
		}
	}

	_ALWAYS_INLINE_ bool compare_exchange_weak(T &p_expected, T p_desired) {
		T old_value = p_expected;

		p_expected = __sync_val_compare_and_swap(&_value, p_expected, p_desired);

		return p_expected == old_value;
	}

	_ALWAYS_INLINE_ bool compare_exchange_strong(T &p_expected, T p_desired) {
		T old_value = p_expected;

		p_expected = __sync_val_compare_and_swap(&_value, p_expected, p_desired);

		return p_expected == old_value;
	}

	_ALWAYS_INLINE_ explicit SafeNumeric(T p_value = static_cast<T>(0)) {
		set(p_value);
	}
};

// Template for atomic pointer types
template <class T>
class SafePointer {
	T _value;

public:
	_ALWAYS_INLINE_ void set(T p_value) {
		// Hack for gcc having no plain store with the old syncs.
		// If this ends up having issues on some platforms, a spinlock can be implemented.
		// But people seems to use this as a set operation, so it's linkely not going to be an issue.
		// Gcc docs:
		// Many targets have only minimal support for such locks, and do not support a full exchange operation. In this case, a target may
		// support reduced functionality here by which the only valid value to store is the immediate constant 1.
		// The exact value actually stored in *ptr is implementation defined.
		(void)__sync_lock_test_and_set(&_value, p_value);
	}

	_ALWAYS_INLINE_ T get() const {
		__sync_synchronize();
		return _value;
	}

	_ALWAYS_INLINE_ bool compare_exchange_weak(T &p_expected, T p_desired) {
		T old_value = p_expected;

		p_expected = __sync_val_compare_and_swap(&_value, p_expected, p_desired);

		return p_expected == old_value;
	}

	_ALWAYS_INLINE_ bool compare_exchange_strong(T &p_expected, T p_desired) {
		T old_value = p_expected;

		p_expected = __sync_val_compare_and_swap(&_value, p_expected, p_desired);

		return p_expected == old_value;
	}

	_ALWAYS_INLINE_ explicit SafePointer(T p_value = nullptr) {
		set(p_value);
	}
};

class SafeFlag {
	uint8_t _flag;

public:
	_ALWAYS_INLINE_ bool is_set() const {
		// Hack for gcc having no plain load with the old syncs.
		return __sync_fetch_and_add((volatile uint8_t *)&_flag, 0);
	}

	_ALWAYS_INLINE_ void set() {
		while (true) {
			bool tmp = is_set() ? 1 : 0;

			if (tmp) {
				return;
			}

			if (__sync_bool_compare_and_swap(&_flag, 0, 1)) {
				return;
			}
		}
	}

	_ALWAYS_INLINE_ void clear() {
		while (true) {
			uint8_t tmp = is_set() ? 1 : 0;

			if (!tmp) {
				return;
			}

			if (__sync_bool_compare_and_swap(&_flag, 1, 0)) {
				return;
			}
		}
	}

	_ALWAYS_INLINE_ void set_to(bool p_value) {
		uint8_t vi = p_value ? 1 : 0;

		while (true) {
			uint8_t tmp = is_set() ? 1 : 0;

			if (tmp == vi) {
				return;
			}

			if (__sync_bool_compare_and_swap(&_flag, tmp, vi)) {
				return;
			}
		}
	}

	_ALWAYS_INLINE_ bool test_and_set() {
		bool tmp = is_set();

		if (tmp) {
			return false;
		}

		return __sync_bool_compare_and_swap(&_flag, 0, 1);
	}

	_ALWAYS_INLINE_ explicit SafeFlag(bool p_value = false) {
		_flag = 0;

		set_to(p_value);
	}
};

class SafeRefCount {
	SafeNumeric<uint32_t> count;

public:
	_ALWAYS_INLINE_ bool ref() { // true on success
		return count.conditional_increment() != 0;
	}

	_ALWAYS_INLINE_ uint32_t refval() { // none-zero on success
		return count.conditional_increment();
	}

	_ALWAYS_INLINE_ bool unref() { // true if must be disposed of
		return count.decrement() == 0;
	}

	_ALWAYS_INLINE_ uint32_t unrefval() { // 0 if must be disposed of
		return count.decrement();
	}

	_ALWAYS_INLINE_ uint32_t get() const {
		return count.get();
	}

	_ALWAYS_INLINE_ void init(uint32_t p_value = 1) {
		count.set(p_value);
	}
};

#elif defined(_MSC_VER)

// For MSVC use a separate compilation unit to prevent windows.h from polluting
// the global namespace.

uint32_t atomic_load(volatile uint32_t *ptarget);
void atomic_set(volatile uint32_t *ptarget, volatile uint32_t pw);
uint32_t atomic_add(volatile uint32_t *pw, volatile uint32_t val);
uint32_t atomic_post_add(volatile uint32_t *pw, volatile uint32_t val);
uint32_t atomic_sub(volatile uint32_t *pw, volatile uint32_t val);
uint32_t atomic_post_sub(volatile uint32_t *pw, volatile uint32_t val);
uint32_t atomic_exchange_if_greater(volatile uint32_t *pw, volatile uint32_t val);
uint32_t atomic_conditional_increment(volatile uint32_t *pw);
bool atomic_bool_compare_and_swap(volatile uint32_t *pw, volatile uint32_t oldval, volatile uint32_t newval);
uint32_t atomic_val_compare_and_swap(volatile uint32_t *pw, volatile uint32_t oldval, volatile uint32_t newval);

int32_t atomic_load(volatile int32_t *ptarget);
void atomic_set(volatile int32_t *ptarget, volatile int32_t pw);
int32_t atomic_add(volatile int32_t *pw, volatile int32_t val);
int32_t atomic_post_add(volatile int32_t *pw, volatile int32_t val);
int32_t atomic_sub(volatile int32_t *pw, volatile int32_t val);
int32_t atomic_post_sub(volatile int32_t *pw, volatile int32_t val);
int32_t atomic_exchange_if_greater(volatile int32_t *pw, volatile int32_t val);
int32_t atomic_conditional_increment(volatile int32_t *pw);
bool atomic_bool_compare_and_swap(volatile int32_t *pw, volatile int32_t oldval, volatile int32_t newval);
int32_t atomic_val_compare_and_swap(volatile int32_t *pw, volatile int32_t oldval, volatile int32_t newval);

uint64_t atomic_load(volatile uint64_t *ptarget);
void atomic_set(volatile uint64_t *ptarget, volatile uint64_t pw);
uint64_t atomic_add(volatile uint64_t *pw, volatile uint64_t val);
uint64_t atomic_post_add(volatile uint64_t *pw, volatile uint64_t val);
uint64_t atomic_sub(volatile uint64_t *pw, volatile uint64_t val);
uint64_t atomic_post_sub(volatile uint64_t *pw, volatile uint64_t val);
uint64_t atomic_exchange_if_greater(volatile uint64_t *pw, volatile uint64_t val);
uint64_t atomic_conditional_increment(volatile uint64_t *pw);
bool atomic_bool_compare_and_swap(volatile uint64_t *pw, volatile uint64_t oldval, volatile uint64_t newval);
uint64_t atomic_val_compare_and_swap(volatile uint64_t *pw, volatile uint64_t oldval, volatile uint64_t newval);

int64_t atomic_load(volatile int64_t *ptarget);
void atomic_set(volatile int64_t *ptarget, volatile int64_t pw);
int64_t atomic_add(volatile int64_t *pw, volatile int64_t val);
int64_t atomic_post_add(volatile int64_t *pw, volatile int64_t val);
int64_t atomic_sub(volatile int64_t *pw, volatile int64_t val);
int64_t atomic_post_sub(volatile int64_t *pw, volatile int64_t val);
int64_t atomic_exchange_if_greater(volatile int64_t *pw, volatile int64_t val);
int64_t atomic_conditional_increment(volatile int64_t *pw);
bool atomic_bool_compare_and_swap(volatile int64_t *pw, volatile int64_t oldval, volatile int64_t newval);
int64_t atomic_val_compare_and_swap(volatile int64_t *pw, volatile int64_t oldval, volatile int64_t newval);

void *atomic_load_ptr(volatile void **ptarget);
void atomic_set_ptr(volatile void **ptarget, volatile void *pw);
bool atomic_bool_compare_and_swap_ptr(volatile void **pw, volatile void *oldval, volatile void *newval);
void *atomic_val_compare_and_swap_ptr(volatile void **pw, volatile void *oldval, volatile void *newval);

template <class T>
class SafeNumeric {
	T _value;

public:
	_ALWAYS_INLINE_ void set(T p_value) {
		atomic_set((volatile T *)&_value, p_value);
	}

	_ALWAYS_INLINE_ T get() const {
		return atomic_load((volatile T *)&_value);
	}

	_ALWAYS_INLINE_ T increment() {
		return atomic_add((volatile T *)&_value, 1);
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postincrement() {
		return atomic_post_add((volatile T *)&_value, 1);
	}

	_ALWAYS_INLINE_ T decrement() {
		return atomic_sub((volatile T *)&_value, 1);
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postdecrement() {
		return atomic_post_sub((volatile T *)&_value, 1);
	}

	_ALWAYS_INLINE_ T add(T p_value) {
		return atomic_add((volatile T *)&_value, p_value);
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postadd(T p_value) {
		return atomic_post_add((volatile T *)&_value, p_value);
	}

	_ALWAYS_INLINE_ T sub(T p_value) {
		return atomic_sub((volatile T *)&_value, p_value);
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postsub(T p_value) {
		return atomic_post_sub((volatile T *)&_value, p_value);
	}

	_ALWAYS_INLINE_ T exchange_if_greater(T p_value) {
		return atomic_exchange_if_greater((volatile T *)&_value, p_value);
	}

	_ALWAYS_INLINE_ T conditional_increment() {
		return atomic_conditional_increment((volatile T *)&_value);
	}

	_ALWAYS_INLINE_ bool compare_exchange_weak(T &p_expected, T p_desired) {
		T old_value = p_expected;

		p_expected = atomic_val_compare_and_swap((volatile T *)&_value, p_expected, p_desired);

		return p_expected == old_value;
	}

	_ALWAYS_INLINE_ bool compare_exchange_strong(T &p_expected, T p_desired) {
		T old_value = p_expected;

		p_expected = atomic_val_compare_and_swap((volatile T *)&_value, p_expected, p_desired);

		return p_expected == old_value;
	}

	_ALWAYS_INLINE_ explicit SafeNumeric(T p_value = static_cast<T>(0)) {
		set(p_value);
	}
};

// Template for atomic pointer types
template <class T>
class SafePointer {
	T _value;

public:
	_ALWAYS_INLINE_ void set(T p_value) {
		atomic_set_ptr((void **)&_value, (void *)p_value);
	}

	_ALWAYS_INLINE_ T get() const {
		return (T)atomic_load_ptr((volatile void **)&_value);
	}

	_ALWAYS_INLINE_ bool compare_exchange_weak(T &p_expected, T p_desired) {
		T old_value = p_expected;

		p_expected = (T)atomic_val_compare_and_swap_ptr((void **)&_value, (void *)p_expected, (void *)p_desired);

		return p_expected == old_value;
	}

	_ALWAYS_INLINE_ bool compare_exchange_strong(T &p_expected, T p_desired) {
		T old_value = p_expected;

		p_expected = (T)atomic_val_compare_and_swap_ptr((void **)&_value, (void *)p_expected, (void *)p_desired);

		return p_expected == old_value;
	}

	_ALWAYS_INLINE_ explicit SafePointer(T p_value = nullptr) {
		set(p_value);
	}
};

class SafeFlag {
	uint32_t _flag;

public:
	_ALWAYS_INLINE_ bool is_set() const {
		return atomic_load((volatile uint32_t *)&_flag);
	}

	_ALWAYS_INLINE_ void set() {
		while (true) {
			uint32_t tmp = is_set() ? 1 : 0;

			if (tmp) {
				return;
			}

			if (atomic_bool_compare_and_swap((volatile uint32_t *)&_flag, tmp, 1)) {
				return;
			}
		}
	}

	_ALWAYS_INLINE_ void clear() {
		while (true) {
			uint32_t tmp = is_set() ? 1 : 0;

			if (!tmp) {
				return;
			}

			if (atomic_bool_compare_and_swap((volatile uint32_t *)&_flag, tmp, 0)) {
				return;
			}
		}
	}

	_ALWAYS_INLINE_ void set_to(bool p_value) {
		while (true) {
			uint32_t tmp = is_set() ? 1 : 0;

			if (tmp == (p_value ? 1 : 0)) {
				return;
			}

			if (atomic_bool_compare_and_swap((volatile uint32_t *)&_flag, tmp, p_value ? 1 : 0)) {
				return;
			}
		}
	}

	_ALWAYS_INLINE_ bool test_and_set() {
		uint32_t tmp = is_set() ? 1 : 0;

		if (tmp) {
			return false;
		}

		return atomic_bool_compare_and_swap((volatile uint32_t *)&_flag, tmp, 1);
	}

	_ALWAYS_INLINE_ explicit SafeFlag(bool p_value = false) {
		_flag = 0;

		set_to(p_value);
	}
};

class SafeRefCount {
	SafeNumeric<uint32_t> count;

public:
	_ALWAYS_INLINE_ bool ref() { // true on success
		return count.conditional_increment() != 0;
	}

	_ALWAYS_INLINE_ uint32_t refval() { // none-zero on success
		return count.conditional_increment();
	}

	_ALWAYS_INLINE_ bool unref() { // true if must be disposed of
		return count.decrement() == 0;
	}

	_ALWAYS_INLINE_ uint32_t unrefval() { // 0 if must be disposed of
		return count.decrement();
	}

	_ALWAYS_INLINE_ uint32_t get() const {
		return count.get();
	}

	_ALWAYS_INLINE_ void init(uint32_t p_value = 1) {
		count.set(p_value);
	}
};

#else

//no threads supported?
#error Must provide atomic functions for this platform or compiler!

#endif

#endif // SAFE_REFCOUNT_H
