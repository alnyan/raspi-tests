#pragma once

struct aarch64_smp_ops {
    int (*smp_init) (void);
};

extern struct aarch64_smp_ops *aarch64_smp_ops;

int aarch64_smp_init(void);
void aarch64_smp_init_next(void);
