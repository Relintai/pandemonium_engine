#ifndef PLATFORM_MUTEX_H
#define PLATFORM_MUTEX_H

/*************************************************************************/
/*  mutex.h                                                              */
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
#include "core/typedefs.h"

#include <pthread.h>

class Mutex {
	friend class MutexLock;

	mutable pthread_mutexattr_t attr;
	mutable pthread_mutex_t mutex;

public:
	_ALWAYS_INLINE_ void lock() const {
		pthread_mutex_lock(&mutex);
	}

	_ALWAYS_INLINE_ void unlock() const {
		pthread_mutex_unlock(&mutex);
	}

	_ALWAYS_INLINE_ Error try_lock() const {
		return (pthread_mutex_trylock(&mutex) == 0) ? OK : ERR_BUSY;
	}

	Mutex() {
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&mutex, &attr);
	}

	~Mutex() {
		pthread_mutex_destroy(&mutex);
	}
};

class BinaryMutex {
	friend class MutexLock;

	mutable pthread_mutexattr_t attr;
	mutable pthread_mutex_t mutex;

public:
	_ALWAYS_INLINE_ void lock() const {
		pthread_mutex_lock(&mutex);
	}

	_ALWAYS_INLINE_ void unlock() const {
		pthread_mutex_unlock(&mutex);
	}

	_ALWAYS_INLINE_ Error try_lock() const {
		return (pthread_mutex_trylock(&mutex) == 0) ? OK : ERR_BUSY;
	}

	BinaryMutex() {
		pthread_mutexattr_init(&attr);
		pthread_mutex_init(&mutex, &attr);
	}

	~BinaryMutex() {
		pthread_mutex_destroy(&mutex);
	}
};

// This is written this way instead of being a template to overcome a limitation of C++ pre-17
// that would require MutexLock to be used like this: MutexLock<Mutex> lock;
class MutexLock {
	union {
		Mutex *recursive_mutex;
		BinaryMutex *mutex;
	};
	bool recursive;

public:
	_ALWAYS_INLINE_ explicit MutexLock(const Mutex &p_mutex) {
		recursive_mutex = const_cast<Mutex *>(&p_mutex);
		recursive = true;

		recursive_mutex->lock();
	}
	_ALWAYS_INLINE_ explicit MutexLock(const BinaryMutex &p_mutex) {
		mutex = const_cast<BinaryMutex *>(&p_mutex);
		recursive = false;
		mutex->lock();
	}

	_ALWAYS_INLINE_ ~MutexLock() {
		if (recursive) {
			recursive_mutex->unlock();
		} else {
			mutex->unlock();
		}
	}
};

#endif // PLATFORM_MUTEX_H
