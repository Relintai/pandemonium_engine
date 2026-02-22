/*************************************************************************/
/*  platform_semaphor.cpp                                                */
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

#include "platform_semaphore.h"

#include "core/os/memory.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void Semaphore::post() const {
	ReleaseSemaphore((HANDLE)_semaphore, 1, NULL);
}

void Semaphore::wait() const {
	WaitForSingleObjectEx((HANDLE)_semaphore, INFINITE, false);
}

bool Semaphore::try_wait() const {
	if (WaitForSingleObjectEx((HANDLE)_semaphore, 0, false) == WAIT_TIMEOUT) {
		return false;
	}

	return true;
}

int Semaphore::get() const {
	long previous;
	switch (WaitForSingleObjectEx((HANDLE)_semaphore, 0, false)) {
		case WAIT_OBJECT_0: {
			ERR_FAIL_COND_V(!ReleaseSemaphore((HANDLE)_semaphore, 1, &previous), -1);
			return previous + 1;
		} break;
		case WAIT_TIMEOUT: {
			return 0;
		} break;
		default: {
		}
	}

	ERR_FAIL_V(-1);
}

Semaphore::Semaphore() {
#ifdef UWP_ENABLED
	_semaphore = (void *)CreateSemaphoreEx(
			NULL,
			0,
			0xFFFFFFF, //wathever
			NULL,
			0,
			SEMAPHORE_ALL_ACCESS);
#else
	_semaphore = (void *)CreateSemaphore(
			NULL,
			0,
			0xFFFFFFF, //wathever
			NULL);
#endif
}

Semaphore::~Semaphore() {
	CloseHandle((HANDLE)_semaphore);
}
