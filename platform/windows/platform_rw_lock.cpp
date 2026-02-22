/*************************************************************************/
/*  paltform_rw_lock.cpp                                                 */
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

#include "platform_rw_lock.h"

#include "core/os/memory.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Lock the rwlock, block if locked by someone else
void RWLock::read_lock() const {
	AcquireSRWLockShared((SRWLOCK *)rwlock);
}

// Unlock the rwlock, let other threads continue
void RWLock::read_unlock() const {
	ReleaseSRWLockShared((SRWLOCK *)rwlock);
}

// Attempt to lock the rwlock, OK on success, ERR_BUSY means it can't lock.
Error RWLock::read_try_lock() const {
	if (TryAcquireSRWLockShared((SRWLOCK *)rwlock) == 0) {
		return ERR_BUSY;
	} else {
		return OK;
	}
}

// Lock the rwlock, block if locked by someone else
void RWLock::write_lock() {
	AcquireSRWLockExclusive((SRWLOCK *)rwlock);
}

// Unlock the rwlock, let other thwrites continue
void RWLock::write_unlock() {
	ReleaseSRWLockExclusive((SRWLOCK *)rwlock);
}

// Attempt to lock the rwlock, OK on success, ERR_BUSY means it can't lock.
Error RWLock::write_try_lock() {
	if (TryAcquireSRWLockExclusive((SRWLOCK *)rwlock) == 0) {
		return ERR_BUSY;
	} else {
		return OK;
	}
}

RWLock::RWLock() {
	rwlock = memnew(SRWLOCK);
	InitializeSRWLock((SRWLOCK *)rwlock);
}

RWLock::~RWLock() {
	memdelete((SRWLOCK *)rwlock);
}
