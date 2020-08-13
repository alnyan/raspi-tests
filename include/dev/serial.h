#pragma once
#include <stdint.h>
#include "sys/list.h"

struct serial_device {
    int (*send_byte) (struct serial_device *, uint8_t);

    struct list_head debug_sink_link;
};
