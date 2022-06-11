#ifndef ERROR_MACROS_H
#define ERROR_MACROS_H

#include "core/log/logger.h"
#include "core/typedefs.h"

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

#define RPRINT_TRACE(str) \
	RLogger::print_trace(__FUNCTION__, __FILE__, __LINE__, str);

#define RLOG_TRACE(...)                                                                                \
	{                                                                                                  \
		String *_rlogger_string_ptr = RLogger::get_trace_string_ptr(__FUNCTION__, __FILE__, __LINE__); \
		_RLOG_MACRO_TEMPLATE_FUNC(_rlogger_string_ptr, __VA_ARGS__);                                   \
		RLogger::log_ret_ptr(_rlogger_string_ptr);                                                     \
	}

#define RPRINT_MSG(str) \
	RLogger::print_message(__FUNCTION__, __FILE__, __LINE__, str);

#define RLOG_MSG(...)                                                                                    \
	{                                                                                                    \
		String *_rlogger_string_ptr = RLogger::get_message_string_ptr(__FUNCTION__, __FILE__, __LINE__); \
		_RLOG_MACRO_TEMPLATE_FUNC(_rlogger_string_ptr, __VA_ARGS__);                                     \
		RLogger::log_ret_ptr(_rlogger_string_ptr);                                                       \
	}

#define RPRINT_WARN(str) \
	RLogger::print_warning(__FUNCTION__, __FILE__, __LINE__, str);

#define RLOG_WARN(...)                                                                                   \
	{                                                                                                    \
		String *_rlogger_string_ptr = RLogger::get_warning_string_ptr(__FUNCTION__, __FILE__, __LINE__); \
		_RLOG_MACRO_TEMPLATE_FUNC(_rlogger_string_ptr, __VA_ARGS__);                                     \
		RLogger::log_ret_ptr(_rlogger_string_ptr);                                                       \
	}

#define RPRINT_ERR(str) \
	RLogger::print_error(__FUNCTION__, __FILE__, __LINE__, str);

#define RLOG_ERR(...)                                                                                  \
	{                                                                                                  \
		String *_rlogger_string_ptr = RLogger::get_error_string_ptr(__FUNCTION__, __FILE__, __LINE__); \
		_RLOG_MACRO_TEMPLATE_FUNC(_rlogger_string_ptr, __VA_ARGS__);                                   \
		RLogger::log_ret_ptr(_rlogger_string_ptr);                                                     \
	}

#endif
