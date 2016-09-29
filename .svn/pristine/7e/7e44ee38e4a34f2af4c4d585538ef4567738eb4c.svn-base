TOP_DIR := $(shell cd .; pwd)
BOARD := pb11mp

KERNEL_SOURCE := ${BSP_DEV_ROOT}/linux-2.6
ROOT_PATH := ${BSP_ROOT_PATH}

#
# Include the make variables (CC, etc...)
#
AS	:= $(CROSS_COMPILE)as
LD	:= $(CROSS_COMPILE)ld
CC	:= $(CROSS_COMPILE)gcc
CPP	:= $(CC) -E
AR	:= $(CROSS_COMPILE)ar
NM	:= $(CROSS_COMPILE)nm
STRIP	:= $(CROSS_COMPILE)strip
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
RANLIB	:= $(CROSS_COMPILE)ranlib

all:	armldr_mod

export	FSROOT HOSTCC HOSTCFLAGS CROSS_COMPILE AS LD CC CPP AR NM \
	STRIP OBJCOPY OBJDUMP MAKE CFLAGS EXTRA_CFLAGS LIBRARY

# Kernel module build rules
#
obj-m += armldr.o
armldr-objs := armboot.o ipi.o

armldr_mod:		
	$(MAKE) KBUILD_VERBOSE=1 -C $(KERNEL_SOURCE) SUBDIRS=$(TOP_DIR) modules

clean:
	$(MAKE) KBUILD_VERBOSE=1 -C $(KERNEL_SOURCE) SUBDIRS=$(TOP_DIR) clean
	$(RM) $(PROGS)
	$(RM) $(ROOT_PATH)/sbin/bootarm

install:
	$(MAKE) KBUILD_VERBOSE=1 -C $(KERNEL_SOURCE) SUBDIRS=$(TOP_DIR) INSTALL_MOD_PATH=$(ROOT_PATH) modules_install
	cp -f bootarm $(ROOT_PATH)/sbin

