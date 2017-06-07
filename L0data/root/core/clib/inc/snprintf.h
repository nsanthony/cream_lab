/* @(#)root/clib:$Id: snprintf.h 20882 2007-11-19 11:31:26Z rdm $ */
/* Author: Fons Rademakers  10/12/2000 */

/*
   Write formatted text to buffer 'string', using format string 'format'.
   Returns number of characters written, or -1 if truncated.
   Format string is understood as defined in ANSI C.
*/

#ifndef ROOT_snprintf
#define ROOT_snprintf

#ifndef ROOT_RConfig
#include "RConfig.h"
#endif

#ifdef NEED_SNPRINTF

#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DONTNEED_VSNPRINTF
int vsnprintf(char *string, size_t length, const char *format, va_list args);
#endif
int snprintf(char *string, size_t length, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* NEED_SNPRINTF */

#endif /* ROOT_snprintf */
