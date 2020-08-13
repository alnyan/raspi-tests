#include <stdint.h>

#include "arch/aarch64/bcm283x/uart.h"
#include "arch/aarch64/bcm283x/mbox.h"
#include "sys/debug.h"

_Noreturn void bcm283x_bsp_entry(void) {
    bcm283x_uart_init();

    kdebug("Hello");

    while (1);
}
