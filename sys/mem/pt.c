#include "sys/lib/string.h"
#include "sys/mem/pt.h"

#define PT_TRACK_MAX_INDEX      1024

struct pt_track {
    uint64_t data[PT_TRACK_MAX_INDEX];
    uintptr_t base;
    size_t size;
};

static struct pt_track pt_alloc_state;

void pt_alloc_init(uintptr_t start, size_t len) {
    memset(pt_alloc_state.data, 0, PT_TRACK_MAX_INDEX * sizeof(uint64_t));
    pt_alloc_state.base = start;
    pt_alloc_state.size = len >> 12;
}

void *pt_alloc(void) {
    for (size_t i = 0; i < pt_alloc_state.size; ++i) {
        size_t index = i >> 6;
        uint64_t bit = 1UL << (i & 0x3F);

        if (!(pt_alloc_state.data[index] & bit)) {
            pt_alloc_state.data[index] |= bit;
            void *res = (char *) pt_alloc_state.base + (i << 12);
            memset(res, 0, 0x1000);
            return res;
        }
    }
    return NULL;
}
