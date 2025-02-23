#ifndef PLOGGER_H
#define PLOGGER_H

/*************************************************************************/
/*  logger.h                                                             */
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

#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/typedefs.h"
#include "logger_backend.h"

class String;

#define PLOG_TRACE(str) \
	PLogger::log_trace(__FUNCTION__, __FILE__, __LINE__, str);

#define PLOG_MSG(str) \
	PLogger::log_message(__FUNCTION__, __FILE__, __LINE__, str);

#define PLOG_WARN(str) \
	PLogger::log_warning(__FUNCTION__, __FILE__, __LINE__, str);

#define PLOG_ERR(str) \
	PLogger::log_error(__FUNCTION__, __FILE__, __LINE__, str);

#define PLOG_IMPORTANT(str) \
	PLogger::log_important(__FUNCTION__, __FILE__, __LINE__, str);

#define PLOG_CUSTOM(category, level, str) \
	PLogger::log_custom(category, level, __FUNCTION__, __FILE__, __LINE__, str);

class PLogger : public Object {
public:
	enum LogLevel {
		LOG_LEVEL_TRACE = 0,
		LOG_LEVEL_MESSAGE,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_ERROR,
		LOG_LEVEL_IMPORTANT,
		LOG_LEVEL_NONE,
	};

	static void log_trace(const String &str);
	static void log_trace(const char *str);
	static void log_trace(const char *p_function, const char *p_file, int p_line, const String &str);
	static void log_trace(const char *p_function, const char *p_file, int p_line, const char *str);

	static void log_message(const String &str);
	static void log_message(const char *str);
	static void log_message(const char *p_function, const char *p_file, int p_line, const String &str);
	static void log_message(const char *p_function, const char *p_file, int p_line, const char *str);

	static void log_warning(const String &str);
	static void log_warning(const char *str);
	static void log_warning(const char *p_function, const char *p_file, int p_line, const String &str);
	static void log_warning(const char *p_function, const char *p_file, int p_line, const char *str);

	static void log_error(const String &str);
	static void log_error(const char *str);
	static void log_error(const char *p_function, const char *p_file, int p_line, const char *str);
	static void log_error(const char *p_function, const char *p_file, int p_line, const String &str);

	static void log_important(const String &str);
	static void log_important(const char *str);
	static void log_important(const char *p_function, const char *p_file, int p_line, const char *str);
	static void log_important(const char *p_function, const char *p_file, int p_line, const String &str);

	static void log_custom(const StringName &p_category, const int p_level, const String &p_str);
	static void log_custom(const StringName &p_category, const int p_level, const char *str);
	static void log_custom(const StringName &p_category, const int p_level, const char *p_function, const char *p_file, int p_line, const char *str);
	static void log_custom(const StringName &p_category, const int p_level, const char *p_function, const char *p_file, int p_line, const String &str);

	static void do_log_trace(const String &str);
	static void do_log_message(const String &str);
	static void do_log_warning(const String &str);
	static void do_log_error(const String &str);
	static void do_log_important(const String &str);

	static LogLevel get_log_level();
	static void set_log_level(const LogLevel p_log_level);

	static LogLevel _log_level;
	static Ref<LoggerBackend> _backend;
};

#endif
