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

#include "core/string/print_string.h"
#include "core/typedefs.h"

void PLogger::log_trace(const String &str) {
	if (_log_level > LOG_LEVEL_TRACE) {
		return;
	}

	String s;
	s += "T ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}
void PLogger::log_trace(const char *str) {
	if (_log_level > LOG_LEVEL_TRACE) {
		return;
	}

	String s;
	s += "T ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}
void PLogger::log_trace(const char *p_function, const char *p_file, int p_line, const char *str) {
	if (_log_level > LOG_LEVEL_TRACE) {
		return;
	}

	String s;
	s += "T | ";
	s += p_file;
	s += "::";
	s += p_function;
	s += ":";
	s += String::num(p_line);
	s += " | ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}
void PLogger::log_trace(const char *p_function, const char *p_file, int p_line, const String &str) {
	if (_log_level > LOG_LEVEL_TRACE) {
		return;
	}

	String s;
	s += "T | ";
	s += p_file;
	s += "::";
	s += p_function;
	s += ":";
	s += String::num(p_line);
	s += " | ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}

void PLogger::log_message(const String &str) {
	if (_log_level > LOG_LEVEL_MESSAGE) {
		return;
	}

	String s;
	s += "M ";
	s += str;
	//s += "\n";

	do_log_message(s);
}
void PLogger::log_message(const char *str) {
	if (_log_level > LOG_LEVEL_MESSAGE) {
		return;
	}

	String s;
	s += "M ";
	s += str;
	//s += "\n";

	do_log_message(s);
}
void PLogger::log_message(const char *p_function, const char *p_file, int p_line, const char *str) {
	if (_log_level > LOG_LEVEL_MESSAGE) {
		return;
	}

	String s;
	s += "M | ";
	s += p_file;
	s += "::";
	s += p_function;
	s += ":";
	s += String::num(p_line);
	s += " | ";
	s += str;
	//s += "\n";

	do_log_message(s);
}
void PLogger::log_message(const char *p_function, const char *p_file, int p_line, const String &str) {
	if (_log_level > LOG_LEVEL_MESSAGE) {
		return;
	}

	String s;
	s += "M | ";
	s += p_file;
	s += "::";
	s += p_function;
	s += ":";
	s += String::num(p_line);
	s += " | ";
	s += str;
	//s += "\n";

	do_log_message(s);
}

void PLogger::log_warning(const String &str) {
	if (_log_level > LOG_LEVEL_WARNING) {
		return;
	}

	String s;
	s += "W ";
	s += str;
	//s += "\n";

	do_log_warning(s);
}
void PLogger::log_warning(const char *str) {
	if (_log_level > LOG_LEVEL_WARNING) {
		return;
	}

	String s;
	s += "W ";
	s += str;
	//s += "\n";

	do_log_warning(s);
}
void PLogger::log_warning(const char *p_function, const char *p_file, int p_line, const char *str) {
	if (_log_level > LOG_LEVEL_WARNING) {
		return;
	}

	String s;
	s += "W | ";
	s += p_file;
	s += "::";
	s += p_function;
	s += ":";
	s += String::num(p_line);
	s += " | ";
	s += str;
	//s += "\n";

	do_log_warning(s);
}
void PLogger::log_warning(const char *p_function, const char *p_file, int p_line, const String &str) {
	if (_log_level > LOG_LEVEL_WARNING) {
		return;
	}

	String s;
	s += "W | ";
	s += p_file;
	s += "::";
	s += p_function;
	s += ":";
	s += String::num(p_line);
	s += " | ";
	s += str;
	//s += "\n";

	do_log_warning(s);
}

void PLogger::log_error(const String &str) {
	if (_log_level > LOG_LEVEL_ERROR) {
		return;
	}

	String s;
	s += "E ";
	s += str;
	//s += "\n";

	do_log_error(s);
}
void PLogger::log_error(const char *str) {
	if (_log_level > LOG_LEVEL_ERROR) {
		return;
	}

	String s;
	s += "E ";
	s += str;
	//s += "\n";

	do_log_error(s);
}
void PLogger::log_error(const char *p_function, const char *p_file, int p_line, const char *str) {
	if (_log_level > LOG_LEVEL_ERROR) {
		return;
	}

	String s;
	s += "E | ";
	s += p_file;
	s += "::";
	s += p_function;
	s += ":";
	s += String::num(p_line);
	s += " | ";
	s += str;
	//s += "\n";

	do_log_error(s);
}
void PLogger::log_error(const char *p_function, const char *p_file, int p_line, const String &str) {
	if (_log_level > LOG_LEVEL_ERROR) {
		return;
	}

	String s;
	s += "E | ";
	s += p_file;
	s += "::";
	s += p_function;
	s += ":";
	s += String::num(p_line);
	s += " | ";
	s += str;
	//s += "\n";

	do_log_error(s);
}

void PLogger::log_important(const String &str) {
	if (_log_level > LOG_LEVEL_IMPORTANT) {
		return;
	}

	String s;
	s += "I ";
	s += str;
	//s += "\n";

	do_log_important(s);
}
void PLogger::log_important(const char *str) {
	if (_log_level > LOG_LEVEL_IMPORTANT) {
		return;
	}

	String s;
	s += "I ";
	s += str;
	//s += "\n";

	do_log_important(s);
}
void PLogger::log_important(const char *p_function, const char *p_file, int p_line, const char *str) {
	if (_log_level > LOG_LEVEL_IMPORTANT) {
		return;
	}

	String s;
	s += "I | ";
	s += p_file;
	s += "::";
	s += p_function;
	s += ":";
	s += String::num(p_line);
	s += " | ";
	s += str;
	//s += "\n";

	do_log_important(s);
}
void PLogger::log_important(const char *p_function, const char *p_file, int p_line, const String &str) {
	if (_log_level > LOG_LEVEL_IMPORTANT) {
		return;
	}

	String s;
	s += "I | ";
	s += p_file;
	s += "::";
	s += p_function;
	s += ":";
	s += String::num(p_line);
	s += " | ";
	s += str;
	//s += "\n";

	do_log_important(s);
}

void PLogger::do_log_trace(const String &str) {
	if (_backend.is_valid()) {
		_backend->log_trace(str);
	} else {
		force_print_line(str);
	}
}

void PLogger::do_log_message(const String &str) {
	if (_backend.is_valid()) {
		_backend->log_message(str);
	} else {
		force_print_line(str);
	}
}

void PLogger::do_log_warning(const String &str) {
	if (_backend.is_valid()) {
		_backend->log_warning(str);
	} else {
		force_print_line(str);
	}
}

void PLogger::do_log_error(const String &str) {
	if (_backend.is_valid()) {
		_backend->log_error(str);
	} else {
		force_print_error(str);
	}
}

void PLogger::do_log_important(const String &str) {
	if (_backend.is_valid()) {
		_backend->log_important(str);
	} else {
		force_print_error(str);
	}
}

PLogger::LogLevel PLogger::get_log_level() {
	return _log_level;
}
void PLogger::set_log_level(const LogLevel p_log_level) {
	_log_level = p_log_level;
}

PLogger::LogLevel PLogger::_log_level = LOG_LEVEL_MESSAGE;
Ref<LoggerBackend> PLogger::_backend;
