
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
		ERR_PRINT(str);
		//print_error(str);
	}
}

PLogger::LogLevel PLogger::get_log_level() {
	return _log_level;
}
void PLogger::set_log_level(const LogLevel p_log_level) {
	_log_level = p_log_level;
}

PLogger::LogLevel PLogger::_log_level = LOG_LEVEL_TRACE;
Ref<LoggerBackend> PLogger::_backend;
