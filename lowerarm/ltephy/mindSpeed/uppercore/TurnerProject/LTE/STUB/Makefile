# Makefile for building dummy MAC library
#
# Copyright 2008,2009,2010 by Mindspeed Technologies
# All rights reserved
#

TARGET = libmac.a

#
# Source files
#
SRC = maciface.c

#
# Object files
#
OBJ = $(SRC:.c=.o)

#
# List files
#
LST = $(SRC:.c=.txt)

#
# Include files
#
INC = \
	-I../..\
	-I../../../Common\
	-I../../../../common\
	-I../../OSAL/$(OS)

#
# Default target
#
all:	$(TARGET)

$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJ)

#
# Default rule for building object files from source code
#
.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

.s.o:
	$(AS) $(ASFLAGS) $(INC) $< -o $@

.PHONY : clean
clean:
	-$(RM) $(OBJ)
	-$(RM) $(LST)
	-$(RM) $(TARGET)
