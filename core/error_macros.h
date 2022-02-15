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
    RLogger::log_trace(__FUNCTION__, __FILE__, __LINE__, str);

#define RLOG_MSG(str) \
    RLogger::log_message(__FUNCTION__, __FILE__, __LINE__, str);

#define RLOG_WARN(str) \
    RLogger::log_warning(__FUNCTION__, __FILE__, __LINE__, str);

#define RLOG_ERR(str) \
    RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, str);

#define ERR_FAIL_MSG(msg) \
    RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, msg); \
    return;

#define ERR_FAIL_V_MSG(val, msg) \
    RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, msg); \
    return val;

#define ERR_FAIL_INDEX(index, size) \
    if ((index < 0) || (index >= size)) {\
        RLogger::log_index_error(__FUNCTION__, __FILE__, __LINE__, index, size, ""); \
        return;\
    } else\
        ((void)0)\

#define ERR_FAIL_INDEX_MSG(index, size, msg) \
    if ((index < 0) || (index >= size)) {\
        RLogger::log_index_error(__FUNCTION__, __FILE__, __LINE__, index, size, msg); \
        return;\
    } else\
        ((void)0)\

#define ERR_FAIL_INDEX_V(index, size, val) \
    if ((index < 0) || (index >= size)) {\
        RLogger::log_index_error(__FUNCTION__, __FILE__, __LINE__, index, size, ""); \
        return val;\
    } else\
        ((void)0)\

#define ERR_FAIL_INDEX_V_MSG(index, size, val, msg) \
    if ((index < 0) || (index >= size)) {\
        RLogger::log_index_error(__FUNCTION__, __FILE__, __LINE__, index, size, msg); \
        return val;\
    } else\
        ((void)0)\

#define ERR_FAIL_COND(cond) \
    if (cond) {\
        RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, "ERR_FAIL_COND: \"" #cond "\" is true!"); \
        return;\
    } else\
        ((void)0)\

#define ERR_FAIL_COND_MSG(cond, msg) \
    if (cond) {\
        RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, msg); \
        return;\
    } else\
        ((void)0)\

#define ERR_FAIL_COND_V(cond, val) \
    if (cond) {\
        RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, "ERR_FAIL_COND: \"" #cond "\" is true!"); \
        return val;\
    } else\
        ((void)0)\

#define ERR_FAIL_COND_V_MSG(cond, val, msg) \
    if (cond) {\
        RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, msg); \
        return val;\
    } else\
        ((void)0)\

#define ERR_CONTINUE(cond) \
    if (cond) {\
        RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, "ERR_CONTINUE: \"" #cond "\" is true!"); \
        continue;\
    } else\
        ((void)0)\

#define ERR_CONTINUE_MSG(cond, msg) \
    if (cond) {\
        RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, msg); \
        continue;\
    } else\
        ((void)0)\

#define ERR_CONTINUE_ACTION(cond, action) \
    if (cond) {\
        RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, "ERR_CONTINUE: \"" #cond "\" is true!"); \
        action;\
        continue;\
    } else\
        ((void)0)\

#define ERR_CONTINUE_ACTION_MSG(cond, action, msg) \
    if (cond) {\
        RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, msg); \
        action;\
        continue;\
    } else\
        ((void)0)\

#define CRASH_INDEX(index, size) \
    if ((index < 0) || (index >= size)) {\
        RLogger::log_index_error(__FUNCTION__, __FILE__, __LINE__, index, size, "CRASH!"); \
        GENERATE_TRAP \
    } else\
        ((void)0)\

#define CRASH_COND(cond) \
    if (cond) {\
        RLogger::log_error(__FUNCTION__, __FILE__, __LINE__, "CRASH_COND: \"" #cond "\" is true!"); \
        GENERATE_TRAP \
    } else\
        ((void)0)\

#endif
