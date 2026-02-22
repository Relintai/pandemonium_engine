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
#include "core/typedefs.h"
#include "core/error/error_macros.h"

#include <errno.h>
#include <semaphore.h>

class Semaphore {
public:
	_ALWAYS_INLINE_ void post() const {
		sem_post(&_sem);
	}

	_ALWAYS_INLINE_ void wait() const {
		//return 0 on success; on error, the value of the semaphore is left unchanged, -1 is returned, and errno is set to indicate the error.
		while (sem_wait(&_sem)) {
			if (errno == EINTR) {
				//he call was interrupted by a signal handler;
				errno = 0;
				continue;
			} else {
				return;
			}
		}
	}

	_ALWAYS_INLINE_ bool try_wait() const {
		while (sem_trywait(&_sem)) {
			if (errno == EINTR) {
				errno = 0;
				continue;
			} else if (errno == EAGAIN) {
				return false;
			} else {
				return false;
			}
		}

		return true;
	}

	_ALWAYS_INLINE_ int get() const {
		int val;
		sem_getvalue(&_sem, &val);

		return val;
	}

	Semaphore() {
		int r = sem_init(&_sem, 0, 0);

		ERR_FAIL_COND(r != 0);
	}

	~Semaphore() {
		sem_destroy(&_sem);
	}

private:
	mutable sem_t _sem;
};

#endif // PLATFORM_SEMAPHORE_H
