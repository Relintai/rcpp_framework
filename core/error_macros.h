#ifndef ERROR_MACROS_H
#define ERROR_MACROS_H

#include "core/log/logger.h"

// Based on Godot Engine's error_macros.h
// MIT License
// Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
// Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).


#ifdef _MSC_VER
#define GENERATE_TRAP                       \
	__debugbreak();                         \
	/* Avoid warning about control paths */ \
	for (;;) {                              \
	}
#else
#define GENERATE_TRAP __builtin_trap();
#endif

#define RLOG_TRACE(str) \
    Logger::log_trace(str);

#define RLOG_MSG(str) \
    Logger::log_message(str);

#define RLOG_WARN(str) \
    Logger::log_warning(str);

#define RLOG_ERR(str) \
    Logger::log_error(str);

#define ERR_FAIL_MSG(msg) \
    Logger::_log_error(__FUNCTION__, __FILE__, __LINE__, msg); \
    return;

#define ERR_FAIL_INDEX(index, size) \
    if ((index < 0) || (index >= size)) {\
        Logger::_log_index_error(__FUNCTION__, __FILE__, __LINE__, index, size, ""); \
        return;\
    } else\
        ((void)0)\

#define ERR_FAIL_INDEX_MSG(index, size, msg) \
    if ((index < 0) || (index >= size)) {\
        Logger::_log_index_error(__FUNCTION__, __FILE__, __LINE__, index, size, msg); \
        return;\
    } else\
        ((void)0)\

#define ERR_FAIL_INDEX_V(index, size, val) \
    if ((index < 0) || (index >= size)) {\
        Logger::_log_index_error(__FUNCTION__, __FILE__, __LINE__, index, size, ""); \
        return val;\
    } else\
        ((void)0)\

#define ERR_FAIL_INDEX_V_MSG(index, size, val, msg) \
    if ((index < 0) || (index >= size)) {\
        Logger::_log_index_error(__FUNCTION__, __FILE__, __LINE__, index, size, msg); \
        return val;\
    } else\
        ((void)0)\

#define ERR_FAIL_COND(cond) \
    if (cond) {\
        Logger::_log_error(__FUNCTION__, __FILE__, __LINE__, "ERR_FAIL_COND: \"" #cond "\" is true!"); \
        return;\
    } else\
        ((void)0)\

#define ERR_FAIL_COND_MSG(cond, msg) \
    if (cond) {\
        Logger::_log_error(__FUNCTION__, __FILE__, __LINE__, msg); \
        return;\
    } else\
        ((void)0)\

#define ERR_FAIL_COND_V(cond, val) \
    if (cond) {\
        Logger::_log_error(__FUNCTION__, __FILE__, __LINE__, "ERR_FAIL_COND: \"" #cond "\" is true!"); \
        return val;\
    } else\
        ((void)0)\

#define ERR_FAIL_COND_V_MSG(cond, val, msg) \
    if (cond) {\
        Logger::_log_error(__FUNCTION__, __FILE__, __LINE__, msg); \
        return val;\
    } else\
        ((void)0)\

#define CRASH_INDEX(index, size) \
    if ((index < 0) || (index >= size)) {\
        Logger::_log_index_error(__FUNCTION__, __FILE__, __LINE__, index, size, "CRASH!"); \
        GENERATE_TRAP \
    } else\
        ((void)0)\

#define CRASH_COND(cond) \
    if (cond) {\
        Logger::_log_error(__FUNCTION__, __FILE__, __LINE__, "CRASH_COND: \"" #cond "\" is true!"); \
        GENERATE_TRAP \
    } else\
        ((void)0)\

#endif
