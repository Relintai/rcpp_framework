#include "logger.h"

#include <cstdio>

void Logger::log_trace(const String &str)
{
    log_trace(str.data());
}
void Logger::log_trace(const char *str)
{
    printf("TRACE: %s\n", str);
}

void Logger::log_message(const String &str)
{
    log_message(str.data());
}
void Logger::log_message(const char *str)
{
    printf("MESSAGE: %s\n", str);
}

void Logger::log_warning(const String &str)
{
    log_warning(str.data());
}
void Logger::log_warning(const char *str)
{
    printf("WARNING: %s\n", str);
}

void Logger::log_error(const String &str)
{
    log_error(str.data());
}
void Logger::log_error(const char *str)
{
    printf("ERROR: %s\n", str);
}

void Logger::_log_error(const char *p_function, const char *p_file, int p_line, const char *str)
{
    printf("!ERROR: (%s) %s:%d. %s\n", p_file, p_function, p_line, str);
}
void Logger::_log_msg_error(const char *p_function, const char *p_file, int p_line, const char *p_msg, const char *str)
{
    printf("!ERROR: (%s) %s:%d :: %s. %s\n", p_file, p_function, p_line, str, p_msg);
}
void Logger::_log_index_error(const char *p_function, const char *p_file, int p_line, const int index, const int size, const char *str)
{
    printf("!INDEX ERROR: (%s) %s:%d :: index: %d/%d. %s\n", p_file, p_function, p_line, index, size, str);
}
