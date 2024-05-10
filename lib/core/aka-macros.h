#ifndef AKA_MACROS_H_
#define AKA_MACROS_H_

#if defined(_MSC_VER)
#define aka_inline __inline
#else
#define aka_inline __inline__
#endif

/* 与编译器优化相关的宏 */
/* likely/unlikely */
#if __GNUC__ >= 3
#define aka_likely(expr) __builtin_expect(!!(expr), 1)
#define aka_unlikely(expr) __builtin_expect(!!(expr), 0)
#else /* __GNUC__ */
/* #warning "No compiler optimizations supported try gcc 4.x" */
#define aka_likely(expr) (expr)
#define aka_unlikely(expr) (expr)
#endif /* __GNUC__ */

/* for optimize abort */
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

#if defined(_WIN32)
#define AKA_FUNC __FUNCTION__
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L
#define AKA_FUNC __FUNCTION__
#else
#define AKA_FUNC __func__
#endif

#define AKA_ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#endif
