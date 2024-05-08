#ifndef LOL_CORE_H_
#define LOL_CORE_H_

#include "lol-color.h"
#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOL_LOG_NONE = 0,
    LOL_LOG_FATAL,
    LOL_LOG_ERROR,
    LOL_LOG_WARN,
    LOL_LOG_INFO,
    LOL_LOG_DEBUG,
    LOL_LOG_TRACE,
    LOL_LOG_DEFAULT = LOL_LOG_INFO,
    LOL_LOG_FULL = LOL_LOG_TRACE,
} lol_log_level_e;

lol_log_level_e lol_log_level_string_to_enum(const char *level);

typedef struct lol_context_s {
    void *document;
    const char *config_file;

    int file_enable;
    lol_log_level_e file_level;
    const char *file_location;
    int stdout_enable;
    lol_log_level_e stdout_level;
} lol_context_t;

/* add compile check for printf */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#if !defined (__clang__) && ((__GNUC__ > (4)) || \
     ((__GNUC__ == (4)) && (__GNUC_MINOR__ >= (4))))
#define LOL_GNUC_PRINTF(f, v) __attribute__ ((format (gnu_printf, f, v)))
#else
#define LOL_GNUC_PRINTF(f, v) __attribute__ ((format (__printf__, f, v)))
#endif
#else
#define LOL_GNUC_PRINTF(f, v)
#endif


#define LOL_FUNC __func__

#ifdef __cplusplus
}
#endif

#endif /* LOL_CORE_H_ */
