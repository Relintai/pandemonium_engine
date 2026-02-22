#ifndef PLATFORM_RWLOCK_H
#define PLATFORM_RWLOCK_H

/*************************************************************************/
/*  platform_rw_lock.h                                                   */
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

#include <pthread.h>

class RWLock {
	mutable pthread_rwlock_t rwlock;

public:
	// Lock the rwlock, block if locked by someone else
	_FORCE_INLINE_ void read_lock() const {
		int err = pthread_rwlock_rdlock(&rwlock);
		ERR_FAIL_COND(err != 0);
	}

	// Unlock the rwlock, let other threads continue
	_FORCE_INLINE_ void read_unlock() const {
		pthread_rwlock_unlock(&rwlock);
	}

	// Attempt to lock the rwlock, OK on success, ERR_BUSY means it can't lock.
	_FORCE_INLINE_ Error read_try_lock() const {
		if (pthread_rwlock_tryrdlock(&rwlock) != 0) {
			return ERR_BUSY;
		} else {
			return OK;
		}
	}

	// Lock the rwlock, block if locked by someone else
	_FORCE_INLINE_ void write_lock() {
		int err = pthread_rwlock_wrlock(&rwlock);
		ERR_FAIL_COND(err != 0);
	}

	// Unlock the rwlock, let other thwrites continue
	_FORCE_INLINE_ void write_unlock() {
		pthread_rwlock_unlock(&rwlock);
	}

	// Attempt to lock the rwlock, OK on success, ERR_BUSY means it can't lock.
	_FORCE_INLINE_ Error write_try_lock() {
		if (pthread_rwlock_trywrlock(&rwlock) != 0) {
			return ERR_BUSY;
		} else {
			return OK;
		}
	}

	RWLock() {
		//rwlock=PTHREAD_RWLOCK_INITIALIZER; fails on OSX
		pthread_rwlock_init(&rwlock, NULL);
	}

	~RWLock() {
		pthread_rwlock_destroy(&rwlock);
	}
};

class RWLockRead {
	const RWLock &lock;

public:
	RWLockRead(const RWLock &p_lock) :
			lock(p_lock) {
		lock.read_lock();
	}
	~RWLockRead() {
		lock.read_unlock();
	}
};

class RWLockWrite {
	RWLock &lock;

public:
	RWLockWrite(RWLock &p_lock) :
			lock(p_lock) {
		lock.write_lock();
	}
	~RWLockWrite() {
		lock.write_unlock();
	}
};

#endif // PLATFORM_RWLOCK_H
