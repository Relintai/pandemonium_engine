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
#include "core/typedefs.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

struct cgsem {
	int pipefd[2];
};

typedef struct cgsem cgsem_t;

class Semaphore {
public:
	_ALWAYS_INLINE_ void post() const {
		const char buf = 1;

		write(_sem->pipefd[1], &buf, 1);
	}

	_ALWAYS_INLINE_ void wait() const {
		char buf;

		read(_sem->pipefd[0], &buf, 1);
	}

	_ALWAYS_INLINE_ bool try_wait() const {
		char buf;

		read(_sem->pipefd[0], &buf, 1);

		return true;
	}

	_ALWAYS_INLINE_ int get() const {
		return 0;
	}

	Semaphore() {
		int flags, fd, i;

		pipe(_sem->pipefd);

		/* Make the pipes FD_CLOEXEC to allow them to close should we call
		 * execv on restart. */
		for (i = 0; i < 2; i++) {
			fd = _sem->pipefd[i];
			flags = fcntl(fd, F_GETFD, 0);
			flags |= FD_CLOEXEC;
			fcntl(fd, F_SETFD, flags);
		}
	}

	~Semaphore() {
		close(_sem->pipefd[1]);
		close(_sem->pipefd[0]);
	}

private:
	mutable cgsem_t _sem;
};

#endif // PLATFORM_SEMAPHORE_H
