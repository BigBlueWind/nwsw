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

/********************************************************************20**

     Name:     LTE RRC

     Type:     C source file

     Desc:     This file contains Performance measurement related 
               functions

     File:     ve_perf_li.c

     Sid:      ve_perf_li.c@@/main/2 - Wed Dec  1 08:44:27 2010

     Prg:     rbabu 

*********************************************************************21*/
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */

#ifndef SS_SEUM_CAVIUM
#include "ss_err.h"        /* errors */
#include "ss_dep.h"        /* implementation-specific */
#include "ss_queue.h"      /* queues */
#include "ss_strm.h"       /* STREAMS */
#include "ss_msg.h"        /* messaging */
#include "ss_mem.h"        /* memory management interface */
#endif 

#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"

#include "nhu_asn.h"
#include "nhu.h"           /* NHU interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#include "ve_eut.h"
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */

#include "ve_perf.h"

/* RRM related includes */
#include "ve.h" 
#include "ve_sm_init.h"
#include "lve.h"
#ifdef VE_LNX_PCAP_PERF
#include  <pcap.h>
#include "ve_rout.h"
#endif

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#ifndef SS_SEUM_CAVIUM
#include "ss_dep.x"        /* implementation-specific */
#include "ss_queue.x"      /* queues */
#include "ss_task.x"       /* tasking */
#include "ss_timer.x"      /* timers */
#include "ss_strm.x"       /* STREAMS */
#include "ss_msg.x"        /* messaging */
#include "ss_mem.x"           /* System services */
#endif 

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"

#include "nhu_asn.x"
#include "nhu.x"           /* NHU Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#include "ve_eut.x"
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */

/* RRM related includes */
#include "lve.x" 
#include "ve.x" 
#include "ve_sm_init.x"
#ifdef VE_LNX_PCAP_PERF
#include "ve_rout.x"
#endif
#include "ve_dflt.h"

#if (defined(VE_PERF_DL_DATA) || defined(VE_LNX_PCAP_PERF))
PUBLIC U16 veNumofRecfgCmpRecvd;
#endif

#ifdef VE_PERF_MEAS
PUBLIC S16 vePerfLIMRrcReconfigCmpHndlr ARGS((NhuDatIndSdus *datIndSdu));
PUBLIC S16 vePerfLIMRrcSecModCmpHndlr ARGS((NhuDatIndSdus *datIndSdu));
PUBLIC S16 vePerfLIMRrcConSetCmpHndlr ARGS((NhuDatIndSdus *datIndSdu));
PUBLIC S16  vePerfFillNhuUeCfgInfo ARGS((
Ptr           evnt,
NhuUeCfgInfo  *ueCfg,
VeUeCb        *ueCb,
U8            msgType
));

EXTERN S16 vePerfFillSecCfg ARGS((
NhuSecurityCfg     *secCfg,
VeUeCb             *ueCb
));

EXTERN S16 vePerfSndRrcRecfgReq ARGS((
VeUeCb        *ueCb
));

EXTERN S16 vePerfFillNhuRrcConReCfgn ARGS((
VeUeCb        *ueCb,
NhuDatReqSdus *nhDatReqEvntSdu
));

EXTERN S16 vePerfFillRRC_ReCfg_ERABSetupLst ARGS((
Ptr           evnt,
VeUeCb        *ueCb,
NhuRRCConRecfgn_r8_IEs *reCfgIes
));

EXTERN S16 veSndDataStart ARGS((
VeUeCb *ueCb,
U8 evnt
));

EXTERN S16 vePerfSndSecModeCmnd ARGS((
VeUeCb *ueCb /* configuration */
));

EXTERN Void vePerfTmrExp ARGS((
PTR   pCb,       /* protocol control block */
S16   tmrEvnt    /* Timer Event */
));

EXTERN S16 vePerfFillRRC_SecModeCmd ARGS((
NhuDatReqSdus *nhuDatReqSdu,
VeUeCb *ueCb
));

EXTERN Void vePerfStartTmr ARGS((
S16    tmrEvnt      /* Timer Event */
));

EXTERN S16 VeLiPerfPjuDatInd ARGS((
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
Buffer         *mBuf
));

PUBLIC Void vePerfSndData ARGS((Void));
PUBLIC S16 vePerfActvTmr ARGS((Void));



/* Global variables */
unsigned long int  noPkts=0;
U32 noSysTicks=0;
unsigned long int  start=0;
unsigned long int  noBytes=0;
unsigned long int  secDiff=0;

PRIVATE DateTime stDt;
PRIVATE DateTime enDt;
U32      bpsApp=0;
PRIVATE char loadData[VE_PERF_SDU_SIZE]= {'D'};

/* RRC State Machine for Uplink RRC Messages */
/* based on Message Category and Message Type*/
PUBLIC  VeNhuLIMFSM vePerfNhuLIMFSM[VE_NH_MAX_MSG_CLASS][VE_NH_MAX_MSG_TYPE]=
{
   /*DUMMY*/
   {     veNoOpr, 
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr
   },
   { /*DL-DCCH message */
      veNoOpr, 
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr
   },
   { /* UL-DCCH message */
      veNoOpr, 
      veNoOpr,
      vePerfLIMRrcReconfigCmpHndlr, /*RRC Reconfiguration Request Handler*/
      veNoOpr,
      vePerfLIMRrcConSetCmpHndlr,   /*RRC Connection Setup Complete Handler */
      vePerfLIMRrcSecModCmpHndlr,   /*RRC Security Mode Complete handler*/
      veLIMRrcSecModFailHndlr,
      veNoOpr,
      veNoOpr,
      veLIMRrcUlInfoTfrHndlr,
      veNoOpr
   },
   { /*DL-CCCH message */
      veNoOpr, 
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr,
      veNoOpr
   },
   { /* UL-CCCH message */
      veSndRrcConReEst, 
      veSndRrcConReqSetup, /* RRC Connection Request handler */
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr, 
      veNoOpr 
   }
};

/* After RRC Connection Setup Complete 
   eNB should send Security Command Request */
 
/*
*
*       Fun:   vePerfLIMRrcConSetCmpHndlr 
*
*       Desc:  RRC Conn. Setup Cmp Handler
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 vePerfLIMRrcConSetCmpHndlr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 vePerfLIMRrcConSetCmpHndlr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
   U16        ueIdx;

   TRC2(vePerfLIMRrcConSetCmpHndlr);

   /* Get Cell Cb */
   cellCb = veCb.cellCb[0];
   ueIdx = VE_GET_UE_IDX(datIndSdu->hdr.ueId);
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\nvePerfLIMRrcConSetCmpHndlr():\
               Invalid ueIdx(%d)\n",ueIdx));
#endif

      RETVALUE(RFAILED);
   }
   ueCb->rrcConnState = VE_RRC_CONNECTED;
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\n RRC Connection Complete Rcvd.\
            Ue:%d\n",datIndSdu->hdr.ueId));
#endif

   vePerfSndSecModeCmnd(ueCb);

   RETVALUE(ROK);
} /* vePerfLIMRrcConSetCmpHndlr */

/*
*
*       Fun:   vePerfSndSecModeCmnd 
*
*       Desc:  Security Mode Cmd Handler 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 vePerfSndSecModeCmnd 
(
VeUeCb *ueCb /* configuration */
)
#else
PUBLIC S16 vePerfSndSecModeCmnd(ueCb)
VeUeCb *ueCb; /* configuration */
#endif
{
   NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;
   S16   ret;
   NhuUeCfgInfo  *ueCfg;

   TRC2(vePerfSndSecModeCmnd);

   ueCfg = NULLP; 

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\n RRC: vePerfSndSecModeCmnd:\
            Building Security Mode Command ueId=%d\n", ueCb->crnti));
#endif

   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_SECURITYMODECMD);

   nhuDatReqEvntSdu->sdu.ueMsg.isMsgDed = 1;
   nhuDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.isMsgEncoded = FALSE;
   VE_FILL_TKN_UINT(nhuDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.\
         m.dcchMsg.message.val.c1.choice, C4_SECURITYMODECMD);

   /* Build RRC Security Mode Command Message */
   ret = vePerfFillRRC_SecModeCmd(nhuDatReqEvntSdu, ueCb);
   if (ret != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "vePerfSndRrcSecModeCmd:vePerfFillRRC_SecModeCmd failed.\n"));
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }
   /* Fill Config values */
   nhuDatReqEvntSdu->sdu.isUeCfgPres = TRUE;
   /*- Fill the UE configuration information --*/
   ueCfg = (&(nhuDatReqEvntSdu->sdu.ueCfg));
   ret = vePerfFillNhuUeCfgInfo(nhuDatReqEvntSdu, ueCfg, ueCb, C4_SECURITYMODECMD);   
   if(ROK != ret)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "vePerfSndRrcSecModeCmd: veSndUENhuDatReq failed.\n"));
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   if (RFAILED == veSndUENhuDatReq(nhuDatReqEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "vePerfSndRrcSecModeCmd: veSndUENhuDatReq failed.\n"));
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /*vePerfSndSecModeCmnd*/

/*
*
*       Fun:   vePerfFillRRC_SecModeCmd
*
*       Desc:  This function fills Downlink Security Mode Command
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_perf_li.c
*
*/
#ifdef ANSI
PUBLIC S16 vePerfFillRRC_SecModeCmd
(
NhuDatReqSdus *nhuDatReqSdu,
VeUeCb *ueCb
)
#else
PUBLIC S16 vePerfFillRRC_SecModeCmd(nhuDatReqSdu, ueCb)
NhuDatReqSdus *nhuDatReqSdu;
VeUeCb *ueCb;
#endif
{
   NhuDL_DCCH_Msg     *dlMsg = NULLP;
   NhuSecModeCmd      *secModeCmd = NULLP;
   NhuSecConfigSMC    *securityCfg = NULLP;

   TRC2(vePerfFillRRC_SecModeCmd);

   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = VE_PDCP_ID_SRB1;
   dlMsg = &(nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   VE_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(dlMsg->message.choice, DL_DCCH_MSGTYP_C4);
   VE_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C4_SECURITYMODECMD);

   /* SecurityModeCommand  */
   secModeCmd = &(dlMsg->message.val.c1.val.securityModeCmd);

   VE_FILL_TKN_UINT(secModeCmd->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(secModeCmd->rrc_TrnsxnId, VE_VAL_ONE);
   VE_FILL_TKN_UINT(secModeCmd->criticalExtns.choice, CRITICALEXTNS_C18);
   VE_FILL_TKN_UINT(secModeCmd->criticalExtns.val.c1.choice, C18_SECURITYMODECMD_R8);
   VE_FILL_TKN_UINT(secModeCmd->criticalExtns.val.c1.val.securityModeCmd_r8.\
         pres, PRSNT_NODEF);

   /* SecurityConfigSMC */
   securityCfg = &(secModeCmd->criticalExtns.val.c1.val.securityModeCmd_r8.\
         securityConfigSMC);

   VE_FILL_TKN_UINT(securityCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.cipheringAlgorithm,\
         ueCb->secCfg.ciphAlgo);
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.integrityProtAlgorithm,\
         ueCb->secCfg.intgAlgo);

   RETVALUE(ROK);
} /* End of vePerfFillRRC_SecModeCmd */

/*
*
*       Fun:   vePerfFillNhuUeCfgInfo
*
*       Desc:  This function fills UeCfgInfo in event structure
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_perf_li.c
*
*/
#ifdef ANSI
PUBLIC S16  vePerfFillNhuUeCfgInfo 
(
Ptr           evnt,
NhuUeCfgInfo  *ueCfg,
VeUeCb        *ueCb,
U8            msgType
)
#else
PUBLIC S16 vePerfFillNhuUeCfgInfo(evnt, ueCfg, ueCb, msgType)
Ptr           evnt;
NhuUeCfgInfo  *ueCfg;
VeUeCb        *ueCb;
U8            msgType;
#endif
{
   NhuRbCfgList  *rbCfgList = NULLP;
   U8            expCfgBitMask = 0;
   U8            noRAB = 0;

   TRC2(vePerfFillNhuUeCfgInfo);

   switch(msgType)
   {
      case C3_RRCCONSETUP:
         /*  RRC Connection Setup Msg. */ 
         expCfgBitMask = (NHU_EXP_CFG_RB_CFG |\
               NHU_EXP_CFG_TRCH_CFG | NHU_EXP_CFG_UE_CFG);
         /* number of RABs to be cfgd. */
         noRAB = VE_VAL_ONE;
         break;

      case C4_RRCCONRECFGN:
         /* RRC Reconfiguration Msg. */
         expCfgBitMask = (NHU_EXP_CFG_RB_CFG | NHU_EXP_CFG_TRCH_CFG);
         /* number of RABs to be cfgd. */
         noRAB = ueCb->rbInfo.numOfRbCfgd;
         break;

      case C4_SECURITYMODECMD:
         /* Security Mode Command */ 
         expCfgBitMask = NHU_EXP_CFG_SEC_CFG;
         /* number of RABs to be cfgd. */
         noRAB = VE_VAL_ONE;
         break;

      default:
         break;
   }

   /* filling UeCfg type*/
   ueCfg->ueCfgBitMsk = expCfgBitMask;

   /* RB Configuration - ADD/MODIFY/RELEASE */
   if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_RB_CFG)
   {
      /* NHU_CFG_RB_ADD hardcoded here; since RRC internally would free
       * resources (in lower layers as well) when Release complete is 
       * recvd. from UE */
      ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_ADD;
      /* ADD RB Configuration */
      if (ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_ADD)
      {
         rbCfgList = &(ueCfg->rbCfgAllList.rbToAddCfgList);
         rbCfgList->numOfRbCfgInfo = noRAB;
         VE_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * noRAB),\
               &(rbCfgList->rbCfgInfoList));
         /* fill further cfg info */ 
         veFillUeAddRbCfg(&rbCfgList, ueCb, msgType);
      }
   }

   /* Transport Channel Configuration */
   if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_TRCH_CFG)
   {
      veFillTrChCfg(&(ueCfg->trChCfg));
   }


   /* Security Channel Configuration */
   if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_SEC_CFG)
   {
      vePerfFillSecCfg(&(ueCfg->secCfg), ueCb);
   }

   /* UE Info Configuration */
   if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_UE_CFG)
   {
      ueCfg->ueInfo.newCrnti = ueCb->crnti;
   }

   RETVALUE(ROK);
} /*-- End of veFillNhuUeCfgInfo --*/



/*
*
*       Fun:   vePerfFillSecCfg
*
*       Desc:  This function fills Security Configuration
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_perf_li.c
*
*/
#ifdef ANSI
PUBLIC S16 vePerfFillSecCfg
(
NhuSecurityCfg     *secCfg,
VeUeCb             *ueCb
)
#else
PUBLIC S16 vePerfFillSecCfg(secCfg, ueCb)
NhuSecurityCfg     *secCfg;
VeUeCb             *ueCb;
#endif
{
   U8 idx = 0;

   TRC2(veFillSecCfg);

   secCfg->secCfgBitMsk = (NHU_SEC_CIPHER_ALGO | NHU_SEC_INTG_ALGO);
   secCfg->intgCfgInfo.action = NHU_ACTION_ADD;

   /* Intg Cfg */
   secCfg->intgCfgInfo.intgAlgo = 0; /* EEAO */
   for(idx = 0; idx < VE_INTG_KEY_LEN; idx++)
   {
      secCfg->intgCfgInfo.intgKey[idx] = 0; /* Assuming we are not going to use
      */
   }
   /* Ciph Cfg */
   secCfg->cipherCfgInfo.ciphAlgo = 0;
   for(idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
   {
      secCfg->cipherCfgInfo.cipherCpKey[idx] = 0;
      secCfg->cipherCfgInfo.cipherUpKey[idx] = 0;
   }


   RETVALUE(ROK);
} /* End of veFillSecCfg */

/*
*
*       Fun:   vePerfLIMRrcSecModCmpHndlr 
* 
*       Desc:  Security Mode Complete Handler 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16  vePerfLIMRrcSecModCmpHndlr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 vePerfLIMRrcSecModCmpHndlr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
   U16        ueIdx;

   TRC2(vePerfLIMRrcSecModCmpHndlr);

   cellCb = veCb.cellCb[0];
   ueIdx = VE_GET_UE_IDX(datIndSdu->hdr.ueId);
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\nvePerfLIMRrcSecModCmpHndlr(): Invalid ueIdx(%d)\n", ueIdx));
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "**** \
         vePerfLIMRrcSecModCmpHndlr : Received Sec Mode Comp ueId(%d)\
****\n", datIndSdu->hdr.ueId));
#endif

   /* Trigger RRC DL Connection Reconfiguration message*/
   vePerfSndRrcRecfgReq(ueCb);

   RETVALUE(ROK);
} /*vePerfLIMRrcSecModCmpHndlr */

/*
*
*       Fun:   vePerfSndRrcRecfgReq 
*
*       Desc:  RRC Recfg Req  
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 vePerfSndRrcRecfgReq
(
VeUeCb        *ueCb
)
#else
PUBLIC S16 vePerfSndRrcRecfgReq(ueCb)
VeUeCb        *ueCb;
#endif
{
   S16 ret = RFAILED;

   NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;
   NhuUeCfgInfo  *ueCfg = NULLP;

   TRC2(vePerfSndRrcRecfgReq);

   /* Allocate memory for event structure */
   VE_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));

   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_RRCCONRECFGN);

   /* Build RRC Reconfig Message */
   ret = vePerfFillNhuRrcConReCfgn(ueCb, nhuDatReqEvntSdu);
   if (ret != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "vePerfSndRrcRecfgReq:vePerfFillNhuRrcConReCfgn failed.\n"));
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   /* Fill Config values */
   nhuDatReqEvntSdu->sdu.isUeCfgPres = TRUE;
   /*- Fill the UE configuration information --*/
   ueCfg = &(nhuDatReqEvntSdu->sdu.ueCfg);
   ret = vePerfFillNhuUeCfgInfo(nhuDatReqEvntSdu, ueCfg, ueCb, C4_RRCCONRECFGN);
   if (ret != ROK)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "vePerfSndRrcRecfgReq: vePerfFillNhuUeCfgInfo failed.\n"));
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   /* Send RRC Connection Setup through NhuDatRsp Primitive*/
   if (RFAILED == veSndUENhuDatReq(nhuDatReqEvntSdu))
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "vePerfSndRrcRecfgReq: veSndUENhuDatReq failed.\n"));
      VE_FREEEVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* vePerfSndRrcRecfgReq */

/*
*
*       Fun:   vePerfFillNhuRrcConReCfgn
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_perf_li.c
*
*/
#ifdef ANSI
PUBLIC S16 vePerfFillNhuRrcConReCfgn
(
VeUeCb        *ueCb,
NhuDatReqSdus *nhDatReqEvntSdu
)
#else
PUBLIC S16 vePerfFillNhuRrcConReCfgn(ueCb, nhDatReqEvntSdu)
VeUeCb        *ueCb;
NhuDatReqSdus *nhDatReqEvntSdu;
#endif
{
   NhuDL_DCCH_Msg *dcchMsg = NULLP;
   NhuRRCConRecfgn *rrcConReCfg = NULLP;
   NhuRRCConRecfgn_r8_IEs *conRecfgIEs = NULLP;

   TRC2(vePerfFillNhuRrcConReCfgn);

   nhDatReqEvntSdu->sdu.ueMsg.isMsgDed = 1;
   nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.isMsgEncoded = FALSE;

   nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = VE_PDCP_ID_SRB1;
   dcchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   VE_FILL_TKN_UINT(dcchMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(dcchMsg->message.choice, DL_DCCH_MSGTYP_C4);
   VE_FILL_TKN_UINT(dcchMsg->message.val.c1.choice, C4_RRCCONRECFGN);

   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   VE_FILL_TKN_UINT(rrcConReCfg->pres, PRSNT_NODEF);

   VE_FILL_TKN_UINT(rrcConReCfg->rrc_TrnsxnId, VE_VAL_ONE);
   VE_FILL_TKN_UINT(rrcConReCfg->criticalExtns.choice, CRITICALEXTNS_C12);
   VE_FILL_TKN_UINT(rrcConReCfg->criticalExtns.val.c1.choice,\
         C12_RRCCONRECFGN_R8);

   conRecfgIEs = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);


   VE_FILL_TKN_UINT(conRecfgIEs->pres, PRSNT_NODEF);

   vePerfFillRRC_ReCfg_ERABSetupLst(nhDatReqEvntSdu, ueCb,\
         conRecfgIEs);
   /* Bit Rate Set */
   ueCb->rbInfo.aggBr.dl = VE_PERF_AGGR_DL;
   ueCb->rbInfo.aggBr.ul = VE_PERF_AGGR_UL;

   /* MAC-MainConfig */
   veFillNhuMACMainCfg(&conRecfgIEs->radioResourceConfigDedicated, \
         (Ptr)nhDatReqEvntSdu, VE_PDCP_ID_DRB1);

   /* PhysicalConfigDedicated */
   veFillRadResPhyCfgDed(&conRecfgIEs->radioResourceConfigDedicated, \
         (Ptr)nhDatReqEvntSdu, VE_PDCP_ID_DRB1, ueCb);

   RETVALUE(ROK);
} /* End of veFillNhuRrcConReCfgn */

/*
*
*       Fun:   vePerfFillRRC_ReCfg_ERABSetupLst
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_perf_li.c
*
*/
#ifdef ANSI
PUBLIC S16 vePerfFillRRC_ReCfg_ERABSetupLst
(
Ptr           evnt,
VeUeCb        *ueCb,
NhuRRCConRecfgn_r8_IEs *reCfgIes
)
#else
PUBLIC S16 vePerfFillRRC_ReCfg_ERABSetupLst(evnt, ueCb, reCfgIes)
Ptr           evnt;
VeUeCb        *ueCb;
NhuRRCConRecfgn_r8_IEs *reCfgIes;
#endif
{
   NhuRadioResourceConfigDedicated           *rrcCfgDed = NULLP;
   NhuDRB_ToAddMod                           *drbIe = NULLP;
   NhuLogChannelConfigul_SpecificParams      *chCfgParams = NULLP;

   U16 noComp = 0;
   U16 idx = 0;
   U8  rbIdx = 0;

   TRC2(vePerfFillRRC_ReCfg_ERABSetupLst);

   noComp = VE_PERF_NO_DRBS;
   /* initializing */
   rrcCfgDed = &(reCfgIes->radioResourceConfigDedicated);

   VE_FILL_TKN_UINT(rrcCfgDed->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(rrcCfgDed->drb_ToAddModLst.noComp, noComp);

   /* Allocate memory for each Rab */
   VE_GET_MEM(evnt, (sizeof(NhuDRB_ToAddMod) * noComp),\
         &(rrcCfgDed->drb_ToAddModLst.member));

   for(idx = 0; idx < noComp; idx++)
   {
      /* Update  local UE Cb 
       * We assume that drb start */
      rbIdx = ueCb->rbInfo.numOfRbCfgd;
      /* increment numbers of DRBs Cfg */
      ueCb->rbInfo.numOfRbCfgd++;
      /* Assign the RB type appropriately */
      ueCb->rbInfo.rab[rbIdx].rbType = CM_LTE_DRB; 
      ueCb->rbInfo.rab[rbIdx].rbId = (rbIdx + 3); /* starting from 3 .. 10 */
      ueCb->rbInfo.rab[rbIdx].eRABId = VE_PERF_ERAB_ID;
      ueCb->rbInfo.rab[rbIdx].pdcpId = (rbIdx + 3);      
      ueCb->rbInfo.rab[rbIdx].qci = VE_PERF_ERAB_QCI;      

      ueCb->rbInfo.rab[rbIdx].mbr.dl = VE_PERF_MBR_DL;  
      ueCb->rbInfo.rab[rbIdx].mbr.ul = VE_PERF_MBR_UL;
      ueCb->rbInfo.rab[rbIdx].gbr.dl = VE_PERF_GBR_DL;
      ueCb->rbInfo.rab[rbIdx].gbr.ul = VE_PERF_GBR_UL;

      /* Update DRB to ADD Mod List */
      {
         drbIe = &(rrcCfgDed->drb_ToAddModLst.member[idx]);
         VE_FILL_TKN_UINT(drbIe->pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(drbIe->eps_BrIdentity, ueCb->rbInfo.rab[rbIdx].eRABId);
         VE_FILL_TKN_UINT(drbIe->drb_Identity, ueCb->rbInfo.rab[rbIdx].rbId);

         /* PDCP-Config */ 
         VE_FILL_TKN_UINT(drbIe->pdcp_Config.pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(drbIe->pdcp_Config.discardTimer,\
               veDfltDiscardTimer);
         VE_FILL_TKN_UINT(drbIe->pdcp_Config.discardTimer,\
               veDfltDiscardTimer);
         /* rlc_AM */
         VE_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_AM.pres,\
               PRSNT_NODEF);
         VE_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_AM.statusReportReqd,\
               veDfltStatusReportReqd);
         /* headerCompression - NOT USED */
         VE_FILL_TKN_UINT(drbIe->pdcp_Config.headerCompression.choice,\
               HEADERCOMPRESSION_NOTUSED);

         /* RLC-Config */
         VE_FILL_TKN_UINT(drbIe->rlc_Config.choice, RLC_CONFIG_AM);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.pres, PRSNT_NODEF);
         /* UL-AM-RLC */
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.pres, PRSNT_NODEF);
         /* Poll Timer  */
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.t_PollRetransmit,\
               veDfltDrbTPollRetx);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.pollPDU,\
               veDfltDrbTPollPDU);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.pollByte,\
               veDfltDrbTPollByte);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.maxRetxThreshold,\
               veDfltDrbTMaxRetxThres);
         /* DL-AM-RLC */
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.dl_AM_RLC.pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.dl_AM_RLC.t_Reordering,\
               veDfltDrbTReordrng);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.dl_AM_RLC.t_StatusProhibit,\
               veDfltDrbTStatProh);

         /* logicalChannelIdentity  from 3 .. 10 */
         VE_FILL_TKN_UINT(drbIe->logicalChannelIdentity,\
               ueCb->rbInfo.rab[rbIdx].rbId);
         /* LogicalChannelConfig */
         VE_FILL_TKN_UINT(drbIe->logicalChannelConfig.pres, PRSNT_NODEF);
         chCfgParams = &(drbIe->logicalChannelConfig.ul_SpecificParams);
         VE_FILL_TKN_UINT(chCfgParams->pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(chCfgParams->priority, PRSNT_NODEF);
         VE_FILL_TKN_UINT(chCfgParams->prioritisedBitRate, PRSNT_NODEF);
         VE_FILL_TKN_UINT(chCfgParams->bucketSizeDuration, PRSNT_NODEF);
         VE_FILL_TKN_UINT(chCfgParams->logicalChannelGroup, PRSNT_NODEF);
      }
   }

   RETVALUE(ROK);
} /* vePerfFillRRC_ReCfg_ERABSetupLst */

/*
*
*       Fun:   vePerfLIMRrcReconfigCmpHndlr 
*
*       Desc:  RRC Reconfig Complete Handler 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 vePerfLIMRrcReconfigCmpHndlr
(
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 vePerfLIMRrcReconfigCmpHndlr(datIndSdu)
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   VeCellCb *cellCb = NULLP; 
   VeUeCb *ueCb = NULLP;
   U16     ueIdx, i;

   TRC2(vePerfLIMRrcReconfigCmpHndlr);

   cellCb = veCb.cellCb[0];
   ueIdx = VE_GET_UE_IDX(datIndSdu->hdr.ueId);
   if(ueIdx < cellCb->maxUeSupp)
   {
      ueCb = cellCb->ueCbLst[ueIdx];
   }
   else
   {
      VE_DBG_ERROR((VE_PRNT_BUF,
               "\nvePerfLIMRrcSecModCmpHndlr(): Invalid ueIdx(%d)\n",ueIdx));
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
            \n RRC Recfg Complete Message Rcvd. from Ue:%d\n", datIndSdu->hdr.ueId));
#endif

   /*Send Command to DATA APP for local Data Generation towards PDCP creation*/
#if (defined(VE_PERF_DL_DATA) || defined(VE_LNX_PCAP_PERF))
   veNumofRecfgCmpRecvd++;
   if (veNumofRecfgCmpRecvd == VE_MAX_UE)
   {
      for (i = veCb.cellCb[0]->veDfltStartRnti; i<(veCb.cellCb[0]->veDfltStartRnti+VE_MAX_UE); i++)
      {
         ueCb = NULLP;
         ueIdx = VE_GET_UE_IDX(i);
         if(ueIdx < cellCb->maxUeSupp)
         {
            ueCb = cellCb->ueCbLst[ueIdx];
         }
         if (ueCb)
         {
            printf("veSndDataStart: Sending Ind to UeID=%d\n",ueCb->crnti);
            veSndDataStart(ueCb, VE_NOTIFY_SATRT_DATA);
         }
      }
   }
#endif

   RETVALUE(ROK);
} /* vePerfLIMRrcReconfigCmpHndlr */

/*
*
*       Fun:   veSndDataStart 
*
*       Desc:  Send Traffic
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 veSndDataStart 
(
VeUeCb *ueCb,
U8 evnt
)
#else
PUBLIC S16 veSndDataStart(ueCb, evnt)
VeUeCb *ueCb;
U8 evnt;
#endif
{
   Pst pst;
   Buffer *mBuf = NULLP;
   CmLtePdcpId pdcpId;

   S16     ret1;

   TRC2(veSndDataStart);

   cmMemset((U8 *)&pst,0,sizeof(Pst));

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
            \n ***** Sending Data Start Request to Data App ****\n"));
#endif
   pst.dstEnt = ENTEU;
   pst.srcEnt = ENTVE;
   pst.srcProcId = veCb.init.procId;
   pst.dstProcId = veCb.init.procId;
   pst.dstInst = veCb.init.inst;
   pst.srcInst = veCb.init.inst;
   pst.prior   = PRIOR0;
   pst.route   = RTE_PROTO;
   pst.region = veCb.init.region;
   pst.pool   = veCb.init.pool;

   pst.event = (Event) VE_NOTIFY_SATRT_DATA;

   mBuf = NULLP;

   if((ret1 = SGetMsg(pst.region, pst.pool, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if(ret1 != ROK)
      {
         SLogError(pst.srcEnt, pst.rcInst, pst.srcProcId,
               __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUTXXX, (ErrVal)0, "SGetMsg() failed");
      }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }


   pdcpId.ueId    = ueCb->crnti;
   pdcpId.cellId  = ueCb->cellId;
   pdcpId.rbId    = 3; /* RB ID used as 3 Need to look latyer why can not be 1
   */
   pdcpId.rbType  = 1;

   cmPkLtePdcpId(&pdcpId, mBuf);

   RETVALUE(SPstTsk(&pst,mBuf));
}

/*
*
*       Fun:   vePerfStartData 
*
*       Desc:  Start Data Traffic 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 vePerfStartData 
(
Pst *pst,
Buffer *mBuf,
Mem *sMem
)
#else
PUBLIC S16 vePerfStartData( pst, mBuf, sMem)
Pst *pst;
Buffer *mBuf;
Mem *sMem;
#endif
{
   CmLtePdcpId pdcpId;
   U8    idx = 0;

   TRC2(vePerfStartData);

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
            \n ***** Received Event from Relay to start Data ****\n"));
#endif

   cmMemset((U8 *)&pdcpId,0,sizeof(CmLtePdcpId));

   /* Unpk CellId, Ue Id details and mark the state to Ready to send Data */
   cmUnpkLtePdcpId(&pdcpId, mBuf);

   for(idx = 0; idx < VE_MAX_UE; idx++)
   {
      if(veEuCb.ueLst[idx].rbType == 0 )
      {
         /* Update the UECb List with state ready to send data */
         veEuCb.ueLst[idx].cellId =  pdcpId.cellId;
         veEuCb.ueLst[idx].ueId   =  pdcpId.ueId;
         veEuCb.ueLst[idx].rbId   =  pdcpId.rbId;
         veEuCb.ueLst[idx].rbType = 1;  /* rbType is used to send data */ 
         break;
      } 
   }

   if(idx == VE_MAX_UE)
   {
      /* List is full */
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
               \n  *****  All UEs are busy ****\n"));
#endif
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }

   SPutMsg(mBuf);

   /* Start timer if not running */
   if(veEuCb.loadTimers[0].tmrEvnt != VE_PERF_TMR_EVNT_LOAD)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
               \n *****  starting Load Timer ****\n"));
#endif
      vePerfStartTmr(VE_PERF_TMR_EVNT_LOAD);
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
               \n *****  Load Timer is running ****\n"));
#endif
   }
   RETVALUE(ROK);

}/* vePerfStartData */

/*
*
*       Fun:   vePerfActvTmr 
*
*       Desc:  Activate Timer
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 vePerfActvTmr
(
)
#else
PUBLIC S16 vePerfActvTmr(Void)
#endif
{
   TRC2(vePerfActvTmr);

   cmPrcTmr(&veEuCb.tqCp, veEuCb.tq, (PFV) vePerfTmrExp);

   RETVALUE(ROK);
} /* end of vePerfActvTmr*/

/*
*
*       Fun:   vePerfStartTmr 
*
*       Desc:  Start Timer 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC Void vePerfStartTmr
(
S16    tmrEvnt      /* Timer Event */
)
#else
PUBLIC Void vePerfStartTmr (tmrEvnt)
S16    tmrEvnt;      /* Timer Event */
#endif 
{
   CmTmrArg   arg;   

   arg.evnt = tmrEvnt;
   arg.wait = 0; /*Initial Value*/
   TRC2(vePerfStartTmr);

   switch(tmrEvnt)
   {
      case VE_PERF_TMR_EVNT_LOAD:
         {
            arg.wait = VE_PERF_LOAD_TMR_VAL;
            arg.timers = &veEuCb.loadTimers[0];
            arg.max    = VE_PERF_MAX_TMR;  

            break;
         }
      default:
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
                  \n ***** Un expected Event ****\n"));
#endif
         break;
   }

   if (arg.wait != 0)
   {
      arg.tqCp = &veEuCb.tqCp;
      arg.tq   = veEuCb.tq;
      arg.cb   = (PTR)&veEuCb;
      arg.evnt = tmrEvnt;
      arg.tNum = 0;
      cmPlcCbTq(&arg);
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
               \n *****  Timer is not started...wait value is in correct ****\n"));
#endif
   }

   RETVOID;
}

/*
*
*       Fun:   vePerfTmrExp 
*
*       Desc:  Timer Expiry Function
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC Void vePerfTmrExp
(
PTR   pCb,       /* protocol control block */
S16   tmrEvnt    /* Timer Event */
)
#else
PUBLIC Void vePerfTmrExp(pCb, tmrEvnt) 
PTR   pCb;       /* protocol control block */
S16   tmrEvnt;    /* Timer Event */
#endif
{
   TRC2(vePerfTmrExp);

   vePerfStartTmr(VE_PERF_TMR_EVNT_LOAD);
   vePerfSndData();

   RETVOID;
}

/*
*
*       Fun:   vePerfSndData 
*
*       Desc:  Send Data
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC Void vePerfSndData
(
Void
)
#else
PUBLIC Void vePerfSndData(Void)
#endif 
{
   U16  noOfUes, noOfSdus;
   Buffer *mBuf;
   U8 ret; 
   CmLtePdcpId *pdcpId;
   VeLiSapCb            *lSapCb = NULLP;
   /*U32      memInfo;*/

   TRC2(vePerfSndData);
   for (noOfUes = 0; noOfUes < VE_MAX_UE; noOfUes++)
   {
      if(veEuCb.ueLst[noOfUes].rbType == 1)  /* rbType will be set =1 only after get mes from
                                          relay  to start data */
      { 
         for(noOfSdus = VE_PERF_NO_SDUS; noOfSdus; noOfSdus--)
         {
            if ((ret = SGetMsg(veEuCb.mem.region, 3, &mBuf)) != ROK)
            {
               VE_DBG_ERROR((VE_PRNT_BUF,
                        "\n ****** Mem Alloc Failure *****\n"));
               RETVOID;
            }
            if((SAddPstMsgMult((Data *)loadData , sizeof(loadData)-1, mBuf)) != ROK)
            {
               VE_DBG_ERROR((VE_PRNT_BUF,
                        "\n ****** Mem Alloc Failure *****\n"));
               RETVOID;
            }  
            lSapCb = veEuCb.lsapLst[VE_PERF_PDCP_SAP_ID];     
            VE_ALLOC(&pdcpId, sizeof(CmLtePdcpId)); 
            pdcpId->cellId =  veEuCb.ueLst[noOfUes].cellId;
            pdcpId->ueId   =  veEuCb.ueLst[noOfUes].ueId;
            pdcpId->rbId   =  3; /* VE_PERF_DRB_ID; */
            pdcpId->rbType =  CM_LTE_DRB;
            VeLiPjuDatReq(&lSapCb->pst, lSapCb->spId, pdcpId,
                  veEuCb.sduCntr++, mBuf);

         }   
      } 
   }
   if (veEuCb.sduCntr > 500000)
   {
      veEuCb.sduCntr = 1;
#ifndef SS_SEUM_CAVIUM
   /* SRegInfoShow(0, &memInfo);*/
#endif
   }
   
   RETVOID;
}

/*
*
*       Fun:   VeLiPerfPjuDatInd 
*
*       Desc:  PJU Data Ind.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 VeLiPerfPjuDatInd 
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
Buffer         *mBuf
)
#else
PUBLIC S16 VeLiPerfPjuDatInd(pst, suId, pdcpId, mBuf)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
Buffer         *mBuf;
#endif
{
   TRC2(VeLiPerfPjuDatInd);

   /* Drop the packet and deallocate the memory */
   SPutSBuf(pst->region, pst->pool, (Data *) pdcpId, (Size)sizeof(pdcpId));
   SPutMsg(mBuf); 
   mBuf = NULLP;
   RETVALUE(ROK);
}

/*
*
*       Fun:   vePerfHdlNhuDatInd 
*
*       Desc:  NHU Data Ind.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 vePerfHdlNhuDatInd
(
Pst     *pst,          /* post structure */
SuId     suId,
NhuDatIndSdus *datIndSdu/* configuration */
)
#else
PUBLIC S16 vePerfHdlNhuDatInd(pst, suId, datIndSdu)
Pst     *pst;          /* post structure */
SuId     suId;
NhuDatIndSdus *datIndSdu;          /* configuration */
#endif
{
   NhuUL_CCCH_Msg  *ulCcchPdu = NULLP;  /* UL CCCH structure */
   NhuUL_DCCH_Msg  *ulDcchPdu = NULLP;  /* UL DCCH structure */
   U8   messageType = 0;
   U8   messageCat = 0;

   TRC2(vePerfHdlNhuDatInd);

   /* If message is received on CCCH */
   messageCat = datIndSdu->sdu.msgCategory;

   if (NHU_MSG_ULCCCH == messageCat)
   {
      ulCcchPdu = &datIndSdu->sdu.m.ulCcchMsg.ccchMsg;
      if ((FALSE == ulCcchPdu->pres.pres) )
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veHdlNhuDatInd : Unknown Msg. Received.\n"));
         RETVALUE(RFAILED);
      } 

      messageType = ulCcchPdu->message.val.c1.choice.val;

      /*Call the state machine*/
      vePerfNhuLIMFSM[messageCat][messageType](datIndSdu);

   }  /* end of if (NHU_MSG_ULCCCH == datIndSdu.sdu.msgCategory) */
   /* If message is received on DCCH */
   else if (NHU_MSG_ULDCCH == datIndSdu->sdu.msgCategory)
   { 
      ulDcchPdu = &(datIndSdu->sdu.m.ulDcchMsg.dcchMsg);
      if ((FALSE == ulDcchPdu->pres.pres) )
      {
         VE_DBG_ERROR((VE_PRNT_BUF,"veHdlNhuDatInd : Message not Present.\n"));
         RETVALUE(RFAILED);
      }

      messageType = ulDcchPdu->message.val.c1.choice.val;

      /*Call the state machine*/
      vePerfNhuLIMFSM[messageCat][messageType]((NhuDatIndSdus *)datIndSdu);
   }

   RETVALUE(ROK);
}/* veHdlNhuDatInd */
/*
*
*       Fun:   PxLiPjuDatInd 
*
*       Desc:  PJU Data Ind
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 PxLiPjuDatInd
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
Buffer         *mBuf
)
#else
PUBLIC S16 PxLiPjuDatInd(pst, suId, pdcpId, mBuf)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
Buffer         *mBuf;
#endif
{
   U32 crntSysTicks;
   MsgLen msgLen;

   TRC2(PxLiPjuDatInd);

   /* Calculat the Data Received */
   if(!noPkts)
   {
      SGetSysTime(&noSysTicks); 
      SGetDateTime(&stDt);
      SFndLenMsg(mBuf,&msgLen); 
   }
   noPkts++;
   SFndLenMsg(mBuf,&msgLen);
   noBytes+=msgLen;
   if( noPkts > 100000)
   {
      SGetSysTime(&crntSysTicks); 
      SGetDateTime(&enDt);
      secDiff = (enDt.sec + (enDt.min * 60) + (enDt.hour * 3600)) - (stDt.sec + (stDt.min * 60) + (stDt.hour * 3600));
      bpsApp = noBytes * 8/secDiff;
#ifdef DEBUGP
#ifdef ALIGN_64BIT
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
               \n *****  UL Data: bits per sec APP (%d)****\n", bpsApp));
#else
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\
               \n *****  UL Data: bits per sec APP (%ld)****\n", bpsApp));
#endif
#endif
      noBytes = 0;
      noPkts  = 0;
      noSysTicks = crntSysTicks;
      bpsApp = 0;
   }
#ifdef VE_LNX_PCAP_PERF
   /* Send the received IP packet to the eNb App */
   veAppRoutHndlSndPkt(mBuf);
#endif
   /* Release the Memroy */
   SPutSBuf(pst->region, pst->pool, (Data *) pdcpId, (Size)sizeof(pdcpId));
   SPutMsg(mBuf);

   RETVALUE(ROK);
} /* PxLiPjuDatInd */

/*
*
*       Fun:   PxLiPjuDatCfm 
*
*       Desc:  PJU Data Cfm.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 PxLiPjuDatCfm
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuDatCfmInfo  *datCfm
)
#else
PUBLIC S16 PxLiPjuDatCfm(pst, suId, pdcpId, datCfm)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
PjuDatCfmInfo  *datCfm;
#endif
{
   TRC2(PxLiPjuDatCfm);
   /* Calling VeLiPjuDatCfm */
   RETVALUE(VeLiPjuDatCfm(pst, suId, pdcpId, datCfm));
} /* PxLiPjuDatCfm */

/*
*
*       Fun:   PxLiPjuStaInd 
*
*       Desc:  PJU Status Ind.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 PxLiPjuStaInd
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuStaIndInfo  *staInd,
Buffer         *mBuf
)
#else
PUBLIC S16 PxLiPjuStaInd(pst, suId, pdcpId, staInd, mBuf)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
PjuStaIndInfo  *staInd;
Buffer         *mBuf;
#endif
{
   TRC2(PxLiPjuStaInd);
   /* Calling VeLiPjuStaInd */
   RETVALUE(VeLiPjuStaInd(pst, suId, pdcpId, staInd, mBuf));
} /* PxLiPjuStaInd */

/*
*
*       Fun:   PxLiPjuDatFwdInd 
*
*       Desc:  PJU Data Forward Ind.
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_perf_li.c 
*
*/
#ifdef ANSI
PUBLIC S16 PxLiPjuDatFwdInd
(
Pst            *pst,
SuId           suId,
CmLtePdcpId    *pdcpId,
PjuDatFwdIndInfo   *datFwdInd
)
#else
PUBLIC S16 PxLiPjuDatFwdInd(pst, suId, pdcpId, datFwdInd)
Pst            *pst;
SuId           suId;
CmLtePdcpId    *pdcpId;
PjuDatFwdIndInfo   *datFwdInd;
#endif
{
   TRC2(PxLiPjuDatFwdInd);
   /* Calling VeLiPjuDatFwdInd */
#ifdef LTE_HO_SUPPORT
   RETVALUE(VeLiPjuDatFwdInd(pst, suId, pdcpId, datFwdInd));
#endif
} /* PxLiPjuDatFwdInd */

#endif 

/********************************************************************30**
  
         End of file:     ve_perf_li.c@@/main/2 - Wed Dec  1 08:44:27 2010
  
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
