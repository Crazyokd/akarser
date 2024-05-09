#ifndef AKA_LOG_H_
#define AKA_LOG_H_

#include <stdint.h>
#include <stdio.h>

#include "aka-core.h"
#include "aka-config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NOR              "\033[0m" /* all off */
#define FGC_BLACK        "\033[30m" /* Foreground Color: Black */
#define FGC_RED          "\033[31m" /* Foreground Color: Red */
#define FGC_BOLD_RED     "\033[1;31m" /* Foreground Color: Bold Red */
#define FGC_GREEN        "\033[32m" /* Foreground Color: Green */
#define FGC_BOLD_GREEN   "\033[1;32m" /* Foreground Color: Bold Green */
#define FGC_YELLOW       "\033[33m" /* Foreground Color: Yellow */
#define FGC_BOLD_YELLOW  "\033[1;33m" /* Foreground Color: Bold Yellow */
#define FGC_BOLD_BLUE    "\033[1;34m" /* Foreground Color: Bold Blue */
#define FGC_BOLD_MAGENTA "\033[1;35m" /* Foreground Color: Bold Magenta */
#define FGC_BOLD_CYAN    "\033[1;36m" /* Foreground Color: Bold Cyan */
#define FGC_WHITE        "\033[37m" /* Foreground Color: White  */
#define FGC_BOLD_WHITE   "\033[1;37m" /* Foreground Color: Bold White  */
#define FGC_DEFAULT      "\033[39m" /* Foreground Color: default */

typedef enum {
    AKA_LOG_NONE = 0,
    AKA_LOG_FATAL,
    AKA_LOG_ERROR,
    AKA_LOG_WARN,
    AKA_LOG_INFO,
    AKA_LOG_DEBUG,
    AKA_LOG_TRACE,
    AKA_LOG_DEFAULT = AKA_LOG_INFO,
    AKA_LOG_FULL = AKA_LOG_TRACE,
} aka_log_level_e;

typedef struct aka_context_s {
    aka_log_level_e file_level;
    const char *file_location;
    aka_log_level_e stdout_level;
} aka_context_t;

void aka_init(aka_conf_entry_t *entry);

void aka_log_printf(aka_log_level_e level, const char *file, int line,
                    const char *func, int content_only, const char *format, ...)
    AKA_GNUC_PRINTF(6, 7);

#define aka_log_message(level, err, ...) \
    aka_log_printf(level, __FILE__, __LINE__, __func__, 0, __VA_ARGS__)

#define aka_log_print(level, ...) \
    aka_log_printf(level, NULL, 0, NULL, 1, __VA_ARGS__)

#define aka_fatal(...) aka_log_message(AKA_LOG_FATAL, 0, __VA_ARGS__)
#define aka_error(...) aka_log_message(AKA_LOG_ERROR, 0, __VA_ARGS__)
#define aka_warn(...)  aka_log_message(AKA_LOG_WARN, 0, __VA_ARGS__)
#define aka_info(...)  aka_log_message(AKA_LOG_INFO, 0, __VA_ARGS__)
#define aka_debug(...) aka_log_message(AKA_LOG_DEBUG, 0, __VA_ARGS__)
#define aka_trace(...) aka_log_message(AKA_LOG_TRACE, 0, __VA_ARGS__)

#define AKA_HUGE_LEN 8192 /* Can Stack */


#ifdef __cplusplus
}
#endif

#endif // AKA_LOG_H_
