# Makefile for building PHY emulation library for the LTE eNodeB
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#

TRG = libphystub.a
LTESRCPATH = ../../../LteTestMac
LTEPHYPATH = ../../../PHY/LTE/C-Code

#
# Source files
#
SRC = phystub.c

#
# Object files
#
OBJ = $(SRC:.c=.o)

#
# Additional flags
#
DEFS = -DCEVA_INTRINSICS

#
# Additional includes
#
INC = \
	-I../..\
	-I../../../Common\
	-I../../../../common\
	-I$(LTESRCPATH)\
	-I$(LTESRCPATH)/inc\
	-I../../BSP\
	-I../LTE\
	-I$(LTEPHYPATH)

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
