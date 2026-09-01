/*************************************************************************/
/*  sub_process_unix.cpp                                                 */
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

#include "sub_process_unix.h"

#ifdef UNIX_ENABLED

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

Error SubProcessUnix::start() {
#ifdef __EMSCRIPTEN__
	// Don't compile this code at all to avoid undefined references.
	// Actual virtual call goes to OS_JavaScript.
	ERR_FAIL_V(ERR_BUG);
#else

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

	if (_communication_flags == COMMUNICATION_FLAGS_NONE) {
		// We just run it, no need to worry about output

		pid_t pid = fork();
		ERR_FAIL_COND_V(pid < 0, ERR_CANT_FORK);

		if (pid == 0) {
			// is child

			if (!_blocking) {
				// For non blocking calls, create a new session-ID so parent won't wait for it.
				// This ensures the process won't go zombie at end.
				setsid();
			}

			Vector<CharString> cs;
			cs.push_back(_executable_path.utf8());
			for (int i = 0; i < _arguments.size(); i++) {
				cs.push_back(_arguments[i].utf8());
			}

			Vector<char *> args;
			for (int i = 0; i < cs.size(); i++) {
				args.push_back((char *)cs[i].get_data());
			}
			args.push_back(0);

			execvp(_executable_path.utf8().get_data(), &args[0]);
			// still alive? something failed..
			fprintf(stderr, "**ERROR** SubProcessUnix::execute - Could not create child process while executing: %s\n", _executable_path.utf8().get_data());
			raise(SIGKILL);
			return FAILED;
		}

		_process_id = pid;

		if (_blocking) {
			int status;
			waitpid(pid, &status, 0);

			_exitcode = WIFEXITED(status) ? WEXITSTATUS(status) : status;
		}

		return OK;
	}

	// We run it, and also set up the requested pipes

	// Pipe setup

	if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
		ERR_FAIL_COND_V(pipe(_read_std_pipes) != 0, FAILED);
	}

	if ((_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
		ERR_FAIL_COND_V(pipe(_read_std_err_pipes) != 0, FAILED);
	}

	if ((_communication_flags & COMMUNICATION_FLAGS_STDIN) != 0) {
		ERR_FAIL_COND_V(pipe(_write_pipes) != 0, FAILED);
	}

	// Fork it

	pid_t pid = fork();
	ERR_FAIL_COND_V(pid < 0, ERR_CANT_FORK);

	if (pid == 0) {
		// is child

		// Connect inherited pipes to stdout / err / in

		if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
			if (dup2(_read_std_pipes[1], STDOUT_FILENO) != STDOUT_FILENO) {
				fprintf(stderr, "**ERROR** SubProcessUnix::execute - Could not setup std out pipe.\n");
				raise(SIGKILL);
				return FAILED;
			}
		}

		if ((_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
			if (dup2(_read_std_err_pipes[1], STDERR_FILENO) != STDERR_FILENO) {
				fprintf(stderr, "**ERROR** SubProcessUnix::execute - Could not setup std err pipe.\n");
				raise(SIGKILL);
				return FAILED;
			}
		}

		if ((_communication_flags & COMMUNICATION_FLAGS_STDIN) != 0) {
			if (dup2(_write_pipes[0], STDIN_FILENO) != STDIN_FILENO) {
				fprintf(stderr, "**ERROR** SubProcessUnix::execute - Could not setup std in pipe.\n");
				raise(SIGKILL);
				return FAILED;
			}
		}

		// Close pipes (apparently you need to close all of them in the child, because of dup2)

		if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
			close(_read_std_pipes[0]);
			close(_read_std_pipes[1]);
			_read_std_pipes[0] = 0;
			_read_std_pipes[1] = 0;
		}

		if ((_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
			close(_read_std_err_pipes[0]);
			close(_read_std_err_pipes[1]);
			_read_std_err_pipes[0] = 0;
			_read_std_err_pipes[1] = 0;
		}

		if ((_communication_flags & COMMUNICATION_FLAGS_STDIN) != 0) {
			close(_write_pipes[0]);
			close(_write_pipes[1]);
			_write_pipes[0] = 0;
			_write_pipes[1] = 0;
		}

		if (!_blocking) {
			// For non blocking calls, create a new session-ID so parent won't wait for it.
			// This ensures the process won't go zombie at end.
			setsid();
		}

		Vector<CharString> cs;
		cs.push_back(_executable_path.utf8());
		for (int i = 0; i < _arguments.size(); i++) {
			cs.push_back(_arguments[i].utf8());
		}

		Vector<char *> args;
		for (int i = 0; i < cs.size(); i++) {
			args.push_back((char *)cs[i].get_data());
		}
		args.push_back(0);

		// Note that execvp replaces the current process (us) with the one requested.
		execvp(_executable_path.utf8().get_data(), &args[0]);
		// still alive? something failed..
		fprintf(stderr, "**ERROR** SubProcessUnix::execute - Could not create child process while executing: %s\n", _executable_path.utf8().get_data());
		raise(SIGKILL);
		return FAILED;
	}

	// parent

	_process_id = pid;

	// Close unneeded pipes

	if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
		close(_read_std_pipes[1]);
		_read_std_pipes[1] = 0;
	}

	if ((_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
		close(_read_std_err_pipes[1]);
		_read_std_err_pipes[1] = 0;
	}

	if ((_communication_flags & COMMUNICATION_FLAGS_STDIN) != 0) {
		close(_write_pipes[0]);
		_write_pipes[0] = 0;
	}

	if (_blocking) {
		int bytes_in_buffer = 0;
		int err_bytes_in_buffer = 0;

		const int CHUNK_SIZE = 4096;
		ssize_t rbytes = 0;
		ssize_t erbytes = 0;
		for (;;) { // Read StdOut and StdErr from pipe.
			// First go for stdin
			if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
				_bytes.resize(bytes_in_buffer + CHUNK_SIZE);

				rbytes = read(_read_std_pipes[0], _bytes.ptr() + bytes_in_buffer, CHUNK_SIZE);

				if (rbytes < 0) {
					_bytes.resize(bytes_in_buffer);
					stop();
					return ERR_FILE_EOF;
				}

				if (rbytes != 0) {
					// Assume that all possible encodings are ASCII-compatible.
					// Break at newline to allow receiving long output in portions.
					int newline_index = -1;
					for (int i = rbytes - 1; i >= 0; i--) {
						if (_bytes[bytes_in_buffer + i] == '\n') {
							newline_index = i;
							break;
						}
					}

					if (newline_index == -1) {
						bytes_in_buffer += rbytes;
						continue;
					}

					const int bytes_to_convert = bytes_in_buffer + (newline_index + 1);
					_append_to_std_out(_bytes.ptr(), bytes_to_convert);

					bytes_in_buffer = rbytes - (newline_index + 1);
					memmove(_bytes.ptr(), _bytes.ptr() + bytes_to_convert, bytes_in_buffer);
				}
			}

			// StdErr
			if ((_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
				_err_bytes.resize(err_bytes_in_buffer + CHUNK_SIZE);

				erbytes = read(_read_std_err_pipes[0], _err_bytes.ptr() + err_bytes_in_buffer, CHUNK_SIZE);

				if (erbytes < 0) {
					_err_bytes.resize(err_bytes_in_buffer);
					stop();
					return ERR_FILE_EOF;
				}

				if (erbytes != 0) {
					// Assume that all possible encodings are ASCII-compatible.
					// Break at newline to allow receiving long output in portions.
					int newline_index = -1;
					for (int i = erbytes - 1; i >= 0; i--) {
						if (_err_bytes[err_bytes_in_buffer + i] == '\n') {
							newline_index = i;
							break;
						}
					}

					if (newline_index == -1) {
						err_bytes_in_buffer += erbytes;
						continue;
					}

					const int bytes_to_convert = err_bytes_in_buffer + (newline_index + 1);
					_append_to_std_err(_err_bytes.ptr(), bytes_to_convert);

					err_bytes_in_buffer = erbytes - (newline_index + 1);
					memmove(_err_bytes.ptr(), _err_bytes.ptr() + bytes_to_convert, err_bytes_in_buffer);
				}
			}

			// Note that we don't worry about stdin here, as it can only happen if a thread launches a process in blocking mode, an an another writes to it.

			// This is needed to detect if the subprocess have terminated. even if the stdout and stderr is not connected.
			// Also on linux read() will not fail if the subprocess is not alive anymore.
			if (!is_process_running()) {
				break;
			}
		}

		// Read remaining

		// StdIn
		if (bytes_in_buffer > 0) {
			_append_to_std_out(_bytes.ptr(), bytes_in_buffer);
		}

		// StdErr
		if (err_bytes_in_buffer > 0) {
			_append_to_std_err(_err_bytes.ptr(), err_bytes_in_buffer);
		}

		// Close all remaining pipes

		if ((_communication_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
			close(_read_std_pipes[0]);
			_read_std_pipes[0] = 0;
		}

		if ((_communication_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
			close(_read_std_err_pipes[0]);
			_read_std_err_pipes[0] = 0;
		}

		if ((_communication_flags & COMMUNICATION_FLAGS_STDIN) != 0) {
			close(_write_pipes[1]);
			_write_pipes[1] = 0;
		}

		// Cleanup

		// Grab exit code
		int status;
		// "If a child has already changed state, then these calls return immediately."
		waitpid(pid, &status, 0);
		_exitcode = WIFEXITED(status) ? WEXITSTATUS(status) : status;

		// Not running anymore
		_process_id = 0;
	}

	return OK;
#endif
}

Error SubProcessUnix::stop() {
#ifdef __EMSCRIPTEN__
	// Don't compile this code at all to avoid undefined references.
	// Actual virtual call goes to OS_JavaScript.
	ERR_FAIL_V(ERR_BUG);
#else

	if (!_process_id) {
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
		if (_read_std_pipes[i]) {
			close(_read_std_pipes[i]);
			_read_std_pipes[i] = 0;
		}

		if (_read_std_err_pipes[i]) {
			close(_read_std_err_pipes[i]);
			_read_std_err_pipes[i] = 0;
		}

		if (_write_pipes[i]) {
			close(_write_pipes[i]);
			_write_pipes[i] = 0;
		}
	}

	int ret = ::kill(_process_id, SIGKILL);

	if (!ret) {
		//avoid zombie process
		int st;
		::waitpid(_process_id, &st, 0);
	}

	_process_id = 0;

	return ret ? ERR_INVALID_PARAMETER : OK;

#endif
}

Error SubProcessUnix::poll() {
#ifdef __EMSCRIPTEN__
	// Don't compile this code at all to avoid undefined references.
	// Actual virtual call goes to OS_JavaScript.
	ERR_FAIL_V(ERR_BUG);
#else

	if (_process_id == 0) {
		return ERR_UNAVAILABLE;
	}

	if (_blocking) {
		// If it's blocking, and we want to read output from an another thread, we can just do it without poll
		// Just ignore poll calls

		if (!is_process_running()) {
			return ERR_FILE_EOF;
		}

		return OK;
	}

	if (!_read_std_pipes[0] && !_read_std_err_pipes[0]) {
		return ERR_UNAVAILABLE;
	}

	if (_read_std_pipes[0]) {
		const int CHUNK_SIZE = 4096;

		int bytes_in_buffer = _bytes.size();

		_bytes.resize(bytes_in_buffer + CHUNK_SIZE);
		ssize_t rbytes = read(_read_std_pipes[0], _bytes.ptr() + bytes_in_buffer, CHUNK_SIZE);

		if (rbytes < 0) {
			// Note, stop() will process remaning bytes, we had an error, so get rid of the new chunk, as it's empty.
			_bytes.resize(bytes_in_buffer);
			stop();
			return ERR_FILE_EOF;
		}

		if (rbytes != 0) {
			// Assume that all possible encodings are ASCII-compatible.
			// Break at newline to allow receiving long output in portions.
			int newline_index = -1;
			for (int i = rbytes - 1; i >= 0; i--) {
				if (_bytes[bytes_in_buffer + i] == '\n') {
					newline_index = i;
					break;
				}
			}

			if (newline_index == -1) {
				bytes_in_buffer += rbytes;
			} else {
				const int bytes_to_convert = bytes_in_buffer + (newline_index + 1);
				_append_to_std_out(_bytes.ptr(), bytes_to_convert);

				bytes_in_buffer = rbytes - (newline_index + 1);
				memmove(_bytes.ptr(), _bytes.ptr() + bytes_to_convert, bytes_in_buffer);
			}

			_bytes.resize(bytes_in_buffer);
		}
	}

	if (_read_std_err_pipes[0]) {
		const int CHUNK_SIZE = 4096;

		int bytes_in_buffer = _err_bytes.size();

		_err_bytes.resize(bytes_in_buffer + CHUNK_SIZE);
		ssize_t rbytes = read(_read_std_err_pipes[0], _err_bytes.ptr() + bytes_in_buffer, CHUNK_SIZE);

		if (rbytes < 0) {
			// Note, stop() will process remaning bytes, we had an error, so get rid of the new chunk, as it's empty.
			_err_bytes.resize(bytes_in_buffer);
			stop();
			return ERR_FILE_EOF;
		}

		if (rbytes != 0) {
			// Assume that all possible encodings are ASCII-compatible.
			// Break at newline to allow receiving long output in portions.
			int newline_index = -1;
			for (int i = rbytes - 1; i >= 0; i--) {
				if (_err_bytes[bytes_in_buffer + i] == '\n') {
					newline_index = i;
					break;
				}
			}

			if (newline_index == -1) {
				bytes_in_buffer += rbytes;
			} else {
				const int bytes_to_convert = bytes_in_buffer + (newline_index + 1);
				_append_to_std_err(_err_bytes.ptr(), bytes_to_convert);

				bytes_in_buffer = rbytes - (newline_index + 1);
				memmove(_err_bytes.ptr(), _err_bytes.ptr() + bytes_to_convert, bytes_in_buffer);
			}

			_err_bytes.resize(bytes_in_buffer);
		}
	}

	// Need to check, as read will just keep returning 0 if the process has terminated.
	// This should the api more convenient to use.
	if (!is_process_running()) {
		return ERR_FILE_EOF;
	}

	return OK;
#endif
}

Error SubProcessUnix::send_signal(const int p_signal) {
	if (_process_id == 0) {
		return FAILED;
	}

	int err = kill(_process_id, p_signal);

	if (err == EPERM) {
		return ERR_UNAUTHORIZED;
	} else if (err == 0) {
		return OK;
	}

	return FAILED;
}

Error SubProcessUnix::write_to_stdin(const String &p_data) {
	if (_process_id == 0) {
		return ERR_UNAVAILABLE;
	}

	if (!_write_pipes[1]) {
		return ERR_UNAVAILABLE;
	}

	if (p_data.length() == 0) {
		return OK;
	}

	ssize_t sent = 0;
	CharString cs = p_data.utf8();

	if (_std_in_mutex) {
		_std_in_mutex->lock();
	}

	// Note, we are using lenght() to skip sending null terminators!
	while (sent < cs.length()) {
		ssize_t wb = write(_write_pipes[1], cs.get_data(), cs.length());

		// Error
		if (wb < 0) {
			if (_std_in_mutex) {
				_std_in_mutex->unlock();
			}

			stop();
			return ERR_FILE_EOF;
		}

		sent += wb;
	}

	if (_std_in_mutex) {
		_std_in_mutex->unlock();
	}

	return OK;
}

bool SubProcessUnix::is_process_running() const {
#ifdef __EMSCRIPTEN__
	// Don't compile this code at all to avoid undefined references.
	// Actual virtual call goes to OS_JavaScript.
	ERR_FAIL_V(false);
#else

	if (_process_id == 0) {
		return false;
	}

	int status = 0;
	if (waitpid(_process_id, &status, WNOHANG) != 0) {
		return false;
	}

	return true;
#endif
}

SubProcessUnix::SubProcessUnix() :
		SubProcess() {
	_read_std_pipes[0] = 0;
	_read_std_pipes[1] = 0;

	_read_std_err_pipes[0] = 0;
	_read_std_err_pipes[1] = 0;

	_write_pipes[0] = 0;
	_write_pipes[1] = 0;
}
SubProcessUnix::~SubProcessUnix() {
	stop();
}

void SubProcessUnix::_append_to_std_out(char *p_bytes, int p_size) {
	if (_std_out_mutex) {
		_std_out_mutex->lock();
	}
	_std_out += String::utf8(p_bytes, p_size);
	if (_std_out_mutex) {
		_std_out_mutex->unlock();
	}
}

void SubProcessUnix::_append_to_std_err(char *p_bytes, int p_size) {
	if (_std_err_mutex) {
		_std_err_mutex->lock();
	}
	_std_err += String::utf8(p_bytes, p_size);
	if (_std_err_mutex) {
		_std_err_mutex->unlock();
	}
}

#endif //posix_enabled
