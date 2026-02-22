/*************************************************************************/
/*  mutex.cpp                                                            */
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

#include "platform_mutex.h"

#include "core/os/memory.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void Mutex::lock() const {
#ifdef WINDOWS_USE_MUTEX
	WaitForSingleObject((HANDLE)mutex, INFINITE);
#else
	EnterCriticalSection((CRITICAL_SECTION *)mutex);
#endif
}

void Mutex::unlock() const {
#ifdef WINDOWS_USE_MUTEX
	ReleaseMutex((HANDLE)mutex);
#else
	LeaveCriticalSection((CRITICAL_SECTION *)mutex);
#endif
}

Error Mutex::try_lock() const {
#ifdef WINDOWS_USE_MUTEX
	return (WaitForSingleObject((HANDLE)mutex, 0) == WAIT_TIMEOUT) ? ERR_BUSY : OK;
#else

	if (TryEnterCriticalSection((CRITICAL_SECTION *)mutex)) {
		return OK;
	} else {
		return ERR_BUSY;
	}
#endif
}

Mutex::Mutex() {
#ifdef WINDOWS_USE_MUTEX
	mutex = CreateMutex(NULL, FALSE, NULL);
#else

	mutex = memnew(CRITICAL_SECTION);

#ifdef UWP_ENABLED
	InitializeCriticalSectionEx((CRITICAL_SECTION *)mutex, 0, 0);
#else
	InitializeCriticalSection((CRITICAL_SECTION *)mutex);
#endif
#endif
}

Mutex::~Mutex() {
#ifdef WINDOWS_USE_MUTEX
	CloseHandle((HANDLE)mutex);
#else

	DeleteCriticalSection((CRITICAL_SECTION *)mutex);

	memdelete((CRITICAL_SECTION *)mutex);
#endif
}

void BinaryMutex::lock() const {
#ifdef WINDOWS_USE_MUTEX
	WaitForSingleObject((HANDLE)mutex, INFINITE);
#else
	EnterCriticalSection((CRITICAL_SECTION *)mutex);
#endif
}

void BinaryMutex::unlock() const {
#ifdef WINDOWS_USE_MUTEX
	ReleaseMutex((HANDLE)mutex);
#else
	LeaveCriticalSection((CRITICAL_SECTION *)mutex);
#endif
}

Error BinaryMutex::try_lock() const {
#ifdef WINDOWS_USE_MUTEX
	return (WaitForSingleObject((HANDLE)mutex, 0) == WAIT_TIMEOUT) ? ERR_BUSY : OK;
#else

	if (TryEnterCriticalSection((CRITICAL_SECTION *)mutex)) {
		return OK;
	} else {
		return ERR_BUSY;
	}
#endif
}

BinaryMutex::BinaryMutex() {
#ifdef WINDOWS_USE_MUTEX
	mutex = CreateMutex(NULL, FALSE, NULL);
#else

	mutex = memnew(CRITICAL_SECTION);

#ifdef UWP_ENABLED
	InitializeCriticalSectionEx((CRITICAL_SECTION *)mutex, 0, 0);
#else
	InitializeCriticalSection((CRITICAL_SECTION *)mutex);
#endif
#endif
}

BinaryMutex::~BinaryMutex() {
#ifdef WINDOWS_USE_MUTEX
	CloseHandle((HANDLE)mutex);
#else

	DeleteCriticalSection((CRITICAL_SECTION *)mutex);

	memdelete((CRITICAL_SECTION *)mutex);
#endif
}
