#include "arch/aarch64/mem/map.h"
#include "sys/mem/mm.h"
#include "sys/debug.h"
#include <stdint.h>

#define PAGESIZE    4096

// AArch64-specific attributes
#define MM_PAGE_PRESENT         (1 << 0)
#define MM_PAGE_TABLE           (1 << 1)
#define MM_PAGE_ACCESS          (1 << 10)
#define MM_PAGE_ATTR(i)         (((i) & 0xF) << 2)
#define MM_PAGE_ISH             (3 << 8)
#define MM_PAGE_OSH             (2 << 8)

// MMU control registers
// TCR_EL1
#define TCR_TBI1                (1 << 38)
#define TCR_TBI0                (1 << 37)
#define TCR_IPS(n)              (((n) & 0x7) << 32)
#define TCR_TG1_4K              (2 << 30)
#define TCR_SH1_ISH             (3 << 28)
#define TCR_ORGN1_WB            (1 << 26)
#define TCR_IRGN1_WB            (1 << 24)
#define TCR_EPD1                (1 << 23)
#define TCR_TG0_4K              (0 << 14)
#define TCR_SH0_ISH             (3 << 12)
#define TCR_ORGN0_WB            (1 << 10)
#define TCR_IRGN0_WB            (1 << 8)
#define TCR_EPD0                (1 << 7)

// SCTLR_EL1
#define SCTLR_RES               (0xC00800UL)
#define SCTLR_EE                (1 << 25)           // Data access + translation in EL1 is BE
#define SCTLR_E0E               (1 << 24)           // Data access in EL0 is BE
#define SCTLR_WXN               (1 << 19)           // "Write" implies NX
#define SCTLR_I                 (1 << 12)           // Enable instruction cache
#define SCTLR_SA0               (1 << 4)            // SP alignment check for EL0
#define SCTLR_SA                (1 << 3)            // SP alignment check for EL1
#define SCTLR_C                 (1 << 2)            // Enable data cache
#define SCTLR_A                 (1 << 1)            // Alignment check
#define SCTLR_M                 (1 << 0)            // Enable MMU

// ID_AA64MMFR0_EL1
#define ID_AA64MMFR0_PA_MASK    (0xF)               // PARange field mask
#define ID_AA64MMFR0_TGRAN4     (0xFUL << 28)       // 4K granule not supported
#define ID_AA64MMFR0_TGRAN64    (0xFUL << 24)       // 64K granule not supported
#define ID_AA64MMFR0_TGRAN16    (0xFUL << 20)       // 16K granule not supported

// MAIR_EL1
#define MAIR_ATTR(i)            ((i) << 3)          // Each entry is 8 bits
#define MAIR_o_NC               (4 << 4)            // Normal memory, outer NC
#define MAIR_i_NC               (4)                 // Normal memory, inner NC

// (static) upper identity mapping for kernel
uint64_t aarch64_l1_kernel[512] __attribute__((aligned(0x1000)));

int mm_map_single(mm_space_t space,
                  uintptr_t virt,
                  uintptr_t phys,
                  uintptr_t flags,
                  enum page_usage u) {
    (void) space;
    (void) virt;
    (void) phys;
    (void) flags;
    (void) u;
    while (1);
    return -1;
}
