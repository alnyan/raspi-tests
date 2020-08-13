#include "dev/serial.h"
#include "sys/debug.h"
#include "sys/list.h"

static LIST_HEAD(debug_serial_sinks);

void debug_add_serial_sink(struct serial_device *dev) {
    list_add(&dev->debug_sink_link, &debug_serial_sinks);
}

void debug_putc(int ch) {
    struct serial_device *dev;
    list_for_each_entry(dev, &debug_serial_sinks, debug_sink_link) {
        dev->send_byte(dev, ch & 0xFF);
    }
}

void debug_puts(const char *str) {
    char c;
    for (; (c = *str); ++str) {
        debug_putc(c);
    }
}

static int debug_vcprintf_out(void *ctx, const char *data, size_t len) {
    (void) ctx;
    for (; len; ++data, --len) {
        debug_putc(*data);
    }
    return 0;
}

int debug_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int res = vcprintf(NULL, debug_vcprintf_out, fmt, args);
    va_end(args);
    return res;
}
