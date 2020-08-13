#!/bin/sh

set -e

QEMU_OPTS="-M raspi3 \
           -kernel build/kernel.img \
           -serial null \
           -serial stdio \
           -display none"

if [ "${QEMU_DEBUG}x" != x ]; then
    QEMU_OPTS="${QEMU_OPTS} -s -S"
fi

make ${MAKEOPTS}
qemu-system-aarch64 ${QEMU_OPTS}
