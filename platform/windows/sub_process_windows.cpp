/*************************************************************************/
/*  sub_process_windows.cpp                                              */
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

	_setup_pipe_mutex();

	if (_std_out_mutex) {
		_std_out_mutex->lock();
	}
	_std_out = String();
	if (_std_out_mutex) {
		_std_out_mutex->unlock();
	}

	if (_std_err_mutex) {
		_std_err_mutex->lock();
	}
	_std_err = String();
	if (_std_err_mutex) {
		_std_err_mutex->unlock();
	}

	String path = _executable_path.replace("/", "\\");

	String cmdline = _quote_command_line_argument(path);
	for (int i = 0; i < _arguments.size(); ++i) {
		cmdline += " " + _quote_command_line_argument(_arguments[i]);
	}

	ZeroMemory(&_process_info.si, sizeof(_process_info.si));
	_process_info.si.cb = sizeof(_process_info.si);
	ZeroMemory(&_process_info.pi, sizeof(_process_info.pi));
	LPSTARTUPINFOW si_w = (LPSTARTUPINFOW)&_process_info.si;

	Char16String modstr = cmdline.utf16(); // Windows wants to change this no idea why.

	bool inherit_handles = false;

	// Setup Pipes
	if (_communication_flags != COMMUNICATION_FLAGS_NONE) {
		if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
			// Create pipe for StdOut
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = true;
			sa.lpSecurityDescriptor = NULL;

			_process_info.si.dwFlags |= STARTF_USESTDHANDLES;

			ERR_FAIL_COND_V(!CreatePipe(&_read_std_handles[0], &_read_std_handles[1], &sa, 0), ERR_CANT_FORK);
			ERR_FAIL_COND_V(!SetHandleInformation(_read_std_handles[0], HANDLE_FLAG_INHERIT, 0), ERR_CANT_FORK); // Read handle is for host process only and should not be inherited.

			_process_info.si.hStdOutput = _read_std_handles[1];

			inherit_handles = true;
		}

		if ((_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
			// Create pipe for StdOut
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = true;
			sa.lpSecurityDescriptor = NULL;

			_process_info.si.dwFlags |= STARTF_USESTDHANDLES;

			ERR_FAIL_COND_V(!CreatePipe(&_read_std_err_handles[0], &_read_std_err_handles[1], &sa, 0), ERR_CANT_FORK);
			ERR_FAIL_COND_V(!SetHandleInformation(_read_std_err_handles[0], HANDLE_FLAG_INHERIT, 0), ERR_CANT_FORK); // Read handle is for host process only and should not be inherited.

			_process_info.si.hStdError = _read_std_err_handles[1];

			inherit_handles = true;
		}

		if ((_communication_flags & COMMUNICATION_FLAGS_STDIN) != 0) {
			// Create pipe for StdOut and StdErr.
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = true;
			sa.lpSecurityDescriptor = NULL;

			ERR_FAIL_COND_V(!CreatePipe(&_write_handles[0], &_write_handles[1], &sa, 0), ERR_CANT_FORK);
			ERR_FAIL_COND_V(!SetHandleInformation(_write_handles[0], HANDLE_FLAG_INHERIT, 0), ERR_CANT_FORK); // Read handle is for host process only and should not be inherited.

			_process_info.si.dwFlags |= STARTF_USESTDHANDLES;
			_process_info.si.hStdInput = _write_handles[1];

			inherit_handles = true;
		}
	}

	// Setup process
	DWORD creaton_flags = NORMAL_PRIORITY_CLASS;
	if (_open_console) {
		creaton_flags |= CREATE_NEW_CONSOLE;
	} else {
		creaton_flags |= CREATE_NO_WINDOW;
	}

	int ret = CreateProcessW(nullptr, (LPWSTR)(modstr.ptrw()), nullptr, nullptr, inherit_handles, creaton_flags, nullptr, nullptr, si_w, &_process_info.pi);
	if (!ret) {
		if (_communication_flags != COMMUNICATION_FLAGS_NONE) {
			// Cleanup pipe handles.
			for (int i = 0; i < 2; ++i) {
				if (_read_std_handles[i]) {
					CloseHandle(_read_std_handles[i]);
					_read_std_handles[i] = NULL;
				}

				if (_read_std_err_handles[i]) {
					CloseHandle(_read_std_err_handles[i]);
					_read_std_err_handles[i] = NULL;
				}

				if (_write_handles[i]) {
					CloseHandle(_write_handles[i]);
					_write_handles[i] = NULL;
				}
			}
		}

		return ERR_CANT_FORK;
	}

	// Close handles that were passed to the subprocess.

	if (_communication_flags != COMMUNICATION_FLAGS_NONE) {
		if (_read_std_handles[1]) {
			CloseHandle(_read_std_handles[1]);
			_read_std_handles[1] = NULL;
		}

		if (_read_std_handles[1]) {
			CloseHandle(_read_std_err_handles[1]);
			_read_std_err_handles[1] = NULL;
		}

		if (_write_handles[1]) {
			CloseHandle(_write_handles[1]);
			_write_handles[1] = NULL;
		}
	}

	if (_blocking) {
		int bytes_in_buffer = 0;
		int err_bytes_in_buffer = 0;

		const int CHUNK_SIZE = 4096;
		DWORD read = 0;
		DWORD err_read = 0;
		for (;;) { // Read StdOut and StdErr from pipe.
			// First go for stdin
			if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
				_bytes.resize(bytes_in_buffer + CHUNK_SIZE);
				const bool success = ReadFile(_read_std_handles[0], _bytes.ptr() + bytes_in_buffer, CHUNK_SIZE, &read, NULL);
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
				_append_to_std_out(_bytes.ptr(), bytes_to_convert);

				bytes_in_buffer = read - (newline_index + 1);
				memmove(_bytes.ptr(), _bytes.ptr() + bytes_to_convert, bytes_in_buffer);
			}

			// StdErr
			if ((_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
				_err_bytes.resize(err_bytes_in_buffer + CHUNK_SIZE);
				const bool success = ReadFile(_read_std_err_handles[0], _err_bytes.ptr() + err_bytes_in_buffer, CHUNK_SIZE, &err_read, NULL);
				if (!success || err_read == 0) {
					break;
				}
				// Assume that all possible encodings are ASCII-compatible.
				// Break at newline to allow receiving long output in portions.
				int newline_index = -1;
				for (int i = err_read - 1; i >= 0; i--) {
					if (_err_bytes[err_bytes_in_buffer + i] == '\n') {
						newline_index = i;
						break;
					}
				}

				if (newline_index == -1) {
					err_bytes_in_buffer += err_read;
					continue;
				}

				const int bytes_to_convert = err_bytes_in_buffer + (newline_index + 1);
				_append_to_std_err(_err_bytes.ptr(), bytes_to_convert);

				err_bytes_in_buffer = err_read - (newline_index + 1);
				memmove(_err_bytes.ptr(), _err_bytes.ptr() + bytes_to_convert, err_bytes_in_buffer);
			}

			// Note that we don't worry about stdin here, as it can only happen if a thread launches a process in blocking mode, an an another writes to it.
		}

		// StdIn
		if (bytes_in_buffer > 0) {
			_append_to_std_out(_bytes.ptr(), bytes_in_buffer);
		}

		// StdErr
		if (err_bytes_in_buffer > 0) {
			_append_to_std_err(_err_bytes.ptr(), err_bytes_in_buffer);
		}

		if (_read_std_handles[0]) {
			CloseHandle(_read_std_handles[0]);
			_read_std_handles[0] = NULL;
		}

		if (_read_std_err_handles[0]) {
			CloseHandle(_read_std_err_handles[0]);
			_read_std_err_handles[0] = NULL;
		}

		if (_write_handles[0]) {
			CloseHandle(_write_handles[0]);
			_write_handles[0] = NULL;
		}

		WaitForSingleObject(_process_info.pi.hProcess, INFINITE);

		DWORD ret2;
		GetExitCodeProcess(_process_info.pi.hProcess, &ret2);
		_exitcode = ret2;

		CloseHandle(_process_info.pi.hProcess);
		CloseHandle(_process_info.pi.hThread);
	} else {
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

	if (!_blocking) {
		// Process remaining data when doing a non-blocking call, if there any

		// StdIn
		if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
			if (_bytes.size() > 0) {
				_append_to_std_out(_bytes.ptr(), _bytes.size());
			}
		}

		// StdErr
		if ((_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
			if (_err_bytes.size() > 0) {
				_append_to_std_err(_err_bytes.ptr(), _err_bytes.size());
			}
		}
	}

	// Cleanup pipe handles.
	for (int i = 0; i < 2; ++i) {
		if (_read_std_handles[i]) {
			CloseHandle(_read_std_handles[i]);
			_read_std_handles[i] = NULL;
		}

		if (_read_std_err_handles[i]) {
			CloseHandle(_read_std_err_handles[i]);
			_read_std_err_handles[i] = NULL;
		}

		if (_write_handles[i]) {
			CloseHandle(_write_handles[i]);
			_write_handles[i] = NULL;
		}
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
		return ERR_UNAVAILABLE;
	}

	if (_blocking) {
		// This should the api more convenient to use.
		if (!is_process_running()) {
			return ERR_FILE_EOF;
		}

		// If it's blocking, and we want to read output from an another thread, we can just do it without poll
		// Just ignore poll calls
		return OK;
	}

	if (!_read_std_handles[0] && !_read_std_err_handles[0]) {
		return ERR_UNAVAILABLE;
	}

	if (_read_std_handles[0]) {
		int bytes_in_buffer = _bytes.size();

		const int CHUNK_SIZE = 4096;
		DWORD read = 0;

		_bytes.resize(bytes_in_buffer + CHUNK_SIZE);
		const bool success = ReadFile(_read_std_handles[0], _bytes.ptr() + bytes_in_buffer, CHUNK_SIZE, &read, NULL);

		if (!success) {
			// Note, stop() will process remaning bytes, we had an error, so get rid of the new chunk, as it's empty.
			_bytes.resize(bytes_in_buffer);
			stop();
			return ERR_FILE_EOF;
		}

		if (read != 0) {
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
			} else {
				const int bytes_to_convert = bytes_in_buffer + (newline_index + 1);
				_append_to_std_out(_bytes.ptr(), bytes_to_convert);

				bytes_in_buffer = read - (newline_index + 1);
				memmove(_bytes.ptr(), _bytes.ptr() + bytes_to_convert, bytes_in_buffer);
			}

			_bytes.resize(bytes_in_buffer);
		}
	}

	if (_read_std_err_handles[0]) {
		int bytes_in_buffer = _err_bytes.size();

		const int CHUNK_SIZE = 4096;
		DWORD read = 0;

		_err_bytes.resize(bytes_in_buffer + CHUNK_SIZE);
		const bool success = ReadFile(_read_std_err_handles[0], _err_bytes.ptr() + bytes_in_buffer, CHUNK_SIZE, &read, NULL);

		if (!success) {
			// Note, stop() will process remaning bytes, we had an error, so get rid of the new chunk, as it's empty.
			_err_bytes.resize(bytes_in_buffer);
			stop();
			return ERR_FILE_EOF;
		}

		if (read != 0) {
			// Assume that all possible encodings are ASCII-compatible.
			// Break at newline to allow receiving long output in portions.
			int newline_index = -1;
			for (int i = read - 1; i >= 0; i--) {
				if (_err_bytes[bytes_in_buffer + i] == '\n') {
					newline_index = i;
					break;
				}
			}

			if (newline_index == -1) {
				bytes_in_buffer += read;
			} else {
				const int bytes_to_convert = bytes_in_buffer + (newline_index + 1);
				_append_to_std_err(_err_bytes.ptr(), bytes_to_convert);

				bytes_in_buffer = read - (newline_index + 1);
				memmove(_err_bytes.ptr(), _err_bytes.ptr() + bytes_to_convert, bytes_in_buffer);
			}

			_err_bytes.resize(bytes_in_buffer);
		}
	}

	return OK;
}

Error SubProcessWindows::send_signal(const int p_signal) {
	// Signals doesn't exists on Windows.
	return ERR_UNAVAILABLE;
}

Error SubProcessWindows::write_to_stdin(const String &p_data) {
	if (!is_process_running()) {
		return ERR_UNCONFIGURED;
	}

	if (p_data.length() == 0) {
		return OK;
	}

	CharString cs = p_data.utf8();

	DWORD total_written = 0;

	if (_std_in_mutex) {
		_std_in_mutex->lock();
	}

	// Note, we are using lenght() to skip sending null terminators!
	for (;;) {
		DWORD written;
		const bool success = WriteFile(_write_handles[0], cs.get_data(), cs.length() - total_written, &written, NULL);

		if (!success) {
			if (_std_in_mutex) {
				_std_in_mutex->unlock();
			}

			stop();
			return ERR_FILE_EOF;
		}

		total_written += written;

		if (total_written >= cs.length()) {
			break;
		}
	}

	if (_std_in_mutex) {
		_std_in_mutex->unlock();
	}

	return OK;
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

void SubProcessWindows::_append_to_std_out(char *p_bytes, int p_size) {
	// Try to convert from default ANSI code page to Unicode.
	LocalVector<wchar_t> wchars;
	int total_wchars = MultiByteToWideChar(CP_ACP, 0, p_bytes, p_size, nullptr, 0);
	if (total_wchars > 0) {
		wchars.resize(total_wchars);
		if (MultiByteToWideChar(CP_ACP, 0, p_bytes, p_size, wchars.ptr(), total_wchars) == 0) {
			wchars.clear();
		}
	}

	if (_std_out_mutex) {
		_std_out_mutex->lock();
	}
	if (wchars.empty()) {
		// Let's hope it's compatible with UTF-8.
		_std_out += String::utf8(p_bytes, p_size);
	} else {
		_std_out += String(wchars.ptr(), total_wchars);
	}
	if (_std_out_mutex) {
		_std_out_mutex->unlock();
	}
}

void SubProcessWindows::_append_to_std_err(char *p_bytes, int p_size) {
	// Try to convert from default ANSI code page to Unicode.
	LocalVector<wchar_t> wchars;
	int total_wchars = MultiByteToWideChar(CP_ACP, 0, p_bytes, p_size, nullptr, 0);
	if (total_wchars > 0) {
		wchars.resize(total_wchars);
		if (MultiByteToWideChar(CP_ACP, 0, p_bytes, p_size, wchars.ptr(), total_wchars) == 0) {
			wchars.clear();
		}
	}

	if (_std_err_mutex) {
		_std_err_mutex->lock();
	}
	if (wchars.empty()) {
		// Let's hope it's compatible with UTF-8.
		_std_err += String::utf8(p_bytes, p_size);
	} else {
		_std_err += String(wchars.ptr(), total_wchars);
	}
	if (_std_err_mutex) {
		_std_err_mutex->unlock();
	}
}

SubProcessWindows::SubProcessWindows() :
		SubProcess() {
	_read_std_handles[0] = NULL;
	_read_std_handles[1] = NULL;

	_read_std_err_handles[0] = NULL;
	_read_std_err_handles[1] = NULL;

	_write_handles[0] = NULL;
	_write_handles[1] = NULL;

	_process_started = false;

	ZeroMemory(&_process_info.si, sizeof(_process_info.si));
	_process_info.si.cb = sizeof(_process_info.si);
	ZeroMemory(&_process_info.pi, sizeof(_process_info.pi));
}
SubProcessWindows::~SubProcessWindows() {
	stop();
}
