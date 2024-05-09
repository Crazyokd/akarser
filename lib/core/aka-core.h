/**
 * @file aka-core.h
 * @author Rekord (wildrekord@gmail.com)
 * @date 2024-05-09 06:55
 * 
 * @brief core function
 */

#ifndef AKA_CORE_H
#define AKA_CORE_H

#include <stdlib.h>
#include "aka-macros.h"

/* macros related to memory */
#define aka_malloc(__size) malloc(__size)
#define aka_calloc(__nmemb, __size) calloc(__nmemb, __size)
#define aka_realloc(__ptr, __size) realloc(__ptr, __size)
#define aka_free(__ptr) free(__ptr)

#endif
