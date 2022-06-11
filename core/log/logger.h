
#ifndef LOGGER_H
#define LOGGER_H

#include <cstring>
#include <functional>
#include <iostream>
#include <vector>

class String;

class RLogger {
public:
	static void print_trace(const String &str);
	static void print_trace(const char *str);
	static void print_trace(const char *p_function, const char *p_file, int p_line, const String &str);
	static void print_trace(const char *p_function, const char *p_file, int p_line, const char *str);

	static void print_message(const String &str);
	static void print_message(const char *str);
	static void print_message(const char *p_function, const char *p_file, int p_line, const String &str);
	static void print_message(const char *p_function, const char *p_file, int p_line, const char *str);

	static void print_warning(const String &str);
	static void print_warning(const char *str);
	static void print_warning(const char *p_function, const char *p_file, int p_line, const String &str);
	static void print_warning(const char *p_function, const char *p_file, int p_line, const char *str);

	static void print_error(const String &str);
	static void print_error(const char *str);
	static void print_error(const char *p_function, const char *p_file, int p_line, const char *str);
	static void print_error(const char *p_function, const char *p_file, int p_line, const String &str);
	static void print_msg_error(const char *p_function, const char *p_file, int p_line, const char *p_msg, const char *str);
	static void print_index_error(const char *p_function, const char *p_file, int p_line, const int index, const int size, const char *str);

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
	static void log_msg_error(const char *p_function, const char *p_file, int p_line, const char *p_msg, const char *str);
	static void log_index_error(const char *p_function, const char *p_file, int p_line, const int index, const int size, const char *str);

	static String *get_string_ptr(const int p_default_size = 100);
	static String *get_string_ptr(const char *p_function, const char *p_file, int p_line, const int p_default_size = 300);
	static String *get_string_ptr(const char *p_prefix, const char *p_function, const char *p_file, int p_line, const int p_default_size = 300);
	static void return_string_ptr(String *str);

	static String *get_trace_string_ptr(const int p_default_size = 100);
	static String *get_message_string_ptr(const int p_default_size = 100);
	static String *get_warning_string_ptr(const int p_default_size = 100);
	static String *get_error_string_ptr(const int p_default_size = 100);

	static String *get_trace_string_ptr(const char *p_function, const char *p_file, int p_line, const int p_default_size = 300);
	static String *get_message_string_ptr(const char *p_function, const char *p_file, int p_line, const int p_default_size = 300);
	static String *get_warning_string_ptr(const char *p_function, const char *p_file, int p_line, const int p_default_size = 300);
	static String *get_error_string_ptr(const char *p_function, const char *p_file, int p_line, const int p_default_size = 300);

	static void log_ptr(String *str);
	static void log_ret_ptr(String *str);
};

#endif
