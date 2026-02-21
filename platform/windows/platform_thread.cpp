/*************************************************************************/
/*  thread_posix.cpp                                                     */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "platform_thread.h"

// Is there any upsides to hashing thread ids at all?
// If the issue was that threads increase by one, it's still doesn't makes sense,
// as it's trivial to figure out which int casued a given hash...
//ID id = _thread_id_hash(std::thread::id());

#include "core/object/script_language.h"
#include "core/os/memory.h"
#include "core/os/safe_refcount.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

Thread::ID Thread::_main_thread_id = 0;
static thread_local Thread::ID _caller_id = 0;
static thread_local bool _caller_id_cached = false;

static DWORD WINAPI thread_callback(LPVOID userdata) {
	Thread *t = reinterpret_cast<Thread *>(userdata);

	t->_id = (ID)GetCurrentThreadId(); // must implement

	_caller_id = t->_id;
	_caller_id_cached = true;

	if (t->_settings.priority == Thread::PRIORITY_LOW) {
		SetThreadPriority((HANDLE)t->_handle, THREAD_PRIORITY_LOWEST);
	} else if (t->_settings.priority == Thread::PRIORITY_HIGH) {
		SetThreadPriority((HANDLE)t->_handle, THREAD_PRIORITY_HIGHEST);
	}

	ScriptServer::thread_enter(); //scripts may need to attach a stack

	t->_callback(t->_user);
	SetEvent(t->_handle);

	ScriptServer::thread_exit();

	return 0;
}

Thread::ID Thread::get_caller_id() {
	if (likely(_caller_id_cached)) {
		return _caller_id;
	} else {
		_caller_id = (ID)GetCurrentThreadId();
		_caller_id_cached = true;

		return new_id;
	}
}

Error Thread::set_name(const String &p_name) {
	return ERR_UNAVAILABLE;
}

void Thread::start(Thread::Callback p_callback, void *p_user, const Settings &p_settings) {
	ERR_FAIL_COND_MSG(_handle != NULL, "A Thread cannot be started without wait_to_finish() having been called on it. Please do so to ensure correct cleanup of the thread.");

	_settings = p_settings;
	_callback = p_callback;
	_user = p_user;
	_handle = CreateEvent(NULL, TRUE, FALSE, NULL);

	QueueUserWorkItem(thread_callback, this, WT_EXECUTELONGFUNCTION);
}

bool Thread::is_started() const {
	return _id != 0;
}

///< waits until thread is finished, and deallocates it.
void Thread::wait_to_finish() {
	if (_handle == NULL) {
		return;
	}

	ERR_FAIL_COND_MSG(_id == get_caller_id(), "A Thread can't wait for itself to finish.");

	WaitForSingleObject((HANDLE)_handle, INFINITE);
	CloseHandle((HANDLE)_handle);

	_handle = 0;
	_id = 0;
}

Thread::Thread() {
	_handle = NULL;
	_id = 0;
	_callback = NULL;
	_user = NULL;
}

Thread::~Thread() {
	if (_handle != NULL) {
#ifdef DEBUG_ENABLED
		WARN_PRINT("A Thread object has been destroyed without wait_to_finish() having been called on it. Please do so to ensure correct cleanup of the thread.");
#endif

		CloseHandle((HANDLE)_handle);
	}
}

void Thread::setup_main_thread_id(Thread::ID p_main_thread_id) {
	_main_thread_id = p_main_thread_id;

	_caller_id = _main_thread_id;
	_caller_id_cached = true;
}
