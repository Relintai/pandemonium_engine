#ifndef SUB_PROCESS_H
#define SUB_PROCESS_H

/*************************************************************************/
/*  sub_process.h                                                        */
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

#include "core/containers/list.h"
#include "core/math/math_defs.h"
#include "core/os/memory.h"
#include "core/os/mutex.h"
#include "core/string/ustring.h"
#include "core/typedefs.h"
#include "core/variant/variant.h"

/**
 * Multi-Platform abstraction for running and communicating with sub processes
 */

// TODO allow setting environment variables too

class SubProcess {
public:
	typedef int64_t ProcessID;

	static SubProcess *create();

	enum SubProcessCommunicationFlags {
		COMMUNICATION_FLAGS_NONE = 0,
		COMMUNICATION_FLAGS_STDOUT = 1 << 0,
		COMMUNICATION_FLAGS_STDERR = 1 << 1,
		COMMUNICATION_FLAGS_STDIN = 1 << 2,

		COMMUNICATION_FLAGS_READ = COMMUNICATION_FLAGS_STDOUT | COMMUNICATION_FLAGS_STDERR,
		COMMUNICATION_FLAGS_WRITE = COMMUNICATION_FLAGS_STDIN,
		COMMUNICATION_FLAGS_ALL = COMMUNICATION_FLAGS_STDOUT | COMMUNICATION_FLAGS_STDERR | COMMUNICATION_FLAGS_STDIN,
	};

	String get_executable_path() const;
	void set_executable_path(const String &p_executable_path);

	Vector<String> get_arguments() const;
	void set_arguments(const Vector<String> &p_arguments);

	bool get_blocking() const;
	void set_blocking(const bool p_value);

	int get_communication_flags() const;
	void set_communication_flags(const int p_flags);

	bool get_use_pipe_mutex() const;
	void set_use_pipe_mutex(const bool p_value);

	bool get_open_console() const;
	void set_open_console(const bool p_value);

	String get_std_out();
	String get_std_err();

	int get_process_id() const {
		return _process_id;
	}

	int get_exitcode() const {
		return _exitcode;
	}

	virtual Error start() = 0;
	virtual Error stop() = 0;
	virtual Error poll() = 0;
	virtual Error send_signal(const int p_signal) = 0;

	virtual Error write_to_stdin(const String &p_data) = 0;
	virtual Error write_to_stdin_utf8(const String &p_data) = 0;
	virtual Error write_to_stdin_utf16(const String &p_data) = 0;
	virtual Error write_to_stdin_utf32(const String &p_data) = 0;
	virtual Error write_data_to_stdin(const PoolByteArray &p_data) = 0;

	virtual bool is_process_running() const = 0;

	Error run(const String &p_executable_path, const Vector<String> &p_arguments, const int p_communication_flags = COMMUNICATION_FLAGS_STDOUT, bool p_blocking = true, bool p_use_pipe_mutex = false, bool p_open_console = false);

	template <class T>
	static void make_default() {
		create_func = _create_builtin<T>;
	}

	SubProcess();
	virtual ~SubProcess() {}

protected:
	void _setup_pipe_mutex();

	String _executable_path;
	Vector<String> _arguments;

	bool _blocking;

	int _communication_flags;

	String _std_out;
	String _std_err;

	bool _use_pipe_mutex;

	Mutex *_std_out_mutex;
	Mutex *_std_err_mutex;
	Mutex *_std_in_mutex;

	bool _open_console;

	ProcessID _process_id;
	int _exitcode;

public:
	typedef SubProcess *(*CreateFunc)();

private:
	static CreateFunc create_func;
	template <class T>
	static SubProcess *_create_builtin() {
		return memnew(T);
	}
};

struct SubProcessRef {
	SubProcess *f;

	_FORCE_INLINE_ bool is_null() const { return f == nullptr; }
	_FORCE_INLINE_ bool is_valid() const { return f != nullptr; }

	_FORCE_INLINE_ operator bool() const { return f != nullptr; }
	_FORCE_INLINE_ operator SubProcess *() { return f; }

	_FORCE_INLINE_ SubProcess *operator->() {
		return f;
	}

	SubProcessRef(SubProcess *fa) { f = fa; }
	SubProcessRef(SubProcessRef &&other) {
		f = other.f;
		other.f = nullptr;
	}
	~SubProcessRef() {
		if (f) {
			memdelete(f);
		}
	}
};

#endif
