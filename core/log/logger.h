#ifndef LOGGER_H
#define LOGGER_H

#include "core/string.h"

class Logger {
public:
    static void log_trace(const String &str);
    static void log_trace(const char *str);

    static void log_message(const String &str);
    static void log_message(const char *str);

    static void log_warning(const String &str);
    static void log_warning(const char *str);

    static void log_error(const String &str);
    static void log_error(const char *str);

    static void _log_error(const char *p_function, const char *p_file, int p_line, const char *str);
    static void _log_msg_error(const char *p_function, const char *p_file, int p_line, const char *p_msg, const char *str);
    static void _log_index_error(const char *p_function, const char *p_file, int p_line, const int index, const int size, const char *str);
};


#endif