#ifndef SUB_PROCESS_WINDOWS_H
#define SUB_PROCESS_WINDOWS_H

/*************************************************************************/
/*  sub_process_windows.h                                                */
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

#include "core/os/sub_process.h"

#include "os_windows.h"

class SubProcessWindows : public SubProcess {
public:
	virtual Error start();
	virtual Error stop();
	virtual Error poll();
	virtual Error send_signal(const int p_signal);
	virtual Error write_to_stdin(const String &p_data);
	virtual bool is_process_running() const;

	SubProcessWindows();
	~SubProcessWindows();

protected:
	bool _read_from_std_out(int &bytes_in_buffer);
	bool _read_from_std_err(int &err_bytes_in_buffer);

	String _quote_command_line_argument(const String &p_text) const;
	void _append_to_std_out(char *p_bytes, int p_size);
	void _append_to_std_err(char *p_bytes, int p_size);

	struct ProcessInfo {
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
	};

	bool _process_started;

	// Pipes are unidirectional, [0] is the read end, [1] is the write end
	HANDLE _read_std_handles[2];
	HANDLE _read_std_err_handles[2];
	HANDLE _write_handles[2];

	ProcessInfo _process_info;
	LocalVector<char> _bytes;
	LocalVector<char> _err_bytes;
};

#endif
