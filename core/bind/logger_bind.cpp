
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
void _PLogger::log_trace(const char *str) {
	PLogger::log_trace(str);
}
void _PLogger::log_trace(const char *p_function, const char *p_file, int p_line, const char *str) {
	PLogger::log_trace(p_function, p_file, p_line, str);
}
void _PLogger::log_trace(const char *p_function, const char *p_file, int p_line, const String &str) {
	PLogger::log_trace(p_function, p_file, p_line, str);
}

void _PLogger::log_message(const String &str) {
	PLogger::log_message(str);
}
void _PLogger::log_message(const char *str) {
	PLogger::log_message(str);
}
void _PLogger::log_message(const char *p_function, const char *p_file, int p_line, const char *str) {
	PLogger::log_message(p_function, p_file, p_line, str);
}
void _PLogger::log_message(const char *p_function, const char *p_file, int p_line, const String &str) {
	PLogger::log_message(p_function, p_file, p_line, str);
}

void _PLogger::log_warning(const String &str) {
	PLogger::log_warning(str);
}
void _PLogger::log_warning(const char *str) {
	PLogger::log_warning(str);
}
void _PLogger::log_warning(const char *p_function, const char *p_file, int p_line, const char *str) {
	PLogger::log_warning(p_function, p_file, p_line, str);
}
void _PLogger::log_warning(const char *p_function, const char *p_file, int p_line, const String &str) {
	PLogger::log_warning(p_function, p_file, p_line, str);
}

void _PLogger::log_error(const String &str) {
	PLogger::log_error(str);
}
void _PLogger::log_error(const char *str) {
	PLogger::log_error(str);
}
void _PLogger::log_error(const char *p_function, const char *p_file, int p_line, const char *str) {
	PLogger::log_error(p_function, p_file, p_line, str);
}
void _PLogger::log_error(const char *p_function, const char *p_file, int p_line, const String &str) {
	PLogger::log_error(p_function, p_file, p_line, str);
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
}

_PLogger *_PLogger::_self = nullptr;
