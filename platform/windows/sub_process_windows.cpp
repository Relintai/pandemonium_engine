/*************************************************************************/
/*  dir_access_unix.cpp                                                  */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "sub_process_windows.h"

#include "core/containers/local_vector.h"

// TODO clean these up
#include <avrt.h>
#include <direct.h>
#include <knownfolders.h>
#include <process.h>
#include <regstr.h>
#include <shlobj.h>
#include <wchar.h>

Error SubProcessWindows::start() {
	if (_executable_path.empty()) {
		return ERR_FILE_BAD_PATH;
	}

	if (is_process_running()) {
		return ERR_BUSY;
	}

	String path = _executable_path.replace("/", "\\");

	String cmdline = _quote_command_line_argument(path);
	const List<String>::Element *I = _arguments.front();
	while (I) {
		cmdline += " " + _quote_command_line_argument(I->get());
		I = I->next();
	}

	ZeroMemory(&_process_info.si, sizeof(_process_info.si));
	_process_info.si.cb = sizeof(_process_info.si);
	ZeroMemory(&_process_info.pi, sizeof(_process_info.pi));
	LPSTARTUPINFOW si_w = (LPSTARTUPINFOW)&_process_info.si;

	Char16String modstr = cmdline.utf16(); // Windows wants to change this no idea why.

	bool inherit_handles = false;

	if (_read_output) {
		// Create pipe for StdOut and StdErr.
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = true;
		sa.lpSecurityDescriptor = NULL;

		ERR_FAIL_COND_V(!CreatePipe(&_pipe_handles[0], &_pipe_handles[1], &sa, 0), ERR_CANT_FORK);
		ERR_FAIL_COND_V(!SetHandleInformation(_pipe_handles[0], HANDLE_FLAG_INHERIT, 0), ERR_CANT_FORK); // Read handle is for host process only and should not be inherited.

		_process_info.si.dwFlags |= STARTF_USESTDHANDLES;
		_process_info.si.hStdOutput = _pipe_handles[1];
		if (_read_std_err) {
			_process_info.si.hStdError = _pipe_handles[1];
		}
		inherit_handles = true;
	}

	DWORD creaton_flags = NORMAL_PRIORITY_CLASS;
	if (_open_console) {
		creaton_flags |= CREATE_NEW_CONSOLE;
	} else {
		creaton_flags |= CREATE_NO_WINDOW;
	}

	int ret = CreateProcessW(nullptr, (LPWSTR)(modstr.ptrw()), nullptr, nullptr, inherit_handles, creaton_flags, nullptr, nullptr, si_w, &_process_info.pi);
	if (!ret && _read_output) {
		CloseHandle(_pipe_handles[0]); // Cleanup pipe handles.
		CloseHandle(_pipe_handles[1]);

		_pipe_handles[0] = NULL;
		_pipe_handles[1] = NULL;
	}

	ERR_FAIL_COND_V(ret == 0, ERR_CANT_FORK);

	if (_blocking) {
		if (_read_output) {
			CloseHandle(_pipe_handles[1]); // Close pipe write handle (only child process is writing).

			int bytes_in_buffer = 0;

			const int CHUNK_SIZE = 4096;
			DWORD read = 0;
			for (;;) { // Read StdOut and StdErr from pipe.
				_bytes.resize(bytes_in_buffer + CHUNK_SIZE);
				const bool success = ReadFile(_pipe_handles[0], _bytes.ptr() + bytes_in_buffer, CHUNK_SIZE, &read, NULL);
				if (!success || read == 0) {
					break;
				}
				// Assume that all possible encodings are ASCII-compatible.
				// Break at newline to allow receiving long output in portions.
				int newline_index = -1;
				for (int i = read - 1; i >= 0; i--) {
					if (_bytes[bytes_in_buffer + i] == '\n') {
						newline_index = i;
						break;
					}
				}

				if (newline_index == -1) {
					bytes_in_buffer += read;
					continue;
				}

				const int bytes_to_convert = bytes_in_buffer + (newline_index + 1);
				_append_to_pipe(_bytes.ptr(), bytes_to_convert);

				bytes_in_buffer = read - (newline_index + 1);
				memmove(_bytes.ptr(), _bytes.ptr() + bytes_to_convert, bytes_in_buffer);
			}

			if (bytes_in_buffer > 0) {
				_append_to_pipe(_bytes.ptr(), bytes_in_buffer);
			}

			CloseHandle(_pipe_handles[0]); // Close pipe read handle.
		}

		WaitForSingleObject(_process_info.pi.hProcess, INFINITE);

		DWORD ret2;
		GetExitCodeProcess(_process_info.pi.hProcess, &ret2);
		_exitcode = ret2;

		CloseHandle(_process_info.pi.hProcess);
		CloseHandle(_process_info.pi.hThread);
	} else {
		if (_read_output) {
			//eventually we will need to keep this
			CloseHandle(_pipe_handles[1]); // Close pipe write handle (only child process is writing).
			_pipe_handles[1] = NULL;
		}

		_process_started = true;

		ProcessID pid = _process_info.pi.dwProcessId;
		_process_id = pid;
	}

	return OK;
}

Error SubProcessWindows::stop() {
	if (!_process_started) {
		return OK;
	}

	if (_pipe_handles[0]) {
		CloseHandle(_pipe_handles[0]); // Cleanup pipe handles.
		_pipe_handles[0] = NULL;
	}

	if (_pipe_handles[1]) {
		CloseHandle(_pipe_handles[1]);
		_pipe_handles[1] = NULL;
	}

	const int ret = TerminateProcess(_process_info.pi.hProcess, 0);

	CloseHandle(_process_info.pi.hProcess);
	CloseHandle(_process_info.pi.hThread);

	ZeroMemory(&_process_info.si, sizeof(_process_info.si));
	_process_info.si.cb = sizeof(_process_info.si);
	ZeroMemory(&_process_info.pi, sizeof(_process_info.pi));

	_process_started = false;

	return ret != 0 ? OK : FAILED;
}

Error SubProcessWindows::poll() {
	if (!_process_started) {
		return FAILED;
	}

	if (!_pipe_handles[0]) {
		return FAILED;
	}

	_pipe.clear();

	int bytes_in_buffer = 0;

	const int CHUNK_SIZE = 4096;
	DWORD read = 0;

	_bytes.resize(bytes_in_buffer + CHUNK_SIZE);
	const bool success = ReadFile(_pipe_handles[0], _bytes.ptr() + bytes_in_buffer, CHUNK_SIZE, &read, NULL);

	if (!success) {
		stop();
		return ERR_FILE_EOF;
	}

	if (read == 0) {
		return OK;
	}

	// Assume that all possible encodings are ASCII-compatible.
	// Break at newline to allow receiving long output in portions.
	int newline_index = -1;
	for (int i = read - 1; i >= 0; i--) {
		if (_bytes[bytes_in_buffer + i] == '\n') {
			newline_index = i;
			break;
		}
	}

	if (newline_index == -1) {
		bytes_in_buffer += read;
		return OK;
	}

	const int bytes_to_convert = bytes_in_buffer + (newline_index + 1);
	_append_to_pipe(_bytes.ptr(), bytes_to_convert);

	bytes_in_buffer = read - (newline_index + 1);
	memmove(_bytes.ptr(), _bytes.ptr() + bytes_to_convert, bytes_in_buffer);

	if (bytes_in_buffer > 0) {
		_append_to_pipe(_bytes.ptr(), bytes_in_buffer);
	}

	return OK;
}

Error SubProcessWindows::send_signal(const int p_signal) {
	//Not Yet Impl
	ERR_FAIL_V(ERR_BUG);
}

Error SubProcessWindows::send_data(const String &p_data) {
	//Not Yet Impl
	ERR_FAIL_V(ERR_BUG);
}

bool SubProcessWindows::is_process_running() const {
	if (_process_id == 0) {
		return false;
	}

	if (!_process_started) {
		return false;
	}

	DWORD dw_exit_code = 0;
	if (!GetExitCodeProcess(_process_info.pi.hProcess, &dw_exit_code)) {
		return false;
	}

	if (dw_exit_code != STILL_ACTIVE) {
		return false;
	}

	return true;
}

String SubProcessWindows::_quote_command_line_argument(const String &p_text) const {
	for (int i = 0; i < p_text.size(); i++) {
		CharType c = p_text[i];
		if (c == ' ' || c == '&' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '^' || c == '=' || c == ';' || c == '!' || c == '\'' || c == '+' || c == ',' || c == '`' || c == '~') {
			return "\"" + p_text + "\"";
		}
	}
	return p_text;
}

void SubProcessWindows::_append_to_pipe(char *p_bytes, int p_size) {
	// Try to convert from default ANSI code page to Unicode.
	LocalVector<wchar_t> wchars;
	int total_wchars = MultiByteToWideChar(CP_ACP, 0, p_bytes, p_size, nullptr, 0);
	if (total_wchars > 0) {
		wchars.resize(total_wchars);
		if (MultiByteToWideChar(CP_ACP, 0, p_bytes, p_size, wchars.ptr(), total_wchars) == 0) {
			wchars.clear();
		}
	}

	if (_pipe_mutex) {
		_pipe_mutex->lock();
	}
	if (wchars.empty()) {
		// Let's hope it's compatible with UTF-8.
		_pipe += String::utf8(p_bytes, p_size);
	} else {
		_pipe += String(wchars.ptr(), total_wchars);
	}
	if (_pipe_mutex) {
		_pipe_mutex->unlock();
	}
}

SubProcessWindows::SubProcessWindows() :
		SubProcess() {

	_pipe_handles[0] = NULL;
	_pipe_handles[1] = NULL;

	_process_started = false;

	ZeroMemory(&_process_info.si, sizeof(_process_info.si));
	_process_info.si.cb = sizeof(_process_info.si);
	ZeroMemory(&_process_info.pi, sizeof(_process_info.pi));
}
SubProcessWindows::~SubProcessWindows() {
	stop();
}
