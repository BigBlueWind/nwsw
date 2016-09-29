# Makefile for building SCTP library for the Linux (lksctp)
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#

TRG = libsctp.a

#
# Source files
#
CSRC = addrs.c bindx.c connectx.c opt_info.c peeloff.c recvmsg.c sendmsg.c

ASRC =

#
# Object files
#
OBJS = $(CSRC:.c=.o) $(ASRC:.s=.o)

#
# Include files
#
INC = -I.. -I../..

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
