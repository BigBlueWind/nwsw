/********************************************************************16**

                         (c) COPYRIGHT 2010 by
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

     Name:     LTE-SCL Layer

     Type:     C souce file

     Desc:     Phy UL Module. Configures and handles uplink.

     File:     ys_ms_ul.c

     Sid:

     Prg:      mraj

**********************************************************************/

/* Trillium Includes */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list defines  */
#include "cm_hash.h"       /* Common hashlist defines */
#include "cm_lte.h"        /* Common LTEE defines */
#include "ys_ms_err.h"        /* CL error header file */
#include "tfu.h"
#include "ctf.h"
#include "lys.h"
#include "ys_ms.h"

#ifdef TENB_AS_SECURITY
#include "spaccdrv.h" /*sumanth*/
#endif

#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"        /* Common LTE includes */
#include "cm_lib.x"
#include "tfu.x"
#include "ctf.x"
#include "lys.x"

/* Silicon Includes */
#include "LtePhyL2Api.h"
#include "apidefs.h"
#include "mlog.h"

#include "ys_ms.x"

int      delayedHarq =0;
/*ys004.102 :  Merged MSPD code with phy 1.7 */

PRIVATE S16 ysMsPrcUlSchSduInd ARGS((
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb,
TfuDatIndInfo   *datIndInfo
));

PRIVATE S16 ysMsUpdCrcInd ARGS((
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb,
TfuCrcIndInfo   *crcIndInfo
));
PRIVATE S16 ysMsPrcHarqInd ARGS((
YsCellCb        *cellCb,
TfuHqIndInfo    *harqIndInfo,
CmLteTimingInfo *timingInfo,
U8              *hqBuf,
Bool            isPusch,
CmLteRnti       rnti,
U32             numHarqBits
));


#ifdef TFU_UPGRADE
PRIVATE S16 ysMsPrcDlRiInd ARGS((
YsCellCb         *cellCb,
CmLteRnti        rnti,
CmLteTimingInfo  timingInfo,
U8               numRiBits,
U8               riByte
));
#endif

PRIVATE S16 ysMsPrcDlCqiInd ARGS((
YsCellCb         *cellCb,
CmLteRnti        rnti,
CmLteTimingInfo  timingInfo,
U8               numCqiBits,
U8               *cqiBits
));

#ifndef YS_MS_NO_ULCQI
PRIVATE S16 ysMsPrcUlCqiInd ARGS((
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb,
TfuUlCqiIndInfo **ulCqiIndRef
));
#endif

PRIVATE S16 ysMsPrcSrInd ARGS((
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb,
TfuSrIndInfo    *srIndInfo
));


PRIVATE S16 ysMsPrcCqiPmiRiHqInd ARGS((
PRXSTATUSIND       pRxStatusInd,
YsCellCb        	 *cellCb
));

#ifndef YS_MS_NO_TA
PRIVATE S16 ysMsPrcTmAdvInd ARGS((
PRXSDUIND       rxSduInd,
YsCellCb         *cellCb,
TfuTimingAdvIndInfo **tmAdvIndInfoRef
));
#endif

#if 0
PRIVATE S16 ysMsPrachPresent ARGS((
TfuRecpReqInfo  *recpReq,
RACHCTRL       *rachCtrl
));
#endif

#ifdef TENB_AS_SECURITY
PUBLIC S16 ysMsKDF  (YsMsSecInfo *pSecInfo,  U8 *key);
#endif

PRIVATE S16 ysMsPrcPucchPwrInd ARGS((
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb
));

PRIVATE S16 ysMsPrcPucchDeltaPwrInd ARGS((
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb,
YsUeCb          *ue,
TfuPucchDeltaPwrIndInfo *pucchDeltaPwrIndInfo
));

extern S16 DiagDumpPhyApi(PTR vector);

static    U32     delayedApiCnt = 0;
static    U32     ysOneRachCompleted = 0;
#ifdef HARQ_STATISTICS
extern U32 totl_ulTbs;
extern U32 ulAcks;
extern U32 ulNacks;
extern U32 dlNacks;
extern U32 dlAcks;
#endif
extern U32 ulCqiRptCnt[16];
extern U32 dlCqiRptCnt[16];
extern U32 totDlCqiRpt;
extern U32 totUlCqiRpt;

/****************ULM Modules ********************************/

/*********       Utility Functions **************************/


#ifndef TFU_UPGRADE
/*
*
*       Fun:   ysMsUlmDelSchLst
*
*       Desc:  Delete Scheduling Info
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsUlmDelSchLst
(
U16             period,
U16             offset,
U16             max,
CmLListCp       *lCp,
YsUeCb          *ueCb
)
#else
PRIVATE S16 ysMsUlmDelSchLst(period, offset, max, lCp, ueCb)
U16             period;
U16             offset;
U16             max;
CmLListCp       *lCp;
YsUeCb          *ueCb;
#endif
{
   U16         tmpPeriod;
   CmLListCp  *tmpCp;
   CmLList    *cmLstEnt;

   tmpPeriod = period - 1;
   tmpPeriod = offset;
      MSPD_DBG("CL delete UE:offset %d \n", offset);
   do
   {
      tmpCp    = &lCp[tmpPeriod];
      cmLstEnt = tmpCp->first;

      while (cmLstEnt)
      {
         if (((YsUeCb*)(cmLstEnt->node))->ueId == ueCb->ueId)
         {
            cmLListDelFrm(tmpCp, cmLstEnt);
            ysUtlDeAlloc((Data*) cmLstEnt, sizeof(cmLstEnt));
            break;
         }
         else
         {
            cmLstEnt = cmLstEnt->next;
         }
      }
      tmpPeriod += period;
   }while (tmpPeriod<max);

   RETVALUE (ROK);
} /* End of ysMsUlmDelSchLst */

/*
*
*       Fun:   ysMsUlmSetSchLst
*
*       Desc:  Set Scheduling Info
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsUlmSetSchLst
(
U16             period,
U16             offset,
U16             max,
CmLListCp       *lCp,
YsUeCb          *ueCb
)
#else
PRIVATE S16 ysMsUlmSetSchLst(period, max, lCp, cmListEnt)
U16             period;
U16             offset;
U16             max;
CmLListCp       *lCp;
YsUeCb          *ueCb;
#endif
{
   U16              tmpPeriod;
   CmLListCp       *tmpCp;
   CmLList         *cmListEnt;

   tmpPeriod = offset;
   do
   {
      tmpCp = &lCp[tmpPeriod];

      /* Allocate Mem for list entity */
      cmListEnt = (CmLList*)ysUtlMalloc(sizeof(CmLList));
      if (!cmListEnt)
      {
         YS_LOGERR_DBG(EYSXXX, sizeof(CmLList), "ysUtlMalloc(): Failed Allocation\n");
         RETVALUE (RFAILED);
      }

      /* Save UE Control Block */
      cmListEnt->node = (PTR)ueCb;

      /* Adding List Element */
      cmLListAdd2Tail(tmpCp, cmListEnt);

      /* Moving to Next Element */
      tmpPeriod += period;

   }while (tmpPeriod<max);

   RETVALUE (ROK);
} /* End of ysMsUlmSetSchLst */

/*
*
*       Fun:   ysMsUlmPrcUeCfg
*
*       Desc:  Process Scheduling Info
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUlmPrcUeCfg
(
YsCellCb        *cellCb,
YsUeCb          *ueCb
)
#else
PUBLIC S16 ysMsUlmPrcUeCfg(cellCb, ueCb)
YsCellCb        *cellCb;
YsUeCb          *ueCb;
#endif
{
   CtfDedCfgInfo   *ueDedCfg;
   CtfCqiRptModePeriodic   *periodicCqi;
   CtfDedSrsUlCfgInfo      *srsCfgInfo;
   CtfDedSRCfgInfo         *srCfgInfo;

   ueDedCfg = &ueCb->ueCfg;
   MSPD_DBG("In ysMsUlmPrcUeCfg, ueId= %d\n",ueCb->ueId);

   /* Cqi Configuration */
   if (ueDedCfg->cqiRptCfg.pres)
   {
      if (CTF_CQI_RPTMODE_PRDIOC == ueDedCfg->cqiRptCfg.reportingMode)
      {
         periodicCqi = &(ueDedCfg->cqiRptCfg.reportMode.periodicRpt);
         if (periodicCqi->cqiPeriodicCfgType == CTF_IE_CFG_SETUP)
         {
            ueCb->cqiCfgInfo = &ysCqiPmiCfgDb[periodicCqi->cqiSetup.cqiPmiCfgIndx];
            ysMsUlmSetSchLst(ueCb->cqiCfgInfo->cqiPeriod,
                  ueCb->cqiCfgInfo->cqiSfOffSet, YS_CQI_PERIOD_160,
                  cellCb->cqiPeriodList, ueCb);
         }
      }
   }

   /* SRS Configuration */
   if (ueDedCfg->srsUlCfg.pres)
   {
      srsCfgInfo = &ueDedCfg->srsUlCfg;
      if (CTF_CONFIG == srsCfgInfo->dedSrsUlCfgType)
      {
         ueCb->srsCfgInfo = &ysSrsCfgDb[srsCfgInfo->dedSrsSetup.srsCfgIdx];
         ysMsUlmSetSchLst(ueCb->srsCfgInfo->srsPeriod,
               ueCb->srsCfgInfo->srsSfOffSet, YS_SRS_PERIOD_320,
               cellCb->srsPeriodList, ueCb);
      }
   }

   /* SR Configuration */
   if (ueDedCfg->dedSRCfg.pres)
   {
      srCfgInfo = &ueDedCfg->dedSRCfg;
      if (CTF_CONFIG == srCfgInfo->dedSRCfgType)
      {
         ueCb->srCfgInfo = &ysSrCfgDb[srCfgInfo->dedSrSetup.srCfgIdx];
         ysMsUlmSetSchLst(ueCb->srCfgInfo->srPeriod,
               ueCb->srCfgInfo->srSfOffSet, YS_SR_PERIOD_80,
               cellCb->srPeriodList, ueCb);
         YS_DBG_ERR((_ysp, "SR configuration: idx %d, period %d, offset %d\n",
            srCfgInfo->dedSrSetup.srCfgIdx,ueCb->srCfgInfo->srPeriod,
            ueCb->srCfgInfo->srSfOffSet));
      }
   }

   ueCb->timingAdvErrInfo.mSetDefaults = 1;

   RETVALUE (ROK);

} /* End of ysMsUlmPrcUeCfg */

/*
*
*       Fun:   ysMsUlmPrcDelUeCfg
*
*       Desc:  Delete UE Configuration
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUlmPrcDelUeCfg
(
YsCellCb        *cellCb,
YsUeCb          *ueCb
)
#else
PUBLIC S16 ysMsUlmPrcDelUeCfg(cellCb, ueCb)
YsCellCb        *cellCb;
YsUeCb          *ueCb;
#endif
{
   CtfDedCfgInfo           *ueDedCfg;

   ueDedCfg = &ueCb->ueCfg;

   /* Removing UE Cqi Information */
   if (ueDedCfg->cqiRptCfg.pres)
   {
      ysMsUlmDelSchLst(ueCb->cqiCfgInfo->cqiPeriod,
            ueCb->cqiCfgInfo->cqiSfOffSet, YS_CQI_PERIOD_160,
                                     cellCb->cqiPeriodList, ueCb);
   }

   /* Removing UE SRS Information */
   if (ueDedCfg->srsUlCfg.pres)
   {
      ysMsUlmDelSchLst(ueCb->srsCfgInfo->srsPeriod,
            ueCb->srsCfgInfo->srsSfOffSet, YS_SRS_PERIOD_320,
                            cellCb->srsPeriodList, ueCb);
   }

   /* Removing UE SR Information */
   if (ueDedCfg->dedSRCfg.pres)
   {
      ysMsUlmDelSchLst(ueCb->srCfgInfo->srPeriod, ueCb->srCfgInfo->srSfOffSet, YS_SR_PERIOD_80,
            cellCb->srPeriodList, ueCb);
   }

   RETVALUE(ROK);

} /* End of ysMsUlmPrcDelUeCfg */

#endif /* TFU_UPGRADE */

#ifdef TENB_AS_SECURITY
#ifdef ANSI
PUBLIC S16 YsMsUlmPrcKenbStarCfg
(
CtfKenbStarInfo*       kenbInf,
CtfKenbStarCfmInfo       *kdfCfm
)
#else
PUBLIC S16 YsMsUlmPrcKenbStarCfg(kenbInf)
CtfAskeyCfmInfo*       kenbInf;
#endif
{

   S16       ret;
   YsMsSecInfo  *pSecInfo;
   U8 i;


   TRC2(YsMsUlmPrcKenbCfg)
   YS_DBG_INFO((_ysp, "YsUiCtfDatReq(): spId = %d",spId));
   ret = ROK;

   YS_MS_ALLOC(pSecInfo , sizeof(YsMsSecInfo));
   if(pSecInfo == NULLP)
   {
      MSPD_DBG("YsMsUlmPrcKenbStarCfg failed line %d \n", __LINE__);
   }


   cmMemcpy(pSecInfo->hash_key,kenbInf->secKey,CTF_SEC_KEY_LEN);

   /* Building the String S = FC || P0 || L0 || P1 || L1 */

   /*Values are initialized as per section A7 33.401*/
   pSecInfo->strS[0] = 0x13;  pSecInfo->strS[3] = 0x00; pSecInfo->strS[4] = 0x02;
   pSecInfo->strS[7] = 0x00; pSecInfo->strS[8] = 0x02;


   /*******************************************
    *************kENB* from kENB*************
    *******************************************/

   /*PCI*/
   pSecInfo->strS[1] = kenbInf->physCellId >> 8;
   pSecInfo->strS[2] = (U8)kenbInf->physCellId;

   /*DL frequency*/
   pSecInfo->strS[5] = kenbInf->dlCarrFreq >> 8;
   pSecInfo->strS[6] = (U8)kenbInf->dlCarrFreq;

   pSecInfo->strSlen = 9;

   /*MSPD_DBG("FREQ %d PCI %d length %d\n", kenbInf->dlCarrFreq, kenbInf->physCellId, strlen(pSecInfo->strS));

   MSPD_DBG("&&&&&&&&&&&&&&&&&STRING &&&&&&&&&&&&&&&&&&&&&&&\n");
   for(i=0;i<9;i++)
   MSPD_DBG("%x ", pSecInfo->strS[i]);*/

   ret = ysMsKDF(pSecInfo, kdfCfm->secKey);
   if(ret!=ROK)
   {
     YS_DBG_ERR((_ysp, "ysMsSpaccClose failed\n"));
     RETVALUE(RFAILED);
   }/*End of if(ret!=ROK)*/


   YS_MS_FREE(pSecInfo, sizeof(YsMsSecInfo));


   RETVALUE(ROK);
}/*End of function*/


#ifdef ANSI
PUBLIC S16 YsMsUlmPrcKenbCfg
(
CtfAsKeyInfo*       kenbInf,
CtfAskeyCfmInfo       *kdfCfm
)
#else
PUBLIC S16 YsMsUlmPrcKenbCfg(kenbInf)
CtfAskeyCfmInfo*       kenbInf;
#endif
{

   S16       ret;
   YsMsSecInfo  *pSecInfo;


   TRC2(YsMsUlmPrcKenbCfg)
   YS_DBG_INFO((_ysp, "YsUiCtfDatReq(): spId = %d",spId));
   ret = ROK;

   YS_MS_ALLOC(pSecInfo , sizeof(YsMsSecInfo));
   if(pSecInfo == NULLP)
   {
      MSPD_DBG("YsMsUlmPrcKenbCfg failed line %d \n", __LINE__);
   }


   cmMemcpy(pSecInfo->hash_key,kenbInf->secKey,CTF_SEC_KEY_LEN);

   /* Building the String S = FC || P0 || L0 || P1 || L1 */

   /*Values are initialized as per section A7 33.401*/
   pSecInfo->strS[0] = 0x15;  pSecInfo->strS[2] = 0x00; pSecInfo->strS[3] = 0x01;
   pSecInfo->strS[5] = 0x00; pSecInfo->strS[6] = 0x01;

   pSecInfo->strSlen = 7;

   /*******************************************
    *************CP Ciphering key*************
    *******************************************/
   pSecInfo->strS[1] = 0x3;
   pSecInfo->strS[4] = kenbInf->ciphAlgoType;

   ret = ysMsKDF(pSecInfo, kdfCfm->cpCiphKey);
   if(ret!=ROK)
   {
     YS_DBG_ERR((_ysp, "ysMsSpaccClose failed\n"));
     RETVALUE(RFAILED);
   }/*End of if(ret!=ROK)*/



   /*******************************************
    *************UP Ciphering key*************
    *******************************************/
   pSecInfo->strS[1] = 0x5;
   pSecInfo->strS[4] = kenbInf->ciphAlgoType;

   ret = ysMsKDF(pSecInfo, kdfCfm->upCiphKey);
   if(ret!=ROK)
   {
     YS_DBG_ERR((_ysp, "ysMsSpaccClose failed\n"));
     RETVALUE(RFAILED);
   }/*End of if(ret!=ROK)*/



   /*******************************************
    ************RRC Integrity key*************
    *******************************************/
   pSecInfo->strS[1] = 0x4;
   pSecInfo->strS[4] = kenbInf->intgAlgoType;

   ret = ysMsKDF(pSecInfo, kdfCfm->intKey);
   if(ret!=ROK)
   {
     YS_DBG_ERR((_ysp, "ysMsSpaccClose failed\n"));
     RETVALUE(RFAILED);
   }/*End of if(ret!=ROK)*/

#if 0
   MSPD_DBG("INNNNNNTTEEGGRRRRRRRRRRRity KEEEEEEYYYYY\n");
   for(idx=0;idx<32;idx++)
   MSPD_DBG("%x ",  pSecInfo->pOutBuf[idx]);
   MSPD_DBG("\n");
   MSPD_DBG("ALGOOOOOOOOOOOOO %d\n",kenbInf->algoType);
#endif



   YS_MS_FREE(pSecInfo, sizeof(YsMsSecInfo));


   RETVALUE(ROK);
}/*End of function*/
#endif

/*
*       Fun:   ysMsSetupUlCqiInfo
*
*       Desc:
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*/
#ifdef ANSI
PRIVATE S16 ysMsSetupUlCqiInfo
(
YsCellCb        *cellCb
)
#else
PRIVATE S16 ysMsSetupUlCqiInfo(cellCb)
YsCellCb        *cellCb;
#endif
{
   U8        sinr;
   U8        mcs;
   U8        *sinrToMcs = cellCb->cellInfo.sinrToMcs;
   U8        *mcsToCqi  = cellCb->cellInfo.mcsToCqi;

   /*
    * SINR Index  Mcs Index
    * [0:101]     0           < 10% throughput
    * [102:103]   0           10% throughput
    * [104:105]   0           ...
    * [106:107]   0
    * [108:109]   0
    * [110:111]   0
    * [112:113]   0
    * [114:115]   0
    * [116:117]   0
    * [118:119]   0
    * [120:121]   0
    * [122:131]   1
    * [132:133]   2
    * [134:135]   3
    * [136:137]   4
    * [138:139]   5
    * [140:141]   6
    * [142:143]   7
    * [144:145]   8
    * [146:147]   9
    * 148         10
    * 149         11
    * 150         12
    * 151         13
    * 152         14
    * 153         15
    * 154         16
    * 155         17
    * 156         18
    * 157         19
    * 158         20
    * [159:162]   21
    * [163:166]   22
    * [167:170]   23
    * [171:174]   24
    * [175:178]   25
    * [179:182]   26
    * [183:186]   27
    * [187:190]   28
    * [191:255]   28
    */
   /* Based on above, fill SINR to MCS mapping */
   sinr = 0;
   /* if (sinr <= 101) cqi may be considered 0 */
   for (; sinr <= 121; ++sinr)
      sinrToMcs[sinr] = 0;
   for (; sinr <= 131; ++sinr)
      sinrToMcs[sinr] = 1;
   for (; sinr <= 133; ++sinr)
      sinrToMcs[sinr] = 2;
   for (; sinr <= 135; ++sinr)
      sinrToMcs[sinr] = 3;
   for (; sinr <= 137; ++sinr)
      sinrToMcs[sinr] = 4;
   for (; sinr <= 139; ++sinr)
      sinrToMcs[sinr] = 5;
   for (; sinr <= 141; ++sinr)
      sinrToMcs[sinr] = 6;
   for (; sinr <= 143; ++sinr)
      sinrToMcs[sinr] = 7;
   for (; sinr <= 145; ++sinr)
      sinrToMcs[sinr] = 8;
   for (; sinr <= 147; ++sinr)
      sinrToMcs[sinr] = 9;
   for (; sinr <= 148; ++sinr)
      sinrToMcs[sinr] = 10;
   for (; sinr <= 149; ++sinr)
      sinrToMcs[sinr] = 11;
   for (; sinr <= 150; ++sinr)
      sinrToMcs[sinr] = 12;
   for (; sinr <= 151; ++sinr)
      sinrToMcs[sinr] = 13;
   for (; sinr <= 152; ++sinr)
      sinrToMcs[sinr] = 14;
   for (; sinr <= 153; ++sinr)
      sinrToMcs[sinr] = 15;
   for (; sinr <= 154; ++sinr)
      sinrToMcs[sinr] = 16;
   for (; sinr <= 155; ++sinr)
      sinrToMcs[sinr] = 17;
   for (; sinr <= 156; ++sinr)
      sinrToMcs[sinr] = 18;
   for (; sinr <= 157; ++sinr)
      sinrToMcs[sinr] = 19;
   for (; sinr <= 158; ++sinr)
      sinrToMcs[sinr] = 20;
   for (; sinr <= 162; ++sinr)
      sinrToMcs[sinr] = 21;
   for (; sinr <= 166; ++sinr)
      sinrToMcs[sinr] = 22;
   for (; sinr <= 170; ++sinr)
      sinrToMcs[sinr] = 23;
   for (; sinr <= 174; ++sinr)
      sinrToMcs[sinr] = 24;
   for (; sinr <= 178; ++sinr)
      sinrToMcs[sinr] = 25;
   for (; sinr <= 182; ++sinr)
      sinrToMcs[sinr] = 26;
   for (; sinr <= 186; ++sinr)
      sinrToMcs[sinr] = 27;
   for (; sinr <= 254; ++sinr)
      sinrToMcs[sinr] = 28;
   sinrToMcs[sinr] = 28;

   /*
    * In MAC, for normal cyclic prefix, this is what we have
    * cqi=0  -> iTbs=0  -> iMcs=0
    * cqi=1  -> iTbs=0  -> iMcs=0
    * cqi=2  -> iTbs=0  -> iMcs=0
    * cqi=3  -> iTbs=2  -> iMcs=2
    * cqi=4  -> iTbs=4  -> iMcs=4
    * cqi=5  -> iTbs=6  -> iMcs=6
    * cqi=6  -> iTbs=8  -> iMcs=8
    * cqi=7  -> iTbs=10 -> iMcs=10
    * cqi=8  -> iTbs=12 -> iMcs=13
    * cqi=9  -> iTbs=15 -> iMcs=16
    * cqi=10 -> iTbs=16 -> iMcs=17
    * cqi=11 -> iTbs=19 -> iMcs=20
    * cqi=12 -> iTbs=21 -> iMcs=23
    * cqi=13 -> iTbs=23 -> iMcs=25
    * cqi=14 -> iTbs=25 -> iMcs=27
    * cqi=15 -> iTbs=26 -> iMcs=28
    */
   /* Based on above, fill mcs to cqi mapping */
   mcs = 0;
   mcsToCqi[mcs++] = 0;
   mcsToCqi[mcs++] = 0;
   mcsToCqi[mcs++] = 3; mcsToCqi[mcs++] = 3;
   mcsToCqi[mcs++] = 4; mcsToCqi[mcs++] = 4;
   mcsToCqi[mcs++] = 5; mcsToCqi[mcs++] = 5;
   mcsToCqi[mcs++] = 6; mcsToCqi[mcs++] = 6;
   mcsToCqi[mcs++] = 7; mcsToCqi[mcs++] = 7; mcsToCqi[mcs++] = 7;
   mcsToCqi[mcs++] = 8; mcsToCqi[mcs++] = 8; mcsToCqi[mcs++] = 8;
   mcsToCqi[mcs++] = 9;
   mcsToCqi[mcs++] = 10; mcsToCqi[mcs++] = 10; mcsToCqi[mcs++] = 10; mcsToCqi[mcs++] = 10;
   mcsToCqi[mcs++] = 11; mcsToCqi[mcs++] = 11;
   mcsToCqi[mcs++] = 12; mcsToCqi[mcs++] = 12;
   mcsToCqi[mcs++] = 13; mcsToCqi[mcs++] = 13;
   mcsToCqi[mcs++] = 14;
   mcsToCqi[mcs++] = 15;
   RETVALUE(ROK);
}


/*
*
*       Fun:   ysMsUlmPrcCellCfg
*
*       Desc:  Process Cell Configuration
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUlmPrcCellCfg
(
YsCellCb        *cellCb
)
#else
PUBLIC S16 ysMsUlmPrcCellCfg(cellCb)
YsCellCb        *cellCb;
#endif
{
#ifndef TFU_UPGRADE
   U16                                idx;
#endif
   CtfPrachCfgInfo                   *prachCfg;
   YsPrachFddCfgInfo                 *ysPrachInfo;
   YsPrachCb                         *rachCb;
   U16                                subloop;
   U16                                i;
   CtfPucchCfgInfo                   *pucchCfgInfo;
   CtfSrsUlCfgInfo                   *srsUlCfg;
   CtfPuschCfgInfo                   *puschCfg;
   CtfPuschCfgBasic                  *puschCfgBasic;
   CtfPuschUlRS                      *puschUlRs;

   /* L1 */
   RACHCTRL                          *rachCtl;
   ULSUBFRCMNCTRL                    *ulSubFrCmnCtrl;


#ifndef TFU_UPGRADE
   /* Initialize the Periodic Scheduling Lists */
   for (idx=0; idx<YS_SRS_PERIOD_320; idx++)
   {
      cmLListInit(&(cellCb->srsPeriodList[idx]));
   }
   for (idx=0; idx<YS_CQI_PERIOD_160; idx++)
   {
      cmLListInit(&(cellCb->cqiPeriodList[idx]));
   }
   for (idx=0; idx<YS_SR_PERIOD_80; idx++)
   {
      cmLListInit(&(cellCb->srPeriodList[idx]));
   }
#endif /* TFU_UPGRADE */

   /* Initialize RACH Scheduling */
   prachCfg            = &cellCb->cellCfg.prachCfg;
   ysPrachInfo         = &ysPrachFddCfgDb[prachCfg->prachCfgIndex];
   rachCb             = &cellCb->prachCb;
   rachCb->rachCfgInfo = ysPrachInfo;
   rachCb->ysPrachPres = 0x00;

   /* ODD frames not support for FDD */
   MSPD_DBG("prachCfg->prachCfgIndex:%d, numSf:%d \n",  prachCfg->prachCfgIndex, ysPrachInfo->numSf);
      for(subloop=0; subloop<ysPrachInfo->numSf; subloop++)
      {
         MSPD_DBG("ysPrachInfo->sf[subloop]:%d\n", ysPrachInfo->sf[subloop]);
         rachCb->ysPrachPres |= 0x01<<ysPrachInfo->sf[subloop];
      }
	   MSPD_DBG("rachCb->ysPrachPres:%d\n",rachCb->ysPrachPres );
   /* Rach Predefine Control Information */
   rachCtl    = &cellCb->preDefVal.rachCtrl;
   if ((rachCtl->prachEnable = cellCb->cellCfg.prachCfg.pres) != NULLP)
   {
      prachCfg = &cellCb->cellCfg.prachCfg;
      rachCtl->rootSequenceIndex         = prachCfg->rootSequenceIndex;
      rachCtl->prachConfigIndex          = prachCfg->prachCfgIndex;

      rachCtl->highSpeedFlag             = prachCfg->highSpeedFlag;
      rachCtl->zeroCorrelationZoneConfig = prachCfg->zeroCorrelationZoneCfg;
      rachCtl->prachFreqOffset           = prachCfg->prachFreqOffset;
   }
   MSPD_DBG("rootSeqIndex %d , PrachConfigIndex %d , highSpeedFlag %d , zeroCorr %d , PrachFreqOffset %d\n",rachCtl->rootSequenceIndex,rachCtl->prachConfigIndex , rachCtl->highSpeedFlag,rachCtl->zeroCorrelationZoneConfig, rachCtl->prachFreqOffset);
   /* SubFrame Common Control Structure */
   ulSubFrCmnCtrl    = &(cellCb->preDefVal.ulSubFrCmnCtrl);
   if (cellCb->cellCfg.pucchCfg.pres)
   {
      pucchCfgInfo    = &cellCb->cellCfg.pucchCfg;
      ulSubFrCmnCtrl->deltaPUCCHShift         = pucchCfgInfo->deltaShift+1;
      ulSubFrCmnCtrl->nRBCQI                  = pucchCfgInfo->nRB;
      ulSubFrCmnCtrl->nCSAn                   = pucchCfgInfo->nCS;
      ulSubFrCmnCtrl->n1PucchAN               = pucchCfgInfo->n1PUCCH;
      ulSubFrCmnCtrl->pad                     = PAD;
   } /* End of pucchCfg.pres */

   if (cellCb->cellCfg.srsUlCfg.pres)
   {
      srsUlCfg = &cellCb->cellCfg.srsUlCfg;
      ulSubFrCmnCtrl->srsBandwitdhConfig      = srsUlCfg->srsSetup.srsBw;
      ulSubFrCmnCtrl->srsSubframeConfig       = srsUlCfg->srsSetup.sfCfg;
      ulSubFrCmnCtrl->ackNackSRSSimultaneousTransmission
                                              = srsUlCfg->srsSetup.srsANSimultTx;
   }

   if (cellCb->cellCfg.puschCfg.pres)
   {
      puschCfg       = &cellCb->cellCfg.puschCfg;
      puschCfgBasic  = &puschCfg->puschBasicCfg;

      ulSubFrCmnCtrl->nSB                      = puschCfgBasic->noOfsubBands;
      ulSubFrCmnCtrl->hoppingMode              = puschCfgBasic->hoppingMode;
      ulSubFrCmnCtrl->enable64QAM              = puschCfgBasic->enable64QAM;
      ulSubFrCmnCtrl->puschhoppingOffset       = puschCfgBasic->hoppingOffset;

      puschUlRs      = &puschCfg->puschUlRS;
      ulSubFrCmnCtrl->groupHoppingEnabled      = puschUlRs->grpHopEnabled;
      ulSubFrCmnCtrl->groupAssignmentPUSCH     = puschUlRs->grpNum;
      ulSubFrCmnCtrl->sequenceHoppingEnabled   = puschUlRs->seqHopEnabled;
      ulSubFrCmnCtrl->cyclicShift              = puschUlRs->cycShift;
      ulSubFrCmnCtrl->padding                  = PAD;
   }

   switch(cellCb->cellCfg.bwCfg.dlBw)
   	{
   	  case CTF_BW_RB_6 :
	  	cellCb->cellInfo.dlBw = 6;
		break;
      case CTF_BW_RB_15:
   	    cellCb->cellInfo.dlBw = 15;
		break;
      case CTF_BW_RB_25:
	  	cellCb->cellInfo.dlBw = 25;
		break;
      case CTF_BW_RB_50:
   	    cellCb->cellInfo.dlBw = 50;
		break;
      case CTF_BW_RB_75:
	  	cellCb->cellInfo.dlBw = 75;
		break;
      case CTF_BW_RB_100:
	  	cellCb->cellInfo.dlBw = 100;
		break;
	  default:
	  	YS_DBG_ERR((_ysp, "Invalid DL BW %u", cellCb->cellCfg.bwCfg.dlBw));
   	}

   if (cellCb->cellInfo.dlBw > 63)
   {
	   cellCb->cellInfo.rbgSize  = 4;
   }
   else if ( cellCb->cellInfo.dlBw > 26)
   {
	   cellCb->cellInfo.rbgSize  = 3;
   }
   else if (cellCb->cellInfo.dlBw > 10)
   {
	   cellCb->cellInfo.rbgSize  = 2;
   }
   else
   {
	   cellCb->cellInfo.rbgSize  = 1;
   }

   switch(cellCb->cellCfg.bwCfg.ulBw)
   	{
   	  case CTF_BW_RB_6 :
	  	 cellCb->cellInfo.ulBw = 6;
		 break;
      case CTF_BW_RB_15:
   	     cellCb->cellInfo.ulBw = 15;
		 break;
      case CTF_BW_RB_25:
	  	 cellCb->cellInfo.ulBw = 25;
		 break;
      case CTF_BW_RB_50:
   	     cellCb->cellInfo.ulBw = 50;
		 break;
      case CTF_BW_RB_75:
	  	 cellCb->cellInfo.ulBw = 75;
		 break;
      case CTF_BW_RB_100:
	     cellCb->cellInfo.ulBw = 100;
	  	 break;
	  default:
	  	 YS_DBG_ERR((_ysp, "Invalid UL BW %u", cellCb->cellCfg.bwCfg.ulBw));
   	}
/* Filling the band ID for automating the ADI ID */
   cellCb->cellInfo.bandId = cellCb->cellCfg.bwCfg.eUtraBand ;

   switch(cellCb->cellCfg.phichCfg.resource)
	{
	  case CTF_PHICH_RSRC_ONESIXTH:
		 cellCb->cellInfo.phichSixNg = 1;
		 break;
	  case CTF_PHICH_RSRC_HALF:
		 cellCb->cellInfo.phichSixNg = 3;
		 break;
	  case CTF_PHICH_RSRC_ONE:
		 cellCb->cellInfo.phichSixNg = 6;
		 break;
	  case CTF_PHICH_RSRC_TWO:
		 cellCb->cellInfo.phichSixNg = 12;
		break;
	  default:
			YS_DBG_ERR((_ysp, "Invalid NG config %u", cellCb->cellCfg.phichCfg.resource));
			break;
	}

   ysMsSetupUlCqiInfo(cellCb);

   for (i = 0; i < YS_NUM_SUB_FRAMES; i++)
   {
      YS_DBG_INFO((_ysp, "ZEROING OUT the ulEncL1Msgs subframe (%d) \n", i));
      cmMemset((U8*)&(cellCb->ulEncL1Msgs[i]), 0, (sizeof(YsUlEncL1Msgs)));
   }

   cellCb->cellInfo.numAntPorts = YS_MS_GET_NUM_TX_ANT(cellCb->cellCfg.antennaCfg);
   cellCb->cellInfo.cmnTransMode = cellCb->cellInfo.numAntPorts > 1 ? TXDIVERSITY : SINGLEANT;

   RETVALUE(ROK);

} /* End of ysMsUlmPrcCellCfg*/

/*
*
*       Fun:   ysMsUlmPrcDelCellCfg
*
*       Desc:  Delete Cell Configuration
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUlmPrcDelCellCfg
(
YsCellCb        *cellCb
)
#else
PUBLIC S16 ysMsUlmPrcDelCellCfg(cellCb)
YsCellCb        *cellCb;
#endif
{
   /* No persistent memory from Ulm */
   RETVALUE (ROK);
} /* End of ysMsUlmPrcDelCellcfg */

/*
*
*       Fun:   ysMsUlmPrcCellRecfg
*
*       Desc:  Process Cell Reconfiguration
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUlmPrcCellRecfg
(
YsCellCb        *cellCb
)
#else
PUBLIC S16 ysMsUlmPrcCellRecfg(cellCb)
YsCellCb        *cellCb;
#endif
{

   /* TODO */

   RETVALUE (ROK);
} /* End Of ysMsUlmPrcCellRecfg */



#ifndef TFU_UPGRADE
/*
*
*       Fun:   ysMsUlmGetSchInfo
*
*       Desc:  Get the Scheduling Information for given list
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsUlmGetSchInfo
(
U8               type,
CmLListCp       *offsetList,
CmLteTimingInfo *timingInfo,
YsUeLst         *ueLst
)
#else
PUBLIC S16 ysMsUlmGetSchInfo(type, offsetList, timingInfo, ueLst)
U8               type;
CmLListCp       *offsetList;
CmLteTimingInfo *timingInfo;
YsUeLst         *ueLst;
#endif
{
   CmLList   *cmLstEnt;
   YsUeCb    *ueCb;

   TRC2(ysMsUlmGetSchInfo)

   /* Schedule invdividual UE */
   cmLstEnt = offsetList->first;
   while (cmLstEnt != NULLP)
   {
      Bool onPucch = FALSE;
      Bool alreadyOnPucch;
      ueCb = (YsUeCb*)(cmLstEnt->node);
      /* CL WORKAROUND ReEst Fix Start */
      alreadyOnPucch = ueCb->subFrInfo.onPucch;
      /* CL WORKAROUND ReEst Fix End */
      ueCb->subFrInfo.schPres    = TRUE;

      if (type & YS_MS_CQI_REQ)
      {
         onPucch    = TRUE;
         ueCb->subFrInfo.cqiPres    = TRUE;
      }
      else if (type & YS_MS_SR_REQ)
      {
         onPucch   = TRUE;
         ueCb->subFrInfo.srPres    = TRUE;
      }
      else
      {
         ueCb->subFrInfo.srsPres    = TRUE;
      }
      /* If ue is not scheduled on pusch, add to pucch/srs list if needed */
      if (!ueCb->subFrInfo.schReqInfo && !alreadyOnPucch && onPucch)
      {
         ueCb->subFrInfo.onPucch = onPucch;
         ueLst->pucchLst[ueLst->numPucch++] = ueCb;
      }
      if (type & YS_MS_SRS_REQ)
      {
         ueLst->srsLst[ueLst->numSrs++] = ueCb;
      }
      cmLstEnt                     = cmLstEnt->next;
   }

   RETVALUE(ROK);
} /* End of ysMsUlmGetSchInfo */



/*
*
*       Fun:   ysMsUlmGetRecpInfo
*
*       Desc:  Builds Reception Informaiton
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsUlmGetRecpInfo
(
YsCellCb        *cellCb,
TfuRecpReqInfo  *recpReq,
YsUeLst         *ysUeLst
)
#else
PUBLIC S16 ysMsUlmGetRecpInfo(cellCb, recpReq, ysUeLst)
YsCellCb        *cellCb;
TfuRecpReqInfo  *recpReq;
YsUeLst         *ysUeLst;
#endif
{
   CmLList          *cmLstEnt;
   TfuUeRecpReqInfo *recpReqInfo;
   YsUeCb           *ueCb;
   YsSubFrInfo      *ysSubFrInfo;
   /* HARQ */
   U8               rxSduSf;

   cmLstEnt = recpReq->ueRecpReqLst.first;

   YS_DBG_INFO((_ysp, "ysMsUlmGetRecpInfo (): (%d) (%d);\n",
            recpReq->timingInfo.sfn, recpReq->timingInfo.subframe));

   ysSubFrInfo = &cellCb->ulEncL1Msgs[recpReq->timingInfo.subframe].ysSubFrInfo;
   rxSduSf = (recpReq->timingInfo.subframe + 2) % YS_NUM_SUB_FRAMES;

   while (cmLstEnt != NULLP)
   {
      recpReqInfo = (TfuUeRecpReqInfo*)cmLstEnt->node;
      ueCb = ysMsCfgGetUe(cellCb, recpReqInfo->rnti);
      if  (!ueCb)
      {
         if (recpReqInfo->type == TFU_RECP_REQ_MSG3)
         {

            YS_DBG_INFO((_ysp, "ysMsUlmGetRecpInfo Filling in msg3RecpReq (): (%d) (%d) number (%d);\n",
                     recpReq->timingInfo.sfn, recpReq->timingInfo.subframe, ysSubFrInfo->msg3Lst.numMsg3));
            /* copy msg 3 recp requests */
            ysSubFrInfo->msg3Lst.msg3RecpLst[ysSubFrInfo->msg3Lst.numMsg3++] = *recpReqInfo;


            MSPD_DBG("ysMsUlmGetRecpInfo Filling in msg3RecpReq(%d) (%d) at cell time (%d,%d)\
            : number (%d) rxSduSf(%d)\n",
                     recpReq->timingInfo.sfn, recpReq->timingInfo.subframe,
                     cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
                     ysSubFrInfo->msg3Lst.numMsg3, rxSduSf);

            cmLstEnt = cmLstEnt->next;
            continue;
         }
         else
         {
            YS_DBG_ERR((_ysp, "Failed to find ueCb for %d", recpReqInfo->rnti));
            MSPD_ERR("Failed to find ueCb for %d", recpReqInfo->rnti);
            RETVALUE (RFAILED);
         }
      }

      ueCb->subFrInfo.schPres           = TRUE;
      if (recpReqInfo->type == TFU_RECP_REQ_PUCCH)
      {
         if (!ueCb->subFrInfo.schReqInfo)
         {
            /* This assumes that pusch would have appeared
             * earlier */
            ysUeLst->pucchLst[ysUeLst->numPucch++]  = ueCb;
         }
         ueCb->subFrInfo.harqReqInfo = recpReqInfo;
      }
      else
      {
         ysUeLst->puschLst[ysUeLst->numPusch++]  = ueCb;
         ueCb->subFrInfo.schReqInfo = recpReqInfo;

      }

      cmLstEnt = cmLstEnt->next;
   } /* cmLstEnt while */

   RETVALUE(ROK);

} /* End of ysMsUlmGetRecpInfo */


/*
*
*       Fun:   ysMsUlmGetSubFrInfo
*
*       Desc:  Builds Sub Frame Information
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsUlmGetSubFrInfo
(
YsCellCb        *cellCb,
TfuRecpReqInfo  *recpReq
)
#else
PRIVATE S16 ysMsUlmGetSubFrInfo(cellCb, recpReq)
YsCellCb        *cellCb;
TfuRecpReqInfo  *recpReq;
#endif
{
   YsSubFrInfo      *ysSubFrInfo;
   YsUeLst          *ueLst;
   CmLListCp        *offsetList;
   U16               offset;
   CmLteTimingInfo  *timingInfo;

   TRC2(ysMsUlmGetSubFrInfo)

   YS_DBG_INFO((_ysp, "ysMsUlmGetSubFrInfo (): (%d) (%d);\n",
            recpReq->timingInfo.sfn, recpReq->timingInfo.subframe));

   ysSubFrInfo = &cellCb->ulEncL1Msgs[recpReq->timingInfo.subframe].ysSubFrInfo;
   timingInfo  = &recpReq->timingInfo;

   if (ysSubFrInfo->pres)
   {
      YS_DBG_ERR((_ysp, "SubFrmInfo still persists"));
      RETVALUE(RFAILED);
   }

   cmMemset((U8*)ysSubFrInfo, 0, sizeof(YsSubFrInfo));

   ueLst = &ysSubFrInfo->ueLst;

   ysSubFrInfo->prachPres = (cellCb->prachCb.ysPrachPres &
         (0x01 << ((recpReq->timingInfo.sfn * YS_NUM_SUB_FRAMES)
                   + recpReq->timingInfo.subframe) % YS_NUM_PRACH_PRES_ARR))?1:0;

   /* Get Ue PUCCH/PUSCH Info */
   ysMsUlmGetRecpInfo(cellCb, recpReq, ueLst);

   /* Get Sr Scheduling Info*/
   offset = (((timingInfo->sfn * YS_NUM_SUB_FRAMES) +
                  timingInfo->subframe) % YS_SR_PERIOD_80);
   offsetList = &(cellCb->srPeriodList[offset]);

   ysMsUlmGetSchInfo(YS_MS_SR_REQ, offsetList, &recpReq->timingInfo, ueLst);

   /* Get Srs Scheduling Info*/
   offset = (((timingInfo->sfn * YS_NUM_SUB_FRAMES) +
                  timingInfo->subframe) % YS_SRS_PERIOD_320);
   offsetList = &(cellCb->srsPeriodList[offset]);

   ysMsUlmGetSchInfo(YS_MS_SRS_REQ, offsetList, &recpReq->timingInfo, ueLst);


   /* Get CQI Scheduling Info */
   offset = (((timingInfo->sfn * YS_NUM_SUB_FRAMES) +
                  timingInfo->subframe) % YS_CQI_PERIOD_160);
   offsetList = &(cellCb->cqiPeriodList[offset]);

   ysMsUlmGetSchInfo(YS_MS_CQI_REQ, offsetList, &recpReq->timingInfo, ueLst);


   RETVALUE(ROK); /*Always a happy function*/

} /* End of ysMsUlmGetSubFrInfo */
#endif /* TFU_UPGRADE */

/*
*
*       Fun:   ysMsUlmPrcRecpReq
*
*       Desc:  Process Reception request
*
*       Ret:   void
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUlmPrcRecpReq
(
YsCellCb        *cellCb,
TfuRecpReqInfo  *recpReq
)
#else
PUBLIC S16 ysMsUlmPrcRecpReq(cellCb, recpReq)
YsCellCb        *cellCb;
TfuRecpReqInfo  *recpReq;
#endif

{
   PGENMSGDESC   pMsgDesc;
   PULSUBFRDESC  rxVector;
#ifndef TFU_UPGRADE
   YsSubFrInfo   *subFrInfo;
#endif /* TFU_UPGRADE */
   RACHCTRL      *rachCtrl;
/*Radisys*/

   CtfPrachCfgInfo                   *prachCfg;
   YsPrachFddCfgInfo                 *ysPrachInfo;

/*Radisys*/

   TRC2(ysMsUlmPrcRecpReq)

   YS_DBG_INFO((_ysp, "ysMsUlmPrcRecpReq (): (%d) (%d);\n",
            recpReq->timingInfo.sfn, recpReq->timingInfo.subframe));

/*   MSPD_DBG(" Entering PrcRecpReq\n");*/

#ifndef TFU_UPGRADE
   /* Get Subframe Information */
   if(ysMsUlmGetSubFrInfo(cellCb, recpReq) != ROK)
   {
	   YS_DBG_ERR((_ysp, "Failed to get SubFr Info"));
	   RETVALUE(RFAILED);
   }
#endif /* TFU_UPGRADE */

   /* Allocating Memory */
   /* Allocate a message towards Phy using sysCore */
   pMsgDesc = (PGENMSGDESC) ysMsUtlAllocSvsrMsg (FALSE);
   if (pMsgDesc == NULLP)
   {
      YS_DBG_ERR((_ysp, "Unable to allocate memory for TxVector"));
      MSPD_ERR("Unable to allocate memory for TxVector");
      RETVALUE (RFAILED);
   }

   rxVector = (PULSUBFRDESC)(pMsgDesc + 1);

   pMsgDesc->msgType     = PHY_RXSTART_REQ;
   pMsgDesc->phyEntityId = YS_PHY_INST_ID;
   pMsgDesc->msgSpecific = sizeof(ULSUBFRDESC);

#ifndef TFU_UPGRADE
   subFrInfo = &(cellCb->ulEncL1Msgs[recpReq->timingInfo.subframe].ysSubFrInfo);
#endif

   rxVector->numberofChannelDescriptors			= 0;
   rxVector->numberOfCtrlChannelDescriptors		= 0;
   rxVector->numberSrsinSf								= 0;
   cmMemset((U8*)&(rxVector->ulSfrCtrl), 0, sizeof(ULSUBFRCMNCTRL));

#ifndef TFU_UPGRADE
   /* Map Reception Request to PHY API */
   if(ysMsUtlMapRxVector(cellCb, subFrInfo, (PULSUBFRDESC *)rxVector, recpReq->timingInfo) != ROK)
   {
      YS_DBG_ERR((_ysp, "Rx Vector Mapping failed"));
      MSPD_ERR("Rx Vector Mapping failed");
      /* TODO free allocated rxvector memory */
      RETVALUE(RFAILED);
   }
#else
   if(ysMsUtlMapRxVector(cellCb, recpReq, (PULSUBFRDESC )rxVector, recpReq->timingInfo) != ROK)
   {
      YS_DBG_ERR((_ysp, "Rx Vector Mapping failed"));
      MSPD_ERR("Rx Vector Mapping failed");
      /* TODO free allocated rxvector memory */
      RETVALUE(RFAILED);
   }
#endif /* TFU_UPGRADE */

   YS_DBG_PARAM((_ysp, "ysMsUlmPrcRecpReq: numberofChannelDescriptors = (%d) ctrlChannel (%d) \n",
            rxVector->numberofChannelDescriptors, rxVector->numberOfCtrlChannelDescriptors));

      /* ASK for RACH only when subframe == 1 - this is true for configuration idex 3 */
      //if ((cellCb->gotRACH) &&
   YS_DBG_INFO((_ysp, "\n ysMsUlmPrcRecpReq RX recpReqTime (%d) (%d) Cell Time (%d) (%d)\n",
            recpReq->timingInfo.sfn, recpReq->timingInfo.subframe,
            cellCb->timingInfo.sfn, cellCb->timingInfo.subframe));

    /* change */
   rachCtrl = (RACHCTRL *)((U32)rxVector + rxVector->offsetRachCtrlStruct);
   prachCfg            = &cellCb->cellCfg.prachCfg;
   ysPrachInfo         = &ysPrachFddCfgDb[prachCfg->prachCfgIndex];
   rachCtrl->prachEnable					= 0;

if(((ysPrachInfo->sfn == YS_EVEN_SFN) && (recpReq->timingInfo.sfn % 2 == 0)) || (ysPrachInfo->sfn == YS_ALL_SFN))
{
     rachCtrl->prachEnable	 = (cellCb->prachCb.ysPrachPres &
         (0x01 <<  recpReq->timingInfo.subframe))?1:0;

 }

   if (ysOneRachCompleted)
   {
      rachCtrl->prachEnable					= 0;
   }
   /* Rach Predefine Control Information */
     /* rachCtrl    = &cellCb->preDefVal.rachCtrl;*/

/*      prachCfg = &cellCb->cellCfg.prachCfg;*/
      rachCtrl->rootSequenceIndex         = prachCfg->rootSequenceIndex;
      rachCtrl->prachConfigIndex          = prachCfg->prachCfgIndex;

      rachCtrl->highSpeedFlag             = prachCfg->highSpeedFlag;
      rachCtrl->zeroCorrelationZoneConfig = prachCfg->zeroCorrelationZoneCfg;
      rachCtrl->prachFreqOffset           = prachCfg->prachFreqOffset;

    rxVector->offsetCustomFeatures					= 0;

   rxVector->frameNumber 								= recpReq->timingInfo.sfn;
   rxVector->subframeNumber 						   = recpReq->timingInfo.subframe;
   rxVector->subframeType 							 	= ULRX;
   rxVector->antennaPortcount							= 1;

   rxVector->frameNumber 								= recpReq->timingInfo.sfn;
   rxVector->subframeNumber 						   = recpReq->timingInfo.subframe;
   rxVector->subframeType 							 	= ULRX;
   rxVector->antennaPortcount							= 1;
   /* there should not be any rxVector already present for this subframe */
   if(cellCb->ulEncL1Msgs[recpReq->timingInfo.subframe].rxVector != NULLP)
   {
      YS_DBG_ERR((_ysp, "Fatal error, RX vector is still there... exiting\n"));
      MSPD_ERR("Fatal error, RX vector is still there... exiting (%u,%u)\n",
         recpReq->timingInfo.sfn, recpReq->timingInfo.subframe);
      MSPD_ERR("Losing Memory Need to cleanup\n");
   }

   /* Save rxVector */
   cellCb->ulEncL1Msgs[recpReq->timingInfo.subframe].rxVector = pMsgDesc;

   RETVALUE(ROK);
} /* End of ysMsUlmPrcRecpReq */
#if 0
#ifdef ANSI
PRIVATE S16 ysMsPrachPresent
(
TfuRecpReqInfo  *recpReq,
RACHCTRL       *rachCtrl
)
#else
PRIVATE S16 ysMsPrachPresent(rxSduInd, rachCtrl)
TfuRecpReqInfo  *recpReq;
RACHCTRL       *rachCtrl;
#endif
{
    switch(rachCtrl->prachConfigIndex)
    {
       case 0:
	     if ((recpReq->timingInfo.subframe == 1) && (recpReq->timingInfo.sfn % 2 == 0))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 1:
	     if ((recpReq->timingInfo.subframe == 4) && (recpReq->timingInfo.sfn % 2 == 0))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 2:
	     if ((recpReq->timingInfo.subframe == 7) && (recpReq->timingInfo.sfn % 2 == 0))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 3:
	     if ((recpReq->timingInfo.subframe == 1))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 4:
	     if ((recpReq->timingInfo.subframe == 4))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 5:
	     if ((recpReq->timingInfo.subframe == 7))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 6:
	     if ((recpReq->timingInfo.subframe == 1) ||
			     (recpReq->timingInfo.subframe == 6))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 7:
	     if ((recpReq->timingInfo.subframe == 2) ||
			     (recpReq->timingInfo.subframe == 7))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 8:
	     if ((recpReq->timingInfo.subframe == 3) ||
			     (recpReq->timingInfo.subframe == 8))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 9:
	     if ((recpReq->timingInfo.subframe == 1) ||
			     (recpReq->timingInfo.subframe == 4) ||
			     (recpReq->timingInfo.subframe == 7))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 10:
	     if ((recpReq->timingInfo.subframe == 2) ||
			     (recpReq->timingInfo.subframe == 5) ||
			     (recpReq->timingInfo.subframe == 8))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 11:
	     if ((recpReq->timingInfo.subframe == 3) ||
			     (recpReq->timingInfo.subframe == 6) ||
			     (recpReq->timingInfo.subframe == 9))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 12:
	     if (((recpReq->timingInfo.subframe % 2) == 0))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 13:
	     if (((recpReq->timingInfo.subframe % 2) == 1))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       case 14:
	     rachCtrl->prachEnable = 1;
	     break;
       case 15:
	     if ((recpReq->timingInfo.subframe == 9) && (recpReq->timingInfo.sfn % 2 == 0))
	     {
		     rachCtrl->prachEnable = 1;
	     }else
	     {
		     rachCtrl->prachEnable = 0;
	     }
	     break;
       default:
	     MSPD_DBG("Invalid Prach config Index %d\n",rachCtrl->prachConfigIndex);
    }
   RETVALUE(ROK);
} /* End of ysMsUlmPrcRecpReq */
#endif

#ifdef ANSI
PUBLIC S16 ysMsPrcUciInd
(
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb
)
#else
PUBLIC S16 ysMsPrcUciInd(rxSduInd, cellCb)
PRXSDUIND       rxSduInd;
YsCellCb        *cellCb;
#endif
{
   TfuSrIndInfo    *srIndInfo;
   TfuHqIndInfo    *harqIndInfo;
   U32             pucchFormat;
   CmLteTimingInfo timingInfo;
   U8              *hqBuf;
   CmLteRnti       rnti;
   U32 numHarqBits;

   TRC2(ysMsPrcUciInd)

   ysMsGetRntiFrmChanId(cellCb, &rnti,rxSduInd->subFrameNum, rxSduInd->channelId);

   if (rxSduInd->srdetected)
   {
      srIndInfo = cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].srIndInfo;
      ysMsPrcSrInd(rxSduInd, cellCb, srIndInfo);
   }

   pucchFormat = rxSduInd->pucchType;
   timingInfo.sfn = rxSduInd->frameNum;
   timingInfo.subframe = rxSduInd->subFrameNum;
   if (pucchFormat == FORMAT1A || pucchFormat == FORMAT1B || pucchFormat == FORMAT2A || pucchFormat == FORMAT2B)
   {
      numHarqBits = 1;
      if (pucchFormat == FORMAT1B || pucchFormat == FORMAT2B)
        numHarqBits = 2;
      harqIndInfo = cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].harqIndInfo;
      hqBuf = (U8 *)&rxSduInd->pRxSdu;
     if (!rxSduInd->pucchDetected)
     {
       static U32 dtxCnt;
       ++dtxCnt;
       if ((cellCb->timingInfo.sfn % 10 == 0)&& (cellCb->timingInfo.subframe == 0))
       {
         MSPD_DBG("Recieved DTX @(%u,%u) ueId(%u) cnt(%u)\n",
           cellCb->timingInfo.sfn, cellCb->timingInfo.subframe, rnti, dtxCnt);
       }
     }
     else
     {
        ysMsPrcHarqInd(cellCb, harqIndInfo,&timingInfo,hqBuf,FALSE,rnti, numHarqBits);
     }
   }
   if (pucchFormat == FORMAT2 || pucchFormat == FORMAT2A || pucchFormat == FORMAT2B)
   {
     if (!rxSduInd->pucchDetected)
     {
        static U32 cqiDetectFailCnt = 0;
       ++cqiDetectFailCnt;
       MSPD_DBG("CQIDetectFail @(%u,%u) ueId(%u) cnt(%u)\n",
         cellCb->timingInfo.sfn, cellCb->timingInfo.subframe, rnti,
         cqiDetectFailCnt);
     }
     else
     {
#ifdef YSMS_RLF_DETCT
        {
           S32 pucchSnr = 0;
           pucchSnr = (rxSduInd->ul_CQI - 128);
           pucchSnr = pucchSnr /2;
           if (pucchSnr <= -6)
           {
              MSPD_DBG("[%d]DL CQI Dropped (%d,%d) at CL\n", rnti, rxSduInd->frameNum, rxSduInd->subFrameNum);
              RETVALUE(ROK);
           }
        }
#endif

        if (rxSduInd->cqiPmiConf & 0x3)
           ysMsPrcDlCqiInd(cellCb, rnti, timingInfo, 4, (U8*)&rxSduInd->pRxSdu);
        else
           MSPD_ERR("invalid numCqiPmiBytes. Cfn: %x\n", rxSduInd->cqiPmiConf);
     }
   }
    RETVALUE(ROK);
}


/*
*
*       Fun:   ysMsUlmSduInd
*
*       Desc:  Handles the SDU Indication
*
*       Ret:   void
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUlmSduInd
(
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb
)
#else
PUBLIC S16 ysMsUlmSduInd(rxSduInd, cellCb)
PRXSDUIND       rxSduInd;
YsCellCb        *cellCb;
#endif
{
   TfuDatIndInfo *datIndInfo;
   TfuCrcIndInfo *crcIndInfo;
   CmLteRnti     rnti;
   YsUeCb        *ueCb;
   Bool          isDummyRxSdu = FALSE; 

   TRC2(ysMsUlmSduInd)

   datIndInfo = NULLP;
   crcIndInfo = NULLP;

   YS_DBG_INFO((_ysp, "Received RX SDU from PHY : length=%d for TTI(%d, %d) type (%d) at cell(%d, %d) status (%d)\n",
				rxSduInd->numBitsRx,rxSduInd->frameNum, rxSduInd->subFrameNum, rxSduInd->chanType,
				cellCb->timingInfo.sfn, cellCb->timingInfo.subframe, rxSduInd->status));

   /* Update the timing advance error average values in ueCb */
   ysMsGetRntiFrmChanId(cellCb, &rnti, rxSduInd->subFrameNum, rxSduInd->channelId);
   ueCb = ysMsCfgGetUe(cellCb, rnti);
   if (ueCb)
   {
      ueCb->timingAdvErrInfo.mSetDefaults = 0;
      ueCb->timingAdvErrInfo.mErrAvgCh[0] = rxSduInd->mErrAvgCh[0];
      ueCb->timingAdvErrInfo.mErrAvgCh[1] = rxSduInd->mErrAvgCh[1];
      ueCb->timingAdvErrInfo.mErrExpo = rxSduInd->mErrExpo;
   }
   if ((rxSduInd->status == TIMEOUT_RXSTART_REQ) || (rxSduInd->status == INVALID_TX_VECTOR))
   {
      ++delayedApiCnt;
	  isDummyRxSdu = TRUE;
      MSPD_DBG("Delayed API RXSDU chanType (%u) cnt(%u) status(%u)\n", 
	  	rxSduInd->chanType, delayedApiCnt, rxSduInd->status);
   }
   if(rxSduInd->chanType == PUSCH)
   {
      static U32 crcErrCnt = 0;
#ifdef HARQ_STATISTICS
   totl_ulTbs ++;
#endif
      if (rxSduInd->status)
      {
         CmLteRnti rnti;
         ysMsGetRntiFrmChanId(cellCb, &rnti, rxSduInd->subFrameNum, rxSduInd->channelId);
         MSPD_DBG("Rcvd PUSCH RX SDU at PHY at (%d,%d) rnti %u ta %u status %d SNR %d\n",
          rxSduInd->frameNum, rxSduInd->subFrameNum, rnti, (U16)rxSduInd->timingAdv,
          rxSduInd->status, (rxSduInd->ul_CQI - 128)/2);
          ++crcErrCnt;
      }

      if ((cellCb->timingInfo.sfn % 10 ==0) && cellCb->timingInfo.subframe == 0)
	  	{
 	         MSPD_DBG("Received PUSCH RX SDU at cell time(%d,%d) Total CRC Errors %d\n",cellCb->timingInfo.sfn,
                      cellCb->timingInfo.subframe, crcErrCnt);

      }

   	YS_DBG_FATAL((_ysp, "Received RX SDU from PHY : length=%d for TTI(%d, %d) type (%d) at cell(%d, %d) status (%d)\n",
         rxSduInd->numBitsRx,rxSduInd->frameNum, rxSduInd->subFrameNum, rxSduInd->chanType,
		   cellCb->timingInfo.sfn, cellCb->timingInfo.subframe, rxSduInd->status));

      if (!rxSduInd->status)
      {
         datIndInfo = cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].tfuDatIndInfo;

         /* Process UL Sch Sdu Indication. add data to TfuDatIndication
                     for the corresponding subframe  */
         if((ysMsPrcUlSchSduInd(rxSduInd, cellCb, datIndInfo)) != ROK)
         {
            YS_DBG_ERR((_ysp, "ysMsUlmSduInd(): Processing of ysMsPrcUlSchSduInd failed\n"));
            MSPD_ERR("Processing of ysMsPrcUlSchSduInd failed\n");
            RETVALUE(RFAILED);
         } /* end of if statement */
      }

      /* Update the CRC Indication structure */
      crcIndInfo = cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].tfuCrcIndInfo;
      if((ysMsUpdCrcInd(rxSduInd, cellCb, crcIndInfo)) != ROK)
      {
          YS_DBG_ERR((_ysp, "ysMsUlmSduInd(): Processing of ysMsUpdCrcInd failed\n"));
          MSPD_ERR("Processing of ysMsUpdCrcInd failed\n");
          RETVALUE(RFAILED);
      } /* end of if statement */

#ifndef YS_MS_NO_TA
      if (!isDummyRxSdu)
      {
         ysMsPrcTmAdvInd(rxSduInd, cellCb,
            &cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].tmAdvIndInfo);
      }
#endif
#ifndef YS_MS_NO_ULCQI
      if (!isDummyRxSdu)
      {
         ysMsPrcUlCqiInd(rxSduInd, cellCb,
            &cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].ulCqiIndInfo);
      }
#endif

#ifndef TFU_UPGRADE
      cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].ysSubFrInfo.numRxSduRcvd++;
#endif
   } /* end of (rxSduInd->chanType == UL_SCH) */
#ifndef DLHQ_RTT_OPT
   else if (rxSduInd->chanType == PUCCH)
   {
      if ((ysMsUlmPrcPucch(rxSduInd, cellCb)) != ROK)
      {
         YS_DBG_ERR((_ysp, "ysMsUlmPrcPucch(): Processing of PUCCH failed\n"));
         MSPD_ERR("ysMsUlmPrcPucch(): Processing of PUCCH failed\n");
         RETVALUE(RFAILED);
      } /* end of if statement */
   }
#endif
   else
   {
      YS_DBG_ERR((_ysp, "ysMsUlmSduInd(): Invalid channel type(%d) received\n"
               ,rxSduInd->chanType));
      RETVALUE(RFAILED);

   } /* end of else */

   RETVALUE(ROK);

} /* end of ysMsUlmSduInd */

/*
*
*       Fun:   ysMsUlmRxStartCnf
*
*       Desc:  Handles the Rx Start confirmation
*
*       Ret:   void
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PUBLIC Void ysMsUlmRxStartCnf
(
PINITIND        rxStartCnf,
YsCellCb        *cellCb
)
#else
PUBLIC Void ysMsUlmRxStartCnf(rxStartCnf, cellCb)
PINITIND        rxStartCnf;
YsCellCb        *cellCb;
#endif
{

   TRC2(ysMsUlmRxStartCnf)

   /* Check the Status. If the status is NOK then
      print the error else do nothing*/
   if(rxStartCnf->status != 0)
   {
      /* Print the SubFrame number and the error information */
      YS_DBG_INFO((_ysp, "ERROR SubFrame : %d\n",rxStartCnf->reserved));
      ysMsUtlPrntErrInfo(rxStartCnf->status);
   } /* end of if statement */

   RETVOID;

} /* end of ysMsUlmRxStartCnf */

/*
*
*       Fun:   ysMsUlmRxStartInd
*
*       Desc:  Handles the Rx Start Indication
*
*       Ret:   void
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsUlmRxStartInd
(
PMSGIND        rxStartInd,
YsCellCb        *cellCb
)
#else
PUBLIC S16 ysMsUlmRxStartInd(rxStartInd, cellCb)
PMSGIND        rxStartInd;
YsCellCb        *cellCb;
#endif
{


   TRC2(ysMsUlmRxStartInd)

   /* Check the Status. If the status is NOK then
      print debug info and return */
   if(rxStartInd->status != 0)
   {
      /* Print the SubFrame number and the error information */
      YS_DBG_INFO((_ysp, "ERROR SubFrame : %d\n",rxStartInd->subFrameNum));
      ysMsUtlPrntErrInfo(rxStartInd->status);
   } /* end of if statement */

   YS_FREE_SDU(cellCb->ulEncL1Msgs[rxStartInd->subFrameNum].tfuCrcIndInfo);
   YS_FREE_SDU(cellCb->ulEncL1Msgs[rxStartInd->subFrameNum].tfuDatIndInfo);
   YS_FREE_SDU(cellCb->ulEncL1Msgs[rxStartInd->subFrameNum].rachIndInfo);
   YS_FREE_SDU(cellCb->ulEncL1Msgs[rxStartInd->subFrameNum].dlCqiIndInfo);
   YS_FREE_SDU(cellCb->ulEncL1Msgs[rxStartInd->subFrameNum].ulCqiIndInfo);
   YS_FREE_SDU(cellCb->ulEncL1Msgs[rxStartInd->subFrameNum].harqIndInfo);
   YS_FREE_SDU(cellCb->ulEncL1Msgs[rxStartInd->subFrameNum].srIndInfo);
   YS_FREE_SDU(cellCb->ulEncL1Msgs[rxStartInd->subFrameNum].tmAdvIndInfo);

   /* HARQ */
   cellCb->isCrcExptd[rxStartInd->subFrameNum] = FALSE;
#ifdef RG_ULSCHED_AT_CRC
   cellCb->crcSent[rxStartInd->subFrameNum] = FALSE;
#endif

	YS_DBG_INFO((_ysp, "RXSTART_IND at (%d) (%d) RXSTART_IND_time (%d) (%d) :\n", cellCb->timingInfo.sfn,
				cellCb->timingInfo.subframe, rxStartInd->frameNumber, rxStartInd->subFrameNum));
   /* Data is about to come. Initialize the data structure */

   /*Invoke the Simulator for the UL handling */

   RETVALUE(ROK);
} /* end of ysMsUlmRxStartInd */


#ifndef TFU_UPGRADE
#ifdef ANSI
PRIVATE Void ysMsResetUeLst
(
U16    *countRef,
YsUeCb *ueLst[],
U8     subframe
)
#else
PRIVATE Void ysMsResetUeLst(countRef, ueLst, subframe)
U16    *countRef;
YsUeCb *ueLst[];
U8     subframe;
#endif
{
   U16  count = *countRef;
   while (count--)
   {
      YsUeCb         *ueCb      = ueLst[count];
      YsUeSubFrInfo  *subFrInfo = &ueCb->ueSubFrInfo[subframe];
      subFrInfo->schPres = 0;
      subFrInfo->onPucch = FALSE;
   }
   *countRef = 0;
}
#endif

/*
*
*       Fun:   ysMsUlmRxEndIndAtRxSdu
*
*       Desc:  Handles the Rx End Indication at CRC reception
*
*       Ret:   void
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PUBLIC Void ysMsUlmRxEndIndAtRxSdu
(
CmLteTimingInfo rxEndTimingInfo,
YsCellCb        *cellCb
)
#else
PUBLIC Void ysMsUlmRxEndIndAtRxSdu(rxEndTimingInfo, cellCb)
CmLteTimingInfo rxEndTimingInfo;
YsCellCb        *cellCb;
#endif
{
   TfuCrcIndInfo     *crcIndInfo;
   TfuDatIndInfo     *datIndInfo;
#ifndef TFU_UPGRADE
   TfuDlCqiIndInfo   *dlCqiIndInfo;
#else
   TfuRawCqiIndInfo  *dlCqiIndInfo;
#endif
   TfuUlCqiIndInfo   *ulCqiIndInfo;
#ifndef DLHQ_RTT_OPT
   TfuHqIndInfo      *harqIndInfo;
#endif
   TfuSrIndInfo      *srIndInfo;
   TfuTimingAdvIndInfo *tmAdvIndInfo;
   TfuPucchDeltaPwrIndInfo *pucchDeltaPwrIndInfo;

#ifndef TFU_UPGRADE
   YsSubFrInfo       *ysSubFrInfo;
   YsUeLst           *ueLst;
   YsUeLst          *ueLst;
#endif /* TFU_UPGRADE */

#ifdef MSPD_MLOG_NEW
   volatile int      t;
#endif


   TRC2(ysMsUlmRxEndIndAtRxSdu)

    /* HARQ_DBG: timing */
   YS_DBG_INFO((_ysp, "RXEND_IND at (%d) (%d) RXEND_IND_time (%d) (%d) : \n", cellCb->timingInfo.sfn,
				cellCb->timingInfo.subframe, rxEndTimingInfo.sfn, rxEndTimingInfo.subframe));


   crcIndInfo = cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].tfuCrcIndInfo;
   datIndInfo = cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].tfuDatIndInfo;
   dlCqiIndInfo = cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].dlCqiIndInfo;
   ulCqiIndInfo = cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].ulCqiIndInfo;
#ifndef DLHQ_RTT_OPT
   harqIndInfo = cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].harqIndInfo;
#endif
   srIndInfo = cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].srIndInfo;
   tmAdvIndInfo = cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].tmAdvIndInfo;
   pucchDeltaPwrIndInfo = cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].pucchDeltaPwrIndInfo;
#ifndef TFU_UPGRADE
   ysSubFrInfo = &cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].ysSubFrInfo;
   /* - Ue Specific Sub Frame Info -*/
   ueLst = &ysSubFrInfo->ueLst;
   ysMsResetUeLst(&ueLst->numPusch, ueLst->puschLst, rxEndTimingInfo.subframe);
   ysMsResetUeLst(&ueLst->numPucch, ueLst->pucchLst, rxEndTimingInfo.subframe);
   ysMsResetUeLst(&ueLst->numSrs, ueLst->srsLst, rxEndTimingInfo.subframe);
   /* - Cell Specific Sub Frame Info -*/
   cmMemset((U8*)ysSubFrInfo, 0, sizeof(YsSubFrInfo));
#endif

   /* Send the CRC Indication to MAC for the corresponding subframe */
   if (crcIndInfo != NULLP)
   {
      if(crcIndInfo->crcLst.count > 0)
      {
         
         /*71827: If a second CRC Indication for the subframe is erroneously being generated to MAC, stop it*/
      	 if(cellCb->crcSent[cellCb->timingInfo.subframe] == TRUE)
      	 {
      	    MSPD_ERR("CRC already sent for this subframe\n");
			YS_FREE_SDU(crcIndInfo);
      	 }
	  	 else
	  	 {
#ifdef MSPD_MLOG_NEW
            t = MacGetTick();
#endif
     	    ysSendCrcToMac(cellCb, crcIndInfo);
        	cellCb->isCrcExptd[cellCb->timingInfo.subframe] = FALSE;
#ifdef RG_ULSCHED_AT_CRC
         	cellCb->crcSent[cellCb->timingInfo.subframe] = TRUE;
#endif

#ifdef MSPD_MLOG_NEW
          	MLogTask(PID_CRC_IND_REAL, RESOURCE_LARM, t, MacGetTick());
#endif
	  	 }
      }
      else
      {
         YS_FREE_SDU(crcIndInfo);
      }/* end of else part */
   }

   /* Send the Data Indication to MAC for the corresponding subframe */
   if (datIndInfo != NULLP)
   {
      if(datIndInfo->datIndLst.count > 0)
      {
         YsUiTfuDatInd(&ysCb.macTfuSap->sapPst, ysCb.macTfuSap->suId, datIndInfo);
      } /* end of if statement */
      else
      {
         YS_FREE_SDU(datIndInfo);
      }/* end of else part */
   }


    /* Send the DL CQI Indication
     to MAC for the corresponding subframe */
   if (dlCqiIndInfo != NULLP)
   {
#ifndef TFU_UPGRADE
      if(dlCqiIndInfo->dlCqiRptsLst.count > 0 )
      {
         YsUiTfuDlCqiInd(&cellCb->schTfuSap->sapPst, cellCb->schTfuSap->suId, dlCqiIndInfo);
      } /* end of if statement */
#else
      if(dlCqiIndInfo->rawCqiRpt.count > 0 )
      {
         YsUiTfuRawCqiInd(&cellCb->schTfuSap->sapPst, cellCb->schTfuSap->suId, dlCqiIndInfo);
      } /* end of if statement */
#endif
      else
      {
         YS_FREE_SDU(dlCqiIndInfo);
      }/* end of else part */
   }

   /* Send the DL CQI Indication
     to MAC for the corresponding subframe */
   if (ulCqiIndInfo != NULLP)
   {
      if(ulCqiIndInfo->ulCqiRpt.count > 0 )
      {
         YsUiTfuUlCqiInd(&cellCb->schTfuSap->sapPst, cellCb->schTfuSap->suId, ulCqiIndInfo);
      } /* end of if statement */
      else
      {
         YS_FREE_SDU(ulCqiIndInfo);
      }/* end of else part */
   }

#ifndef DLHQ_RTT_OPT
    /* Send the HARQ Indication
     to MAC for the corresponding subframe */
   if (harqIndInfo != NULLP)
   {
      if(harqIndInfo->hqIndLst.count > 0 )
      {
         YsUiTfuHqInd(&cellCb->schTfuSap->sapPst, cellCb->schTfuSap->suId, harqIndInfo);
      }
      else
      {
         YS_FREE_SDU(harqIndInfo);
      }/* end of else part */
   }
#endif
    /* Send the SR Indication
     to MAC for the corresponding subframe */
   if (srIndInfo != NULLP)
   {
      if(srIndInfo->srLst.count > 0 )
      {
         YsUiTfuSrInd(&cellCb->schTfuSap->sapPst, cellCb->schTfuSap->suId, srIndInfo);
         //MSPD_DBG("SR Ind processed by MAC\n");

      } /* end of if statement */
      else
      {
         YS_FREE_SDU(srIndInfo);
      }/* end of else part */
   }

   if (tmAdvIndInfo != NULLP)
   {
      if (tmAdvIndInfo->timingAdvLst.count > 0 )
      {
         YsUiTfuTimingAdvInd(&cellCb->schTfuSap->sapPst, cellCb->schTfuSap->suId, tmAdvIndInfo);
      }
      else
      {
         YS_FREE_SDU(tmAdvIndInfo);
      }
   }

   if (pucchDeltaPwrIndInfo != NULLP)
   {
      if (pucchDeltaPwrIndInfo->pucchDeltaPwrLst.count > 0)
      {
         YsUiTfuPucchDeltaPwrInd(&cellCb->schTfuSap->sapPst, cellCb->schTfuSap->suId,
		 	pucchDeltaPwrIndInfo);
      }
      else
      {
         YS_FREE_SDU(pucchDeltaPwrIndInfo);
      }
   }
   cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].tfuCrcIndInfo = NULLP;
   cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].tfuDatIndInfo = NULLP;
   cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].dlCqiIndInfo = NULLP;
   cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].ulCqiIndInfo = NULLP;
#ifndef DLHQ_RTT_OPT
   cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].harqIndInfo = NULLP;
#endif
   cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].srIndInfo = NULLP;
   cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].tmAdvIndInfo = NULLP;
   cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].pucchDeltaPwrIndInfo = NULLP;
   cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].rxSduPres = FALSE;
#ifndef TFU_UPGRADE
   cellCb->ulEncL1Msgs[rxEndTimingInfo.subframe].ysSubFrInfo.numRxSduRcvd = 0;
#endif
   RETVOID;

} /* end of ysMsUlmRxEndIndAtRxSdu */

/*
*
*       Fun:   ysMsUlmRachRxEndInd
*
*       Desc:  Handles the Rx End Indication
*
*       Ret:   void
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PUBLIC Void ysMsUlmRachRxEndInd
(
PMSGIND        rxEndInd,
YsCellCb        *cellCb
)
#else
PUBLIC Void ysMsUlmRachRxEndInd(rxEndInd, cellCb)
PMSGIND        rxEndInd;
YsCellCb        *cellCb;
#endif
{

   TfuRaReqIndInfo   *rachIndInfo;

   YS_DBG_INFO((_ysp, "RXEND_IND at (%d) (%d) RXEND_IND_time (%d) (%d) : \n", cellCb->timingInfo.sfn,
				cellCb->timingInfo.subframe, rxEndInd->frameNumber, rxEndInd->subFrameNum));

   rachIndInfo = cellCb->ulEncL1Msgs[rxEndInd->subFrameNum].rachIndInfo;
   /* Send the RACH Indication
     to MAC for the corresponding subframe */
   if (rachIndInfo != NULLP)
   {
      if((rachIndInfo->nmbOfRaRnti != 0) && !ysOneRachCompleted)
      {

		 YS_DBG_INFO((_ysp, "Sending RA REQ to MAC at : (%d) (%d) numRnti (%d) raRnti (%d) numPreamble (%d)\n",
                  cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
                  rachIndInfo->nmbOfRaRnti,
                  rachIndInfo->rachInfoArr[0].raRnti,
                  rachIndInfo->rachInfoArr[0].numRaReqInfo));

         YsUiTfuRaReqInd(&cellCb->schTfuSap->sapPst, cellCb->schTfuSap->suId, rachIndInfo);
		 /* ysOneRachCompleted = 1;*/
      } /* end of if statement */
      else
      {
         YS_DBG_INFO((_ysp, "NO RA REQ to MAC freeup the memeory at : (%d) (%d) \n",
                  cellCb->timingInfo.sfn, cellCb->timingInfo.subframe));
         YS_FREE_SDU(rachIndInfo);
      }
   }
   cellCb->ulEncL1Msgs[rxEndInd->subFrameNum].rachIndInfo = NULLP;

   RETVOID;
} /* end of ysMsUlmRachRxEndInd */

/*
*
*       Fun:   ysMsUlmRxEndInd
*
*       Desc:  Handles the Rx End Indication
*
*       Ret:   void
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PUBLIC Void ysMsUlmRxEndInd
(
PMSGIND        rxEndInd,
YsCellCb        *cellCb
)
#else
PUBLIC Void ysMsUlmRxEndInd(rxEndInd, cellCb)
PMSGIND        rxEndInd;
YsCellCb        *cellCb;
#endif
{

   CmLteTimingInfo rxEndTiming;
   CmLteTimingInfo rxSduSf;
#ifndef TFU_UPGRADE
   U8              numPuschCh;
#endif


   rxEndTiming.sfn = rxEndInd->frameNumber;
   rxEndTiming.subframe = rxEndInd->subFrameNum;

#ifndef TFU_UPGRADE
   numPuschCh = cellCb->ulEncL1Msgs[rxEndTiming.subframe].ysSubFrInfo.numCh - cellCb->ulEncL1Msgs[rxEndTiming.subframe].ysSubFrInfo.numCtrl;
   if (numPuschCh != cellCb->ulEncL1Msgs[rxEndTiming.subframe].ysSubFrInfo.numRxSduRcvd)   {
     stop_printf("Missed some PUSCH SDU @ rxEndtime(%d,%d) @ celltime (%u,%u): exp(%u) rcvd(%u)\n",
         rxEndTiming.sfn, rxEndTiming.subframe,
         cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
         numPuschCh,cellCb->ulEncL1Msgs[rxEndTiming.subframe].ysSubFrInfo.numRxSduRcvd);
   }
#endif

   rxSduSf = rxEndTiming;
   YS_INCR_TIMING_INFO(rxSduSf,2);
   ysMsUlmRxEndIndAtRxSdu(rxEndTiming, cellCb);
   RETVOID;
} /* end of ysMsUlmRxEndInd */

/*
*
*       Fun:   ysMsPrcUlSchSduInd
*
*       Desc:  Processes the UL_SCH SDU Indication
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsPrcUlSchSduInd
(
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb,
TfuDatIndInfo   *datIndInfo
)
#else
PRIVATE S16 ysMsPrcUlSchSduInd(rxSduInd, cellCb, datIndInfo)
PRXSDUIND       rxSduInd;
YsCellCb        *cellCb;
TfuDatIndInfo   *datIndInfo;
#endif
{
   S16         ret;
   TfuDatInfo *datInfo;
   U16         lenInBytes;
   Data        incoming_lcid = 0;
   volatile U32 t = GetTIMETICK();

   TRC2(ysMsPrcUlSchSduInd)

   ret = ROK;
   datInfo = NULLP;

  YS_DBG_INFO((_ysp, "\n Received PUSCH SDU numbits (%d) \n", rxSduInd->numBitsRx));
   if(datIndInfo == NULLP)
   {
      /*This is a normal scenario, we are not going to allocate the structure
       * unless we need it.We are here means that this is the first SDU
       * recieved for this SF. Time to allocate the structure */

      /* Allocate memory for Data Indication Info */
       ret = ysUtlAllocEventMem((Ptr *)&datIndInfo,
               sizeof(TfuDatIndInfo));
       if(ret == RFAILED)
       {
          YS_DBG_ERR((_ysp, "ysMsPrcUlSchSduInd(): Memory allocation failed for DatInd\n"));
          MSPD_ERR("Memory allocation failed for DatInd\n");
          RETVALUE(RFAILED);
       } /* end of if statement */
       cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].tfuDatIndInfo = datIndInfo;

       /* Initialize the Data Indication List */
       cmLListInit(&(datIndInfo->datIndLst));

       datIndInfo->cellId = ysCb.cellId;
       datIndInfo->timingInfo.sfn = rxSduInd->frameNum;
       datIndInfo->timingInfo.subframe = rxSduInd->subFrameNum;
   } /* end of datIndInfo == NULLP */
   {
      lenInBytes = (U16)(rxSduInd->numBitsRx >> 3);

      if (cmGetMem(&datIndInfo->memCp, sizeof(TfuDatInfo),
            (Ptr *)&datInfo) != ROK)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcUlSchSduInd(): cmGetMem returned failure\n"));
         MSPD_ERR("cmGetMem returned failure\n");
         RETVALUE (RFAILED);
      } /* end of if statement */



	  ysMsGetRntiFrmChanId(cellCb, &(datInfo->rnti), rxSduInd->subFrameNum, rxSduInd->channelId);
      YS_MS_ALLOC_BUF(datInfo->mBuf);
      if(NULLP == datInfo->mBuf)
      {
         YS_DBG_ERR((_ysp, "SGetMsg returned failure\n"));
         MSPD_ERR("SGetMsg returned failure\n");
         RETVALUE (RFAILED);
      }

      ret = SAddPstMsgMult((Data*)&(rxSduInd->pRxSdu), lenInBytes, datInfo->mBuf);
      if(ret != ROK)
      {
         YS_DBG_ERR((_ysp, "SAddPstMsgMult returned failure\n"));
         YS_MS_FREE_BUF(datInfo->mBuf);
         RETVALUE (RFAILED);
      } /* end of if statement */

	  YS_DBG_FATAL((_ysp, "Received PUSCH SDU (%x)\n", ((U8)rxSduInd->pRxSdu)));

      SExamMsg (&incoming_lcid, datInfo->mBuf, 1);

      datInfo->lnk.node = (PTR)datInfo;

      cmLListAdd2Tail(&(datIndInfo->datIndLst), &(datInfo->lnk));

   } /* end of else statement */
#ifdef MSPD_MLOG
   MLogTask(PID_ysMsPrcUlSchSduInd, RESOURCE_LARM, t, GetTIMETICK());
#endif

   RETVALUE(ret);
} /* end of ysMsPrcUlSchSduInd */

/*
*
*       Fun:   ysMsUpdCrcInd
*
*       Desc:  Update Crc Indication
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsUpdCrcInd
(
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb,
TfuCrcIndInfo   *crcIndInfo
)
#else
PRIVATE S16 ysMsUpdCrcInd(rxSduInd, cellCb, crcIndInfo)
PRXSDUIND       rxSduInd;
YsCellCb        *cellCb;
TfuCrcIndInfo   *crcIndInfo;
#endif
{
   S16         ret;
   TfuCrcInfo  *crcInfo;


   TRC2(ysMsUpdCrcInd)

   ret = ROK;
   crcInfo = NULLP;

   if(NULLP == crcIndInfo)
   {
      /* Allocate memory for the structures corresponding to the sub frame.
         rxStartInd->reserved has the subframe number */
      /* Allocate memory for CRC Indication Info */
       ret = ysUtlAllocEventMem((Ptr *)&crcIndInfo,
               sizeof(TfuCrcIndInfo));
       if(ret == RFAILED)
       {
          YS_DBG_ERR((_ysp, "ysMsUpdCrcInd(): Memory allocation failed for CrcInd\n"));
          MSPD_ERR("ysMsUpdCrcInd(): Memory allocation failed for CrcInd\n");
          RETVALUE(RFAILED);
       } /* end of if statement */

       cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].tfuCrcIndInfo = crcIndInfo;
       cmLListInit(&(crcIndInfo->crcLst));

       crcIndInfo->cellId = ysCb.cellId;
       crcIndInfo->timingInfo.sfn = rxSduInd->frameNum;
       crcIndInfo->timingInfo.subframe = rxSduInd->subFrameNum;
   }
   {
      if (cmGetMem(&crcIndInfo->memCp, sizeof(TfuCrcIndInfo),
            (Ptr *)&crcInfo) != ROK)
      {
         YS_DBG_ERR((_ysp, "ysMsUpdCrcInd(): cmGetMem returned failure\n"));
         MSPD_ERR("cmGetMem returned failure\n");
         RETVALUE (RFAILED);
      } /* end of if statement */



	  ysMsGetRntiFrmChanId(cellCb, &(crcInfo->rnti), rxSduInd->subFrameNum, rxSduInd->channelId);

      if(rxSduInd->status)
      {
          crcInfo->isFailure = TRUE;
#ifdef HARQ_STATISTICS
	  ulNacks++;
#endif
      } /* end of if statement */
      else
      {
          crcInfo->isFailure = FALSE;
#ifdef HARQ_STATISTICS
	  ulAcks++;
#endif
      } /* end of else part */

      crcInfo->lnk.node = (PTR)crcInfo;

      cmLListAdd2Tail(&(crcIndInfo->crcLst), &(crcInfo->lnk));

   } /* end of else statement */


   RETVALUE(ret);
} /* end of ysMsUpdCrcInd*/


/*
*
*       Fun:   ysMsPrcHarqInd
*
*       Desc:  Processes the HARQ Indication
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsPrcHarqInd
(
YsCellCb        *cellCb,
TfuHqIndInfo    *harqIndInfo,
CmLteTimingInfo *timingInfo,
U8              *hqBuf,
Bool            isPusch,
CmLteRnti       rnti,
U32             numHarqBits
)
#else
PRIVATE S16 ysMsPrcHarqInd(cellCb, harqIndInfo, timingInfo, hqBuf, isPusch, rnti)
YsCellCb        *cellCb;
TfuHqIndInfo    *harqIndInfo;
CmLteTimingInfo *timingInfo;
U8              *hqBuf;
Bool            isPusch;
CmLteRnti       rnti,
U32             numHarqBits;
#endif
{
   S16           ret;
   TfuHqInfo     *harqInfo;
   U8            isAck;
   U8            isAckCw2;
#ifndef TFU_UPGRADE
   CmLteTimingInfo txsubframe;
   YsUeCb      *ueCb = NULL;
#endif
   TRC2(ysMsPrcHarqInd)

   ret = ROK;
   harqInfo = NULLP;

   if(harqIndInfo == NULLP)
   {
      ret = ysUtlAllocEventMem((Ptr *)&harqIndInfo, sizeof(TfuHqIndInfo));
      if(ret == RFAILED)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcHarqInd(): Memory allocation failed for HqInd\n"));
         MSPD_ERR("Memory allocation failed for HqInd\n");
         RETVALUE(RFAILED);
      }
      cellCb->ulEncL1Msgs[timingInfo->subframe].harqIndInfo = harqIndInfo;

      cmLListInit(&(harqIndInfo->hqIndLst));

      harqIndInfo->cellId = ysCb.cellId;
      harqIndInfo->timingInfo = *timingInfo;
   }
   {
      static unsigned int ackCw2Cnt = 0;
      static unsigned int nackCw2Cnt = 0;
      static unsigned int ackCnt = 0;
      static unsigned int nackCnt = 0;
   if (!isPusch)
   {
      isAck = (hqBuf[1] & 0x04)? TRUE: FALSE;
      if (numHarqBits == 2)
         isAckCw2 = (hqBuf[1] & 0x02)? TRUE: FALSE;
      else
         isAckCw2 = isAck;
   }
   else
   {
      isAck = (*hqBuf & 0x80) ? TRUE:FALSE;
      if (numHarqBits == 2)
         isAckCw2 = (*hqBuf & 0x40) ? TRUE:FALSE;
      else
         isAckCw2 = isAck;
   }

#ifdef HARQ_STATISTICS
      if (isAck)
         dlAcks++;
      else
         dlNacks++;
#else
   {

      if (isAck)
         ++ackCnt;
      else
      {
         ++nackCnt;

		 if ((cellCb->timingInfo.sfn % 10 ==0) && (cellCb->timingInfo.subframe == 0)){
         MSPD_DBG("CW1: Nacks/acks %u/%u, nack at phytime(%u,%u) cltime(%u,%u)\n",
            nackCnt, ackCnt, timingInfo->sfn, timingInfo->subframe,
            cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
      }
         }
      }
   if (numHarqBits == 2)
   {

	  if (isAckCw2)
         ++ackCw2Cnt;
      else
      {
         ++nackCw2Cnt;

		 if ((cellCb->timingInfo.sfn % 10 ==0) && (cellCb->timingInfo.subframe == 0)){
         MSPD_DBG("CW2: Nacks/acks %u/%u, nack at phytime(%u,%u) cltime(%u,%u)\n",
            nackCw2Cnt, ackCw2Cnt, timingInfo->sfn, timingInfo->subframe,
            cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
      }
        }
   }
#endif /* HARQ_STATISTICS */

      if (cmGetMem(&harqIndInfo->memCp, sizeof(TfuHqInfo),
            (Ptr *)&harqInfo) != ROK)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcHarqInd(): cmGetMem returned failure\n"));
         MSPD_ERR("cmGetMem returned failure\n");
         RETVALUE (RFAILED);
      } /* end of if statement */

      cmMemset ((U8 *)harqInfo, 0, sizeof(TfuHqInfo));


	   harqInfo->rnti = rnti;
      /* harqInfo->noOfTbs = YS_NUM_OF_TBS; MAC currently does not care about it */

#ifndef TFU_UPGRADE
      ueCb = ysMsCfgGetUe(cellCb, rnti);
      if (ueCb == NULLP)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcHarqInd: UE doesnt exist\n"));
         MSPD_ERR("UE doesnt exist\n");
         RETVALUE (RFAILED);
      }
      YS_MS_DECR_TIME((*timingInfo), txsubframe, 4);
      if (TRUE == ueCb->cwSwpflg[txsubframe.subframe])
      {
      	/* MSPD_DBG("\n Swapping the feedbacks rcvd at (%d,%d) as tbtocwflag was set at (%d,%d) transmission\n",timingInfo->sfn,timingInfo->subframe,\
			 txsubframe.sfn,txsubframe.subframe);     */
         harqInfo->isAck[0] = (TfuHqFdbk)isAckCw2;
         harqInfo->isAck[1] = (TfuHqFdbk)isAck;
         ueCb->cwSwpflg[txsubframe.subframe] = FALSE;
      }
      else
#endif
      {
      harqInfo->isAck[0] = (TfuHqFdbk)isAck;
      harqInfo->isAck[1] = (TfuHqFdbk)isAckCw2;
      }
      harqInfo->lnk.node = (PTR)harqInfo;

      cmLListAdd2Tail(&(harqIndInfo->hqIndLst), &(harqInfo->lnk));
#ifndef TFU_UPGRADE
#ifdef HARQ_STATISTICS
      if(ueCb->cwCount[txsubframe.subframe] > 1)
      {
         if (isAckCw2)
            dlAcks++;
         else
            dlNacks++;
      }
      /* resetting the count */
      ueCb->cwCount[txsubframe.subframe] = 0;
#endif
#endif
   } /* end of else statement */

   RETVALUE(ret);
} /* end of ysMsPrcHarqInd*/

/*
*
*       Fun:   ysMsPrcSrInd
*
*       Desc:  Processes the SR Indication
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsPrcSrInd
(
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb,
TfuSrIndInfo    *srIndInfo
)
#else
PRIVATE S16 ysMsPrcSrInd(rxSduInd, cellCb, srIndInfo)
PRXSDUIND       rxSduInd;
YsCellCb        *cellCb;
TfuSrIndInfo    *srIndInfo;
#endif
{
   S16           ret;
   TfuSrInfo     *srInfo;

   TRC2(ysMsPrcSrInd)

   ret = ROK;
   srInfo = NULLP;

   if(srIndInfo == NULLP)
   {
      ret = ysUtlAllocEventMem((Ptr *)&srIndInfo, sizeof(TfuSrIndInfo));
      if(ret == RFAILED)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcSrInd(): Memory allocation failed for SrInd\n"));
         MSPD_ERR("Memory allocation failed for SrInd\n");
         RETVALUE(RFAILED);
      }
      cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].srIndInfo = srIndInfo;

      cmLListInit(&(srIndInfo->srLst));

      srIndInfo->cellId = ysCb.cellId;
      srIndInfo->timingInfo.sfn = rxSduInd->frameNum;
      srIndInfo->timingInfo.subframe = rxSduInd->subFrameNum;
   }
   {
      if (cmGetMem(&srIndInfo->memCp, sizeof(TfuSrInfo),
            (Ptr *)&srInfo) != ROK)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcSrInd(): cmGetMem returned failure\n"));
         MSPD_ERR("cmGetMem returned failure\n");
         RETVALUE (RFAILED);
      } /* end of if statement */

      cmMemset ((U8 *)srInfo, 0, sizeof(TfuSrInfo));



	  ysMsGetRntiFrmChanId(cellCb, &(srInfo->rnti), rxSduInd->subFrameNum, rxSduInd->channelId);

      srInfo->lnk.node = (PTR)srInfo;

      cmLListAdd2Tail(&(srIndInfo->srLst), &(srInfo->lnk));

   } /* end of else statement */

   RETVALUE(ret);
} /* end of ysMsPrcSrInd*/


/*
*
*       Fun:   ysMsPrcRachInd
*
*       Desc:  Process RACH Indication
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsPrcRachInd
(
PRXSTATUSIND       pRxStatusInd,
YsCellCb        	 *cellCb,
TfuRaReqIndInfo 	 *rachIndInfo
)
#else
PUBLIC S16 ysMsPrcRachInd(pRxStatusInd, cellCb, rachIndInfo)
PRXSTATUSIND       	pRxStatusInd;
YsCellCb        		*cellCb;
TfuRaReqIndInfo 		*rachIndInfo;
#endif
{
   U8 preamble;
   PRX_PRACH_STATUS_EVENT pPrachEvents;


   TRC2(ysMsPrcRachInd)

   if(rachIndInfo == NULLP)
   {
      S16 ret;

      /* Allocate memory for the structures corresponding to the sub frame.
           rxStartInd->reserved has the subframe number */
      /* Allocate memory for Rach Indication Info */
      ret = ysUtlAllocEventMem((Ptr *)&rachIndInfo,
            sizeof(TfuRaReqIndInfo));
      if(ret == RFAILED)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcRachInd(): Memory allocation failed for RaReq \n"));
         MSPD_ERR("Memory allocation failed for RaReq \n");
         RETVALUE(RFAILED);
      } /* end of if statement */

      cellCb->ulEncL1Msgs[pRxStatusInd->subFrameNum].rachIndInfo = rachIndInfo;
      if ((ret = cmGetMem (&(rachIndInfo->memCp),
           (sizeof(TfuRachInfo) * YS_NUM_OF_RACH_OCCASIONS),
                (Ptr *)&rachIndInfo->rachInfoArr)) != ROK)
      {
         YS_DBG_ERR((_ysp, "UtlAllocEventMem(): Memory allocation failed for rachInfoArr\n"));
         MSPD_ERR("Memory allocation failed for rachInfoArr\n");
         RETVALUE(RFAILED);
      } /* end of if statement */
      else
      {
       if ((ret = cmGetMem (&(rachIndInfo->memCp),
            (sizeof(TfuRaReqInfo) * YS_NUM_OF_PREAMBLES),
                           (Ptr *)&rachIndInfo->rachInfoArr->raReqInfoArr)) != ROK)
       {
         YS_DBG_ERR((_ysp, "UtlAllocEventMem(): Memory allocation failed for rachInfoArr\n"));
         MSPD_ERR("Memory allocation failed for rachInfoArr\n");
         RETVALUE(RFAILED);
       } /* end of if statement */
   } /* end of else part */

   rachIndInfo->cellId 		= ysCb.cellId;
   rachIndInfo->timingInfo.sfn 	= pRxStatusInd->frameNum;
   rachIndInfo->timingInfo.subframe  = pRxStatusInd->subFrameNum;
   }

   pPrachEvents = (PRX_PRACH_STATUS_EVENT)&pRxStatusInd->pEventList;

   /* CL_CLEANUP: Removed the hard coding of values */
   preamble = pPrachEvents->prachPreambleId;
   rachIndInfo->nmbOfRaRnti = YS_NUM_OF_RA_RNTIS;
   /* 3GPP 36.321 5.1.4 */
   /* - RA RNTI as defined in MAC Spec */
   /*- rarnti = 1 + t_id + 10 * f_id */
   /* t_id ==> index of first sbframe of the prach */
   /* f_id ==> index of specified PRACH with that subframe */
   /* for FDD there is only on Resource and configured on first
    * subframe, hence f_id=0 !!!!
    * hence
      ra_rnti = 1 + t_id */
   rachIndInfo->rachInfoArr[0].raRnti = 1 + pRxStatusInd->subFrameNum;
   rachIndInfo->rachInfoArr[0].numRaReqInfo = 1;

   /* Get the preamble Id  and other parameters */
   /* CL_CLEANUP: Removed the hard coding of values */
   rachIndInfo->rachInfoArr[0].raReqInfoArr[0].rapId = preamble;
   if (cellCb->cellCfg.period == 5)
   {
      rachIndInfo->rachInfoArr[0].raReqInfoArr[0].ta = 0;
   }
   else
   {
      rachIndInfo->rachInfoArr[0].raReqInfoArr[0].ta = pPrachEvents->timingOffset;
   }
   rachIndInfo->rachInfoArr[0].raReqInfoArr[0].cqiPres = FALSE;

   YS_DBG_INFO((_ysp, "\n Filling up RA REQ for  MAC at : (%d) (%d) numRnti (%d) raRnti (%d) numPreamble (%d) preamble (%d) ta (%d)\n",
   			cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
   			rachIndInfo->nmbOfRaRnti,
   			rachIndInfo->rachInfoArr[0].raRnti,
   			rachIndInfo->rachInfoArr[0].numRaReqInfo, preamble, pPrachEvents->timingOffset));
MSPD_DBG("Filling up RA REQ for  MAC at : (%d) (%d) numRnti (%d) raRnti (%d) numPreamble (%d) preamble (%d) ta (%d)\n",
   			cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
   			rachIndInfo->nmbOfRaRnti,
   			rachIndInfo->rachInfoArr[0].raRnti,
   			rachIndInfo->rachInfoArr[0].numRaReqInfo, preamble, pPrachEvents->timingOffset);

   RETVALUE(ROK);
} /* end of  ysMsPrcRachInd */


#ifndef YS_MS_NO_ULCQI
/*
*
*       Fun:   ysMsGetUlCqi
*
*       Desc:  Process UL CQI
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PRIVATE S16 ysMsGetUlCqi
(
YsCellCb        *cellCb,
U8              sinr
)
#else
PRIVATE S16 ysMsGetUlCqi(cellCb, sinr)
YsCellCb        *cellCb;
U8              sinr;
#endif
{
   U32      mcs;
   if (sinr <= 101)
      return 0;
   mcs = cellCb->cellInfo.sinrToMcs[sinr];
#if 0
   /* SRIKY: CRC error even after UL LA prompted for lowering of mcs */
   if (mcs > 3)
   {
      mcs -= 3;
   }
#endif
   return cellCb->cellInfo.mcsToCqi[mcs];
} /* end of  ysMsGetUlCqi */
#endif

#ifndef YS_MS_NO_ULCQI
/*
*
*       Fun:   ysMsPrcUlCqiInd
*
*       Desc:  Process UL CQI
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PRIVATE S16 ysMsPrcUlCqiInd
(
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb,
TfuUlCqiIndInfo **ulCqiIndRef
)
#else
PRIVATE S16 ysMsPrcUlCqiInd(rxSduInd, cellCb, ulCqiIndRef)
PRXSDUIND       rxSduInd;
YsCellCb        *cellCb;
TfuUlCqiIndInfo **ulCqiIndRef;
#endif
{
   TfuUlCqiIndInfo   *ulCqiInd = *ulCqiIndRef;
   TfuUlCqiRpt       *ulCqiRpt;
   YsUeCb            *ueCb = NULLP;
   U8                wideCqi;
   U16               rnti;
   U32               currentTti, ttiDiff;

   TRC2(ysMsPrcUlCqiInd)

   /* TODO: Restructure calls so that you get rnti first thing when you
    * process a UE's data (ULSCH/UCI) from PHY, and then the rnti is just
    * passed around */
   ysMsGetRntiFrmChanId(cellCb, &rnti, rxSduInd->subFrameNum,
                                                 rxSduInd->channelId);
   ueCb = ysMsCfgGetUe(cellCb, rnti);
   if (ueCb == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysMsPrcUlCqiInd(): UE doesnt exist\n"));
      MSPD_ERR("UE doesnt exist\n");
      RETVALUE (RFAILED);
   }
   /* Smooting algorithm weighs the last report and all the previous */
   /* reports equally.                                               */
   wideCqi             = ysMsGetUlCqi(cellCb, rxSduInd->ul_CQI);
   ueCb->lastRptdUlCqi *= 8;
   ueCb->lastRptdUlCqi += (wideCqi *  2);
   ueCb->lastRptdUlCqi += 9;        // Ceiling
   ueCb->lastRptdUlCqi /= 10;

   currentTti = ((rxSduInd->frameNum * 10) + rxSduInd->subFrameNum);
   if (currentTti < ueCb->ulCqiLastTti)
      ttiDiff = ((currentTti + 10240) - ueCb->ulCqiLastTti);
   else
      ttiDiff = (currentTti - ueCb->ulCqiLastTti);

   /* Send UL CQI once every 20ms */
   if (ttiDiff < 20)
   {
      RETVALUE(ROK);
   }
   ueCb->ulCqiLastTti = currentTti;


#ifdef YS_ALLOC_ON_NEED
   if(NULLP == ulCqiInd)
   {
      S16 ret;

      /* Allocate memory for the structures corresponding to the sub frame.*/
      ret = ysUtlAllocEventMem((Ptr *)&ulCqiInd, sizeof(TfuUlCqiIndInfo));
      if(ret == RFAILED)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcUlCqiInd(): Mem alloc failed for UlCqi\n"));
         MSPD_ERR("Mem alloc failed for UlCqi\n");
         RETVALUE(RFAILED);
      }

      cmLListInit(&(ulCqiInd->ulCqiRpt));
      ulCqiInd->cellId = ysCb.cellId;
      ulCqiInd->timingInfo.sfn = rxSduInd->frameNum;
      ulCqiInd->timingInfo.subframe = rxSduInd->subFrameNum;
      *ulCqiIndRef = ulCqiInd;
   }
#endif
   if (cmGetMem(&ulCqiInd->memCp, sizeof(TfuUlCqiRpt), (Ptr *)&ulCqiRpt) != ROK)
   {
         ysUtlFreeEventMem(ulCqiInd);
      YS_DBG_ERR((_ysp, "cmGetMem returned failure\n"));
      MSPD_ERR("cmGetMem returned failure\n");
      RETVALUE (RFAILED);
   }

   /* Added for Limiting the CQI for stability Check*/
#ifdef LIMIT_DL_UL_CQI
   if (ueCb->lastRptdUlCqi >= 10)
   {
      ueCb->lastRptdUlCqi = 10;
   }
#endif
   /* Fill the report with the values */

   ulCqiRpt->isTxPort0  = TRUE;
   ulCqiRpt->wideCqi    = ueCb->lastRptdUlCqi;
   ulCqiRpt->rnti       = rnti;
   ulCqiRptCnt[ulCqiRpt->wideCqi]++;
   totUlCqiRpt++;
   ulCqiRpt->numSubband = 0;
   ulCqiRpt->lnk.node   = (PTR)ulCqiRpt;
   cmLListAdd2Tail(&(ulCqiInd->ulCqiRpt), &(ulCqiRpt->lnk));
   MSPD_DBG("Feeding MAC UL CQI %d for RNTI %d\n", ueCb->lastRptdUlCqi,
                                                   ueCb->ueId);
   RETVALUE(ROK);
} /* end of  ysMsPrcUlCqiInd */
#endif

#ifdef TFU_UPGRADE
/*
*
*       Fun:   ysMsPrcDlRiInd
*
*       Desc:  Process DL RI indication
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PRIVATE S16 ysMsPrcDlRiInd
(
YsCellCb         *cellCb,
CmLteRnti        rnti,
CmLteTimingInfo  timingInfo,
U8               numRiBits,
U8               riByte
)
#else
PRIVATE S16 ysMsPrcDlRiInd(cellCb, rnti, timingInfo, numRiBits, riByte)
YsCellCb         *cellCb;
CmLteRnti        rnti;
CmLteTimingInfo  timingInfo;
U8               numRiBits;
U8               riByte;
#endif
{
   TfuRawCqiIndInfo  *dlCqiIndInfo;
   TfuRawCqiRpt      *dlCqiRpt;
   S16         ret;
   YsUeCb      *ueCb;

   TRC2(ysMsPrcDlRiInd)

   ret = ROK;

   ueCb = ysMsCfgGetUe(cellCb, rnti);
   if (ueCb == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysMsPrcDlRiInd(): UE doesnt exist\n"));
      MSPD_ERR("Error: UE not found\n");
      RETVALUE (RFAILED);
   }

   dlCqiIndInfo = cellCb->ulEncL1Msgs[timingInfo.subframe].dlCqiIndInfo;
#ifdef YS_ALLOC_ON_NEED
   if(NULLP == dlCqiIndInfo)
   {
      /* Allocate memory for the structures corresponding to the sub frame.*/
      ret = ysUtlAllocEventMem((Ptr *)&dlCqiIndInfo,
                           sizeof(TfuRawCqiIndInfo));
      if(ret == RFAILED)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcDlRiInd(): Memory allocation failed for DlCqi\n"));
         MSPD_ERR("Memory allocation failed for DlCqi\n");
         RETVALUE(RFAILED);
      }

      cellCb->ulEncL1Msgs[timingInfo.subframe].dlCqiIndInfo = dlCqiIndInfo;
      cmLListInit(&(dlCqiIndInfo->rawCqiRpt));

      dlCqiIndInfo->cellId = ysCb.cellId;
      dlCqiIndInfo->timingInfo = timingInfo;
   }
#endif

   if (cmGetMem(&dlCqiIndInfo->memCp, sizeof(TfuRawCqiRpt),
            (Ptr *)&dlCqiRpt) != ROK)
   {
      YS_DBG_ERR((_ysp, "ysMsPrcDlRiInd(): cmGetMem returned failure\n"));
      MSPD_ERR("cmGetMem returned failure\n");
      RETVALUE (RFAILED);
   }

  /* Only periodic CQI is expected currently as the configuration from RRC
     is intended for periodic. 36.213 Section 7.2.2 */
   dlCqiRpt->crnti = rnti;
   dlCqiRpt->numBits = numRiBits;
   /* MAC expects information from LSB startting from the cqiBits[0]*/
   dlCqiRpt->ri |= (riByte >> (8 - numRiBits));
   //MSPD_DBG("\nSYED: AT CL numBits, riByte are [%d, %d]\n", dlCqiRpt->numBits, dlCqiRpt->ri);

   dlCqiRpt->lnk.node = (PTR)dlCqiRpt;

   cmLListAdd2Tail(&(dlCqiIndInfo->rawCqiRpt), &(dlCqiRpt->lnk));

   RETVALUE(ret);
} /* end of ysMsPrcDlRiInd */
#endif
/*
*
*       Fun:   ysMsPrcDlCqiInd
*
*       Desc:  Process DL CQI indication
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PRIVATE S16 ysMsPrcDlCqiInd
(
YsCellCb         *cellCb,
CmLteRnti        rnti,
CmLteTimingInfo  timingInfo,
U8               numCqiBits,
U8               *cqiBits
)
#else
PRIVATE S16 ysMsPrcDlCqiInd(cellCb, rnti, timingInfo, numCqiBits, cqiBits)
YsCellCb         *cellCb;
CmLteRnti        rnti;
CmLteTimingInfo  timingInfo;
U8               numCqiBits;
U8               *cqiBits;
#endif
{
#ifndef TFU_UPGRADE
   TfuDlCqiIndInfo  *dlCqiIndInfo;
   TfuDlCqiRpt *dlCqiRpt;
#endif
#ifdef TFU_UPGRADE
   TfuRawCqiIndInfo  *dlCqiIndInfo;
   TfuRawCqiRpt      *dlCqiRpt;
   U8                numRiBits;
#endif
   S16         ret;
   YsUeCb      *ueCb;
   U8          cqi;

   TRC2(ysMsPrcDlCqiInd)

   ret = ROK;

   if (cqiBits[0] & 0x0f) {
      MSPD_DBG("Bad CQI value: cqiBits=%u, phytime(%u,%u) cltime(%u,%u)\n",
         cqiBits[0], timingInfo.sfn, timingInfo.subframe,
         cellCb->timingInfo.sfn, cellCb->timingInfo.subframe);
      YS_DBG_INFO((_ysp, "ysMsPrcDlCqiInd(): Bad CQI value reported\n"));
      RETVALUE(ROK);
   }
   cqi = cqiBits[0] >> 4;
#ifdef LTEMAC_MIMO
   /* Added for Limiting the CQI for stability Check*/
#ifdef LIMIT_DL_UL_CQI
   if (cqi >= 13)
   {
      cqi = 13;
   }
#endif
#endif

   ueCb = ysMsCfgGetUe(cellCb, rnti);
   if (ueCb == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysMsPrcDlCqiInd(): UE doesnt exist\n"));
	MSPD_ERR("UE not found\n");
      RETVALUE (RFAILED);
   }

   dlCqiIndInfo = cellCb->ulEncL1Msgs[timingInfo.subframe].dlCqiIndInfo;
#ifdef YS_ALLOC_ON_NEED
   if(NULLP == dlCqiIndInfo)
   {
      /* Allocate memory for the structures corresponding to the sub frame.*/
#ifndef TFU_UPGRADE
      ret = ysUtlAllocEventMem((Ptr *)&dlCqiIndInfo,
                           sizeof(TfuDlCqiIndInfo));
#else
      ret = ysUtlAllocEventMem((Ptr *)&dlCqiIndInfo,
                           sizeof(TfuRawCqiIndInfo));
#endif
      if(ret == RFAILED)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcDlCqiInd(): Memory allocation failed for DlCqi\n"));
          MSPD_ERR("Memory allocation failed for DlCQI\n");
         RETVALUE(RFAILED);
      }

      cellCb->ulEncL1Msgs[timingInfo.subframe].dlCqiIndInfo = dlCqiIndInfo;
#ifndef TFU_UPGRADE
      cmLListInit(&(dlCqiIndInfo->dlCqiRptsLst));
#else
      cmLListInit(&(dlCqiIndInfo->rawCqiRpt));
#endif

      dlCqiIndInfo->cellId = ysCb.cellId;
      dlCqiIndInfo->timingInfo = timingInfo;
   }
#endif

#ifndef TFU_UPGRADE
   if (cmGetMem(&dlCqiIndInfo->memCp, sizeof(TfuDlCqiRpt),
            (Ptr *)&dlCqiRpt) != ROK)
#else
   if (cmGetMem(&dlCqiIndInfo->memCp, sizeof(TfuRawCqiRpt),
            (Ptr *)&dlCqiRpt) != ROK)
#endif
   {
      YS_DBG_ERR((_ysp, "ysMsPrcDlCqiInd(): cmGetMem returned failure\n"));
	  ysUtlFreeEventMem(dlCqiIndInfo);
      RETVALUE (RFAILED);
   }

  /* Only periodic CQI is expected currently as the configuration from RRC
     is intended for periodic. 36.213 Section 7.2.2 */
#ifndef TFU_UPGRADE
   dlCqiRpt->rnti = rnti;
   dlCqiRpt->isPucchInfo = TRUE;
   dlCqiRpt->dlCqiInfo.pucchCqi.mode =  TFU_PUCCH_CQI_MODE10;

   if(!cqi)
   {
      /* MS_WORAKAROUND: Considering CQI 0 as invalid CQI and using last reported valid CQI */
      cqi = ueCb->lastRptdDlCqi;
   }
   else
   {
      ueCb->lastRptdDlCqi = cqi;
   }

   dlCqiRpt->dlCqiInfo.pucchCqi.u.mode10Info.type = TFU_RPT_CQI;
   dlCqiRpt->dlCqiInfo.pucchCqi.u.mode10Info.u.cqi = cqi;


   MSPD_DBG("DL CQI(%u) @ cellTime(%u,%u) for PHY time(%u,%u) ueId(%u)\n",
   	cqi, cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
   	timingInfo.sfn, timingInfo.subframe, rnti);

#endif
#ifdef TFU_UPGRADE
#ifdef LTEMAC_MIMO
   numRiBits = 1; /* TODO: Assuming 2x2 MIMO */
#else
   numRiBits = 0;
#endif
   dlCqiRpt->crnti = rnti;
   dlCqiRpt->numBits = numCqiBits;
   if(!cqi)
   {
      /* MS_WORAKAROUND: Considering CQI 0 as invalid CQI and using last reported valid CQI */
      cqi = ueCb->lastRptdDlCqi;
   }
   else
   {
      ueCb->lastRptdDlCqi = cqi;
   }
   /* MAC expects information from LSB startting from the cqiBits[0]*/
   dlCqiRpt->cqiBits[TFU_MAX_CQI_BYTES-1] |= cqi;
   /* CL cannot differentiate between the type of feedback cqi/RI,
    * hence filling both cqiBits[] and ri field of dlCqiRpt. MAC
    * knows what to expect and would read the appropriate field */
   dlCqiRpt->ri |= (cqiBits[0] >> (8 - numRiBits));
#endif

   dlCqiRpt->lnk.node = (PTR)dlCqiRpt;

#ifndef TFU_UPGRADE
   cmLListAdd2Tail(&(dlCqiIndInfo->dlCqiRptsLst), &(dlCqiRpt->lnk));
#else
   cmLListAdd2Tail(&(dlCqiIndInfo->rawCqiRpt), &(dlCqiRpt->lnk));
#endif

   RETVALUE(ret);
} /* end of ysMsPrcDlCqiInd */

#ifndef YS_MS_NO_TA
/*
*
*       Fun:   ysMsPrcTmAdvInd
*
*       Desc:  Process Timing Advance indication
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsPrcTmAdvInd
(
PRXSDUIND       rxSduInd,
YsCellCb         *cellCb,
TfuTimingAdvIndInfo **tmAdvIndInfoRef
)
#else
PRIVATE S16 ysMsPrcTmAdvInd(rxSduInd, cellCb, tmAdvIndInfoRef)
PRXSDUIND       rxSduInd;
YsCellCb         *cellCb;
TfuTimingAdvIndInfo **tmAdvIndInfoRef;
#endif
{
   TfuTimingAdvIndInfo *tmAdvIndInfo = *tmAdvIndInfoRef;
   TfuTimingAdvInfo *tmAdvInfo;
   U16         ta = (U16)(rxSduInd->timingAdv);
   S16         ret; 
   CmLteRnti   ueId = 0;
   YsUeCb      *ueCb = NULLP;

   TRC2(ysMsPrcTmAdvInd)

   ret = ROK;

   if(!(ta <=63))
   {
      YS_DBG_ERR((_ysp, "ysMsPrcTmAdvInd(): ta=%u not in range [0,63]\n", ta));
      MSPD_ERR("ta=%u not in range [0,63]\n", ta);
      RETVALUE(ROK);
   }

   /* TODO: Restructure calls so that you get rnti first thing when you
    * process a UE's data (ULSCH/UCI) from PHY, and then the rnti is just
    * passed around */
   ysMsGetRntiFrmChanId(cellCb, &ueId, rxSduInd->subFrameNum,
         rxSduInd->channelId);

   ueCb = ysMsCfgGetUe(cellCb,ueId);
   if (ueCb == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysMsPrcTmAdvInd(): UE doesnt exist\n"));
      MSPD_ERR("UE doesnt exist\n");
      RETVALUE(ROK);
   }

#ifndef YS_MS_NO_TA

   if (ta == 31)//YS_MS_TA_THROTTLED(ueCb))
   {
      //MSPD_DBG("[TA] Not reporting\n");
      /* For now don't report TA to MAC when there is no drift,
       * OR, when TA reporting is throttled at this time */
      RETVALUE(ROK);
   }

   ueCb->ulTaLastTti = ta;

   /*MSPD_DBG("[TA] Reporting TA to MAC prev TA %d crnt TA %d rnti %u\n",
         ueCb->tarptInfo.ta, ta, ueId); */
   ueCb->tarptInfo.ta = ta;
#endif

   if (tmAdvIndInfo == NULLP)
   {
       ret = ysUtlAllocEventMem((Ptr *)&tmAdvIndInfo,
               sizeof(*tmAdvIndInfo));
       if(ret != ROK)
       {
          YS_DBG_ERR((_ysp, "ysMsPrcUlSchSduInd(): Memory allocation failed for DatInd\n"));
          MSPD_ERR("Memory allocation failed for DatInd\n");
          RETVALUE(ret);
       }

       /* Initialize the Data Indication List */
       cmLListInit(&tmAdvIndInfo->timingAdvLst);

       tmAdvIndInfo->cellId = ysCb.cellId;
       tmAdvIndInfo->timingInfo.sfn = rxSduInd->frameNum;
       tmAdvIndInfo->timingInfo.subframe = rxSduInd->subFrameNum;

       *tmAdvIndInfoRef = tmAdvIndInfo;
       MSPD_DBG("*********** TA feeding to schduler: %u rnti %u \n", ta, ueId);
   }

   if (cmGetMem(&tmAdvIndInfo->memCp, sizeof(TfuTimingAdvInfo),
            (Ptr *)&tmAdvInfo) != ROK)
   {
      YS_DBG_ERR((_ysp, "ysMsPrcTmAdvInd(): cmGetMem returned failure\n"));
      MSPD_ERR("cmGetMem returned failure\n");
      RETVALUE (RFAILED);
   }

   tmAdvInfo->rnti      = ueId;
   tmAdvInfo->timingAdv = ta;
   tmAdvInfo->lnk.node = (PTR)tmAdvInfo;
   cmLListAdd2Tail(&tmAdvIndInfo->timingAdvLst, &tmAdvInfo->lnk);
   RETVALUE(ret);
} /* end of ysMsPrcTmAdvInd */
#endif

/*
*
*       Fun:   ysMsPrcRxStatusInd
*
*       Desc:  Process RX_STATUS_IND message from PHY
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PUBLIC S16 ysMsPrcRxStatusInd
(
PRXSTATUSIND       pRxStatusInd,
YsCellCb           *cellCb
)
#else
PUBLIC S16 ysMsPrcRxStatusInd(pRxStatusInd, cellCb)
PRXSTATUSIND       	pRxStatusInd;
YsCellCb            *cellCb;
#endif
{
   TfuRaReqIndInfo 		  *rachIndInfo = NULLP;

   TRC2(ysMsPrcRxStatusInd);

   YS_DBG_INFO((_ysp, "Received PHY_RXSTATUS_IND subframe (%d) statustype (%d) eventNumber (%d) \n",
            pRxStatusInd->subFrameNum, pRxStatusInd->statusType, pRxStatusInd->eventNumber));

   if (pRxStatusInd->eventNumber != 0)
   {
      if (pRxStatusInd->statusType == PRACH_RESULT)
      {
         rachIndInfo = cellCb->ulEncL1Msgs[pRxStatusInd->subFrameNum].rachIndInfo;
         MSPD_DBG("Received PRACH from UE : celltime (%d,%d) PRACH(%d,%d) \n",
         cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
         pRxStatusInd->frameNum , pRxStatusInd->subFrameNum);
         YS_DBG_FATAL((_ysp, "Received PRACH from UE subframe cell(%d, %d) at(%d,%d)\n",
         cellCb->timingInfo.sfn, cellCb->timingInfo.subframe,
         pRxStatusInd->frameNum , pRxStatusInd->subFrameNum));
         cellCb->gotRACH++;
         ysMsPrcRachInd(pRxStatusInd, cellCb, rachIndInfo);
      }
	   else if (pRxStatusInd->statusType == CQIRIHI_RESULT)
      {
         if ((pRxStatusInd->status == TIMEOUT_RXSTART_REQ) || (pRxStatusInd->status == INVALID_TX_VECTOR))
         {
            ++delayedApiCnt;/*SRIKY MMM */
            MSPD_DBG("Delayed API RXSTATUS cnt(%u) status(%u)\n", delayedApiCnt, pRxStatusInd->status);
         }
         ysMsPrcCqiPmiRiHqInd(pRxStatusInd, cellCb);
      }
      else if (pRxStatusInd->statusType == SRS_RESULT)
      {
      }
      else
      {
         uart_printf("Unhandled status Type %d in ysMsPrcRxStatusInd()\n",
            pRxStatusInd->statusType);
      }

   }

   RETVALUE(ROK);
} /* end of  ysMsPrcRxStatusInd */

/*
*
*       Fun:   ysMsPrcCqiPmiRiHqInd
*
*       Desc:  Process CQI/PMI/RI/HARQ indication on PUSCH
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/

#ifdef ANSI
PRIVATE S16 ysMsPrcCqiPmiRiHqInd
(
PRXSTATUSIND       pRxStatusInd,
YsCellCb        	 *cellCb
)
#else
PUBLIC S16 ysMsPrcCqiPmiRiHqInd(pRxStatusInd, cellCb)
PRXSTATUSIND       	pRxStatusInd;
YsCellCb        		*cellCb;
#endif
{
   PRX_CQIRIHI_STATUS_EVENT pCqiPmiRiHqEvents;
   U8                       numCqiPmiBytes = 0;
   U8                       numHqBytes = 0;
   U8                       numRiBytes = 0;
   U8                       *msgPtr;
   U8                       cqiPmiMsg[32];
   U8                       hqByte;
#ifdef TFU_UPGRADE
   U8                       riByte;
#endif
   CmLteTimingInfo          timingInfo;
   TfuHqIndInfo             *harqIndInfo;
   CmLteRnti                rnti;


   TRC2(ysMsPrcCqiPmiRiHqInd)

   pCqiPmiRiHqEvents = (PRX_CQIRIHI_STATUS_EVENT)&pRxStatusInd->pEventList;

   timingInfo.sfn = pRxStatusInd->frameNum;
   timingInfo.subframe = pRxStatusInd->subFrameNum;

   ysMsGetRntiFrmChanId(cellCb, &rnti, timingInfo.subframe, pCqiPmiRiHqEvents->chanId);

   if (pCqiPmiRiHqEvents->harq_pt)
   {
      numHqBytes   = 1;
   }

   if (pCqiPmiRiHqEvents->ri_pt)
   {
      numRiBytes   = 1;
   }
   if (pCqiPmiRiHqEvents->cqi_pmi_pt)
   {
      numCqiPmiBytes = pCqiPmiRiHqEvents->status_len - numHqBytes - numRiBytes;
   }

   msgPtr = (U8 *) ((U8 *)pCqiPmiRiHqEvents + sizeof(PRX_CQIRIHI_STATUS_EVENT));
   if (numCqiPmiBytes)
   {
      cmMemcpy(cqiPmiMsg,msgPtr,numCqiPmiBytes);
      msgPtr = msgPtr + numCqiPmiBytes;
      if (numCqiPmiBytes != 1)
      {
         MSPD_ERR("numCqiPmiBytes=%u, should be 1\n", numCqiPmiBytes);
      }
      if (pCqiPmiRiHqEvents->cqiPmiConf & 0x3)
          ysMsPrcDlCqiInd(cellCb, rnti, timingInfo, 4, cqiPmiMsg);
      else
        MSPD_ERR("invalid numCqiPmiBytes. Cfn: %x\n", pCqiPmiRiHqEvents->cqiPmiConf);
   }
   if (numRiBytes)
   {
      riByte = *msgPtr;
#ifdef TFU_UPGRADE
          /* TODO: Assuming 2 TX antenna, hence a RI bitwidth of 1 */
          ysMsPrcDlRiInd(cellCb, rnti, timingInfo, 1, riByte);
#endif
      msgPtr = msgPtr+1;
   }
   if (numHqBytes)
   {
      hqByte = *msgPtr;
      msgPtr = msgPtr+1;
      if (hqByte & 0x20)
      {
         harqIndInfo = cellCb->ulEncL1Msgs[pRxStatusInd->subFrameNum].harqIndInfo;
         ysMsPrcHarqInd(cellCb, harqIndInfo, &timingInfo, &hqByte, TRUE, rnti, pCqiPmiRiHqEvents->harq_pt);
      }
   }
   else
   {
      static U32 muxDtxCnt = 0;
      muxDtxCnt++;
      if ((cellCb->timingInfo.sfn % 10 == 0) && (cellCb->timingInfo.subframe == 0))
      {
         MSPD_DBG("MUX DTX Received @ (%d,%d) ueId(%u) Cnt(%u)\n",
          cellCb->timingInfo.sfn, cellCb->timingInfo.subframe, rnti, muxDtxCnt);
      }
   }



   RETVALUE(ROK);
} /* end of  ysMsPrcRachInd */


/*
*
*       Fun:   ysMsPrcPucchDeltaPwrInd
*
*       Desc:  Processes the PUCCH delta power indication
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PRIVATE S16 ysMsPrcPucchDeltaPwrInd
(
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb,
YsUeCb          *ue,
TfuPucchDeltaPwrIndInfo *pucchDeltaPwrIndInfo
)
#else
PRIVATE S16 ysMsPrcPucchDeltaPwrInd(rxSduInd, cellCb, ue, pucchDeltaPwrIndInfo)
RXSDUIND       rxSduInd;
YsCellCb        *cellCb;
YsUeCb          *ue;
TfuPucchDeltaPwrIndInfo *pucchDeltaPwrIndInfo;
#endif
{
   S16               ret;
   TfuPucchDeltaPwr  *pucchDeltaPwrInfo;
   S32               pucchSnr;

   TRC2(ysMsPrcPucchDeltaPwrInd)

   ret = ROK;
   pucchDeltaPwrInfo = NULLP;

   if(pucchDeltaPwrIndInfo == NULLP)
   {
      ret = ysUtlAllocEventMem((Ptr *)&pucchDeltaPwrIndInfo, sizeof(TfuPucchDeltaPwrIndInfo));
      if(ret == RFAILED)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcPucchDeltaPwrInd(): Memory allocation failed for SrInd\n"));
         MSPD_ERR("Memory allocation failed for SrInd\n");
         RETVALUE(RFAILED);
      }
      cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].pucchDeltaPwrIndInfo = pucchDeltaPwrIndInfo;

      cmLListInit(&(pucchDeltaPwrIndInfo->pucchDeltaPwrLst));

      pucchDeltaPwrIndInfo->cellId = ysCb.cellId;
      pucchDeltaPwrIndInfo->timingInfo.sfn = rxSduInd->frameNum;
      pucchDeltaPwrIndInfo->timingInfo.subframe = rxSduInd->subFrameNum;
   }
   {
      if (cmGetMem(&pucchDeltaPwrIndInfo->memCp, sizeof(TfuPucchDeltaPwr),
            (Ptr *)&pucchDeltaPwrInfo) != ROK)
      {
         YS_DBG_ERR((_ysp, "ysMsPrcPucchDeltaPwrInd(): cmGetMem returned failure\n"));
         MSPD_ERR("cmGetMem returned failure\n");
         RETVALUE (RFAILED);
      } /* end of if statement */

      cmMemset ((U8 *)pucchDeltaPwrInfo, 0, sizeof(TfuPucchDeltaPwr));


	  ysMsGetRntiFrmChanId(cellCb, &(pucchDeltaPwrInfo->rnti),
	  	rxSduInd->subFrameNum, rxSduInd->channelId);

	  pucchSnr = ue->lastRptdPucchSnr;
      MSPD_DBG("PUCCH SNR (%d) UE (%u)\n", pucchSnr, ue->ueId);

	  if ((pucchSnr < 10) || (pucchSnr > 15) )
         pucchDeltaPwrInfo->pucchDeltaPwr =  10 - pucchSnr;

      else
         pucchDeltaPwrInfo->pucchDeltaPwr =  0;

      pucchDeltaPwrInfo->lnk.node = (PTR)pucchDeltaPwrInfo;

      cmLListAdd2Tail(&(pucchDeltaPwrIndInfo->pucchDeltaPwrLst), &(pucchDeltaPwrInfo->lnk));

   } /* end of else statement */

   RETVALUE(ret);
} /* end of ysMsPrcSrInd*/
/*
*
*       Fun:   ysMsPrcPucchPwrInd
*
*       Desc:  Process PUCCH power indication
*
*       Ret:   S16
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsPrcPucchPwrInd
(
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb
)
#else
PUBLIC S16 ysMsPrcPucchPwrInd(rxSduInd, cellCb)
PRXSDUIND       rxSduInd;
YsCellCb        *cellCb;
#endif
{
   TfuPucchDeltaPwrIndInfo *pucchDeltaPwrInfo;
   CmLteRnti       rnti;
   YsUeCb          *ueCb = NULLP;
   S32              currSnr = 0;
   U32             currentTti, ttiDiff;

   TRC2(ysMsPrcPucchPwrInd)

   ysMsGetRntiFrmChanId(cellCb, &rnti,rxSduInd->subFrameNum, rxSduInd->channelId);

   ueCb = ysMsCfgGetUe(cellCb, rnti);
   if (ueCb == NULLP)
   {
      YS_DBG_ERR((_ysp, "ysMsPrcUlCqiInd(): UE doesnt exist\n"));
      MSPD_ERR(" UE doesnt exist\n");
      RETVALUE (RFAILED);
   }
   /* Smooting algorithm weighs the last report and all the previous */
   /* reports equally.                                               */
   currSnr = (rxSduInd->ul_CQI - 128)/2;
   /* MSPD_DBG("SNR from PHY %u lastRptdSnr %u ue %u\n",
   	currSnr, ueCb->lastRptdPucchSnr, ueCb->ueId); */
   ueCb->lastRptdPucchSnr *= 8;
   ueCb->lastRptdPucchSnr += (currSnr *  2);
   ueCb->lastRptdPucchSnr += 5;        // Rounded
   ueCb->lastRptdPucchSnr /= 10;

   currentTti = ((rxSduInd->frameNum * 10) + rxSduInd->subFrameNum);
   if (currentTti < ueCb->pucchSnrLastTti)
      ttiDiff = ((currentTti + 10240) - ueCb->pucchSnrLastTti);
   else
      ttiDiff = (currentTti - ueCb->pucchSnrLastTti);

   /* Send SNR report once every 64ms or greater since the values from PHY are averaged over 64 subframes */
   if (ttiDiff < 64)
   {
      RETVALUE(ROK);
   }
   ueCb->pucchSnrLastTti = currentTti;


   pucchDeltaPwrInfo = cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].pucchDeltaPwrIndInfo;
   ysMsPrcPucchDeltaPwrInd(rxSduInd, cellCb, ueCb, pucchDeltaPwrInfo);

    RETVALUE(ROK);
}

/*
*
*       Fun:   ysMsUlmPrcPucch
*
*       Desc:  Handles PUCCH SDUs
*
*       Ret:   void
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUlmPrcPucch
(
PRXSDUIND       rxSduInd,
YsCellCb        *cellCb
)
#else
PUBLIC S16 ysMsUlmPrcPucch(rxSduInd, cellCb)
PRXSDUIND       rxSduInd;
YsCellCb        *cellCb;
#endif
{

   TRC2(ysMsUlmPrcPucch)

   if ((ysMsPrcUciInd(rxSduInd, cellCb)) != ROK)
   {
      YS_DBG_ERR((_ysp, "ysMsUlmSduInd(): Processing of ysMsPrcUci failed\n"));
      MSPD_ERR("Processing of ysMsPrcUci failed\n");
      RETVALUE(RFAILED);
   } /* end of if statement */
#ifndef YS_MS_NO_TA
      /* This is blocked as PHY sends 0 always for ta on PUCCH */
      /* ysMsPrcTmAdvInd(rxSduInd, cellCb,
            *     &cellCb->ulEncL1Msgs[rxSduInd->subFrameNum].tmAdvIndInfo); */
#endif
      /* SNR for SR is not to be considered for PUCCH power control */
   if ((rxSduInd->pucchType != FORMAT1) &&
   	   !((rxSduInd->status == TIMEOUT_RXSTART_REQ) ||
   	     (rxSduInd->status == INVALID_TX_VECTOR)))
   {
      ysMsPrcPucchPwrInd(rxSduInd, cellCb);
   }

   RETVALUE(ROK);

} /* end of ysMsUlmSduInd */


#ifdef DLHQ_RTT_OPT
/*
*
*       Fun:   ysMsUlmPrcUlCntrl
*
*       Desc:  Sends UL control information to SCH
*
*       Ret:   void
*
*       Notes: None
*
*       File:  ys_ms_ul.c
*
*/
#ifdef ANSI
PUBLIC S16 ysMsUlmPrcUlCntrl
(
CmLteTimingInfo *ulCntrlTiming,
YsCellCb        *cellCb
)
#else
PUBLIC S16 ysMsUlmPrcUlCntrl(ulCntrlTiming, cellCb)
CmLteTimingInfo *ulCntrlTiming;
YsCellCb        *cellCb;
#endif
{
   CmLteTimingInfo rxSduSf;
   TfuHqIndInfo    *harqIndInfo;

   rxSduSf = *ulCntrlTiming;
   harqIndInfo = cellCb->ulEncL1Msgs[ulCntrlTiming->subframe].harqIndInfo;

   YS_INCR_TIMING_INFO(rxSduSf,1);

   if (!YS_TIMING_INFO_SAME(rxSduSf, cellCb->timingInfo))
   {
	   delayedHarq++;
          MSPD_DBG("DL HARQ IND (%u,%u) is received late/early from PHY Exptime(%d,%d)\n",
	  	   ulCntrlTiming->sfn,ulCntrlTiming->subframe,
	  	   rxSduSf.sfn, rxSduSf.subframe);
	  MSPD_DBG("\n DL HARQ IND (%u,%u) is received late/early from PHY Exptime(%d,%d)"
		 	" rcvdtime(%d,%d)\n",
	  	   ulCntrlTiming->sfn, ulCntrlTiming->subframe,
	       rxSduSf.sfn, rxSduSf.subframe,
	       cellCb->timingInfo.sfn, cellCb->timingInfo.subframe); 
	}
	/* sending the DL HARQ feedbacks to SCH as and when it is received */
    if (harqIndInfo != NULLP)
    {
       if(harqIndInfo->hqIndLst.count > 0 )
       {
          YsUiTfuHqInd(&cellCb->schTfuSap->sapPst, cellCb->schTfuSap->suId, harqIndInfo);
       }
       else
       {
          YS_FREE_SDU(harqIndInfo);
       }/* end of else part */
    }

	cellCb->ulEncL1Msgs[ulCntrlTiming->subframe].harqIndInfo = NULLP;
   RETVALUE(ROK);
}
#endif


/********************************************************************30**

         End of file:     ys_ms_ul.c

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
----------- -------- ---- -----------------------------------------------
/main/1      ---      mraj   1. initial release.
/main/1    ys004.102  vr     1. Merged MSPD code with phy 1.7
*********************************************************************91*/
