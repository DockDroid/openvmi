ccflags-y += -I$(src) -Wno-error=implicit-int -Wno-int-conversion
obj-m := ashmem_linux.o
ashmem_linux-y := deps.o ashmem.o

KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build
$(warning "KERNEL_SRC: $(KERNEL_SRC), DESTDIR: $(DESTDIR), MAKE: $(MAKE), ccflags-y: $(ccflags-y)")

all:
	$(MAKE) -C $(KERNEL_SRC) V=0 M=$$PWD

clean:
	rm -rf deps.h *.o *.ko *.mod.c *.symvers *.order .*.cmd .tmp_versions .cache.mk
