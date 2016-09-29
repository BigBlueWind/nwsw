#********************************************************************16**
#
#       (c) COPYRIGHT 1989-1997 by Trillium Digital Systems, Inc.
#                         All rights reserved.
#
#    This software is confidential and proprietary to Trillium
#    Digital Systems, Inc.  No part of this software may be reproduced,
#    stored, transmitted, disclosed or used in any form or by any means
#    other than as expressly provided by the written license agreement
#    between Trillium and its licensee.
#
#    Trillium warrants that for a period, as provided by the written
#    license agreement between Trillium and its licensee, this
#    software will perform substantially to Trillium specifications as
#    published at the time of shipment and the media used for delivery
#    of this software will be free from defects in materials and
#    workmanship.
#
#    TRILLIUM MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
#    WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
#    A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE OR ANY RELATED
#    MATERIALS.
#
#    IN NO EVENT SHALL TRILLIUM BE LIABLE FOR ANY INDIRECT, SPECIAL,
#    OR CONSEQUENTIAL DAMAGES IN CONNECTION WITH OR ARISING OUT OF
#    THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
#    ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
#    LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
#    OF THE POSSIBILITY OF SUCH DAMAGE.
#
#                      Restricted Rights Legend
#
#    This software and all related materials licensed hereby are
#    classified as "restricted computer software" as defined in clause
#    52.227-19 of the Federal Acquisition Regulation ("FAR") and were
#    developed entirely at private expense for nongovernmental purposes,
#    are commercial in nature and have been regularly used for
#    nongovernmental purposes, and, to the extent not published and
#    copyrighted, are trade secrets and confidential and are provided
#    with all rights reserved under the copyright laws of the United
#    States.  The Government's rights to the software and related
#    materials are limited and restricted as provided in clause
#    52.227-19 of the FAR.Œ.
#
#                   IMPORTANT LIMITATION(S) ON USE
#
#    The use of this software is limited to the Use set
#    forth in the written License Agreement between Trillium and
#    its Licensee. Among other things, the Use of this software
#    may be limited to a particular type of Designated Equipment.
#    Before any installation, use or transfer of this software, please
#    consult the written License Agreement or contact Trillium at
#    the location set forth below in order to confirm that you are
#    engaging in a permissible Use of the software.
#
#                   Trillium Digital Systems, Inc.
#                 12100 Wilshire Boulevard, suite 1800
#                   Los Angeles, CA 90025-7118, USA
#
#                       Tel: +1 (310) 442-9222
#                       Fax: +1 (310) 442-1162
#
#                  Email: tech_support@trillium.com
#                    Web: http://www.trillium.com
#
#********************************************************************17*/
#
#********************************************************************20**
#
#       Name:   MTSS - acceptance test
#
#       Type:   Makefile
#
#       Desc:   Build the System Services acceptance test with
#               MTSS-Solaris.
#
#               The default setup of this file is for UNIX  
#
#               For UNIX make:
#
#               1) ensure DOS Microsoft 5.1 link is commented out
#               2) ensure DOS Microsoft 6.0 link is commented out
#               3) ensure UNIX link is not commented out
#               4) ensure all	$(CC)'s are preceded by tab
#               5) ensure macros are set correctly for UNIX
#
#               Note:
#
#               The full path for .h files has to be specified
#               in INC_DIR macro
#
#       File:   mt_acc.mak
#
#       Sid:      mt_acc.mak@@/main/2 - Mon Apr  5 18:19:17 2010
#
#       Prg:    ada
#  
#********************************************************************21*/
#
#-----------------------------------------------------------------------
#

SSOPTS= -DCMFILE_REORG_1 -USTDIO_INCLD 
MTOPTS= -DCMFILE_REORG_1 -USTDIO_INCLD 
ACOPTS= -DCMFILE_REORG_1 -USTDIO_INCLD 
CMOPTS= -DCMFILE_REORG_1 -USTDIO_INCLD 

#-----------------------------------------------------------------------
# macro for output file name and makefile name
#

OUTNAME=mt_acc
MAKENAME=mt_acc.mak

#-----------------------------------------------------------------------
# macro for include directories
#

SY_DIR=.
SS_DIR=.
CM_DIR=.
MT_DIR=.
AC_DIR=.
INC_DIR=-I. -I$(CM_DIR) -I$(SS_DIR)  # include directory
#Enable this Macro for C++ compilation.
#INC_DIR=-I. -I$(CM_DIR) -I$(SS_DIR) -I/usr/include/nptl   # include directory

#-----------------------------------------------------------------------
# macros for compile and link options
#
  
TRACE=                                   # trace
LIST=                                    # listing

# environment:
# -DMSC60    microsoft c 6.0
# -DMSC51    microsoft c 5.1
# -DMSC70    microsoft c 7.0
# -DBOR31    borland c++ 3.1
# -DANSI     ansi
#
#            dos
# -DUNIX     unix
# -DSUNOS    sunos

#ENV=-DANSI -DWIN32                       # environment, microsoft
ENV=-DANSI -DSUNOS -D_REENTRANT -DSS_MT -DSS_LINUX -DLONG_MSG -DCMFILE_REORG_2  -DSS_MULTICORE_SUPPORT -DSS_M_PROTO_REGION -DDEBUGNOEXIT -DSLES9_PLUS -DSS_DRVR_SUPPORT -DSS_SINGLE_THREADED   # environment, sunos
#ENV=-DANSI -DSUNOS -D_REENTRANT -DSS_MT -DSS_MULTICORE_SUPPORT -DSS_M_PROTO_REGION -DDEBUGNOEXIT \
    -USS_DRVR_SUPPORT -USS_RTR_SUPPORT -DSS_DRVR_SUPPORT -DSS_SINGLE_THREADED   # environment, sunos

# lint:

LINT=lin.bat                             # dos batch command

# lint options:

LNTOPTS=-dDSTDIO_INCLD -dCONSTDIO          # lint option (same as MTOPTS)
LNTENV=-dMSC60 -dANSI -dDOS                # lint environment (same as ENV-DSS_DRVR_SUPPORT -DSS_SINGLE_THREADED)
LNTOPTION=$(LNTOPTS) $(LNTENV) -i$(CM_DIR)

# compiler:
# cl    DOS
# cc    UNIX

#CC=cl                                    # compiler, microsoft
#CC=cc                                    # compiler, sunos
CC=gcc                                   # compiler, gnu

# PURIIFY
PURE=
#PURE=/usr/ccpu/rational/releases/purecov.i386_linux2.7.0/purecov
#PURE=/opt/pure/purify/purify
 
# object files:
# obj   DOS
# o     UNIX

#OBJ=obj                                  # object files, microsoft
OBJ=o                                    # object files, sunos

# memory model:
# -AS   small     UNIX
# -AM   medium
# -AC   compact
# -AL   large
# -AH   huge  

#MODEL=                                  # memory model, microsoft
MODEL=                                   # memory model, sunos

# compiler options: 
#
# microsoft
#
# -D    define
# -W3   warning level 3
# -Od   optimization disabled
# -Za   disable language extensions
# -Zi   required for Code View
# -c    compile without linking
# -I    include path
# -Gs   disable stack check
# -EM   extended memory manager
#
# sunos
#
# -g    debugger
# -fast optimize for speed
# -Xa   conforming ANSI mode
# -Xc   maximally conforming ANSI mode

#COPTS=-Od -W3                            # compiler options, microsoft
#COPTS=-g3 -traditional -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
#      -Wconversion
                                          # GNU K&R compiler options
#COPTS=-g3 -Wall -Wno-comment -pipe -Wshadow -Wcast-qual \
      -Wstrict-prototypes -Wmissing-prototypes
                                          # GNU ansi compiler options
#COPTS=-Xa -g -mt                          # compiler options, solaris
COPTS= -g -ansi

# linker options:
# /CO   required for Code View 
# /NOI  preserve case sensitivity
# /ST:<val>  stack siz

#LOPTS=/CO /NOI /NOE /ST:25000            # linker options
#LOPTS=-lpthread -lposix4
LOPTS=-L/usr/lib/nptl -lpthread -lrt 

CMTOPTS=$(TRACE) $(MODEL) $(MTOPTS) $(ENV) $(COPTS) $(INC_DIR)
CACOPTS=$(TRACE) $(MODEL) $(ACOPTS) $(ENV) $(COPTS) $(INC_DIR)
CCMOPTS=$(TRACE) $(MODEL) $(CMOPTS) $(ENV) $(COPTS) $(INC_DIR)
CSSOPTS=$(TRACE) $(MODEL) $(SSOPTS) $(ENV) $(COPTS) $(INC_DIR)
LOPTION=$(LOPTS)

#-----------------------------------------------------------------------
# macros for include files
#

ENV_INC=$(CM_DIR)/envdep.h $(CM_DIR)/envind.h $(CM_DIR)/envopt.h   # environment
SSI_INC=$(CM_DIR)/ssi.h $(CM_DIR)/ssi.x                     # system services
GEN_INC=$(CM_DIR)/gen.h $(CM_DIR)/gen.x                     # general layer
SS_INC=$(SS_DIR)/ss_dep.h $(SS_DIR)/ss_dep.x $(SS_DIR)/ss_err.h $(SS_DIR)/ss_drvr.x $(SS_DIR)/ss_gen.h $(SS_DIR)/ss_gen.x $(SS_DIR)/ss_mem.h $(SS_DIR)/ss_mem.x $(SS_DIR)/ss_msg.h $(SS_DIR)/ss_msg.x $(SS_DIR)/ss_queue.h $(SS_DIR)/ss_queue.x $(SS_DIR)/ss_strm.h $(SS_DIR)/ss_strm.x $(SS_SIR)/ss_task.h $(SS_DIR)/ss_task.x $(SS_DIR)/ss_timer.x                                        # common SS
MT_INC=$(MT_DIR)/mt_ss.h $(MT_DIR)/mt_ss.x $(MT_DIR)/mt_err.h      # MTSS

#-----------------------------------------------------------------------
# macro for object files
#

SRCS= \
        $(SS_DIR)/ss_gen.c \
        $(SS_DIR)/ss_task.c \
        $(SS_DIR)/ss_drvr.c \
        $(SS_DIR)/ss_timer.c \
        $(SS_DIR)/ss_mem.c \
        $(SS_DIR)/ss_strm.c \
        $(SS_DIR)/ss_msg.c \
        $(SS_DIR)/ss_queue.c \
        $(SS_DIR)/ss_pack.c \
        $(SS_DIR)/ss_rtr.c \
        $(SS_DIR)/ss_acc.c \
	$(CM_DIR)/cm_mem.c \
	$(CM_DIR)/cm_bdy5.c \
        $(MT_DIR)/mt_ss.c \
	$(MT_DIR)/mt_id.c

OBJS=  \
        $(SY_DIR)/ss_gen.$(OBJ) \
        $(SY_DIR)/ss_task.$(OBJ) \
        $(SY_DIR)/ss_drvr.$(OBJ) \
        $(SY_DIR)/ss_timer.$(OBJ) \
        $(SY_DIR)/ss_mem.$(OBJ) \
        $(SY_DIR)/ss_strm.$(OBJ) \
        $(SY_DIR)/ss_msg.$(OBJ) \
        $(SY_DIR)/ss_queue.$(OBJ) \
        $(SY_DIR)/ss_pack.$(OBJ) \
        $(SY_DIR)/ss_rtr.$(OBJ) \
        $(SY_DIR)/ss_acc.$(OBJ) \
	$(SY_DIR)/cm_mem.$(OBJ) \
	$(SY_DIR)/cm_bdy5.$(OBJ) \
	$(SY_DIR)/mt_ss.$(OBJ) \
	$(SY_DIR)/mt_id.$(OBJ)

#-----------------------------------------------------------------------
# UNIX link - absolute object file depends on relocatable object
# (.o) files
#

$(OUTNAME): $(OBJS)
	$(PURE)	$(CC) -o $(OUTNAME) $(OBJS) $(LOPTS)

depend: $(SRCS)
	makedepend -f $(MAKENAME) -- $(ENV) $(MTOPTS) $(CMOPTS) $(INC_DIR) -- $(SRCS)

clean:
	-/bin/rm -f $(OUTNAME) *.o *.i

#-----------------------------------------------------------------------
# lint
#

#lint:
# $(LINT) $(LNTOPTION) $(OUTNAME).lnt > $(OUTNAME).ler

#-----------------------------------------------------------------------
# DOS Microsoft 6.0 link - absolute object (.exe) file depends on
# relocatable object (.obj) files
#

#$(OUTNAME).exe: $(OBJS) $(OUTNAME).lnk
# link $(LOPTION) @$(OUTNAME).lnk
   
#-----------------------------------------------------------------------
# relocatable object (.obj) files depend on respective source (.c) and
# include (.h and .x) files
#
# note: if an include file includes another include file then the second
# include file must also be shown on the dependency list
#

$(SY_DIR)/ss_gen.$(OBJ): $(SS_DIR)/ss_gen.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_gen.c

$(SY_DIR)/ss_task.$(OBJ): $(SS_DIR)/ss_task.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_task.c

$(SY_DIR)/ss_drvr.$(OBJ): $(SS_DIR)/ss_drvr.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_drvr.c

$(SY_DIR)/ss_timer.$(OBJ): $(SS_DIR)/ss_timer.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_timer.c

$(SY_DIR)/ss_mem.$(OBJ): $(SS_DIR)/ss_mem.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_mem.c

$(SY_DIR)/ss_strm.$(OBJ): $(SS_DIR)/ss_strm.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_strm.c

$(SY_DIR)/ss_msg.$(OBJ): $(SS_DIR)/ss_msg.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_msg.c

$(SY_DIR)/ss_queue.$(OBJ): $(SS_DIR)/ss_queue.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_queue.c

$(SY_DIR)/ss_pack.$(OBJ): $(SS_DIR)/ss_pack.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_pack.c

$(SY_DIR)/ss_rtr.$(OBJ): $(SS_DIR)/ss_rtr.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_rtr.c

$(SY_DIR)/ss_acc.$(OBJ): $(SS_DIR)/ss_acc.c
	$(PURE) $(CC) -c $(CSSOPTS) $(SS_DIR)/ss_acc.c

$(SY_DIR)/cm_mem.$(OBJ): $(CM_DIR)/cm_mem.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_mem.c

$(SY_DIR)/cm_bdy5.$(OBJ): $(CM_DIR)/cm_bdy5.c
	$(PURE)	$(CC) -c $(CCMOPTS) $(CM_DIR)/cm_bdy5.c

$(SY_DIR)/mt_ss.$(OBJ): $(MT_DIR)/mt_ss.c
	$(PURE)	$(CC) -c $(CMTOPTS) $(MT_DIR)/mt_ss.c

$(SY_DIR)/mt_id.$(OBJ): $(MT_DIR)/mt_id.c
	$(PURE)	$(CC) -c $(CMTOPTS) $(MT_DIR)/mt_id.c

#-----------------------------------------------------------------------
# DOS Microsoft 5.1 link - absolute object (.exe) file depends on
# relocatable object (.obj) files
#

#$(OUTNAME).exe: $(OBJS) $(OUTNAME).lnk
# link $(LOPTION) @$(OUTNAME).lnk
 
#----------------------------------------------------------------
# lint
#
 
#lint:
# $(LINT) $(LNTOPTION) $(OUTNAME).lnt > $(OUTNAME).ler

# DO NOT DELETE THIS LINE -- make depend depends on it.

