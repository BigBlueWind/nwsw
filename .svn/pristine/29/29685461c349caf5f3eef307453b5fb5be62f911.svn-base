# Makefile for building OSAL library for the Linux
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#

TARGE = libosal.a

#
# Source files
#
CSRC = osal.c nulltask.c settings.c

ASRC =

#
# Object files
#
OBJS = $(CSRC:.c=.o) $(ASRC:.s=.o)

#
# Include files
#
INC = -I../.. -I../../../Common -I../../../../common

#
# Default target
#
all:	$(TARGE)

$(TARGE): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGE) $(OBJS)

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
	-$(RM) $(TARGE)
