
#include "logger.h"

#include "core/print_string.h"
#include "core/typedefs.h"

void PLogger::log_trace(const String &str) {
	String s;
	s += "T ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}
void PLogger::log_trace(const char *str) {
	String s;
	s += "T ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}
void PLogger::log_trace(const char *p_function, const char *p_file, int p_line, const char *str) {
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
	String s;
	s += "M ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}
void PLogger::log_message(const char *str) {
	String s;
	s += "M ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}
void PLogger::log_message(const char *p_function, const char *p_file, int p_line, const char *str) {
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

	do_log_trace(s);
}
void PLogger::log_message(const char *p_function, const char *p_file, int p_line, const String &str) {
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

	do_log_trace(s);
}

void PLogger::log_warning(const String &str) {
	String s;
	s += "W ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}
void PLogger::log_warning(const char *str) {
	String s;
	s += "W ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}
void PLogger::log_warning(const char *p_function, const char *p_file, int p_line, const char *str) {
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

	do_log_trace(s);
}
void PLogger::log_warning(const char *p_function, const char *p_file, int p_line, const String &str) {
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

	do_log_trace(s);
}

void PLogger::log_error(const String &str) {
	String s;
	s += "E ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}
void PLogger::log_error(const char *str) {
	String s;
	s += "E ";
	s += str;
	//s += "\n";

	do_log_trace(s);
}
void PLogger::log_error(const char *p_function, const char *p_file, int p_line, const char *str) {
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

	do_log_trace(s);
}
void PLogger::log_error(const char *p_function, const char *p_file, int p_line, const String &str) {
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

	do_log_trace(s);
}

void PLogger::do_log_trace(const String &str) {
	if (_backend.is_valid()) {
		_backend->log_trace(str);
	} else {
		print_line(str);
	}
}

void PLogger::do_log_message(const String &str) {
	if (_backend.is_valid()) {
		_backend->log_message(str);
	} else {
		print_line(str);
	}
}

void PLogger::do_log_warning(const String &str) {
	if (_backend.is_valid()) {
		_backend->log_warning(str);
	} else {
		print_line(str);
	}
}

void PLogger::do_log_error(const String &str) {
	if (_backend.is_valid()) {
		_backend->log_error(str);
	} else {
		print_line(str);
	}
}

Ref<LoggerBackend> PLogger::_backend;
