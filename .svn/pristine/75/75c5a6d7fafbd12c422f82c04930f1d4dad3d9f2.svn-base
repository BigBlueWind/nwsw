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
  
     Desc:     SIB Module 

     Ret :     ROK - success
               RFAILED - failure

     File:     ve_sib.c

     Sid:      ve_sib.c@@/main/2 - Wed Dec  1 08:44:30 2010

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
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#include "czt.h"
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */


/* RRM related includes */
#include "ve_eut.h"
#include "ve.h" 
#include "lve.h"

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
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#include "czt.x"
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
/* Include of pal related header */
#include "ve_dflt_pal.x"
#else
#include "ve_dflt_ms.x"
#endif

#ifdef __cplusplus
extern "C" {
#ifdef MSPD
EXTERN CONSTANT U16 veDfltRefSignalPower = 18;
#endif
}
#endif /* __cplusplus */


#ifdef RGR_RRM_TICK

/*
*
*       Fun:   veSibBldMIBMsg
*
*       Desc:  Fills BCCH_BCH  message (DL BCCH BCH)
*
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  ve_sib.c
*
*/
#ifdef ANSI
PUBLIC S16 veSibBldMIBMsg
(
NhuDatReqSdus *nhDatReqEvntSdu,
U16           schedSFN
)
#else
PUBLIC S16 veSibBldMIBMsg(nhDatReqEvntSdu, schedSFN)
NhuDatReqSdus *nhDatReqEvntSdu;
U16           schedSFN;
#endif
{
   NhuBCCH_BCH_Msg *bcchBchMsg = NULLP;
   NhuTimingInfo *tmngInf = NULLP;

   TRC2(veSibBldMIBMsg);

   bcchBchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchBchMsg);

   /* Filling Timing Info */
   tmngInf = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo); 
   tmngInf->sfn = schedSFN;
   tmngInf->subframe = VE_VAL_ZERO;


   VE_FILL_TKN_UINT(bcchBchMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(bcchBchMsg->message.pres, PRSNT_NODEF);
   /* dl-Bandwidth */
   VE_FILL_TKN_UINT(bcchBchMsg->message.dl_Bandwidth, veDfltDlSysbw);
   /* PHICH-Config */
   VE_FILL_TKN_UINT(bcchBchMsg->message.phich_Config.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(bcchBchMsg->message.phich_Config.phich_Duration,\
         veDfltPhichDur);
   VE_FILL_TKN_UINT(bcchBchMsg->message.phich_Config.phich_Resource,\
         veDfltPhichRes);
   /* systemFrameNumber */
   VE_GET_MEM(nhDatReqEvntSdu, sizeof(U8),\
         &(bcchBchMsg->message.systemFrameNumber.val));
   bcchBchMsg->message.systemFrameNumber.pres = PRSNT_NODEF;
   bcchBchMsg->message.systemFrameNumber.len  = VE_BYTE_LEN;
   /* Assign the 8 most significant bits of SFN */
   VE_GET_SFN_MIB(bcchBchMsg->message.systemFrameNumber.val[0], schedSFN);

   /* Spare, since it is not optional */
   VE_GET_MEM(nhDatReqEvntSdu, (2 * sizeof(U8)), &(bcchBchMsg->message.spare.val));
   bcchBchMsg->message.spare.pres = PRSNT_NODEF;
   bcchBchMsg->message.spare.len  = VE_MIB_SPARE_LEN;

   RETVALUE(ROK);
} /* End of veSibBldMIBMsg */



/*
*
*       Fun:   veSibBldSIB1Msg
*
*       Desc:  Fills System Info Block Type1 message (DL BCCH DLSCH)
*
*       Ret:   
*
*       Notes: None
*
*       File:  ve_sib.c
*
*/
#ifdef ANSI
PUBLIC S16 veSibBldSIB1Msg
(
NhuDatReqSdus *nhDatReqEvntSdu,
U16           schedSFN
)
#else
PUBLIC S16 veSibBldSIB1Msg(nhDatReqEvntSdu, schedSFN)
NhuDatReqSdus *nhDatReqEvntSdu;
U16           schedSFN;
#endif
{
   NhuBCCH_DL_SCH_Msg *bcchSchMsg = NULLP;
   NhuSysInfoBlockTyp1 *sibTyp1 = NULLP;
   NhuTimingInfo *tmngInf = NULLP;
   NhuPLMN_IdentityLst *plmnLst = NULLP;
   NhuSchedulingInfo *schedInf = NULLP;

   U8 idx;
   U8 digit;
   U32 allocSize = 0;

   TRC2(veSibBldSIB1Msg);

   bcchSchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);

   /* Filling Timing Info */
   tmngInf = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo); 
   tmngInf->sfn = schedSFN;
   tmngInf->subframe = VE_SIB1_SUBFRM_NO;

   VE_FILL_TKN_UINT(bcchSchMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(bcchSchMsg->message.choice, BCCH_DL_SCH_MSGTYP_C1);
   VE_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice, C1_SYSTEMINFOBLOCKTYP1);

   sibTyp1 = &(bcchSchMsg->message.val.c1.val.systemInfoBlockTyp1);
   VE_FILL_TKN_UINT(sibTyp1->pres, PRSNT_NODEF);
   /* cellAccessRelatedInfo */
   VE_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.pres, PRSNT_NODEF);
   /* plmn-IdentityList */
   plmnLst = &(sibTyp1->cellAccessRelatedInfo.plmn_IdentityLst);
   VE_FILL_TKN_UINT(plmnLst->noComp, VE_VAL_ONE);

   allocSize = ((plmnLst->noComp.val) * (sizeof(NhuPLMN_IdentityInfo)));

   VE_GET_MEM(nhDatReqEvntSdu, allocSize, &(plmnLst->member));

   for(idx = 0; idx < (plmnLst->noComp.val); idx++)
   {
      VE_FILL_TKN_UINT(plmnLst->member[idx].pres, PRSNT_NODEF);
      /* PLMN-IdentityInfo */
      VE_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.pres, PRSNT_NODEF);
      /* Allocating 3 instances of MCC digit */
      VE_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mcc.noComp,\
            VE_MCC_SIZE);
      VE_GET_MEM(nhDatReqEvntSdu,(VE_MCC_SIZE * (sizeof(NhuMCC_MNC_Digit))),
            &(plmnLst->member[idx].plmn_Identity.mcc.member));

      for(digit = 0; digit < VE_MCC_SIZE; digit++)
      {
         VE_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mcc.member[digit],\
               veCb.cellCb[0]->sysRelInfo.mcc[digit].val);
      }
      

      /* changes are incorporated*/
      /* Allocating  instances of MNC digit */
      VE_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mnc.noComp,\
         veCb.cellCb[0]->sysRelInfo.mnc_nocomp);
      VE_GET_MEM(nhDatReqEvntSdu, (veCb.cellCb[0]->sysRelInfo.mnc_nocomp * (sizeof(NhuMCC_MNC_Digit))),
         &(plmnLst->member[idx].plmn_Identity.mnc.member));
      for(digit = 0; digit < veCb.cellCb[0]->sysRelInfo.mnc_nocomp; digit++)
      {
         VE_FILL_TKN_UINT(plmnLst->member[idx].plmn_Identity.mnc.member[digit],\
               veCb.cellCb[0]->sysRelInfo.mnc[digit].val);
      }
      /* cellReservedForOperatorUse */
      VE_FILL_TKN_UINT(plmnLst->member[idx].cellReservedForOperatorUse,\
                     veDfltCellRsrvdForOperUse);   
            
      
   }

   /* TrackingAreaCode */
   sibTyp1->cellAccessRelatedInfo.trackingAreaCode.pres = PRSNT_NODEF;
   sibTyp1->cellAccessRelatedInfo.trackingAreaCode.len = VE_TRAC_LEN;
   VE_GET_MEM(nhDatReqEvntSdu, sizeof(U16),\
         &(sibTyp1->cellAccessRelatedInfo.trackingAreaCode.val));
   /* IOT changes are incorporated ID ccpu00117333*/
   /* fill tracking area code values */
   sibTyp1->cellAccessRelatedInfo.trackingAreaCode.val[1]=(U8)(veCb.cellCb[0]->sysRelInfo.trackAreaCode & 0xff);
   sibTyp1->cellAccessRelatedInfo.trackingAreaCode.val[0]=(U8)((veCb.cellCb[0]->sysRelInfo.trackAreaCode & 0xff00)>>8);
   /* CellIdentity  Bitstring of 28 bits len */
   sibTyp1->cellAccessRelatedInfo.cellIdentity.pres = PRSNT_NODEF;
   sibTyp1->cellAccessRelatedInfo.cellIdentity.len = VE_CELL_ID_LEN;
   VE_GET_MEM(nhDatReqEvntSdu, sizeof(U32),\
         &(sibTyp1->cellAccessRelatedInfo.cellIdentity.val));
   cmMemcpy((U8 *)(sibTyp1->cellAccessRelatedInfo.cellIdentity.val), \
         (U8 *)&(veCb.cellCb[0]->cellId), sizeof(U32));

   /* cellBarred */
   VE_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.cellBarred, veDfltCellBarred);
   /*  intraFreqReselection */
   VE_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.intraFreqReselection,\
         veDfltIntraFreqResel);
   /* csg-Indication */
   VE_FILL_TKN_UINT(sibTyp1->cellAccessRelatedInfo.csg_Ind, FALSE);
   /* csg-Identity left empty*/

   /* Q-RxLevMin */ 
   VE_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.q_RxLevMin,\
         veDfltqRxlevmin);
   VE_FILL_TKN_UINT(sibTyp1->cellSelectionInfo.q_RxLevMinOffset,\
         veDfltqRxlevminoffset);
   /* P-Max */
   VE_FILL_TKN_UINT(sibTyp1->p_Max, veDfltPMax);
   /* freqBandIndicator */
   VE_FILL_TKN_UINT(sibTyp1->freqBandIndicator,\
         veCb.cellCb[0]->sysRelInfo.freqBandInd);
   /*TODO Atul : Fill the Scheduling Info for the SIB3, SIB4 and SIB5 */
   /* SchedulingInfoList */
#ifndef SI_NEW
   VE_FILL_TKN_UINT(sibTyp1->schedulingInfoLst.noComp, VE_VAL_ONE);
#else
   VE_FILL_TKN_UINT(sibTyp1->schedulingInfoLst.noComp, VE_SI_CNT);
#endif
   VE_GET_MEM(nhDatReqEvntSdu, 
              (sizeof(NhuSchedulingInfo) * sibTyp1->schedulingInfoLst.noComp.val),
         &(sibTyp1->schedulingInfoLst.member));

   for(idx = 0; idx < (sibTyp1->schedulingInfoLst.noComp.val); idx++)
   {
      schedInf = &(sibTyp1->schedulingInfoLst.member[idx]);
      VE_FILL_TKN_UINT(schedInf->pres, PRSNT_NODEF);
#ifndef SI_NEW
      VE_FILL_TKN_UINT(schedInf->si_Periodicity,\
            veDfltSiPeriodicity);
      /* Zero means only SIB2 by default */
      VE_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp,\
            VE_VAL_ZERO);
#else
      VE_FILL_TKN_UINT(schedInf->si_Periodicity,\
            veDfltSiPeriodicity[idx]);
      if (idx == 0)
      {
         /* Zero means only SIB2 by default */
         VE_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp,\
               VE_VAL_ZERO);
      }
      else
      {
         VE_FILL_TKN_UINT(schedInf->sib_MappingInfo.noComp,\
               veDfltSiMapInfoCnt[idx]);
         VE_GET_MEM(nhDatReqEvntSdu, 
                    (sizeof(NhuSIB_Typ) * schedInf->sib_MappingInfo.noComp.val),
               &(schedInf->sib_MappingInfo.member));
         VE_FILL_TKN_UINT(schedInf->sib_MappingInfo.member[0],\
               veDfltSiMapInfoVal[idx]);
      }
#endif
   }

#ifdef LTE_TDD
   /* TDD-Config */
   VE_FILL_TKN_UINT(sibTyp1->tdd_Config.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(sibTyp1->tdd_Config.subframeAssignment,\
         veDfltSubframeAssignment);
   VE_FILL_TKN_UINT(sibTyp1->tdd_Config.specialSubfrmPatterns,\
         veDfltSpecialSubfrmPatterns);
#endif /* end of LTE_TDD */

   /* si-WindowLength */
   VE_FILL_TKN_UINT(sibTyp1->si_WindowLength,\
         veDfltSiWindowLength);
   /* systemInfoValueTag */
   VE_FILL_TKN_UINT(sibTyp1->systemInfoValueTag, veDfltSystemInfoValueTag);
   /* nonCriticalExtension */
   VE_FILL_TKN_UINT(sibTyp1->nonCriticalExtn.pres, PRSNT_NODEF);

   RETVALUE(ROK);
} /* End of veSibBldSIB1Msg */


/*
*
*       Fun:   veSibBldSIB2Msg
*
*       Desc:  Fills System Info message (DL BCCH DLSCH)
*
*       Ret:   
*
*       Notes: None
*
*       File:  ve_sib.c
*
*/
#ifdef ANSI
PUBLIC S16 veSibBldSIB2Msg
(
NhuDatReqSdus *nhDatReqEvntSdu,
U16 schedSFN,
U8  subframe
)
#else
PUBLIC S16 veSibBldSIB2Msg(nhDatReqEvntSdu, schedSFN, subframe)
NhuDatReqSdus *nhDatReqEvntSdu;
U16 schedSFN;
U8 subframe;
#endif
{
   NhuBCCH_DL_SCH_Msg *bcchSchMsg = NULLP;
   NhuSysInfocriticalExtns *critExt = NULLP;
   NhuTimingInfo *tmngInf = NULLP;
   NhuRACH_ConfigCmn *rachCfg = NULLP;
   NhuRACH_ConfigCmnpreambleInfo *preAmbInf = NULLP;
   NhuBCCH_Config *bcchCfg = NULLP;
   NhuPCCH_Config *pcchCfg = NULLP;
   NhuPRACH_ConfigSIB *prachCfg = NULLP;
   NhuPDSCH_ConfigCmn *pdschCfg = NULLP;
   NhuPUSCH_ConfigCmn *puschCfg = NULLP;
   NhuPUCCH_ConfigCmn *pucchCfg = NULLP;
   NhuSoundingRS_UL_ConfigCmn *srsCfg = NULLP;
   NhuUlnkPowerCntrlCmn *ulPwrCntrl = NULLP;
   /* NhuSysInfoBlockTyp2ac_BarringInfo *acBarringInfo = NULLP; */
   U8 idx = 0;

   NhuSysInfoBlockTyp2 *sib2 = NULLP;

   TRC2(veSibBldSIB2Msg);

   bcchSchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);
   /* Filling Timing Info */
   tmngInf = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo); 
   tmngInf->sfn = schedSFN;
   tmngInf->subframe = subframe;

   VE_FILL_TKN_UINT(bcchSchMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(bcchSchMsg->message.choice, BCCH_DL_SCH_MSGTYP_C1);
   VE_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice, C1_SYSTEMINFO);
   VE_FILL_TKN_UINT(bcchSchMsg->message.val.c1.val.systemInfo.pres,\
         PRSNT_NODEF);

   critExt = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns);

   VE_FILL_TKN_UINT(critExt->choice, CRITICALEXTNS_SYSTEMINFO_R8);
   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.nonCriticalExtn.pres,\
         PRSNT_NODEF);

   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.noComp,\
         VE_VAL_ONE);

   VE_GET_MEM(nhDatReqEvntSdu, (critExt->val.systemInfo_r8.sib_TypAndInfo.\
            noComp.val)*(sizeof(NhuSysInfo_r8_IEssib_TypAndInfoMember)),
         &(critExt->val.systemInfo_r8.sib_TypAndInfo.member));

   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].choice,\
         MEMBER_SIB2);
   /*-- For sib2 */
   sib2 = &(critExt->val.systemInfo_r8.sib_TypAndInfo.member[idx].val.sib2);
   VE_FILL_TKN_UINT(sib2->pres, PRSNT_NODEF);


   /*-Fill Radio Resource Config Common -*/
#ifdef LTERRC_REL9
    VE_FILL_TKN_UINT(sib2->radioResourceConfigCmn.pres, PRSNT_NODEF);

   /* RACH cfg */
   rachCfg = &(sib2->radioResourceConfigCmn.rach_ConfigCmn);
   VE_FILL_TKN_UINT(rachCfg->pres, PRSNT_NODEF);
   preAmbInf = &(rachCfg->preambleInfo);
   VE_FILL_TKN_UINT(preAmbInf->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(preAmbInf->numberOfRA_Preambles,\
         veDfltNumberOfRaPreambles);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.sizeOfRA_PreamblesGroupA,\
         veDflSizeOfRaPreamblesGroupA);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.messageSizeGroupA,\
         veDfltMessageSizeGroupA);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.messagePowerOffsetGroupB,\
         veDfltMessagePowerOffsetGroupB);

   VE_FILL_TKN_UINT(rachCfg->powerRampingParams.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(rachCfg->powerRampingParams.powerRampingStep,\
         veDfltPowerRampingStep);
   VE_FILL_TKN_UINT(rachCfg->powerRampingParams.preambleInitReceivedTgtPower,\
         veDfltPreambleInitRecvdTgtPwr);

   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.preambleTransMax,\
         veDfltPreambleTransMax);
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.ra_RspWindowSize,\
         veDfltRaRspWindowSize);
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.mac_ContentionResolutionTimer,\
         veDfltMacContentResTmr);

   VE_FILL_TKN_UINT(rachCfg->maxHARQ_Msg3Tx, veDfltMaxHARQMsg3Tx);

#else
   VE_FILL_TKN_UINT(sib2->radioResourceConfigCmn.pres, PRSNT_NODEF);

   /* RACH cfg */
   rachCfg = &(sib2->radioResourceConfigCmn.rach_Config);
   VE_FILL_TKN_UINT(rachCfg->pres, PRSNT_NODEF);
   preAmbInf = &(rachCfg->preambleInfo);
   VE_FILL_TKN_UINT(preAmbInf->pres, PRSNT_NODEF);
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(preAmbInf->numberOfRA_Preambles,\
         NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn52Enum);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.pres, NOTPRSNT);
#else
   VE_FILL_TKN_UINT(preAmbInf->numberOfRA_Preambles,\
         veDfltNumberOfRaPreambles);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.pres, PRSNT_NODEF);
#endif
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.sizeOfRA_PreamblesGroupA,\
         veDflSizeOfRaPreamblesGroupA);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.messageSizeGroupA,\
         veDfltMessageSizeGroupA);
   VE_FILL_TKN_UINT(preAmbInf->preamblesGroupAConfig.messagePowerOffsetGroupB,\
         veDfltMessagePowerOffsetGroupB);

   VE_FILL_TKN_UINT(rachCfg->powerRampingParams.pres, PRSNT_NODEF);
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(rachCfg->powerRampingParams.powerRampingStep,\
         NhuRACH_ConfigCmnpowerRampingParamspowerRampingStepdB2Enum);
#else
   VE_FILL_TKN_UINT(rachCfg->powerRampingParams.powerRampingStep,\
         veDfltPowerRampingStep);
#endif
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(rachCfg->powerRampingParams.preambleInitReceivedTgtPower,\
         NhuRACH_ConfigCmnpowerRampingParamspreambleInitReceivedTgtPowerdBm_104Enum);
#else
   VE_FILL_TKN_UINT(rachCfg->powerRampingParams.preambleInitReceivedTgtPower,\
         veDfltPreambleInitRecvdTgtPwr);
#endif

   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.pres, PRSNT_NODEF);
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.preambleTransMax,\
         NhuRACH_ConfigCmnra_SupervisionInfopreambleTransMaxn6Enum);
#else
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.preambleTransMax,\
         veDfltPreambleTransMax);
#endif
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.ra_RspWindowSize,\
         NhuRACH_ConfigCmnra_SupervisionInfora_RspWindowSizesf10Enum);
#else
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.ra_RspWindowSize,\
         veDfltRaRspWindowSize);
#endif
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.mac_ContentionResolutionTimer,\
         NhuRACH_ConfigCmnra_SupervisionInfomac_ContentionResolutionTimersf48Enum);
#else
   VE_FILL_TKN_UINT(rachCfg->ra_SupervisionInfo.mac_ContentionResolutionTimer,\
         veDfltMacContentResTmr);
#endif

   VE_FILL_TKN_UINT(rachCfg->maxHARQ_Msg3Tx, veDfltMaxHARQMsg3Tx);
#endif
   /* BCCH CFG */
   bcchCfg = &(sib2->radioResourceConfigCmn.bcch_Config);
   VE_FILL_TKN_UINT(bcchCfg->pres, PRSNT_NODEF);
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(bcchCfg->modificationPeriodCoeff, NhuBCCH_ConfigmodificationPeriodCoeffn4Enum);
#else
   VE_FILL_TKN_UINT(bcchCfg->modificationPeriodCoeff, veDfltModPeriodCoeff);
#endif

   /* PCCH CFG */
   pcchCfg = &(sib2->radioResourceConfigCmn.pcch_Config);
   VE_FILL_TKN_UINT(pcchCfg->pres, PRSNT_NODEF);
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(pcchCfg->defaultPagCycle, NhuPCCH_ConfigdefaultPagCyclerf128Enum);
   VE_FILL_TKN_UINT(pcchCfg->nB, NhuPCCH_ConfignBoneTEnum);
#else
   VE_FILL_TKN_UINT(pcchCfg->defaultPagCycle, veDfltDefaultPagCycle);
   VE_FILL_TKN_UINT(pcchCfg->nB, veDfltPCCHCfgNB);
#endif

   /* PRACH Cfg */
   prachCfg = &(sib2->radioResourceConfigCmn.prach_Config);
   VE_FILL_TKN_UINT(prachCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(prachCfg->rootSequenceIdx, veDfltRootSequenceIdx);
   VE_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.prach_ConfigIdx,\
         veDfltPrachCfgIdx);
   VE_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.highSpeedFlag,\
         (U8)veDfltHighSpeedFlag);/* klock warning fix */
   VE_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.zeroCorrelationZoneConfig,\
         veDfltZeroCorrelZoneCfg);
   VE_FILL_TKN_UINT(prachCfg->prach_ConfigInfo.prach_FreqOffset,\
         veDfltPrachFreqOffset);

#ifdef LTERRC_REL9
    /* PDSCH Cfg */
   pdschCfg = &(sib2->radioResourceConfigCmn.pdsch_ConfigCmn);
   VE_FILL_TKN_UINT(pdschCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(pdschCfg->referenceSignalPower, veDfltRefSignalPower);
   VE_FILL_TKN_UINT(pdschCfg->p_b, veDfltPDSCHCfgPB);
   /* PUSCH Cfg*/
   puschCfg = &(sib2->radioResourceConfigCmn.pusch_ConfigCmn);
   VE_FILL_TKN_UINT(puschCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.n_SB, veDfltNSB);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.hoppingMode,\
         veDfltHoppingMode);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.pusch_HoppingOffset,\
         veDfltPuschHoppingOff);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.enable64QAM,\
         veDfltEnable64QAM);

   /* UL-ReferenceSignalsPUSCH Cfg */
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.groupHoppingEnabled,\
         veDfltGroupHoppingEnabled);
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH,\
         veDfltGroupAssignPUSCH);
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled,\
         veDfltSeqHoppingEnabled);
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.cyclicShift,\
         veDfltCyclicShift);

   /* PUCCH Cfg */
   pucchCfg = &(sib2->radioResourceConfigCmn.pucch_ConfigCmn);
   VE_FILL_TKN_UINT(pucchCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(pucchCfg->deltaPUCCH_Shift, veDfltPUCCHDeltaShift);
   VE_FILL_TKN_UINT(pucchCfg->nRB_CQI, veDfltPUCCHnRBCQI);
   VE_FILL_TKN_UINT(pucchCfg->nCS_AN, veDfltPUCCHnCSAN);
   VE_FILL_TKN_UINT(pucchCfg->n1PUCCH_AN, veDfltNlPUCCHAn);

   /* SoundingsRS UL Cfg */
   srsCfg = &(sib2->radioResourceConfigCmn.soundingRS_UL_ConfigCmn);
   VE_FILL_TKN_UINT(srsCfg->choice, SOUNDINGRS_UL_CONFIGCMN_SETUP);
   VE_FILL_TKN_UINT(srsCfg->val.setup.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(srsCfg->val.setup.srs_BandwidthConfig,\
         veDfltSrsBandwidthCfg);
   VE_FILL_TKN_UINT(srsCfg->val.setup.srs_SubfrmConfig,\
         veDfltSrsSubfrmCfg);
   VE_FILL_TKN_UINT(srsCfg->val.setup.ackNackSRS_SimultaneousTransmission,\
         veDfltAckNackSrsSimTrns);
   VE_FILL_TKN_UINT(srsCfg->val.setup.srs_MaxUpPts, veDfltSrsMaxUpPts);

   /* Uplink Power Control Config */
   ulPwrCntrl = &(sib2->radioResourceConfigCmn.uplinkPowerCntrlCmn);
   VE_FILL_TKN_UINT(ulPwrCntrl->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(ulPwrCntrl->p0_NominalPUSCH, veDfltP0NominalPUSCH);
   VE_FILL_TKN_UINT(ulPwrCntrl->alpha, veDfltULPwrCtrlAlpha);
   VE_FILL_TKN_UINT(ulPwrCntrl->p0_NominalPUCCH, veDfltP0NominalPUCCH);

   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format1,\
         veDfltDeltaFPUCCHFormat1);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format1b,\
         veDfltDeltaFPUCCHFormat1b);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2,\
         veDfltDeltaFPUCCHFormat2);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2a,\
         veDfltDeltaFPUCCHFormat2a);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2b,\
         veDfltDeltaFPUCCHFormat2b);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaPreambleMsg3,\
         veDfltUlDeltaPreambleMsg3);

#else
   /* PDSCH Cfg */
   pdschCfg = &(sib2->radioResourceConfigCmn.pdsch_Config);
   VE_FILL_TKN_UINT(pdschCfg->pres, PRSNT_NODEF);
/* MSPD changes */
#ifndef MSPD
   VE_FILL_TKN_UINT(pdschCfg->referenceSignalPower, veDfltRefSignalPowerSib);/* Ticket id:ccpu00117377 removal of hard coding for reference signal power to PHY hence introduced new varible for sending in SIB */
#else
   VE_FILL_TKN_UINT(pdschCfg->referenceSignalPower, veDfltRefSignalPower);/* Ticket id:ccpu00117377 removal of hard coding for reference signal power to PHY hence introduced new varible for sending in SIB */
#endif
   VE_FILL_TKN_UINT(pdschCfg->p_b, veDfltPDSCHCfgPB);

   /* PUSCH CFG */
   puschCfg = &(sib2->radioResourceConfigCmn.pusch_Config);
   VE_FILL_TKN_UINT(puschCfg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.n_SB, veDfltNSB);
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.hoppingMode,\
         NhuPUSCH_ConfigCmnpusch_ConfigBasichoppingModeinterSubFrameEnum);
#else
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.hoppingMode,\
         veDfltHoppingMode);
#endif
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.pusch_HoppingOffset,\
         veDfltPuschHoppingOff);
   VE_FILL_TKN_UINT(puschCfg->pusch_ConfigBasic.enable64QAM,\
         (U8)veDfltEnable64QAM);/* klock warning fix */

   /* UL-ReferenceSignalsPUSCH Cfg */
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.groupHoppingEnabled,\
         (U8)veDfltGroupHoppingEnabled);/* klock warning fix */
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH,\
         veDfltGroupAssignPUSCH);
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled,\
         (U8)veDfltSeqHoppingEnabled);/* klock warning fix */
   VE_FILL_TKN_UINT(puschCfg->ul_ReferenceSignalsPUSCH.cyclicShift,\
         veDfltCyclicShift);

   /* PUCCH Cfg */
   pucchCfg = &(sib2->radioResourceConfigCmn.pucch_Config);
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(pucchCfg->pres, NOTPRSNT);
#else
   VE_FILL_TKN_UINT(pucchCfg->pres, PRSNT_NODEF);
#endif
   VE_FILL_TKN_UINT(pucchCfg->deltaPUCCH_Shift, veDfltPUCCHDeltaShift);
   VE_FILL_TKN_UINT(pucchCfg->nRB_CQI, veDfltPUCCHnRBCQI);
   VE_FILL_TKN_UINT(pucchCfg->nCS_AN, veDfltPUCCHnCSAN);
   VE_FILL_TKN_UINT(pucchCfg->n1PUCCH_AN, veDfltNlPUCCHAn);

   /* SoundingsRS UL Cfg */
   srsCfg = &(sib2->radioResourceConfigCmn.soundingRS_UL_Config);
/* MSPD changes */
#ifdef MSPD 
   VE_FILL_TKN_UINT(srsCfg->choice, SOUNDINGRS_UL_CONFIGCMN_RELEASE);
#else
   VE_FILL_TKN_UINT(srsCfg->choice, SOUNDINGRS_UL_CONFIGCMN_SETUP);
   VE_FILL_TKN_UINT(srsCfg->val.setup.pres, PRSNT_NODEF);
#ifdef MSPD
   VE_FILL_TKN_UINT(srsCfg->val.setup.srs_BandwidthConfig,\
         NhuSoundingRS_UL_ConfigCmnsetupsrs_BandwidthConfigbw2Enum);
#else
   VE_FILL_TKN_UINT(srsCfg->val.setup.srs_BandwidthConfig,\
         veDfltSrsBandwidthCfg);
#endif
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(srsCfg->val.setup.srs_SubfrmConfig,\
         NhuSoundingRS_UL_ConfigCmnsetupsrs_SubfrmConfigsc15Enum); /* change SRS subfrm from 0 to 15 */
#else
   VE_FILL_TKN_UINT(srsCfg->val.setup.srs_SubfrmConfig,\
         veDfltSrsSubfrmCfg);
#endif
   VE_FILL_TKN_UINT(srsCfg->val.setup.ackNackSRS_SimultaneousTransmission,\
         (U8)veDfltAckNackSrsSimTrns); /* klock warning fix */
   VE_FILL_TKN_UINT(srsCfg->val.setup.srs_MaxUpPts, veDfltSrsMaxUpPts);
#endif

   /* Uplink Power Control Config */
   ulPwrCntrl = &(sib2->radioResourceConfigCmn.uplinkPowerCntrl); 
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(ulPwrCntrl->pres, NOTPRSNT);
#else
   VE_FILL_TKN_UINT(ulPwrCntrl->pres, PRSNT_NODEF);
#endif
   VE_FILL_TKN_UINT(ulPwrCntrl->p0_NominalPUSCH, veDfltP0NominalPUSCH);
   VE_FILL_TKN_UINT(ulPwrCntrl->alpha, veDfltULPwrCtrlAlpha);
   VE_FILL_TKN_UINT(ulPwrCntrl->p0_NominalPUCCH, veDfltP0NominalPUCCH);

   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format1,\
         veDfltDeltaFPUCCHFormat1);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format1b,\
         veDfltDeltaFPUCCHFormat1b);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2,\
         veDfltDeltaFPUCCHFormat2);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2a,\
         veDfltDeltaFPUCCHFormat2a);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaFLst_PUCCH.deltaF_PUCCH_Format2b,\
         veDfltDeltaFPUCCHFormat2b);
   VE_FILL_TKN_UINT(ulPwrCntrl->deltaPreambleMsg3,\
         veDfltUlDeltaPreambleMsg3);
#endif

/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(sib2->radioResourceConfigCmn.ul_CyclicPrefixLength,\
         NhuUL_CyclicPrefixLengthlen1Enum);
#else
   VE_FILL_TKN_UINT(sib2->radioResourceConfigCmn.ul_CyclicPrefixLength,\
         veDfltUlCyclicPrefixLength);
#endif

   /*-Fill ue Timers And Constants -*/
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.pres, PRSNT_NODEF);
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t300,
         NhuUE_TmrsAndConstst300ms1000Enum);
#else
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t300,
         veDfltTmrsAndConstsT300);
#endif
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t301,
         NhuUE_TmrsAndConstst301ms1000Enum);
#else
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t301,
         veDfltTmrsAndConstsT301);
#endif
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t310,
         NhuUE_TmrsAndConstst310ms1000Enum);
#else
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t310,
         veDfltTmrsAndConstsT310);
#endif
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.n310,
         NhuUE_TmrsAndConstsn310n1Enum);
#else
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.n310,
         veDfltTmrsAndConstsN310);
#endif
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t311,
         NhuUE_TmrsAndConstst311ms10000Enum);
#else
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.t311,
         veDfltTmrsAndConstsT311);
#endif
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.n311,
         NhuUE_TmrsAndConstsn311n1Enum);
#else
   VE_FILL_TKN_UINT(sib2->ue_TmrsAndConsts.n311,
         veDfltTmrsAndConstsN311);
#endif

   /*-Fill Freuency Infomration -*/
   VE_FILL_TKN_UINT(sib2->freqInfo.pres, PRSNT_NODEF);
#ifndef LTE_TDD
    /* Uplink Frequency has been changed */
#ifndef MSPD
#ifdef VE_DONGWON_OAM
   /*OAM_FIX : Replaced the hardcoded values with the default variables*/
   VE_FILL_TKN_UINT(sib2->freqInfo.ul_CarrierFreq,veDfltUlEarFcn); /* 36.101 5.7.3  Specific 782Mhz[23230]*/
#else
   VE_FILL_TKN_UINT(sib2->freqInfo.ul_CarrierFreq,23230); /* 36.101 5.7.3  Specific 782Mhz[23230]*/
#endif
   VE_FILL_TKN_UINT(sib2->freqInfo.ul_Bandwidth, veDfltUlBandwidth);
#endif
#endif
   VE_FILL_TKN_UINT(sib2->freqInfo.additionalSpectrumEmission,\
         veDfltAddSpectEmn);
   /* skipping MBSFN subframe config */
   /* TimeAlignmentTimer */
/* MSPD changes */
#ifdef MSPD
   VE_FILL_TKN_UINT(sib2->timeAlignmentTimerCmn, NhuTimeAlignmentTimersf750Enum);
#else
   VE_FILL_TKN_UINT(sib2->timeAlignmentTimerCmn, veDfltTimeAlignTmrCmn);
#endif
   RETVALUE(ROK);
} /* End of veSibBldSIB2Msg */


/*
*
*       Fun:   veSibBldSIB3Msg
*
*       Desc:  Fills System Info message (DL BCCH DLSCH)
*
*       Ret:   
*
*       Notes: None
*
*       File:  ve_sib.c
*
*/
#ifdef ANSI
PUBLIC S16 veSibBldSIB3Msg
(
NhuDatReqSdus *nhDatReqEvntSdu,
U16 schedSFN,
U8  subframe
)
#else
PUBLIC S16 veSibBldSIB3Msg(nhDatReqEvntSdu, schedSFN, subframe)
NhuDatReqSdus *nhDatReqEvntSdu;
U16 schedSFN;
U8 subframe;
#endif
{
   NhuBCCH_DL_SCH_Msg *bcchSchMsg = NULLP;
   NhuSysInfocriticalExtns *critExt = NULLP;
   NhuTimingInfo *tmngInf = NULLP;


   NhuSysInfoBlockTyp3 *sib3 = NULLP;

   TRC2(veSibBldSIB3Msg);

   bcchSchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);
   /* Filling Timing Info */
   tmngInf = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo); 
   tmngInf->sfn = schedSFN;
   tmngInf->subframe = subframe;

   VE_FILL_TKN_UINT(bcchSchMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(bcchSchMsg->message.choice, BCCH_DL_SCH_MSGTYP_C1);
   VE_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice, C1_SYSTEMINFO);
   VE_FILL_TKN_UINT(bcchSchMsg->message.val.c1.val.systemInfo.pres,\
         PRSNT_NODEF);

   critExt = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns);
   VE_FILL_TKN_UINT(critExt->choice, CRITICALEXTNS_SYSTEMINFO_R8);
   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.nonCriticalExtn.pres,\
         PRSNT_NODEF);

   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.noComp,\
         VE_VAL_ONE);

   VE_GET_MEM(nhDatReqEvntSdu, (critExt->val.systemInfo_r8.sib_TypAndInfo.\
            noComp.val)*(sizeof(NhuSysInfo_r8_IEssib_TypAndInfoMember)),
         &(critExt->val.systemInfo_r8.sib_TypAndInfo.member));

   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].choice,\
         MEMBER_SIB3);
   /*-- For sib3 */
   sib3 = &(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].val.sib3);
   /* Code for Filling SIB3*/
   VE_FILL_TKN_UINT(sib3->pres, PRSNT_NODEF);

   /*-Fill Cell Reselection Information Common -*/
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.pres, PRSNT_NODEF);
#ifdef VE_DONGWON_OAM
   /*OAM_FIX : Replaced the hardcoded values with the default variables*/
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.q_Hyst,veDfltQHyst);
#else
  VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.q_Hyst,
         NhuSysInfoBlockTyp3cellReselectionInfoCmnq_HystdB0Enum);
#endif   
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
         speedStateReselectionPars.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
         speedStateReselectionPars.mobilityStateParams.pres,
         PRSNT_NODEF);
#ifdef VE_DONGWON_OAM
     /*OAM_FIX : Replaced the hardcoded values with the default variables*/
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
         speedStateReselectionPars.mobilityStateParams.t_Evaluation,
        veDfltS3t_Evaluation);
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
         speedStateReselectionPars.mobilityStateParams.t_HystNormal,
         veDfltS3t_HystNormal);
#else
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
         speedStateReselectionPars.mobilityStateParams.t_Evaluation,
         NhuMobilityStateParamst_Evaluations30Enum);
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
         speedStateReselectionPars.mobilityStateParams.t_HystNormal,
         NhuMobilityStateParamst_HystNormals30Enum);
#endif

   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
         speedStateReselectionPars.mobilityStateParams.\
         n_CellChangeMedium, veDfltS3nCellChangeMedium);
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
         speedStateReselectionPars.mobilityStateParams.\
         n_CellChangeHigh, veDfltS3nCellChangeHigh);
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
         speedStateReselectionPars.q_HystSF.pres,PRSNT_NODEF);
#ifndef VE_DONGWON_OAM
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
        speedStateReselectionPars.q_HystSF.sf_Medium,
        NhuSysInfoBlockTyp3cellReselectionInfoCmnspeedStateReselectionParsq_HystSFsf_MediumdB_6Enum
        );
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
         speedStateReselectionPars.q_HystSF.sf_High,
         NhuSysInfoBlockTyp3cellReselectionInfoCmnspeedStateReselectionParsq_HystSFsf_HighdB_6Enum
         );
#else

     /*OAM_FIX : Replaced the hardcoded values with the default variables*/
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
        speedStateReselectionPars.q_HystSF.sf_Medium,
        veDfltQHystSFMedium);
   VE_FILL_TKN_UINT(sib3->cellReselectionInfoCmn.\
         speedStateReselectionPars.q_HystSF.sf_High,
         veDfltQHystSFHigh);
#endif
   /*-Fill Cell Reselection Serving Frequency Information -*/
   VE_FILL_TKN_UINT(sib3->cellReselectionServingFreqInfo.pres,
         PRSNT_NODEF);
   VE_FILL_TKN_UINT(sib3->cellReselectionServingFreqInfo.s_NonIntraSearch,
         veDfltS3sNonIntraSearch);
   VE_FILL_TKN_UINT(sib3->cellReselectionServingFreqInfo.threshServingLow,
         veDfltS3ThreshServingLow);
   VE_FILL_TKN_UINT(sib3->cellReselectionServingFreqInfo.\
         cellReselectionPriority, veDfltS3CellReselectionPriority);

   /*-Fill Intra-Frequency Cell Reselection Information -*/
   VE_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.q_RxLevMin,veDfltS3qRxLevMin);
   VE_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.p_Max, veDfltS3pMax);
   VE_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.s_IntraSearch, veDfltS3sIntraSearch);
   VE_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.allowedMeasBandwidth, veDfltS3AllowedMeasBandwidth);
   /* klock warning fix */
   VE_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.presenceAntennaPort1,(U8)veDfltS3PresenceAntennaPort1 );
   VE_FILL_BIT_STR(sib3->intraFreqCellReselectionInfo.neighCellConfig, 2, veDfltS3NeighCellCfg,
   nhDatReqEvntSdu); 
   VE_FILL_TKN_UINT(sib3->intraFreqCellReselectionInfo.t_ReselectionEUTRA, veDfltS3tReselectionEUTRA);
   RETVALUE(ROK);
}


/*
*
*       Fun:   veSibBldSIB4Msg
*
*       Desc:  Fills System Info message SIB4
*
*       Ret:   
*
*       Notes: None
*
*       File:  ve_sib.c
*
*/
#ifdef ANSI
PUBLIC S16 veSibBldSIB4Msg
(
NhuDatReqSdus *nhDatReqEvntSdu,
U16 schedSFN,
U8  subframe
)
#else
PUBLIC S16 veSibBldSIB4Msg(nhDatReqEvntSdu, schedSFN, subframe)
NhuDatReqSdus *nhDatReqEvntSdu;
U16 schedSFN;
U8 subframe;
#endif
{
   NhuBCCH_DL_SCH_Msg *bcchSchMsg = NULLP;
   NhuSysInfocriticalExtns *critExt = NULLP;
   NhuTimingInfo *tmngInf = NULLP;
 
   U8 idx = 0;

   NhuSysInfoBlockTyp4 *sib4 = NULLP;

   TRC2(veSibBldSIB4Msg);

   bcchSchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);
   /* Filling Timing Info */
   tmngInf = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo); 
   tmngInf->sfn = schedSFN;
   tmngInf->subframe = subframe;

   VE_FILL_TKN_UINT(bcchSchMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(bcchSchMsg->message.choice, BCCH_DL_SCH_MSGTYP_C1);
   VE_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice, C1_SYSTEMINFO);
   VE_FILL_TKN_UINT(bcchSchMsg->message.val.c1.val.systemInfo.pres,\
         PRSNT_NODEF);

   critExt = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns);
   VE_FILL_TKN_UINT(critExt->choice, CRITICALEXTNS_SYSTEMINFO_R8);
   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.nonCriticalExtn.pres,\
         PRSNT_NODEF);

   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.noComp,\
         VE_VAL_ONE);

   VE_GET_MEM(nhDatReqEvntSdu, (critExt->val.systemInfo_r8.sib_TypAndInfo.\
            noComp.val)*(sizeof(NhuSysInfo_r8_IEssib_TypAndInfoMember)),
         &(critExt->val.systemInfo_r8.sib_TypAndInfo.member));

   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].choice,\
         MEMBER_SIB4);

   sib4 =&(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].\
         val.sib4);
   VE_FILL_TKN_UINT(sib4->pres, PRSNT_NODEF);

   /*-Fill Intra-Frequency Neighbouring Cell List-*/
   /* klock warning fix */
   VE_FILL_TKN_UINT(sib4->intraFreqNeighCellLst.noComp, (U16)veDfltS4MaxCellIntra);
   VE_GET_MEM(nhDatReqEvntSdu,
         (sib4->intraFreqNeighCellLst.noComp.val)
         *(sizeof(NhuIntraFreqNeighCellInfo)),
         &(sib4->intraFreqNeighCellLst.member));
   for ( idx = 0;
         idx < sib4->intraFreqNeighCellLst.noComp.val;
         idx++
       )
   {
      VE_FILL_TKN_UINT(sib4->intraFreqNeighCellLst.member[idx].\
            pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(sib4->intraFreqNeighCellLst.member[idx].\
            physCellId, veDfltS4PhysCellId);
      VE_FILL_TKN_UINT(sib4->intraFreqNeighCellLst.member[idx].\
            q_OffsetCell,
            veDfltS4QOffsetCell);
   }

   /*-Fill Intra-Frequency Black Listed Cell List-*/
   /* klock warning fix */
   VE_FILL_TKN_UINT(sib4->intraFreqBlackCellLst.noComp, (U16)veDfltS4MaxCellBlack);
   VE_GET_MEM(nhDatReqEvntSdu,
         (sib4->intraFreqBlackCellLst.noComp.val)
         *(sizeof(NhuPhysCellIdRange)),
         &(sib4->intraFreqBlackCellLst.member));
   for (idx = 0;
         idx < sib4->intraFreqBlackCellLst.noComp.val; idx++)
   {
      VE_FILL_TKN_UINT(sib4->intraFreqBlackCellLst.member[idx].\
            pres, PRSNT_NODEF);
      VE_FILL_TKN_UINT(sib4->intraFreqBlackCellLst.member[idx].\
            start, veDfltS4BCStart);
      VE_FILL_TKN_UINT(sib4->intraFreqBlackCellLst.member[idx].\
            range, veDfltS4BCRange);
   }

   VE_FILL_TKN_UINT(sib4->csg_PhysCellIdRange.pres, NOTPRSNT);
   VE_FILL_TKN_UINT(sib4->csg_PhysCellIdRange.start, veDfltS4PCStart);
   VE_FILL_TKN_UINT(sib4->csg_PhysCellIdRange.range,
         veDfltS4BCRange);

   RETVALUE(ROK);
}

/*
*
*       Fun:   veSibBldSIB5Msg
*
*       Desc:  Fills System Info message SIB5 
*
*       Ret:   
*
*       Notes: None
*
*       File:  ve_sib.c
*
*/
#ifdef ANSI
PUBLIC S16 veSibBldSIB5Msg
(
NhuDatReqSdus *nhDatReqEvntSdu,
U16 schedSFN,
U8  subframe
)
#else
PUBLIC S16 veSibBldSIB5Msg(nhDatReqEvntSdu, schedSFN, subframe)
NhuDatReqSdus *nhDatReqEvntSdu;
U16 schedSFN;
U8 subframe;
#endif
{
   NhuBCCH_DL_SCH_Msg *bcchSchMsg = NULLP;
   NhuSysInfocriticalExtns *critExt = NULLP;
   NhuTimingInfo *tmngInf = NULLP;
 
   U8 idx1,idx2 = 0;

   NhuSysInfoBlockTyp5 *sib5 = NULLP;

   TRC2(veSibBldSIB5Msg);

   bcchSchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);
   /* Filling Timing Info */
   tmngInf = &(nhDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo); 
   tmngInf->sfn = schedSFN;
   tmngInf->subframe = subframe;

   VE_FILL_TKN_UINT(bcchSchMsg->pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(bcchSchMsg->message.choice, BCCH_DL_SCH_MSGTYP_C1);
   VE_FILL_TKN_UINT(bcchSchMsg->message.val.c1.choice, C1_SYSTEMINFO);
   VE_FILL_TKN_UINT(bcchSchMsg->message.val.c1.val.systemInfo.pres,\
         PRSNT_NODEF);

   critExt = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns);
   VE_FILL_TKN_UINT(critExt->choice, CRITICALEXTNS_SYSTEMINFO_R8);
   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.pres, PRSNT_NODEF);
   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.nonCriticalExtn.pres,\
         PRSNT_NODEF);

   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.noComp,\
         VE_VAL_ONE);

   VE_GET_MEM(nhDatReqEvntSdu, (critExt->val.systemInfo_r8.sib_TypAndInfo.\
            noComp.val)*(sizeof(NhuSysInfo_r8_IEssib_TypAndInfoMember)),
         &(critExt->val.systemInfo_r8.sib_TypAndInfo.member));

   VE_FILL_TKN_UINT(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].choice,\
         MEMBER_SIB5);

   sib5 = &(critExt->val.systemInfo_r8.sib_TypAndInfo.member[0].
                              val.sib5);
   VE_FILL_TKN_UINT(sib5->pres, PRSNT_NODEF);
   /*-Fill Inter-Frequency Carrier Frequency List -*/
   VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.noComp, 1);
   VE_GET_MEM(nhDatReqEvntSdu, (sib5->interFreqCarrierFreqLst.\
            noComp.val)
         *(sizeof(NhuInterFreqCarrierFreqInfo)),
         &(sib5->interFreqCarrierFreqLst.member));
   for (idx1 = 0;
         idx1 < sib5->interFreqCarrierFreqLst.noComp.val; idx1++)
   {
      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].pres,
            PRSNT_NODEF);
      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].dl_CarrierFreq,
            veDfltS5DlCarrierFreq);
      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].q_RxLevMin,
            veDfltS5QRxLevMin);
      /*VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].p_Max,
            veDfltInterFreqpMax); p-Max is not present */
      sib5->interFreqCarrierFreqLst.member[idx1].p_Max.pres = NOTPRSNT;
      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].t_ReselectionEUTRA,
            veDfltS5InterFreqtReselection);
      /*VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].t_ReselectionEUTRA_SF.pres,
           NOTPRSNT);*/
      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            threshX_High, veDfltS5ThreshXHigh);
      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            threshX_Low, veDfltS5ThreshXLow);
      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            allowedMeasBandwidth, veDfltS5AllowedMeasBandwidth);
/* klock warning fix */
      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            presenceAntennaPort1, (U8)veDfltS5PresenceAntennaPort1);
      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            cellReselectionPriority, veDfltS5CellReselectionPriority);
      VE_FILL_BIT_STR(sib5->interFreqCarrierFreqLst.member[idx1].\
            neighCellConfig,2, veDfltS5NeighCellCfg, nhDatReqEvntSdu);
      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            q_OffsetFreq, veDfltS5QOffsetFreq);
      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            interFreqNeighCellLst.noComp, 1);
      VE_GET_MEM(nhDatReqEvntSdu, (sib5->interFreqCarrierFreqLst.\
               member[idx1].interFreqNeighCellLst.noComp.val)
            *(sizeof(NhuInterFreqNeighCellInfo)),
            &(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqNeighCellLst.member));
      for (idx2 = 0;
            idx2 < sib5->interFreqCarrierFreqLst.member[idx1].\
            interFreqNeighCellLst.noComp.val;
            idx2++)
      {
         VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqNeighCellLst.member[idx2].pres,
               PRSNT_NODEF);
         VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].
               interFreqNeighCellLst.member[idx2].
               physCellId, 1);
         VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].
               interFreqNeighCellLst.member[idx2].q_OffsetCell,
               0);
      }

      VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
            interFreqBlackCellLst.noComp, 1);
#ifdef LTERRC_REL9
      VE_GET_MEM(nhDatReqEvntSdu, (sib5->interFreqCarrierFreqLst.
               member[idx1].interFreqBlackCellLst.noComp.val)
            *(sizeof(NhuPhysCellIdRange)),
            &(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member));

      for (idx2 = 0;
            idx2 < sib5->interFreqCarrierFreqLst.member[idx1].\
            interFreqBlackCellLst.noComp.val; idx2++)
      {
         VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].physCellIdRange.pres, 1);
         VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].physCellIdRange.start, 1);
         VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].physCellIdRange.range, 1);
      }
#else
      VE_GET_MEM(nhDatReqEvntSdu, (sib5->interFreqCarrierFreqLst.
               member[idx1].interFreqBlackCellLst.noComp.val)
            *(sizeof(NhuInterFreqBlackCellInfo)),
            &(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member));
      for (idx2 = 0;
            idx2 < sib5->interFreqCarrierFreqLst.member[idx1].\
            interFreqBlackCellLst.noComp.val; idx2++)
      {
         VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].pres, PRSNT_NODEF);
         VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].physCellIdRange.pres, 1);
         VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].physCellIdRange.start, 1);
         VE_FILL_TKN_UINT(sib5->interFreqCarrierFreqLst.member[idx1].\
               interFreqBlackCellLst.member[idx2].physCellIdRange.range, 1);
      }
#endif
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   vePagHdlTmrInd
*
*       Desc:  Function handler called every Tmr indication.
*
*       Ret:   
*
*       Notes: None
*
*       File:  ve_sib.c
*
*/
#ifdef ANSI
PUBLIC S16 vePagHdlTmrInd
(
 RgrTtiIndInfo *tmngInfo
)
#else
PUBLIC S16 vePagHdlTmrInd(tmngInfo)
RgrTtiIndInfo *tmngInfo;
#endif
{
   CmLList         *node;
   VeUePagingInfo  *uePagingInfo = NULLP;
   Pst           *pst = NULLP;
   NhuDatReqSdus *pagMsg = NULLP; /* Added pagMsg */

   TRC2(vePagHdlTmrInd);

   node = NULLP;
   pst = &veCb.nhuSap[0]->pst;
   pst->event   = EVTNHUDATREQ;

#ifdef DEBUGP   
    VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "vePagHdlTmrInd:\
            Page Module Timer Ind. Handler\n"));
#endif

   /* For all the UEs in the Pending Paging List, compute
    * if there is any paging message to be scheduled
    */
   node = veCb.cellCb[0]->pendPagUesLst.first;
   while(node)
   {
      uePagingInfo =  (VeUePagingInfo *)node->node;
      /* Check if this UE needs to be scheduled for paging */ 
      if((((tmngInfo->sfn + VE_SFN_INCREMENT) % VE_MAX_SFN)  % (uePagingInfo->T)) == (uePagingInfo->pf))
      {
          /* Check if the RRC Paging message is constructed and send it out */
         if(uePagingInfo->pagMsg)
         {
            uePagingInfo->pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo.sfn = (tmngInfo->sfn + VE_SFN_INCREMENT) % VE_MAX_SFN;
            uePagingInfo->pagMsg->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.timingInfo.subframe = uePagingInfo->po;
         
            /* Allocate memory */
            VE_ALLOCEVNT(&pagMsg, sizeof(NhuDatReqSdus));
            cmMemcpy((U8*)pagMsg,(U8*)uePagingInfo->pagMsg, sizeof(NhuDatReqSdus));

            /* Issue the primitive */
            if(ROK != VeLiNhuDatReq(pst, veCb.nhuSap[0]->spId,pagMsg))
            {
               VE_DBG_ERROR((VE_PRNT_BUF,"vePagHdlTmrInd: VeLiNhuDatReq failed.\n"));
               RETVALUE(RFAILED);
            } /* end of if statement */

         } /* end of if statement */
         else
         {
            VE_DBG_ERROR((VE_PRNT_BUF,"vePagHdlTmrInd: RRC Paging Message not available.\n"));
            RETVALUE(RFAILED);

         } /* end of else part */

      } /* end of if statement */
      node = node->next;
   } /* end of while loop */

   RETVALUE(ROK);
} /* vePagHdlTmrInd */



/*
*
*       Fun:   veSibHdlTmrInd
*
*       Desc:  Function handler called every Tmr indication.
*
*       Ret:   
*
*       Notes: None
*
*       File:  ve_sib.c
*
*/
#ifdef ANSI
PUBLIC S16 veSibHdlTmrInd
(
RgrTtiIndInfo *tmngInfo
)
#else
PUBLIC S16 veSibHdlTmrInd(tmngInfo)
RgrTtiIndInfo *tmngInfo;
#endif
{
#ifndef SI_NEW
   U8              bitMap = 0;
   CmLteTimingInfo tmnInf;
   U16             sfn;
#endif

   TRC2(veSibHdlTmrInd);

#ifndef SI_NEW
#ifdef DEBUGP   
   VE_LYR_DBGP(DBGMASK_SM, (veCb.init.prntBuf, "veSibHdlTmrInd:\
            SIB Module Timer Ind. Handler\n"));
#endif

   /* Updating SFN */
   veCb.cellCb[0]->sfn = tmngInfo->sfn;
   sfn = (tmngInfo->sfn + VE_SFN_INCREMENT) % VE_MAX_SFN;

   if ((sfn % VE_MIB_PERIOD) == 0)
   {
      bitMap |= VE_MIB_PRESENT;
   }
  
   /*RNT:Transmission of SIBs will be controleed by this compilation
    flag */
#ifdef VE_SIBS_ENBLD
   if ((sfn % VE_SIB1_PERIOD) == 0)
   {
      bitMap |= VE_SIB1_PRESENT;
   }
   if ((sfn  % VE_SIB2_PERIOD) == 0)
   {
      bitMap |= VE_SIB2_PRESENT;
   }
#endif
   tmnInf.sfn = sfn;
/* MSPD changes */
#ifdef MSPD
   {
      static int iCpuCounter = 0;
      /*U32 mem;  */
         if((iCpuCounter  % 1000) == 0) /* print for every 1000 frames = 1sec*/
         {
           /* printf("%d \n", (iCpuCounter*10));*/
/*        VE_DBG_ERROR(("TmrInd():  sfn=%d Total TTI Count=%d \n", sfn,(iCpuCounter*10)));*/
            printf("TmrInd():  sfn=%d Total TTI Count=%d \n", sfn,(iCpuCounter*10));
/*            SRegInfoShow(0,&mem);*/
         }
         iCpuCounter ++;
   }
#endif
   /* Sending every fourth subframe*/
   if (bitMap & VE_MIB_PRESENT)    
   {
      tmnInf.subframe = VE_MIB_SUBFRAME;
      /* Build and send MIB */
      veSndCmnNhuDatReq(NHU_MSG_DLBCCHBCH, VE_SYS_MIB, tmnInf);
   }
#ifdef VE_SIBS_ENBLD
   if (bitMap & VE_SIB2_PRESENT) 
   {
      tmnInf.subframe = VE_SIB2_SUBFRAME; /* SIB 2 should be on subframe no. 0 */
      /* Build and send SIB1 */
      veSndCmnNhuDatReq(NHU_MSG_DLBCCHDLSCH, VE_SYS_SIB2, tmnInf);
   }

   if (bitMap & VE_SIB1_PRESENT) 
   {
      tmnInf.subframe = VE_SIB1_SUBFRAME; /* SIB1 on subframe no. 5 */
      /* Build and send SIB2 */
      veSndCmnNhuDatReq(NHU_MSG_DLBCCHDLSCH, VE_SYS_SIB1, tmnInf);
   }
#endif
#endif
   RETVALUE(ROK);
} /* veSibHdlTmrInd */
#endif
/********************************************************************30**

         End of file:     ve_sib.c@@/main/2 - Wed Dec  1 08:44:30 2010

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
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
