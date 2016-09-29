# Makefile for building BSP library for RTXC
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#

TRG = libbsp.a

#
# Source files
#
CSRC = bsp.c gemac.c icpu.c phi.c timers.c uart.c hostiface.c \
	../BARE/gemac_hal.c ../BARE/mmu.c ../BARE/clkcore.c ../BARE/isr.c \
	../BARE/idma.c ../BARE/intc.c ../BARE/auxiface.c

ASRC = ../$(TOOLCHAIN)/init.S irqhdlr.S

#
# Object files
#
OBJS = $(CSRC:.c=.o) $(ASRC:.S=.o)

#
# List files
#
LST = $(CSRC:.c=.txt)

#
# Additional includes
#
INC = -I.. -I../BARE -I../.. -I../../OSAL/$(OS)

#
# Default target
#
all:	$(TRG)

$(TRG):	$(OBJS) Makefile
	$(AR) $(ARFLAGS) $(TRG) $(OBJS)

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
	-$(RM) $(TRG)
