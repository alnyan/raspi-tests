#include <stdint.h>

#include "arch/aarch64/bcm283x/uart.h"
#include "arch/aarch64/bcm283x/mbox.h"
#include "arch/aarch64/mem/mem.h"
#include "sys/debug.h"

_Noreturn void bcm283x_bsp_entry(void) {
    bcm283x_uart_init();
    aarch64_mem_init();

    while (1);
}
