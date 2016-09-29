# Makefile for building OSAL library for the Linux
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#
include makecfg
CP := cp -f
RM := rm -f
MAKE := make
MKDIR := mkdir -p

LIBDIR = ..//.
API = ../Inc/.


TARGET = testApp

#
# Source files
#
CSRC = testApp.c 

ASRC =

#
# Object files
#
OBJS = $(CSRC:.c=.o) $(ASRC:.s=.o)

#
## C Flags
#
CFLAGS=-DUSE_LINUX -DC4KEVM -DLTE -DSTUB -DUSE_RT -DUSE_GNUPOSIX #-DUSE_POLL


#
# Include files
#
INC = -I$(API)
LIB = -L$(LIBDIR)
LIBS = -lturnerlinux -lpthread -lrt -ldl 

#
# Default target
#
all:	$(TARGET).elf 

$(TARGET).elf: $(OBJS)
	$(LD) $(LIB) -o $(TARGET).elf $(OBJS) $(LIBS)

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
	-$(RM) $(TARGET).elf


