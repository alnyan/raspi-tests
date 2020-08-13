#include "arch/aarch64/bcm283x/mbox.h"

volatile uint32_t __attribute__((aligned(16))) bcm283x_mbox[36];

#define VIDEOCORE_MBOX      (0x3F00B880)
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
