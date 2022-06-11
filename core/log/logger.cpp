
#include "logger.h"

#include "core/string.h"
#include <cstdio>

#include "logger.h"
#include <stdio.h>
#include <thread>


void RLogger::print_trace(const String &str) {
	print_trace(str.data());
}
void RLogger::print_trace(const char *str) {
	printf("T %s\n", str);
}
void RLogger::print_trace(const char *p_function, const char *p_file, int p_line, const char *str) {
	printf("T | %s::%s:%d | %s\n", p_file, p_function, p_line, str);
}
void RLogger::print_trace(const char *p_function, const char *p_file, int p_line, const String &str) {
	printf("T | %s::%s:%d | %s\n", p_file, p_function, p_line, str.c_str());
}

void RLogger::print_message(const String &str) {
	print_message(str.data());
}
void RLogger::print_message(const char *str) {
	printf("M %s\n", str);
}
void RLogger::print_message(const char *p_function, const char *p_file, int p_line, const char *str) {
	printf("M | %s::%s:%d | %s\n", p_file, p_function, p_line, str);
}
void RLogger::print_message(const char *p_function, const char *p_file, int p_line, const String &str) {
	printf("M | %s::%s:%d | %s\n", p_file, p_function, p_line, str.c_str());
}

void RLogger::print_warning(const String &str) {
	print_warning(str.data());
}
void RLogger::print_warning(const char *str) {
	printf("W %s\n", str);
}
void RLogger::print_warning(const char *p_function, const char *p_file, int p_line, const char *str) {
	printf("W | %s::%s:%d | %s\n", p_file, p_function, p_line, str);
}
void RLogger::print_warning(const char *p_function, const char *p_file, int p_line, const String &str) {
	printf("W | %s::%s:%d | %s\n", p_file, p_function, p_line, str.c_str());
}

void RLogger::print_error(const String &str) {
	print_error(str.data());
}
void RLogger::print_error(const char *str) {
	printf("E %s\n", str);
}

void RLogger::print_error(const char *p_function, const char *p_file, int p_line, const char *str) {
	printf("E | %s::%s:%d | %s\n", p_file, p_function, p_line, str);
}
void RLogger::print_error(const char *p_function, const char *p_file, int p_line, const String &str) {
	printf("E | %s::%s:%d | %s\n", p_file, p_function, p_line, str.c_str());
}
void RLogger::print_msg_error(const char *p_function, const char *p_file, int p_line, const char *p_msg, const char *str) {
	printf("E | %s::%s:%d | :: %s. %s\n", p_file, p_function, p_line, str, p_msg);
}
void RLogger::print_index_error(const char *p_function, const char *p_file, int p_line, const int index, const int size, const char *str) {
	printf("E (INDEX) | %s::%s:%d | :: index: %d/%d. %s\n", p_file, p_function, p_line, index, size, str);
}

void RLogger::log_trace(const String &str) {
	log_trace(str.data());
}
void RLogger::log_trace(const char *str) {
	printf("T %s\n", str);
}
void RLogger::log_trace(const char *p_function, const char *p_file, int p_line, const char *str) {
	printf("T | %s::%s:%d | %s\n", p_file, p_function, p_line, str);
}
void RLogger::log_trace(const char *p_function, const char *p_file, int p_line, const String &str) {
	printf("T | %s::%s:%d | %s\n", p_file, p_function, p_line, str.c_str());
}

void RLogger::log_message(const String &str) {
	log_message(str.data());
}
void RLogger::log_message(const char *str) {
	printf("M %s\n", str);
}
void RLogger::log_message(const char *p_function, const char *p_file, int p_line, const char *str) {
	printf("M | %s::%s:%d | %s\n", p_file, p_function, p_line, str);
}
void RLogger::log_message(const char *p_function, const char *p_file, int p_line, const String &str) {
	printf("M | %s::%s:%d | %s\n", p_file, p_function, p_line, str.c_str());
}

void RLogger::log_warning(const String &str) {
	log_warning(str.data());
}
void RLogger::log_warning(const char *str) {
	printf("W %s\n", str);
}
void RLogger::log_warning(const char *p_function, const char *p_file, int p_line, const char *str) {
	printf("W | %s::%s:%d | %s\n", p_file, p_function, p_line, str);
}
void RLogger::log_warning(const char *p_function, const char *p_file, int p_line, const String &str) {
	printf("W | %s::%s:%d | %s\n", p_file, p_function, p_line, str.c_str());
}

void RLogger::log_error(const String &str) {
	log_error(str.data());
}
void RLogger::log_error(const char *str) {
	printf("E %s\n", str);
}

void RLogger::log_error(const char *p_function, const char *p_file, int p_line, const char *str) {
	printf("E | %s::%s:%d | %s\n", p_file, p_function, p_line, str);
}
void RLogger::log_error(const char *p_function, const char *p_file, int p_line, const String &str) {
	printf("E | %s::%s:%d | %s\n", p_file, p_function, p_line, str.c_str());
}
void RLogger::log_msg_error(const char *p_function, const char *p_file, int p_line, const char *p_msg, const char *str) {
	printf("E | %s::%s:%d | :: %s. %s\n", p_file, p_function, p_line, str, p_msg);
}
void RLogger::log_index_error(const char *p_function, const char *p_file, int p_line, const int index, const int size, const char *str) {
	printf("E (INDEX) | %s::%s:%d | :: index: %d/%d. %s\n", p_file, p_function, p_line, index, size, str);
}

String *RLogger::get_string_ptr(const int p_default_size) {
	return new String(p_default_size);
}
String *RLogger::get_string_ptr(const char *p_function, const char *p_file, int p_line, const int p_default_size) {
	String *s = new String(p_default_size);

	s->append_str(p_function);
	s->append_str("::");
	s->append_str(p_file);
	s->append_str(":");
	s->append_str(String::num(p_line));
	s->append_str(" | ");

	return s;
}
String *RLogger::get_string_ptr(const char *p_prefix, const char *p_function, const char *p_file, int p_line, const int p_default_size) {
	String *s = new String(p_default_size);

	s->append_str(p_prefix);
	s->append_str(" | ");
	s->append_str(p_function);
	s->append_str("::");
	s->append_str(p_file);
	s->append_str(":");
	s->append_str(String::num(p_line));
	s->append_str(" | ");

	return s;
}
void RLogger::return_string_ptr(String *str) {
	delete str;
}

String *RLogger::get_trace_string_ptr(const int p_default_size) {
	String *str = get_string_ptr(p_default_size);
	str->append_str("T ");
	return str;
}
String *RLogger::get_message_string_ptr(const int p_default_size) {
	String *str = get_string_ptr(p_default_size);
	str->append_str("M ");
	return str;
}
String *RLogger::get_warning_string_ptr(const int p_default_size) {
	String *str = get_string_ptr(p_default_size);
	str->append_str("W ");
	return str;
}
String *RLogger::get_error_string_ptr(const int p_default_size) {
	String *str = get_string_ptr(p_default_size);
	str->append_str("E ");
	return str;
}

String *RLogger::get_trace_string_ptr(const char *p_function, const char *p_file, int p_line, const int p_default_size) {
	return get_string_ptr("T", p_function, p_file, p_line, p_default_size);
}
String *RLogger::get_message_string_ptr(const char *p_function, const char *p_file, int p_line, const int p_default_size) {
	return get_string_ptr("M", p_function, p_file, p_line, p_default_size);
}
String *RLogger::get_warning_string_ptr(const char *p_function, const char *p_file, int p_line, const int p_default_size) {
	return get_string_ptr("W", p_function, p_file, p_line, p_default_size);
}
String *RLogger::get_error_string_ptr(const char *p_function, const char *p_file, int p_line, const int p_default_size) {
	return get_string_ptr("E", p_function, p_file, p_line, p_default_size);
}

void RLogger::log_ptr(String *str) {
	printf("%s\n", str->data());
}

void RLogger::log_ret_ptr(String *str) {
	log_ptr(str);

	return_string_ptr(str);
}
