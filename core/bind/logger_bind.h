#ifndef PLOGGER_BIND_H
#define PLOGGER_BIND_H

/*************************************************************************/
/*  logger_bind.h                                                        */
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

#include "core/log/logger_backend.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/typedefs.h"

class String;

class _PLogger : public Object {
	GDCLASS(_PLogger, Object);

public:
	enum LogLevel {
		LOG_LEVEL_TRACE = 0,
		LOG_LEVEL_MESSAGE,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_ERROR,
		LOG_LEVEL_IMPORTANT,
		LOG_LEVEL_NONE,
	};

	Ref<LoggerBackend> get_backend();
	void set_backend(const Ref<LoggerBackend> &backend);

	void log_trace(const String &str);
	void log_message(const String &str);
	void log_warning(const String &str);
	void log_error(const String &str);
	void log_important(const String &str);
	void log_custom(const StringName &p_category, const int p_level, const String &str);

	LogLevel get_log_level();
	void set_log_level(const LogLevel p_log_level);

	static _PLogger *get_singleton();

	_PLogger();
	~_PLogger();

protected:
	static void _bind_methods();

	static _PLogger *_self;
};

VARIANT_ENUM_CAST(_PLogger::LogLevel);

#endif
