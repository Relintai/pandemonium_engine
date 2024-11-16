/*************************************************************************/
/*  logger_bind.cpp                                                      */
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

#include "logger_bind.h"

#include "core/log/logger.h"

Ref<LoggerBackend> _PLogger::get_backend() {
	return PLogger::_backend;
}
void _PLogger::set_backend(const Ref<LoggerBackend> &backend) {
	PLogger::_backend = backend;
}

void _PLogger::log_trace(const String &str) {
	PLogger::log_trace(str);
}

void _PLogger::log_message(const String &str) {
	PLogger::log_message(str);
}

void _PLogger::log_warning(const String &str) {
	PLogger::log_warning(str);
}

void _PLogger::log_error(const String &str) {
	PLogger::log_error(str);
}

void _PLogger::log_important(const String &str) {
	PLogger::log_important(str);
}

_PLogger::LogLevel _PLogger::get_log_level() {
	return static_cast<LogLevel>(static_cast<int>(PLogger::get_log_level()));
}
void _PLogger::set_log_level(const LogLevel p_log_level) {
	PLogger::set_log_level(static_cast<PLogger::LogLevel>(static_cast<int>(p_log_level)));
}

_PLogger *_PLogger::get_singleton() {
	return _self;
}

_PLogger::_PLogger() {
	_self = this;
}
_PLogger::~_PLogger() {
	_self = nullptr;
}

void _PLogger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_backend"), &_PLogger::get_backend);
	ClassDB::bind_method(D_METHOD("set_backend", "backend"), &_PLogger::set_backend);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "backend", PROPERTY_HINT_RESOURCE_TYPE, "LoggerBackend"), "set_backend", "get_backend");

	ClassDB::bind_method(D_METHOD("log_trace", "str"), &_PLogger::log_trace);
	ClassDB::bind_method(D_METHOD("log_message", "str"), &_PLogger::log_message);
	ClassDB::bind_method(D_METHOD("log_warning", "str"), &_PLogger::log_warning);
	ClassDB::bind_method(D_METHOD("log_error", "str"), &_PLogger::log_error);
	ClassDB::bind_method(D_METHOD("log_important", "str"), &_PLogger::log_important);

	ClassDB::bind_method(D_METHOD("get_log_level"), &_PLogger::get_log_level);
	ClassDB::bind_method(D_METHOD("set_log_level", "log_level"), &_PLogger::set_log_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "log_level", PROPERTY_HINT_ENUM, "Trace,Message,Warning,Error,None"), "set_log_level", "get_log_level");

	BIND_ENUM_CONSTANT(LOG_LEVEL_TRACE);
	BIND_ENUM_CONSTANT(LOG_LEVEL_MESSAGE);
	BIND_ENUM_CONSTANT(LOG_LEVEL_WARNING);
	BIND_ENUM_CONSTANT(LOG_LEVEL_ERROR);
	BIND_ENUM_CONSTANT(LOG_LEVEL_IMPORTANT);
	BIND_ENUM_CONSTANT(LOG_LEVEL_NONE);
}

_PLogger *_PLogger::_self = nullptr;
