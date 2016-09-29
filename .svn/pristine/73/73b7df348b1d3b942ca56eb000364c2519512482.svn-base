# Makefile for building LTE eNodeB (RCT) library
#
# Copyright 2008,2009 by Mindspeed Technologies
# All rights reserved
#

TRG = libmac.a

LTESRCPATH = ../../../LteTestMac
RRCPATH = ../../RRC/$(MACTYPE)
RLCPATH = ../../RLC
LTEPHYPATH = ../../../PHY/LTE/C-Code

#
# Source files
#
SRC = maciface.c \
	$(LTESRCPATH)/ltemac.c \
	$(LTESRCPATH)/bsmac.c \
	$(LTESRCPATH)/macMemUtil.c \
	$(LTESRCPATH)/simu_util.c \
	$(LTESRCPATH)/table.c \
	$(LTESRCPATH)/apihdlr.c \
	$(LTESRCPATH)/fdxmac.c \
	$(LTESRCPATH)/LteMac2PhyInterface.c \
	$(LTESRCPATH)/LteMac2RrcInterface.c \
	$(LTESRCPATH)/LteBsMacDlScheduler.c \
	$(LTESRCPATH)/LteBsMacUlScheduler.c \
	$(LTESRCPATH)/LteMacUeContext.c \
	$(LTESRCPATH)/LteBsMacDlMux.c \
	$(LTESRCPATH)/LteBsMacPdcchMgr.c \
	$(LTESRCPATH)/LteBsMacCcch.c \
	$(LTESRCPATH)/LteBsMacPdschMgr.c \
	$(LTESRCPATH)/LteMacTestCfgMgr.c \
	$(RRCPATH)/rrc_main.c \
	$(RRCPATH)/rrciface.c \
	$(RLCPATH)/LteMacRlcInterface.c \
	$(LTEPHYPATH)/LTE_test_config.c \
	RrcMacParse.c 
	

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
	-I../../../T4KDev/SYSCORE/SERVICES\
	-I../../../Common\
	-I../../BSP\
	-I../../BSP/Linux\
	-I$(LTESRCPATH)\
	-I$(LTESRCPATH)/inc\
	-I$(RRCPATH)\
	-I$(RLCPATH)\
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
