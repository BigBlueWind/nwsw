# Makefile for building Turner firmware using RVDS 3.x / 4.x, GNU GCC
#
# Copyright 2008,2009,2010 by Mindspeed Technologies
# All rights reserved
#

#
# Application specific
#
# OS can be one of: LINUX, BARE, RTXC
# BOARD must be one of: C4KEVM, C4KFPGA, SIMUPC, M823XX
# TOOLCHAIN can be one of: RVDS, CS, GCC
# MACTYPE can be one of: BSMAC, MSMAC, EHDMAC, ENODEB, UE, STUB
# FOURG can be one of: WIMAX, LTE, LTE-RCT
#
TARGET = C4KFW
BOARD = C4KEVM
OS = LINUX
TOOLCHAIN = CS
MACTYPE = STUB
FOURG = LTE-RCT
ADDDEF = 

include config.mk 
include makelib.mk

#
# Configuration specific settings
#
include CONFIG-$(OS)-$(TOOLCHAIN)

DEFS = $(DEF) $(ADDDEF)

export CC
export AS
export LD
export AR
export DEFS
export INC
export OPT
export CFLAGS
export ASFLAGS
export LIBS
export LDFLAGS
export ARFLAGS
export OS

APP = c4k$(call lowercase,$(MACTYPE))

#
# Source files
#
CSRC =	ctrlmsg.c \
	console.c \
	diags.c \
	main.c \
	startup.c \
	spusched.c \
	svsr.c \
	phyapi.c

ASRC =

#
# Object files
#
OBJS = $(CSRC:.c=.o) $(ASRC:.s=.o)

#
# List files
#
LST = $(CSRC:.c=.txt)

#
# Default target
#
all:	$(APP).elf

$(APP).elf:	$(ARM_LINUX_CONFIG_FILE) $(OBJS) BSP/$(OS)/libbsp.a OSAL/$(OS)/libosal.a $(FOURG)/$(MACTYPE)/libmac.a PHYSTUB/$(MACTYPE)/libphystub.a
	$(LD) $(OBJS) $(LIBS) $(LDFLAGS) -o $(APP).elf

BSP/$(OS)/libbsp.a:
	$(MAKE) -f Makefile -C BSP/$(OS)

OSAL/$(OS)/libosal.a:
	$(MAKE) -f Makefile -C OSAL/$(OS)

$(FOURG)/$(MACTYPE)/libmac.a:
	$(MAKE) -f Makefile -C $(FOURG)/$(MACTYPE)

PHYSTUB/$(MACTYPE)/libphystub.a:
	$(MAKE) -f Makefile -C PHYSTUB/$(MACTYPE)

ifneq ($(ARM_LINUX_CONFIG_FILE),)
$(ARM_LINUX_CONFIG_FILE):
	-if not exist $@ armcc --arm_linux_configure --arm_linux_config_file=$@
endif

#
# Default rule for building object files from source code
#
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.s.o:
	$(AS) $(ASFLAGS) $< -o $@

.S.o:
	$(AS) $(ASFLAGS) $< -o $@

#
# Build Framework for RTXC
#
.PHONY : rtxc
rtxc:
	$(MAKE) "OS=RTXC" "TOOLCHAIN=RVDS" "BOARD=M823XX"
	
#
# Build Framework for Linux using CodeSourcery toolchain
#
.PHONY : linux
linux:
	$(MAKE) "OS=LINUX" "TOOLCHAIN=CS" "BOARD=C4KEVM"

#
# Build Framework for Linux using RVDS toolchain
#
.PHONY : linux-rvds
linux-rvds:
	$(MAKE) "OS=LINUX" "TOOLCHAIN=RVDS" "BOARD=C4KEVM"

#
# Build default target for C4K FPGA
#
.PHONY : c4kfpga
c4kfpga:
	$(MAKE) "OS=LINUX" "TOOLCHAIN=CS" "BOARD=C4KFPGA"

#
# Build default target for C4K EVM
#
.PHONY : c4k
c4k:
	$(MAKE) "OS=LINUX" "TOOLCHAIN=CS" "BOARD=C4KEVM"

#
# Build default target for BS MAC (WiMAX)
#
.PHONY : bsmac
bsmac:
	$(MAKE) "OS=LINUX" "TOOLCHAIN=CS" "BOARD=C4KEVM" "FOURG=WIMAX" "MACTYPE=BSMAC"

.PHONY : enodeb
enodeb:
	$(MAKE) "OS=LINUX" "TOOLCHAIN=CS" "BOARD=C4KEVM" "FOURG=LTE-RCT" "MACTYPE=ENODEB"

.PHONY : enodebrt
enodebrt:
	$(MAKE) "OS=LINUX" "TOOLCHAIN=CS" "BOARD=C4KEVM" "FOURG=LTE-RCT" "MACTYPE=ENODEB" "ADDDEF="-DMAC_RT""

.PHONY : ehdmac
ehdmac:
	$(MAKE) "OS=LINUX" "TOOLCHAIN=CS" "BOARD=C4KEVM" "FOURG=LTE-RCT" "MACTYPE=EHDMAC"

.PHONY : stub
stub:
	$(MAKE) "OS=LINUX" "TOOLCHAIN=CS" "BOARD=C4KEVM" "FOURG=LTE-RCT" "MACTYPE=STUB"

.PHONY : stubgcc
stubgcc:
	$(MAKE) "OS=LINUX" "TOOLCHAIN=GCC" "BOARD=C4KEVM" "FOURG=LTE-RCT" "MACTYPE=STUB"

#
# Configure RVDS for Linux
#
.PHONY : config
config:
	$(CC) --arm_linux_configure --arm_linux_config_file=$(ARM_LINUX_CONFIG_FILE)
	
#
# Clean up the project
#
.PHONY : clean
clean:
	-$(RM) $(OBJS)
	-$(RM) $(LST)
	-$(RM) $(APP).lst
	-$(RM) $(APP).sym
	-$(RM) $(APP).elf
	-$(MAKE) -f Makefile -C BSP/$(OS) clean
	-$(MAKE) -f Makefile -C OSAL/$(OS) clean
	-$(MAKE) -f Makefile -C $(FOURG)/$(MACTYPE) clean
	-$(MAKE) -f Makefile -C PHYSTUB/$(MACTYPE) clean

#
# Clean everything
#
.PHONY : xclean
xclean: clean
	"$(MAKE)" OS=BARE TOOLCHAIN=RVDS BOARD=M823XX clean
	"$(MAKE)" OS=BARE TOOLCHAIN=RVDS BOARD=C4KEVM clean
	"$(MAKE)" OS=RTXC TOOLCHAIN=RVDS BOARD=M823XX clean
	"$(MAKE)" clean
	"$(MAKE)" MACTYPE=BSMAC clean
	"$(MAKE)" MACTYPE=MSMAC clean
	"$(MAKE)" FOURG=LTE-RCT MACTYPE=ENODEB clean
	"$(MAKE)" FOURG=LTE-RCT MACTYPE=UE clean
	"$(MAKE)" FOURG=LTE-RCT MACTYPE=STUB clean
	