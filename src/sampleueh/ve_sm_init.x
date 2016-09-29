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

     Name:     LTE-Uu Integrated Stack
  
     Type:     Include (.x) file
  
     Desc:     

     File:     ve_sm_init.x

     Sid:      ve_sm_init.x@@/main/2 - Wed Dec  1 08:44:33 2010

     Prg:      an 

**********************************************************************/

#ifndef __SMINITX__
#define __SMINITX__

#ifdef __cplusplus
extern "C" {
#endif

#include "gen.h"
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_mblk.h"       /* Common Mem Allocation Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_pasn.h"       /* Common Pasn Defines */
#include "nhu.h"
#include "lnh.h"
#include "lkw.h"
#include "lrg.h"
#include "lve.h"

#include "gen.x"
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_mblk.x"       /* Common Mem Allocation */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_pasn.x"       /* Common Pasn Defines */
#include "nhu.x"
#include "lnh.x"
#include "lkw.x"
#include "lrg.x"
#include "lve.x"


typedef struct _smCfgCb
{
   U32          cellId;           /* cell Id */
   U8           modType;          /* modulation type */
   U16          duplexMode;       /* Duplex Mode */
   U8           maxUeSupp;        /* max UE supported */
   TknU8        mcc[3];           /* mcc   */
   TknU8        mnc[3];           /* mnc  */
   U16          trackAreaCode;    /* Track area code */
   U8           freqBandInd;      /* Frequency Band Indicator */

   U32          enbIpAddr;        /* EnodeB IP address */
   U32          mmeIpAddr[VE_MAX_MMES];        /* MME IP Address    */
   U16          numMmes;
   U32          mmeIdx[VE_MAX_MMES];
   U32          rejectWaitTime;
   U32          sctpIpAddr;       /* SCTP IP Address    */
   /* unused param removed */ 
   U8           hiDbg;            /* TUCL Debug  */
   U8           sbDbg;            /* SCTP Debug  */
   U8           szDbg;            /* S1AP Debug  */
   U8           egDbg;            /* EGTP Debug  */
   U8           veDbg;            /* RRC APP Debug  */
   U8           nhDbg;            /* RRC Debug  */
   U8           kwDbg;            /* RLC/PDCP Debug  */
   U8           rgDbg;            /* MAC Debug  */
   U8           ysDbg;            /* CL Debug  */
   /* Fix for CRId- 115717 */
   U8           smDbg;            /* SM Debug  */

   U32          smDbgMsk;

   U32          inactvTmrVal;
   /* Fix for CRId- 115717 */
   /* Modified max expires value from U8 to U32 */
   U32          maxExpires;
   U16             macRntiStart;               /* Start RNTI for MAC */
   U16             maxMacRntis;                /* Max RNTIs managed by MAC */
   U8              numOfMacPrm;               /* dedicated prmbls for MAC */
   U16             maxRrmRntis;                /* Max RNTIs managed by RRM */
   U8              rrmDedPrmStart;             /* initial rapId for RRM */
   U8              rrmNumDedPrm;               /* dedicated prmbls for RRM */
   /* service type made configurable */
   U8           sctpUdpServiceType;
   /* PHY configuration parameters */
   U16      opMode;
   U32      counter;
   U32      period;
  /*RRC connection timer*/
   U32          rrcConTmrVal;
   U32      eNBsctpPortNum;
   U32      mmesctpPortNum;
   VeeNBType       eNBType;                    /* Type of eNB HOME/MACRO */
#ifdef LTE_HO_SUPPORT
   U8              czDbg;            /* X2AP Debug  */
   U32             x2PrepTimerVal;
   U32             x2OvrAllTimerVal;
   U32             x2TimeToWaitTimerVal;
   U16             pci;                        /* Physical Cell ID */
   U8              enbName[MAX_ENB_NAME_LEN];  /* eNB Name */
   U8              noOfBrdCstPlmn;             /* numbre of broadcasr plmns */
   U8              plmnId[MAX_NO_OF_PLMNS][3];
   VeCellFddCfg    fddCb;                     /* FDD config INFO */
   VeCellTddCfg    tddCb;                     /* TDD config info */
   U16             noOfNghInfo;                /* number of neighbor info */
   VeNghInfoCfg    **nghInfo;                  /* neighbor info */
   U8              noOfguGrp;                  /* number of pools configured */
   VeGuGrpIdCfg    **guGrpId;                  /* Pool ID list */
   U32             s1PrepTimerVal;             /* Preparation Timer Value */
   U32             s1OvrAllTimerVal;           /* Overall Timer Value */
   U32             ueHoAttachTimerVal;         /* Time within which UE \
                                                  must detach from source\
                                                  cell and attach to \
                                                  target cell during \
                                                  Handover. */
   U32             maxX2Peers;                 /* MAX number of X2 Peers */
   U8              noOfNghCfg;                 /* number of neighbor cells configured */
   VeNghCellCfg    **veNghCellCfg;             /* neighbor cell config info */
   U8              suppMsrmntCfg;              /* enable meas config */
   U32             rsrpTrshldRange;           /* Serving Cell threshold 
                                                   RSRP value */
   U8              noOfnghENBs;                /* Number of neighbor eNBs */
   VeNghENBCfg     **nghENBCfg;                /* List of neighbor eNB Configurations */ 

#endif 
}SmCfgCb;

EXTERN SmCfgCb   smCfgCb;

typedef struct _smCb
{
   TskInit         init;       /* Task Init info */
   VeSmState       smState;    /* state for state machine */
   SmCfgCb         cfgCb;      /* Configurations read from the file */
   U16             nmbMmes;    /* Number of mmes  */
   /* Adding log file informations */
#if (VE_SM_LOG_TO_FILE && DEBUGP)
   FILE*         dbgFp;                         /*!< Debug file pointer */
   Txt           filePath[VE_SM_MAX_FILE_PATH]; /*!< Path to store log files */
   U32           nmbDbgLines;                   /*!< Number of lines per debug file */
   U32           cfgDbgLines;                   /*!< Cfgd number of lines/Dbg file */
#endif /* (VE_SM_LOG_TO_FILE && DEBUGP) */

}SmCb;

EXTERN SmCb   smCb;

/* Adding log file functions */
#if (VE_SM_LOG_TO_FILE && DEBUGP)
EXTERN Void veSmCreateDbgFile(Void);
EXTERN S16 veGetSId ARGS((SystemId *s));
#endif

EXTERN S16 SRegInfoShow  ARGS((Region region, U32 *availmem));
EXTERN S16 smVeReadConfigParams ARGS((Void));
EXTERN Void smVeCfgGetTagNum ARGS((U8* tag, U16* tagNum));
EXTERN Void veCfgValdtInput ARGS((U8* input, Bool* status, U8 *noOfElem));
EXTERN Void veCfgGetParamVal ARGS((U8* tagVal, U8 paramLst[][50], U16* noOfParams));

/* Prototypes of Init functions */
EXTERN S16 smVeInit ARGS((SSTskId sysTskId));
EXTERN S16 smSzInit ARGS((SSTskId sysTskId));

#ifdef LTE_HO_SUPPORT
EXTERN S16 smCzInit ARGS((SSTskId sysTskId));
#endif
EXTERN S16 smEgInit ARGS((SSTskId sysTskId));
EXTERN S16 smEuInit ARGS((SSTskId sysTskId));
EXTERN S16 smSbInit ARGS((SSTskId sysTskId));
EXTERN S16 smHiInit ARGS((SSTskId sysTskId));
EXTERN S16 smNhInit ARGS ((SSTskId sysTskId));
EXTERN S16 smPjInit ARGS ((SSTskId sysTskId));
EXTERN S16 smRgInit ARGS ((SSTskId sysTskId));
EXTERN S16 smKwInit ARGS ((SSTskId sysTskId));
EXTERN S16 smYsInit ARGS ((SSTskId sysTskId));


/* Prototypes of state machine functions */
EXTERN Void smVeProcSm          ARGS((U8 event));
EXTERN S16  veSmSelfPstMsg      ARGS((Pst *pst));
EXTERN Void veSmHdlTuclCfgEvent ARGS((U8 event));
EXTERN Void veSmHdlSctpCfgEvent ARGS((U8 event));
EXTERN Void veSmHdlS1apCfgEvent ARGS((U8 event));
EXTERN Void veSmHdlEgtpCfgEvent ARGS((U8 event));
EXTERN Void veSmHdlAppCfgEvent  ARGS((U8 event));
EXTERN Void veSmHdlRrcCfgEvent  ARGS((U8 event));
EXTERN Void veSmHdlPdcpCfgEvent ARGS((U8 event));
EXTERN Void veSmHdlRlcCfgEvent  ARGS((U8 event));
EXTERN Void veSmHdlMacCfgEvent  ARGS((U8 event));
EXTERN Void veSmHdlCLCfgEvent   ARGS((U8 event));
EXTERN Void veSmHdlBndS1apStack ARGS((U8 event));
EXTERN Void veSmHdlBndEgtpStack ARGS((U8 event));
EXTERN Void veSmHdlBndRrcStack  ARGS((U8 event));
EXTERN Void veSmHdlEnbAlarms    ARGS((U8 event));
EXTERN Void veSmHdlEnbDebugs    ARGS((U8 event));
EXTERN Void veSmHdlInitS1Setup  ARGS((U8 event));
EXTERN Void veSmHdlInitCellCfg  ARGS((U8 event));

#ifdef LTE_HO_SUPPORT
EXTERN Void veSmHdlX2apCfgEvent ARGS((U8 event));
EXTERN Void veSmHdlBndX2apStack ARGS((U8 event));
EXTERN Void veSmHdlInitX2Setup  ARGS((U8 event));
#endif

/* Actv Task  and Actv Init*/
EXTERN S16 nhActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 nhActvInit ARGS((Ent, Inst, Region, Reason));
EXTERN S16 kwActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 kwActvInit ARGS((Ent, Inst, Region, Reason));
EXTERN S16 rgActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 rgActvInit ARGS((Ent, Inst, Region, Reason));
EXTERN S16 schActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 schActvInit ARGS((Ent, Inst, Region, Reason));
EXTERN S16 egActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 egActvInit ARGS((Ent, Inst, Region, Reason));
EXTERN S16 pjActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 pjActvInit ARGS((Ent, Inst, Region, Reason));
#ifdef LTE_ENB_PAL
EXTERN S16 tfActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 tfActvInit ARGS((Ent, Inst, Region, Reason));
#else
#endif /* LTE_ENB_PAL */

EXTERN S16 smEgActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smRgActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smNhActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smKwActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smPjActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smHiActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smSbActvTsk ARGS((Pst *, Buffer *));
EXTERN S16 smSzActvTsk ARGS((Pst *, Buffer *));

#ifdef LTE_HO_SUPPORT
EXTERN S16 smSzActvTsk ARGS((Pst *, Buffer *));
#endif

/* Config functions */
EXTERN Void veSmHiCfg ARGS((Void));
EXTERN Void veSmSbCfg ARGS((Void));
EXTERN Void veSmSzCfg ARGS((Void));
EXTERN Void veSmEgCfg ARGS((Void));
EXTERN Void veSmVeCfg ARGS((Void));
EXTERN Void veSmNhCfg ARGS((Void));
EXTERN Void veSmPjCfg ARGS((Void));
EXTERN Void veSmKwCfg ARGS((Void));
EXTERN Void veSmRgCfg ARGS((Void));
EXTERN Void veSmYsCfg ARGS((Void));

#ifdef LTE_HO_SUPPORT
EXTERN Void veSmCzCfg ARGS((Void));
#endif


/* HI(TUCL) Build Functions */
EXTERN Void smBuildHiGenCfg     ARGS((Void));
EXTERN Void smBuildHiTSapCfg    ARGS((SpId spId));
EXTERN Void smBuildHiDbgCntrl   ARGS((Void));

/* SB(SCTP) Build Functions */
EXTERN Void smBuildSbGenCfg     ARGS((Void));
EXTERN Void smBuildSbTSapCfg    ARGS((SpId spId, SuId suId));
EXTERN Void smBuildSbSctSapCfg  ARGS((SpId spId));
EXTERN Void smBindSbToLSap      ARGS((SpId spId));
EXTERN Void smBuildSbDbgCntrl   ARGS((Void));

/* SZ(S1AP) Build Functions */
EXTERN S16 smBuildSzGenCfg     ARGS((Void));
EXTERN S16 smBuildSzLSapCfg    ARGS((SpId spId, SuId suId));
EXTERN S16 smBuildSzUSapCfg    ARGS((SpId spId));
EXTERN S16 smBuildSzProtCfg    ARGS((Void));
EXTERN S16 smBuildSzPeerCfg    ARGS((Void));
EXTERN S16 smBindSzToLSap      ARGS((Void));
EXTERN S16 smBuildSzGenCntrl   ARGS((Void));
EXTERN S16 smBuildSzDbgCntrl   ARGS((Void));


#ifdef LTE_HO_SUPPORT
/* CZ(X2AP) Build Functions */
EXTERN S16 smBuildCzGenCfg     ARGS((Void));
EXTERN S16 smBuildCzProtCfg    ARGS((Void));
EXTERN S16 smBuildCzLSapCfg    ARGS((SuId suId, SpId spId));
EXTERN S16 smBuildCzUSapCfg    ARGS((SpId spId));
EXTERN S16 smBuildCzPeerCfg    ARGS((Void));
EXTERN S16 smBuildCzUstaCntrl    ARGS((Void));
EXTERN S16 smBuildCzDbgCntrl    ARGS((Void));
EXTERN S16 smBuildCzShutDownCntrl    ARGS((Void));
EXTERN S16 smBuildCzLSapTrcCntrl    ARGS((U16 action));
EXTERN S16 smBuildCzPeerTrcCntrl  ARGS((Void));
EXTERN S16 smBuildCzEndPtCntrl    ARGS((Void));
EXTERN S16 smBuildCzAssocCntrl    ARGS((Void));
EXTERN S16 smBuildCzGenStsReq    ARGS((Void));
EXTERN S16 smBuildCzPeerStsReq    ARGS((Void));
EXTERN S16 smBuildCzGenStaReq    ARGS((Void));
EXTERN S16 smBuildCzSidStaReq    ARGS((Void));
EXTERN S16 smBuildCzUSapStaReq    ARGS((Void));
EXTERN S16 smBuildCzLSapStaReq    ARGS((Void));
EXTERN S16 smBuildCzPeerStaReq    ARGS((Void));
EXTERN S16 smBindCzToLSap    ARGS((Void));
EXTERN S16 smUbindCzFrmUSap    ARGS((Void));
EXTERN S16 smDelCzUSap    ARGS((Void));
EXTERN S16 smUbindCzFrmLSap    ARGS((Void));
EXTERN S16 smDelCzLSap    ARGS((Void));
EXTERN S16 smDelCzPeer    ARGS((Void));
#endif

/* EG(EGTP) Build Functions */
EXTERN S16  smBuildEgGenCfg     ARGS((Void));
EXTERN S16  smBuildEgUSapCfg    ARGS((SpId spId));
EXTERN S16  smBuildEgLSapCfg    ARGS((SpId spId, SuId suId));
/* Shwetha - adding IP Address configuration for EGTP-U */
EXTERN S16  smBuildEgIpAddrCfg  ARGS((SpId  spId));
EXTERN S16  smBuildTServerCfg   ARGS((Void));
EXTERN Void smBindEgToHitSap    ARGS((SpId spId));
EXTERN Void  smBuildEgDbgCntrl   ARGS((Void));

/* YS(CL) Build Functions */
EXTERN S16  smBuildYsGenCfg     ARGS((Void));
EXTERN S16  smBuildYsTfuSapCfg  ARGS((SpId spId, SuId suId));
EXTERN S16  smBuildYsSchTfuSapCfg  ARGS((SpId spId, SuId suId));
EXTERN S16  smBuildYsCtfSapCfg  ARGS((SpId spId, SuId suId));
EXTERN S16  smBuildYsGenCntrl   ARGS((Void));
EXTERN S16  smBuildVeDbgCntrl   ARGS((Void));

/* VE(APP) Build Functions */
EXTERN S16 smBuildVeGenCfg     ARGS((Void));
EXTERN S16 smBuildVeCtfLSapCfg ARGS((SpId spId, SuId suId));
EXTERN S16 smBuildVeNhuLSapCfg ARGS((SpId spId, SuId suId));
EXTERN S16 smBuildVeRgrLSapCfg ARGS((SpId spId, SuId suId));
EXTERN S16 smBuildVeSztLSapCfg ARGS((SpId spId, SuId suId));

#ifdef LTE_HO_SUPPORT
EXTERN S16 smBuildVeCztLSapCfg ARGS((SpId spId, SuId suId));
EXTERN S16 smBindVeToCztSap ARGS((Void));
EXTERN S16 smBuildVeNghCfg     ARGS((Void));
#endif

#ifdef EU_DAT_APP
EXTERN S16 smBuildVeEgtLSapCfg ARGS((SpId spId, SuId suId));
EXTERN S16 smBuildVePjuLSapCfg ARGS((SpId spId, SuId suId));
EXTERN S16 smBindVeToEgtSap    ARGS((Void));
EXTERN S16 smBindVeToPjuSap    ARGS((Void));
#endif
EXTERN S16 smBindVeToSztSap    ARGS((Void));
EXTERN S16 smBindVeToNhuSap    ARGS((Void));
EXTERN S16 smBindVeToCtfSap    ARGS((Void));
EXTERN S16 smBindVeToRgrSap    ARGS((Void));
EXTERN S16 smBuildVeGenCntrl   ARGS((Void));
EXTERN S16 smBuildVeS1InitCntrl   ARGS((Void));
EXTERN S16 smBuildVeCellCntrl   ARGS((Void));
EXTERN S16 smBuildVeDbgCntrl   ARGS((Void));

/* NH(LTE RRC) Build Functions */
EXTERN S16 smBuildNhGenCfg     ARGS((Void));
EXTERN S16 smBuildNhProtCfg    ARGS((Void));
EXTERN S16 smBuildNhUsapCfg    ARGS((Void));
EXTERN S16 smBuildNhKwuLsapCfg ARGS((Void));
EXTERN S16 smBuildNhCkwLsapCfg ARGS((Void));
EXTERN S16 smBuildNhCrgLsapCfg ARGS((Void));
EXTERN S16 smBuildNhCpjLsapCfg ARGS((Void));
EXTERN S16 smBuildNhPjuLsapCfg ARGS((Void));
EXTERN S16 smBuildNhLsapCfg    ARGS((U8 elemId));
EXTERN S16 smBindNhToKwuSap    ARGS((Void));
EXTERN S16 smBindNhToCkwSap    ARGS((Void));
EXTERN S16 smBindNhToCrgSap    ARGS((Void));
EXTERN S16 smBindNhToCpjSap    ARGS((Void));
EXTERN S16 smBindNhToPjuSap    ARGS((Void));
EXTERN S16 smBindNhToLsap      ARGS((U8 elemId));
EXTERN S16 smBuildNhGenCntrl   ARGS((Void));
EXTERN S16 smBuildNhDbgCntrl   ARGS((Void));
EXTERN S16 smBuildCzGenCntrl ARGS((Void));
EXTERN S16 smBuildRgSchDbgCntrl ARGS((Void));
EXTERN S16 smBuildVeX2InitCntrl ARGS((Void));


/* PJ(LTE PDCP) Build Functions */
EXTERN S16 smBuildPjGenCfg     ARGS((Void));
EXTERN S16 smBuildPjCpjSapCfg  ARGS((Void));
EXTERN S16 smBuildPjPjuSapCfg  ARGS((SpId spId));

/* KW(LTE RLC) Build Functions */
EXTERN S16 smBuildKwGenCfg     ARGS((Void));
EXTERN S16 smBuildKwUsapCfg    ARGS((U8 elemId, U8 ent));
EXTERN S16 smBuildCkwUsapCfg   ARGS((Void));
EXTERN S16 smBuildKwuUsapCfg   ARGS((U8 ent));
EXTERN S16 smBuildKwRguLsapCfg ARGS((Void));
EXTERN S16 smBindKwToRguSap    ARGS((Void));
EXTERN S16 smBuildKwDbgCntrl   ARGS((Void));
EXTERN S16 smBuildYsDbgCntrl   ARGS((Void));

/* RG(LTE MAC) Build Functions */
EXTERN S16 smBuildRgGenCfg     ARGS((Void));
EXTERN S16 smBuildRgUsapCfg    ARGS((U8 elemId));
EXTERN S16 smBuildCrgUsapCfg   ARGS((Void));
EXTERN S16 smBuildRguUsapCfg   ARGS((Void));
EXTERN S16 smBuildRgSchInsCfg   ARGS((Void));
EXTERN S16 smBuildRgTfuLsapCfg ARGS((Void));
EXTERN S16 smBindRgToTfuSap    ARGS((Void));
EXTERN S16 smBindRgSchToTfuSap ARGS((Void));
EXTERN S16 smBuildRgGenCntrl   ARGS((Void));
EXTERN S16 smBuildRgDbgCntrl   ARGS((Void));

typedef enum {
 VE_TAG_CELL_ID,
 VE_TAG_MOD_TYPE,
 VE_TAG_DUP_MODE,
 VE_TAG_MAX_UE_SUPPORT,
 VE_TAG_MCC_0,
 VE_TAG_MCC_1,
 VE_TAG_MCC_2,
 VE_TAG_MNC_0,
 VE_TAG_MNC_1,
 VE_TAG_MNC_2,
 VE_TAG_TA_CODE,
 VE_TAG_FREQ_BAND_IND,
 VE_TAG_ENB_IP_ADDR,
 VE_TAG_MME_IP_ADDR,
 VE_TAG_REJECT_WAIT_TIME,
 VE_TAG_SCTP_IP_ADDR,
 VE_TAG_HI_DBG,
 VE_TAG_SB_DBG,
 VE_TAG_SZ_DBG,
 VE_TAG_EG_DBG,
 VE_TAG_VE_DBG,
 VE_TAG_NH_DBG,
 VE_TAG_KW_DBG,
 VE_TAG_RG_DBG,
 VE_TAG_YS_DBG,
 VE_TAG_SM_DBG,
 VE_TAG_INACTIVITY_TIMER_VAL,
 VE_TAG_MAX_EXPIRY,
 VE_TAG_CZ_DBG,
 VE_TAG_X2_PREP_TMR,
 VE_TAG_X2_OVRALL_TMR,
 VE_TAG_ENB_NAME,
 VE_TAG_NO_OF_BRDCST_PLMN,
 VE_TAG_PLMN_ID_LST,
 VE_TAG_CELL_FDD_CFG,
 VE_TAG_CELL_TDD_CFG,
 VE_TAG_NO_OF_NGH_INFO,
 VE_TAG_NGH_INFO_CFG,
 VE_TAG_NO_OF_GU_GRP,
 VE_TAG_GU_GRP_ID_CFG,
 VE_TAG_S1_PREP_TMR,
 VE_TAG_S1_OVRALL_TMR,
 VE_TAG_NO_OF_NGH_CFG,
 VE_TAG_NGH_CELL_CFG,
 VE_TAG_MEAS_CFG_ENB,
 VE_TAG_RSRP_THRSHLD_VAL,
 VE_TAG_NO_OF_NGH_ENB_CFG,
 VE_TAG_NGH_ENB_CFG,
 VE_TAG_ENB_TYPE,
 VE_TAG_PCI_VAL,
 VE_TAG_MAX_X2_PEERS,
 VE_TAG_X2_TIME_TO_WAIT,
 VE_TAG_UE_HO_ATTACH_TMR,
 VE_TAG_MAC_RNTI_STRT,
 VE_TAG_MAX_MAC_RNTIS,
 VE_TAG_MAC_NO_OF_PREMBL,
 VE_TAG_MAX_RRM_RNTIS,
 VE_TAG_RRM_NO_OF_DED_PREMBL,
 VE_TAG_RRM_DED_PREMBL_STRT,
 VE_TAG_SCTP_SRVC_TYPE,
 VE_TAG_OP_MODE,
 VE_TAG_COUNTER,
 VE_TAG_PERIOD,
 VE_RRC_CON_TIMER,
 VE_TAG_ENB_SCTP_PORT,
 VE_TAG_MME_SCTP_PORT,
 /* Add the new Tag nums above this */
 MAX_CFG_PARAM
}veCfgTagNum;

EXTERN const S8 *veCfgTagLst[MAX_CFG_PARAM];

#ifdef __cplusplus
}
#endif

#endif /* __SMINITX__ */

/********************************************************************30**
  
         End of file:     ve_sm_init.x@@/main/2 - Wed Dec  1 08:44:33 2010
  
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
