.PHONY: clean all

###
O=build

CC=clang
LD=ld.lld
kernel_CFLAGS=-target aarch64 \
			  -mcpu=cortex-a53 \
			  -DARCH_AARCH64 \
			  -mgeneral-regs-only \
			  -Iinclude \
			  -ggdb \
			  -ffreestanding \
			  -Werror \
			  -Wextra \
			  -Wall \
			  -Wpedantic \
			  -Wno-gnu-statement-expression \
			  -Wno-language-extension-token \
			  -Wno-gnu-zero-variadic-macro-arguments \
			  -Wno-gnu-binary-literal
kernel_LDFLAGS=-O0
kernel_LDS=arch/aarch64/bcm283x/bcm2837.ld
###
DIRS=$(shell find sys arch dev -type d -printf "$(O)/%p ")
OBJS=$(O)/arch/aarch64/bcm283x/entry.S.o \
	 $(O)/arch/aarch64/bcm283x/ap_entry.S.o \
	 $(O)/arch/aarch64/bcm283x/entry.c.o \
	 $(O)/arch/aarch64/bcm283x/uart.c.o \
	 $(O)/arch/aarch64/bcm283x/mbox.c.o \
	 $(O)/arch/aarch64/mem/map.c.o \
	 $(O)/arch/aarch64/mem/mem.c.o \
	 $(O)/arch/aarch64/mem/early.S.o \
	 $(O)/arch/aarch64/exc.c.o \
	 $(O)/arch/aarch64/smp.c.o \
	 $(O)/arch/aarch64/cpu.c.o \
	 $(O)/arch/aarch64/spin.S.o \
	 $(O)/sys/kernel.c.o \
	 $(O)/sys/mem/pt.c.o \
	 $(O)/sys/mem/phys.c.o \
	 $(O)/sys/lib/printf.c.o \
	 $(O)/sys/lib/string.c.o \
	 $(O)/sys/debug.c.o
HDRS=$(shell find include -type f -name "*.h")
###

all: $(DIRS) $(O)/kernel.img

$(DIRS):
	mkdir -p $@

clean:
	rm -rf $(O)

###

$(O)/kernel.img: $(OBJS) $(kernel_LDS)
	$(LD) $(kernel_LDFLAGS) -o $@ -T$(kernel_LDS) $(OBJS)

$(O)/%.S.o: %.S $(HDRS)
	$(CC) $(kernel_CFLAGS) -c -o $@ $<

$(O)/%.c.o: %.c $(HDRS)
	$(CC) $(kernel_CFLAGS) -c -o $@ $<
