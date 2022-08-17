
#include "logger_backend.h"

#include "core/string/print_string.h"

void LoggerBackend::log_trace(const String &str) {
	call("_log_trace", str);
}
void LoggerBackend::log_message(const String &str) {
	call("_log_trace", str);
}
void LoggerBackend::log_warning(const String &str) {
	call("_log_trace", str);
}
void LoggerBackend::log_error(const String &str) {
	call("_log_trace", str);
}

void LoggerBackend::_log_trace(const String &str) {
	print_line(str);
}
void LoggerBackend::_log_message(const String &str) {
	print_line(str);
}
void LoggerBackend::_log_warning(const String &str) {
	print_line(str);
}
void LoggerBackend::_log_error(const String &str) {
	print_line(str);
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

	ClassDB::bind_method(D_METHOD("log_trace", "str"), &LoggerBackend::log_trace);
	ClassDB::bind_method(D_METHOD("log_message", "str"), &LoggerBackend::log_message);
	ClassDB::bind_method(D_METHOD("log_warning", "str"), &LoggerBackend::log_warning);
	ClassDB::bind_method(D_METHOD("log_error", "str"), &LoggerBackend::log_error);

	ClassDB::bind_method(D_METHOD("_log_trace", "str"), &LoggerBackend::_log_trace);
	ClassDB::bind_method(D_METHOD("_log_message", "str"), &LoggerBackend::_log_message);
	ClassDB::bind_method(D_METHOD("_log_warning", "str"), &LoggerBackend::_log_warning);
	ClassDB::bind_method(D_METHOD("_log_error", "str"), &LoggerBackend::_log_error);
}
