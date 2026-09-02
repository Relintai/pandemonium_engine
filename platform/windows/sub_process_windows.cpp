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

	_bytes.clear();
	_err_bytes.clear();

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
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = true;
		sa.lpSecurityDescriptor = NULL;

		_process_info.si.dwFlags |= STARTF_USESTDHANDLES;

		if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
			// Create pipe for StdOut

			ERR_FAIL_COND_V(!CreatePipe(&_read_std_handles[0], &_read_std_handles[1], &sa, 0), ERR_CANT_FORK);
			ERR_FAIL_COND_V(!SetHandleInformation(_read_std_handles[0], HANDLE_FLAG_INHERIT, 0), ERR_CANT_FORK); // Read handle is for host process only and should not be inherited.

			_process_info.si.hStdOutput = _read_std_handles[1];

			inherit_handles = true;
		}

		if ((_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
			// Create pipe for StdOut

			ERR_FAIL_COND_V(!CreatePipe(&_read_std_err_handles[0], &_read_std_err_handles[1], &sa, 0), ERR_CANT_FORK);
			ERR_FAIL_COND_V(!SetHandleInformation(_read_std_err_handles[0], HANDLE_FLAG_INHERIT, 0), ERR_CANT_FORK); // Read handle is for host process only and should not be inherited.

			_process_info.si.hStdError = _read_std_err_handles[1];

			inherit_handles = true;
		}

		if ((_communication_flags & COMMUNICATION_FLAGS_STDIN) != 0) {
			// Create pipe for StdOut and StdErr.

			ERR_FAIL_COND_V(!CreatePipe(&_write_handles[0], &_write_handles[1], &sa, 0), ERR_CANT_FORK);
			ERR_FAIL_COND_V(!SetHandleInformation(_write_handles[1], HANDLE_FLAG_INHERIT, 0), ERR_CANT_FORK); // Write handle is for host process only and should not be inherited.

			_process_info.si.hStdInput = _write_handles[0];

			inherit_handles = true;
		}
	}

	// Setup process
	DWORD creation_flags = NORMAL_PRIORITY_CLASS;
	if (_open_console) {
		creation_flags |= CREATE_NEW_CONSOLE;
	} else {
		creation_flags |= CREATE_NO_WINDOW;
	}

	// Setup environment vars

	// This is a series of null terminated strings with a double null at the end
	// in key=value format. You can't have = in the name
	// MSDN:
	// All strings in the environment block must be sorted alphabetically by name.
	// The sort is case-insensitive, Unicode order, without regard to locale.
	// Because the equal sign is a separator, it must not be used in the name of an environment variable.
	wchar_t *current_env = NULL;

	if (_inherit_environment && _environment_variables.size() == 0) {
		// No need to do anything, just keep current_env as NULL.
	} else {
		HashMap<String, String> new_env_map;

		if (_inherit_environment) {
			wchar_t *initial_env = GetEnvironmentStringsW();

			if (initial_env) {
				wchar_t *temp_env = initial_env;

				while (*temp_env != L'\0') {
					String p = String::utf16((const char16_t *)temp_env);

					temp_env += p.size();

					if (p.length() > 0) {
						int indx = p.find("=");

						if (indx != -1) {
							String key = p.substr_index(0, indx);
							String value = p.substr_index(indx + 1, p.length()); // +1 to skip =

							new_env_map[key] = value;
						}
					}

					FreeEnvironmentStringsW(initial_env);
				}
			}
		}

		for (const HashMap<StringName, String>::Element *E = _environment_variables.front(); E; E = E->next) {
			new_env_map[E->key()] = E->value();
		}

		// Unless MSDN is lying we need to write them in a sorted order.

		PoolStringArray psa;

		// Also let's count lengths for later
		int length_count = 0;

		for (const HashMap<String, String>::Element *E = new_env_map.front(); E; E = E->next) {
			String sk = E->key();
			psa.push_back(sk);
			length_count += sk.length();
			length_count += E->value().length();
		}

		if (length_count > 0) {
			// Hopefully is enough
			psa.sort();

			// We only just need to write out everything
			// All chars + each entry has an = and \0. and closing \0
			current_env = memnew_arr(wchar_t, length_count + psa.size() * 2 + 1);

			// MSDN: Note that an ANSI environment block is terminated by two zero bytes: one for the last string,
			// one more to terminate the block. A Unicode environment block is terminated by four zero bytes:
			// two for the last string, two more to terminate the block.
			// So if this is not set, only the first env var will be used.
			creation_flags |= CREATE_UNICODE_ENVIRONMENT;

			// Write data

			uint64_t current_offset = 0;
			int psa_size = psa.size();
			PoolStringArray::Read r = psa.read();
			for (int i = 0; i < psa_size; ++i) {
				String key = r[i];

				// Write key
				{
					Char16String cs = key.utf16();
					// Length does not includes a \0
					uint64_t str_byte_length = cs.length() * sizeof(char16_t);

					memcpy(&current_env[current_offset], (const char *)cs.get_data(), str_byte_length);
					current_offset += cs.length();
				}

				// Add =
				current_env[current_offset] = L'=';
				++current_offset;

				// Write value
				{
					String value = new_env_map[key];
					Char16String cs = value.utf16();
					// Size includes a \0
					uint64_t str_byte_length = cs.size() * sizeof(char16_t);

					memcpy(&current_env[current_offset], (const char *)cs.get_data(), str_byte_length);
					current_offset += cs.size();
				}
			}

			current_env[current_offset] = L'\0';
		}
	}

	Char16String app_name = _application_name.utf16();

	int ret = CreateProcessW(app_name.get_data(), (LPWSTR)(modstr.ptrw()), nullptr, nullptr, inherit_handles, creation_flags, current_env, nullptr, si_w, &_process_info.pi);

	if (current_env) {
		memdelete_arr(current_env);
		current_env = NULL;
	}

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

		if (_read_std_err_handles[1]) {
			CloseHandle(_read_std_err_handles[1]);
			_read_std_err_handles[1] = NULL;
		}

		if (_write_handles[0]) {
			CloseHandle(_write_handles[0]);
			_write_handles[0] = NULL;
		}
	}

	if (_blocking) {
		int bytes_in_buffer = 0;
		int err_bytes_in_buffer = 0;

		if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0 ||
				(_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
			for (;;) { // Read StdOut and StdErr from pipe.
				bool had_error = false;

				// First go for stdin
				if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
					if (_read_from_std_out(bytes_in_buffer)) {
						had_error = true;
					}
				}

				// StdErr
				if ((_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
					// We want to read even if stdin errored!
					if (_read_from_std_err(err_bytes_in_buffer)) {
						had_error = true;
					}
				}

				// Note that we don't worry about stdin here, as it can only happen if a thread launches a process in blocking mode, an an another writes to it.

				// This is needed to detect if the subprocess have terminated. even if the stdout and stderr is not connected.
				if (had_error) {
					break;
				}
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

		} else {
			WaitForSingleObject(_process_info.pi.hProcess, INFINITE);
		}

		if (_write_handles[1]) {
			CloseHandle(_write_handles[1]);
			_write_handles[1] = NULL;
		}

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
		// If it's blocking, and we want to read output from an another thread, we can just do it without poll
		// Just ignore poll calls

		// This should the api more convenient to use.
		if (!is_process_running()) {
			return ERR_FILE_EOF;
		}

		return OK;
	}

	if (!_read_std_handles[0] && !_read_std_err_handles[0]) {
		return ERR_UNAVAILABLE;
	}

	bool had_error = false;

	if (_read_std_handles[0]) {
		if (_poll_read_from_std_out()) {
			had_error = true;
		}
	}

	if (_read_std_err_handles[0]) {
		if (_poll_read_from_std_err()) {
			had_error = true;
		}
	}

	// This should the api more convenient to use.
	if (had_error) {
		stop();
		return ERR_FILE_EOF;
	}

	return OK;
}

Error SubProcessWindows::send_signal(const int p_signal) {
	// Signals doesn't exists on Windows.
	return ERR_UNAVAILABLE;
}

Error SubProcessWindows::write_to_stdin(const String &p_data) {
	return write_to_stdin_utf16(p_data);
}

Error SubProcessWindows::write_to_stdin_utf8(const String &p_data) {
	if (!is_process_running()) {
		return ERR_UNCONFIGURED;
	}

	if (p_data.length() == 0) {
		return OK;
	}

	CharString cs = p_data.utf8();

	if (_std_in_mutex) {
		_std_in_mutex->lock();
	}

	DWORD total_written = 0;

	// Note, we are using length() to skip sending null terminators!
	for (;;) {
		DWORD written;
		const bool success = WriteFile(_write_handles[1], (cs.get_data()) + total_written, cs.length() - total_written, &written, NULL);

		if (!success) {
			if (_std_in_mutex) {
				_std_in_mutex->unlock();
			}

			stop();
			return ERR_FILE_EOF;
		}

		total_written += written;

		if ((int)total_written >= cs.length()) {
			break;
		}
	}

	if (_std_in_mutex) {
		_std_in_mutex->unlock();
	}

	return OK;
}

Error SubProcessWindows::write_to_stdin_utf16(const String &p_data) {
	if (!is_process_running()) {
		return ERR_UNCONFIGURED;
	}

	if (p_data.length() == 0) {
		return OK;
	}

	Char16String cs = p_data.utf16();
	// Note, we are using length() to skip sending null terminators!
	int length_bytes = cs.length() * sizeof(char16_t);

	if (_std_in_mutex) {
		_std_in_mutex->lock();
	}

	DWORD total_written = 0;

	for (;;) {
		DWORD written;
		const bool success = WriteFile(_write_handles[1], ((const char *)cs.get_data()) + total_written, length_bytes - total_written, &written, NULL);

		if (!success) {
			if (_std_in_mutex) {
				_std_in_mutex->unlock();
			}

			stop();
			return ERR_FILE_EOF;
		}

		total_written += written;

		if ((int)total_written >= length_bytes) {
			break;
		}
	}

	if (_std_in_mutex) {
		_std_in_mutex->unlock();
	}

	return OK;
}

Error SubProcessWindows::write_to_stdin_utf32(const String &p_data) {
	if (!is_process_running()) {
		return ERR_UNCONFIGURED;
	}

	if (p_data.length() == 0) {
		return OK;
	}

	Char16String cs = p_data.utf16();

	if (_std_in_mutex) {
		_std_in_mutex->lock();
	}

	DWORD total_written = 0;
	// Note, we are using length() to skip sending null terminators!
	int length_bytes = p_data.length() * sizeof(CharType);

	for (;;) {
		DWORD written;
		const bool success = WriteFile(_write_handles[1], ((const char *)cs.get_data()) + total_written, length_bytes - total_written, &written, NULL);

		if (!success) {
			if (_std_in_mutex) {
				_std_in_mutex->unlock();
			}

			stop();
			return ERR_FILE_EOF;
		}

		total_written += written;

		if ((int)total_written >= length_bytes) {
			break;
		}
	}

	if (_std_in_mutex) {
		_std_in_mutex->unlock();
	}

	return OK;
}
Error SubProcessWindows::write_data_to_stdin(const PoolByteArray &p_data) {
	if (!is_process_running()) {
		return ERR_UNCONFIGURED;
	}

	if (p_data.size() == 0) {
		return OK;
	}

	if (_std_in_mutex) {
		_std_in_mutex->lock();
	}

	int size = p_data.size();
	PoolByteArray::Read r = p_data.read();

	DWORD total_written = 0;

	for (;;) {
		DWORD written;
		const bool success = WriteFile(_write_handles[1], r.ptr() + total_written, size - total_written, &written, NULL);

		if (!success) {
			if (_std_in_mutex) {
				_std_in_mutex->unlock();
			}

			stop();
			return ERR_FILE_EOF;
		}

		total_written += written;

		if ((int)total_written >= size) {
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

bool SubProcessWindows::_read_from_std_out(int &bytes_in_buffer) {
	const int CHUNK_SIZE = 4096;
	DWORD read = 0;
	_bytes.resize(bytes_in_buffer + CHUNK_SIZE);
	// Unlike in linux, here ReadFile blocks, until either it can read up to chunk size, or an error happens
	// So if read is 0, there was an issue.
	const bool success = ReadFile(_read_std_handles[0], _bytes.ptr() + bytes_in_buffer, CHUNK_SIZE, &read, NULL);
	if (!success || read == 0) {
		// No need
		//_bytes.resize(bytes_in_buffer);
		return true;
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
		return false;
	}

	const int bytes_to_convert = bytes_in_buffer + (newline_index + 1);
	_append_to_std_out(_bytes.ptr(), bytes_to_convert);

	bytes_in_buffer = read - (newline_index + 1);
	memmove(_bytes.ptr(), _bytes.ptr() + bytes_to_convert, bytes_in_buffer);

	return false;
}

bool SubProcessWindows::_read_from_std_err(int &err_bytes_in_buffer) {
	const int CHUNK_SIZE = 4096;
	DWORD err_read = 0;
	_err_bytes.resize(err_bytes_in_buffer + CHUNK_SIZE);
	const bool success = ReadFile(_read_std_err_handles[0], _err_bytes.ptr() + err_bytes_in_buffer, CHUNK_SIZE, &err_read, NULL);
	if (!success || err_read == 0) {
		// No need
		//_err_bytes.resize(err_bytes_in_buffer);
		return true;
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
		return false;
	}

	const int bytes_to_convert = err_bytes_in_buffer + (newline_index + 1);
	_append_to_std_err(_err_bytes.ptr(), bytes_to_convert);

	err_bytes_in_buffer = err_read - (newline_index + 1);
	memmove(_err_bytes.ptr(), _err_bytes.ptr() + bytes_to_convert, err_bytes_in_buffer);
	return false;
}

bool SubProcessWindows::_poll_read_from_std_out() {
	int bytes_in_buffer = _bytes.size();

	const int CHUNK_SIZE = 4096;
	int read_size = CHUNK_SIZE;

	DWORD total_available_bytes;
	if (!PeekNamedPipe(_read_std_handles[0], 0, 0, 0, &total_available_bytes, 0)) {
		return true;
	}

	if (total_available_bytes == 0) {
		return false;
	}

	if (total_available_bytes < CHUNK_SIZE) {
		read_size = total_available_bytes;
	}

	DWORD read = 0;

	_bytes.resize(bytes_in_buffer + CHUNK_SIZE);
	const bool success = ReadFile(_read_std_handles[0], _bytes.ptr() + bytes_in_buffer, read_size, &read, NULL);

	if (!success || read == 0) {
		// Note, stop() will process remaning bytes, we had an error, so get rid of the new chunk, as it's empty.
		_bytes.resize(bytes_in_buffer);
		//stop();
		return true;
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
	} else {
		// 0 read, remove chunk. Should probably save actual size as a variable eventually.
		_bytes.resize(bytes_in_buffer);
	}

	return false;
}
bool SubProcessWindows::_poll_read_from_std_err() {
	int bytes_in_buffer = _err_bytes.size();

	const int CHUNK_SIZE = 4096;
	int read_size = CHUNK_SIZE;

	DWORD total_available_bytes;
	if (!PeekNamedPipe(_read_std_handles[0], 0, 0, 0, &total_available_bytes, 0)) {
		return true;
	}

	if (total_available_bytes == 0) {
		return false;
	}

	if (total_available_bytes < CHUNK_SIZE) {
		read_size = total_available_bytes;
	}

	DWORD read = 0;

	_err_bytes.resize(bytes_in_buffer + CHUNK_SIZE);
	const bool success = ReadFile(_read_std_err_handles[0], _err_bytes.ptr() + bytes_in_buffer, read_size, &read, NULL);

	if (!success || read == 0) {
		// Note, stop() will process remaning bytes, we had an error, so get rid of the new chunk, as it's empty.
		_err_bytes.resize(bytes_in_buffer);
		//stop();
		return true;
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
	} else {
		// 0 read, remove chunk. Should probably save actual size as a variable eventually.
		_err_bytes.resize(bytes_in_buffer);
	}

	return false;
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
