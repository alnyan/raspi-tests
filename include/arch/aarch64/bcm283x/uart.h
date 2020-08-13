#pragma once

struct serial_device;

extern struct serial_device *const bcm283x_uart1;

void bcm283x_uart_init(void);
