
#ifndef PLOGGER_H
#define PLOGGER_H

#include "core/reference.h"
#include "core/typedefs.h"
#include "core/ustring.h"
#include "logger_backend.h"

class String;

class PLogger : public Object {
public:
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

	static void do_log_trace(const String &str);
	static void do_log_message(const String &str);
	static void do_log_warning(const String &str);
	static void do_log_error(const String &str);

	static Ref<LoggerBackend> _backend;
};

// template methods for the variadic log macros. Add more as needed.
template <class STR, class A>
_FORCE_INLINE_ void _RLOG_MACRO_TEMPLATE_FUNC(STR str, A p0) {
	str->append(p0);
}

template <class STR, class A, class B>
_FORCE_INLINE_ void _RLOG_MACRO_TEMPLATE_FUNC(STR str, A p0, B p1) {
	str->append(p0);
	str->push_back(' ');
	str->append(p1);
}

template <class STR, class A, class B, class C>
_FORCE_INLINE_ void _RLOG_MACRO_TEMPLATE_FUNC(STR str, A p0, B p1, C p2) {
	str->append(p0);
	str->push_back(' ');
	str->append(p1);
	str->push_back(' ');
	str->append(p2);
}

template <class STR, class A, class B, class C, class D>
_FORCE_INLINE_ void _RLOG_MACRO_TEMPLATE_FUNC(STR str, A p0, B p1, C p2, D p3) {
	str->append(p0);
	str->push_back(' ');
	str->append(p1);
	str->push_back(' ');
	str->append(p2);
	str->push_back(' ');
	str->append(p3);
}

template <class STR, class A, class B, class C, class D, class E>
_FORCE_INLINE_ void _RLOG_MACRO_TEMPLATE_FUNC(STR str, A p0, B p1, C p2, D p3, E p4) {
	str->append(p0);
	str->push_back(' ');
	str->append(p1);
	str->push_back(' ');
	str->append(p2);
	str->push_back(' ');
	str->append(p3);
	str->push_back(' ');
	str->append(p4);
}

#define PLOG_TRACE(str) \
	PLogger::print_trace(__FUNCTION__, __FILE__, __LINE__, str);

#define PALOG_TRACE(...)                                                                               \
	{                                                                                                  \
		String *_rlogger_string_ptr = PLogger::get_trace_string_ptr(__FUNCTION__, __FILE__, __LINE__); \
		_RLOG_MACRO_TEMPLATE_FUNC(_rlogger_string_ptr, __VA_ARGS__);                                   \
		PLogger::log_ret_ptr(_rlogger_string_ptr);                                                     \
	}

#define PLOG_MSG(str) \
	PLogger::print_message(__FUNCTION__, __FILE__, __LINE__, str);

#define PALOG_MSG(...)                                                                                   \
	{                                                                                                    \
		String *_rlogger_string_ptr = PLogger::get_message_string_ptr(__FUNCTION__, __FILE__, __LINE__); \
		_RLOG_MACRO_TEMPLATE_FUNC(_rlogger_string_ptr, __VA_ARGS__);                                     \
		PLogger::log_ret_ptr(_rlogger_string_ptr);                                                       \
	}

#define PLOG_WARN(str) \
	PLogger::print_warning(__FUNCTION__, __FILE__, __LINE__, str);

#define PALOG_WARN(...)                                                                                  \
	{                                                                                                    \
		String *_rlogger_string_ptr = PLogger::get_warning_string_ptr(__FUNCTION__, __FILE__, __LINE__); \
		_RLOG_MACRO_TEMPLATE_FUNC(_rlogger_string_ptr, __VA_ARGS__);                                     \
		PLogger::log_ret_ptr(_rlogger_string_ptr);                                                       \
	}

#define PLOG_ERR(str) \
	PLogger::print_error(__FUNCTION__, __FILE__, __LINE__, str);

#define PALOG_ERR(...)                                                                                 \
	{                                                                                                  \
		String *_rlogger_string_ptr = PLogger::get_error_string_ptr(__FUNCTION__, __FILE__, __LINE__); \
		_RLOG_MACRO_TEMPLATE_FUNC(_rlogger_string_ptr, __VA_ARGS__);                                   \
		PLogger::log_ret_ptr(_rlogger_string_ptr);                                                     \
	}

#endif
