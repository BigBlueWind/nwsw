# Makefile for building LTE eNodeB (RCT) library
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#

TRG = libmac.a

#
# Source files
#
SRC = maciface.c \

#
# Object files
#
OBJ = $(SRC:.c=.o)

#
# Additional flags
#
DEFS = 

#
# Additional includes
#
INC = \
	-I../..\
	-I../../../T4KDev/SYSCORE/SERVICES\
	-I../../../Common\
	-I../../BSP\
	-I../../BSP/Linux\

#
# Default target
#
all:	$(TRG)

$(TRG): $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

#
# Default rule for building object files from source code
#
.c.o:
	$(CC) $(CFLAGS) $(DEFS) $(INC) -c $< -o $@

.PHONY : clean
clean:
	-$(RM) $(OBJ)
	-$(RM) $(TRG)
