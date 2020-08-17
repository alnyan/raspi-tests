#include <stdbool.h>
#include "sys/mem/phys.h"
#include "sys/mem/mm.h"
#include "sys/debug.h"

struct page {
    enum page_usage usage;
    uint32_t refcount;
};

// Stats
static size_t pages_commit = 0;     // Total number of known pages
static size_t pages_avail = 0;      // Total number of memory pages
static size_t pages_alloc = 0;      // Total number of allocated pages

static size_t page_limit = 0;
static uintptr_t first_nonreserved = 0;
// Virtual address
struct page *phys_pages = NULL;

void mm_phys_init(uintptr_t at, size_t len) {
    phys_pages = (struct page *) at;
    page_limit = len;

    // Set all pages to "reserved" state
    for (size_t i = 0; i < len; ++i) {
        phys_pages[i].usage = PU_RESERVED;
    }
}

uintptr_t mm_phys_page_alloc(void) {
    for (size_t i = first_nonreserved >> 12; i < page_limit; ++i) {
        if (phys_pages[i].usage == PU_UNSPEC && phys_pages[i].refcount == 0) {
            ++phys_pages[i].refcount;
            return i << 12;
        }
    }

    return MM_NADDR;
}

void mm_phys_page_free(uintptr_t page) {
    // TODO: check that page refcount is exactly 1
    struct page *pg = GET_PAGE(page);
    if (pg->usage != PU_DEVICE) {
        // Return the page back to "unspecified" state
        pg->usage = PU_UNSPEC;
    }
    pg->refcount = 0;
}

static bool can_commit(uintptr_t page) {
    extern char _kernel_start, _kernel_end;
    uintptr_t kernel_start = ((uintptr_t) &_kernel_start) & ~0xFFF;
    uintptr_t kernel_end = ((uintptr_t) &_kernel_end + 0xFFF) & ~0xFFF;

    uintptr_t track = MM_PHYS(phys_pages) & ~0xFFF;
    uintptr_t track_end = (MM_PHYS(phys_pages) + page_limit * sizeof(struct page) + 0xFFF) & ~0xFFF;

    return !(page >= track && page < track_end) &&
           !(page >= kernel_start && page < kernel_end);
}

// Usage is either PU_DEVICE or PU_UNSPEC
void mm_phys_commit(uintptr_t page, enum page_usage usage) {
    // Check if page is inside phys_pages-reserved region
    if (!can_commit(page)) {
        // Track it as both committed and used
        ++pages_alloc;
        ++pages_commit;
        return;
    }

    struct page *pg = GET_PAGE(page);
    pg->usage = usage;

    if (page < first_nonreserved) {
        first_nonreserved = page;
    }

    if (usage == PU_UNSPEC) {
        ++pages_avail;
    }
    ++pages_commit;
}

void mm_phys_stat(struct phys_stat *st) {
    st->pages_commit = pages_commit;
    st->pages_avail = pages_avail;
    st->pages_alloc = 0;
}
