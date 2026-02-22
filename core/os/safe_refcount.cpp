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

void set(T p_value);
T get() const;

T increment();
// Returns the original value instead of the new one
T postincrement();

T decrement();
// Returns the original value instead of the new one
T postdecrement();

T add(T p_value);
// Returns the original value instead of the new one
T postadd(T p_value);

T sub(T p_value);
// Returns the original value instead of the new one
T postsub(T p_value);

T exchange_if_greater(T p_value);

T conditional_increment();

bool compare_exchange_weak(T &p_expected, T p_desired);
bool compare_exchange_strong(T &p_expected, T p_desired);

bool is_set() const;
void set();

void clear();

void set_to(bool p_value);

bool test_and_set();

#endif
