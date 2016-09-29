/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/**********************************************************************

     Name:     LTE eNodeB Integrated Stack
  
     Type:     Include (.h) file
  
     Desc:     

     File:     ve_sm_init.h

     Sid:      ve_sm_init.h@@/main/2 - Wed Dec  1 08:44:33 2010

     Prg:      an 

**********************************************************************/

#ifndef __VE_SM_INIT_H__
#define __VE_SM_INIT_H__

#ifdef VE_PICO
#include "lteenb_common.h"
#endif
#ifdef LTE_HO_SUPPORT
/* X2AP Default configuration Macros */
#define DFLT_CZ_RES_THR_UP 2
#define DFLT_CZ_RES_THR_LOW 1
#define UNKNOWN_VALUE 1
#define MAX_ENB_NAME_LEN 20
#endif

/* define the procs */
#ifdef SS_SEUM_CAVIUM
#define SM_SM_PROC  cvmx_get_core_num()
#define SM_NH_PROC  cvmx_get_core_num()
#define SM_NX_PROC  cvmx_get_core_num()
#define SM_RX_PROC  cvmx_get_core_num()
#define SM_SZ_PROC  cvmx_get_core_num()
#ifdef LTE_HO_SUPPORT
#define SM_CZ_PROC  cvmx_get_core_num()
#endif
#define SM_SB_PROC  cvmx_get_core_num()
#define SM_HI_PROC  cvmx_get_core_num()
#define SM_EG_PROC  cvmx_get_core_num()
#define SM_VE_PROC  cvmx_get_core_num()
#ifdef VE_PICO
#define SM_PJ_PROC  WG_TO_CL1_CORE 
#define SM_TF_PROC  WG_TO_CL1_CORE
#define SM_KW_PROC  WG_TO_CL1_CORE
#define SM_RG_PROC  WG_TO_CL1_CORE
#define SM_YS_PROC  WG_TO_CL1_CORE
#else
#define SM_PJ_PROC  0
#define SM_TF_PROC  0
#define SM_KW_PROC  0
#define SM_RG_PROC  0
#define SM_YS_PROC  0
#endif
#elif SS_4GMX_UCORE
#define SM_SM_PROC  0
#define SM_NH_PROC  0
#define SM_NX_PROC  0
#define SM_RX_PROC  0
#define SM_SZ_PROC  0
#ifdef LTE_HO_SUPPORT
#define SM_CZ_PROC  0
#endif
#define SM_SB_PROC  0
#define SM_HI_PROC  0
#define SM_EG_PROC  0
#define SM_VE_PROC  0
#define SM_PJ_PROC  1
#define SM_TF_PROC  1
#define SM_KW_PROC  1
#define SM_RG_PROC  1
#define SM_YS_PROC  1
#else
#define SM_SM_PROC  100
#define SM_NH_PROC  100
#define SM_NX_PROC  100
#define SM_RX_PROC  100
#define SM_SZ_PROC  100
#ifdef LTE_HO_SUPPORT
#define SM_CZ_PROC  100
#endif

#define SM_SB_PROC  100
#define SM_HI_PROC  100
#define SM_EG_PROC  100
#define SM_VE_PROC  100
#define SM_PJ_PROC  100
#define SM_TF_PROC  100
#define SM_KW_PROC  100
#define SM_RG_PROC  100
#define SM_YS_PROC  100
#endif

/* Assigned Pool values to 0 */
/* MSPD changes */
#if (!defined(VE_PICO) && !defined(MSPD))
#define RG_POOL     0
#define HI_POOL     0
#define SB_POOL     0
#define SZ_POOL     0

#ifdef LTE_HO_SUPPORT
#define CZ_POOL     0
#endif
#define NH_POOL     0
#define VE_POOL     0
#define EG_POOL     0
#define YS_POOL     0
#define PJ_POOL     0
#define KW_POOL     0
#else
#define RG_POOL     1
#define HI_POOL     1
#define SB_POOL     1
#define SZ_POOL     1
#ifdef LTE_HO_SUPPORT
#define CZ_POOL     1
#endif
#define NH_POOL     1
#define VE_POOL     1
#define EG_POOL     1
#define YS_POOL     1
#define PJ_POOL     1
#define KW_POOL     1
#endif
#define SCH_INST_ID   1

#define VE_YS_SUID                      0
#define VE_YS_SPID                      0
#define VE_NH_SUID                      1
#define VE_NH_SPID                      0
#define VE_PJ_SUID                      2
#define VE_PJ_SPID                      1 /* Vamsee */
#define VE_RG_SUID                      0
#define VE_RG_SPID                      0
#define VE_SZ_SPID                      0
#define VE_EG_SUID                      5
#define VE_EG_SPID                      0

#define NH_PJ_SPID                      0 /* Vamsee */
#define NH_PJ_SUID                      0

#define RG_YS_SUID                      0
#define RG_YS_SPID                      0
#define RG_SCH_YS_SPID                  1

#define SB_HI_SUID                      0
#define SB_HI_SPID                      0

#define EG_HI_SUID                      0
#define EG_HI_SPID                      1

#define SZ_SB_SUID                      0
#define SZ_SB_SPID                      0
#ifdef LTE_HO_SUPPORT
#define CZ_SB_SUID                      0
#define CZ_SB_SPID                      1
#endif

#define SZ_HI_SUID                      0
#define SZ_HI_SPID                      0

#define NH_KW_KWU_SUID                  0
#define NH_KW_CKW_SUID                  1
#define NH_RG_CRG_SUID                  2
#define NH_KW_CPJ_SUID                  3
#define NH_KW_PJU_SUID                  4



#define VE_RG_GEN_CFG                   1
#define VE_RG_RGU_SAP_CFG               2
#define VE_RG_CRG_SAP_CFG               4
#define VE_RG_SCH_INS_CFG               8
#define VE_RG_TFU_SAP_CFG              16

#define VE_KW_GEN_CFG                  1
#define VE_KW_RGU_SAP_CFG              2
#define VE_KW_CKW_SAP_CFG              4
#define VE_KW_KWU_SAP_CFG              8

#define VE_NH_GEN_CFG            1
#define VE_NH_PROT_CFG           2
#define VE_NH_USAP_CFG           4
#define VE_NH_CKW_SAP_CFG        8
#define VE_NH_KWU_SAP_CFG       16 
#define VE_NH_CRG_SAP_CFG       32
#define VE_NH_CPJ_SAP_CFG       64
#define VE_NH_PJU_SAP_CFG      128
#define VE_NH_CTF_SAP_CFG      256

#define VE_SZ_GEN_CFG                  1 
#define VE_SZ_PROT_CFG                 2
#define VE_SZ_SZT_SAP_CFG              4
#define VE_SZ_SCT_SAP_CFG              8
#define VE_SZ_PEER_CFG                16

#ifdef LTE_HO_SUPPORT
#define VE_CZ_GEN_CFG                  1 
#endif

#define VE_CZ_PROT_CFG                 2
#define VE_CZ_CZT_SAP_CFG              4
#define VE_CZ_SCT_SAP_CFG              8
#define VE_CZ_PEER_CFG                16

#define VE_SB_GEN_CFG                  1
#define VE_SB_TSAP_CFG                 2
#define VE_SB_SCT_SAP_CFG              4 

#define VE_EG_GEN_CFG                  1
#define VE_EG_EGT_SAP_CFG              2
#define VE_EG_TSAP_CFG                 4
#define VE_EG_TSRV_CFG                 8
#define VE_EG_IPADDR_CFG              16

#define VE_VE_GEN_CFG                  1
#define VE_VE_CTF_SAP_CFG              2
#define VE_VE_NHU_SAP_CFG              4
#define VE_VE_PJU_SAP_CFG              8
#define VE_VE_RGR_SAP_CFG             16
#define VE_VE_SZT_SAP_CFG             32
#define VE_VE_EGT_SAP_CFG             64
#ifdef LTE_HO_SUPPORT
#define VE_VE_CZT_SAP_CFG             128
#define VE_VE_NGH_CFG                 256
#endif

#define VE_YS_GEN_CFG                  1
#define VE_YS_CTF_SAP_CFG              2
#define VE_YS_TFU_SAP_CFG              4

#define VE_PJ_GEN_CFG                  1
#define VE_PJ_CPJ_SAP_CFG              2
#define VE_PJ_PJU_SAP_CFG              4
/* Added maximum file path */
#if (VE_SM_LOG_TO_FILE && DEBUGP)
#define VE_SM_MAX_FILE_PATH            256
#endif

#define VE_SM_PJ_CONFIGURED  (VE_PJ_GEN_CFG | VE_PJ_CPJ_SAP_CFG | \
                                              VE_PJ_PJU_SAP_CFG)

#ifdef EU_DAT_APP
#ifndef LTE_HO_SUPPORT
#define VE_SM_VE_CONFIGURED  (VE_VE_GEN_CFG | VE_VE_CTF_SAP_CFG | \
                              VE_VE_NHU_SAP_CFG | VE_VE_PJU_SAP_CFG | \
                              VE_VE_RGR_SAP_CFG | VE_VE_SZT_SAP_CFG | \
                              VE_VE_EGT_SAP_CFG)
#else                              
#define VE_SM_VE_CONFIGURED  (VE_VE_GEN_CFG | VE_VE_CTF_SAP_CFG | \
                              VE_VE_NHU_SAP_CFG | VE_VE_PJU_SAP_CFG | \
                              VE_VE_RGR_SAP_CFG | VE_VE_SZT_SAP_CFG | \
                              VE_VE_CZT_SAP_CFG | VE_VE_EGT_SAP_CFG | \
                              VE_VE_NGH_CFG)
#endif /* End of LTE_HO_SUPPORT */                              
#else
#define VE_SM_VE_CONFIGURED  (VE_VE_GEN_CFG | VE_VE_CTF_SAP_CFG | \
                              VE_VE_NHU_SAP_CFG | \
                              VE_VE_RGR_SAP_CFG | VE_VE_SZT_SAP_CFG)
#endif /* end of EU_DAT_APP */

#define VE_HI_GEN_CFG        1
#define VE_HI_TSAP_CFG       2

#define VE_SM_HI_CONFIGURED  (VE_HI_GEN_CFG | VE_HI_TSAP_CFG)
#define VE_SM_SB_CONFIGURED  (VE_SB_GEN_CFG | VE_SB_TSAP_CFG | VE_SB_SCT_SAP_CFG)
#define VE_SM_SZ_CONFIGURED  (VE_SZ_GEN_CFG | VE_SZ_PROT_CFG | VE_SZ_SZT_SAP_CFG | \
                              VE_SZ_SCT_SAP_CFG | VE_SZ_PEER_CFG)

#ifdef LTE_HO_SUPPORT
#define VE_SM_CZ_CONFIGURED      (VE_CZ_GEN_CFG | VE_CZ_PROT_CFG | VE_CZ_CZT_SAP_CFG | \
                                  VE_CZ_SCT_SAP_CFG | VE_CZ_PEER_CFG)
#define VE_SM_CZ_CONFGRD_WO_PEER (VE_CZ_GEN_CFG | VE_CZ_PROT_CFG | VE_CZ_CZT_SAP_CFG | \
                                  VE_CZ_SCT_SAP_CFG)
#endif

#define VE_SM_EG_CONFIGURED  (VE_EG_GEN_CFG | VE_EG_EGT_SAP_CFG | \
                              VE_EG_TSAP_CFG | VE_EG_TSRV_CFG | VE_EG_IPADDR_CFG)


#define VE_SM_YS_CONFIGURED   (VE_YS_GEN_CFG | VE_YS_CTF_SAP_CFG | VE_YS_TFU_SAP_CFG)

#define VE_SM_RG_CONFIGURED  (VE_RG_GEN_CFG |  VE_RG_RGU_SAP_CFG |\
                              VE_RG_CRG_SAP_CFG | VE_RG_SCH_INS_CFG |\
                              VE_RG_TFU_SAP_CFG)
#define VE_SM_KW_CONFIGURED  (VE_KW_GEN_CFG |  VE_KW_RGU_SAP_CFG |\
                              VE_KW_CKW_SAP_CFG | VE_KW_KWU_SAP_CFG)
#define VE_SM_NH_CONFIGURED  (VE_NH_GEN_CFG | VE_NH_PROT_CFG | VE_NH_USAP_CFG | \
                              VE_NH_CKW_SAP_CFG | VE_NH_KWU_SAP_CFG | \
                              VE_NH_CRG_SAP_CFG | VE_NH_CPJ_SAP_CFG | \
                              VE_NH_PJU_SAP_CFG )

#define ENTVE 0xdf

#define VESM_LYS_MAX_TFU_SAPS         2
#define VESM_LYS_NMB_UES              2

#define VE_FILL_PST(_pst,_srcProc,_dstEnt,_srcEnt,_event,_pool,_selector) \
{                                                                \
   cmMemset((U8 *)&(_pst), 0, sizeof(Pst));                       \
   _pst.dstProcId = SM_SM_PROC;                                    \
   _pst.srcProcId = _srcProc;                                    \
   _pst.dstEnt = _dstEnt;                                        \
   _pst.srcEnt = _srcEnt;                                        \
   _pst.dstInst = 0;                                             \
   _pst.srcInst = 0;                                             \
   _pst.event  = _event;                                         \
   _pst.pool   = _pool;                                         \
   _pst.selector= _selector;                                     \
}

#define SM_SET_ZERO(_buf, _size)   \
    cmMemset((U8 *)(_buf), 0, _size);

#define SM_FREE(_region, _pool, _buf, _size)          \
do                                                    \
{                                                     \
   if (_buf != NULLP)                                 \
   {                                                  \
   (Void) SPutSBuf(_region, _pool, (Data *) _buf,     \
   (Size) _size);                                     \
   (_buf) = NULLP;                                    \
   }                                                  \
}                                                     \
while(0)

#define VESM_ALLOC(_buf, _size)         \
do                                                    \
{                                                     \
   if (SGetSBuf(DFLT_REGION, DFLT_POOL, (Data **)(_buf),      \
                _size) == ROK)                        \
   {                                                  \
      SM_SET_ZERO((*_buf), _size);                    \
   }                                                  \
   else                                               \
   {                                                  \
      (*_buf) = NULLP;                                \
   }                                                  \
}                                                     \
while(0)


#define SM_DFLT_MEM_UPPER_THR_CFG   9
#define SM_DFLT_MEM_LOWER_THR_CFG   2
#define SM_DFLT_MSEC_TMR_CFG        10

#define SM_INST_ZERO   0
#define SM_ZERO_VAL    0

#define KWU_SAP_ID 0x00
#define CKW_SAP_ID 0x01
#define CRG_SAP_ID 0x02
#define CPJ_SAP_ID 0x03
#define PJU_SAP_ID 0x04
#define RGR_SAP_ID 0x05
#define RGU_SAP_ID 0x05
#define NHU_SAP_ID 0x06


#define SM_SELECTOR_LC      0
#define SM_SELECTOR_TC      1
#define SM_SELECTOR_LWLC    2


#define VE_SZ_SPID                      0
#define VE_SZ_SUID                      0

#ifdef LTE_HO_SUPPORT
#define VE_CZ_SPID                      0
#define VE_CZ_SUID                      0
#endif

#define VESM_LSZ_MAX_SZTSAP   10
#define VESM_LSZ_MAX_SCTSAP   10

#ifdef LTE_HO_SUPPORT
#define VESM_LCZ_MAX_CZTSAP   3
#define VESM_LCZ_MAX_SCTSAP   3
#endif

#define VESM_LSZ_TIMERES      10

#ifdef LTE_HO_SUPPORT
#define VESM_LCZ_TIMERES      10
#endif

#define VESM_MSGPRIOR        PRIOR0    


#ifdef LTE_HO_SUPPORT
/* CZ ---->SM posts */
#define VESM_CZSMSEL           0
/* SM ---->CZ posts */
#define VESM_SMCZSEL           0
#endif

/* SZ ---->SM posts */
#define VESM_SZSMSEL           0
/* SM ---->SZ posts */
#define VESM_SMSZSEL           0
 
/* YS ---->SM posts */
#define VESM_YSSMSEL           0
/* SM ---->YS posts */
#define VESM_SMYSSEL           0
 
/* SB ----> SM selector */
#define VESM_SBSMSEL           0
/* SM ----> SB selector */
#define VESM_SMSBSEL           0

/* RG ----> YS selector */
#ifdef CL_MAC_LWLC
#define VESM_RGYSSEL           2 /* was 1 L2Split */
#define VESM_YSRGSEL           2 /* was 1 L2Split */
#else
#define VESM_RGYSSEL           1
/* YS ----> RG selector */
#define VESM_YSRGSEL           1
#endif


/* HI ----> SM selector */
#define VESM_HISMSEL           0
/* SM ----> HI selector */
#define VESM_SMHISEL           0


/* VE ----> SZ selector */
#define VESM_VESZSEL           2
/* SZ ----> VE selector */
#define VESM_SZVESEL           2


#ifdef LTE_HO_SUPPORT
/* VE ----> CZ selector */
#define VESM_VECZSEL           2
/* CZ ----> VE selector */
#define VESM_CZVESEL           2
#endif

/* SZ ----> SB selector */
#define VESM_SZSBSEL           0

#define VESM_SZHISEL           0
/* SB ----> SZ selector */
#define VESM_SBSZSEL           0

#ifdef LTE_HO_SUPPORT
/* CZ ----> SB selector */
#define VESM_CZSBSEL           0
/* SB ----> CZ selector */
#define VESM_SBCZSEL           0
#endif

/* SB ----> HI selector */
#define VESM_SBHISEL           0
/* HI ----> SB selector */
#define VESM_HISBSEL           0

/* EG ----> HI selector */
#define VESM_EGHISEL           0
/* HI ----> EG selector */
#define VESM_HIEGSEL           0

/* VE ----> SM selector */
#define VESM_VESMSEL           0
/* SM ----> VE selector */
#define VESM_SMVESEL           0

/* VE ----> YS selector */
#define VESM_VEYSSEL           0
/* YS ----> VE selector */
#define VESM_YSVESEL           0

/* VE ----> NH selector */
#define VESM_VENHSEL           0
/* YS ----> VE selector */
#define VESM_NHVESEL           0

/* VE ----> PJ selector */
#define VESM_VEPJSEL           0
/* PJ ----> VE selector */
#define VESM_PJVESEL           0

/* VE ----> RG selector */
#define VESM_VERGSEL           0
/* RG ----> VE selector */
#define VESM_RGVESEL           0

/* VE ----> EG selector */
#define VESM_VEEGSEL           2
/* EG ----> VE selector */
#define VESM_EGVESEL           2

#define VESM_LSZ_VAL_3        3
#define VESM_LSZ_VAL_5        5
#define VESM_LSZ_VAL_10       10 
#define VESM_LSZ_VAL_50       50
#ifdef LTE_HO_SUPPORT
#define VESM_LCZ_VAL_3        3
#define VESM_LCZ_VAL_10       10 
#define VESM_LCZ_VAL_50       50
#endif

#ifndef CNE_UE_SIM_TEST
#define VESM_ENB_IPADDR       0x0A010101
#define VESM_MME_IPADDR       0x0A010102
#define VESM_SCTP_IP_ADDR     0x0A010102
#else
#define VESM_ENB_IPADDR       0xAC19005E
#define VESM_MME_IPADDR       0xAC1A0006
#define VESM_SCTP_IP_ADDR     0xAC19005E
#endif
/* MSPD changes */
#ifdef MSPD
#define VESM_LSZ_ENB_SCTPORT  10000
#define VESM_LSZ_MME_SCTPORT  20000
#else
/*  CR ID- ccpu00116764-IOT changes*/
#define VESM_LSZ_ENB_SCTPORT  36412 
#define VESM_LSZ_MME_SCTPORT  36412
#endif

#ifndef VE_SB_SCTP
#define VESM_LSZ_ENB_SCTPORT  10000
#define VESM_LSZ_MME_SCTPORT  20000
#endif

#define VESM_LSZ_ENB_PRC0     0


#define VESM_MAX_UE   (smCfgCb.maxMacRntis + smCfgCb.maxRrmRntis) 
#ifdef LTE_HO_SUPPORT
#define VESM_LCZ_ENB_SCTPORT  36422
#define VESM_LCZ_ENB_PRC0     0
#define VESM_X2HO_PREP_MAX_TIME          3
#define VESM_X2HO_OVRALL_MAX_TIME        20
#define VESM_X2HO_TIME_TO_WAIT           20
#endif

#define VESM_LSZ_MME_PRC1     1

#define VESM_MAX_PROFILE      50
#define VESM_LSZ_INV_NODE     10
#define VESM_LSZ_RESETRETRY   10

/* Peer Cfg */
#define VESM_LSZ_NUM_PEERS       1
#define VESM_LSZ_OUT_STRMS       3


#ifdef LTE_HO_SUPPORT
#define VESM_LCZ_OUT_STRMS       3
#endif

/* Peer cntrl */
#define VESM_LSZ_PEERID       0

#define VESM_LSZ_MAX_PEER     10 


#ifdef LTE_HO_SUPPORT
#define VESM_LCZ_MAX_PEER     20
#endif

#define VESM_HI_NUMBSAPS           10
#define VESM_HI_NMBCONS            10

#ifdef HI_MULTI_THREADED
#define VESM_HI_FDS                1024
#else
#define VESM_HI_FDS                64
#endif /* HI_MULTI_THREADED */
#define VESM_HI_FDBINS             4

#define VESM_HI_SELTIMEOUT         2
#define VESM_HI_UDP_MSGS_TOREAD    20
#define VESM_HI_NUMCL_TOACCEPT     5

#define VESM_HI_PERM_TSK_FLAG           TRUE
#define VESM_HI_SCHDTMR_VAL             1
/* timer resolution unused */
#define VESM_HI_TIME_RES                0
#define VESM_HI_STRT_THRESH             1
#define VESM_HI_DRP_THRESH              1
#define VESM_HI_STP_THRESH              1

#define VESM_HI_CONG_STRT                15000
#define VESM_HI_CONG_DRP                 20000
#define VESM_HI_CONG_STP                 10000
#define VESM_HI_NMB_HLBINS               16


#define VESM_LVE_VAL_10                 100 
#define VESM_LVE_MAX_BND_RETRY           3

#define VESM_CELLID                      1

/* Added sm layer name and print buf */
#if (VE_SM_LOG_TO_FILE && DEBUGP)
#define SMLAYERNAME                      "EnodeB Sm"
#define SM_PRNT_BUF                      smCb.init.prntBuf
#endif
  
#define SM_GETSBUF(_size, _datPtr, _ret)               \
{                                                      \
   _ret = SGetSBuf(DFLT_REGION,           \
                   DFLT_POOL,             \
                   (Data**)&_datPtr, _size);           \
}

#define     EVTVETUCLCFG       10
#define     EVTVESCTPCFG       11
#define     EVTVES1APCFG       12
#define     EVTVEEGTPCFG       13
#define     EVTVEAPPCFG        14
#define     EVTVERRCCFG        15
#define     EVTVEPDCPCFG       16
#define     EVTVERLCCFG        17
#define     EVTVEMACCFG        18
#define     EVTVECLCFG         19

#ifdef LTE_HO_SUPPORT
#define     EVTVEX2APCFG       0
#endif
#define     EVTVES1APSTKBND    20
#define     EVTVEEGTPSTKBND    21

#ifdef LTE_HO_SUPPORT
#define     EVTVEX2APSTKBND    0
#endif
#define     EVTVERRCSTKBND     22
#define     EVTVEENBDEBUGS     23
#define     EVTVEENBALRMS      24
#define     EVTVEINITS1SETUP   25
#define     EVTVECELLCFG       26
#define     EVTVEINITX2SETUP   27

typedef enum  {
    VE_SM_STATE_INIT = 0,
    VE_SM_STATE_TUCL_CFG_DONE,
    VE_SM_STATE_SCTP_CFG_DONE,
    VE_SM_STATE_S1AP_CFG_DONE,

#ifdef LTE_HO_SUPPORT
    VE_SM_STATE_X2AP_CFG_DONE,
#endif
    VE_SM_STATE_EGTP_CFG_DONE,
    VE_SM_STATE_APP_CFG_DONE,
    VE_SM_STATE_RRC_CFG_DONE,
    VE_SM_STATE_PDCP_CFG_DONE,
    VE_SM_STATE_RLC_CFG_DONE,
    VE_SM_STATE_MAC_CFG_DONE,
    VE_SM_STATE_CL_CFG_DONE,
    VE_SM_STATE_S1AP_STK_BND_DONE,
    VE_SM_STATE_EGTP_STK_BND_DONE,
#ifdef LTE_HO_SUPPORT
    VE_SM_STATE_X2AP_STK_BND_DONE,
#endif
    VE_SM_STATE_RRC_STK_BND_DONE,
    VE_SM_STATE_ENB_DEBUGS_DONE,
    VE_SM_STATE_ENB_ALRMS_DONE,
    VE_SM_STATE_AWAIT_S1_CON,
    VE_SM_STATE_AWAIT_CELL_UP,
    VE_SM_STATE_CELL_UP
}VeSmState;
#define VE_SM_HI_DBG_MASK 0x01
#define VE_SM_SB_DBG_MASK 0x02
#define VE_SM_SZ_DBG_MASK 0x04
#define VE_SM_EG_DBG_MASK 0x08
#define VE_SM_VE_DBG_MASK 0x10
#define VE_SM_NH_DBG_MASK 0x20
#define VE_SM_KW_DBG_MASK 0x40
#define VE_SM_RG_DBG_MASK 0x80
#define VE_SM_YS_DBG_MASK 0x100

#ifdef LTE_HO_SUPPORT
#define VE_SM_CZ_DBG_MASK 0x200
#endif


#endif /* __VE_SM_INIT_H__ */

/********************************************************************30**
  
         End of file:     ve_sm_init.h@@/main/2 - Wed Dec  1 08:44:33 2010
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
