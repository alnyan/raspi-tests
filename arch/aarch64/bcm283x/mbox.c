#include "arch/aarch64/bcm283x/mbox.h"
#include "sys/debug.h"

volatile uint32_t __attribute__((aligned(16))) bcm283x_mbox[36];

#define VIDEOCORE_MBOX      (0xFFFFFF8000000000 + 0x3F00B880)
#define MBOX_READ           ((volatile uint32_t *) (VIDEOCORE_MBOX + 0x00))
#define MBOX_POLL           ((volatile uint32_t *) (VIDEOCORE_MBOX + 0x10))
#define MBOX_SENDER         ((volatile uint32_t *) (VIDEOCORE_MBOX + 0x14))
#define MBOX_STATUS         ((volatile uint32_t *) (VIDEOCORE_MBOX + 0x18))
#define MBOX_CONFIG         ((volatile uint32_t *) (VIDEOCORE_MBOX + 0x1C))
#define MBOX_WRITE          ((volatile uint32_t *) (VIDEOCORE_MBOX + 0x20))
#define MBOX_RESPONSE       0x80000000
#define MBOX_STATUS_FULL    0x80000000
#define MBOX_STATUS_EMPTY   0x40000000

int mbox_call(uint8_t channel) {
    uint32_t status, value;

    value = ((uint32_t) bcm283x_mbox) | (channel & 0xF);

    do {
        status = *MBOX_STATUS;
    } while (status & MBOX_STATUS_FULL);

    *MBOX_WRITE = value;

    while (1) {
        do {
            asm volatile("nop");
        } while (*MBOX_STATUS & MBOX_STATUS_EMPTY);

        if (value == *MBOX_READ) {
            if (bcm283x_mbox[1] == MBOX_RESPONSE) {
                return 0;
            } else {
                return -1;
            }
        }
    }
}

// Common calls
int mbox_get_arm_memory(size_t *memory) {
    (void) memory;

    bcm283x_mbox[0] = 7 * 4;        // Buffer size + res 8 bytes
    bcm283x_mbox[1] = 0;            // Request: process
    bcm283x_mbox[2] = 0x00010005;   // Get ARM memory
    bcm283x_mbox[3] = 8;            // 8 bytes for response
    bcm283x_mbox[4] = 0;            // Request code: request
    bcm283x_mbox[5] = 0;            // Reserve buffer
    bcm283x_mbox[6] = 0;            // Reserve buffer

    if (mbox_call(8) != 0) {
        return -1;
    } else {
        if (bcm283x_mbox[4] != 0x80000008) {
            return -1;
        }

        *memory = ((size_t) bcm283x_mbox[5] << 32) | bcm283x_mbox[6];
        return 0;
    }
}
