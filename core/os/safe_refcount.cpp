/*************************************************************************/
/*  safe_refcount.cpp                                                    */
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

#if !defined(NO_THREADS) && defined(_MSC_VER)

#include "safe_refcount.h"
#include "core/error/error_macros.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define ATOMIC_CONDITIONAL_INCREMENT_BODY(m_pw, m_win_type, m_win_cmpxchg, m_cpp_type) \
	/* try to increment until it actually works */                                     \
	/* taken from boost */                                                             \
	while (true) {                                                                     \
		m_cpp_type tmp = static_cast<m_cpp_type const volatile &>(*(m_pw));            \
		if (tmp == 0)                                                                  \
			return 0; /* if zero, can't add to it anymore */                           \
		if (m_win_cmpxchg((m_win_type volatile *)(m_pw), tmp + 1, tmp) == tmp)         \
			return tmp + 1;                                                            \
	}

#define ATOMIC_EXCHANGE_IF_GREATER_BODY(m_pw, m_val, m_win_type, m_win_cmpxchg, m_cpp_type) \
	while (true) {                                                                          \
		m_cpp_type tmp = static_cast<m_cpp_type const volatile &>(*(m_pw));                 \
		if (tmp >= m_val)                                                                   \
			return tmp; /* already greater, or equal */                                     \
		if (m_win_cmpxchg((m_win_type volatile *)(m_pw), m_val, tmp) == tmp)                \
			return m_val;                                                                   \
	}

void atomic_set(volatile uint32_t *ptarget, volatile uint32_t pw) {
	InterlockedExchange((LONG volatile *)ptarget, pw);
}
uint32_t atomic_add(volatile uint32_t *pw, volatile uint32_t val) {
	return InterlockedAdd((LONG volatile *)pw, val) + val;
}
uint32_t atomic_post_add(volatile uint32_t *pw, volatile uint32_t val) {
	return InterlockedAdd((LONG volatile *)pw, val);
}
uint32_t atomic_sub(volatile uint32_t *pw, volatile uint32_t val) {
	return InterlockedExchangeAdd((LONG volatile *)pw, -(int32_t)val) - val;
}
uint32_t atomic_post_sub(volatile uint32_t *pw, volatile uint32_t val) {
	return InterlockedExchangeAdd((LONG volatile *)pw, -(int32_t)val);
}
uint32_t atomic_exchange_if_greater(volatile uint32_t *pw, volatile uint32_t val){
	ATOMIC_EXCHANGE_IF_GREATER_BODY(pw, val, LONG, InterlockedCompareExchange, uint32_t)
} uint32_t atomic_conditional_increment(volatile uint32_t *pw) {
	ATOMIC_CONDITIONAL_INCREMENT_BODY(pw, LONG, InterlockedCompareExchange, uint32_t)
}
bool atomic_bool_compare_and_swap(volatile uint32_t *pw, volatile uint32_t oldval, volatile uint32_t newval) {
	uint32_t val = InterlockedExchange((LONG volatile *)pw, oldval, newval);

	return val == oldval;
}
uint32_t atomic_val_compare_and_swap(volatile uint32_t *pw, volatile uint32_t oldval, volatile uint32_t newval) {
	return InterlockedExchange((LONG volatile *)pw, oldval, newval);
}

void atomic_set(volatile int32_t *ptarget, volatile int32_t pw) {
	InterlockedExchange((LONG volatile *)ptarget, pw);
}
int32_t atomic_add(volatile int32_t *pw, volatile int32_t val) {
	return InterlockedAdd((LONG volatile *)pw, val) + val;
}
int32_t atomic_post_add(volatile int32_t *pw, volatile int32_t val) {
	return InterlockedAdd((LONG volatile *)pw, val);
}
int32_t atomic_sub(volatile int32_t *pw, volatile int32_t val) {
	return InterlockedExchangeAdd((LONG volatile *)pw, -(int32_t)val) - val;
}
int32_t atomic_post_sub(volatile int32_t *pw, volatile int32_t val) {
	return InterlockedExchangeAdd((LONG volatile *)pw, -(int32_t)val);
}
int32_t atomic_exchange_if_greater(volatile int32_t *pw, volatile int32_t val){
	ATOMIC_EXCHANGE_IF_GREATER_BODY(pw, val, LONG, InterlockedCompareExchange, int32_t)
} int32_t atomic_conditional_increment(volatile int32_t *pw) {
	ATOMIC_CONDITIONAL_INCREMENT_BODY(pw, LONG, InterlockedCompareExchange, int32_t)
}
bool atomic_bool_compare_and_swap(volatile int32_t *pw, volatile int32_t oldval, volatile int32_t newval) {
	int32_t val = InterlockedExchange((LONG volatile *)pw, oldval, newval);

	return val == oldval;
}
int32_t atomic_val_compare_and_swap(volatile int32_t *pw, volatile int32_t oldval, volatile int32_t newval) {
	return InterlockedExchange((LONG volatile *)pw, oldval, newval);
}

void atomic_set(volatile uint64_t *ptarget, volatile uint64_t pw) {
	InterlockedExchange64((LONGLONG volatile *)ptarget, pw);
}
uint64_t atomic_add(volatile uint64_t *pw, volatile uint64_t val) {
	return InterlockedAdd64((LONGLONG volatile *)pw, val) + val;
}
uint64_t atomic_post_add(volatile uint64_t *pw, volatile uint64_t val) {
	return InterlockedAdd64((LONGLONG volatile *)pw, val);
}
uint64_t atomic_sub(volatile uint64_t *pw, volatile uint64_t val) {
	return InterlockedExchangeAdd64((LONGLONG volatile *)pw, -(int64_t)val) - val;
}
uint64_t atomic_post_sub(volatile uint64_t *pw, volatile uint64_t val) {
	return InterlockedExchangeAdd64((LONGLONG volatile *)pw, -(int64_t)val);
}
uint64_t atomic_exchange_if_greater(volatile uint64_t *pw, volatile uint64_t val){
	ATOMIC_EXCHANGE_IF_GREATER_BODY(pw, val, LONGLONG, InterlockedCompareExchange64, uint64_t)
} uint64_t atomic_conditional_increment(volatile uint64_t *pw) {
	ATOMIC_CONDITIONAL_INCREMENT_BODY(pw, LONGLONG, InterlockedCompareExchange64, uint64_t)
}
bool atomic_bool_compare_and_swap(volatile uint64_t *pw, volatile uint64_t oldval, volatile uint64_t newval) {
	uint64_t val = InterlockedExchange((LONGLONG volatile *)pw, oldval, newval);

	return val == oldval;
}
uint64_t atomic_val_compare_and_swap(volatile uint64_t *pw, volatile uint64_t oldval, volatile uint64_t newval) {
	return InterlockedExchange((LONGLONG volatile *)pw, oldval, newval);
}

void atomic_set(volatile int64_t *ptarget, volatile int64_t pw) {
	InterlockedExchange64((LONGLONG volatile *)ptarget, pw);
}
int64_t atomic_add(volatile int64_t *pw, volatile int64_t val) {
	return InterlockedAdd64((LONGLONG volatile *)pw, val) + val;
}
int64_t atomic_post_add(volatile int64_t *pw, volatile int64_t val) {
	return InterlockedAdd64((LONGLONG volatile *)pw, val);
}
int64_t atomic_sub(volatile int64_t *pw, volatile int64_t val) {
	return InterlockedExchangeAdd64((LONGLONG volatile *)pw, -val) - val;
}
int64_t atomic_post_sub(volatile int64_t *pw, volatile int64_t val) {
	return InterlockedExchangeAdd64((LONGLONG volatile *)pw, -val);
}
int64_t atomic_exchange_if_greater(volatile int64_t *pw, volatile int64_t val){
	ATOMIC_EXCHANGE_IF_GREATER_BODY(pw, val, LONGLONG, InterlockedCompareExchange64, int64_t)
} int64_t atomic_conditional_increment(volatile int64_t *pw) {
	ATOMIC_CONDITIONAL_INCREMENT_BODY(pw, LONGLONG, InterlockedCompareExchange64, int64_t)
}
bool atomic_bool_compare_and_swap(volatile int64_t *pw, volatile int64_t oldval, volatile int64_t newval) {
	int64_t val = InterlockedExchange((LONGLONG volatile *)pw, oldval, newval);

	return val == oldval;
}
int64_t atomic_val_compare_and_swap(volatile int64_t *pw, volatile int64_t oldval, volatile int64_t newval) {
	return InterlockedExchange((LONGLONG volatile *)pw, oldval, newval);
}

void atomic_set_ptr(volatile void **ptarget, volatile void *pw) {
	InterlockedExchangePointer((PVOID volatile *)ptarget, pw);
}
bool atomic_bool_compare_and_swap_ptr(volatile void **pw, volatile void *oldval, volatile void *newval) {
	void *val = InterlockedExchangePointer((PVOID volatile *)pw, oldval, newval);

	return val == oldval;
}
void *atomic_val_compare_and_swap_ptr(volatile void **pw, volatile void *oldval, volatile void *newval) {
	return InterlockedExchangePointer((PVOID volatile *)pw, oldval, newval);
}

#endif
