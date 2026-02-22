#ifndef PLATFORM_SEMAPHORE_H
#define PLATFORM_SEMAPHORE_H

/*************************************************************************/
/*  paltform_semaphore.h                                                 */
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

#include "core/error/error_list.h"
#include "core/error/error_macros.h"
#include "core/os/safe_refcount.h"
#include "core/typedefs.h"

#include <dispatch/dispatch.h>
#include <errno.h>

class Semaphore {
public:
	_ALWAYS_INLINE_ void post() const {
		_count.increment();
		dispatch_semaphore_signal(_sem);
	}

	_ALWAYS_INLINE_ void wait() const {
		dispatch_semaphore_wait(_sem, DISPATCH_TIME_FOREVER);
		_count.decrement();
	}

	_ALWAYS_INLINE_ bool try_wait() const {
		//Returns zero on success, or non-zero if the timeout occurred.
		if (dispatch_semaphore_wait(_sem, DISPATCH_TIME_NOW) == 0) {
			_count.decrement();
			return true;
		}

		return false;
	}

	_ALWAYS_INLINE_ int get() const {
		return _count.get();
	}

	Semaphore() {
		// Passing zero for the value is useful for when two threads need to reconcile
		// the completion of a particular event. Passing a value greater than zero is
		// useful for managing a finite pool of resources, where the pool size is equal
		// to the value.
		_sem = dispatch_semaphore_create(0);
	}

	~Semaphore() {
	}

private:
	mutable dispatch_semaphore_t _sem;
	mutable SafeNumeric<uint32_t> _count;
};

#endif // PLATFORM_SEMAPHORE_H
