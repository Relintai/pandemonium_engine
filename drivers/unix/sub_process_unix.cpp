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

#include "sub_process_unix.h"

#ifdef UNIX_ENABLED

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

Error SubProcessUnix::start() {
	if (_executable_path.empty()) {
		return ERR_FILE_BAD_PATH;
	}

	if (is_process_running()) {
		return ERR_BUSY;
	}

#ifdef __EMSCRIPTEN__
	// Don't compile this code at all to avoid undefined references.
	// Actual virtual call goes to OS_JavaScript.
	ERR_FAIL_V(ERR_BUG);
#else
	if (_blocking && _read_output) {
		String argss;
		argss = "\"" + _executable_path + "\"";

		for (int i = 0; i < _arguments.size(); i++) {
			argss += String(" \"") + _arguments[i] + "\"";
		}

		if (_read_std_err) {
			argss += " 2>&1"; // Read stderr too
		} else {
			argss += " 2>/dev/null"; //silence stderr
		}
		FILE *f = popen(argss.utf8().get_data(), "r");

		ERR_FAIL_COND_V_MSG(!f, ERR_CANT_OPEN, "Cannot pipe stream from process running with following arguments '" + argss + "'.");

		char buf[65535];

		while (fgets(buf, 65535, f)) {
			if (_pipe_mutex) {
				_pipe_mutex->lock();
			}
			_pipe += String::utf8(buf);
			if (_pipe_mutex) {
				_pipe_mutex->unlock();
			}
		}
		int rv = pclose(f);

		_exitcode = WEXITSTATUS(rv);

		return OK;
	}

	if (!_blocking && _read_output) {
		String argss;
		argss = "\"" + _executable_path + "\"";

		for (int i = 0; i < _arguments.size(); i++) {
			argss += String(" \"") + _arguments[i] + "\"";
		}

		if (_read_std_err) {
			argss += " 2>&1"; // Read stderr too
		} else {
			argss += " 2>/dev/null"; //silence stderr
		}

		_process_fp = popen(argss.utf8().get_data(), "r");

		ERR_FAIL_COND_V_MSG(!_process_fp, ERR_CANT_OPEN, "Cannot pipe stream from process running with following arguments '" + argss + "'.");

		return OK;
	}

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
	}

	if (_blocking) {
		int status;
		waitpid(pid, &status, 0);

		_exitcode = WIFEXITED(status) ? WEXITSTATUS(status) : status;
	} else {
		_process_id = pid;
	}

	return OK;
#endif
}

Error SubProcessUnix::stop() {
	if (_process_fp) {
		int rv = pclose(_process_fp);
		_process_fp = NULL;
		_exitcode = WEXITSTATUS(rv);
		_process_id = 0;
		return OK;
	}

	if (_process_id) {
		int ret = ::kill(_process_id, SIGKILL);

		if (!ret) {
			//avoid zombie process
			int st;
			::waitpid(_process_id, &st, 0);
		}

		_process_id = 0;

		return ret ? ERR_INVALID_PARAMETER : OK;
	}

	ERR_FAIL_V(ERR_BUG);
}

Error SubProcessUnix::poll() {
#ifdef __EMSCRIPTEN__
	// Don't compile this code at all to avoid undefined references.
	// Actual virtual call goes to OS_JavaScript.
	ERR_FAIL_V(ERR_BUG);
#else

	if (_process_fp) {
		if (fgets(_process_buf, 65535, _process_fp)) {
			if (_pipe_mutex) {
				_pipe_mutex->lock();
			}
			_pipe = String::utf8(_process_buf);
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
	//Not Yet Impl
	ERR_FAIL_V(ERR_BUG);
}

Error SubProcessUnix::send_data(const String &p_data) {
	//Not Yet Impl
	ERR_FAIL_V(ERR_BUG);
}

bool SubProcessUnix::is_process_running() const {
	if (_process_fp) {
		return !feof(_process_fp);
	}

	if (_process_id == 0) {
		return false;
	}

	int status = 0;
	if (waitpid(_process_id, &status, WNOHANG) != 0) {
		return false;
	}

	return true;
}

SubProcessUnix::SubProcessUnix() : SubProcess() {
	_process_fp = NULL;
}
SubProcessUnix::~SubProcessUnix() {
}

/*
Error SubProcessUnix::execute(const String &p_path, const List<String> &p_arguments, bool p_blocking, ProcessID *r_child_id, String *r_pipe, int *r_exitcode, bool read_stderr, Mutex *p_pipe_mutex, bool p_open_console) {
#ifdef __EMSCRIPTEN__
	// Don't compile this code at all to avoid undefined references.
	// Actual virtual call goes to OS_JavaScript.
	ERR_FAIL_V(ERR_BUG);
#else
	if (p_blocking && r_pipe) {
		String argss;
		argss = "\"" + p_path + "\"";

		for (int i = 0; i < p_arguments.size(); i++) {
			argss += String(" \"") + p_arguments[i] + "\"";
		}

		if (read_stderr) {
			argss += " 2>&1"; // Read stderr too
		} else {
			argss += " 2>/dev/null"; //silence stderr
		}
		FILE *f = popen(argss.utf8().get_data(), "r");

		ERR_FAIL_COND_V_MSG(!f, ERR_CANT_OPEN, "Cannot pipe stream from process running with following arguments '" + argss + "'.");

		char buf[65535];

		while (fgets(buf, 65535, f)) {
			if (p_pipe_mutex) {
				p_pipe_mutex->lock();
			}
			(*r_pipe) += String::utf8(buf);
			if (p_pipe_mutex) {
				p_pipe_mutex->unlock();
			}
		}
		int rv = pclose(f);
		if (r_exitcode) {
			*r_exitcode = WEXITSTATUS(rv);
		}

		return OK;
	}

	pid_t pid = fork();
	ERR_FAIL_COND_V(pid < 0, ERR_CANT_FORK);

	if (pid == 0) {
		// is child

		if (!p_blocking) {
			// For non blocking calls, create a new session-ID so parent won't wait for it.
			// This ensures the process won't go zombie at end.
			setsid();
		}

		Vector<CharString> cs;
		cs.push_back(p_path.utf8());
		for (int i = 0; i < p_arguments.size(); i++) {
			cs.push_back(p_arguments[i].utf8());
		}

		Vector<char *> args;
		for (int i = 0; i < cs.size(); i++) {
			args.push_back((char *)cs[i].get_data());
		}
		args.push_back(0);

		execvp(p_path.utf8().get_data(), &args[0]);
		// still alive? something failed..
		fprintf(stderr, "**ERROR** SubProcessUnix::execute - Could not create child process while executing: %s\n", p_path.utf8().get_data());
		raise(SIGKILL);
	}

	if (p_blocking) {
		int status;
		waitpid(pid, &status, 0);
		if (r_exitcode) {
			*r_exitcode = WIFEXITED(status) ? WEXITSTATUS(status) : status;
		}

	} else {
		if (r_child_id) {
			*r_child_id = pid;
		}
	}

	return OK;
#endif
}

*/

#endif //posix_enabled
