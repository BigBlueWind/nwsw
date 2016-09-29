# Makefile for building OSAL library for bare metal access
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#

TARGET = libosal.a

#
# Source files
#
CSRC = osal.c nulltask.c settings.c

ASRC =

#
# Object files
#
OBJS = $(CSRC:.c=.o)

#
# List files
#
LST = $(CSRC:.c=.txt)

#
# Include files
#
INC = \
	-I../..\
	-I../../../Common\
	-I../../BSP\

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)

#
# Default rule for building object files from source code
#
.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.S.o:
	$(AS) $(ASFLAGS) $(INC) $< -o $@

.PHONY : clean
clean:
	-$(RM) $(OBJS)
	-$(RM) $(LST)
	-$(RM) $(TARGET)
