#include <time.h>
#include <sys/time.h>
#include "log.h"
#include "parser.h"


static char *log_content(char *buf, char *last,
        const char *format, va_list ap)
{
    va_list bp;

    va_copy(bp, ap);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
    buf = lol_vslprintf(buf, last, format, bp);
#pragma GCC diagnostic pop
    va_end(bp);

    return buf;
}

static char *log_timestamp(char *buf, char *last,
        int use_color)
{
    struct timeval tv;
    struct tm tm;
    char nowstr[32];

    gettimeofday(&tv, NULL);
    memset(&tm, 0, sizeof(tm));
    (void)localtime_r(&(tv.tv_sec), &tm);
    strftime(nowstr, sizeof nowstr, "%m/%d %H:%M:%S", &tm);

    buf = lol_slprintf(buf, last, "%s%s.%03d%s: ",
            use_color ? FGC_GREEN : "",
            nowstr, (int)(tv.tv_usec/1000),
            use_color ? NOR : "");

    return buf;
}

#if 0
static char *log_domain(char *buf, char *last,
        const char *name, int use_color)
{
    buf = lol_slprintf(buf, last, "[%s%s%s] ",
            use_color ? FGC_YELLOW : "",
            name,
            use_color ? NOR : "");

    return buf;
}
#endif

static const char *level_strings[] = {
    NULL,
    "FATAL", "ERROR", "WARNING", "INFO", "DEBUG", "TRACE",
};

static char *log_level(char *buf, char *last,
        lol_log_level_e level, int use_color)
{
    const char *colors[] = {
        NOR,
        FGC_BOLD_RED, FGC_RED, FGC_BOLD_YELLOW, 
        FGC_BOLD_GREEN, FGC_BOLD_CYAN, FGC_WHITE,
    };

    buf = lol_slprintf(buf, last, "%s%s%s: ",
            use_color ? colors[level] : "",
            level_strings[level],
            use_color ? NOR : "");

    return buf;
}

static char *log_linefeed(char *buf, char *last)
{
    if (buf > last - 2)
        buf = last - 2;

    return lol_slprintf(buf, last, "\n");
}

void lol_log_vprintf(lol_log_level_e level, const char *file, int line,
                     const char *func, int content_only, const char *format,
                     va_list ap)
{
    char logstr[HUGE_LEN];
    char *p, *last;

    int wrote_stderr = 0;

    if (!wrote_stderr)
    {
        int use_color = 0;
#if !defined(_WIN32)
        use_color = 1;
#endif

        p = logstr;
        last = logstr + HUGE_LEN;

        if (!content_only) {
            p = log_timestamp(p, last, use_color);
            p = log_level(p, last, level, use_color);
        }
        p = log_content(p, last, format, ap);
        if (!content_only) {
            p = lol_slprintf(p, last, " (%s:%d)", file, line);
            p = lol_slprintf(p, last, " %s()", func);
            p = log_linefeed(p, last);
        }

        fprintf(stderr, "%s", logstr);
        fflush(stderr);
    }
}


void lol_log_printf(lol_log_level_e level, const char *file, int line,
                    const char *func, int content_only, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    lol_log_vprintf(level, file, line, func, content_only, format, args);
    va_end(args);
}


void lol_init(const char* file) {
    read_yaml_config(file);
    parse_yaml_config();
}
