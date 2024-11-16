/*************************************************************************/
/*  logger.cpp                                                           */
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

#include "logger.h"

#include "core/config/project_settings.h"
#include "core/os/dir_access.h"
#include "core/os/os.h"
#include "core/os/time.h"
#include "core/string/print_string.h"

// va_copy was defined in the C99, but not in C++ standards before C++11.
// When you compile C++ without --std=c++<XX> option, compilers still define
// va_copy, otherwise you have to use the internal version (__va_copy).
#if !defined(va_copy)
#if defined(__GNUC__)
#define va_copy(d, s) __va_copy((d), (s))
#else
#define va_copy(d, s) ((d) = (s))
#endif
#endif

#if defined(MINGW_ENABLED) || defined(_MSC_VER)
#define sprintf sprintf_s
#endif

bool Logger::should_log(bool p_err) {
	return (!p_err || _print_error_enabled) && (p_err || _print_line_enabled);
}

bool Logger::_flush_stdout_on_print = true;

void Logger::set_flush_stdout_on_print(bool value) {
	_flush_stdout_on_print = value;
}

void Logger::log_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, ErrorType p_type, bool p_force) {
	if (!p_force && !should_log(true)) {
		return;
	}

	const char *err_type = "ERROR";
	switch (p_type) {
		case ERR_ERROR:
			err_type = "ERROR";
			break;
		case ERR_WARNING:
			err_type = "WARNING";
			break;
		case ERR_SCRIPT:
			err_type = "SCRIPT ERROR";
			break;
		case ERR_SHADER:
			err_type = "SHADER ERROR";
			break;
		default:
			ERR_PRINT("Unknown error type");
			break;
	}

	const char *err_details;
	if (p_rationale && *p_rationale) {
		err_details = p_rationale;
	} else {
		err_details = p_code;
	}

	logf_error("%s: %s\n", err_type, err_details);
	logf_error("   at: %s (%s:%i) - %s\n", p_function, p_file, p_line, p_code);
}

void Logger::logf(const char *p_format, ...) {
	va_list argp;
	va_start(argp, p_format);

	logv(p_format, argp, false, false);

	va_end(argp);
}

void Logger::logf_error(const char *p_format, ...) {
	va_list argp;
	va_start(argp, p_format);

	logv(p_format, argp, true, false);

	va_end(argp);
}

Logger::~Logger() {}

void RotatedFileLogger::close_file() {
	if (file) {
		memdelete(file);
		file = nullptr;
	}
}

void RotatedFileLogger::clear_old_backups() {
	int max_backups = max_files - 1; // -1 for the current file

	String basename = base_path.get_file().get_basename();
	String extension = base_path.get_extension();

	DirAccess *da = DirAccess::open(base_path.get_base_dir());
	if (!da) {
		return;
	}

	da->list_dir_begin();
	String f = da->get_next();
	RBSet<String> backups;
	while (f != String()) {
		if (!da->current_is_dir() && f.begins_with(basename) && f.get_extension() == extension && f != base_path.get_file()) {
			backups.insert(f);
		}
		f = da->get_next();
	}
	da->list_dir_end();

	if (backups.size() > max_backups) {
		// since backups are appended with timestamp and Set iterates them in sorted order,
		// first backups are the oldest
		int to_delete = backups.size() - max_backups;
		for (RBSet<String>::Element *E = backups.front(); E && to_delete > 0; E = E->next(), --to_delete) {
			da->remove(E->get());
		}
	}

	memdelete(da);
}

void RotatedFileLogger::rotate_file() {
	close_file();

	if (FileAccess::exists(base_path)) {
		if (max_files > 1) {
			String timestamp = Time::get_singleton()->get_datetime_string_from_system().replace(":", ".");
			String backup_name = base_path.get_basename() + timestamp;
			if (base_path.get_extension() != String()) {
				backup_name += "." + base_path.get_extension();
			}

			DirAccess *da = DirAccess::open(base_path.get_base_dir());
			if (da) {
				da->copy(base_path, backup_name);
				memdelete(da);
			}
			clear_old_backups();
		}
	} else {
		DirAccess *da = DirAccess::create(DirAccess::ACCESS_USERDATA);
		if (da) {
			da->make_dir_recursive(base_path.get_base_dir());
			memdelete(da);
		}
	}

	file = FileAccess::open(base_path, FileAccess::WRITE);
}

RotatedFileLogger::RotatedFileLogger(const String &p_base_path, int p_max_files) :
		base_path(p_base_path.simplify_path()),
		max_files(p_max_files > 0 ? p_max_files : 1),
		file(nullptr) {
	rotate_file();
}

void RotatedFileLogger::logv(const char *p_format, va_list p_list, bool p_err, bool p_force) {
	if (!p_force && !should_log(p_err)) {
		return;
	}

	if (file) {
		const int static_buf_size = 512;
		char static_buf[static_buf_size];
		char *buf = static_buf;
		va_list list_copy;
		va_copy(list_copy, p_list);
		int len = vsnprintf(buf, static_buf_size, p_format, p_list);
		if (len >= static_buf_size) {
			buf = (char *)Memory::alloc_static(len + 1);
			vsnprintf(buf, len + 1, p_format, list_copy);
		}
		va_end(list_copy);
		file->store_buffer((uint8_t *)buf, len);

		if (len >= static_buf_size) {
			Memory::free_static(buf);
		}

		if (p_err || _flush_stdout_on_print) {
			// Don't always flush when printing stdout to avoid performance
			// issues when `print()` is spammed in release builds.
			file->flush();
		}
	}
}

RotatedFileLogger::~RotatedFileLogger() {
	close_file();
}

void StdLogger::logv(const char *p_format, va_list p_list, bool p_err, bool p_force) {
	if (!p_force && !should_log(p_err)) {
		return;
	}

	if (p_err) {
		vfprintf(stderr, p_format, p_list);
	} else {
		vprintf(p_format, p_list);
		if (_flush_stdout_on_print) {
			// Don't always flush when printing stdout to avoid performance
			// issues when `print()` is spammed in release builds.
			fflush(stdout);
		}
	}
}

StdLogger::~StdLogger() {}

CompositeLogger::CompositeLogger(Vector<Logger *> p_loggers) :
		loggers(p_loggers) {
}

void CompositeLogger::logv(const char *p_format, va_list p_list, bool p_err, bool p_force) {
	if (!p_force && !should_log(p_err)) {
		return;
	}

	for (int i = 0; i < loggers.size(); ++i) {
		va_list list_copy;
		va_copy(list_copy, p_list);
		loggers[i]->logv(p_format, list_copy, p_err, p_force);
		va_end(list_copy);
	}
}

void CompositeLogger::log_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, ErrorType p_type, bool p_force) {
	if (!p_force && !should_log(true)) {
		return;
	}

	for (int i = 0; i < loggers.size(); ++i) {
		loggers[i]->log_error(p_function, p_file, p_line, p_code, p_rationale, p_type, p_force);
	}
}

void CompositeLogger::add_logger(Logger *p_logger) {
	loggers.push_back(p_logger);
}

CompositeLogger::~CompositeLogger() {
	for (int i = 0; i < loggers.size(); ++i) {
		memdelete(loggers[i]);
	}
}
