.PHONY: clean all

###
O=build

CC=clang
LD=ld.lld
kernel_CFLAGS=-target aarch64
kernel_LDLFAGS=
kernel_LDS=arch/aarch64/bcm283x/bcm2837.ld
###
OBJS=$(O)/arch/aarch64/bcm283x/entry.S.o
DIRS=$(shell find sys arch dev -type d -printf "$(O)/%p ")
###

all: $(DIRS) $(O)/kernel.img

$(DIRS):
	mkdir -p $@

clean:
	rm -rf $(O)

###

$(O)/kernel.img: $(OBJS) $(kernel_LDS)
	$(LD) $(kernel_LDFLAGS) -o $@ -T$(kernel_LDS) $(OBJS)

$(O)/%.S.o: %.S
	$(CC) $(kernel_CFLAGS) -c -o $@ $<
