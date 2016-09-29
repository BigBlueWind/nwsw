##/********************************************************************16**
#
#                         (c) COPYRIGHT 2009 by 
#                         Continuous Computing Corporation.
#                         All rights reserved.
#
#     This software is confidential and proprietary to Continuous Computing 
#     Corporation (CCPU).  No part of this software may be reproduced,
#     stored, transmitted, disclosed or used in any form or by any means
#     other than as expressly provided by the written Software License 
#     Agreement between CCPU and its licensee.
#
#     CCPU warrants that for a period, as provided by the written
#     Software License Agreement between CCPU and its licensee, this
#     software will perform substantially to CCPU specifications as
#     published at the time of shipment, exclusive of any updates or 
#     upgrades, and the media used for delivery of this software will be 
#     free from defects in materials and workmanship.  CCPU also warrants 
#     that has the corporate authority to enter into and perform under the   
#     Software License Agreement and it is the copyright owner of the software 
#     as originally delivered to its licensee.
#
#     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
#     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
#     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
#     MATERIALS.
#
#     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
#     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
#     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
#     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
#     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
#     OF THE POSSIBILITY OF SUCH DAMAGE.
#
#                       Restricted Rights Legend
#
#     This software and all related materials licensed hereby are
#     classified as "restricted computer software" as defined in clause
#     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
#     developed entirely at private expense for nongovernmental purposes,
#     are commercial in nature and have been regularly used for
#     nongovernmental purposes, and, to the extent not published and
#     copyrighted, are trade secrets and confidential and are provided
#     with all rights reserved under the copyright laws of the United
#     States.  The government's rights to the software and related
#     materials are limited and restricted as provided in clause
#     52.227-19 of the FAR.
#
#                    IMPORTANT LIMITATION(S) ON USE
#
#     The use of this software is limited to the use set
#     forth in the written Software License Agreement between CCPU and
#     its Licensee. Among other things, the use of this software
#     may be limited to a particular type of Designated Equipment, as 
#     defined in such Software License Agreement.
#     Before any installation, use or transfer of this software, please
#     consult the written Software License Agreement or contact CCPU at
#     the location set forth below in order to confirm that you are
#     engaging in a permissible use of the software.
#
#
#                    Continuous Computing Corporation
#                    9450, Carroll Park Drive
#                    San Diego, CA-92121, USA
#
#                    Tel: +1 (858) 882 8800
#                    Fax: +1 (858) 777 3388
#
#                    Email: support@trillium.com
#                    Web: http://www.ccpu.com
#
#*********************************************************************17*/
#
#********************************************************************20**
#
#       Name:   LTE RRC - makefile
#
#       Type:   Makefile
#
#       Desc:   Compile, assemble and link product software for
#               various configurations. Further comments are
#               embedded in the file. The following targets are
#               defined for use:
#
#               This file supports a variety of environments and
#               build targets.
#
#               ENVIRONMENTS:
#
#               Solaris 2.x with SUNpro C
#
#               Solaris 2.x with GNU C (gcc)
#
#               Solaris 2.x with MRI C (mcc68k) cross compiled for
#               a relatively standard 68040 processor board
# 
#               Microsoft Visual C++ 4.x for Windows '95 and Windows NT
#               environments(msvc)
#
#               BUILD TARGETS:
#
#               lib              library (with MTSS)
#               pt               portable software (without MTSS) 
#
#               depend.pt        makedepend for portable software
#
#               clean            remove all object files
#
#       Sid:      ys_pc.mak
#
#       Prg:    
#
#********************************************************************21*/


#=======================================================================
#-------------------------------------------------------------#
#Makefile for product YS - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
CM_INC_DIR=../ltephy/picochip/common/include

CCysFLAGS=-DTF -DYS -DYS_PICO -DLCLYS -DLCYSMILYS -DLCSMMILYS -DTFU_VER_2 -UTFU_TDD -DLCTFU -DRG -DSS_CAVIUM \
           -DDEBUGP -UxNO_ERRCLS -UxNOERRCHK -DYS_PICO_DBG -DLCYSUICTF -UxTRANS_EMU -ULTE_TDD -DDUPLEX_MODE=FDD_DUPLEX 

# Product sources-------------------------------------------------------
ysSRCS= \
    $(IN_DIR)/ys_pc_edm.c \
    $(IN_DIR)/ys_pc_ta_alg.c \
    $(IN_DIR)/ys_ex_ms.c \
    $(IN_DIR)/ys_db.c \
    $(IN_DIR)/ys_id.c \
    $(IN_DIR)/ys_ui.c \
    $(IN_DIR)/ys_pc_ui.c \
    $(IN_DIR)/ys_mi.c \
    $(IN_DIR)/ys_pc_li.c \
    $(IN_DIR)/ys_utl.c \
    $(IN_DIR)/ys_pc_utl.c \
    $(IN_DIR)/ys_pc_msg.c \
    $(IN_DIR)/ys_pc_sm.c \
    $(IN_DIR)/ys_ptmi.c \
    $(IN_DIR)/ys_ptui.c \
    $(IN_DIR)/ys_tmr.c \
    $(IN_DIR)/ys_init.c 


# Product objects-------------------------------------------------------
ysOBJS= \
    $(OUT_DIR)/ys_pc_edm.$(OBJ) \
    $(OUT_DIR)/ys_pc_ta_alg.$(OBJ) \
    $(OUT_DIR)/ys_ex_ms.$(OBJ) \
    $(OUT_DIR)/ys_db.$(OBJ) \
    $(OUT_DIR)/ys_id.$(OBJ) \
    $(OUT_DIR)/ys_ui.$(OBJ) \
    $(OUT_DIR)/ys_pc_ui.$(OBJ) \
    $(OUT_DIR)/ys_mi.$(OBJ) \
    $(OUT_DIR)/ys_pc_li.$(OBJ) \
    $(OUT_DIR)/ys_utl.$(OBJ) \
    $(OUT_DIR)/ys_pc_utl.$(OBJ) \
    $(OUT_DIR)/ys_pc_msg.$(OBJ) \
    $(OUT_DIR)/ys_pc_sm.$(OBJ) \
    $(OUT_DIR)/ys_ptmi.$(OBJ) \
    $(OUT_DIR)/ys_ptui.$(OBJ) \
    $(OUT_DIR)/ys_init.$(OBJ) 

#    $(OUT_DIR)/ys_tmr.$(OBJ) 

YSCM_INC=$(IN_DIR)/ys.h $(IN_DIR)/ys.x $(IN_DIR)/ys_err.h $(IN_DIR)/ys_pc.h \
         $(CM_INC_DIR)/L1ApiCommon.h $(IN_DIR)/ys_pc.x

ALL_INC=$(YSCM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/ys_pc_edm.$(OBJ): $(IN_DIR)/ys_pc_edm.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pc_edm.c -o \
	$(OUT_DIR)/ys_pc_edm.$(OBJ)

$(OUT_DIR)/ys_pc_ta_alg.$(OBJ): $(IN_DIR)/ys_pc_ta_alg.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pc_ta_alg.c -o \
	$(OUT_DIR)/ys_pc_ta_alg.$(OBJ)

$(OUT_DIR)/ys_ex_ms.$(OBJ): $(IN_DIR)/ys_ex_ms.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ex_ms.c -o \
	$(OUT_DIR)/ys_ex_ms.$(OBJ)

$(OUT_DIR)/ys_db.$(OBJ): $(IN_DIR)/ys_db.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_db.c -o \
	$(OUT_DIR)/ys_db.$(OBJ)

$(OUT_DIR)/ys_id.$(OBJ): $(IN_DIR)/ys_id.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_id.c -o \
	$(OUT_DIR)/ys_id.$(OBJ)

$(OUT_DIR)/ys_pc_ui.$(OBJ): $(IN_DIR)/ys_pc_ui.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pc_ui.c -o \
	$(OUT_DIR)/ys_pc_ui.$(OBJ)

$(OUT_DIR)/ys_ui.$(OBJ): $(IN_DIR)/ys_ui.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ui.c -o \
	$(OUT_DIR)/ys_ui.$(OBJ)

$(OUT_DIR)/ys_mi.$(OBJ): $(IN_DIR)/ys_mi.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_mi.c -o \
	$(OUT_DIR)/ys_mi.$(OBJ)

$(OUT_DIR)/ys_pc_li.$(OBJ): $(IN_DIR)/ys_pc_li.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pc_li.c -o \
	$(OUT_DIR)/ys_pc_li.$(OBJ)

$(OUT_DIR)/ys_pc_utl.$(OBJ): $(IN_DIR)/ys_pc_utl.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pc_utl.c -o \
	$(OUT_DIR)/ys_pc_utl.$(OBJ)

$(OUT_DIR)/ys_utl.$(OBJ): $(IN_DIR)/ys_utl.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_utl.c -o \
	$(OUT_DIR)/ys_utl.$(OBJ)

$(OUT_DIR)/ys_pc_msg.$(OBJ): $(IN_DIR)/ys_pc_msg.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pc_msg.c -o \
	$(OUT_DIR)/ys_pc_msg.$(OBJ)

$(OUT_DIR)/ys_pc_sm.$(OBJ): $(IN_DIR)/ys_pc_sm.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_pc_sm.c -o \
	$(OUT_DIR)/ys_pc_sm.$(OBJ)

$(OUT_DIR)/ys_ptmi.$(OBJ): $(IN_DIR)/ys_ptmi.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ptmi.c -o \
	$(OUT_DIR)/ys_ptmi.$(OBJ)

$(OUT_DIR)/ys_ptui.$(OBJ): $(IN_DIR)/ys_ptui.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_ptui.c -o \
	$(OUT_DIR)/ys_ptui.$(OBJ)

$(OUT_DIR)/ys_init.$(OBJ): $(IN_DIR)/ys_init.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_init.c -o \
	$(OUT_DIR)/ys_init.$(OBJ)

$(OUT_DIR)/ys_tmr.$(OBJ): $(IN_DIR)/ys_tmr.c $(ALL_INC)
	$(CC) -c $(COPTS) $(IOPTS) $(POPTS) $(CCysFLAGS) $(IN_DIR)/ys_tmr.c -o \
	$(OUT_DIR)/ys_tmr.$(OBJ)

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/liblnxpcys.a:$(ysOBJS)
	ar -cr $(OUT_DIR)/liblnxpcys.a $(ysOBJS) 

$(OUT_DIR)/libcavpcys.a:$(ysOBJS)
	ar -cr $(OUT_DIR)/libcavpcys.a $(ysOBJS) 
#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libcavpcys.a $(ysOBJS)

#
#********************************************************************30**
#  
#        End of file:     ys_pc.mak
#    
#********************************************************************31*/
#
#********************************************************************40**
#  
#        Notes:
#  
#********************************************************************41*/
#
#********************************************************************50**
#
#********************************************************************51*/
#
#********************************************************************60**
#  
#       Revision history:
#  
#********************************************************************61*/
#  
#********************************************************************90**
#
#   ver       pat    init                  description
#---------- -------- ---- -----------------------------------------------
#********************************************************************91*/

# DO NOT DELETE THIS LINE -- make depend depends on it.

