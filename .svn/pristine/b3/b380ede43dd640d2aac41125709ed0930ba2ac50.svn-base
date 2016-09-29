# Makefile for building BSP library for the Linux
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#

TARGE := libbsp.a

#
# Source files
#
CSRC = hostiface.c protocol.c icpu.c icpu2_lnx.c bsp.c phi.c gemac.c uart.c timer.c packetif.c tinyarp.c auxiface.c

ASRC =

#
# Object files
#
OBJS = $(CSRC:.c=.o) $(ASRC:.s=.o)

#
# Include files
#
INC = -I.. -I../.. -I../../../Common -I../../../../common

#
# Libraries
#
LIBS = libsctp/libsctp.a

#
# Default target
#
all:	$(TARGE) $(LIBS)

$(TARGE): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

libsctp/libsctp.a:
	$(MAKE) -f Makefile -C libsctp

#
# Default rule for building object files from source code
#
.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.s.o:
	$(AS) $(ASFLAGS) $(INC) $< -o $@

.PHONY : clean
clean:
	-$(MAKE) -f Makefile -C libsctp clean
	-$(RM) $(OBJS)
	-$(RM) $(TARGE)
