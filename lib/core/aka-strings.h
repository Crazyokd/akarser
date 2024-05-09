#ifndef AKA_STRINGS_H_
#define AKA_STRINGS_H_

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* (variable) formatted output to sized buffer */
int aka_vsnprintf(char *str, size_t size, const char *format, va_list ap);
/* (variable) safe limited formatted output */
char *aka_vslprintf(char *str, char *last, const char *format, va_list ap);
char *aka_slprintf(char *str, char *last, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif