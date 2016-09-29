#/********************************************************************16**
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
#                    Continuous Computing Corporation
#                    9380, Carroll Park Drive
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
#       Name:   LTE eNodeB Sample Application
#
#       Type:   make file
#
#       Desc:   Compile, assemble and link product software for
#               various configurations. Further comments are
#               embedded in the file. 
#
#               This file supports a variety of environments and
#               build targets. The default build target will be the
#               portable target for the Linu 2.4.x with GNU C(gcc)
#
#       Env:    Linux 2.4.x with GNU C (gcc)
#
#               BUILD TARGETS:
#
#               clean            remove all object files
#   
#       File:  stack.mak
#
#
#       Prg:   an
#
#********************************************************************21*/
#=======================================================================

#-----------------------------------------------------------------------
# macro for output file name and makefile name
#

lnxperf:
	make -f enodeb_pal_lnx.mak acc BLDENV=lnx_perf

lnxe2e:
	@make -f enodeb_pal_lnx.mak acc BLDENV=lnx_pre
	@perl Tokenize_Pre.pl
	@make -f enodeb_pal_lnx.mak acc BLDENV=lnx_e2e

mse2e:
	@make -f enodeb_uarm.mak acc BLDENV=lnx_ms_pre
	@perl Tokenize_Pre.pl
	@make -f enodeb_uarm.mak acc BLDENV=lnx_e2e_ms

lnxe2e_oam:
	@make -f enodeb_pal_lnx.mak acc BLDENV=lnx_pre OAM_ENV=WR_RSYS_OAM
	@perl Tokenize_Pre.pl
	@make -f enodeb_pal_lnx.mak acc BLDENV=lnx_e2e OAM_ENV=WR_RSYS_OAM

mse2e_oam:
	@make -f enodeb_uarm.mak acc BLDENV=lnx_ms_pre OAM_ENV=WR_RSYS_OAM
	@perl Tokenize_Pre.pl
	@make -f enodeb_uarm.mak acc BLDENV=lnx_e2e_ms OAM_ENV=WR_RSYS_OAM

lnx_withoutdiag:
	@make -f enodeb_pal_lnx.mak acc BLDENV=lnx_withoutdiag

mse2e_withoutdiag:
	@make -f enodeb_uarm.mak acc BLDENV=lnx_ms_withoutdiag

lnx_withoutdiag_oam:
	@make -f enodeb_pal_lnx.mak acc BLDENV=lnx_withoutdiag OAM_ENV=WR_RSYS_OAM

mse2e_withoutdiag_oam:
	@make -f enodeb_uarm.mak acc BLDENV=lnx_ms_withoutdiag OAM_ENV=WR_RSYS_OAM

ms4gmxe2e:
	make -f enodeb_larm.mak acc BLDENV=4gmx_e2e

ms4gmxe2elib:
	make -f enodeb_larm.mak acc BLDENV=4gmx_e2e_lib


lnx_lib:
	make -f enodeb_pal_lnx.mak acc BLDENV=lnx_lib

lnxclean:
	@make -f enodeb_pal_lnx.mak clean

cleanstack:
	make -f enodeb_larm.mak cleanstack

msclean:
	make -f enodeb_uarm.mak clean

lnxkworks:
	@make -f enodeb_pal_lnx.mak acc BLDENV=lnx_kworks

lnxclean_oam:
	cd ../../tenb_commonplatform/software/products/fap/hbs2/hbs2-iu/;	make clean PRODUCT=generic HARDWARE=generic BUILD=i686-linux;

msclean_oam:
	cd ../../tenb_commonplatform/software/products/fap/hbs2/hbs2-iu/;	make clean PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-none-linux-gnueabi;

# DO NOT DELETE THIS LINE -- make depend depends on it.

