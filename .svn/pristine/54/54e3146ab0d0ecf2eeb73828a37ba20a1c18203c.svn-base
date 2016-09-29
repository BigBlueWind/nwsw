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

     Name:     LTE-RRC User
  
     Type:     C Source file
  
     Desc:     Build configuration and message

     Ret :     ROK - success
               RFAILED - failure

     File:     ve_bldmsg.c

     Sid:      ve_bldmsg.c@@/main/2 - Wed Dec  1 08:44:21 2010

     Prg:      

**********************************************************************/
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
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
#include "pju.h"           /* PJU interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#include "czt.h"           /* S1AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */



/* RRM related includes */
#include "ve.h" 
#include "lve.h"
#ifdef LTE_ENB_PERF
#include "ve_perf.h"
#endif

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

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
#include "cpj.x"           /* CPJ Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#include "czt.x"           /* S1AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */

/* RRM related includes */
#include "lve.x" 
#include "ve_eut.x" 
#include "ve.x" 
/* MSPD changes */
#ifndef MSPD
#include "ve_dflt.h"
#include "ve_dflt_pal.x"
#else
#include "ve_dflt_ms.x"
#endif


PUBLIC S16 veKDF ARGS(( U8  *secKey, U8  *strS, U8  *derKey));
PUBLIC S16 veExtractKeys ARGS(( VeUeSecCfg  *secCfg, TknStrBSXL  *secKey));
#ifdef VE_DONGWON_OAM
U32 enumMapNhuRlcPollByte[]=
{
   NHU_POLL_BYTE_KB25 ,
   NHU_POLL_BYTE_KB50 ,
   NHU_POLL_BYTE_KB75 ,
   NHU_POLL_BYTE_KB100 ,
   NHU_POLL_BYTE_KB125 ,
   NHU_POLL_BYTE_KB250 ,
   NHU_POLL_BYTE_KB375 ,
   NHU_POLL_BYTE_KB500 ,
   NHU_POLL_BYTE_KB750 ,
   NHU_POLL_BYTE_KB1000 ,
   NHU_POLL_BYTE_KB1250 ,
   NHU_POLL_BYTE_KB1500 ,
   NHU_POLL_BYTE_KB2000 ,
   NHU_POLL_BYTE_KB3000 ,
   NHU_POLL_BYTE_INF
};

U32 enumMapNhuTReOrdering[]=
{
   NHU_TREORD_MS0 ,
   NHU_TREORD_MS5 ,
   NHU_TREORD_MS10 ,
   NHU_TREORD_MS15 ,
   NHU_TREORD_MS20 ,
   NHU_TREORD_MS25 ,
   NHU_TREORD_MS30 ,
   NHU_TREORD_MS35 ,
   NHU_TREORD_MS40 ,
   NHU_TREORD_MS45 ,
   NHU_TREORD_MS50 ,
   NHU_TREORD_MS55 ,
   NHU_TREORD_MS60 ,
   NHU_TREORD_MS65 ,
   NHU_TREORD_MS70 ,
   NHU_TREORD_MS75 ,
   NHU_TREORD_MS80 ,
   NHU_TREORD_MS85 ,
   NHU_TREORD_MS90 ,
   NHU_TREORD_MS95 ,
   NHU_TREORD_MS100 ,
   NHU_TREORD_MS110 ,
   NHU_TREORD_MS120 ,
   NHU_TREORD_MS130 ,
   NHU_TREORD_MS140 ,
   NHU_TREORD_MS150 ,
   NHU_TREORD_MS160 ,
   NHU_TREORD_MS170 ,
   NHU_TREORD_MS180 ,
   NHU_TREORD_MS190 ,
   NHU_TREORD_MS200
};

U32 enumMapNhuMaxRetxThres[]=
{
   NHU_MAX_RETX_THRES_T1 ,
   NHU_MAX_RETX_THRES_T2 ,
   NHU_MAX_RETX_THRES_T3 ,
   NHU_MAX_RETX_THRES_T4 ,
   NHU_MAX_RETX_THRES_T6 ,
   NHU_MAX_RETX_THRES_T8 ,
   NHU_MAX_RETX_THRES_T16 ,
   NHU_MAX_RETX_THRES_T32
};

U32 enumMapNhuTPollReTx[]=
{
   NHU_TPOLL_RETX_MS5 ,
   NHU_TPOLL_RETX_MS10 ,
   NHU_TPOLL_RETX_MS15 ,
   NHU_TPOLL_RETX_MS20 ,
   NHU_TPOLL_RETX_MS25 ,
   NHU_TPOLL_RETX_MS30 ,
   NHU_TPOLL_RETX_MS35 ,
   NHU_TPOLL_RETX_MS40 ,
   NHU_TPOLL_RETX_MS45 ,
   NHU_TPOLL_RETX_MS50 ,
   NHU_TPOLL_RETX_MS55 ,
   NHU_TPOLL_RETX_MS60 ,
   NHU_TPOLL_RETX_MS65 ,
   NHU_TPOLL_RETX_MS70 ,
   NHU_TPOLL_RETX_MS75 ,
   NHU_TPOLL_RETX_MS80 ,
   NHU_TPOLL_RETX_MS85 ,
   NHU_TPOLL_RETX_MS90 ,
   NHU_TPOLL_RETX_MS95 ,
   NHU_TPOLL_RETX_MS100 ,
   NHU_TPOLL_RETX_MS105 ,
   NHU_TPOLL_RETX_MS110 ,
   NHU_TPOLL_RETX_MS115 ,
   NHU_TPOLL_RETX_MS120 ,
   NHU_TPOLL_RETX_MS125 ,
   NHU_TPOLL_RETX_MS130 ,
   NHU_TPOLL_RETX_MS135 ,
   NHU_TPOLL_RETX_MS140 ,
   NHU_TPOLL_RETX_MS145 ,
   NHU_TPOLL_RETX_MS150 ,
   NHU_TPOLL_RETX_MS155 ,
   NHU_TPOLL_RETX_MS160 ,
   NHU_TPOLL_RETX_MS165 ,
   NHU_TPOLL_RETX_MS170 ,
   NHU_TPOLL_RETX_MS175 ,
   NHU_TPOLL_RETX_MS180 ,
   NHU_TPOLL_RETX_MS185 ,
   NHU_TPOLL_RETX_MS190 ,
   NHU_TPOLL_RETX_MS195 ,
   NHU_TPOLL_RETX_MS200 ,
   NHU_TPOLL_RETX_MS205 ,
   NHU_TPOLL_RETX_MS210 ,
   NHU_TPOLL_RETX_MS215 , 
   NHU_TPOLL_RETX_MS220 ,
   NHU_TPOLL_RETX_MS225 ,
   NHU_TPOLL_RETX_MS230 ,
   NHU_TPOLL_RETX_MS235 ,
   NHU_TPOLL_RETX_MS240 ,
   NHU_TPOLL_RETX_MS245 ,
   NHU_TPOLL_RETX_MS250 ,
   NHU_TPOLL_RETX_MS300 ,
   NHU_TPOLL_RETX_MS350 ,
   NHU_TPOLL_RETX_MS400 ,
   NHU_TPOLL_RETX_MS450 ,
   NHU_TPOLL_RETX_MS500
};

U32 enumMapNhuTStatusProh[]=
{
   NHU_STATUS_PROH_MS0 ,
   NHU_STATUS_PROH_MS5 ,
   NHU_STATUS_PROH_MS10 ,
   NHU_STATUS_PROH_MS15 ,
   NHU_STATUS_PROH_MS20 ,
   NHU_STATUS_PROH_MS25 ,
   NHU_STATUS_PROH_MS30 ,
   NHU_STATUS_PROH_MS35 ,
   NHU_STATUS_PROH_MS40 ,
   NHU_STATUS_PROH_MS45 ,
   NHU_STATUS_PROH_MS50 ,
   NHU_STATUS_PROH_MS55 ,
   NHU_STATUS_PROH_MS60 ,
   NHU_STATUS_PROH_MS65 ,
   NHU_STATUS_PROH_MS70 ,
   NHU_STATUS_PROH_MS75 ,
   NHU_STATUS_PROH_MS80 ,
   NHU_STATUS_PROH_MS85 ,
   NHU_STATUS_PROH_MS90 ,
   NHU_STATUS_PROH_MS95 ,
   NHU_STATUS_PROH_MS100 ,
   NHU_STATUS_PROH_MS105 ,
   NHU_STATUS_PROH_MS110 ,
   NHU_STATUS_PROH_MS115 ,
   NHU_STATUS_PROH_MS120 ,
   NHU_STATUS_PROH_MS125 ,
   NHU_STATUS_PROH_MS130 ,
   NHU_STATUS_PROH_MS135 ,
   NHU_STATUS_PROH_MS140 ,
   NHU_STATUS_PROH_MS145 ,
   NHU_STATUS_PROH_MS150 ,
   NHU_STATUS_PROH_MS155 ,
   NHU_STATUS_PROH_MS160 ,
   NHU_STATUS_PROH_MS165 ,
   NHU_STATUS_PROH_MS170 ,
   NHU_STATUS_PROH_MS175 ,
   NHU_STATUS_PROH_MS180 ,
   NHU_STATUS_PROH_MS185 ,
   NHU_STATUS_PROH_MS190 ,
   NHU_STATUS_PROH_MS195 ,
   NHU_STATUS_PROH_MS200 ,
   NHU_STATUS_PROH_MS205 ,
   NHU_STATUS_PROH_MS210 ,
   NHU_STATUS_PROH_MS215 ,
   NHU_STATUS_PROH_MS220 ,
   NHU_STATUS_PROH_MS225 ,
   NHU_STATUS_PROH_MS230 ,
   NHU_STATUS_PROH_MS235 , 
   NHU_STATUS_PROH_MS240 ,
   NHU_STATUS_PROH_MS245 ,
   NHU_STATUS_PROH_MS250 ,
   NHU_STATUS_PROH_MS300 ,
   NHU_STATUS_PROH_MS350 ,
   NHU_STATUS_PROH_MS400 ,
   NHU_STATUS_PROH_MS450 ,
   NHU_STATUS_PROH_MS500
};

U32 enumMapNhuPollPdu[]=
{
   NHU_POLL_PDU_P4 ,
   NHU_POLL_PDU_P8 ,
   NHU_POLL_PDU_P16 ,
   NHU_POLL_PDU_P32 ,
   NHU_POLL_PDU_P128 ,
   NHU_POLL_PDU_P256 ,
   NHU_POLL_PDU_P384 ,
   NHU_POLL_PDU_INF
};

#endif /* VE_DONGWON_OAM */ 
/* ======================================= */
/* RRC Message Buiding Functions */
/* ======================================= */
/*
*
*       Fun:   veGetPMICfgIdx 
*
*       Desc:  Assign PMI Cfg Index
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c 
*
*/
#ifdef ANSI
PUBLIC S16 veGetPMICfgIdx
(
TknU32 *tkn
)
#else
PUBLIC S16 veGetPMICfgIdx(tkn)
TknU32 *tkn;
#endif
{   /* klock warning resolved */
   PRIVATE U8 pmiCfgIdx = 0;

   TRC2(veGetPMICfgIdx);

   tkn->pres = PRSNT_NODEF;

   switch(pmiCfgIdx)
   {
      case 0:
         tkn->val = 38;
         break;
      case 1:
         tkn->val = 48;
         break;
      case 2:
         tkn->val = 58;
         break;
      case 3:
         tkn->val = 68;
         break;
      default:
         VE_DBG_ERROR((VE_PRNT_BUF,"Could not assign valid pmiCfgIdx,"
                  " assigning zero\n"));
         pmiCfgIdx = 4;
         tkn->val = 0;
         break;
   }
   pmiCfgIdx++;

   RETVALUE(ROK);
}/* veGetPMICfgIdx */ 

/*
*
*       Fun:   veGetCqiPmiCfgIdx 
*
*       Desc:  Assign Cqi/PMI Cfg Index
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c 
*
*/
#ifdef ANSI
PUBLIC S16 veGetCqiPmiCfgIdx
(
TknU8 *tkn
)
#else
PUBLIC S16 veGetCqiPmiCfgIdx(tkn)
TknU8 *tkn;
#endif
{
   PRIVATE U8 cqiPmiCfgIdxIdx = 0;

   TRC2(veGetCqiPmiCfgIdx);

   tkn->pres = PRSNT_NODEF;

   switch(cqiPmiCfgIdxIdx)
   {
      case 0:
/* MSPD changes */
#ifdef MSPD
         tkn->val = 38;
         break;
      case 1:
         tkn->val = 39;
         break;
      case 2:
         tkn->val = 40;
         break;
      case 3:
         tkn->val = 41;
	 break;
      case 4:
         tkn->val = 42;
         break;
      case 5:
         tkn->val = 43;
         break;
      case 6:
         tkn->val = 44;
         break;
      case 7:
         tkn->val = 45;
         break;
      case 8:
         tkn->val = 46;
         break;
      case 9:
         tkn->val = 47;
         break;
#endif
      default:
         tkn->val = 154; /* Changed the value from 155 to 154*/
         VE_DBG_ERROR((VE_PRNT_BUF, "Could not assigned cqiPmiCfgIdxIdx,"
                  " assigning a reserved value\n"));
         cqiPmiCfgIdxIdx = 4;
         break;
   }
   cqiPmiCfgIdxIdx++;

   RETVALUE(ROK);
}/* veGetCqiPmiCfgIdx */ 

/*
*
*       Fun:   veGetSRCfgIdx 
*
*       Desc:  Assign SR Cfg Index
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c 
*
*/
#ifdef ANSI
PUBLIC S16 veGetSRCfgIdx
(
TknU8 *tkn
)
#else
PUBLIC S16 veGetSRCfgIdx(tkn)
TknU8 *tkn;
#endif
{
   PRIVATE U8 srCfgIdx = 0;

   TRC2(veGetSRCfgIdx);

   tkn->pres = PRSNT_NODEF;

   switch(srCfgIdx)
   {
      case 0:
/* MSPD changes */
#ifdef MSPD
         tkn->val = 17;
#else
         tkn->val = 42;
#endif
         break;
      case 1:
#ifdef MSPD
         tkn->val = 18;
#else
         tkn->val = 52;
#endif
         break;
      case 2:
#ifdef MSPD
         tkn->val = 19;
#else
         tkn->val = 62;
#endif
         break;
      case 3:
#ifdef MSPD
         tkn->val = 20;
#else
         tkn->val = 72;
#endif
         break;
#ifdef MSPD
      case 4:
         tkn->val = 21;
         break;
      case 5:
         tkn->val = 22;
         break;
      case 6:
         tkn->val = 23;
         break;
      case 7:
         tkn->val = 24;
         break;
      case 8:
         tkn->val = 25;
         break;
      case 9:
         tkn->val = 26;
         break;
#endif
      default:
         tkn->val = 154; /* Changed the value from 155 to 154*/
         VE_DBG_ERROR((VE_PRNT_BUF, "Could not assigned srCfgIdx,"
                  " assigning a reserved value\n"));
         srCfgIdx = 4;
         break;
   }
   srCfgIdx++;

   RETVALUE(ROK);
}/* veGetSRCfgIdx */ 



/*
*
*       Fun:   veFillRRC_ConSetup
*
*       Desc:  This function fills RRC Connection Setup Msg
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRC_ConSetup
(
NhuDatRspSdus *nhuDatRspSdu,
VeUeCb *ueCb
)
#else
PUBLIC S16 veFillRRC_ConSetup(nhuDatRspSdu, ueCb)
NhuDatRspSdus *nhuDatRspSdu;
VeUeCb *ueCb;
#endif
{
   NhuDL_CCCH_Msg *dlMsg = NULLP;
   NhuRRCConSetup *conSetUp = NULLP;
   NhuRadioResourceConfigDedicated *resCfgDed = NULLP;

   TRC2(veFillRRC_ConSetup);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRC_ConSetup: Building \
            Connection Setup Msg.\n"));
#endif

   if(NULLP == nhuDatRspSdu)
  {
     VE_DBG_ERROR((VE_PRNT_BUF,"veFillRRC_ConSetup : nhuDatRspSdu is NULL"));

     RETVALUE(RFAILED);
  }

   nhuDatRspSdu->sdu.msgCategory = NHU_MSG_DLCCCH;

   dlMsg = &(nhuDatRspSdu->sdu.m.dlCcchMsg.ccchMsg);

   VE_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(dlMsg->message.choice, DL_CCCH_MSGTYP_C3);
   VE_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C3_RRCCONSETUP);

   /* RRCConnectionSetup */
   conSetUp = &(dlMsg->message.val.c1.val.rrcConSetup);
   VE_FILL_TKN_UINT(conSetUp->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(conSetUp->rrc_TrnsxnId, VE_VAL_ONE);
   VE_FILL_TKN_UINT(conSetUp->criticalExtns.choice, CRITICALEXTNS_C14);
   /* RRCConnectionSetup-r8-IEs  */
   VE_FILL_TKN_UINT(conSetUp->criticalExtns.val.c1.choice, C16_RRCCONSETUP_R8);
   VE_FILL_TKN_UINT(conSetUp->criticalExtns.val.c1.val.rrcConSetup_r8.pres,
         PRSNT_NODEF);

   /* RadioResourceConfigDedicated */
   resCfgDed = &(conSetUp->criticalExtns.val.c1.val.rrcConSetup_r8.\
         radioResourceConfigDedicated);

   VE_FILL_TKN_UINT(resCfgDed->pres, PRSNT_NODEF);
   /* SRB Add for now only SRB1 */
   veFillNhuSRBAddModLst(resCfgDed, (Ptr)nhuDatRspSdu, VE_PDCP_ID_SRB1);

   /* RRC dedicated MAC Cfg */
   /* MAC-MainConfig  */
   VE_FILL_TKN_UINT(resCfgDed->mac_MainConfig.choice, \
         MAC_MAINCONFIG_DEFAULTVALUE);

   /* Phy. Dedicated Cfg */
   veFillRadResPhyCfgDed(resCfgDed,(Ptr)nhuDatRspSdu, VE_PDCP_ID_SRB1, ueCb);

   /* RRCConnectionSetup-r8-IEs - nonCriticalExtension */
   VE_FILL_TKN_UINT(conSetUp->criticalExtns.val.c1.val.rrcConSetup_r8.\
         nonCriticalExtn.pres, NOTPRSNT); /* Madhur : Fix done for sysdsoft */

   RETVALUE(ROK);
} /* End of veFillRRC_ConSetup */
/*
*
*       Fun:   veFillRRC_ConRej
*
*       Desc:  This function fills RRC Connection reject Msg
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRC_ConRej
(
NhuDatRspSdus *nhuDatRspSdu,
U32 waitTime
)
#else
PUBLIC S16 veFillRRC_ConRej(nhuDatRspSdu, waitTime)
NhuDatRspSdus *nhuDatRspSdu;
U32 waitTime;
#endif
{
   NhuDL_CCCH_Msg *dlMsg = NULLP;
   NhuRRCConRej *conRej = NULLP;

   TRC2(veFillRRC_ConRej);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRC_ConRej: Building \
            Connection Reject Msg.\n"));
#endif

   if(NULLP == nhuDatRspSdu)
  {
     VE_DBG_ERROR((VE_PRNT_BUF,"veFillRRC_ConRej : nhuDatRspSdu is NULL"));

     RETVALUE(RFAILED);
  }

   nhuDatRspSdu->sdu.msgCategory = NHU_MSG_DLCCCH;

   dlMsg = &(nhuDatRspSdu->sdu.m.dlCcchMsg.ccchMsg);

   VE_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(dlMsg->message.choice, DL_CCCH_MSGTYP_C3);
   VE_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C3_RRCCONREJ);

   /* RRCConnectionReject */
   conRej = &(dlMsg->message.val.c1.val.rrcConRej);
   VE_FILL_TKN_UINT(conRej->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(conRej->criticalExtns.choice, CRITICALEXTNS_C13);
/* RRCConnectionReject-r8-IEs  */
   VE_FILL_TKN_UINT(conRej->criticalExtns.val.c1.choice, C14_RRCCONREJ_R8);
   VE_FILL_TKN_UINT(conRej->criticalExtns.val.c1.val.rrcConRej_r8.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(conRej->criticalExtns.val.c1.val.rrcConRej_r8.waitTime,
         waitTime);
   VE_FILL_TKN_UINT(conRej->criticalExtns.val.c1.val.rrcConRej_r8.\
         nonCriticalExtn.pres, NOTPRSNT); /* Madhur : Fix done for sysdsoft */

   RETVALUE(ROK);
}

/* RRC-ReEstab */

/*
*
*       Fun:   veFillRRC_DlInfTr
*
*       Desc:  This function fills Downlink Information Transfer
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRC_DlInfTr
(
NhuDatReqSdus *nhuDatReqSdu,
U16 pduLen,
U8 *nasPdu,
U8 pdcpId
)
#else
PUBLIC S16 veFillRRC_DlInfTr(nhuDatReqSdu, pduLen, nasPdu, pdcpId)
NhuDatReqSdus *nhuDatReqSdu;
U16 pduLen;
U8 *nasPdu;
U8 pdcpId;
#endif
{
   NhuDL_DCCH_Msg     *dlMsg = NULLP;
   NhuDLInfoTfr       *infoTfrMsg = NULLP;
   NhuDLInfoTfr_r8_IEs *msgIe = NULLP;
   U8 rrc_transId = VE_VAL_ONE;
   U16 idx = 0;

   TRC2(veFillRRC_DlInfTr);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRC_DlInfTr: Building\
            DL Info Transfer\n"));
#endif

   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = pdcpId;
   dlMsg = &(nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   VE_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(dlMsg->message.choice, DL_DCCH_MSGTYP_C4);
   VE_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C4_DLINFOTFR);

   /* DLInformationTransfer */
   infoTfrMsg = &(dlMsg->message.val.c1.val.dlInfoTfr);
   VE_FILL_TKN_UINT(infoTfrMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(infoTfrMsg->rrc_TrnsxnId, rrc_transId);
   VE_FILL_TKN_UINT(infoTfrMsg->criticalExtns.choice, CRITICALEXTNS_C8);
   VE_FILL_TKN_UINT(infoTfrMsg->criticalExtns.val.c1.choice, C8_DLINFOTFR_R8);

   /* DLInformationTransfer-r8-IEs */ 
   msgIe = &(infoTfrMsg->criticalExtns.val.c1.val.dlInfoTfr_r8);
   VE_FILL_TKN_UINT(msgIe->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(msgIe->dedicatedInfoTyp.choice, DEDICATEDINFOTYP_DEDICATEDINFONAS);

   /* NAS PDU */
   VE_GET_MEM(nhuDatReqSdu, pduLen, &(msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.val));
   msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.pres = PRSNT_NODEF;
   msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.len = pduLen;
   /* copy each byte */
   for(idx = 0; idx < pduLen; idx++)
   {
      msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.val[idx] = nasPdu[idx];
   }

   /* criticalExtensionsFuture */
   VE_FILL_TKN_UINT(msgIe->nonCriticalExtn.pres, NOTPRSNT);

   RETVALUE(ROK);
} /* End of veFillRRC_DlInfTr */


/*
*
*       Fun:   veFillRRC_SecModeCmd
*
*       Desc:  This function fills Downlink Security Mode Command
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRC_SecModeCmd
(
NhuDatReqSdus *nhuDatReqSdu,
VeUeCb *ueCb
)
#else
PUBLIC S16 veFillRRC_SecModeCmd(nhuDatReqSdu, ueCb)
NhuDatReqSdus *nhuDatReqSdu;
VeUeCb *ueCb;
#endif
{
   NhuDL_DCCH_Msg     *dlMsg = NULLP;
   NhuSecModeCmd      *secModeCmd = NULLP;
   NhuSecConfigSMC    *securityCfg = NULLP;

   TRC2(veFillRRC_SecModeCmd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRC_SecModeCmd: Building \
            Security Mode Command.\n"));
#endif

   if(NULLP == nhuDatReqSdu)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRRC_SecModeCmd : nhuDatReqSdu is NULL."));

      RETVALUE(RFAILED);
   }

   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = VE_PDCP_ID_SRB1;
   nhuDatReqSdu->sdu.ueMsg.isMsgDed = TRUE;

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

/* Verify*/
#ifdef MSPD
   /* Fix is given by pradeep */
   VE_FILL_TKN_UINT(securityCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.pres, PRSNT_NODEF);
   ueCb->secCfg.ciphAlgo = 0; /* eea0 i.e NULL Algorithm */
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.cipheringAlgorithm,\
         ueCb->secCfg.ciphAlgo);
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.integrityProtAlgorithm,\
         NhuSecAlgorithmConfigintegrityProtAlgorithmspare1Enum); /* Spare Value */
#else /* Verify*/
#ifndef VE_PICO
   VE_FILL_TKN_UINT(securityCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.cipheringAlgorithm,\
         ueCb->secCfg.ciphAlgo);
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.integrityProtAlgorithm,\
         ueCb->secCfg.intgAlgo);
#else
   VE_FILL_TKN_UINT(securityCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.cipheringAlgorithm,\
         NhuSecAlgorithmConfigcipheringAlgorithmspare1Enum);
   VE_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.integrityProtAlgorithm,\
         NhuSecAlgorithmConfigintegrityProtAlgorithmspare1Enum);
#endif   
#endif
   RETVALUE(ROK);
} /* End of veFillRRC_SecModeCmd */

/* Build Ue Capability Enq and RA Cap Info */
#ifdef UE_RAD_CAP
/*
*
*       Fun:   veFillRRC_UeCapEnq
*
*       Desc:  This function fills Ue Capability Enquiry info
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRC_UeCapEnq
(
NhuDatReqSdus *nhuDatReqSdu,
VeUeCb *ueCb
)
#else
PUBLIC S16 veFillRRC_UeCapEnq(nhuDatReqSdu, ueCb)
NhuDatReqSdus *nhuDatReqSdu;
VeUeCb *ueCb;
#endif
{
   NhuDL_DCCH_Msg     *dlMsg = NULLP;
   NhuUECapEnquiry    *ueCapEnq = NULLP;
   NhuUE_CapRqst      *ueCapReq = NULLP;
   TRC2(veFillRRC_UeCapEnq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRC_UeCapEnq: Building \
            Ue Capability Enquiry.\n"));
#endif

   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = VE_PDCP_ID_SRB1;
   nhuDatReqSdu->sdu.ueMsg.isMsgDed = TRUE;

   dlMsg = &(nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   VE_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(dlMsg->message.choice, DL_DCCH_MSGTYP_C4);
   VE_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C4_UECAPENQUIRY);


   /* UeCapabilityEnquiry */
   ueCapEnq = &(dlMsg->message.val.c1.val.ueCapEnquiry);

   VE_FILL_TKN_UINT(ueCapEnq->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(ueCapEnq->rrc_TrnsxnId, VE_VAL_ONE);
   VE_FILL_TKN_UINT(ueCapEnq->criticalExtns.choice, CRITICALEXTNS_C19);
   VE_FILL_TKN_UINT(ueCapEnq->criticalExtns.val.c1.choice, C19_UECAPENQUIRY_R8);
   VE_FILL_TKN_UINT(ueCapEnq->criticalExtns.val.c1.val.ueCapEnquiry_r8.\
         pres, PRSNT_NODEF);

   /* UeCapInfo */
   ueCapReq = &(ueCapEnq->criticalExtns.val.c1.val.ueCapEnquiry_r8.\
        ue_CapRqst);

   VE_FILL_TKN_UINT(ueCapReq->noComp, VE_VAL_ONE);
   /* Allocate memory required for num components */
   VE_GET_MEM(nhuDatReqSdu, sizeof(NhuRAT_Typ)*ueCapReq->noComp.val,
                    &(ueCapReq->member));
   VE_FILL_TKN_UINT(ueCapReq->member[0],NhuRAT_TypeutraEnum);

   RETVALUE(ROK);
} /* End of veFillRRC_UeCapEnq */

/*
*
*       Fun:   veFillRRC_UeRadAccessCapInfo
*
*       Desc:  This function fills Ue RadioAccess Cap info
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRC_UeRadAccessCapInfo
(
TknStrOSXL    *str,
NhuEncReqSdus *encReq,
VeUeCb *ueCb
)
#else
PUBLIC S16 veFillRRC_UeRadAccessCapInfo(str, encReq, ueCb)
TknStrOSXL    *str;
NhuEncReqSdus *encReq;
VeUeCb *ueCb;
#endif
{
   NhuUERadioAccessCapInfo_r8_IEsue_RadioAccessCapInfo *raCap;
   NhuUERadioAccessCapInfo *ueRACapInfo;

   TRC2(veFillRRC_UeRadAccessCapInfo);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRC_UeRadAccessCapInfo: Building \
            Ue RA Capability Info.\n"));
#endif
   
   ueRACapInfo = &encReq->sdu.u.ueRadioCapInfo;
   VE_FILL_TKN_UINT(ueRACapInfo->pres,PRSNT_NODEF);
   VE_FILL_TKN_UINT(ueRACapInfo->criticalExtns.choice, CRITICALEXTNS_C24);
   VE_FILL_TKN_UINT(ueRACapInfo->criticalExtns.val.c1.choice,C24_UERADIOACCESSCAPINFO_R8);
   VE_FILL_TKN_UINT(ueRACapInfo->criticalExtns.val.c1.val.ueRadioAccessCapInfo_r8.pres,PRSNT_NODEF);

   raCap = &ueRACapInfo->criticalExtns.val.c1.val.ueRadioAccessCapInfo_r8.\
                ue_RadioAccessCapInfo;
   VE_FILL_TKN_STR_UINT(raCap,str->val,str->len); 

   RETVALUE(ROK);
} /* End of veFillRRC_UeRadAccessCapInfo */
#endif

/*
 *
 *       Fun:   veFillRRC_ConRls
 *
 *       Desc:  This function fills RRC Connection Release Message
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *
 *       File:  ve_bldmsg.c
 *
 */
#ifdef ANSI
PUBLIC S16 veFillRRC_ConRls
(
NhuDatReqSdus *nhuDatReqSdu
)
#else
PUBLIC S16 veFillRRC_ConRls(nhuDatReqSdu)
NhuDatReqSdus *nhuDatReqSdu;
#endif
{
   NhuDL_DCCH_Msg     *dlMsg = NULLP;
   NhuRRCConRls       *conRls = NULLP;
   NhuIdleModeMobilityCntrlInfo *idleModeMobility;

   TRC2(veFillRRC_ConRls);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRC_ConRls: Building \
            RRC Connection Release.\n"));
#endif
   
   if(NULLP == nhuDatReqSdu)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRRC_ConRls : nhuDatReqSdu is NULL."));

      RETVALUE(RFAILED);
   }
 
   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = VE_PDCP_ID_SRB1;
   dlMsg = &(nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   nhuDatReqSdu->sdu.ueMsg.isMsgDed = TRUE;

   VE_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(dlMsg->message.choice, DL_DCCH_MSGTYP_C4);
   VE_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C4_RRCCONRLS);

   /* RRCConnectionRelease */
   conRls = &(dlMsg->message.val.c1.val.rrcConRls);
   VE_FILL_TKN_UINT(conRls->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(conRls->rrc_TrnsxnId, VE_VAL_ONE);
   VE_FILL_TKN_UINT(conRls->criticalExtns.choice, CRITICALEXTNS_C15);
   VE_FILL_TKN_UINT(conRls->criticalExtns.val.c1.choice, C15_RRCCONRLS_R8);
   /* RRCConnectionRelease-r8-IEs */
   VE_FILL_TKN_UINT(conRls->criticalExtns.val.c1.val.rrcConRls_r8.pres,\
         PRSNT_NODEF);
   VE_FILL_TKN_UINT(conRls->criticalExtns.val.c1.val.rrcConRls_r8.releaseCause,\
         NhuRlsCauseotherEnum);

   /* IdleModeMobilityCntrlInfo : Madhur */
   idleModeMobility = &(conRls->criticalExtns.val.c1.val.rrcConRls_r8.idleModeMobilityCntrlInfo);
   VE_FILL_TKN_UINT(idleModeMobility->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(idleModeMobility->t320, NhuIdleModeMobilityCntrlInfot320min180Enum);
   

   RETVALUE(ROK);
} /* End of veFillRRC_ConRls */

/*
*
*       Fun:   veKDF 
*
*       Desc:  Key Derv. Function 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ve_bldmsg.c 
*
*/
#ifdef ANSI
PUBLIC S16 veKDF
(
U8  *secKey,
U8  *strS,
U8  *derKey
)
#else
PUBLIC S16 veKDF(secKey, strS, derKey)
U8  *secKey;
U8  *strS;
U8  *derKey;
#endif
{
   U8 extractKey[32];
   U8 idx = 0;
   U8 idxKey = 0;

   TRC2(veKDF);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veKDF: Key Derivation Function\n"));
#endif

   cmMemset(&extractKey[0], '\0', 32);

   /* Call a HMAC-SHA256 Algorithm API
    * currently not implemented */

   /* truncate the extracted key to 16 bytes long */
   for(idx = 16; idx < 32; idx++)
   {
      derKey[idxKey] = extractKey[idx];
      idxKey++;
   }

   RETVALUE(ROK);
} /* veKDF */

/*
*
*       Fun:   veExtractKeys 
*
*       Desc:  Extract key from the Bit string 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 veExtractKeys
(
VeUeSecCfg  *secCfg,
TknStrBSXL  *secKey
)
#else
PUBLIC S16 veExtractKeys(secCfg, secKey)
VeUeSecCfg *secCfg;
TknStrBSXL *secKey;
#endif
{
   U8  stringS[VE_STRING_S_LEN];
   U8  dervdKey[VE_CIPH_KEY_LEN];
   U8  type = 0;
   U8  idx = 0;

   TRC2(veExtractKeys);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veExtractKeys: Extract Intg. and\
            ciphering keys.\n"));
#endif


   /* Memset */
   cmMemset((U8 *)&stringS[0], '\0', VE_STRING_S_LEN);

   /* Building the String S = FC || P0 || L0 || P1 || L1 */
   /* FC */
   stringS[0] = 0x15;

   /* L0 */
   stringS[2] = 0x00;
   stringS[3] = 0x01;

   /* L1 */
   stringS[5] = 0x00;
   stringS[6] = 0x01;

   for(type = 0; type <= VE_INTG_KEY; type++)
   {
      /* Memset to zero */
      cmMemset((U8 *)&dervdKey[0], '\0', VE_CIPH_KEY_LEN);

      /* Fill P0 & P1 */ 
      switch(type)
      {
         case VE_CP_CIPH_KEY:
            /* P0 */
            stringS[1] = 0x03;
            /* P1 *//* klock warning resolved */
            stringS[4] = (U8)secCfg->ciphAlgo;

            /* Use the Key Derivation Function */
            veKDF((U8 *)secKey->val, (U8 *)stringS, (U8 *)dervdKey);

            for (idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
            {
               secCfg->cpCiphKey[idx] = dervdKey[idx];
            }

            break;

         case VE_UP_CIPH_KEY:
            /* P0 */
            stringS[1] = 0x05;
            /* P1 */ /* klock warning resolved */
            stringS[4] = (U8)secCfg->ciphAlgo;

            /* Use the Key Derivation Function */
            veKDF((U8 *)secKey->val, (U8 *)stringS, (U8 *)dervdKey);

            for (idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
            {
               secCfg->upCiphKey[idx] = dervdKey[idx];
            }
            break;

         case VE_INTG_KEY:
            /* P0 */
            stringS[1] = 0x04;
            /* P1 *//* klock warning resolved */
            stringS[4] = (U8)secCfg->intgAlgo;

            /* Use the Key Derivation Function */
            veKDF((U8 *)secKey->val, (U8 *)stringS, (U8 *)dervdKey);

            for (idx = 0; idx < VE_INTG_KEY_LEN; idx++)
            {
               secCfg->intgKey[idx] = dervdKey[idx];
            }
            break;

         default:
            RETVALUE(RFAILED); 
            break;
      } /* switch */
   } /* for loop */
   RETVALUE(ROK); 
} /* veExtractKeys */


/*
*
*       Fun:   veFillAndAddPagingInfo
*
*       Desc:  Fills and Adds Paging message 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillAndAddPagingInfo
(
S1apPdu       *s1apPdu
)
#else
PUBLIC S16 veFillAndAddPagingInfo(s1apPdu)
S1apPdu       *s1apPdu;
#endif
{
   VeUePagingInfo   *uePagingInfo = NULLP;
   SztPag           *sztPag = NULLP;
   SztProtIE_Field_PagIEs *pagMember = NULLP;
   U16 numCompPagMsg = 0;
   U32 protId = 0;
   U16 memberIdx = 0;
   TknBStr32  *ueIdxBitStr = NULLP;
   NhuDatReqSdus *pagMsg = NULLP;
   U32         ueSpcPagDrx = 0;/* g++ compilation */
   U8          idx=0;
   U8          digit[15]={0};
   U16         T, N, Ns, Nb = 0;/* g++ compilation */
   U16         pf, i_s, po = 0;/* g++ compilation */
  /* g++ compilation */
   U32         ueId = 0;
   TknStrOSXL  *imsi = NULLP;
   U32           cellId = 0;
   Bool        pagingDrxPres = FALSE;
   U32         ueSpecificValue = 0;
   U32         defaultValue = 0;
   U8          cnt = 0;

   TRC2(veFillAndAddPagingInfo);

   if(s1apPdu->pdu.val.initiatingMsg.procedureCode.pres == TRUE
         && s1apPdu->pdu.val.initiatingMsg.procedureCode.val != Sztid_Pag)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillAndAddPagingInfo:\
               Invalid Message \n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Get the reference of the Paging Message */
   sztPag = &(s1apPdu->pdu.val.initiatingMsg.value.u.sztPag);

   /* Allocate memory for UE Paging Info */
   VE_ALLOC(&uePagingInfo, sizeof(VeUePagingInfo));
   if(uePagingInfo == NULLP)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillAndAddPagingInfo:\
               Failure in the allocation of Paging Info\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   numCompPagMsg = sztPag->protocolIEs.noComp.val;

   /* Accessing the IEs within the S1AP pdu */
   for(memberIdx = 0; memberIdx < numCompPagMsg; memberIdx++)
   {
      pagMember= &(sztPag->protocolIEs.member[memberIdx]);
      protId = pagMember->id.val;
      switch(protId)
      {
         case Sztid_UEIdentityIdxValue:
            {
               ueIdxBitStr = &(pagMember->value.u.sztUEIdentityIdxValue);
               /*IOT changes are incoporated*/
               /* UEID is only 10 bits so it has been shifted for 6 bits
                  ccpu00117365  as bigendian IOT changes*/
               ueId = 0;
               ueId =((ueIdxBitStr->val[0] << 2) | ((ueIdxBitStr->val[1] & 0xC0) >> 6));
               break;
            }
         case Sztid_UEPagID:
         {
            if(pagMember->value.u.sztUEPagID.choice.val == UEPAGID_IMSI)
            {
               imsi = &pagMember->value.u.sztUEPagID.val.iMSI;
               /* klock warning resolved */
               for(idx = 0; idx < (pagMember->value.u.sztUEPagID.val.iMSI.len - 1); idx++)
               {
                  digit[idx]   = pagMember->value.u.sztUEPagID.val.iMSI.val[idx] & 0xf0;
                  digit[idx+1] = pagMember->value.u.sztUEPagID.val.iMSI.val[idx] & 0x0f;
               } /* end of for loop */

            }
            else if (pagMember->value.u.sztUEPagID.choice.val == UEPAGID_S_TMSI)
            {
               uePagingInfo->sTMSI.pres = TRUE;
               uePagingInfo->sTMSI.mmec = pagMember->value.u.sztUEPagID.val.s_TMSI.mMEC.val[0];

               for(cnt = 0; cnt < 4; cnt++)
               {
                  uePagingInfo->sTMSI.mTMSI <<= 8;
                  uePagingInfo->sTMSI.mTMSI |= (U8)(pagMember->value.u.sztUEPagID.val.s_TMSI.m_TMSI.val[cnt]);
               } /* end of for loop */
      
            }
            else
            {
               VE_DBG_ERROR((VE_PRNT_BUF,"veFillAndAddPagingInfo:\
                   Valid UE Identity should be provided\n"));
               RETVALUE(RFAILED);
            } /* end of else statement */
            break;
         } /* end of Sztid_UEPagID */
         case Sztid_pagingDRX:
         {
            pagingDrxPres = TRUE;
            ueSpcPagDrx = pagMember->value.u.sztPagDRX.val;
            break;
         } /* end of Sztid_pagingDRX */
         case Sztid_CNDomain:
         { /* klock warning resolved */
            uePagingInfo->domIndType = (U8)(pagMember->value.u.sztCNDomain.val);
            break;
         }
         default:
         {
            break;
         }
      } /* end of switch statement */
   } /* end of for loop */
       
   
   /* Fill the Value of T (DRX cycle of the UE)
    * T = min(UE Specific DRX value allocated by upper layers, default DRX
    * broadcast in System Information) i.e. min( ueSpcPagDrx, veDfltDefaultPagCycle)
    */
     if(pagingDrxPres == FALSE)
     {
        VE_GET_PAGING_CYCLE(veDfltDefaultPagCycle, defaultValue);
        uePagingInfo->T = T =  (U16)(defaultValue);/* klock warning resolved */
     }
     else
     {
        VE_GET_PAGING_CYCLE(ueSpcPagDrx, ueSpecificValue);
        VE_GET_PAGING_CYCLE(veDfltDefaultPagCycle, defaultValue);
        /* klock warning resolved */
        T = (U16)((ueSpecificValue < defaultValue)?ueSpecificValue:defaultValue);
        uePagingInfo->T = T;
     }

 
   /* Fill the Value of N 
    *  N = min( T, nB)
    *  i.e. min(T, veDfltPCCHCfgNB) */
     VE_GET_NB_VALUE(veDfltPCCHCfgNB, T, Nb);
     N = ( T < Nb) ? T : Nb;  
     uePagingInfo->N = (U8)N;/* klock warning resolved */
      
   /* Fill the Value of Ns 
    *  Ns = max(1, nB/T)*/
     Ns  = ((Nb / T) > 1 ) ? (Nb / T) : 1;
     uePagingInfo->Ns = (U8)Ns;/* klock warning resolved */

   uePagingInfo->ue_Id = ueId;

   /* Fill the Value of pf */
    /* klock warning resolved */
     pf = (U16)(( T / N ) * (ueId % N));
     uePagingInfo->pf = pf;

   /* Fill the Value of po */
    /* klock warning resolved */
     i_s = (U16)(((ueId / N)) % Ns);

#ifndef LTE_TDD
     /* Following are the values of the paging occasion
        for FDD as per 36.304 880 specification */
     if(((Ns == 1) && (i_s == 0)) || ((Ns == 2) && (i_s == 1))
                 || ((Ns == 4) && (i_s == 3)))
     {
        po = 9;
     }
     else if ( ((Ns == 2) && (i_s == 0 )) || ((Ns == 4) && (i_s == 1)))
     {
        po = 4;
     }
     else if ((Ns == 4) && (i_s == 0))
     {
        po = 0;
     }
     else if((Ns == 4) && (i_s == 2))
     {
        po = 5;
     } /* end of else part */
#else
     /* Following are the values of the paging occasion
        for TDD as per 36.304 880 specification */
     if(( i_s == 0))
     {
        po = 0;
     }
     else if(((i_s == 1) && (Ns == 2)) || ((Ns == 4) && (i_s == 2)))
     {
        po = 5;
     }
     else if ((i_s == 1) && (Ns == 4))
     {
        po = 1;
     }
     else if ((i_s == 3) && (Ns == 4))
     {
        po = 4;
     }

#endif
     uePagingInfo->po = (U8)po;/* klock warning resolved */

   VE_ALLOCEVNT(&pagMsg, sizeof(NhuDatReqSdus));

   /* Cell ID *//* klock warning resolved */
   cellId = veCb.cellCb[0]->cellId;

   /* Fill HDR Info */
   veFillNhuEvntHdr(&(pagMsg->hdr), cellId, VE_VAL_ZERO, C2_PAGING);
   pagMsg->sdu.ueMsg.isMsgDed = FALSE;
   pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgCategory = NHU_MSG_DLPCCH;
   pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId = VE_PCCH_ID;
   /* Fill the NhuDatReqSdus paging message */
   veFillRRCPagingMsg(pagMsg, uePagingInfo);   

   uePagingInfo->pagMsg = pagMsg;

   /* Add the Paging info into the linked list of
      cellCb */
   cmLListAdd2Tail(&(veCb.cellCb[0]->pendPagUesLst), &(uePagingInfo->lnk));
   uePagingInfo->lnk.node = (PTR)uePagingInfo; 
   RETVALUE(ROK);

} /* end of veFillAndAddPagingInfo */

/*
*
*       Fun:   veFillRRCPagingMsg
*
*       Desc:  Fills RRC  Paging message. This function
*              needs to be enhanced when multiple records
*              need to be built in the same RRC Paging 
*              message
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRCPagingMsg
(
NhuDatReqSdus *datReqSdus,
VeUePagingInfo   *uePagingInfo
)
#else
PUBLIC S16 veFillRRCPagingMsg(datReqSdus, uePagingInfo)
NhuDatReqSdus *datReqSdus;
VeUePagingInfo   *uePagingInfo;
#endif
{
   NhuPCCH_Msg *pcchMsg;
   NhuPag *page;
   Mem   sMem;
   U8    numOfUe;
   U8    ueIdType;
   U8    noOfImsi;
   U16   idx, idx1;
   VeCellCb *cellCb = NULLP;
   cellCb = veCb.cellCb[0];

   /* Added trace macro */
   TRC2(veFillRRCPagingMsg)

   sMem.region = 0;
   sMem.pool = 0;


   pcchMsg = &(datReqSdus->sdu.ueMsg.msg.cmnMsg.cmn.pcchMsg);
   numOfUe = 1;
   if(uePagingInfo->sTMSI.pres == TRUE)
   {
      ueIdType = UEPAGID_S_TMSI;
   }
   else
   {
      ueIdType = UEPAGID_IMSI;
   }
   /* The number of digits in IMSI shall be twice the number of 
    * bytes in the octet string */
   noOfImsi = 6;
   
   VE_FILL_TKN_UINT(pcchMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(pcchMsg->message.choice,0);

   if(!(pcchMsg->message.choice.val))
   {
      VE_FILL_TKN_UINT(pcchMsg->message.val.c1.choice,0);
      page = &(pcchMsg->message.val.c1.val.paging);

      VE_FILL_TKN_UINT(page->pres, PRSNT_NODEF);
#ifndef VE_PICO      
      VE_FILL_TKN_UINT(page->systemInfoModfn, NhuPagsystemInfoModfntrueEnum);
      VE_FILL_TKN_UINT(page->etws_Ind, 0);
#endif      
      VE_FILL_TKN_UINT(page->pagingRecordLst.noComp, numOfUe);

      datReqSdus->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.tbsSize = 7 * numOfUe;
      VE_GET_MEM(datReqSdus, (page->pagingRecordLst.noComp.val)*
           (sizeof(NhuPagRecord)),&(page->pagingRecordLst.member));
      for(  idx = 0;  idx < (page->pagingRecordLst.noComp.val); idx++)
      {
         VE_FILL_TKN_UINT(page->pagingRecordLst.member[idx].pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(page->pagingRecordLst.member[idx].cn_Domain,
                              uePagingInfo->domIndType);
         VE_FILL_TKN_UINT(page->pagingRecordLst.member[idx].ue_Identity.choice,
                              ueIdType);
         switch(ueIdType)
         {
            case 0: /* s-TMSI based */
            {
               VE_FILL_TKN_UINT(page->pagingRecordLst.member[idx].ue_Identity.val.s_TMSI.pres, PRSNT_NODEF);
               VE_FILL_BIT_STR(page->pagingRecordLst.member[idx].ue_Identity.val.s_TMSI.mmec, 8,(uePagingInfo->sTMSI.mmec), datReqSdus);
               VE_FILL_BIT_STR_VAL(page->pagingRecordLst.member[idx].ue_Identity.val.s_TMSI.m_TMSI, 32, (uePagingInfo->sTMSI.mTMSI), datReqSdus);
               break;
             } /* end of case 0 */
             case 1: /* IMSI based */
             {
                VE_FILL_TKN_UINT(page->pagingRecordLst.member[idx].ue_Identity.val.imsi.noComp, noOfImsi);
                VE_GET_MEM(datReqSdus, (page->pagingRecordLst.member[idx].ue_Identity.val.imsi.noComp.val) * (sizeof(NhuIMSI_Digit)), &(page->pagingRecordLst.member[idx].ue_Identity.val.imsi.member));
                /* Fill the Digits of the IMSI */
                for(  idx1 = 0;  idx1 < noOfImsi; idx1++)
                {
                   /*IOT changes are incoporated*/
                   if(idx1 < 3)
                   {
                      VE_FILL_TKN_UINT(page->pagingRecordLst.member[idx].ue_Identity.val.imsi.member[idx1],
                            (cellCb->sysRelInfo.mcc[idx1].val));
                   }
                   else
                   {
                      VE_FILL_TKN_UINT(page->pagingRecordLst.member[idx].ue_Identity.val.imsi.member[idx1],
                            (cellCb->sysRelInfo.mnc[idx1-3].val));
                   }

                } /* end of for loop */
                break;
             } /* end of case 1 */
          } /* end of switch statement */
        } /* end of for loop */
      } /* end of if statement */

   RETVALUE(ROK);

} /* veFillRRCPagingMsg */

/*
*
*       Fun:   veFetchUeCntxModSecInfo
*
*       Desc:  Featch the Security information from the 
*              UE context modification.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFetchUeCntxModSecInfo
(
VeUeCb        *ueCb,
S1apPdu       *s1apPdu
)
#else
PUBLIC S16 veFetchUeCntxModSecInfo(ueCb, s1apPdu)
VeUeCb        *ueCb;
S1apPdu       *s1apPdu;
#endif
{
   U16 noComp = 0;
   U32 protId = 0;
   U16 idx = 0;
#ifndef S1AP_REL8A0
   TknBStr32  *bitStr = NULLP;
#endif
   SztProtIE_Field_UECntxtModificationRqstIEs    *ueCntxMod = NULLP;
   SztUECntxtModificationRqst                    *cntxMod = NULLP;

   TRC2(veFetchSecInfo);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf,
               "veFetchUeCntxModSecInfo: Fetching Security info\n"));
#endif

   /* retrieve info from S1AP Pdu */
   cntxMod = &(s1apPdu->pdu.val.initiatingMsg.value.u.sztUECntxtModificationRqst);

   noComp = cntxMod->protocolIEs.noComp.val;

   /* DedicatedInfoNAS List */
   for(idx = 0; idx < noComp; idx++)
   {
      ueCntxMod = &(cntxMod->protocolIEs.member[idx]);
      protId = ueCntxMod->id.val;
      switch(protId)
      {
#ifndef S1AP_REL8A0
         case Sztid_UESecurCapabilities:
            {
               /* fetch ciphering algo */
               bitStr = 
                  &(ueCntxMod->value.u.sztUESecurCapabilities.\
                         encryptionAlgorithms);

               if (bitStr->val[0] & 0x80)
               {
                  /* means EEA0 */
                  ueCb->secCfg.ciphAlgo = 0; 
               }
               else if (bitStr->val[0] & 0x40)
               {
                  /* means EEA1 */
                  ueCb->secCfg.ciphAlgo = 1; 
               }
               else if (bitStr->val[0] & 0x20)
               {
                  /* means EEA2 */
                  ueCb->secCfg.ciphAlgo = 2; 
               }

               /* fetch Integrity protection algo */
               bitStr = 
                  &(ueCntxMod->value.u.sztUESecurCapabilities.\
                         integrityProtectionAlgorithms);

               if (bitStr->val[0] & 0x80)
               {
                  /* means EIA1 */
                  ueCb->secCfg.intgAlgo = 1; 
               }
               else if (bitStr->val[0] & 0x40)
               {
                  /* means EIA2 */
                  ueCb->secCfg.intgAlgo = 2; 
               } 
            }
            break;
#endif
         case Sztid_SecurKey:
            {
               /* fetch ciphering key */
               veExtractKeys(&ueCb->secCfg, &ueCntxMod->value.u.sztSecurKey); 
            }
            break;
         default:
            break;
      }
   }
   RETVALUE(ROK);
} /* End of veFetchUeCntxModSecInfo */

/*
*
*       Fun:   veFetchSecInfo
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFetchSecInfo
(
VeUeCb        *ueCb,
S1apPdu       *s1apPdu
)
#else
PUBLIC S16 veFetchSecInfo(ueCb, s1apPdu)
VeUeCb        *ueCb;
S1apPdu       *s1apPdu;
#endif
{
   SztInitCntxtSetupRqst *initCntxtSetup = NULLP;
   SztProtIE_Field_InitCntxtSetupRqstIEs *member = NULLP;
   U16 numCompIntCntxtMsg = 0;
   U32 protId = 0;
   U16 memberIdx = 0;
   TknBStr32  *bitStr = NULLP;

   TRC2(veFetchSecInfo);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFetchSecInfo: Fetching Security \
            related info. from S1AP Pdu.\n"));
#endif


   /* retrieve info from S1AP Pdu */
   initCntxtSetup = &(s1apPdu->pdu.val.initiatingMsg.value.u.sztInitCntxtSetupRqst);

   numCompIntCntxtMsg = initCntxtSetup->protocolIEs.noComp.val;

   /* DedicatedInfoNAS List */
   for(memberIdx = 0; memberIdx < numCompIntCntxtMsg; memberIdx++)
   {
      member = &(initCntxtSetup->protocolIEs.member[memberIdx]);
      protId = member->id.val;
      switch(protId)
      {
         case Sztid_UESecurCapabilities:
            {
               /* fetch ciphering algo */
               bitStr = 
                  &(member->value.u.sztUESecurCapabilities.encryptionAlgorithms);

               if (!(bitStr->val[0] & 0xFF))        /*IDCC CR */
               {
                  /* means EEA0 */
                  ueCb->secCfg.ciphAlgo = 0;            /*IDCC CR */ 
               }
               else if (bitStr->val[0] & 0x80)
               {
                  /* means EEA1 */
                  ueCb->secCfg.ciphAlgo = 1; 
               }
               else if (bitStr->val[0] & 0x40)           /*IDCC CR */
               {
                  /* means EEA2 */
                  ueCb->secCfg.ciphAlgo = 2; 
               }

               /* fetch Integrity protection algo */
               bitStr = 
                  &(member->value.u.sztUESecurCapabilities.integrityProtectionAlgorithms);

               if (bitStr->val[0] & 0x80)
               {
                  /* means EIA1 */
                  ueCb->secCfg.intgAlgo = 1; 
               }
               else if (bitStr->val[0] & 0x40)
               {
                  /* means EIA2 */
                  ueCb->secCfg.intgAlgo = 2; 
               } 
            }
            break;

         case Sztid_SecurKey:
            {
               /* fetch ciphering key */
               veExtractKeys(&ueCb->secCfg, &member->value.u.sztSecurKey); 
            }
            break;
         default:
            break;
      }
   }
   RETVALUE(ROK);
} /* End of veFetchSecInfo */

/*
*
*       Fun:   veGetBitRateUeCntxtMod
*
*       Desc:  Fills connection reconfiguration message from UE context
*              modification request message
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veGetBitRateUeCntxtMod
(
VeUeCb                       *ueCb,
SztUECntxtModificationRqst   *s1apMsg,
U8                            *upd
)
#else
PUBLIC S16 veGetBitRateUeCntxtMod(ueCb, s1apMsg,upd)
VeUeCb                       *ueCb;
SztUECntxtModificationRqst   *s1apMsg;
U8                           *upd;
#endif
{
   SztProtIE_Field_UECntxtModificationRqstIEs *member = NULLP;
   U16 noComp = 0;
   U32 protId = 0;
   U16 idx = 0;
   U32 dlBitRate = 0;
   U32 ulBitRate = 0;

   TRC2(veFillRrcRecfgInitCntxtSetup);

   *upd = FALSE;

   noComp = s1apMsg->protocolIEs.noComp.val;

   for(idx = 0; idx < noComp; idx++)
   {
      member = &(s1apMsg->protocolIEs.member[idx]);
      protId = member->id.val;
      switch(protId)
      {
         case Sztid_uEaggregateMaxBitrate:
            {
               /* Update UeCb with UE Aggregate Maximum Bit Rate */
               VE_GET_U32_FRM_OSXL(dlBitRate, \
                     member->value.u.sztUEAggMaxBitrate.uEaggregateMaxBitRateDL);
               ueCb->rbInfo.aggBr.dl = dlBitRate;

               VE_GET_U32_FRM_OSXL(ulBitRate, \
                     member->value.u.sztUEAggMaxBitrate.uEaggregateMaxBitRateUL);
               ueCb->rbInfo.aggBr.ul = ulBitRate;
               *upd = TRUE;
            }
            break;
         default:
            break;
      }
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   veFillRrcRecfgInitCntxtSetup
*
*       Desc:  Fills connection reconfiguration message
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRrcRecfgInitCntxtSetup
(
VeUeCb                  *ueCb,
NhuDatReqSdus           *nhDatReqEvntSdu,
SztInitCntxtSetupRqst   *s1apMsg
)
#else
PUBLIC S16 veFillRrcRecfgInitCntxtSetup(ueCb, nhDatReqEvntSdu, s1apMsg)
VeUeCb        *ueCb;
NhuDatReqSdus *nhDatReqEvntSdu;
SztInitCntxtSetupRqst   *s1apMsg;
#endif
{
   NhuDL_DCCH_Msg *dcchMsg = NULLP;
   NhuRRCConRecfgn *rrcConReCfg = NULLP;
   NhuRRCConRecfgn_r8_IEs *conRecfgIEs = NULLP;
/* MIMO changes */
#ifdef LTEMAC_MIMO
   NhuAntennaInfoCmn    *antennaInfoCmn = NULLP;
#endif

   SztProtIE_Field_InitCntxtSetupRqstIEs *member = NULLP;

   U16 numCompIntCntxtMsg = 0;
   U32 protId = 0;
   U16 memberIdx = 0;
   U32 dlBitRate = 0;
   U32 ulBitRate = 0;

   TRC2(veFillRrcRecfgInitCntxtSetup);

   dcchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   conRecfgIEs = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   /* TransId for InitCntxt Setup */
   VE_FILL_TKN_UINT(rrcConReCfg->rrc_TrnsxnId, VE_INIT_CNTXT_SETUP_REQ);

   numCompIntCntxtMsg = s1apMsg->protocolIEs.noComp.val;

   /* DedicatedInfoNAS List */
   for(memberIdx = 0; memberIdx < numCompIntCntxtMsg; memberIdx++)
   {
      member = &(s1apMsg->protocolIEs.member[memberIdx]);
      protId = member->id.val;
      switch(protId)
      {
         case Sztid_E_RABToBeSetupLstCtxtSUReq:
            /* fills 
             * 1)DedicatedInfoNAS List
             * 2)DRB-ToAddModList
             * based on the values in the s1ap context setup
             * message.
             */
            if(ROK != veFillRRCReCfgFromInitCntxtERABSetup(nhDatReqEvntSdu, ueCb,\
                  conRecfgIEs, member))
            {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veFillRrcRecfgInitCntxtSetup:\
                    Failed to Fill RRCReCfgFromInitCntxtERABSetup.\n"));
#endif
               RETVALUE(RFAILED);
            }
            break;
         case Sztid_uEaggregateMaxBitrate:
            {
               /* Update UeCb with UE Aggregate Maximum Bit Rate */
               VE_GET_U32_FRM_OSXL(dlBitRate, \
                     member->value.u.sztUEAggMaxBitrate.uEaggregateMaxBitRateDL);
               ueCb->rbInfo.aggBr.dl = dlBitRate;

               VE_GET_U32_FRM_OSXL(ulBitRate, \
                     member->value.u.sztUEAggMaxBitrate.uEaggregateMaxBitRateUL);
               ueCb->rbInfo.aggBr.ul = ulBitRate;
            }
            break;
         default:
            break;
      }
   }

/* MIMO changes */
#ifdef LTEMAC_MIMO
   /* Antenna common Config */
   antennaInfoCmn = &(conRecfgIEs->mobilityCntrlInfo.radioResourceConfigCmn.antennaInfoCmn); 
   VE_FILL_TKN_UINT(antennaInfoCmn->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(antennaInfoCmn->antennaPortsCount, veDfltAntCmnInfoPorts);
#endif
   /* MAC-MainConfig */
   if(ROK != veFillNhuMACMainCfg(&conRecfgIEs->radioResourceConfigDedicated, \
         (Ptr)nhDatReqEvntSdu, VE_PDCP_ID_DRB1))
   {
#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veFillRrcRecfgInitCntxtSetup:\
                    Failed to Fill RRCReCfgFromInitCntxtERABSetup.\n"));
#endif
               RETVALUE(RFAILED);
   }

   /* PhysicalConfigDedicated */
   if(ROK != veFillRadResPhyCfgDed(&conRecfgIEs->radioResourceConfigDedicated, \
         (Ptr)nhDatReqEvntSdu, VE_PDCP_ID_DRB1, ueCb))
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRrcRecfgInitCntxtSetup:\
                    Failed to Fill RadResPhyCfgDed .\n"));
#endif
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/*
*
*       Fun:   veFillRrcRecfgERABSetReq
*
*       Desc:  Fills connection reconfiguration message
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRrcRecfgERABSetReq
(
VeUeCb                  *ueCb,
NhuDatReqSdus           *nhDatReqEvntSdu,
SztE_RABSetupRqst   *s1apMsg
)
#else
PUBLIC S16 veFillRrcRecfgERABSetReq(ueCb, nhDatReqEvntSdu, s1apMsg)
VeUeCb        *ueCb;
NhuDatReqSdus *nhDatReqEvntSdu;
SztE_RABSetupRqst   *s1apMsg;
#endif
{
   NhuDL_DCCH_Msg *dcchMsg = NULLP;
   NhuRRCConRecfgn *rrcConReCfg = NULLP;
   NhuRRCConRecfgn_r8_IEs *conRecfgIEs = NULLP;

   SztProtIE_Field_E_RABSetupRqstIEs *member = NULLP;

   U16 numOfComps = 0;
   U32 protId = 0;
   U16 memberIdx = 0;
   U32 dlBitRate = 0;
   U32 ulBitRate = 0;


   TRC2(veFillRrcRecfgERABSetReq);

   dcchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   conRecfgIEs = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);

   /* TransId for ERAB Setup Req */
   VE_FILL_TKN_UINT(rrcConReCfg->rrc_TrnsxnId, VE_ERAB_SETUP_REQ);

   numOfComps = s1apMsg->protocolIEs.noComp.val;

   /* DedicatedInfoNAS List */
   for(memberIdx = 0; memberIdx < numOfComps; memberIdx++)
   {
      member = &(s1apMsg->protocolIEs.member[memberIdx]);
      protId = member->id.val;
      switch(protId)
      {
         case Sztid_E_RABToBeSetupLstBrSUReq:
            /* fills 
             * 1)DedicatedInfoNAS List
             * 2)DRB-ToAddModList
             * based on the values in the s1ap context setup
             * message.
             */
            veFillRRCReCfgFromERABSetupReq(nhDatReqEvntSdu, ueCb,\
                  conRecfgIEs, member);
            break;
         case Sztid_uEaggregateMaxBitrate:
            {
               /* Update UeCb with UE Aggregate Maximum Bit Rate */
               VE_GET_U32_FRM_OSXL(dlBitRate, \
                     member->value.u.sztUEAggMaxBitrate.uEaggregateMaxBitRateDL);
               ueCb->rbInfo.aggBr.dl = dlBitRate;

               VE_GET_U32_FRM_OSXL(ulBitRate, \
                     member->value.u.sztUEAggMaxBitrate.uEaggregateMaxBitRateUL);
               ueCb->rbInfo.aggBr.ul = ulBitRate;
            }
            break;
         default:
            break;
      }
   }

   /* MAC-MainConfig */
   if(ROK != veFillNhuMACMainCfg(&conRecfgIEs->radioResourceConfigDedicated, \
         (Ptr)nhDatReqEvntSdu, VE_PDCP_ID_DRB2))
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"Failed to Fill veFillNhuMACMainCfg \n"));
#endif
      RETVALUE(RFAILED);
   }

   /* PhysicalConfigDedicated */
   if(ROK != veFillRadResPhyCfgDed(&conRecfgIEs->radioResourceConfigDedicated, \
         (Ptr)nhDatReqEvntSdu, VE_PDCP_ID_DRB2, ueCb))
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"Failed to Fill veFillRadResPhyCfgDed \n"));
#endif
      RETVALUE(RFAILED);    
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   veFillRrcRecfgERABModReq
*
*       Desc:  Fills connection reconfiguration message for E-RAB Modify 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRrcRecfgERABModReq
(
VeUeCb             *ueCb,
NhuDatReqSdus      *nhDatReqEvntSdu,
SztE_RABMdfyRqst   *s1apMsg
)
#else
PUBLIC S16 veFillRrcRecfgERABModReq(ueCb, nhDatReqEvntSdu, s1apMsg)
VeUeCb             *ueCb;
NhuDatReqSdus      *nhDatReqEvntSdu;
SztE_RABMdfyRqst   *s1apMsg;
#endif
{
   /* E-RAB Modify */
   NhuDL_DCCH_Msg                   *dcchMsg = NULLP;
   NhuRRCConRecfgn                  *rrcConReCfg = NULLP;
   NhuRRCConRecfgn_r8_IEs           *conRecfgIEs = NULLP;
   NhuRadioResourceConfigDedicated  *rrcCfgDed = NULLP;
   SztProtIE_Field_E_RABMdfyRqstIEs *member = NULLP;

   U16 numOfComps = 0;
   U32 protId = 0;
   U16 memberIdx = 0;
   U32 dlBitRate = 0;
   U32 ulBitRate = 0;

   TRC2(veFillRrcRecfgERABModReq);

   dcchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   conRecfgIEs = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   rrcCfgDed   = &(conRecfgIEs->radioResourceConfigDedicated);

   VE_FILL_TKN_UINT(rrcCfgDed->pres, PRSNT_NODEF);
   rrcCfgDed->srb_ToAddModLst.noComp.pres = NOTPRSNT;
   rrcCfgDed->drb_ToAddModLst.noComp.pres = NOTPRSNT;
   rrcCfgDed->drb_ToRlsLst.noComp.pres = NOTPRSNT;
   rrcCfgDed->mac_MainConfig.choice.pres = NOTPRSNT;
   rrcCfgDed->mac_MainConfig.val.explicitValue.pres.pres = NOTPRSNT;
   rrcCfgDed->sps_Config.pres.pres = NOTPRSNT;
   rrcCfgDed->physicalConfigDedicated.pres.pres = NOTPRSNT;

   numOfComps = s1apMsg->protocolIEs.noComp.val;
   for(memberIdx = 0; memberIdx < numOfComps; memberIdx++)
   {
      member = &(s1apMsg->protocolIEs.member[memberIdx]);
      protId = (member->id.val);
      switch(protId)
      {
         case Sztid_uEaggregateMaxBitrate:
            {
               /* Update UeCb with UE Aggregate Maximum Bit Rate */
               VE_GET_U32_FRM_OSXL(dlBitRate, 
                     member->value.u.sztUEAggMaxBitrate.uEaggregateMaxBitRateDL);
               ueCb->rbInfo.tempAggBr.dl = dlBitRate;

               VE_GET_U32_FRM_OSXL(ulBitRate, \
                     member->value.u.sztUEAggMaxBitrate.uEaggregateMaxBitRateUL);
               ueCb->rbInfo.tempAggBr.ul = ulBitRate;
            }
            break;
         case Sztid_E_RABToBeMdfdLstBrModReq:
            {
               if(ROK !=  veFillRRCReCfgFromERABMdfyReq(nhDatReqEvntSdu, ueCb,\
                        conRecfgIEs, member))
               {
#ifdef DEBUGP
                  VE_DBG_ERROR((VE_PRNT_BUF,"veFillRRCReCfgFromERABMdfyReq Failed \n"));
#endif
                  RETVALUE(RFAILED);
               }
            }
            break;
         default:
            break;

      }
   }

   /* MAC-MainConfig */
   if(ROK != veFillNhuMACMainCfg(&conRecfgIEs->radioResourceConfigDedicated, \
            (Ptr)nhDatReqEvntSdu, VE_PDCP_ID_DRB2))
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"Failed to Fill veFillNhuMACMainCfg \n"));
#endif
      RETVALUE(RFAILED);
   }

   /* PhysicalConfigDedicated */
   if(ROK != veFillRadResPhyCfgDed(&conRecfgIEs->radioResourceConfigDedicated, \
            (Ptr)nhDatReqEvntSdu, VE_PDCP_ID_DRB2, ueCb))
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"Failed to Fill veFillRadResPhyCfgDed \n"));
#endif
      RETVALUE(RFAILED);
   }

   /* E-RAB Modify End */
   RETVALUE(ROK);
}

/*
*
*       Fun:   veFillRrcRecfgERABRelCom
*
*       Desc:  Fills connection reconfiguration message
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRrcRecfgERABRelCom
(
VeUeCb                  *ueCb,
NhuDatReqSdus           *nhDatReqEvntSdu,
SztE_RABRlsCmmd   *s1apMsg
)
#else
PUBLIC S16 veFillRrcRecfgERABRelCom(ueCb, nhDatReqEvntSdu, s1apMsg)
VeUeCb        *ueCb;
NhuDatReqSdus *nhDatReqEvntSdu;
SztE_RABRlsCmmd   *s1apMsg;
#endif
{
   NhuDL_DCCH_Msg *dcchMsg = NULLP;
   NhuRRCConRecfgn *rrcConReCfg = NULLP;
   NhuRRCConRecfgn_r8_IEs *conRecfgIEs = NULLP;

   SztProtIE_Field_E_RABRlsCmmdIEs *member;

   U16 numOfComps = 0;
   U32 protId = 0;
   U16 memberIdx = 0;


   TRC2(veFillRrcRecfgERABRelCom);

   if(NULLP == nhDatReqEvntSdu)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRrcRecfgERABRelCom:nhDatReqEvntSdu is NULL. \n"));
#endif
      RETVALUE(RFAILED);
   }

   dcchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   conRecfgIEs = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   /* TransId for ERAB Release Command */
   VE_FILL_TKN_UINT(rrcConReCfg->rrc_TrnsxnId, VE_ERAB_RLS_COMM);

   numOfComps = s1apMsg->protocolIEs.noComp.val;

   /* DedicatedInfoNAS List */
   for(memberIdx = 0; memberIdx < numOfComps; memberIdx++)
   {
      member = &(s1apMsg->protocolIEs.member[memberIdx]);
      protId = member->id.val;
      switch(protId)
      {
         case Sztid_E_RABToBeRlsdLst:
            {
               /* fills 
                * 1)DRB-ToAddModList
                * based on the values in the  ERAB-Release
                * message.
                */
               veFillRRCReCfgFromERABRelCmd(nhDatReqEvntSdu, ueCb,\
                     conRecfgIEs, member);
            }
            break;
/* ADD CRID:ccpu00117391  ERAB_RELEASE feature*/
         case Sztid_NAS_PDU:
            {
               /* fills 
                * 1)DedicatedInfoNAS List
                * based on the values in the ERAB-Release
                * message.
                */
               veFillRRCReCfgNASFromERABRelCmd(nhDatReqEvntSdu, \
                     conRecfgIEs, member);

            }
            break;
         default:
            break;
      }
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   veFillNhuRrcConReCfgnWithMbuf
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillNhuRrcConReCfgnWithMbuf
(
VeUeCb            *ueCb,
NhuDatReqSdus *nhDatReqEvntSdu,
Buffer        *mBuf
)
#else
PUBLIC S16 veFillNhuRrcConReCfgnWithMbuf(ueCb, nhDatReqEvntSdu, mBuf)
VeUeCb        *ueCb;
NhuDatReqSdus *nhDatReqEvntSdu;
Buffer        *mBuf;
#endif
{
   S16 ret = ROK;

   TRC2(veFillNhuRrcConReCfgnWithMbuf);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillNhuRrcConReCfgnWithMbuf: Building\
            RRC Reconfiguration Message \n"));
#endif
   /* Fill HDR Info */
   veFillNhuEvntHdr(&(nhDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_RRCCONRECFGN);

   /* Populating NHU Dat Req */
   nhDatReqEvntSdu->sdu.ueMsg.isMsgDed = TRUE;
   nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = VE_PDCP_ID_SRB1;
   nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.isMsgEncoded = TRUE;
   nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.mBuf = mBuf;

   /* Fill Config values */
   nhDatReqEvntSdu->sdu.isUeCfgPres = FALSE;

   RETVALUE(ret);
} /* End of veFillNhuRrcConReCfgnWithMbuf */




/*
*
*       Fun:   veFillNhuRrcConReCfgn
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillNhuRrcConReCfgn
(
VeUeCb            *ueCb,
NhuDatReqSdus *nhDatReqEvntSdu,
U8            s1Event
)
#else
PUBLIC S16 veFillNhuRrcConReCfgn(ueCb, nhDatReqEvntSdu, s1Event)
VeUeCb        *ueCb;
NhuDatReqSdus *nhDatReqEvntSdu;
U8            s1Event;
#endif
{

   NhuDL_DCCH_Msg *dcchMsg = NULLP;
   NhuRRCConRecfgn *rrcConReCfg = NULLP;
   NhuRRCConRecfgn_r8_IEs *conRecfgIEs = NULLP;
   NhuUeCfgInfo  *ueCfg = NULLP;

   S1apPdu       *s1apPdu = NULLP;

   SztInitCntxtSetupRqst *initCntxtSetup = NULLP;
   SztUECntxtModificationRqst *ueCntxtMod = NULLP;
   SztE_RABSetupRqst *eRABSetupReq = NULLP;
   SztE_RABMdfyRqst *eRABMdfyRqst = NULLP;
   SztE_RABRlsCmmd *eRABRlsCom = NULLP;

   S16 ret = ROK;

   TRC2(veFillNhuRrcConReCfgn);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillNhuRrcConReCfgn: Building\
            RRC Reconfiguration Message \n"));
#endif

   /* Populating NHU Dat Req */
   nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = VE_PDCP_ID_SRB1;
   dcchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   nhDatReqEvntSdu->sdu.ueMsg.isMsgDed = TRUE;

   /* Fill Config values */
   nhDatReqEvntSdu->sdu.isUeCfgPres = TRUE;
   /*- Fill the UE configuration information --*/
   ueCfg = &(nhDatReqEvntSdu->sdu.ueCfg);

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
   s1apPdu = (S1apPdu *)ueCb->pdu;

   switch(s1Event)
   {
      case Sztid_InitCntxtSetup:

         /* retrieve info from S1AP Pdu */
         initCntxtSetup = (SztInitCntxtSetupRqst *) &(s1apPdu->pdu.val.initiatingMsg.\
               value.u.sztInitCntxtSetupRqst);
#ifdef VE_SRB2_SUPPORT
         veFillNhuSRBAddModLst(&(conRecfgIEs->radioResourceConfigDedicated), 
               (Ptr)nhDatReqEvntSdu, VE_PDCP_ID_SRB2);
#endif
/*RRC ReEst */
        if(ueCb->rrcConnState != VE_SRB2_DRB_SUSPENDED)
         {  
#ifdef LTE_HO_SUPPORT
         /* Filling the measurement configuration */
         veFillNhuMeasCfgInfo(&(conRecfgIEs->measConfig),nhDatReqEvntSdu);
#endif
          }
         /* Filling RRC Recfg msg from Initial context setup msg. */
         ret = veFillRrcRecfgInitCntxtSetup(ueCb, nhDatReqEvntSdu, initCntxtSetup);
         if (ret != ROK)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veFillNhuRrcConReCfgn: veFillRrcRecfgInitCntxtSetup Failed \n"));
#endif

            VE_FREEEVNT(nhDatReqEvntSdu);
            RETVALUE(RFAILED);
         }

         /* Fill respective config related info. */
         ret = veFillNhuUeCfgInfo(nhDatReqEvntSdu, ueCfg, ueCb, C4_RRCCONRECFGN);
         if (ret != ROK)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veFillNhuRrcConReCfgn: veFillNhuUeCfgInfo Failed \n"));
#endif

            VE_FREEEVNT(nhDatReqEvntSdu);
            RETVALUE(RFAILED);
         }
         break;

      case Sztid_E_RABSetup:
         /* retrieve info from S1AP Pdu */
         eRABSetupReq = (SztE_RABSetupRqst *) &(s1apPdu->pdu.val.initiatingMsg.\
               value.u.sztE_RABSetupRqst);

         ret = veFillRrcRecfgERABSetReq(ueCb, nhDatReqEvntSdu, eRABSetupReq);
         if (ret != ROK)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veFillNhuRrcConReCfgn: veFillRrcRecfgERABSetReq\
                     Failed \n"));
#endif

            VE_FREEEVNT(nhDatReqEvntSdu);
            RETVALUE(RFAILED);
         }
         break;

      case Sztid_E_RABMdfy:
         /* retrieve info from S1AP Pdu */

         /* E-RAB Modify */
         eRABMdfyRqst = (SztE_RABMdfyRqst *) &(s1apPdu->pdu.val.initiatingMsg.\
               value.u.sztE_RABMdfyRqst);

         ret = veFillRrcRecfgERABModReq(ueCb, nhDatReqEvntSdu, eRABMdfyRqst);
         if (ret != ROK)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veFillNhuRrcConReCfgn: veFillRrcRecfgERABModReq\
                     Failed \n"));
#endif

            VE_FREEEVNT(nhDatReqEvntSdu);
            RETVALUE(RFAILED);
         }
         break;  /* E-RAB Modify End*/
         
      case Sztid_E_RABRls:
         eRABRlsCom = (SztE_RABRlsCmmd *) &(s1apPdu->pdu.val.initiatingMsg.\
               value.u.sztE_RABRlsCmmd);

         ret = veFillRrcRecfgERABRelCom(ueCb, nhDatReqEvntSdu, eRABRlsCom);
         if (ret != ROK)
         {
#ifdef DEBUGP
            VE_DBG_ERROR((VE_PRNT_BUF,"veFillNhuRrcConReCfgn: veFillRrcRecfgERABRelCom\
                     Failed \n"));
#endif

            VE_FREEEVNT(nhDatReqEvntSdu);
            RETVALUE(RFAILED);
         }
         break;

      case Sztid_UECntxtModification:
         ueCntxtMod = &(s1apPdu->pdu.val.initiatingMsg.value.u.\
                        sztUECntxtModificationRqst);
         break;
      default:
         break;
   }

   RETVALUE(ROK);
} /* End of veFillNhuRrcConReCfgn */

/*
*
*       Fun:   veFillRRCReCfgFromERABSetupReq
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRCReCfgFromERABSetupReq
(
NhuDatReqSdus           *evnt,
VeUeCb                      *ueCb,
NhuRRCConRecfgn_r8_IEs *reCfgIes,
SztProtIE_Field_E_RABSetupRqstIEs *member
)
#else
PUBLIC S16 veFillRRCReCfgFromERABSetupReq(evnt, ueCb, reCfgIes, member)
NhuDatReqSdus           *evnt;
VeUeCb                       *ueCb;
NhuRRCConRecfgn_r8_IEs *reCfgIes;
SztProtIE_Field_E_RABSetupRqstIEs *member;
#endif
{
   SztProtIE_SingleCont_E_RABToBeSetupItemBrSUReqIEs *protIe = NULLP;
   SztE_RABToBeSetupItemBrSUReq *itemIe = NULLP;

   NhuUeCfgInfo  *ueCfg = NULLP;
   NhuRbCfgList  *rbCfgList = NULLP;
   NhuRbCfgInfo *rbCfg = NULLP;

   NhuRRCConRecfgn_r8_IEsdedicatedInfoNASLst *nasLst = NULLP; 
   NhuRadioResourceConfigDedicated           *rrcCfgDed = NULLP;
   NhuDRB_ToAddMod                           *drbIe = NULLP;
   NhuLogChannelConfigul_SpecificParams      *chCfgParams = NULLP;

   U8  rbIdx = 0;
   U8  nasIdx = 0;
   U8  freeIdx = 0;
   U8  shiftBits = 0;
   U16 noComp = 0;
   U16 idx = 0;
   U16 ipAddrIdx = 0;
   U32 dlBitRate = 0;
   U32 ulBitRate = 0;
   U32 addrMsk = 0;


   TRC2(veFillRRCReCfgFromERABSetupReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRCReCfgFromERABSetupReq:\
            Building RRC Reconfiguration Msg - ERAB Setup List \n"));
#endif

   /* the number of RABs to be setup */
   noComp = member->value.u.sztE_RABToBeSetupLstBrSUReq.noComp.val;

   /* ================================================== 
    * Fill UE Configuration
    * ================================================== */
   {
      /* Fill Config values */
      evnt->sdu.isUeCfgPres = TRUE;
      /*- Fill the UE configuration information --*/
      ueCfg = &(evnt->sdu.ueCfg);
      /* filling UeCfg type*/
      ueCfg->ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
      ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_ADD;

      rbCfgList = &(ueCfg->rbCfgAllList.rbToAddCfgList);
      /* klock warning resolved */
      rbCfgList->numOfRbCfgInfo = (U8)noComp;
      VE_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * noComp),\
            &(rbCfgList->rbCfgInfoList));
   }

   /* initializing */
   rrcCfgDed = &(reCfgIes->radioResourceConfigDedicated);
   VE_FILL_TKN_UINT(rrcCfgDed->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(rrcCfgDed->drb_ToAddModLst.noComp, noComp);

   /* Allocate memory for each Rab */
   VE_GET_MEM(evnt, (sizeof(NhuDRB_ToAddMod) * noComp),\
         &(rrcCfgDed->drb_ToAddModLst.member));

   for(idx = 0; idx < noComp; idx++)
   {
      protIe = &(member->value.u.sztE_RABToBeSetupLstBrSUReq.member[idx]);
      itemIe = &(protIe->value.u.sztE_RABToBeSetupItemBrSUReq);

      /* Update  local UE Cb 
       * We assume that drb start */
      /* find an rbIdx which is not in use */
      for (freeIdx = 0; freeIdx < VE_MAX_RABS_IN_UE; freeIdx++)
      {
         if(ueCb->rbInfo.rab[freeIdx].inUse == VE_DRB_NOT_IN_USE )
         {
            rbIdx = freeIdx;
            ueCb->rbInfo.rab[rbIdx].inUse = VE_DRB_CONNECTING;
            break;
         }
      }

      /* increment numbers of DRBs Cfg */
      ueCb->rbInfo.numOfRbCfgd++;

      ueCb->rbInfo.rab[rbIdx].rbType = CM_LTE_DRB;
      /* dont always send VE_PDCP_ID_DRB2 */
      ueCb->rbInfo.rab[rbIdx].rbId = 3 + rbIdx; /* starting from 4 .. 10 */
      ueCb->rbInfo.rab[rbIdx].eRABId = itemIe->e_RAB_ID.val;
      ueCb->rbInfo.rab[rbIdx].pdcpId = 3 + rbIdx;      
      ueCb->rbInfo.rab[rbIdx].qci = itemIe->e_RABlevelQoSParams.qCI.val;      
#ifdef LTE_HO_SUPPORT
      ueCb->rbInfo.rab[rbIdx].rbDir = VE_DL_UL;
      ueCb->rbInfo.rab[rbIdx].rbMode = NHU_RLC_MODE_AM; 
#endif

      VE_GET_U32_FRM_OSXL(dlBitRate, \
            itemIe->e_RABlevelQoSParams.gbrQosInform.e_RAB_MaxBitrateDL);
      ueCb->rbInfo.rab[rbIdx].mbr.dl = dlBitRate;  

      VE_GET_U32_FRM_OSXL(ulBitRate, \
            itemIe->e_RABlevelQoSParams.gbrQosInform.e_RAB_MaxBitrateUL);
      ueCb->rbInfo.rab[rbIdx].mbr.ul = ulBitRate;

      VE_GET_U32_FRM_OSXL(dlBitRate, \
            itemIe->e_RABlevelQoSParams.gbrQosInform.e_RAB_GuaranteedBitrateDL);
      ueCb->rbInfo.rab[rbIdx].gbr.dl = dlBitRate;

      VE_GET_U32_FRM_OSXL(ulBitRate, \
            itemIe->e_RABlevelQoSParams.gbrQosInform.e_RAB_GuaranteedBitrateUL);
      ueCb->rbInfo.rab[rbIdx].gbr.ul = ulBitRate;

      /* Allocate memory for the octect string */ 
      VE_ALLOC(&(ueCb->rbInfo.rab[rbIdx].remEgtpTunId.val), (itemIe->gTP_TEID.len));
      /* Copy Byte by Byte */
      cmMemcpy(ueCb->rbInfo.rab[rbIdx].remEgtpTunId.val, itemIe->gTP_TEID.val, itemIe->gTP_TEID.len);
      ueCb->rbInfo.rab[rbIdx].remEgtpTunId.len = itemIe->gTP_TEID.len;
      /* Store local Tunnel Id */
      /* if(veCb.mmeCb) 
      {
         VE_GET_LOC_TUNNEL_ID((veCb.mmeCb)->locEgtpTunCntr);
         ueCb->rbInfo.rab[rbIdx].locEgtpTunId = (veCb.mmeCb)->locEgtpTunCntr;
      } */
      if(ueCb->s1ConCb->mmeCb)
      {
         VE_GET_LOC_TUNNEL_ID((ueCb->s1ConCb->mmeCb)->locEgtpTunCntr);
         ueCb->rbInfo.rab[rbIdx].locEgtpTunId = (ueCb->s1ConCb->mmeCb)->locEgtpTunCntr;
      }

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
      /* retrieve Serv. GW Address later will be used for tunnel creation */
      {
         switch(itemIe->transportLyrAddr.len)
         {
            case 32:
               ueCb->rbInfo.rab[rbIdx].sgwAddr.type = CM_TPTADDR_IPV4;
               ueCb->rbInfo.rab[rbIdx].sgwAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

               /* copy 4bytes into the U32 */
               /* klock warning resolved */
               shiftBits = (U8)(itemIe->transportLyrAddr.len / 8);
               addrMsk = 0xFF000000;
               for(ipAddrIdx = 0; ipAddrIdx < 4; ipAddrIdx++)
               {
                  shiftBits--;
                  ueCb->rbInfo.rab[rbIdx].sgwAddr.u.ipv4TptAddr.address |=
                     ((U32)(itemIe->transportLyrAddr.val[ipAddrIdx] << (8 * shiftBits)) & addrMsk);
                  addrMsk = addrMsk >> 8;
               }
               break;

            case 128:
               ueCb->rbInfo.rab[rbIdx].sgwAddr.type = CM_TPTADDR_IPV6;
               ueCb->rbInfo.rab[rbIdx].sgwAddr.u.ipv6TptAddr.port = VE_DFLT_EGTP_PORT; 
               for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
               {
                  ueCb->rbInfo.rab[rbIdx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = 
                     itemIe->transportLyrAddr.val[ipAddrIdx];
               }
               break;

            default:
               /* would be in case if len = 160 bits which means support for both ipv4 and
                * ipv6 */
               break;
         }
      }
      /*RRC ReEst */
      if(ueCb->rrcConnState != VE_SRB2_DRB_SUSPENDED)
      {
      /* NAS PDU list */
      if(itemIe->nAS_PDU.pres)
      {
         nasLst = &(reCfgIes->dedicatedInfoNASLst);

         nasLst->noComp.pres = PRSNT_NODEF;
         nasIdx = (U8)nasLst->noComp.val;/* klock warning resolved */
         /* Update RRC Cfg Pdu structure */
         nasLst->noComp.val++;

         /* Allocating memory for TknStrOSXL */
         VE_GET_MEM(evnt, (sizeof(NhuDedicatedInfoNAS)),\
               &nasLst->member);

         nasLst->member[nasIdx].pres = PRSNT_NODEF;
         nasLst->member[nasIdx].len = itemIe->nAS_PDU.len;

         /* Allocate memory for the octect string */
         VE_GET_MEM(evnt, itemIe->nAS_PDU.len,\
               &(nasLst->member[nasIdx].val));

         nasLst->member[nasIdx].pres = PRSNT_NODEF;
         nasLst->member[nasIdx].len = itemIe->nAS_PDU.len;

         /* Copy Byte by Byte */
         cmMemcpy((U8 *)nasLst->member[nasIdx].val, (U8 *)itemIe->nAS_PDU.val,\
               (itemIe->nAS_PDU.len));
      }
     }
      /* ================================================== 
       * Fill UE Configuration
       * ================================================== */
      {
         rbCfg = (NhuRbCfgInfo *) &(rbCfgList->rbCfgInfoList[idx]);
         rbCfg->rbCfgBitMsk = (NHU_RB_CFG_UL |\
               NHU_RB_CFG_DL | NHU_RB_CFG_PDCP_INFO); 

         rbCfg->rbId.rbId = ueCb->rbInfo.rab[rbIdx].rbId;
         rbCfg->rbId.rbTyp = CM_LTE_DRB;


         /* UL Logical Channel Mapping Info */
         /* Bit mask calculate properly */
         rbCfg->ulRbCfg.ulRbBitMsk |= 
            (NHU_RB_CFG_LOGCH_MAP_UL | NHU_RB_CFG_RLC_MODE_UL);

         /* Uplink Logical Channel Mapping Info.  */
         {
            /* Transport Channel type */
            rbCfg->ulRbCfg.ulLogChMap.trChType = CM_LTE_TRCH_UL_SCH;

            /* Logical Channel Id and Type */
            rbCfg->lcId.lcId = rbCfg->rbId.rbId;
            rbCfg->lcId.logChType = (CmLteLcType)CM_LTE_LCH_DTCH;

            /* Fill Logical Channel Cfg */
            veFillUlLogChCfg(&(rbCfg->ulRbCfg.ulLogChMap), rbCfg->rbId.rbId);

            /* ulRlcMode */
            rbCfg->ulRbCfg.ulRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            rbCfg->ulRbCfg.ulRlcMode.mode = NHU_RLC_MODE_AM;  
            veFillUlRlcModeInfo(&(rbCfg->ulRbCfg.ulRlcMode), rbCfg->rbId.rbId);
         }

         /* Downlink Logical Channel Mapping Info */
         rbCfg->dlRbCfg.dlRbBitMsk |=
            (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* Downlink Logical Channel Mapping Info. */
         {
            /* Logical Channel ID and type */
            rbCfg->lcId.lcId = rbCfg->rbId.rbId;
            rbCfg->lcId.logChType = (CmLteLcType)CM_LTE_LCH_DTCH;
            /* Transport Channel Type */
            rbCfg->dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_DL_SCH;
            /* DL-RLC mode */
            rbCfg->dlRbCfg.dlRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            rbCfg->dlRbCfg.dlRlcMode.mode = NHU_RLC_MODE_AM;
            veFillDlRlcModeInfo(&(rbCfg->dlRbCfg.dlRlcMode), rbCfg->rbId.rbId);
         }

         /* PDCP Cfg */
         veFillPdcpCfg(&(rbCfg->pdcpCfg), rbCfg->rbId.rbId, FALSE);
      }
   } /* end of for loop */

   RETVALUE(ROK);

} /* veFillRRCReCfgFromERABSetupReq */

/* E-RAB Modify */
/*
*
*       Fun:   veFillRRCReCfgFromERABMdfyReq
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRCReCfgFromERABMdfyReq
(
NhuDatReqSdus                     *evnt,
VeUeCb                            *ueCb,
NhuRRCConRecfgn_r8_IEs            *reCfgIes,
SztProtIE_Field_E_RABMdfyRqstIEs  *member
)
#else
PUBLIC S16 veFillRRCReCfgFromERABMdfyReq(evnt, ueCb, reCfgIes, member)
NhuDatReqSdus                     *evnt;
VeUeCb                            *ueCb;
NhuRRCConRecfgn_r8_IEs            *reCfgIes;
SztProtIE_Field_E_RABMdfyRqstIEs  *member;
#endif
{
   /*   SztProtIE_SingleCont_E_RABToBeMdfdItemBrModReqIEs *protIe = NULLP;
        SztE_RABToBeMdfdItemBrModReq *itemIe = NULLP;    */
   SztE_RABToBeMdfdLstBrModReq               *rabLst = NULLP;
   SztE_RABToBeMdfdItemBrModReq              *rabInfo =  NULLP;

   NhuUeCfgInfo                              *ueCfg = NULLP;
   NhuRbCfgList                              *rbCfgList = NULLP;
   NhuRbCfgInfo                              *rbCfg = NULLP;
   NhuDRB_ToAddMod                           *drbIe = NULLP;
   NhuRadioResourceConfigDedicated           *rrcCfgDed = NULLP;
   NhuLogChannelConfigul_SpecificParams      *chCfgParams = NULLP;
   NhuRRCConRecfgn_r8_IEsdedicatedInfoNASLst *nasLst = NULLP; 

   U8  rbIdx      = 0;
   U8  nasIdx     = 0;
   U8  erabIdx    = 0;
   U16 noComp     = 0;
   U16 idx        = 0;
   U32 dlBitRate  = 0;
   U32 ulBitRate  = 0;
   U8 rcvdQciType = 0;
   U8 cntQciType  = 0;
   U8 failIdx     = 0;
   Bool dupRbId;

   TRC2(veFillRRCReCfgFromERABMdfyReq);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRCReCfgFromERABMdfyReq:\
            Building RRC Reconfiguration Msg - ERAB Modify List \n"));
#endif

  
   /* the number of RABs to be modify*/
   rabLst = &(member->value.u.sztE_RABToBeMdfdLstBrModReq);
   noComp = rabLst->noComp.val;

   /* ================================================== 
    * Fill UE Configuration
    * ================================================== */
   {
      /* Fill Config values */
      evnt->sdu.isUeCfgPres = TRUE;
      /*- Fill the UE configuration information --*/
      ueCfg = &(evnt->sdu.ueCfg);
      /* filling UeCfg type*/
      ueCfg->ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
      ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_MODIFY;

      rbCfgList = &(ueCfg->rbCfgAllList.rbToModCfgList);
      rbCfgList->numOfRbCfgInfo = (U8)noComp;
      VE_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * noComp),\
            &(rbCfgList->rbCfgInfoList));
   }

   /* initializing */
   rrcCfgDed = &(reCfgIes->radioResourceConfigDedicated);
   VE_FILL_TKN_UINT(rrcCfgDed->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(rrcCfgDed->drb_ToAddModLst.noComp, noComp);

   /* Allocate memory for each Rab */
   VE_GET_MEM(evnt, (sizeof(NhuDRB_ToAddMod) * noComp),\
         &(rrcCfgDed->drb_ToAddModLst.member));

   for(idx = 0; idx < noComp; idx++)
   {
      rabInfo = &rabLst->member[idx].value.u.sztE_RABToBeMdfdItemBrModReq;

      for (erabIdx  = 0; erabIdx < VE_MAX_RABS_IN_UE; erabIdx++)
      {
         if(ueCb->rbInfo.rab[erabIdx].inUse == TRUE)
         {
            if(ueCb->rbInfo.rab[erabIdx].eRABId == rabInfo->e_RAB_ID.val)
            {
  
               rbIdx = erabIdx;
          break;
            }
         }
      }

      VE_FIND_QCI_TYPE (rabInfo->e_RABLvlQoSParams.qCI.val, rcvdQciType);
      VE_FIND_QCI_TYPE (ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.qci, cntQciType);

      if ((0 == cntQciType) || (rcvdQciType != cntQciType))
      {
         /* Error occurred, related to the QCI value. So queuing E-RAB ID for failure RSP */
         ueCb->rbInfo.rabMdfyFailLst[failIdx ++] = rbIdx;
         continue;
      } 

      {     
         U8 _idx;                                          
         dupRbId = FALSE;                                     
         for (_idx =0; _idx < VE_MAX_RABS_IN_UE; _idx++)  
         {                                                 
            if(ueCb->rbInfo.rabMdfyFailLst[_idx] == rbIdx) 
            {                                              
               dupRbId = TRUE;                                
               break;                                      
            }                                              
            if(ueCb->rbInfo.rabLst[_idx] == rbIdx)         
            {                                              
               dupRbId = TRUE;                                
               break;                                      
            }                                              
         }                                                 
      }
      
      if( dupRbId == TRUE)
      {
         /* Error occurred, related to the QCI value. So queuing E-RAB ID for failure RSP */
         ueCb->rbInfo.rabMdfyFailLst[failIdx ++] = rbIdx;
         continue;
      }

   

      ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.qci = rabInfo->e_RABLvlQoSParams.qCI.val;      

      if (rabInfo->e_RABLvlQoSParams.gbrQosInform.pres.pres == PRSNT_NODEF)
      {
         SztGBR_QosInform    *gbrIe;
         gbrIe = &rabInfo->e_RABLvlQoSParams.gbrQosInform;

         VE_GET_U32_FRM_OSXL(dlBitRate, gbrIe->e_RAB_MaxBitrateDL);
         ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.mbr.dl = dlBitRate;  

         VE_GET_U32_FRM_OSXL(ulBitRate, gbrIe->e_RAB_MaxBitrateUL);
         ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.mbr.ul = ulBitRate;

         VE_GET_U32_FRM_OSXL(dlBitRate, gbrIe->e_RAB_GuaranteedBitrateDL);
         ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.gbr.dl = dlBitRate;

         VE_GET_U32_FRM_OSXL(ulBitRate, gbrIe->e_RAB_GuaranteedBitrateUL);
         ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.gbr.ul = ulBitRate;

         ueCb->rbInfo.rab[rbIdx].rabMdfyInfo.rabInfoModified = TRUE;
      }
   

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

   
      /* NAS PDU list */
      if(rabInfo->nAS_PDU.pres)
      {
         nasLst = &(reCfgIes->dedicatedInfoNASLst);

         nasLst->noComp.pres = PRSNT_NODEF;
         nasIdx = (U8)nasLst->noComp.val;
         /* Update RRC Cfg Pdu structure */
         nasLst->noComp.val++;

         /* Allocating memory for TknStrOSXL */
         VE_GET_MEM(evnt, (sizeof(NhuDedicatedInfoNAS)),\
               &nasLst->member);

         nasLst->member[nasIdx].pres = PRSNT_NODEF;
         nasLst->member[nasIdx].len = rabInfo->nAS_PDU.len;

         /* Allocate memory for the octect string */
         VE_GET_MEM(evnt, rabInfo->nAS_PDU.len,\
               &(nasLst->member[nasIdx].val));


         /* Copy Byte by Byte */
         cmMemcpy((U8 *)nasLst->member[nasIdx].val, (U8 *)rabInfo->nAS_PDU.val,\
               (rabInfo->nAS_PDU.len));
      }

      /* ================================================== 
       * Fill UE Configuration
       * ================================================== */
      {
         rbCfg = (NhuRbCfgInfo *) &(rbCfgList->rbCfgInfoList[idx]);
         rbCfg->rbCfgBitMsk = (NHU_RB_CFG_UL |\
               NHU_RB_CFG_DL | NHU_RB_CFG_PDCP_INFO); 

         rbCfg->rbId.rbId = ueCb->rbInfo.rab[rbIdx].rbId;
         rbCfg->rbId.rbTyp = CM_LTE_DRB;


         /* UL Logical Channel Mapping Info */
         /* Bit mask calculate properly */
         rbCfg->ulRbCfg.ulRbBitMsk |= 
            (NHU_RB_CFG_LOGCH_MAP_UL | NHU_RB_CFG_RLC_MODE_UL);

         /* Uplink Logical Channel Mapping Info.  */
         {
            /* Transport Channel type */
            rbCfg->ulRbCfg.ulLogChMap.trChType = CM_LTE_TRCH_UL_SCH;

            /* Logical Channel Id and Type */
            rbCfg->lcId.lcId = rbCfg->rbId.rbId;
            rbCfg->lcId.logChType = (CmLteLcType)CM_LTE_LCH_DTCH;

            /* Fill Logical Channel Cfg */
            veFillUlLogChCfg(&(rbCfg->ulRbCfg.ulLogChMap), rbCfg->rbId.rbId);

            /* ulRlcMode */
            rbCfg->ulRbCfg.ulRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            rbCfg->ulRbCfg.ulRlcMode.mode = NHU_RLC_MODE_AM;  
            veFillUlRlcModeInfo(&(rbCfg->ulRbCfg.ulRlcMode), rbCfg->rbId.rbId);
         }

         /* Downlink Logical Channel Mapping Info */
         rbCfg->dlRbCfg.dlRbBitMsk |=
            (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* Downlink Logical Channel Mapping Info. */
         {
            /* Logical Channel ID and type */
            rbCfg->lcId.lcId = rbCfg->rbId.rbId;
            rbCfg->lcId.logChType = (CmLteLcType)CM_LTE_LCH_DTCH;
            /* Transport Channel Type */
            rbCfg->dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_DL_SCH;
            /* DL-RLC mode */
            rbCfg->dlRbCfg.dlRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            rbCfg->dlRbCfg.dlRlcMode.mode = NHU_RLC_MODE_AM;
            veFillDlRlcModeInfo(&(rbCfg->dlRbCfg.dlRlcMode), rbCfg->rbId.rbId);
         }

         /* PDCP Cfg */
         veFillPdcpCfg(&(rbCfg->pdcpCfg), rbCfg->rbId.rbId, FALSE);
      }
   } /* end of for loop */


   RETVALUE(ROK);

} /* veFillRRCReCfgFromERABMdfyReq */
  
/* E-RAB Modify END */

/*
*
*       Fun:   veFillRRCReCfgFromERABRelCmd
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRCReCfgFromERABRelCmd
(
NhuDatReqSdus           *evnt,
VeUeCb                       *ueCb,
NhuRRCConRecfgn_r8_IEs *reCfgIes,
SztProtIE_Field_E_RABRlsCmmdIEs *member
)
#else
PUBLIC S16 veFillRRCReCfgFromERABRelCmd(evnt, ueCb, reCfgIes, member)
NhuDatReqSdus           *evnt;
VeUeCb                      *ueCb;
NhuRRCConRecfgn_r8_IEs *reCfgIes;
SztProtIE_Field_E_RABRlsCmmdIEs *member;
#endif
{
   SztProtIE_SingleCont_E_RABItemIEs *protIe = NULLP;
   SztE_RABItem *itemIe = NULLP;

   NhuUeCfgInfo  *ueCfg = NULLP;
   NhuRbRlsCfgList  *rbCfgList = NULLP;
  
   NhuRadioResourceConfigDedicated           *rrcCfgDed = NULLP;
  

   U8  rbIdx = 0;
   U8  rbCfgd = 0;
   U8  rrcIdx = 0;
   U16 noComp = 0;
   U16 idx = 0;


   TRC2(veFillRRCReCfgFromERABRelCmd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRCReCfgFromERABRelCmd:\
            Building RRC Reconfiguration Msg - ERAB Setup List \n"));
#endif

   /* the number of RAB details */
   noComp = member->value.u.sztE_RABLst.noComp.val;

   /* ================================================== 
    * Fill UE Configuration
    * ================================================== */
   {
      /* Fill Config values */
      evnt->sdu.isUeCfgPres = TRUE;
      /*- Fill the UE configuration information --*/
      ueCfg = &(evnt->sdu.ueCfg);
      /* filling UeCfg type RRC ReEst */
      if (ueCb->reEstCause != ERAB_REQ_FAIL_ANY_CAUSE)
      {
          ueCfg->ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
      }
      else
      {
          ueCfg->ueCfgBitMsk = NHU_CONN_RECFG_AFTER_RESTB|NHU_EXP_CFG_TRCH_CFG;
      }   
      ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_RLS;
      rbCfgList = &(ueCfg->rbCfgAllList.rbToRlsCfgList);
      /* klock warning resolved */
      rbCfgList->numOfRbCfgInfo = (U8)noComp;
      VE_GET_MEM(evnt, (sizeof(NhuRbId) * noComp),\
            &(rbCfgList->rbsToRel));
   }

   /* initializing */
   rrcCfgDed = &(reCfgIes->radioResourceConfigDedicated);
   VE_FILL_TKN_UINT(rrcCfgDed->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(rrcCfgDed->drb_ToRlsLst.noComp, noComp);

   /* Allocate memory for each Rab */
   VE_GET_MEM(evnt, (sizeof(NhuDRB_Identity) * noComp),\
         &(rrcCfgDed->drb_ToRlsLst.member));
   /* number of DRBs configured */
   rbCfgd = ueCb->rbInfo.numOfRbCfgd;
   /* ERAB IDs to be released */
   for(idx = 0; idx < noComp; idx++)
   {
      protIe = &(member->value.u.sztE_RABLst.member[idx]);
      itemIe = &(protIe->value.u.sztE_RABItem);

      for (rbIdx = 0; rbIdx < rbCfgd; rbIdx++)
      {
         if(ueCb->rbInfo.rab[rbIdx].eRABId == itemIe->e_RAB_ID.val)
         {
            /* filling DRB Identity to be released */
            VE_FILL_TKN_UINT(rrcCfgDed->drb_ToRlsLst.member[rrcIdx], ueCb->rbInfo.rab[rbIdx].rbId);

            /* Update VeUeRAB in ueCb */
            /* initialize to unsed */
            ueCb->rbInfo.rab[rbIdx].inUse = VE_DRB_DELETING;
            ueCb->rbInfo.numOfRbCfgd--;

            { 
               /* ================================================== 
                * Fill UE Configuration
                * ================================================== */
               rbCfgList->rbsToRel[rrcIdx].rbId = ueCb->rbInfo.rab[rbIdx].rbId;
               rbCfgList->rbsToRel[rrcIdx].rbTyp = CM_LTE_DRB;
            }
            /* updating RRC Index */
            rrcIdx++;
         }

      }
   } 
/*RRC ReEst*/
   if(ueCb->rrcConnState != ERAB_REQ_FAIL_ANY_CAUSE)
    {
       /*FIXME*/
veFillRRCReCfgNASFromERABRelCmd(evnt,\
                     reCfgIes, member);


    }
   RETVALUE(ROK);
} /* veFillRRCReCfgFromERABRelCmd */
/* ADD CRID:ccpu00117391  ERAB_RELEASE feature*/
/*
*
*       Fun:   veFillRRCReCfgNASFromERABRelCmd
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRCReCfgNASFromERABRelCmd
(
NhuDatReqSdus           *evnt,
NhuRRCConRecfgn_r8_IEs *reCfgIes,
SztProtIE_Field_E_RABRlsCmmdIEs *member
)
#else
PUBLIC S16 veFillRRCReCfgNASFromERABRelCmd(evnt, reCfgIes, member)
NhuDatReqSdus           *evnt;
NhuRRCConRecfgn_r8_IEs *reCfgIes;
SztProtIE_Field_E_RABRlsCmmdIEs *member;
#endif
{
  
   NhuRRCConRecfgn_r8_IEsdedicatedInfoNASLst *nasLst = NULLP; 
   U8  nasIdx = 0;


   TRC2(veFillRRCReCfgNASFromERABRelCmd);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRCReCfgNASFromERABRelCmd:\
            Building RRC Reconfiguration Msg - ERAB Setup List \n"));
#endif
   /* NAS PDU list; Only NAS PDU would exist */
   if(member->value.u.sztNAS_PDU.pres == PRSNT_NODEF)
   {
      nasLst = &(reCfgIes->dedicatedInfoNASLst);

      nasLst->noComp.pres = PRSNT_NODEF;
      nasLst->noComp.val = VE_VAL_ONE;
      nasIdx = 0;

      /* Allocating memory for TknStrOSXL */
      VE_GET_MEM(evnt, (sizeof(NhuDedicatedInfoNAS)),\
            &nasLst->member);

      /* Allocate memory for the octect string */ 
      VE_GET_MEM(evnt, member->value.u.sztNAS_PDU.len,\
            &(nasLst->member[nasIdx].val));

      nasLst->member[nasIdx].pres = PRSNT_NODEF;
      nasLst->member[nasIdx].len =  member->value.u.sztNAS_PDU.len;

      /* Copy Byte by Byte */
      cmMemcpy((U8 *)nasLst->member[nasIdx].val, (U8 *)member->value.u.sztNAS_PDU.val,\
            (member->value.u.sztNAS_PDU.len));
   }
   RETVALUE(ROK);
} /* veFillRRCReCfgNASFromERABRelCmd */




/*
*
*       Fun:   veFillRRCReCfgFromInitCntxtERABSetup
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRCReCfgFromInitCntxtERABSetup
(
Ptr           evnt,
VeUeCb        *ueCb,
NhuRRCConRecfgn_r8_IEs *reCfgIes,
SztProtIE_Field_InitCntxtSetupRqstIEs *member
)
#else
PUBLIC S16 veFillRRCReCfgFromInitCntxtERABSetup(evnt, ueCb, reCfgIes, member)
Ptr           evnt;
VeUeCb        *ueCb;
NhuRRCConRecfgn_r8_IEs *reCfgIes;
SztProtIE_Field_InitCntxtSetupRqstIEs *member;
#endif
{
   SztProtIE_SingleCont_E_RABToBeSetupItemCtxtSUReqIEs *protIe = NULLP;
   SztE_RABToBeSetupItemCtxtSUReq *itemIe = NULLP;

   NhuRRCConRecfgn_r8_IEsdedicatedInfoNASLst *nasLst = NULLP; 
   NhuRadioResourceConfigDedicated           *rrcCfgDed = NULLP;
   NhuDRB_ToAddMod                           *drbIe = NULLP;
   NhuLogChannelConfigul_SpecificParams      *chCfgParams = NULLP;

   U8  rbIdx = 0;
   U8  freeIdx = 0;
   U8  nasIdx = 0;
   U8  shiftBits = 0;
   U16 noComp = 0;
   U16 idx = 0;
   U16 ipAddrIdx = 0;
   U32 dlBitRate = 0;
   U32 ulBitRate = 0;
   U32 addrMsk = 0;


   TRC2(veFillRRCReCfgFromInitCntxtERABSetup);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRCReCfgFromInitCntxtERABSetup:\
            Building RRC Reconfiguration Msg - ERAB Setup List \n"));
#endif

   /* the number of RAB details */
   noComp = member->value.u.sztE_RABToBeSetupLstCtxtSUReq.noComp.val;

   /* initializing */
   rrcCfgDed = &(reCfgIes->radioResourceConfigDedicated);
   VE_FILL_TKN_UINT(rrcCfgDed->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(rrcCfgDed->drb_ToAddModLst.noComp, noComp);

   /* Allocate memory for each Rab */
   VE_GET_MEM(evnt, (sizeof(NhuDRB_ToAddMod) * noComp),\
         &(rrcCfgDed->drb_ToAddModLst.member));

   for(idx = 0; idx < noComp; idx++)
   {
      protIe = &(member->value.u.sztE_RABToBeSetupLstCtxtSUReq.member[idx]);
      itemIe = &(protIe->value.u.sztE_RABToBeSetupItemCtxtSUReq);

      /* find an rbIdx which is not in use */
      for (freeIdx = 0; freeIdx < VE_MAX_RABS_IN_UE; freeIdx++)
      {
         if(ueCb->rbInfo.rab[freeIdx].inUse == VE_DRB_NOT_IN_USE)
         {
            rbIdx = freeIdx;
            ueCb->rbInfo.rab[rbIdx].inUse = VE_DRB_CONNECTING;
            break;
         }
      }

      /* increment numbers of DRBs Cfg */
      ueCb->rbInfo.numOfRbCfgd++;

      ueCb->rbInfo.rab[rbIdx].rbType = CM_LTE_DRB;
      ueCb->rbInfo.rab[rbIdx].rbId = (rbIdx + 3); /* starting from 3 .. 10 */
      ueCb->rbInfo.rab[rbIdx].eRABId = itemIe->e_RAB_ID.val;
      ueCb->rbInfo.rab[rbIdx].pdcpId = (rbIdx + 3);      
      ueCb->rbInfo.rab[rbIdx].qci = itemIe->e_RABlevelQoSParams.qCI.val;      

      VE_GET_U32_FRM_OSXL(dlBitRate, \
            itemIe->e_RABlevelQoSParams.gbrQosInform.e_RAB_MaxBitrateDL);
      ueCb->rbInfo.rab[rbIdx].mbr.dl = dlBitRate;  

      VE_GET_U32_FRM_OSXL(ulBitRate, \
            itemIe->e_RABlevelQoSParams.gbrQosInform.e_RAB_MaxBitrateUL);
      ueCb->rbInfo.rab[rbIdx].mbr.ul = ulBitRate;

      VE_GET_U32_FRM_OSXL(dlBitRate, \
            itemIe->e_RABlevelQoSParams.gbrQosInform.e_RAB_GuaranteedBitrateDL);
      ueCb->rbInfo.rab[rbIdx].gbr.dl = dlBitRate;

      VE_GET_U32_FRM_OSXL(ulBitRate, \
            itemIe->e_RABlevelQoSParams.gbrQosInform.e_RAB_GuaranteedBitrateUL);
      ueCb->rbInfo.rab[rbIdx].gbr.ul = ulBitRate;

      /* Allocate memory for the octect string */ 
      VE_ALLOC(&(ueCb->rbInfo.rab[rbIdx].remEgtpTunId.val), (itemIe->gTP_TEID.len));

      if(NULLP == ueCb->rbInfo.rab[rbIdx].remEgtpTunId.val)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veFillRRCReCfgFromInitCntxtERABSetup:\
               Failed to allocate Memory.\n"));
#endif
         RETVALUE(RFAILED);
      }
      /* Copy Byte by Byte */
      cmMemcpy(ueCb->rbInfo.rab[rbIdx].remEgtpTunId.val, itemIe->gTP_TEID.val, itemIe->gTP_TEID.len);
      ueCb->rbInfo.rab[rbIdx].remEgtpTunId.len = itemIe->gTP_TEID.len;
      /* Store local Tunnel Id */
      /* if(veCb.mmeCb)
      {
         VE_GET_LOC_TUNNEL_ID((veCb.mmeCb)->locEgtpTunCntr);
         ueCb->rbInfo.rab[rbIdx].locEgtpTunId = (veCb.mmeCb)->locEgtpTunCntr;
      }*/

      if(ueCb->s1ConCb->mmeCb)
      {
         VE_GET_LOC_TUNNEL_ID((ueCb->s1ConCb->mmeCb)->locEgtpTunCntr);
         ueCb->rbInfo.rab[rbIdx].locEgtpTunId = (ueCb->s1ConCb->mmeCb)->locEgtpTunCntr;
      }

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
         /*  changing DRB from AM to UM */
#ifndef VE_UM_MODE
         /* rlc_AM */
         VE_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_AM.pres,\
               PRSNT_NODEF);
         VE_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_AM.statusReportReqd,\
               veDfltStatusReportReqd);
#else
         /* rlc_UM */
         VE_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_UM.pres,\
               PRSNT_NODEF);
         VE_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_UM.pdcp_SN_Size,\
               NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum);
#endif
         /* headerCompression - NOT USED */
         VE_FILL_TKN_UINT(drbIe->pdcp_Config.headerCompression.choice,\
               HEADERCOMPRESSION_NOTUSED);


/*  changing DRB from AM to UM */
#ifndef VE_UM_MODE
#ifdef LTE_HO_SUPPORT
         ueCb->rbInfo.rab[rbIdx].rbDir = VE_DL_UL;
         ueCb->rbInfo.rab[rbIdx].rbMode = NHU_RLC_MODE_AM; 
#endif
         /* RLC-Config */
         VE_FILL_TKN_UINT(drbIe->rlc_Config.choice, RLC_CONFIG_AM);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.pres, PRSNT_NODEF);
         /* UL-AM-RLC */
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.pres, PRSNT_NODEF);
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
#else
#ifndef  VE_UM_MODE_BI_DIR
#ifdef LTE_HO_SUPPORT
         ueCb->rbInfo.rab[rbIdx].rbDir = VE_DL;
         ueCb->rbInfo.rab[rbIdx].rbMode = NHU_RLC_MODE_AM;
#endif
         /* RLC-Config */
         VE_FILL_TKN_UINT(drbIe->rlc_Config.choice, RLC_CONFIG_UM_UNI_DIRECTIONAL_DL);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Uni_Directional_DL.pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Uni_Directional_DL.dl_UM_RLC.pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Uni_Directional_DL.dl_UM_RLC.sn_FieldLength, 
         NhuSN_FieldLengthsize10Enum);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Uni_Directional_DL.dl_UM_RLC.t_Reordering, 
         veDfltDrbTReordrng);
#else /* ifndef  VE_UM_MODE_BI_DIR */
#ifdef LTE_HO_SUPPORT
         ueCb->rbInfo.rab[rbIdx].rbDir = VE_DL_UL;
         ueCb->rbInfo.rab[rbIdx].rbMode = NHU_RLC_MODE_UM;
#endif
         /* RLC-Config */
         VE_FILL_TKN_UINT(drbIe->rlc_Config.choice, RLC_CONFIG_UM_BI_DIRECTIONAL);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.ul_UM_RLC.pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.ul_UM_RLC.sn_FieldLength,
         NhuSN_FieldLengthsize10Enum);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.dl_UM_RLC.pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.dl_UM_RLC.sn_FieldLength,
         NhuSN_FieldLengthsize10Enum);
         VE_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.dl_UM_RLC.t_Reordering,
         veDfltDrbTReordrng);
#endif /* ifndef  VE_UM_MODE_BI_DIR */
#endif /* ifndef VE_UM_MODE */

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
      /* retrieve Serv. GW Address later will be used for tunnel creation */
      {
         switch(itemIe->transportLyrAddr.len)
         {
            case 32:
               ueCb->rbInfo.rab[rbIdx].sgwAddr.type = CM_TPTADDR_IPV4;
               ueCb->rbInfo.rab[rbIdx].sgwAddr.u.ipv4TptAddr.port = VE_DFLT_EGTP_PORT; 

               /* copy 4bytes into the U32 */
               /* klock warning resolved */
               shiftBits = (U8)(itemIe->transportLyrAddr.len / 8);
               addrMsk = 0xFF000000;
               for(ipAddrIdx = 0; ipAddrIdx < 4; ipAddrIdx++)
               {
                  shiftBits--;
                  ueCb->rbInfo.rab[rbIdx].sgwAddr.u.ipv4TptAddr.address |=
                     ((U32)(itemIe->transportLyrAddr.val[ipAddrIdx] << (8 * shiftBits)) & addrMsk);
                  addrMsk = addrMsk >> 8;
               }
               break;

            case 128:
               ueCb->rbInfo.rab[rbIdx].sgwAddr.type = CM_TPTADDR_IPV6;
               ueCb->rbInfo.rab[rbIdx].sgwAddr.u.ipv6TptAddr.port = VE_DFLT_EGTP_PORT; 
               for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
               {
                  ueCb->rbInfo.rab[rbIdx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] = 
                     itemIe->transportLyrAddr.val[ipAddrIdx];
               }
               break;

            default:
               /* would be in case if len = 160 bits which means support for both ipv4 and
                * ipv6 */
               break;
         }
      }
      /*RRC ReEst */
      if(ueCb->reEstCause != ICS_OTHER_FAIL_CSFALBK_SET)
      {
      /* NAS PDU list */
      if(itemIe->nAS_PDU.pres)
      {
         nasLst = &(reCfgIes->dedicatedInfoNASLst);

         nasLst->noComp.pres = PRSNT_NODEF;
         nasIdx = (U8)nasLst->noComp.val;/* klock warning resolved */
         /* Update RRC Cfg Pdu structure */
         nasLst->noComp.val++;

         /* Allocating memory for TknStrOSXL */
         VE_GET_MEM(evnt, (sizeof(NhuDedicatedInfoNAS)),\
               &nasLst->member);
         /* Allocate memory for the octect string */
            /* fix added for attach accept */
         VE_GET_MEM(evnt, (itemIe->nAS_PDU.len),\
               &(nasLst->member[nasIdx].val));
         nasLst->member[nasIdx].pres = PRSNT_NODEF;
         nasLst->member[nasIdx].len  = itemIe->nAS_PDU.len;

         cmMemcpy((U8 *)nasLst->member[nasIdx].val, (U8 *)itemIe->nAS_PDU.val,\
                 (itemIe->nAS_PDU.len));
      }
      }/* RRC ReEst */
   }

   RETVALUE(ROK);

} /* veFillRRCReCfgFromInitCntxtERABSetup  */



/*
*
*       Fun:   veFillNhuSRBAddModLst
*
*       Desc:  This function fills SRB(SRB1/SRB2) Add Modify List in the message 
*              to be sent to UE
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillNhuSRBAddModLst
(
NhuRadioResourceConfigDedicated *radioResCfg,
Ptr         evnt,
U8          srbId
)
#else
PUBLIC S16 veFillNhuSRBAddModLst(radioResCfg, evnt, srbId)
NhuRadioResourceConfigDedicated *radioResCfg;
Ptr         evnt;
U8          srbId;
#endif
{
   NhuSRB_ToAddMod *srbIe = NULLP;
   NhuRLC_Configam *rlcCfgAm = NULLP;
   NhuLogChannelConfig *logChCfg = NULLP;

   TRC2(veFillNhuSRBAddModLst);  

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillNhuSRBAddModLst: \
            Constructing SRBAddMod List\n"));
#endif

   /* SRB-ToAddModList */
   VE_FILL_TKN_UINT(radioResCfg->srb_ToAddModLst.noComp, VE_VAL_ONE);

   VE_GET_MEM(evnt, (sizeof(NhuSRB_ToAddMod) * \
            radioResCfg->srb_ToAddModLst.noComp.val), \
         &(radioResCfg->srb_ToAddModLst.member));

   srbIe = &(radioResCfg->srb_ToAddModLst.member[VE_VAL_ZERO]);

   VE_FILL_TKN_UINT(srbIe->pres, PRSNT_NODEF);
   if (srbId == VE_PDCP_ID_SRB1)
   {
      VE_FILL_TKN_UINT(srbIe->srb_Identity, VE_PDCP_ID_SRB1);
   }
   else
   {
      VE_FILL_TKN_UINT(srbIe->srb_Identity, VE_PDCP_ID_SRB2);
   }

   VE_FILL_TKN_UINT(srbIe->rlc_Config.choice, RLC_CONFIG_EXPLICITVALUE);

   VE_FILL_TKN_UINT(srbIe->rlc_Config.val.explicitValue.choice,\
         RLC_CONFIG_AM);

   /* RLC Configuration for AM Mode */
   rlcCfgAm = &(srbIe->rlc_Config.val.explicitValue.val.am);
   VE_FILL_TKN_UINT(rlcCfgAm->pres, PRSNT_NODEF);

   /* Uplink AM RLC Config */
   VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.t_PollRetransmit,\
         veDfltSrbPollRetrans);
   VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pollPDU, veDfltSrbPollPdu);
   VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pollByte, veDfltSrbPollByte);
   VE_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.maxRetxThreshold, \
         veDfltSrbMaxReTxThresh);

   /* Downlink AM RLC Config */
   VE_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.t_Reordering, veDfltSrbTreordrng);
   VE_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.t_StatusProhibit, \
         veDfltSrbTStatProhibit);

   /* Logical Channel Configuration for SRB */
   VE_FILL_TKN_UINT(srbIe->logicalChannelConfig.choice,\
         LOGICALCHANNELCONFIG_EXPLICITVALUE);
   logChCfg = &(srbIe->logicalChannelConfig.val.explicitValue);
   VE_FILL_TKN_UINT(logChCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(logChCfg->\
         ul_SpecificParams.pres, PRSNT_NODEF);

   if (srbId == VE_PDCP_ID_SRB1)
   {

      VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.priority,\
            veDfltSrb1LogchcfgPrior);
   }
   else
   {
      VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.priority,\
            veDfltSrb2LogchcfgPrior);

   }

   VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.prioritisedBitRate, \
         veDfltSrbLogchcfgPriBitRate);
   VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.bucketSizeDuration, \
         veDfltSrbLogchcfgBktSzDur);
   VE_FILL_TKN_UINT(logChCfg->ul_SpecificParams.logicalChannelGroup, \
         veDfltSrbLogchcfgGrp);

   RETVALUE(ROK);

} /* veFillNhuSRBAddModLst */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   veFillNhuMeasCfgInfo
*
*       Desc:  This function fills the measurement configurations.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillNhuMeasCfgInfo
(
NhuMeasConfig *measCfg,
Ptr         evnt
)
#else
PUBLIC S16 veFillNhuMeasCfgInfo(measCfg, evnt)
NhuMeasConfig *measCfg;
Ptr         evnt;
#endif
{
   S16 ret = ROK;
   NhuMeasObjectToAddMod *measObject = NULLP;
   NhuMeasObjectEUTRA *measObjEutra  = NULLP;
   NhuReportConfigToAddMod *reportCfg = NULLP;
   NhuReportConfigEUTRA *reportCfgEutra = NULLP;
   NhuMeasIdToAddMod *measId = NULLP;
   TRC2(veFillNhuMeasCfgInfo);
  
   printf("\n ********** FILLING MEASUREMENT CFG ***************\n");
   /* Fill the following configuration as per the requirements
      1. Measurement Object 
      2. Measurement Report
      3. Measurement Identity 
      4. Measurement Quantity
      5. Measurement Gap
    */

   /* Fill the Measuremnet Object Configurations */

   VE_FILL_TKN_UINT(measCfg->pres, PRSNT_NODEF);
   measCfg->measObjectToRemoveLst.noComp.pres = NOTPRSNT;
#ifdef INTER_FREQ_MEAS
   VE_FILL_TKN_UINT(measCfg->measObjectToAddModLst.noComp, VE_VAL_TWO);
#else
   measCfg->measObjectToAddModLst.noComp.val = VE_VAL_ONE;
#endif

   /* Filling EUTRA measurement object configuration for serving cell*/ 
   VE_GET_MEM(evnt, (sizeof(NhuMeasObjectToAddMod) * \
            measCfg->measObjectToAddModLst.noComp.val), \
         &(measCfg->measObjectToAddModLst.member));
   measObject = &(measCfg->measObjectToAddModLst.member[VE_VAL_ZERO]); 
   VE_FILL_TKN_UINT(measObject->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(measObject->measObjectId,veDfltMeasObjIdOne);
   VE_FILL_TKN_UINT(measObject->measObject.choice, VE_VAL_ZERO);  

   measObjEutra = &measObject->measObject.val.measObjectEUTRA;  
   VE_FILL_TKN_UINT(measObjEutra->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(measObjEutra->carrierFreq, veDfltMeasServCellDlFreq);
   /* Fill Trnasmission Bandwidth (Number Of Resource Block) refer 36.104*/
   VE_FILL_TKN_UINT(measObjEutra->allowedMeasBandwidth, veDfltMeasServCellDlBwdth);
   veFillTknU8(&measObjEutra->presenceAntennaPort1, 1);
   measObjEutra->neighCellConfig.pres = PRSNT_DEF;
   measObjEutra->neighCellConfig.len = 2;
   cmGetMem(evnt, 1, (Ptr *)&measObjEutra->neighCellConfig.val);
   *measObjEutra->neighCellConfig.val = 0x00; /* ngh cells have same DL/UL allocations as in serving cell */
   VE_FILL_TKN_UINT(measObjEutra->offsetFreq, NhuQ_OffsetRangedB0Enum);
   measObjEutra->blackCellsToRemoveLst.noComp.pres = NOTPRSNT;
   measObjEutra->blackCellsToAddModLst.noComp.pres = NOTPRSNT;
   measObjEutra->cellForWhichToReportCGI.pres =  NOTPRSNT;
#ifdef INTER_FREQ_MEAS
   /* Filling EUTRA measurement object configuration for neighbor cell 
      which operates on another carrier frequency*/ 
   measObject = &(measCfg->measObjectToAddModLst.member[VE_VAL_ONE]); 
   VE_FILL_TKN_UINT(measObject->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(measObject->measObjectId, veDfltMeasObjIdTwo);
   VE_FILL_TKN_UINT(measObject->measObject.choice, VE_VAL_ZERO);  

   measObjEutra = &measObject->measObject.val.measObjectEUTRA;  
   VE_FILL_TKN_UINT(measObjEutra->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(measObjEutra->carrierFreq, veDfltMeasNghCellDlFreq);
   /* Fill Trnasmission Bandwidth (Number Of Resource Block) refer 36.104*/
   VE_FILL_TKN_UINT(measObjEutra->allowedMeasBandwidth, 
      veDfltMeasNghCellDlBwdth);
   measObjEutra->presenceAntennaPort1.pres = NOTPRSNT;
   measObjEutra->neighCellConfig.pres = NOTPRSNT; 
   VE_FILL_TKN_UINT(measObjEutra->offsetFreq, veDfltMeaseutraFreqOffset);
   measObjEutra->blackCellsToRemoveLst.noComp.pres = NOTPRSNT;
   measObjEutra->blackCellsToAddModLst.noComp.pres = NOTPRSNT;
   measObjEutra->cellForWhichToReportCGI.pres = NOTPRSNT;
  
#endif 
   /* Fill the Measurement Report Configuration */
   measCfg->reportConfigToRemoveLst.noComp.pres = NOTPRSNT;
/* SELVA NSN-CHANGE */
   VE_FILL_TKN_UINT(measCfg->reportConfigToAddModLst.noComp, VE_VAL_ONE);
   VE_GET_MEM(evnt, (sizeof(NhuReportConfigToAddMod) * \
            measCfg->reportConfigToAddModLst.noComp.val), \
         &(measCfg->reportConfigToAddModLst.member));
   reportCfg = &(measCfg->reportConfigToAddModLst.member[VE_VAL_ZERO]);  
   VE_FILL_TKN_UINT(reportCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(reportCfg->reportConfigId, veDfltMeasRprtCfgId);
   /* Fill EUTRA Report Configuration*/
   VE_FILL_TKN_UINT(reportCfg->reportConfig.choice, VE_VAL_ZERO);
   reportCfgEutra = &reportCfg->reportConfig.val.reportConfigEUTRA;
   VE_FILL_TKN_UINT(reportCfgEutra->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(reportCfgEutra->triggerTyp.choice, VE_VAL_ZERO);
   VE_FILL_TKN_UINT(reportCfgEutra->triggerTyp.val.event.pres, PRSNT_NODEF);
   /* Fill event as A3 "Neighbour becomes amount of offset better than serving"
    */ 
   VE_FILL_TKN_UINT(reportCfgEutra->triggerTyp.val.event.eventId.choice, 
      veDfltMeasEvntA3); 
   VE_FILL_TKN_UINT(reportCfgEutra->triggerTyp.val.event.eventId.val.eventA3.pres, 
      PRSNT_NODEF);
   /* The actual offset value is (a3_Offset * 0.5 dB) i.e the 
      neighbor(target) cell should be 1 dB better than serving to enter 
      in to this event*/
   VE_FILL_TKN_UINT(
      reportCfgEutra->triggerTyp.val.event.eventId.val.eventA3.a3_Offset, 
      veDfltMeasA3Offset);
   VE_FILL_TKN_UINT(
      reportCfgEutra->triggerTyp.val.event.eventId.val.eventA3.reportOnLeave, 
      FALSE);
   VE_FILL_TKN_UINT(reportCfgEutra->triggerTyp.val.event.hysteresis, 
      veDfltMeasHysteresis);
   /* This is being set to zero so that as and when the entry
     happens into the event the measurement report will be triggered */
   VE_FILL_TKN_UINT(reportCfgEutra->triggerTyp.val.event.timeToTrigger, 
      veDfltMeasTTT);
   VE_FILL_TKN_UINT(reportCfgEutra->triggerQuantity, veDfltMeasTrgrQnty);
   VE_FILL_TKN_UINT(reportCfgEutra->reportQuantity, veDfltMeasRprtQnty);
   VE_FILL_TKN_UINT(reportCfgEutra->maxReportCells, veDfltMeasMaxRprtCells);
   VE_FILL_TKN_UINT(reportCfgEutra->reportInterval, veDfltMeasIntrvl);
   VE_FILL_TKN_UINT(reportCfgEutra->reportAmount, veDfltMeasRprtAmnt); 

   /* Fill Measurement Identity config */
   measCfg->measIdToRemoveLst.noComp.pres = NOTPRSNT;
#ifdef INTER_FREQ_MEAS
   VE_FILL_TKN_UINT(measCfg->measIdToAddModLst.noComp, VE_VAL_TWO);
#else
   measCfg->measIdToAddModLst.noComp.val = 1;
#endif
   /* Fill Measurement Identity for serving cell MeasObject */
   VE_GET_MEM(evnt, (sizeof(NhuMeasIdToAddMod) * \
            measCfg->measIdToAddModLst.noComp.val), \
         &(measCfg->measIdToAddModLst.member));
   measId = &(measCfg->measIdToAddModLst.member[VE_VAL_ZERO]); 
   VE_FILL_TKN_UINT(measId->pres, PRSNT_NODEF); 
   VE_FILL_TKN_UINT(measId->measId, veDfltMeasIdOne);
   VE_FILL_TKN_UINT(measId->measObjectId, veDfltMeasObjIdOne);
   VE_FILL_TKN_UINT(measId->reportConfigId, veDfltMeasRprtCfgId);
#ifdef INTER_FREQ_MEAS
   /* Fill Measurement Identity for neighbour cell MeasObject */
   measId = &(measCfg->measIdToAddModLst.member[VE_VAL_ONE]); 
   VE_FILL_TKN_UINT(measId->pres, PRSNT_NODEF); 
   VE_FILL_TKN_UINT(measId->measId, veDfltMeasIdTwo);
   VE_FILL_TKN_UINT(measId->measObjectId, veDfltMeasObjIdTwo);
   VE_FILL_TKN_UINT(measId->reportConfigId, veDfltMeasRprtCfgId);
#endif   
   /* Fill Measurement Quantity Configuration */
   VE_FILL_TKN_UINT(measCfg->quantityConfig.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(measCfg->quantityConfig.quantityConfigEUTRA.pres, 
      PRSNT_NODEF); 
   /* Set the filterCoefficient to 0 to disable layer 3 filtring at UE side */
   VE_FILL_TKN_UINT(
      measCfg->quantityConfig.quantityConfigEUTRA.filterCoefficientRSRP, 
      veDfltMeasL3FilterCoeff);
   VE_FILL_TKN_UINT(
      measCfg->quantityConfig.quantityConfigEUTRA.filterCoefficientRSRQ, 
      veDfltMeasL3FilterCoeff);
   measCfg->quantityConfig.quantityConfigUTRA.pres.pres = NOTPRSNT;
   measCfg->quantityConfig.quantityConfigGERAN.pres.pres = NOTPRSNT;
   measCfg->quantityConfig.quantityConfigCDMA2000.pres.pres = NOTPRSNT;

   /* Fill Measurement Gap Configuration */
#ifdef INTER_FREQ_MEAS
   VE_FILL_TKN_UINT(measCfg->measGapConfig.choice, VE_VAL_ONE);
   VE_FILL_TKN_UINT(measCfg->measGapConfig.val.setup.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(measCfg->measGapConfig.val.setup.gapOffset.choice, 
      VE_VAL_ZERO);
   VE_FILL_TKN_UINT(measCfg->measGapConfig.val.setup.gapOffset.val.gp0, 
      veDfltMeasGapOffset);
#else
   /* This can be omitted in case of intra frequency measurement as 
      it is non gap assisted */
     measCfg->measGapConfig.choice.pres = NOTPRSNT;
#endif
  
   VE_FILL_TKN_UINT(measCfg->s_Measure, veCb.cellCb[0]->rsrpTrshldRange);
   
   measCfg->preRegistrationInfoHRPD.pres.pres = NOTPRSNT;
   measCfg->speedStatePars.choice.pres = NOTPRSNT;
   
   RETVALUE(ret);
}
#endif
/*
*
*       Fun:   veFillNhuMACMainCfg
*
*       Desc:  This function fills NHU Radio Resource Configuration
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillNhuMACMainCfg
(
NhuRadioResourceConfigDedicated *radioResCfg,
Ptr         evnt,
U8          rbId
)
#else
PUBLIC S16 veFillNhuMACMainCfg(radioResCfg, evnt, rbId)
NhuRadioResourceConfigDedicated *radioResCfg;
Ptr         evnt;
U8          rbId;
#endif
{
   NhuMAC_MainConfig *macCfg = NULLP;

   TRC2(veFillNhuMACMainCfg);

   if(NULLP == radioResCfg)
   {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veFillNhuMACMainCfg:\
               radioResCfg is NULL.\n"));
#endif
         RETVALUE(RFAILED);      
   }

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillNhuMACMainCfg:\
            Filling MAC Main Cfg \n"));
#endif

   /* MAC-MainConfig  */
   VE_FILL_TKN_UINT(radioResCfg->mac_MainConfig.choice, \
         MAC_MAINCONFIG_EXPLICITVALUE);
   macCfg = &(radioResCfg->mac_MainConfig.val.explicitValue);

   VE_FILL_TKN_UINT(macCfg->pres, PRSNT_NODEF);

   /* ul-SCH-Config */
   VE_FILL_TKN_UINT(macCfg->ul_SCH_Config.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(macCfg->ul_SCH_Config.maxHARQ_Tx, veDfltMacCfgMaxHARQTx);
   VE_FILL_TKN_UINT(macCfg->ul_SCH_Config.periodicBSR_Timer, veDfltMacCfgPeriodicBSRTimer);
   VE_FILL_TKN_UINT(macCfg->ul_SCH_Config.retxBSR_Timer, veDfltMacCfgRetxBSRTimer);
   VE_FILL_TKN_UINT(macCfg->ul_SCH_Config.ttiBundling, veDfltMacCfgTtiBundling);

   /* TimeAlignmentTimer */
   VE_FILL_TKN_UINT(macCfg->timeAlignmentTimerDedicated,\
         veDfltMacCfgTimeAligTmrDed);

   /* phr-Config */ 
   VE_FILL_TKN_UINT(macCfg->phr_Config.choice, PHR_CONFIG_SETUP);
   VE_FILL_TKN_UINT(macCfg->phr_Config.val.setup.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(macCfg->phr_Config.val.setup.periodicPHR_Timer,\
         veDfltMacCfgPeriodicPHR_Timer);
   VE_FILL_TKN_UINT(macCfg->phr_Config.val.setup.prohibitPHR_Timer,\
         veDfltMacCfgProhibitPHRTimer);
   VE_FILL_TKN_UINT(macCfg->phr_Config.val.setup.dl_PathlossChange,\
         veDfltMacCfgDlPathlossChange);

   RETVALUE(ROK);
} /* veFillNhuMACMainCfg */

/*
*
*       Fun:   veFillRadResPhyCfgDed
*
*       Desc:  This function fills Physical Configuration for UE
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRadResPhyCfgDed
(
NhuRadioResourceConfigDedicated *radioResCfg,
Ptr evnt,
U8 rbId,
VeUeCb *ueCb
)
#else
PUBLIC S16 veFillRadResPhyCfgDed(radioResCfg, evnt, rbId, ueCb)
NhuRadioResourceConfigDedicated *radioResCfg;
Ptr evnt;
U8 rbId;
VeUeCb *ueCb;
#endif
{
   NhuPDSCH_ConfigDedicated        *pdschCfg = NULLP;
   NhuPUCCH_ConfigDedicated        *pucchCfg = NULLP;
   NhuPUSCH_ConfigDedicated        *puschCfg = NULLP;
   NhuUlnkPowerCntrlDedicated      *uplnkPwrCntrl = NULLP;
   NhuTPC_PDCCH_Config             *tpcPdcchCfgPucch = NULLP;
   NhuTPC_PDCCH_Config             *tpcPdcchCfgPusch = NULLP;
   NhuCQI_ReportConfig             *cqiRprtng = NULLP;
   NhuSoundingRS_UL_ConfigDedicated      *soundngRsUlCfg = NULLP;
   NhuSchedulingRqstConfig               *schedulingRqstConfig = NULLP;
   NhuPhysicalConfigDedicatedantennaInfo *antennaInfo = NULLP;

#ifndef MSPD
   NhuCQI_ReportPeriodicsetup            *cqiRepPerSetup = NULLP;
#endif

   TRC2(veFillRadResPhyCfgDed);

   if(NULLP == radioResCfg)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRadResPhyCfgDed : radioResCfg is NULL. \n"));
#endif
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRadResPhyCfgDed:\
            Filling Radio Res. Phy Cfg. Dedicated Channels \n"));
#endif

   VE_FILL_TKN_UINT(radioResCfg->physicalConfigDedicated.pres,PRSNT_NODEF);

   if(rbId == VE_PDCP_ID_SRB1)
   {
      /* PDSCH Configuration */
      pdschCfg = &(radioResCfg->physicalConfigDedicated.pdsch_ConfigDedicated);

      VE_FILL_TKN_UINT(pdschCfg->pres,PRSNT_NODEF);
      VE_FILL_TKN_UINT(pdschCfg->p_a, veDfltDedPhyCfgPa);

      /* PUCCH Configuration */
      pucchCfg = &(radioResCfg->physicalConfigDedicated.pucch_ConfigDedicated);
      VE_FILL_TKN_UINT(pucchCfg->pres, PRSNT_NODEF);
      /* Default Release - 36.508 */
      VE_FILL_TKN_UINT(pucchCfg->ackNackRepetition.choice, ACKNACKREPETITION_RELEASE);
      /* absent for FDD & TDD use bundling */
      VE_FILL_TKN_UINT(pucchCfg->tdd_AckNackFeedbackMode, veDfltTddAckNackFdbckMode);

      /* PUSCH ConfigDedicated */
      puschCfg = &(radioResCfg->physicalConfigDedicated.pusch_ConfigDedicated); 
      VE_FILL_TKN_UINT(puschCfg->pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(puschCfg->betaOffset_ACK_Idx, veDfltBetaOffACKIdx);
      VE_FILL_TKN_UINT(puschCfg->betaOffset_RI_Idx, veDfltBetaOffRIIdx);
      VE_FILL_TKN_UINT(puschCfg->betaOffset_CQI_Idx, veDfltBetaOffCQIIdx);

      /* Uplink Power Control Configuration */
      uplnkPwrCntrl = &(radioResCfg->physicalConfigDedicated.uplinkPowerCntrlDedicated); 
      VE_FILL_TKN_UINT(uplnkPwrCntrl->pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUSCH, veDfltP0UEPUSCH);
      VE_FILL_TKN_UINT(uplnkPwrCntrl->deltaMCS_Enabled, veDfltDeltaMCSEnabled);
      VE_FILL_TKN_UINT(uplnkPwrCntrl->accumulationEnabled, veDfltAccumulationEnabled);
      VE_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUCCH, veDfltP0UEPUCCH);
      VE_FILL_TKN_UINT(uplnkPwrCntrl->pSRS_Offset, veDfltpSRSOffset);
      VE_FILL_TKN_UINT(uplnkPwrCntrl->filterCoefficient, veDfltFilterCoef);


      /* CQI Reporting Related Information */
      cqiRprtng = &(radioResCfg->physicalConfigDedicated.cqi_ReportConfig);
/* MSPD changes */
#ifdef MSPD
      VE_FILL_TKN_UINT(cqiRprtng->pres, PRSNT_NODEF);
#endif

/* MSPD changes */
#ifdef MSPD
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportModeAperiodic, \
            veDfltCqiRprtModeAperiodic);
      VE_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset, \
            veDfltCqiNomPDSCHRSEPREoff);
#ifdef VE_DL_CQI
      cqiRprtng->cqi_ReportPeriodic.choice.pres = PRSNT_NODEF;
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.choice, \
            CQI_REPORTPERIODIC_SETUP);
      VE_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset, 0);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.pres, \
            PRSNT_NODEF);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_PUCCH_ResourceIdx, \
            veDfltCqiPUCCHResIdx);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_pmi_ConfigIdx, \
            ueCb->cqiPmiCfgIdx);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
                       cqi_FormatIndicatorPeriodic.choice, 
            0);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.simultaneousAckNackAndCQI, \
            veDfltSimultAckNackAndCQI);
#else
      /* NhuCQI_ReportPeriodicsetup - empty */
      cqiRprtng->cqi_ReportPeriodic.choice.pres = NOTPRSNT;
#endif
#else
      /* NhuCQI_ReportAPeriodicsetup - empty */
      cqiRprtng->cqi_ReportModeAperiodic.pres = NOTPRSNT;
      VE_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset, 0);

      VE_FILL_TKN_UINT(cqiRprtng->pres, PRSNT_NODEF);

 /* CR ID- ccpu00116764-  IOT changes*/
      cqiRprtng->cqi_ReportPeriodic.choice.pres = NOTPRSNT;
/* Dynamic CQI has been disabled*/      
#ifdef VE_DL_CQI      
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.choice, \
            CQI_REPORTPERIODIC_SETUP);
      VE_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset, 0);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.pres, \
            PRSNT_NODEF);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_PUCCH_ResourceIdx, \
            veDfltCqiPUCCHResIdx);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_pmi_ConfigIdx, \
            ueCb->cqiPmiCfgIdx);
/* MIMO changes */
#ifdef LTEMAC_MIMO
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.ri_ConfigIdx, \
            veDfltRiConfigIdx);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
                       cqi_FormatIndicatorPeriodic.choice, 
            0);
#else
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
                       cqi_FormatIndicatorPeriodic.choice, 
            1);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
                       cqi_FormatIndicatorPeriodic.val.subbandCQI.pres,
            PRSNT_NODEF);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup. \
                       cqi_FormatIndicatorPeriodic.val.subbandCQI.k,
            1);
#endif
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.simultaneousAckNackAndCQI, \
            veDfltSimultAckNackAndCQI);
#endif
#endif /* MSPD */
      /* antz - filling SR also, copy pasted from else branch below, modified
        * the part where ueCb is accessed */
      /* Scheduling Request Configuration */
      schedulingRqstConfig = &(radioResCfg->physicalConfigDedicated.\
            schedulingRqstConfig);

      VE_FILL_TKN_UINT(schedulingRqstConfig->choice,\
            SCHEDULINGRQSTCONFIG_SETUP);
      VE_FILL_TKN_UINT(schedulingRqstConfig->val.setup.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_PUCCH_ResourceIdx,\
            veDfltSrPUCCHResIdx);

      /* This function is called during Ctf Cfg Req
       * veGetSRCfgIdx(&schedulingRqstConfig->val.setup.sr_ConfigIdx);
       * and the cfg idx is derived then */

      /* antz - hardcoding 42, the first value filled in veGetSRCfgIdx()
       * A better approach may be to pass ueCb here, and to set SR;
       * then make sure that SR is not again configured during the reconfig
       * (below)
       * MSPD integration changing value to 2 for a smaller periodicity
       */
/* MSPD changes */
#ifdef MSPD
      VE_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_ConfigIdx,\
               ueCb->srCfgIdx);
      /*   VE_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_ConfigIdx,\
               2);*/
#else
        
      /* CR ID- ccpu00117665-  IOT changes
       * Modification is done from hardcoded srCfgIdx(42) into ueCb SrCfgIdx */
      VE_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_ConfigIdx,\
               ueCb->srCfgIdx);
#endif

      VE_FILL_TKN_UINT(schedulingRqstConfig->val.setup.dsr_TransMax,\
            veDfltDsrTransMax);

   }
   else
   {
      /*   tpc-PDCCH-ConfigPUCCH  empty 
       *   PDCCH-PUCCH Configuration */
      tpcPdcchCfgPucch = &(radioResCfg->physicalConfigDedicated.\
            tpc_PDCCH_ConfigPUCCH);
      tpcPdcchCfgPucch->choice.pres = NOTPRSNT;

      /* tpc-PDCCH-ConfigPUSCH 
       * PDCCH-PUSCH Configuration */
      tpcPdcchCfgPusch = &(radioResCfg->physicalConfigDedicated.\
            tpc_PDCCH_ConfigPUSCH);

      tpcPdcchCfgPusch->choice.pres = NOTPRSNT;

      /* CQI Reporting Related Information */
      cqiRprtng = &(radioResCfg->physicalConfigDedicated.cqi_ReportConfig);
#ifdef MSPD /* antz - disabling sending CQI stuff here, it was already
       * done during setup (if needed) */
      cqiRprtng->pres.pres = NOTPRSNT;
#else
      VE_FILL_TKN_UINT(cqiRprtng->pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportModeAperiodic,\
            veDfltCqiRprtModeAperiodic);
      VE_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset,\
            veDfltCqiNomPDSCHRSEPREoff);
      VE_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.choice, \
            CQI_REPORTPERIODIC_SETUP);

      /*Madhur : TODO */
      cqiRprtng->cqi_ReportPeriodic.choice.pres = NOTPRSNT;

      /* NhuCQI_ReportPeriodicsetup */ 
      cqiRepPerSetup = &(cqiRprtng->cqi_ReportPeriodic.val.setup);

      VE_FILL_TKN_UINT(cqiRepPerSetup->pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(cqiRepPerSetup->cqi_PUCCH_ResourceIdx,\
            veDfltCqiPUCCHResIdx);

      veGetPMICfgIdx(&cqiRepPerSetup->cqi_pmi_ConfigIdx);
      if(ueCb != NULLP)
      { /* klock warning resolved */
         ueCb->pmiCfgIdx = (U16)(cqiRepPerSetup->cqi_pmi_ConfigIdx.val);
      }

      VE_FILL_TKN_UINT(cqiRepPerSetup->cqi_FormatIndicatorPeriodic.choice,\
            CQI_FORMATINDICATORPERIODIC_WIDEBANDCQI);
      VE_FILL_TKN_UINT(cqiRepPerSetup->simultaneousAckNackAndCQI,\
            veDfltSimultAckNackAndCQI);
#endif /* else of MSPD */

      /* Sounding Rs Uplink Configuration */
      soundngRsUlCfg = &(radioResCfg->physicalConfigDedicated.\
            soundingRS_UL_ConfigDedicated);

      soundngRsUlCfg->choice.pres = NOTPRSNT;

      /* Scheduling Request Configuration */
      schedulingRqstConfig = &(radioResCfg->physicalConfigDedicated.\
            schedulingRqstConfig);

      VE_FILL_TKN_UINT(schedulingRqstConfig->choice,\
            SCHEDULINGRQSTCONFIG_SETUP);
      VE_FILL_TKN_UINT(schedulingRqstConfig->val.setup.pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_PUCCH_ResourceIdx,\
            veDfltSrPUCCHResIdx);

      /* This function is called during Ctf Cfg Req 
       * veGetSRCfgIdx(&schedulingRqstConfig->val.setup.sr_ConfigIdx);
       * and the cfg idx is derived then */

      if(ueCb != NULLP)
      {
         VE_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_ConfigIdx,\
               ueCb->srCfgIdx);
      }

      VE_FILL_TKN_UINT(schedulingRqstConfig->val.setup.dsr_TransMax,\
            veDfltDsrTransMax);
   }

   /*  Antenna Configuration */
   antennaInfo = &(radioResCfg->physicalConfigDedicated.antennaInfo);
   VE_FILL_TKN_UINT(antennaInfo->choice, ANTENNAINFO_EXPLICITVALUE);
   VE_FILL_TKN_UINT(antennaInfo->val.explicitValue.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(antennaInfo->val.explicitValue.transmissionMode,\
         veDfltTransMode);
/* MIMO changes */
#ifdef LTEMAC_MIMO
   VE_FILL_TKN_UINT(antennaInfo->val.explicitValue.codebookSubsetRestrn.choice,\
         CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM3);
   VE_FILL_BIT_STR(antennaInfo->val.explicitValue.codebookSubsetRestrn.val.n2TxAntenna_tm3,\
         2, 3, evnt); /* bit string 11 */
#endif
   VE_FILL_TKN_UINT(antennaInfo->val.explicitValue.ue_TransmitAntennaSelection.choice,\
         UE_TRANSMITANTENNASELECTION_RELEASE);

   RETVALUE(ROK);
} /* End of veFillRadResPhyCfgDed */



/*
*
*       Fun:   veFillNhuUlUmRlc
*
*       Desc:  This function fills Uplink RLC Configuration for UM Mode
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillNhuUlUmRlc
(
NhuUL_UM_RLC  *ulUmRlc
)
#else
PUBLIC S16 veFillNhuUlUmRlc(ulUmRlc)
NhuUL_UM_RLC  *ulUmRlc;
#endif
{
   TRC2(veFillNhuUlUmRlc);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillNhuUlUmRlc: \
            Fill Uplink UM Mode RLC Info\n"));
#endif

   VE_FILL_TKN_UINT(ulUmRlc->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(ulUmRlc->sn_FieldLength, NhuSN_FieldLengthsize10Enum);

   RETVALUE(ROK);
}

/*
*
*       Fun:   veFillNhuDlUmRlc
*
*       Desc:  This function fills Downlink RLC Configuration for UM Mode
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillNhuDlUmRlc
(
NhuDL_UM_RLC  *dlUmRlc
)
#else
PUBLIC S16 veFillNhuDlUmRlc(dlUmRlc)
NhuDL_UM_RLC  *dlUmRlc;
#endif
{
   TRC2(veFillNhuDlUmRlc);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillNhuDlUmRlc:\
            Fill Downlink UM RLC Info.\n"));
#endif

   VE_FILL_TKN_UINT(dlUmRlc->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(dlUmRlc->sn_FieldLength, NhuSN_FieldLengthsize10Enum);

   RETVALUE(ROK);
}

/*
*
*       Fun:   veFillUeAddRbCfg
*
*       Desc:  This function fills UE ADD RB Configuration 
*               in ueCfgInfo structure of event structure
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillUeAddRbCfg
(
NhuRbCfgList  **rbCfgList,
VeUeCb        *ueCb,
U8            msgType
)
#else
PUBLIC S16 veFillUeAddRbCfg(rbCfgList, ueCb, msgType)
NhuRbCfgList  **rbCfgList;
VeUeCb        *ueCb;
U8            msgType;
#endif
{
   NhuRbCfgList *lclRbCfgList = NULLP;
   NhuRbCfgInfo *rbCfg = NULLP;

   U16       idx = 0;
#if (!defined(VE_SRB2_SUPPORT) && !defined(LTE_HO_SUPPORT))
   U16       idx1 = 0;
#endif
   U8        rabId = 0;
   U8        rbtype = 0;
   U16       logChType = 0;
   U8        rlcMode = NHU_RLC_MODE_AM;


   TRC2(veFillUeAddRbCfg);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillUeAddRbCfg:\
            Fill Add UE RB Cfg.\n"));
#endif

   lclRbCfgList = *rbCfgList;
#if (defined(VE_SRB2_SUPPORT) || defined(LTE_HO_SUPPORT))
   /* for all RABs to be cfgd. */
   for(idx = 0; idx < lclRbCfgList->numOfRbCfgInfo; idx++)
   {
      rbCfg = &(lclRbCfgList->rbCfgInfoList[idx]);

      rbCfg->rbCfgBitMsk = (NHU_RB_CFG_UL |\
               NHU_RB_CFG_DL | NHU_RB_CFG_PDCP_INFO); 
#endif
      switch(msgType)
      {
         case C3_RRCCONSETUP:
            /*  RRC Connection Setup Msg. */ 
            logChType = CM_LTE_LCH_DCCH;
            rabId = VE_PDCP_ID_SRB1;
            rbtype = CM_LTE_SRB;
            break;
         case C4_RRCCONRECFGN:
            /* RRC Reconfiguration Msg. */
            if (idx == 0)
            {
#ifdef VE_SRB2_SUPPORT
               logChType = CM_LTE_LCH_DCCH;
               rabId = VE_PDCP_ID_SRB2;
               rbtype = CM_LTE_SRB;
            }
            else
            {
               /* Fix for DRB Lcid */
               if (idx == 0)
               {
#endif
                  rabId = VE_PDCP_ID_DRB1;
                  /*  changing DRB to UM */
#ifndef VE_UM_MODE
                  rlcMode = NHU_RLC_MODE_AM;
#else
                  rlcMode = NHU_RLC_MODE_UM;
#endif
               }
               else
               {
                  rabId++;
               }
               logChType = CM_LTE_LCH_DTCH;
#ifdef VE_SUPPORT_RLC_UM_MODE
         rlcMode = NHU_RLC_MODE_UM;  
#endif
               rbtype = CM_LTE_DRB;
#ifdef VE_SRB2_SUPPORT
            }
#endif
            break;

      case C4_SECURITYMODECMD:
            /* Security Mode Command */ 
            rabId = VE_PDCP_ID_SRB1;
            rbtype = CM_LTE_SRB;
            rbCfg->rbCfgBitMsk = NHU_RB_CFG_PDCP_INFO; 
         break;
#ifdef LTE_HO_SUPPORT
      case VE_TRANSID_UE_CFG:
         {
            if (idx == 0)
            {
               logChType = CM_LTE_LCH_DCCH;
               rabId = VE_PDCP_ID_SRB1;
               rbtype = CM_LTE_SRB;
            }
#ifdef VE_SRB2_SUPPORT
            else if (idx == 1)
            {
               logChType = CM_LTE_LCH_DCCH;
               rabId = VE_PDCP_ID_SRB2;
               rbtype = CM_LTE_SRB;
            }
#endif
            else
            {
#ifdef VE_SRB2_SUPPORT
               if (idx == 2)
#else
               if (idx == 1)
#endif
               {
                  rabId = VE_PDCP_ID_DRB1;
               }
               else
               {
                  rabId++;
               }
               logChType = CM_LTE_LCH_DTCH;
#ifdef VE_SUPPORT_RLC_UM_MODE
               rlcMode = NHU_RLC_MODE_UM;
#endif
               rbtype = CM_LTE_DRB;
            }
         }
         break;
#endif/* LTE_HO_SUPPORT */

         default:
            break;
      }

#if (!defined(VE_SRB2_SUPPORT) && !defined(LTE_HO_SUPPORT))
   /* for all RABs to be cfgd. */
   for(idx1 = 0; idx1 < lclRbCfgList->numOfRbCfgInfo; idx1++)
   {

      rbCfg = &(lclRbCfgList->rbCfgInfoList[idx1]);

      /* Message Type based RB cfgbitmask is set */
      if(C4_SECURITYMODECMD == msgType)
      {
         /* only integrity & ciphering need to be activated */
         rbCfg->rbCfgBitMsk = NHU_RB_CFG_PDCP_INFO; 
      }
      else
      {
         /* shashank changing DRB to Unidirection UM mode */
#if (!defined(VE_UM_MODE) || defined(VE_UM_MODE_BI_DIR))
         rbCfg->rbCfgBitMsk = (NHU_RB_CFG_UL |\
               NHU_RB_CFG_DL | NHU_RB_CFG_PDCP_INFO); 
#else
         if (msgType == C4_RRCCONRECFGN)
         {
            rbCfg->rbCfgBitMsk = ( NHU_RB_CFG_DL | NHU_RB_CFG_PDCP_INFO); 
         }
         else
         {
            rbCfg->rbCfgBitMsk = (NHU_RB_CFG_UL |\
                  NHU_RB_CFG_DL | NHU_RB_CFG_PDCP_INFO); 
         }
#endif
      }
   }
#endif

      rbCfg->rbId.rbId = rabId;
      rbCfg->rbId.rbTyp = rbtype;

      if(rbCfg->rbCfgBitMsk & NHU_RB_CFG_UL)
      {
         /* Bit mask calculate properly */
         rbCfg->ulRbCfg.ulRbBitMsk |= 
            (NHU_RB_CFG_LOGCH_MAP_UL | NHU_RB_CFG_RLC_MODE_UL);

         /* Uplink Logical Channel Mapping Info.  */
         {
            /* Transport Channel type */
            rbCfg->ulRbCfg.ulLogChMap.trChType = CM_LTE_TRCH_UL_SCH;

            /* Logical Channel Id and Type */
            rbCfg->lcId.lcId = rabId;
            rbCfg->lcId.logChType = (CmLteLcType)logChType;

            /* Fill Logical Channel Cfg */
            veFillUlLogChCfg(&(rbCfg->ulRbCfg.ulLogChMap), rabId);

            /* ulRlcMode */
            rbCfg->ulRbCfg.ulRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            rbCfg->ulRbCfg.ulRlcMode.mode = rlcMode;  
            veFillUlRlcModeInfo(&(rbCfg->ulRbCfg.ulRlcMode), rabId);
         }
      } /* if loop - Uplink Logical Channel Mapping Info.  */


      /* DlRbCfg */
      if(rbCfg->rbCfgBitMsk & NHU_RB_CFG_DL)
      {
         rbCfg->dlRbCfg.dlRbBitMsk |=
            (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* Downlink Logical Channel Mapping Info. */
         {
            /* Logical Channel ID and type */
            rbCfg->lcId.lcId = rabId;
            rbCfg->lcId.logChType = (CmLteLcType)logChType;
            /* Transport Channel Type */
            rbCfg->dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_DL_SCH;
            /* DL-RLC mode */
            rbCfg->dlRbCfg.dlRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            rbCfg->dlRbCfg.dlRlcMode.mode = rlcMode;
            veFillDlRlcModeInfo(&(rbCfg->dlRbCfg.dlRlcMode), rabId);
         }
      } /* if loop - Downlink Logical Channel Mapping Info. */


      /* PDCP Cfg */
      if(rbCfg->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
      {
         /* Madhur : Changing the cfmReqd for SRB Channels */
         /* set pdcpId HERE */
         if (rbtype == CM_LTE_SRB)
         {
           veFillPdcpCfg(&(rbCfg->pdcpCfg), rabId, TRUE);
         }
         else
         {
           veFillPdcpCfg(&(rbCfg->pdcpCfg), rabId, FALSE);
         }
      }
/* SELVA MSPDFIX */
#if (defined(VE_SRB2_SUPPORT) || defined(LTE_HO_SUPPORT))
   } /* for loop */
#endif

   RETVALUE(ROK);
} /* veFillUeAddRbCfg */



/*
*
*       Fun:   veFillNhuUeCfgInfo
*
*       Desc:  This function fills UeCfgInfo in event structure
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillNhuUeCfgInfo
(
Ptr           evnt,
NhuUeCfgInfo  *ueCfg,
VeUeCb        *ueCb,
U8            msgType
)
#else
PUBLIC S16 veFillNhuUeCfgInfo(evnt, ueCfg, ueCb, msgType)
Ptr           evnt;
NhuUeCfgInfo  *ueCfg;
VeUeCb        *ueCb;
U8            msgType;
#endif
{
   NhuRbCfgList  *rbCfgList = NULLP;
   U8            expCfgBitMask = 0;
   U8            noRAB = 0;
   U8            noSRB = 0;

   TRC2(veFillNhuUeCfgInfo);

   if(NULLP == ueCb || NULLP == ueCfg)
   {
#ifdef DEBUGP
     VE_DBG_ERROR((VE_PRNT_BUF," ueCb and ueCfg is NULL.\n")); 
#endif
      RETVALUE(RFAILED);
   }

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillNhuUeCfgInfo:\
            Construct UE Cfg Related Info\n"));
#endif

   switch(msgType)
   {
      case C3_RRCCONSETUP:
         /*  RRC Connection Setup Msg. */ 
         expCfgBitMask = (NHU_EXP_CFG_RB_CFG |\
               NHU_EXP_CFG_TRCH_CFG | NHU_EXP_CFG_UE_CFG);
         /* number of RABs to be cfgd. */
#ifdef VE_SRB2_SUPPORT
         noSRB = VE_VAL_ONE;
#else
         noRAB = VE_VAL_ONE;
#endif
         break;

      case C4_RRCCONRECFGN:
         /* RRC Reconfiguration Msg. */
         expCfgBitMask = (NHU_EXP_CFG_RB_CFG | NHU_EXP_CFG_TRCH_CFG);
         /* number of RABs to be cfgd. */
#ifdef VE_SRB2_SUPPORT
         noSRB = VE_VAL_ONE;
#ifdef YS_PICO
         SEND_DBG_MSG("Performing SRB2 Configuration \n");
#endif
#endif
         noRAB = ueCb->rbInfo.numOfRbCfgd;
         break;

      case C4_SECURITYMODECMD:
         /* Security Mode Command */ 
         expCfgBitMask = NHU_EXP_CFG_SEC_CFG;
         /* number of RABs to be cfgd. */
#ifdef VE_SRB2_SUPPORT
         noSRB = VE_VAL_ONE;
#else
         noRAB = VE_VAL_ONE;
#endif
         break;
#ifdef LTE_HO_SUPPORT
      case VE_TRANSID_UE_CFG:
         /* Initialize and Update the cause value as NHU_CONN_RECFG_AT_HNDOVR */
          ueCfg->conReCfgCause = NHU_CONN_RECFG_AT_HNDOVR;
         /* UE Cfg Msg during HO after the Admission success. */
         expCfgBitMask = (NHU_EXP_CFG_RB_CFG | NHU_EXP_CFG_TRCH_CFG | 
            NHU_EXP_CFG_UE_CFG);
         /* number of RABs to be cfgd. */
#ifdef VE_SRB2_SUPPORT
         noSRB = VE_VAL_TWO;
#ifdef YS_PICO
         SEND_DBG_MSG("Performing SRB2 Configuration \n");
#endif
#else
         noSRB = VE_VAL_ONE;
#endif
         noRAB = ueCb->rbInfo.numOfRbCfgd;
         break;
      
#endif/* End of LTE_HO_SUPPORT */
        /* RRC ReEst */
        case C3_RRCCONREESTB:
         expCfgBitMask = (NHU_EXP_CFG_UE_CFG | NHU_EXP_CFG_TRCH_CFG|NHU_EXP_CFG_SEC_CFG);
         ueCfg->conReCfgCause = NHU_CONN_RESTB_EVNT;
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
      {
         rbCfgList = &(ueCfg->rbCfgAllList.rbToAddCfgList);

         if(NULLP == rbCfgList)
         {
            RETVALUE(RFAILED);
         }
#ifdef VE_SRB2_SUPPORT
#ifdef YS_PICO
         SEND_DBG_MSG("SRB2 Cfg %s : %d\n", __FILE__, __LINE__);
#endif
         rbCfgList->numOfRbCfgInfo = noRAB + noSRB;
         VE_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * (noRAB + noSRB)),\
               &(rbCfgList->rbCfgInfoList));
#else
#ifdef LTE_HO_SUPPORT
         rbCfgList->numOfRbCfgInfo = noRAB + noSRB;
         VE_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * (noRAB + noSRB)),\
               &(rbCfgList->rbCfgInfoList));
#else
         rbCfgList->numOfRbCfgInfo = noRAB;
         VE_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * noRAB),\
               &(rbCfgList->rbCfgInfoList));
#endif/* LTE_HO_SUPPORT */
#endif
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
      veFillSecCfg(&(ueCfg->secCfg), ueCb);
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
*       Fun:   veFillPdcpCfg
*
*       Desc:  This function fills PDCP Configuration 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillPdcpCfg
(
NhuPdcpCfg    *pdcpCfg,
U8            rbId,
U8            cfmReqd
)
#else
PUBLIC S16 veFillPdcpCfg(pdcpCfg, rbId, cfmReqd)
NhuPdcpCfg    *pdcpCfg;
U8            rbId;
U8            cfmReqd;
#endif
{
   TRC2(veFillPdcpCfg);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillPdcpCfg: \
            Fill PDCP Cfg \n"));
#endif

   pdcpCfg->pdcpCfgBitMsk = (NHU_PDCP_DISCARD_TMR | NHU_PDCP_STATUS_RPT | NHU_PDCP_CFM_REQD);

   if(pdcpCfg->pdcpCfgBitMsk & NHU_PDCP_DISCARD_TMR)
   {
      /* veDfltDiscardTimer */
      pdcpCfg->discardTmr = NHU_PDCP_DISCARD_TMR_MSINF;
   }

   if(pdcpCfg->pdcpCfgBitMsk & NHU_PDCP_STATUS_RPT)
   {
      /* veDfltStatusReportReqd */
      pdcpCfg->statusRptReq = TRUE;
   }

#ifdef VE_PERF_MEAS
   pdcpCfg->discardTmr = (NhuPdcpDiscardTmr)0;/* g++ compilation */
   pdcpCfg->statusRptReq = FALSE;
#endif
/* MSPD changes */
#ifdef MSPD
   if (rbId == 3)
   {
      pdcpCfg->snSize = NHU_PDCP_SN_SIZE_7BITS;
      pdcpCfg->pdcpCfgBitMsk |=  NHU_PDCP_SN_SIZE;
      pdcpCfg->statusRptReq = FALSE;
      printf ("\n veFillPdcpCfg filling in DRB configuration \n");
   }
#endif
   /* Madhur : Updating cfmreqd for SRB channels */
   pdcpCfg->cfmReqd = cfmReqd;

   /* Currently discard timer disabled */
   pdcpCfg->discardTmr = (NhuPdcpDiscardTmr)0;/* g++ compilation */

   RETVALUE(ROK);
} /* End of veFillPdcpCfg */

/*
*
*       Fun:   veFillTrChCfg
*
*       Desc:  This function fills Transport Channel Configuration
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillTrChCfg
(
NhuTrChCfg    *trChCfg
)
#else
PUBLIC S16 veFillTrChCfg(trChCfg)
NhuTrChCfg    *trChCfg;
#endif
{
   U8 trChCfgType;

   TRC2(veFillTrChCfg);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillTrChCfg: \
            Fill Transport Channel Cfg. \n"));
#endif

   trChCfgType = NHU_CFG_TYPE_EXPLICIT;

   trChCfg->trChCfgType = trChCfgType;
   if(trChCfg->trChCfgType == NHU_CFG_TYPE_EXPLICIT)
   {
      trChCfg->trChBitMsk = (NHU_TRCH_CFG_UL_HARQ);
      trChCfg->ulHarqCfg.ulMaxHarqTx = (NhuMaxHarqTx)2;/* g++ compilation, MAdhur : changing for reduced harq failures */
      trChCfg->ulHarqCfg.deltaHarqOffset = 0;
   } 
   RETVALUE(ROK);
} /* End of veFillTrChCfg */

/*
*
*       Fun:   veFillSecCfg
*
*       Desc:  This function fills Security Configuration
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillSecCfg
(
NhuSecurityCfg     *secCfg,
VeUeCb             *ueCb
)
#else
PUBLIC S16 veFillSecCfg(secCfg, ueCb)
NhuSecurityCfg     *secCfg;
VeUeCb             *ueCb;
#endif
{
   U8 idx = 0;

   TRC2(veFillSecCfg);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillSecCfg:\
            Fill Security Related Cfg Info \n"));
#endif

   secCfg->secCfgBitMsk = (NHU_SEC_CIPHER_ALGO | NHU_SEC_INTG_ALGO);
   /* RRC ReEst */
   if(ueCb->rrcConnState == VE_SRB2_DRB_SUSPENDED)
   {
    secCfg->intgCfgInfo.action = NHU_ACTION_MODIFY;
   }
  else
  {
    secCfg->intgCfgInfo.action = NHU_ACTION_ADD;
  }
   /* Intg Cfg */
   switch(ueCb->secCfg.intgAlgo)
   {
      case 1:
         secCfg->intgCfgInfo.intgAlgo = NHU_INTG_ALGO_EIA1;
         break;

      case 2:
         secCfg->intgCfgInfo.intgAlgo = NHU_INTG_ALGO_EIA2;
         break;

   }
   for(idx = 0; idx < VE_INTG_KEY_LEN; idx++)
   {
      secCfg->intgCfgInfo.intgKey[idx] = ueCb->secCfg.intgKey[idx];
   }
   /* Ciph Cfg */
   switch(ueCb->secCfg.ciphAlgo)
   {
      case 0:
         secCfg->cipherCfgInfo.ciphAlgo = NHU_CIPHER_ALGO_EEA0;
         break;

      case 1:
         secCfg->cipherCfgInfo.ciphAlgo = NHU_CIPHER_ALGO_EEA1;
         break;

      case 2:
         secCfg->cipherCfgInfo.ciphAlgo = NHU_CIPHER_ALGO_EEA2;
         break;

   }

   for(idx = 0; idx < VE_CIPH_KEY_LEN; idx++)
   {
      secCfg->cipherCfgInfo.cipherCpKey[idx] = ueCb->secCfg.cpCiphKey[idx];
      secCfg->cipherCfgInfo.cipherUpKey[idx] = ueCb->secCfg.upCiphKey[idx];
   }

   RETVALUE(ROK);
} /* End of veFillSecCfg */



/*
*
*       Fun:   veFillUlLogChCfg
*
*       Desc:  This function fills Uplink Logical Channel Configuration
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillUlLogChCfg
(
NhuUlLogChMap *ulLogChMap,
U8            rbId
)
#else
PUBLIC S16 veFillUlLogChCfg(ulLogChMap, rbId)
NhuUlLogChMap *ulLogChMap;
U8            rbId;
#endif
{
   U16 priority = 0;
   U16 pbr = 0;
   U16 grp = 0;

   TRC2(veFillUlLogChCfg);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillUlLogChCfg:\
            Fill Uplink Logical Channel Cfg. \n"));
#endif

   ulLogChMap->lcCfg.logChCfgpres = PRSNT_NODEF;
   ulLogChMap->lcCfgType = NHU_CFG_TYPE_EXPLICIT;
   switch(rbId)
   {
      case VE_PDCP_ID_SRB1:
         priority = veDfltSrb1LogchcfgPrior;
         pbr = veDfltSrbLogchcfgPriBitRate;
         grp = veDfltSrbLogchcfgGrp;
         break;

      case VE_PDCP_ID_SRB2:
         priority = veDfltSrb2LogchcfgPrior;
         pbr = veDfltSrbLogchcfgPriBitRate;
         grp = veDfltSrbLogchcfgGrp;
         break;

      case VE_PDCP_ID_DRB1:
      default:
         priority = veDfltDrbPriority;
         pbr = veDfltDrbPriBitRate;
         grp = veDfltDrbGrp;
         break;
   }

   if(ulLogChMap->lcCfg.logChCfgpres == PRSNT_NODEF &&
         ulLogChMap->lcCfgType == NHU_CFG_TYPE_EXPLICIT)
   {
      ulLogChMap->lcCfg.logChGrp =(U8)grp;/* klock warning resolved */
   }

   RETVALUE(ROK);
} /* End of veFillUlLogChCfg */

/*
*
*       Fun:   veFillUlRlcModeInfo
*
*       Desc:  This function fills Uplink RLC Mode information
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillUlRlcModeInfo
(
NhuUlRlcMode  *ulRlcMode,
U8            rabId
)
#else
PUBLIC S16 veFillUlRlcModeInfo(ulRlcMode, rabId)
NhuUlRlcMode  *ulRlcMode;
U8            rabId;
#endif
{
   TRC2(veFillUlRlcModeInfo);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillUlRlcModeInfo:\
            Fill Uplink RLC Mode Info\n"));
#endif

   if(ulRlcMode->mode == NHU_RLC_MODE_AM)
   {
      if((rabId == VE_PDCP_ID_SRB1) || (rabId == VE_PDCP_ID_SRB2))
      {
         /* Using default values specified in 36.331 */
#ifdef VE_DONGWON_OAM
         /*OAM_FIX : Replaced the hardcoded values with the default variables*/
        ulRlcMode->m.amRlc.tStatusProh = enumMapNhuTStatusProh[veDfltSrbTStatProhibit];
        ulRlcMode->m.amRlc.tReordring = enumMapNhuTReOrdering[veDfltSrbTreordrng];
#else
         ulRlcMode->m.amRlc.tStatusProh = NHU_STATUS_PROH_MS35;
         ulRlcMode->m.amRlc.tReordring =  NHU_TREORD_MS0;
#endif
      }
      else
      {
#ifdef VE_DONGWON_OAM
         /*OAM_FIX : Replaced the hardcoded values with the default variables*/
         ulRlcMode->m.amRlc.tStatusProh = enumMapNhuTStatusProh[veDfltDrbTStatProh];
         ulRlcMode->m.amRlc.tReordring = enumMapNhuTReOrdering[veDfltDrbTReordrng];
#else
         ulRlcMode->m.amRlc.tStatusProh = NHU_STATUS_PROH_MS60;
         ulRlcMode->m.amRlc.tReordring = NHU_TREORD_MS40;
#endif
      }
   }
   else if (ulRlcMode->mode == NHU_RLC_MODE_UM)
   {
      ulRlcMode->m.umRlc.snFieldLen = VE_RLC_UM_UL_DFLT_SN_LEN;
      /* TIC_ID:ccpu00116701: MOD: Fixed error for g++ compilation on Linux */
      ulRlcMode->m.umRlc.tReordring = (NhuTReOrdering)veDfltDrbTReordrng;
      /* TIC_ID:ccpu00116701: MOD_END: Fixed error for g++ compilation on Linux */
   }

   RETVALUE(ROK);
} /* End of veFillUlRlcModeInfo */



/*
*
*       Fun:   veFillDlRlcModeInfo
*
*       Desc:  This function fills Downlink RLC Mode information
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillDlRlcModeInfo
(
NhuDlRlcMode  *dlRlcMode,
U8            rabId
)
#else
PUBLIC S16 veFillDlRlcModeInfo(dlRlcMode, rabId)
NhuDlRlcMode  *dlRlcMode;
U8            rabId;
#endif
{
   TRC2(veFillDlRlcModeInfo);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillDlRlcModeInfo:\
            Fill Downlink RLC Mode Info. \n"));
#endif

   if(dlRlcMode->mode == NHU_RLC_MODE_AM)
   {
      if((rabId == VE_PDCP_ID_SRB1) || (rabId == VE_PDCP_ID_SRB2))
      {
         /* Using default values specified in 36.331 */
#ifdef VE_DONGWON_OAM
         /*OAM_FIX:Replaced the hardcoded values with the default variables*/
         dlRlcMode->m.amRlc.maxRetxThresh = enumMapNhuMaxRetxThres[veDfltSrbMaxReTxThresh];
         dlRlcMode->m.amRlc.pollByte = enumMapNhuRlcPollByte[veDfltSrbPollByte];
         dlRlcMode->m.amRlc.pollPdu = enumMapNhuPollPdu[veDfltSrbPollPdu];
         dlRlcMode->m.amRlc.tPollRetx =enumMapNhuTPollReTx[veDfltSrbPollRetrans]; /*NHU_TPOLL_RETX_MS45; */
#else
         dlRlcMode->m.amRlc.maxRetxThresh = NHU_MAX_RETX_THRES_T4;
         dlRlcMode->m.amRlc.pollByte = NHU_POLL_BYTE_INF;
         dlRlcMode->m.amRlc.pollPdu = NHU_POLL_PDU_INF;
         dlRlcMode->m.amRlc.tPollRetx = NHU_TPOLL_RETX_MS400; /*NHU_TPOLL_RETX_MS45; */
#endif
      }
      else
      {
         dlRlcMode->m.amRlc.maxRetxThresh = NHU_MAX_RETX_THRES_T4;
         /* Using the less polling byte and pdu for performance on Uu */ 
#ifdef LTE_ENB_PERF
         dlRlcMode->m.amRlc.pollByte = VE_POLL_BYTE_KB8000;
         dlRlcMode->m.amRlc.pollPdu = NHU_POLL_PDU_P8;
#else
         dlRlcMode->m.amRlc.pollByte = NHU_POLL_BYTE_KB125;
         dlRlcMode->m.amRlc.pollPdu = NHU_POLL_PDU_P4;
#endif
         dlRlcMode->m.amRlc.tPollRetx = NHU_TPOLL_RETX_MS80;
      }
   }
   else if(dlRlcMode->mode == NHU_RLC_MODE_UM)
   {
#ifdef VE_DONGWON_OAM
      /*OAM_FIX : Replaced the hardcoded values with the default variables*/
           if(veDfltSnFieldLen == NhuSN_FieldLengthsize5Enum)
           {
                   dlRlcMode->m.umRlc.snFieldLen = VE_RLC_UM_UL_DFLT_SN_LEN;
           }
           else
           {
                   dlRlcMode->m.umRlc.snFieldLen = VE_RLC_UM_DL_DFLT_SN_LEN;
           }
#else
      dlRlcMode->m.umRlc.snFieldLen = VE_RLC_UM_DL_DFLT_SN_LEN;
#endif
   }

   RETVALUE(ROK);
} /* End of veFillDlRlcModeInfo */


/*
*
*       Fun:   veFillNhuEvntHdr
*
*       Desc:  This function fills NHU Event Header 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillNhuEvntHdr
(
NhuHdr      *nhuHdr,
U32         cellId,
U16         crnti,
U8          msgType
)
#else
PUBLIC S16 veFillNhuEvntHdr(nhuHdr, cellId, crnti, msgType)
NhuHdr      *nhuHdr;
U32         cellId;
U16         crnti;
U8          msgType;
#endif
{

   TRC2(veFillNhuEvntHdr);

#ifdef DEBUGP   
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillNhuEvntHdr:\
            Populate the NHU header info \n"));
#endif

   if(NULLP == nhuHdr)
   {
      RETVALUE(RFAILED);
   }
   nhuHdr->cellId = (U16)cellId;
   nhuHdr->ueId = crnti;
   nhuHdr->transId = msgType;

   RETVALUE(ROK);
} /* End of veFillNhuEvntHdr */

#ifdef WIRESHARK_LOG

#ifdef ANSI
PUBLIC S16 veInitWireShark
(
)
#else
PUBLIC S16 veInitWireShark()
#endif
{
   g_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

   if (g_sockfd < 0)
   {
      printf("Failed to create socket\n");
      return (-1);
   }

   g_sockfd1 = socket(AF_INET, SOCK_DGRAM, 0);

   if (g_sockfd < 0)
   {
      printf("Failed to create socket\n");
      return (-1);
   }

   g_serv_addr.sin_family = AF_INET;
   g_serv_addr.sin_port   = htons(9999);
   g_serv_addr.sin_addr.s_addr = inet_addr("172.17.8.20");

   bind(g_sockfd, &g_serv_addr, sizeof(g_serv_addr));

   g_serv_addr1.sin_family = AF_INET;
   g_serv_addr1.sin_port   = htons(9999);
   g_serv_addr1.sin_addr.s_addr = inet_addr("172.17.1.214");

   bind(g_sockfd, &g_serv_addr1, sizeof(g_serv_addr1));

   printf("Bind successful\n");

}



#ifdef ANSI
PUBLIC S16 veHandleSendMsg
(
U8    *str
)
#else
PUBLIC S16 veHandleSendMsg(str)
U8    *str;
#endif
{

   sendto(g_sockfd1, str, 512, 0, &g_serv_addr1, sizeof(g_serv_addr1));

}
#ifdef YS_PICO
void SEND_DBG_MSG1(U8 *_str, ...)
{
   int i;
}

void SEND_DBG_MSG(U8 *_str, ...)
{
   int i;
}



#ifdef ANSI
PUBLIC S16 veHandleWireSharkMsg
(
Buffer     *mBuf
)
#else
PUBLIC S16 veHandleWireSharkMsg(mBuf)
Buffer     *mBuf;
#endif
{
   MsgLen len;
   U8     statBuffer[5000];
   MsgLen cLen;
   U32     bufOffset=0;
   U16     tmp16;
   U32     ret;

   /* mBuf Contents */
   U8      radioType=1;
   U8      direction;
   U8      rntiType;
   U16     rnti;
   U16     ueId;
   U16     subframe;
   U8      isPredefinedData=0;
   U8      retx=0;
   U8      crcStatus=1;

   /* Added trace macro */
   TRC2(veHandleWireSharkMsg)

   SFndLenMsg(mBuf, &len);
   if (len==0)
   {
      SPutMsg(mBuf);
      /* Added RETVALUE */
      RETVALUE(ROK);
   }


   SUnpkS16(&subframe, mBuf);
   SUnpkS16(&ueId, mBuf);
   SUnpkS16(&rnti, mBuf);
   SUnpkU8(&rntiType, mBuf);
   SUnpkU8(&direction, mBuf);

   memset(statBuffer, 0, sizeof(statBuffer));
   /* Start Buffer Tag for logging */
   memcpy(statBuffer+bufOffset, VE_MAC_LTE_START_STRING,
         strlen(VE_MAC_LTE_START_STRING));
   bufOffset += strlen(VE_MAC_LTE_START_STRING);

   /* MAC Packet Info fileds */
   statBuffer[bufOffset++]    = radioType;
   statBuffer[bufOffset++]    = direction;
   statBuffer[bufOffset++]    = rntiType;

   if (ueId == 65535)
      statBuffer[bufOffset-1] = 4;
   if (ueId < 5)
      statBuffer[bufOffset-1] = 2;

   /* RNTI */
   statBuffer[bufOffset++] = VE_MAC_LTE_RNTI_TAG;
   tmp16 = htons(rnti);
   memcpy(statBuffer+bufOffset, &tmp16, 2);
   bufOffset+= 2;

   /* UEId */
   statBuffer[bufOffset++] = VE_MAC_LTE_UEID_TAG;
   tmp16 = htons(ueId);
   memcpy(statBuffer+bufOffset, &tmp16, 2);
   bufOffset+= 2;

   /* Subframe number */
   statBuffer[bufOffset++] = VE_MAC_LTE_SUBFRAME_TAG;
   tmp16 = htons(subframe);
   memcpy(statBuffer+bufOffset, &tmp16, 2);
   bufOffset+= 2;

   statBuffer[bufOffset++] = VE_MAC_LTE_CRC_STATUS_TAG;
   statBuffer[bufOffset++] = crcStatus;

   /********/
   if (!isPredefinedData) {
      statBuffer[bufOffset++] = VE_MAC_LTE_PREDFINED_DATA_TAG;
      statBuffer[bufOffset++] = isPredefinedData;
   }

   if (retx != 0) {
      statBuffer[bufOffset++] = VE_MAC_LTE_RETX_TAG;
      statBuffer[bufOffset++] = retx;
   }

   /* Actual Payload */
   statBuffer[bufOffset++] = VE_MAC_LTE_PAYLOAD_TAG;

   if (mBuf != NULLP)
   {
      SCpyMsgFix(mBuf, (MsgLen)0, (MsgLen)len,
            (Data*) (statBuffer+bufOffset), (MsgLen*)&cLen);
   }

   bufOffset += len;

   ret = sendto(g_sockfd, statBuffer, bufOffset, 0, &g_serv_addr, sizeof(g_serv_addr));

   SPutMsg(mBuf);

   /* Added RETVALUE */
   RETVALUE(ROK);
}
#endif
#endif

/* RRC-ReEstab */
/*
*
*       Fun:   veFillRRC_ConReest
*
*       Desc:  This function fills RRC Connection Reestablishment Msg
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRC_ConReest
(
NhuDatRspSdus *nhuDatRspSdu,
VeUeCb *ueCb 
)
#else
PUBLIC S16 veFillRRC_ConReest(nhuDatRspSdu, ueCb)
NhuDatRspSdus *nhuDatRspSdu;
VeUeCb *ueCb;
#endif
{
   NhuDL_CCCH_Msg *dlMsg = NULLP;
   NhuRRCConReestb *conReestb = NULLP;
   NhuRRCConReestb_r8_IEs *reestabIe = NULLP;
   NhuRadioResourceConfigDedicated *resCfgDed = NULLP;
   NhuUeCfgInfo  *ueCfg;
   S16 ret = ROK;
   TRC2(veFillRRC_ConReest);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRC_ConSetup: Building \
            RRC Re-establishment Msg.\n"));
#endif

   if(NULLP == nhuDatRspSdu)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRRC_ConReest: nhuDatRspSdu is NULL."));

      RETVALUE(RFAILED);
   }

   nhuDatRspSdu->sdu.msgCategory = NHU_MSG_DLCCCH;

   dlMsg = &(nhuDatRspSdu->sdu.m.dlCcchMsg.ccchMsg);

   VE_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(dlMsg->message.choice, DL_CCCH_MSGTYP_C3);
   VE_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C3_RRCCONREESTB);

   VE_FILL_TKN_UINT(nhuDatRspSdu->sdu.m.dlCcchMsg.ccchMsg.message.val.c1.choice,
                                   0);

   conReestb = &(dlMsg->message.val.c1.val.rrcConReestb);

   VE_FILL_TKN_UINT(conReestb->pres,PRSNT_NODEF);
   VE_FILL_TKN_UINT(conReestb->rrc_TrnsxnId,VE_VAL_ONE); 
   
    VE_FILL_TKN_UINT(conReestb->criticalExtns.choice, CRITICALEXTNS_C13);
       
    if(conReestb->criticalExtns.choice.val == CRITICALEXTNS_C13)
    {
       VE_FILL_TKN_UINT(conReestb->criticalExtns.val.c1.choice,
                                                C13_RRCCONREESTB_R8);
       reestabIe = &conReestb->criticalExtns.val.c1.val.rrcConReestb_r8;
       VE_FILL_TKN_UINT(reestabIe->pres, PRSNT_NODEF);
       VE_FILL_TKN_UINT(reestabIe->radioResourceConfigDedicated.pres,PRSNT_NODEF);
      /* SRB Add for now only SRB1 */
       resCfgDed = &(reestabIe->radioResourceConfigDedicated);
       veFillNhuSRBAddModLst(resCfgDed, (Ptr)nhuDatRspSdu, VE_PDCP_ID_SRB1);
       VE_FILL_TKN_UINT(reestabIe->nextHopChainingCount, 1); 
       VE_FILL_TKN_UINT(reestabIe->nonCriticalExtn.pres, NOTPRSNT);
     
        /*  FIXME *//* Phy. Dedicated Cfg */
       veFillRadResPhyCfgDed(resCfgDed,(Ptr)nhuDatRspSdu, VE_PDCP_ID_SRB1, ueCb);
   }

      /* Fill Config values */
   nhuDatRspSdu->sdu.isUeCfgPres = TRUE;
   /*- Fill the UE configuration information --*/
   ueCfg = &( nhuDatRspSdu->sdu.ueCfg);
   ret = veFillNhuUeCfgInfo(nhuDatRspSdu, ueCfg, ueCb, C3_RRCCONREESTB);
   if (ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veSndRrcConReqSetup: veFillNhuUeCfgInfo Failed \n"));
#endif
      VE_FREEEVNT(nhuDatRspSdu);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* End of veFillRRC_ConReest*/
/*TODO:shishir*/
/*
*
*       Fun:   veFillRRC_ConReestReject
*
*       Desc:  This function fills RRC Connection Reestablishment Reject Msg
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_bldmsg.c
*
*/
#ifdef ANSI
PUBLIC S16 veFillRRC_ConReestRej
(
NhuDatRspSdus *nhuDatRspSdu,
VeUeCb *ueCb
)
#else
PUBLIC S16 veFillRRC_ConReestRej(nhuDatRspSdu, ueCb)
NhuDatRspSdus *nhuDatRspSdu;
VeUeCb *ueCb;
#endif
{
   NhuDL_CCCH_Msg *dlMsg = NULLP;
   NhuRRCConReestbRej *conReestbRej = NULLP;
   NhuRRCConReestbRej_r8_IEs *reestabrejIe = NULLP;
   NhuUeCfgInfo  *ueCfg;
   S16 ret = ROK;
   TRC2(veFillRRC_ConReestRej);

#ifdef DEBUGP
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veFillRRC_ConSetup: Building \
            RRC Re-establishment Reject Msg.\n"));
#endif

   if(NULLP == nhuDatRspSdu)
   {
      VE_DBG_ERROR((VE_PRNT_BUF,"veFillRRC_ConReestReject: nhuDatRspSdu is NULL."));

      RETVALUE(RFAILED);
   }

   nhuDatRspSdu->sdu.msgCategory = NHU_MSG_DLCCCH;

   dlMsg = &(nhuDatRspSdu->sdu.m.dlCcchMsg.ccchMsg);

   VE_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(dlMsg->message.choice, DL_CCCH_MSGTYP_C3);
   VE_FILL_TKN_UINT(dlMsg->message.val.c1.choice,C3_RRCCONREESTBREJ);

   VE_FILL_TKN_UINT(nhuDatRspSdu->sdu.m.dlCcchMsg.ccchMsg.message.val.c1.choice,
                                   1);

   conReestbRej = &(dlMsg->message.val.c1.val.rrcConReestbRej);

   VE_FILL_TKN_UINT(conReestbRej->pres,PRSNT_NODEF);

    VE_FILL_TKN_UINT(conReestbRej->criticalExtns.choice,CRITICALEXTNS_RRCCONREESTBREJ_R8);
       
    if(conReestbRej->criticalExtns.choice.val == CRITICALEXTNS_RRCCONREESTBREJ_R8)
    {
       reestabrejIe = &conReestbRej->criticalExtns.val.rrcConReestbRej_r8;
       VE_FILL_TKN_UINT(reestabrejIe->pres, PRSNT_NODEF);
       VE_FILL_TKN_UINT(reestabrejIe->nonCriticalExtn.pres,NOTPRSNT);
    /*   VE_FILL_TKN_UINT(reestabrejIe->nonCriticalExtn.lateNonCriticalExtn);
       VE_FILL_TKN_UINT(restabrejId->nonCriticalExtn.nonCriticalExtn.pres,PRSNT_NODEF);
    */
   }

      /* Fill Config values */
   nhuDatRspSdu->sdu.isUeCfgPres = FALSE;
   /*- Fill the UE configuration information --*/
   ueCfg = &( nhuDatRspSdu->sdu.ueCfg);
   if(ueCb != NULLP)
   {    
      ret = veFillNhuUeCfgInfo(nhuDatRspSdu, ueCfg, ueCb,C3_RRCCONREESTBREJ);
      if (ret != ROK)
      {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veFillRCC_ConReestRej: veFillNhuUeCfgInfo Failed \n"));
#endif
         VE_FREEEVNT(nhuDatRspSdu);
         RETVALUE(RFAILED);
      }
   }
   /*RRC ReEst receieved E-RAB RLS cmmd from MME and re-establishment cause is reconfiguration failure*/
   if(ueCb != NULLP && ueCb->reEstCause == ERAB_REL_RECON_FAIL)
   {
    VeSndRelReq(NULLP,ueCb->crnti);
   }
 
   RETVALUE(ROK);
} /* End of veFillRRC_ConReestRej*/

#ifdef ANSI
PUBLIC S16 veUtlSztFillERABFailRspFromERABSetReqPdu
(
VeS1ConCb *s1apCon,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillERABFailRspFromERABSetReqPdu(s1apCon, pdu)
VeS1ConCb *s1apCon;
S1apPdu      **pdu;
#endif
{
   S1apPdu *szERABRspPdu = NULLP;
   SztE_RABSetupRqst  *eRABSetupReq = NULLP;
   SztProtIE_Field_E_RABSetupRqstIEs *member = NULLP;
   S1apPdu  *szERABReqPdu = NULLP;
   SztProtIE_Field_E_RABSetupRespIEs *ie = NULLP;
   SztProtIE_SingleCont_E_RABItemIEs *ie1 = NULLP;
   SztProtIE_SingleCont_E_RABToBeSetupItemBrSUReqIEs *protIe = NULLP;
   SztE_RABToBeSetupItemBrSUReq *itemIe = NULLP;
   VeUeCb *ueCb = NULLP;
   U16 numOfComps = 0;
   U8  rbIdx;
   U8  memberIdx;
   U8  noERABIds = 0;
   U8  idx;
  TRC2(veUtlSztFillERABFailRspFromERABSetReqPdu); 

  szERABRspPdu = *pdu;
  ueCb = s1apCon->ueCb;
  szERABReqPdu = s1apCon->ueCb->pdu;
  
  eRABSetupReq = (SztE_RABSetupRqst *) &(szERABReqPdu->pdu.val.initiatingMsg.\
         value.u.sztE_RABSetupRqst);

   numOfComps = eRABSetupReq->protocolIEs.noComp.val;

   /* ERAB Setup List  */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABSetupResp.protocolIEs.member[2];

 for(memberIdx =0;memberIdx < numOfComps;memberIdx++)
 {
   member =&(eRABSetupReq->protocolIEs.member[memberIdx]);
   if(member->id.val == Sztid_E_RABToBeSetupLstBrSUReq)
   {

   noERABIds = (U8)member->value.u.sztE_RABToBeSetupLstBrSUReq.noComp.val;
   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_E_RABFailedToSetupLstBrSURes);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU16(&(ie->value.u.sztE_RABLst.noComp), noERABIds);


    if (cmGetMem(szERABRspPdu,
            (noERABIds * sizeof(SztProtIE_SingleCont_E_RABItemIEs)),
            (Ptr *)&(ie->value.u.sztE_RABLst.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF," ERAB Setup Rsp\
               cmGetMem failed.\n"));
#endif
      RETVALUE(RFAILED);
    }

  
     for(idx =0;idx<noERABIds;idx++)
     {
       ie1 = &(ie->value.u.sztE_RABLst.member[idx]);
       protIe = &(member->value.u.sztE_RABToBeSetupLstBrSUReq.member[idx]);
       itemIe = &(protIe->value.u.sztE_RABToBeSetupItemBrSUReq);
       veFillTknU8(&(ie1->pres),PRSNT_NODEF);
       veFillTknU32(&(ie1->id),Sztid_E_RABItem);
       veFillTknU32(&(ie1->criticality),SztCriticalityignoreEnum);
       cmMemcpy((U8*)&(ie1->value.u.sztE_RABItem.e_RAB_ID),(U8*)&(itemIe->e_RAB_ID),sizeof(SztE_RAB_ID));
       veFillTknU8(&ie1->value.u.sztE_RABItem.cause.choice,0);
       veFillTknU32(&(ie1->value.u.sztE_RABItem.cause.val.radioNw),\
               SztCauseRadioNwfailure_in_radio_interface_procedureEnum);

       for(rbIdx =0 ;rbIdx < VE_MAX_RABS_IN_UE;rbIdx++)
       {

          if(itemIe->e_RAB_ID.val == ueCb->rbInfo.rab[rbIdx].eRABId)
          {
             /* RRC ReEst */
             veSndRgrUeRls(ueCb,&ueCb->rbInfo.rab[rbIdx],RGR_LCH_CFG);
             ueCb->rbInfo.rab[rbIdx].inUse = FALSE;
             ueCb->rbInfo.numOfRbCfgd--;
             VE_FREE(ueCb->rbInfo.rab[rbIdx].remEgtpTunId.val,(itemIe->gTP_TEID.len));
          }
       }    

     ie1->value.u.sztE_RABItem.iE_Extns.noComp.pres = NOTPRSNT;  
     }


     }
  }

 RETVALUE(ROK);

}



#ifdef ANSI
PUBLIC S16 veUtlSztFillERABRlsFailRspFromERABRlsCmmdPdu
(
VeS1ConCb *s1apCon,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillERABRlsFailRspFromERABRlsCmmdPdu(s1apCon, pdu)
VeS1ConCb *s1apCon;
S1apPdu      **pdu;
#endif
{

S1apPdu *szERABRlsRspPdu;
SztE_RABRlsCmmd *eRABRlsCom = NULLP;
SztProtIE_Field_E_RABRlsCmmdIEs *member;
SztProtIE_SingleCont_E_RABItemIEs *protIe= NULLP;
SztProtIE_Field_E_RABRlsRespIEs *ie=NULLP;
/*SztProtIE_SingleCont_E_RABRlsItemBrRelCompIEs *ie1=NULLP;*/
/*SztProtIE_Cont_E_RABRlsRespIEs     *ie1=NULLP;*/
SztProtIE_SingleCont_E_RABItemIEs  *ie1 = NULLP;
S1apPdu *szERABRlsCmmdPdu = NULLP;
SztE_RABItem *itemIe = NULLP;
VeUeCb *ueCb = NULLP;
U8 memberIdx;
U8 noComp = 0;
U8 idx;
U8 numOfComps =0;
U8 rbIdx;
TRC2(veUtlSztFillERABRlsFailRspFromERABRlsReqPdu);

 szERABRlsRspPdu = *pdu;
 szERABRlsCmmdPdu = s1apCon->ueCb->pdu;
 eRABRlsCom = (SztE_RABRlsCmmd *)&(szERABRlsCmmdPdu->pdu.val.initiatingMsg.\
value.u.sztE_RABRlsCmmd);

 ueCb = s1apCon->ueCb;
  ie = &szERABRlsRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABRlsResp.protocolIEs.member[2];

   veFillTknU8(&(ie->pres), PRSNT_NODEF);
   veFillTknU32(&(ie->id), Sztid_E_RABFailedToSetupLstBrSURes);
   veFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veFillTknU16(&(ie->value.u.sztE_RABLst.noComp), 1);


if (cmGetMem(szERABRlsRspPdu,
            (noComp * sizeof(SztProtIE_Cont_E_RABRlsRespIEs)),
            (Ptr *)&(ie->value.u.sztE_RABLst.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF," ERAB Setup Rsp\
               cmGetMem failed.\n"));
#endif
      RETVALUE(RFAILED);
   }


 numOfComps=(U8)eRABRlsCom->protocolIEs.noComp.val;
 for(memberIdx =0;memberIdx < numOfComps;memberIdx++)
 {

  member =&(eRABRlsCom->protocolIEs.member[memberIdx]);

  if(member->id.val == Sztid_E_RABToBeRlsdLst)
  {
    noComp = (U8)member->value.u.sztE_RABLst.noComp.val;
   veFillTknU16(&(ie->value.u.sztE_RABLst.noComp), noComp);


  if(cmGetMem(szERABRlsRspPdu,
            (noComp * sizeof(SztProtIE_Cont_E_RABRlsRespIEs)),
            (Ptr *)&(ie->value.u.sztE_RABLst.member)) !=ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF," ERAB Setup Rsp\
               cmGetMem failed.\n"));
#endif
      RETVALUE(RFAILED);
   }


 for(idx = 0;idx < noComp ;idx++)
     {
       ie1 = &(ie->value.u.sztE_RABLst.member[idx]);
       protIe = &(member->value.u.sztE_RABLst.member[idx]);
       itemIe = &(protIe->value.u.sztE_RABItem);
       veFillTknU8(&(ie1->pres),PRSNT_NODEF);
       veFillTknU32(&(ie1->id),Sztid_E_RABItem);
       veFillTknU32(&(ie1->criticality),SztCriticalityignoreEnum);
       cmMemcpy((U8*)&(ie1->value.u.sztE_RABItem.e_RAB_ID),(U8*)&(itemIe->e_RAB_ID),sizeof(SztE_RAB_ID));
       veFillTknU8(&ie1->value.u.sztE_RABItem.cause.choice,0);
       veFillTknU32(&(ie1->value.u.sztE_RABItem.cause.val.radioNw),\
          SztCauseRadioNwfailure_in_radio_interface_procedureEnum);

       for(rbIdx =0 ;rbIdx < VE_MAX_RABS_IN_UE;rbIdx++)
       {

          if(itemIe->e_RAB_ID.val == ueCb->rbInfo.rab[rbIdx].eRABId)
          {
             ueCb->rbInfo.rab[rbIdx].inUse = TRUE;
             ueCb->rbInfo.numOfRbCfgd++;

          }
       }

     ie1->value.u.sztE_RABItem.iE_Extns.noComp.pres = NOTPRSNT;
     }


}

}

RETVALUE(ROK);
}
/*RRC ReEst */
#ifdef ANSI
PUBLIC S16 veSndRrcReCfgReqAftrReEst
(
VeUeCb *ueCb
)
#else
PUBLIC S16 veSndRrcReCfgReqAftrReEst(ueCb)
VeUeCb *ueCb;
#endif
{
    NhuDatReqSdus *nhuDatReqEvntSdu = NULLP;
    NhuDL_DCCH_Msg *dcchMsg = NULLP;
    NhuRRCConRecfgn *rrcConReCfg = NULLP;
    NhuRRCConRecfgn_r8_IEs *conRecfgIEs = NULLP;
    NhuRadioResourceConfigDedicated             *rrcCfgDed;
    NhuDRB_ToAddMod                             *drbIe;
    NhuRRCConRecfgn_r8_IEsdedicatedInfoNASLst   *dedNasLst;
    NhuRbCfgInfo              *drbCfg;
    NhuUeCfgInfo              *ueCfg;
    NhuRbCfgList              *rbCfgList;
    U8                         numDrbs = 0;
    U8                         rbIdx;
    TRC2(veSndRrcRecfgReqAftrReEst);
    #ifdef VE_PICO
      /* Enabling Call Scenario Prints ccpu00117125*/
       ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
       ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "ENB -> UE : RRC ReConfiguration Request After ReEstablishment");
       ENB_DEBUG_MSG(ENB_DBG_SUMMARY, "**************************************** ");
    #endif
       /* Allocate memory for event structure */
       VE_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
       /* Fill HDR Info */
       veFillNhuEvntHdr(&(nhuDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_RRCCONRECFGN);
       /* Populating NHU Dat Req */
       nhuDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = VE_PDCP_ID_SRB1;
       dcchMsg = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
       nhuDatReqEvntSdu->sdu.ueMsg.isMsgDed = TRUE;
       VE_FILL_TKN_UINT(dcchMsg->pres, PRSNT_NODEF);
       VE_FILL_TKN_UINT(dcchMsg->message.choice, DL_DCCH_MSGTYP_C4);
       VE_FILL_TKN_UINT(dcchMsg->message.val.c1.choice, C4_RRCCONRECFGN)
        rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
       VE_FILL_TKN_UINT(rrcConReCfg->pres, PRSNT_NODEF);
   
       VE_FILL_TKN_UINT(rrcConReCfg->rrc_TrnsxnId, VE_VAL_ONE);
       VE_FILL_TKN_UINT(rrcConReCfg->criticalExtns.choice, CRITICALEXTNS_C12);
       VE_FILL_TKN_UINT(rrcConReCfg->criticalExtns.val.c1.choice,\
             C12_RRCCONRECFGN_R8);
   
       conRecfgIEs = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);

       /* Common UE Cfg Info */
       nhuDatReqEvntSdu->sdu.isUeCfgPres = TRUE;
       ueCfg = &(nhuDatReqEvntSdu->sdu.ueCfg);
       /* filling UeCfg type*/
       ueCfg->ueCfgBitMsk = NHU_CONN_RECFG_AFTER_RESTB|NHU_EXP_CFG_TRCH_CFG;
       /*reestablishment cause is radio link failure and reconfiguration did not triggered for 
        any s1ap req mssg from ENB*/
       if(ueCb->reEstCause == RLF_CAUSE)
       {
           VE_FILL_TKN_UINT(conRecfgIEs->pres, NOTPRSNT);

           RETVALUE(ROK);

       } 
       else
       {
           VE_FILL_TKN_UINT(conRecfgIEs->pres, PRSNT_NODEF);
       }
             
       rrcCfgDed = &(conRecfgIEs->radioResourceConfigDedicated);

 
      VE_FILL_TKN_UINT(rrcCfgDed->pres, PRSNT_NODEF);
      rrcCfgDed->srb_ToAddModLst.noComp.pres = NOTPRSNT;
      rrcCfgDed->drb_ToAddModLst.noComp.pres = NOTPRSNT;
      rrcCfgDed->drb_ToRlsLst.noComp.pres = NOTPRSNT;
      rrcCfgDed->mac_MainConfig.choice.pres = NOTPRSNT;
      rrcCfgDed->mac_MainConfig.val.explicitValue.pres.pres = NOTPRSNT;
      rrcCfgDed->sps_Config.pres.pres = NOTPRSNT;
      rrcCfgDed->physicalConfigDedicated.pres.pres = NOTPRSNT;
       VE_FILL_TKN_UINT(rrcCfgDed->drb_ToAddModLst.noComp, ueCb->rbInfo.numOfRbCfgd);
   
       /* Allocate memory for each Rab */
       VE_GET_MEM(nhuDatReqEvntSdu, (sizeof(NhuDRB_ToAddMod) * ueCb->rbInfo.numOfRbCfgd),\
             &(rrcCfgDed->drb_ToAddModLst.member));
      if (rrcCfgDed->drb_ToAddModLst.member == NULLP)
      {
          RETVALUE(RFAILED);
      }
     
         dedNasLst = &(conRecfgIEs->dedicatedInfoNASLst);
      dedNasLst->noComp.pres = PRSNT_NODEF; 
      dedNasLst->noComp.val=0;  

      nhuDatReqEvntSdu->sdu.isUeCfgPres = TRUE;
      ueCfg = &(nhuDatReqEvntSdu->sdu.ueCfg);
          /* filling UeCfg type*/
          ueCfg->ueCfgBitMsk = NHU_CONN_RECFG_AFTER_RESTB|NHU_EXP_CFG_TRCH_CFG;
          ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_ADD;
   
          rbCfgList = &(ueCfg->rbCfgAllList.rbToAddCfgList);
         
         rbCfgList->numOfRbCfgInfo = ueCb->rbInfo.numOfRbCfgd;
          VE_GET_MEM(nhuDatReqEvntSdu, (sizeof(NhuRbCfgInfo) * ueCb->rbInfo.numOfRbCfgd),\
              &(rbCfgList->rbCfgInfoList));

     for (rbIdx = 0; rbIdx < ueCb->rbInfo.numOfRbCfgd; rbIdx++)
        {
          drbIe   = rrcCfgDed->drb_ToAddModLst.member + rbIdx;
/* Update DRB to ADD Mod List */
          
             drbIe = &(rrcCfgDed->drb_ToAddModLst.member[rbIdx]);
             VE_FILL_TKN_UINT(drbIe->pres, PRSNT_NODEF);
             VE_FILL_TKN_UINT(drbIe->eps_BrIdentity, ueCb->rbInfo.rab[rbIdx].eRABId);
             VE_FILL_TKN_UINT(drbIe->drb_Identity, ueCb->rbInfo.rab[rbIdx].rbId);
   
             /* PDCP-Config */
             VE_FILL_TKN_UINT(drbIe->pdcp_Config.pres, PRSNT_NODEF);
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
                             
              
          drbCfg = (NhuRbCfgInfo *) &(rbCfgList->rbCfgInfoList[rbIdx]);
           drbCfg->rbCfgBitMsk = (NHU_RB_CFG_UL |\
                   NHU_RB_CFG_DL | NHU_RB_CFG_PDCP_INFO);
   
             drbCfg->rbId.rbId = ueCb->rbInfo.rab[rbIdx].rbId;
             drbCfg->rbId.rbTyp = CM_LTE_DRB;
   
   
             /* UL Logical Channel Mapping Info */
             /* Bit mask calculate properly */
             drbCfg->ulRbCfg.ulRbBitMsk |=
                (NHU_RB_CFG_LOGCH_MAP_UL | NHU_RB_CFG_RLC_MODE_UL);
   
             /* Uplink Logical Channel Mapping Info.  */
            
                /* Transport Channel type */
                drbCfg->ulRbCfg.ulLogChMap.trChType = CM_LTE_TRCH_UL_SCH;
   
                /* Logical Channel Id and Type */
                drbCfg->lcId.lcId = drbCfg->rbId.rbId;
                drbCfg->lcId.logChType = (CmLteLcType)CM_LTE_LCH_DTCH;
   
                /* Fill Logical Channel Cfg */
                veFillUlLogChCfg(&(drbCfg->ulRbCfg.ulLogChMap),drbCfg->rbId.rbId);
   
                /* ulRlcMode */
                 drbCfg->ulRbCfg.ulRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
                drbCfg->ulRbCfg.ulRlcMode.mode = NHU_RLC_MODE_AM;
                veFillUlRlcModeInfo(&(drbCfg->ulRbCfg.ulRlcMode), drbCfg->rbId.rbId);
             

/* Downlink Logical Channel Mapping Info */
             drbCfg->dlRbCfg.dlRbBitMsk |=
                (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);
   
            /* Downlink Logical Channel Mapping Info. */
             
                /* Logical Channel ID and type */
                drbCfg->lcId.lcId = drbCfg->rbId.rbId;
                drbCfg->lcId.logChType = (CmLteLcType)CM_LTE_LCH_DTCH;
                /* Transport Channel Type */
                drbCfg->dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_DL_SCH;
                /* DL-RLC mode */
                drbCfg->dlRbCfg.dlRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
                drbCfg->dlRbCfg.dlRlcMode.mode = NHU_RLC_MODE_AM;
                veFillDlRlcModeInfo(&(drbCfg->dlRbCfg.dlRlcMode), drbCfg->rbId.rbId);
             
   
             /* PDCP Cfg */
             veFillPdcpCfg(&(drbCfg->pdcpCfg), drbCfg->rbId.rbId, FALSE);
            
          
           numDrbs++;
       } /* end of for loop */

       rbCfgList->numOfRbCfgInfo = numDrbs;
      /* Send RRC Connection reconfiguration message through NhuDatRsp Primitive*/
       if (RFAILED == veSndUENhuDatReq(nhuDatReqEvntSdu))
         {
          VE_DBG_ERROR((VE_PRNT_BUF,
                   "veSndRrcRecfgReq: veSndUENhuDatReq failed.\n"));
          VE_FREEEVNT(nhuDatReqEvntSdu);
          RETVALUE(RFAILED);
       }
       
      RETVALUE(ROK);
}

/********************************************************************30**

         End of file:     ve_bldmsg.c@@/main/2 - Wed Dec  1 08:44:21 2010

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
$SID$      ----       mnawas       1. eNodeb 3.1 Release
*********************************************************************91*/
