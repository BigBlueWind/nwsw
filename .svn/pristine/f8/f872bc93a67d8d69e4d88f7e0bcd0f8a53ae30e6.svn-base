# Makefile for building BSP library for bare metal access
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#

TARGET = libbsp.a

#
# Source files
#
CSRC = \
	auxiface.c \
	mmu.c \
	isr.c \
	intc.c \
	clkcore.c \
	arbiter.c \
	gpio.c \
	uexp.c \
	memlimits.c \
	gemac_hal.c \
	idma.c \
	icpu.c \
	bsp.c \
	phi.c \
	gemac.c \
	uart.c \
	timers.c \
	hostiface.c \
	protocol.c

ASRC = \
	../$(TOOLCHAIN)/init.S \
	../$(TOOLCHAIN)/vectors.S \
	../$(TOOLCHAIN)/vectorsl.S

#
# Object files
#
OBJS = $(CSRC:.c=.o) $(ASRC:.S=.o)

#
# List files
#
LST = $(CSRC:.c=.txt)

#
# Include files
#
INC = \
	-I..\
	-I../..\
	-I../../../Common

#
# Targets & rules start here. Default target
#
all:	$(TARGET)

$(TARGET):	$(OBJS) Makefile
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

#
# Default rule for building object files from source code
#
.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.S.o:
	$(AS) $(ASFLAGS) $(INC) $< -o $@

#
# Clean up the project
#
clean:
	-$(RM) $(OBJS)
	-$(RM) $(LST)
	-$(RM) $(TARGET)
