
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

	ClassDB::bind_method(D_METHOD("get_log_level"), &_PLogger::get_log_level);
	ClassDB::bind_method(D_METHOD("set_log_level", "log_level"), &_PLogger::set_log_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "log_level", PROPERTY_HINT_ENUM, "Trace,Message,Warning,Error,None"), "set_log_level", "get_log_level");

	BIND_ENUM_CONSTANT(LOG_LEVEL_TRACE);
	BIND_ENUM_CONSTANT(LOG_LEVEL_MESSAGE);
	BIND_ENUM_CONSTANT(LOG_LEVEL_WARNING);
	BIND_ENUM_CONSTANT(LOG_LEVEL_ERROR);
	BIND_ENUM_CONSTANT(LOG_LEVEL_NONE);
}

_PLogger *_PLogger::_self = nullptr;
