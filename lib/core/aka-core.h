#ifndef AKA_CORE_H
#define AKA_CORE_H

#include <stdlib.h>

/* 与编译器优化相关的宏 */
/* likely/unlikely */
#if __GNUC__ >= 3
#define likely(expr) __builtin_expect(!!(expr), 1)
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#else /* __GNUC__ */
/* #warning "No compiler optimizations supported try gcc 4.x" */
#define likely(expr) (expr)
#define unlikely(expr) (expr)
#endif /* __GNUC__ */

#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define AKA_GNUC_NORETURN __attribute__((__noreturn__))
#else
#define AKA_GNUC_NORETURN
#endif

/* add compile check for printf */
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#if !defined(__clang__) \
    && ((__GNUC__ > (4)) || ((__GNUC__ == (4)) && (__GNUC_MINOR__ >= (4))))
#define AKA_GNUC_PRINTF(f, v) __attribute__((format(gnu_printf, f, v)))
#else
#define AKA_GNUC_PRINTF(f, v) __attribute__((format(__printf__, f, v)))
#endif
#else
#define AKA_GNUC_PRINTF(f, v)
#endif


/* macros related to memory */
#define aka_malloc(__size) malloc(__size)
#define aka_calloc(__nmemb, __size) calloc(__nmemb, __size)
#define aka_realloc(__ptr, __size) realloc(__ptr, __size)
#define aka_free(__ptr) free(__ptr)

#endif
