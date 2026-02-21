/*************************************************************************/
/*  thread_posix.h                                                       */
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

#ifndef THREAD_WINDOWS_H
#define THREAD_WINDOWS_H

#include "core/os/safe_refcount.h"
#include "core/typedefs.h"

class String;

class Thread {
public:
	typedef void (*Callback)(void *p_userdata);

	typedef uint64_t ID;

	enum Priority {
		PRIORITY_LOW,
		PRIORITY_NORMAL,
		PRIORITY_HIGH
	};

	struct Settings {
		Priority priority;
		Settings() { priority = PRIORITY_NORMAL; }
	};

	_FORCE_INLINE_ ID get_id() const { return _id; }

	// get the ID of the caller thread
	static ID get_caller_id();

	// get the ID of the main thread
	_FORCE_INLINE_ static ID get_main_id() { return _main_thread_id; }

	_FORCE_INLINE_ static bool is_main_thread() { return get_caller_id() == _main_thread_id; }

	static Error set_name(const String &p_name);

	void start(Thread::Callback p_callback, void *p_user, const Settings &p_settings = Settings());

	bool is_started() const;

	///< waits until thread is finished, and deallocates it.
	void wait_to_finish();

	Thread();
	~Thread();

private:
	friend class Main;

	static void setup_main_thread_id(ID p_main_thread_id);

	// Thread callback in only in the cpp file.

	static ID _main_thread_id;

	ID _id;

	// HANDLE. According to the internets just casting this to a void* should work.
	// Don't want to include windows.h here.
	void *_handle;

	Callback _callback;
	void *_user;

	Settings _settings;
};

#endif
