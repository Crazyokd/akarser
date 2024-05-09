#include "aka-log.h"
#include "aka-strings.h"

#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <execinfo.h>

static aka_log_level_e aka_log_level_string_to_enum(const char *level)
{
    aka_log_level_e level_e;
    if (!strcmp(level, "FATAL")) {
        level_e = AKA_LOG_FATAL;
    } else if (!strcmp(level, "ERROR")) {
        level_e = AKA_LOG_ERROR;
    } else if (!strcmp(level, "WARN")) {
        level_e = AKA_LOG_WARN;
    } else if (!strcmp(level, "INFO")) {
        level_e = AKA_LOG_INFO;
    } else if (!strcmp(level, "DEBUG")) {
        level_e = AKA_LOG_DEBUG;
    } else if (!strcmp(level, "TRACE")) {
        level_e = AKA_LOG_TRACE;
    } else {
        level_e = AKA_LOG_INFO;
    }
    return level_e;
}

static char *log_content(char *buf, char *last, const char *format, va_list ap)
{
    va_list bp;

    va_copy(bp, ap);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
    buf = aka_vslprintf(buf, last, format, bp);
#pragma GCC diagnostic pop
    va_end(bp);

    return buf;
}

static char *log_timestamp(char *buf, char *last, int use_color)
{
    struct timeval tv;
    struct tm tm;
    char nowstr[32];

    gettimeofday(&tv, NULL);
    memset(&tm, 0, sizeof(tm));
    (void)localtime_r(&(tv.tv_sec), &tm);
    strftime(nowstr, sizeof nowstr, "%m/%d %H:%M:%S", &tm);

    buf = aka_slprintf(buf, last, "%s%s.%03d%s: ", use_color ? FGC_GREEN : "",
                       nowstr, (int)(tv.tv_usec / 1000), use_color ? NOR : "");

    return buf;
}

static const char *level_strings[] = {
    NULL, "FATAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE",
};

static char *log_level(char *buf, char *last, aka_log_level_e level,
                       int use_color)
{
    const char *colors[] = {
        NOR,           FGC_BOLD_RED, FGC_RED, FGC_BOLD_YELLOW, FGC_BOLD_GREEN,
        FGC_BOLD_CYAN, FGC_WHITE,
    };

    buf = aka_slprintf(buf, last, "%s%s%s: ", use_color ? colors[level] : "",
                       level_strings[level], use_color ? NOR : "");

    return buf;
}

static char *log_linefeed(char *buf, char *last)
{
    if (buf > last - 2) buf = last - 2;

    return aka_slprintf(buf, last, "\n");
}

static void aka_log_vprintf(aka_log_level_e level, const char *file, int line,
                     const char *func, int content_only, const char *format,
                     va_list ap)
{
    char logstr[AKA_HUGE_LEN];
    char *p, *last;

    int wrote_stderr = 0;

    if (!wrote_stderr) {
        int use_color = 0;
#if !defined(_WIN32)
        use_color = 1;
#endif

        p = logstr;
        last = logstr + AKA_HUGE_LEN;

        if (!content_only) {
            p = log_timestamp(p, last, use_color);
            p = log_level(p, last, level, use_color);
        }
        p = log_content(p, last, format, ap);
        if (!content_only) {
            p = aka_slprintf(p, last, " (%s:%d)", file, line);
            p = aka_slprintf(p, last, " %s()", func);
            p = log_linefeed(p, last);
        }

        fprintf(stderr, "%s", logstr);
        fflush(stderr);
    }
}

void aka_log_printf(aka_log_level_e level, const char *file, int line,
                    const char *func, int content_only, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    aka_log_vprintf(level, file, line, func, content_only, format, args);
    va_end(args);
}

static aka_context_t self;

static aka_context_t *aka_context()
{
    return &self;
}

void aka_init(aka_conf_entry_t *entry)
{
    while (entry) {
        if (!strcmp(entry->key, "format")) {
        } else if (!strcmp(entry->key, "stdout.level")) {
            aka_context()->stdout_level =
                aka_log_level_string_to_enum(entry->value);
        } else if (!strcmp(entry->key, "file.level")) {
            aka_context()->file_level =
                aka_log_level_string_to_enum(entry->value);
        } else if (!strcmp(entry->key, "file.location")) {
            aka_context()->file_location = entry->value;
        } else {
            /* unknown aka log configuration */
        }
        entry = entry->next;
    }
}

#define HAVE_BACKTRACE 1

AKA_GNUC_NORETURN void aka_abort(void)
{
#if HAVE_BACKTRACE
    int i;
    int nptrs;
    void *buffer[100];
    char **strings;

    nptrs = backtrace(buffer, AKA_ARRAY_SIZE(buffer));
    aka_fatal("backtrace() returned %d addresses", nptrs);

    strings = backtrace_symbols(buffer, nptrs);
    if (strings) {
        for (i = 1; i < nptrs; i++)
            aka_log_print(AKA_LOG_FATAL, "%s\n", strings[i]);

        aka_free(strings);
    }

    abort();
#elif defined(_WIN32)
    DebugBreak();
    abort();
    ExitProcess(127);
#else
    abort();
#endif
}
