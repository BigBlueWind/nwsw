# Makefile for building dummy PHY stub library
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#

TRG = libphystub.a

#
# Source files
#
CSRC = phystub.c 

ASRC =

#
# Object files
#
OBJS = $(CSRC:.c=.o) $(ASRC:.s=.o)

#
# Include files
#
INC = \
	-I../..\
	-I../../../Common\
	-I../../../../common

#
# Default target
#
all:	$(TRG)

$(TRG): $(OBJS)
	$(AR) $(ARFLAGS) $(TRG) $(OBJS)

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
	-$(RM) $(TRG)
