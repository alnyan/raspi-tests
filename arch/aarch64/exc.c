#include <stdint.h>

#include "sys/debug.h"

_Noreturn void aarch64_exc_handler(uintptr_t kind, uintptr_t esr, uintptr_t elr, uintptr_t spsr, uintptr_t far) {
    kdebug("Exception kind: %02x", kind);
    kdebug("Exception: %p, %p, %p, %p", esr, elr, spsr, far);
    while (1);
}
