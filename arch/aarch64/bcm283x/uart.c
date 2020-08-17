#include "sys/mem/mm.h"
#include "dev/serial.h"
#include "sys/debug.h"
#include "sys/list.h"

static int bcm283x_muart_send(struct serial_device *dev, uint8_t ch) {
    (void) dev;

    *((volatile uint32_t *) MM_VIRTUALIZE(0x3F215040)) = (uint32_t) ch;

    return 0;
}

static struct serial_device _bcm283x_uart1 = {
    .send_byte = bcm283x_muart_send
};
struct serial_device *const bcm283x_uart1 = &_bcm283x_uart1;

void bcm283x_uart_init(void) {
    list_head_init(&_bcm283x_uart1.debug_sink_link);

    debug_add_serial_sink(bcm283x_uart1);
}
