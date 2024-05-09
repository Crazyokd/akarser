#include <stdio.h>

#include "aka-strings.h"

/* the size parameter include trailing '\0' */
int aka_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    int r = -1;
    r = vsnprintf(str, size, format, ap);
    str[size-1] = '\0';

    return r;
}

char *aka_vslprintf(char *str, char *last, const char *format, va_list ap)
{
    int r = -1;

    if (!str)
        return NULL;

    if (str < last)
        r = aka_vsnprintf(str, last - str, format, ap);

    return (str + r);
}

char *aka_slprintf(char *str, char *last, const char *format, ...)
{
    char *res;
    va_list ap;

    va_start(ap, format);
    {
    int r = -1;
    if (!str)
        return NULL;
    if (str < last)
        r = aka_vsnprintf(str, last - str, format, ap);
    res = (str + r);
    }
    va_end(ap);

    return res;
}
