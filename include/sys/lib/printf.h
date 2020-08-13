#pragma once
#include <stdarg.h>
#include <stddef.h>

typedef int (*vcprintf_func_t) (void *ctx, const char *data, size_t len);

int vcprintf(void *ctx, vcprintf_func_t out, const char *fmt, va_list args);
