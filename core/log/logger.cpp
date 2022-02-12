
#include "logger.h"

#include "core/string.h"
#include <cstdio>

#include <stdio.h>
#include "logger.h"
#include <thread>

void RLogger::log_trace(const String &str) {
	log_trace(str.data());
}
void RLogger::log_trace(const char *str) {
	printf("T %s\n", str);
}

void RLogger::log_message(const String &str) {
	log_message(str.data());
}
void RLogger::log_message(const char *str) {
	printf("M %s\n", str);
}

void RLogger::log_warning(const String &str) {
	log_warning(str.data());
}
void RLogger::log_warning(const char *str) {
	printf("W %s\n", str);
}

void RLogger::log_error(const String &str) {
	log_error(str.data());
}
void RLogger::log_error(const char *str) {
	printf("E %s\n", str);
}

void RLogger::_log_error(const char *p_function, const char *p_file, int p_line, const char *str) {
	printf("!ERROR: (%s) %s:%d. %s\n", p_file, p_function, p_line, str);
}
void RLogger::_log_error(const char *p_function, const char *p_file, int p_line, const String &str) {
	printf("!ERROR: (%s) %s:%d. %s\n", p_file, p_function, p_line, str.c_str());
}
void RLogger::_log_msg_error(const char *p_function, const char *p_file, int p_line, const char *p_msg, const char *str) {
	printf("!ERROR: (%s) %s:%d :: %s. %s\n", p_file, p_function, p_line, str, p_msg);
}
void RLogger::_log_index_error(const char *p_function, const char *p_file, int p_line, const int index, const int size, const char *str) {
	printf("!INDEX ERROR: (%s) %s:%d :: index: %d/%d. %s\n", p_file, p_function, p_line, index, size, str);
}
