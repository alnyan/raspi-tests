#pragma once
#include "sys/lib/printf.h"

#define kdebug(fmt, ...)    debug_printf("[%s:%u] " fmt "\n", __FILE__, __LINE__, #__VA_ARGS__)

struct serial_device;

void debug_add_serial_sink(struct serial_device *dev);
void debug_putc(int ch);
void debug_puts(const char *str);

int debug_printf(const char *fmt, ...);
