.section .text
.global spin_lock
.global spin_release

.type spin_lock, %function
spin_lock:
    // x0 - address
    mov w2, #1          // LOCKED value
    dsb ish
1:
    ldaxr w1, [x0]      // Load
    cbnz w1, 1b         // Locked?
    stxr w1, w2, [x0]   // Try exclusive store of LOCKED value
    cbnz w1, 1b         // Store failed?
    // Locked
    ret
.size spin_lock, . - spin_lock

.type spin_release, %function
spin_release:
    // x0 - address
    dsb ish
1:
    ldaxr w1, [x0]      // Load
    cbz w1, 2f              // Zero?
    stxr w1, wzr, [x0]  // Try exclusive store of zero
    cbnz w1, 1b         // Failed?

    ret
2:
    // Tried to release an already unlocked spin
    b .
.size spin_release, . - spin_release
