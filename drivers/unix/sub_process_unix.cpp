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

	if (_pipe_mutex) {
		_pipe_mutex->lock();
	}
	_std_out = String();
	_std_err = String();
	if (_pipe_mutex) {
		_pipe_mutex->unlock();
	}

	if (_comminucation_flags == COMMUNICATION_FLAGS_NONE) {
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

	if ((_comminucation_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
		ERR_FAIL_COND_V(pipe(_read_std_pipes) != 0, FAILED);
	}

	if ((_comminucation_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
		ERR_FAIL_COND_V(pipe(_read_std_err_pipes) != 0, FAILED);
	}

	if ((_comminucation_flags & COMMUNICATION_FLAGS_STDIN) != 0) {
		ERR_FAIL_COND_V(pipe(_write_pipes) != 0, FAILED);
	}

	// Fork it

	pid_t pid = fork();
	ERR_FAIL_COND_V(pid < 0, ERR_CANT_FORK);

	if (pid == 0) {
		// is child

		// Connect pipes

		if ((_comminucation_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
			if (dup2(_read_std_pipes[1], STDOUT_FILENO) != STDOUT_FILENO) {
				fprintf(stderr, "**ERROR** SubProcessUnix::execute - Could not setup std out pipe.\n");
				raise(SIGKILL);
				return FAILED;
			}
		}

		if ((_comminucation_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
			if (dup2(_read_std_err_pipes[1], STDERR_FILENO) != STDERR_FILENO) {
				fprintf(stderr, "**ERROR** SubProcessUnix::execute - Could not setup std err pipe.\n");
				raise(SIGKILL);
				return FAILED;
			}
		}

		if ((_comminucation_flags & COMMUNICATION_FLAGS_STDIN) != 0) {
			if (dup2(_write_pipes[1], STDIN_FILENO) != STDIN_FILENO) {
				fprintf(stderr, "**ERROR** SubProcessUnix::execute - Could not setup std in pipe.\n");
				raise(SIGKILL);
				return FAILED;
			}
		}

		// Close pipes (appaprnelt you need to close all of them, because of dup2)

		if ((_comminucation_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
			close(_read_std_pipes[0]);
			close(_read_std_pipes[1]);
			_read_std_pipes[0] = 0;
			_read_std_pipes[1] = 0;
		}

		if ((_comminucation_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
			close(_read_std_err_pipes[0]);
			close(_read_std_err_pipes[1]);
			_read_std_err_pipes[0] = 0;
			_read_std_err_pipes[1] = 0;
		}

		if ((_comminucation_flags & COMMUNICATION_FLAGS_STDIN) != 0) {
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

	if ((_comminucation_flags & COMMUNICATION_FLAGS_STDOUT) != 0) {
		close(_read_std_pipes[1]);
		_read_std_pipes[1] = 0;
	}

	if ((_comminucation_flags & COMMUNICATION_FLAGS_STDERR) != 0) {
		close(_read_std_err_pipes[1]);
		_read_std_err_pipes[1] = 0;
	}

	if ((_comminucation_flags & COMMUNICATION_FLAGS_STDIN) != 0) {
		close(_write_pipes[1]);
		_write_pipes[1] = 0;
	}

	if (_blocking) {
		// TODO read output in while loop
		int status;
		waitpid(pid, &status, 0);
		_exitcode = WIFEXITED(status) ? WEXITSTATUS(status) : status;
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

	if (!_read_std_pipes[0] && !_read_std_err_pipes[0]) {
		return ERR_UNAVAILABLE;
	}

	if (_read_std_pipes[0]) {
		// NEeds to be like windows's
		if (read(_read_std_pipes[0], _process_buf, 65535)) {
			if (_pipe_mutex) {
				_pipe_mutex->lock();
			}
			_std_out = String::utf8(_process_buf);
			if (_pipe_mutex) {
				_pipe_mutex->unlock();
			}
		} else {
			// The process finished
			// Cleanup:
			stop();
			return ERR_FILE_EOF;
		}
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

Error SubProcessUnix::send_data(const String &p_data) {
	if (_process_id == 0) {
		return ERR_UNAVAILABLE;
	}

	if (!_write_pipes[0]) {
		return ERR_UNAVAILABLE;
	}

	CharString cs = p_data.utf8();

	if (write(_write_pipes[0], cs.get_data(), cs.size()) != cs.size()) {
		return ERR_FILE_EOF;
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

#endif //posix_enabled
