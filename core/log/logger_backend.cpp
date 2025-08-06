/*************************************************************************/
/*  logger_backend.cpp                                                   */
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

#include "logger_backend.h"

#include "core/io/logger.h"
#include "core/log/logger.h"
#include "core/string/print_string.h"

void LoggerBackend::log_trace(const String &str) {
	call("_log_trace", str);
}
void LoggerBackend::log_message(const String &str) {
	call("_log_message", str);
}
void LoggerBackend::log_warning(const String &str) {
	call("_log_warning", str);
}
void LoggerBackend::log_error(const String &str) {
	call("_log_error", str);
}
void LoggerBackend::log_important(const String &str) {
	call("_log_important", str);
}
void LoggerBackend::log_custom(const StringName &p_category, const int p_level, const String &str) {
	call("_log_custom", p_category, p_level, str);
}

void LoggerBackend::_log_trace(const String &str) {
	force_print_line(str);
}
void LoggerBackend::_log_message(const String &str) {
	force_print_line(str);
}
void LoggerBackend::_log_warning(const String &str) {
	force_print_line(str);
}
void LoggerBackend::_log_error(const String &str) {
	force_print_error(str);
}
void LoggerBackend::_log_important(const String &str) {
	force_print_error(str);
}
void LoggerBackend::_log_custom(const StringName &p_category, const int p_level, const String &str) {
	String s;
	s += p_category;
	s += " : ";
	s += String::num(p_level);
	s += " | ";
	s += str;

	if (p_level < PLogger::LOG_LEVEL_ERROR) {
		force_print_line(s);
	} else {
		force_print_error(s);
	}
}

LoggerBackend::LoggerBackend() {
}

LoggerBackend::~LoggerBackend() {
}

void LoggerBackend::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_log_trace", PropertyInfo(Variant::STRING, "str")));
	BIND_VMETHOD(MethodInfo("_log_message", PropertyInfo(Variant::STRING, "str")));
	BIND_VMETHOD(MethodInfo("_log_warning", PropertyInfo(Variant::STRING, "str")));
	BIND_VMETHOD(MethodInfo("_log_error", PropertyInfo(Variant::STRING, "str")));
	BIND_VMETHOD(MethodInfo("_log_important", PropertyInfo(Variant::STRING, "str")));
	BIND_VMETHOD(MethodInfo("_log_custom",
			PropertyInfo(Variant::STRING_NAME, "category"),
			PropertyInfo(Variant::INT, "level"),
			PropertyInfo(Variant::STRING, "str")));

	ClassDB::bind_method(D_METHOD("log_trace", "str"), &LoggerBackend::log_trace);
	ClassDB::bind_method(D_METHOD("log_message", "str"), &LoggerBackend::log_message);
	ClassDB::bind_method(D_METHOD("log_warning", "str"), &LoggerBackend::log_warning);
	ClassDB::bind_method(D_METHOD("log_error", "str"), &LoggerBackend::log_error);
	ClassDB::bind_method(D_METHOD("log_important", "str"), &LoggerBackend::log_important);
	ClassDB::bind_method(D_METHOD("log_custom", "category", "level", "str"), &LoggerBackend::log_custom);

	ClassDB::bind_method(D_METHOD("_log_trace", "str"), &LoggerBackend::_log_trace);
	ClassDB::bind_method(D_METHOD("_log_message", "str"), &LoggerBackend::_log_message);
	ClassDB::bind_method(D_METHOD("_log_warning", "str"), &LoggerBackend::_log_warning);
	ClassDB::bind_method(D_METHOD("_log_error", "str"), &LoggerBackend::_log_error);
	ClassDB::bind_method(D_METHOD("_log_important", "str"), &LoggerBackend::_log_important);
	ClassDB::bind_method(D_METHOD("_log_custom", "category", "level", "str"), &LoggerBackend::log_custom);
}
