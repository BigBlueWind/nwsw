# Makefile for building OSAL library for the Linux
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#
include makecfg
CP := cp -f
RM := rm -f
MAKE := make


TARGET = libturnerlinux.a

#
# Source files
#
CSRC = Src/msgdisp.c  Src/diags.c  Src/icpu.c  Src/icpu2_lnx.c  Src/nulltask.c  Src/osal.c  Src/settings.c

ASRC =

#
# Object files
#
OBJS = $(CSRC:.c=.o) $(ASRC:.s=.o)

#
# Include files
#
INC = -IInc 

#
# Default target
#
all:	$(TARGET) 

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)


#
# Default rule for building object files from source code
#
.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.s.o:
	$(AS) $(ASFLAGS) $(INC) $< -o $@

.PHONY : clean
clean:
	-$(RM) $(OBJS)
	-$(RM) $(TARGET)


