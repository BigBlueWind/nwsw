/**
 * @file    ueh_bldmsg.c
 * @brief This file contains the functions for building UEH configuration and messages
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "lueh.h"
#include "lueh.x" 
#include "ueh_eut_x.h" 
#include "ueh.h" 
#include "s1ap_cm.h"
#include "ueh_srs_res_mgmt.h"
#include "ueh_cmn_sec.h"
#include "log.h"

#ifdef RRM_PRL_FLAG 
#include "trace.h"
#endif

PUBLIC S16 uehExtractKeys ARGS(( UehUeSecCfg  *secCfg, TknStrBSXL  *secKey));

extern U16 uehTStatusProhibit[];
extern U8 uehSnFieldLen[];
extern U8  uehTReordring[];
extern U8  uehGetMaxUlHarqTx[];
/* ======================================= */
/* RRC Message Buiding Functions */
/* ======================================= */
/*
*
*       Fun:   uehGetPMICfgIdx 
*
*       Desc:  Assign PMI Cfg Index
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_bldmsg.c 
*
*/
PUBLIC S16 uehGetPMICfgIdx
(
TknU32 *tkn
)
{
   U8 pmiCfgIdx = 0;

   logFunc("%s", "uehGetPMICfgIdx");

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
         UEH_ERROR_ELMNT_OUT_OF_RANGE("pmiCfgIdx", pmiCfgIdx, 0, 3);
      	 pmiCfgIdx = 4;
      	 tkn->val = 0;
         break;
   }
   pmiCfgIdx++;

   RETVALUE(ROK);
}/* uehGetPMICfgIdx */ 

/*
*
*       Fun:   uehGetSRCfgIdx 
*
*       Desc:  Assign SR Cfg Index
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_bldmsg.c 
*
*/
PUBLIC S16 uehGetSRCfgIdx
(
TknU8 *tkn
)
{
   PRIVATE U8 srCfgIdx = 0;

   logFunc("%s", "uehGetSRCfgIdx");

   tkn->pres = PRSNT_NODEF;

   switch(srCfgIdx)
   {
      case 0:
         tkn->val = 42;
         break;
      case 1:
         tkn->val = 52;
         break;
      case 2:
         tkn->val = 62;
         break;
      case 3:
         tkn->val = 72;
         break;
      default:
         tkn->val = 155; /* assigning reserved value for now - enhance later */
         UEH_ERROR_ELMNT_OUT_OF_RANGE("srCfgIdx", srCfgIdx, 0, 3);
         srCfgIdx = 4;
         break;
   }
   srCfgIdx++;

   RETVALUE(ROK);
}/* uehGetSRCfgIdx */ 


/*
*
*       Fun:   uehFillRRC_ConSetup
*
*       Desc:  This function fills RRC Connection Setup Msg
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillRRC_ConSetup
(
NhuDatRspSdus *nhuDatRspSdu,
UehUeCb *ueCb
)
{
  NhuDL_CCCH_Msg                  *dlMsg     = NULLP;
  NhuRRCConSetup                  *conSetUp  = NULLP;
  NhuRadioResourceConfigDedicated *resCfgDed = NULLP;

  logFunc("%s", "uehFillRRC_ConSetup");

  nhuDatRspSdu->sdu.msgCategory = NHU_MSG_DLCCCH;

  dlMsg = &(nhuDatRspSdu->sdu.m.dlCcchMsg.ccchMsg);

  UEH_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(dlMsg->message.choice, DL_CCCH_MSGTYP_C3);
  UEH_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C3_RRCCONSETUP);

  /* RRCConnectionSetup */
  conSetUp = &(dlMsg->message.val.c1.val.rrcConSetup);
  UEH_FILL_TKN_UINT(conSetUp->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(conSetUp->rrc_TrnsxnId, UEH_VAL_ONE);
  UEH_FILL_TKN_UINT(conSetUp->criticalExtns.choice, CRITICALEXTNS_C14);
  /* RRCConnectionSetup-r8-IEs  */
  UEH_FILL_TKN_UINT(conSetUp->criticalExtns.val.c1.choice, C16_RRCCONSETUP_R8);
  UEH_FILL_TKN_UINT(conSetUp->criticalExtns.val.c1.val.rrcConSetup_r8.pres, PRSNT_NODEF);

  /* RadioResourceConfigDedicated */
  resCfgDed = &(conSetUp->criticalExtns.val.c1.val.rrcConSetup_r8.radioResourceConfigDedicated);

  UEH_FILL_TKN_UINT(resCfgDed->pres, PRSNT_NODEF);
  /* SRB Add for now only SRB1 */
  uehFillNhuSRBAddModLst((Ptr)nhuDatRspSdu, resCfgDed, UEH_PDCP_ID_SRB1);

  /* RRC dedicated MAC Cfg */
  /* MAC-MainConfig  */
   UEH_FILL_TKN_UINT(resCfgDed->mac_MainConfig.choice, MAC_MAINCONFIG_DEFAULTVALUE);
  //UEH_FILL_TKN_UINT(resCfgDed->mac_MainConfig.choice, MAC_MAINCONFIG_EXPLICITVALUE);
  if (resCfgDed->mac_MainConfig.choice.val == MAC_MAINCONFIG_EXPLICITVALUE)
  {
    uehFillNhuMACMainCfg((Ptr)nhuDatRspSdu, resCfgDed, ueCb);
  } 

  /* Phy. Dedicated Cfg */
  uehFillRadResPhyCfgDed((Ptr)nhuDatRspSdu, resCfgDed, ueCb);

  /* RRCConnectionSetup-r8-IEs - nonCriticalExtension */
  UEH_FILL_TKN_UINT(conSetUp->criticalExtns.val.c1.val.rrcConSetup_r8.nonCriticalExtn.pres, NOTPRSNT);

  RETVALUE(ROK);
} /* End of uehFillRRC_ConSetup */

PUBLIC S16 uehFillRRC_ConReEstblReject 
(
NhuDatRspSdus *nhuDatRspSdu
)
{
NhuDL_CCCH_Msg       *dlMsg = NULLP;
NhuRRCConReestbRej   *conReEstblReject = NULLP;
  
  nhuDatRspSdu->sdu.msgCategory = NHU_MSG_DLCCCH;

  dlMsg = &(nhuDatRspSdu->sdu.m.dlCcchMsg.ccchMsg);

  UEH_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(dlMsg->message.choice, DL_CCCH_MSGTYP_C3);
  UEH_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C3_RRCCONREESTBREJ);

  /* RRCConnectionReject */
  conReEstblReject = &(dlMsg->message.val.c1.val.rrcConReestbRej);
  UEH_FILL_TKN_UINT(conReEstblReject->pres, PRSNT_NODEF);
  /* RRCConnectionSetup-r8-IEs  */
  UEH_FILL_TKN_UINT(conReEstblReject->criticalExtns.choice, C13_RRCCONREESTB_R8); /*Verify this*/

  UEH_FILL_TKN_UINT(conReEstblReject->criticalExtns.val.rrcConReestbRej_r8.pres, PRSNT_NODEF);

  /* RRCConnectionReject-r8-IEs - nonCriticalExtension */
  UEH_FILL_TKN_UINT(conReEstblReject->criticalExtns.val.rrcConReestbRej_r8.nonCriticalExtn.pres, PRSNT_NODEF);

  RETVALUE(ROK);
}
/*
*
*       Fun:   uehFillRRC_ConReject
*
*       Desc:  This function fills RRC Connection Reject Msg
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
void uehFillRRC_ConReject
(
NhuDatRspSdus *nhuDatRspSdu
)
{
  NhuDL_CCCH_Msg *dlMsg = NULLP;
  NhuRRCConRej     *conReject = NULLP;

  logFunc("%s","uehFillRRC_ConReject");

  nhuDatRspSdu->sdu.msgCategory = NHU_MSG_DLCCCH;

  dlMsg = &(nhuDatRspSdu->sdu.m.dlCcchMsg.ccchMsg);

  UEH_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(dlMsg->message.choice, DL_CCCH_MSGTYP_C3);
  UEH_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C3_RRCCONREJ);

  /* RRCConnectionReject */
  conReject = &(dlMsg->message.val.c1.val.rrcConRej);
  UEH_FILL_TKN_UINT(conReject->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(conReject->criticalExtns.choice, CRITICALEXTNS_C14);
  /* RRCConnectionSetup-r8-IEs  */
  UEH_FILL_TKN_UINT(conReject->criticalExtns.val.c1.choice, C14_RRCCONREJ_R8); /*Verify this*/

  UEH_FILL_TKN_UINT(conReject->criticalExtns.val.c1.val.rrcConRej_r8.pres, PRSNT_NODEF);

  /*TODO: wait time should read from the OpConfig->enodeBconfig table */ 
  UEH_FILL_TKN_UINT(conReject->criticalExtns.val.c1.val.rrcConRej_r8.waitTime, uehCb.uehCfgCb.cellCfg.rrcConnRejWaitTime);
  /* RRCConnectionReject-r8-IEs - nonCriticalExtension */
  UEH_FILL_TKN_UINT(conReject->criticalExtns.val.c1.val.rrcConRej_r8.nonCriticalExtn.pres, PRSNT_NODEF);

} /* End of uehFillRRC_ConReject */

/*
*
*       Fun:   uehFillRRC_DlInfTr
*
*       Desc:  This function fills Downlink Information Transfer
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillRRC_DlInfTr
(
NhuDatReqSdus *nhuDatReqSdu,
U16 pduLen,
U8 *nasPdu,
U8 pdcpId
)
{
   NhuDL_DCCH_Msg     *dlMsg = NULLP;
   NhuDLInfoTfr       *infoTfrMsg = NULLP;
   NhuDLInfoTfr_r8_IEs *msgIe = NULLP;
   U8 rrc_transId = UEH_VAL_ONE;
   U16 idx = 0;

   logFunc("%s", "uehFillRRC_DlInfTr");

   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = pdcpId;
   dlMsg = &(nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   UEH_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   UEH_FILL_TKN_UINT(dlMsg->message.choice, DL_DCCH_MSGTYP_C4);
   UEH_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C4_DLINFOTFR);

   /* DLInformationTransfer */
   infoTfrMsg = &(dlMsg->message.val.c1.val.dlInfoTfr);
   UEH_FILL_TKN_UINT(infoTfrMsg->pres, PRSNT_NODEF);
   UEH_FILL_TKN_UINT(infoTfrMsg->rrc_TrnsxnId, rrc_transId);
   UEH_FILL_TKN_UINT(infoTfrMsg->criticalExtns.choice, CRITICALEXTNS_C8);
   UEH_FILL_TKN_UINT(infoTfrMsg->criticalExtns.val.c1.choice, C8_DLINFOTFR_R8);

   /* DLInformationTransfer-r8-IEs */
   msgIe = &(infoTfrMsg->criticalExtns.val.c1.val.dlInfoTfr_r8);
   UEH_FILL_TKN_UINT(msgIe->pres, PRSNT_NODEF);
   UEH_FILL_TKN_UINT(msgIe->dedicatedInfoTyp.choice, DEDICATEDINFOTYP_DEDICATEDINFONAS);

   /* NAS PDU */
   UEH_GET_MEM(nhuDatReqSdu, pduLen, &(msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.val));
   msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.pres = PRSNT_NODEF;
   msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.len = pduLen;
   /* copy each byte */
   for(idx = 0; idx < pduLen; idx++)
   {
      msgIe->dedicatedInfoTyp.val.dedicatedInfoNAS.val[idx] = nasPdu[idx];
   }

   /* criticalExtensionsFuture */
   UEH_FILL_TKN_UINT(msgIe->nonCriticalExtn.pres, NOTPRSNT);

   RETVALUE(ROK);
} /* End of uehFillRRC_DlInfTr */


/*
*
*       Fun:   uehFillRRC_SecModeCmd
*
*       Desc:  This function fills Downlink Security Mode Command
*
*       Ret:   ROK
*
*       Notes: This message contains only the algorithms used by eNodeB
*
*       File:  ueh_bldmsg.c
*
*/
void uehFillRRC_SecModeCmd(
    NhuDatReqSdus *nhuDatReqSdu,
    UehUeCb       *ueCb
  )
{
   NhuDL_DCCH_Msg     *dlMsg = NULLP;
   NhuSecModeCmd      *secModeCmd = NULLP;
   NhuSecConfigSMC    *securityCfg = NULLP;

   logFunc("%s", "uehFillRRC_SecModeCmd");

   nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = UEH_PDCP_ID_SRB1;
   nhuDatReqSdu->sdu.ueMsg.isMsgDed = TRUE;

   dlMsg = &(nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

   UEH_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
   UEH_FILL_TKN_UINT(dlMsg->message.choice, DL_DCCH_MSGTYP_C4);
   UEH_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C4_SECURITYMODECMD);

   /* SecurityModeCommand  */
   secModeCmd = &(dlMsg->message.val.c1.val.securityModeCmd);

   UEH_FILL_TKN_UINT(secModeCmd->pres, PRSNT_NODEF);
   UEH_FILL_TKN_UINT(secModeCmd->rrc_TrnsxnId, UEH_VAL_ONE);
   UEH_FILL_TKN_UINT(secModeCmd->criticalExtns.choice, CRITICALEXTNS_C18);
   UEH_FILL_TKN_UINT(secModeCmd->criticalExtns.val.c1.choice, C18_SECURITYMODECMD_R8);
   UEH_FILL_TKN_UINT(secModeCmd->criticalExtns.val.c1.val.securityModeCmd_r8.\
         pres, PRSNT_NODEF);

   /* SecurityConfigSMC */
   securityCfg = &(secModeCmd->criticalExtns.val.c1.val.securityModeCmd_r8.\
         securityConfigSMC);

   UEH_FILL_TKN_UINT(securityCfg->pres, PRSNT_NODEF);
   UEH_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.pres, PRSNT_NODEF);   
   UEH_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.cipheringAlgorithm, ueCb->secCfg.ciphAlgoSelected);
   UEH_FILL_TKN_UINT(securityCfg->securityAlgorithmConfig.integrityProtAlgorithm, ueCb->secCfg.intgAlgoSelected);

} /* End of uehFillRRC_SecModeCmd */


/*
 *
 *       Fun:   uehFillRRC_ConRls
 *
 *       Desc:  This function fills RRC Connection Release Message
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *
 *       File:  ueh_bldmsg.c
 *
 */
void uehFillRRC_ConRls
(
NhuDatReqSdus *nhuDatReqSdu,
UehUeCb        *ueCb
)
{
  NhuDL_DCCH_Msg     *dlMsg = NULLP;
  NhuRRCConRls       *conRls = NULLP;
#ifdef FT_CELL_RESELECTION
  NhuIdleModeMobilityCntrlInfo *idleModeMobility;
#endif

  logFunc("%s", "uehFillRRC_ConRls");

  nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = UEH_PDCP_ID_SRB1;
  dlMsg = &(nhuDatReqSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
  nhuDatReqSdu->sdu.ueMsg.isMsgDed = TRUE;

  UEH_FILL_TKN_UINT(dlMsg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(dlMsg->message.choice, DL_DCCH_MSGTYP_C4);
  UEH_FILL_TKN_UINT(dlMsg->message.val.c1.choice, C4_RRCCONRLS);

  /* RRCConnectionRelease */
  conRls = &(dlMsg->message.val.c1.val.rrcConRls);
  UEH_FILL_TKN_UINT(conRls->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(conRls->rrc_TrnsxnId, UEH_VAL_ONE);
  UEH_FILL_TKN_UINT(conRls->criticalExtns.choice, CRITICALEXTNS_C15);
  UEH_FILL_TKN_UINT(conRls->criticalExtns.val.c1.choice, C15_RRCCONRLS_R8);
  /* RRCConnectionRelease-r8-IEs */
  UEH_FILL_TKN_UINT(conRls->criticalExtns.val.c1.val.rrcConRls_r8.pres, PRSNT_NODEF);

  /* Assigning the UE release cause from UeCb */
  UEH_FILL_TKN_UINT(conRls->criticalExtns.val.c1.val.rrcConRls_r8.releaseCause, ueCb->ueReleaseCauseVal);
  
#ifdef FT_CELL_RESELECTION
  /* IdleModeMobilityCntrlInfo*/
  idleModeMobility = &(conRls->criticalExtns.val.c1.val.rrcConRls_r8.idleModeMobilityCntrlInfo);
  UEH_FILL_TKN_UINT(idleModeMobility->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(idleModeMobility->t320, NhuIdleModeMobilityCntrlInfot320min180Enum);
#endif
} /* End of uehFillRRC_ConRls */

/*
 *
 *       Fun:     uehInitPendingPagingList
 *
 *       Desc:    This function is used to initialize linked list used to store pending pagine messages.
 *
 *
 *       File:    ueh_mi.c
 *
 */

PUBLIC S16 uehInitPendingPagingList( void )
{
  UehCellCb *cellCb = uehGetCellCb();
  U16 noOfSfn = 0;
  U8  is      = 0;


  for( noOfSfn = 0; noOfSfn < UEH_MAX_SFN_PAGING ; noOfSfn++ )
  {
    for( is = 0; is < UEH_MAX_I_S ; is++ )
    {
      cellCb->uehPagingRecordInfo.noOfRecInGroup[noOfSfn][is] = 0;
    }
  }
  for( noOfSfn = 0; noOfSfn < UEH_MAX_SFN_PAGING ; noOfSfn++ )
  {
    cmLListInit(&(cellCb->uehPagingRecordInfo.uehPagingRecordInfo[noOfSfn]));
  }
  RETVALUE(ROK);
}


/*
 *
 *       Fun:     uehFreePendingPagingList
 *
 *       Desc:    This function is used to Frees linked list used to store pending pagine messages.
 *
 *
 *       File:    ueh_mi.c
 *
 */

PUBLIC S16 uehFreePendingPagingList( void )
{
  U16 sfnNum = 0;
  U8  is      = 0;

  CmLList   *pagingRecord = NULL;
  UehCellCb *uehCellCb = uehGetCellCb();

  for( sfnNum = 0; sfnNum < UEH_MAX_SFN_PAGING ; sfnNum++ )
  {
    while( uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[sfnNum].count )
    {
      logInfo("uehFillRRCPagingMsg: no of pending Paging messages before clearing on OP-STOP = [%d]", (int)uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[sfnNum].count);

      pagingRecord = NULL;
      /* Get First Node from List */
      CM_LLIST_FIRST_NODE(&(uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[sfnNum]), pagingRecord);
      if(NULLP EQ pagingRecord)
      {
        UEH_ERROR_NULL_POINTER_IDENTIFIED();
        uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[sfnNum].count  = 0 ;
        continue;
      }
      /* Free memory and also need to remove from the linked list*/
      cmLListDelFrm(&(uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[sfnNum]), pagingRecord);
      UEH_FREE(pagingRecord->node, sizeof(UehPagingRecord));
    }
    /* Clear the count */
    for( is = 0; is < UEH_MAX_I_S ; is++ )
    {
      uehCellCb->uehPagingRecordInfo.noOfRecInGroup[sfnNum][is] = 0;
    }
  }

  RETVALUE(ROK);
}

/*
*
*       Fun:   uehAddPagingRecord
*
*       Desc:  Adds Paging message to linked list
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehAddPagingRecord
(
  S1PagingInfo     *pagingInfo,
  U16               pagSchSfn,
  U16               tValue,
  U8                i_s
)
{
  Bool               isPagRecordUpdated = FALSE;
  U8                 count = 0;
  U16                tempPagSchSfn = 0;
  UehPagingRecord    *pagRcrdNode = NULLP;


  UehCellCb *uehCellCb = uehGetCellCb();
   /* Allocate memory for UE Paging Info */
  UEH_ALLOC(&pagRcrdNode, sizeof(UehPagingRecord));
  if(pagRcrdNode == NULLP)
  {
    UEH_ERROR_MEM_ALLOC_FAILED(sizeof(UehPagingRecord), "UehPagingRecord");
    RETVALUE(RFAILED);
  } /* end of if statement */


  pagRcrdNode->choice    = pagingInfo->choice;
  pagRcrdNode->cnDomain  = pagingInfo->cnDomain;
  pagRcrdNode->i_s       = i_s;

  switch(pagingInfo->choice)
  {
    case UEPAGID_S_TMSI: /* s-TMSI based */
    {
      pagRcrdNode->val.sTmsi.mmec     =  pagingInfo->val.sTmsi.mmec;
      pagRcrdNode->val.sTmsi.sTMSIVal =  pagingInfo->val.sTmsi.sTMSIVal;
      break;
    }
    case UEPAGID_IMSI: /* IMSI based */
    {  /* Fill the Digits of the IMSI */
      pagRcrdNode->val.imsi.noOfImsiDigits = pagingInfo->val.imsi.noOfImsiDigits;
      for( count = 0;  count < pagingInfo->val.imsi.noOfImsiDigits; count++)
      {
        pagRcrdNode->val.imsi.imsiVal[count] = pagingInfo->val.imsi.imsiVal[count];
      }
      break;
    }
  } /* end of switch statement */

  /* Add the Paging info into the linked list of cellCb */
  /* 1.If No of paging records that are already present < 15
   * 	Then add paging record in current SFN
   * 2.Else
   * 	Find new SFN
   * 	Follow from step1 again till you reached original SFN
   *    (Means all paging records are full and waiting for scheduling)
   * 	Then add this record at the end of the list.
   * */
  tempPagSchSfn = pagSchSfn;
  while(!isPagRecordUpdated)
  {
    if((uehCellCb->uehPagingRecordInfo.noOfRecInGroup[pagSchSfn][i_s]) < MAX_PAGE_REC)
    {
      isPagRecordUpdated = TRUE;
      uehCellCb->uehPagingRecordInfo.noOfRecInGroup[pagSchSfn][i_s]++;
    }
    else
    {
      pagSchSfn = (pagSchSfn + tValue) % UEH_MAX_SFN_PAGING;
      /* In case of all paging records are full and waiting for scheduling
       * Then add paging record in original paging queue */
      if(tempPagSchSfn EQ pagSchSfn)
      {
        isPagRecordUpdated = TRUE;
        uehCellCb->uehPagingRecordInfo.noOfRecInGroup[pagSchSfn][i_s]++;
      }
    }
  }

  pagRcrdNode->lnk.node = (PTR)pagRcrdNode;
  cmLListAdd2Tail(&(uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[pagSchSfn]), &(pagRcrdNode->lnk));

  RETVALUE(ROK);
}
/*
*
*       Fun:   uehFillAndAddPagingRecord
*
*       Desc:  Fills Paging message
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillAndAddPagingRecord
(
  S1PagingInfo *pagingInfo
)
{
  U8                 plmnMncSize = 0;
  U8                 count = 0;
  U8                 i_s = 0;
  Bool               isTaiMatched = FALSE;
  Bool               isMccMatched = FALSE;
  Bool               isMncMatched = FALSE;
  Bool               isPagingRecAdded = FALSE;
  U16                T = 0, N = 0, Ns = 0, Nb = 0;
  U16                pf = 0;
  U16                pagSchSfn = 0;
  U16                curSfn;
  U32                ueSpecificValue = 0;
  U32                defaultValue = 0;

  logFunc("%s", "uehFillAndAddPagingRecord");

  /* Compare TAC */
  for(count = 0 ; count < pagingInfo->noOfTaiPresent ; count ++)
  {
    if(pagingInfo->taiList[count].tac EQ uehCb.uehCfgCb.cellCfg.tac )
    {
      isTaiMatched = TRUE;
      /* PLMN Change*/
      plmnMncSize = uehCb.uehPlmnId.numOfMNCDigits;
      if(pagingInfo->taiList[count].plmnId.fillerBitInMnc EQ TRUE)
      {
        plmnMncSize = 2;
      }
      /* Compare MNC */
      /* PLMN Change*/
      if(0 EQ memcmp(pagingInfo->taiList[count].plmnId.mnc , uehCb.uehPlmnId.plmnMNC , plmnMncSize))
      {
    	isMncMatched = TRUE;
    	/* Compare MCC */
        if(0 EQ memcmp(pagingInfo->taiList[count].plmnId.mcc , uehCb.uehPlmnId.plmnMCC  , MAX_SIZE_CELLCONFIG_PLMNMCC))
        {
          isMccMatched = TRUE;
          break;
        }
        else
        {
          UEH_ERROR_ELMNT_OUT_OF_RANGE("MCC", pagingInfo->taiList[count].plmnId.mcc, NOT_APPLICABLE, uehCb.uehPlmnId.plmnMCC);
        }
      }
      else
      {
        UEH_ERROR_ELMNT_OUT_OF_RANGE("MNC", pagingInfo->taiList[count].plmnId.mnc, NOT_APPLICABLE, uehCb.uehPlmnId.plmnMNC);
      }
    }
    else
    {
      UEH_ERROR_ELMNT_OUT_OF_RANGE("TAC", pagingInfo->taiList[count].tac, NOT_APPLICABLE, uehCb.uehCfgCb.cellCfg.tac );
    }
  }

  if(isMccMatched EQ FALSE)
  {
    RETVALUE(ROK);
  }


  /* Fill the Value of T (DRX cycle of the UE)
   * T = min(UE Specific DRX value allocated by upper layers, default DRX
   * broadcast in System Information) i.e. min( ueSpcPagDrx, veDfltDefaultPagCycle)
   */
  defaultValue = pagingCycleVal[uehCb.uehCfgCb.sib2Cfg.defaultPagCycle];
  if(pagingInfo->ueSpecDrx.pagingDrxPres == FALSE)
  {
    T =  defaultValue;
  }
  else
  {
    ueSpecificValue = pagingCycleVal[pagingInfo->ueSpecDrx.ueSpcPagDrx];
    T = (ueSpecificValue < defaultValue) ? ueSpecificValue : defaultValue;
  }
  /* nB values as per 36.304 are 4T,2T,T,T/2,T/4,T/16,T/32
   * In Implementation we taken as array  UINT8 nBEnumMappingVal[7] = { 4, 2, 1, 2, 4, 8, 16 };
   * In the below comparison 3 means 3 rd array index
   * */
  if(3 > uehCb.uehCfgCb.sib2Cfg.nB)
  {
    Nb = T * nBEnumMappingVal[uehCb.uehCfgCb.sib2Cfg.nB];
    N  = T;
  }
  else
  {
    Nb = T / nBEnumMappingVal[uehCb.uehCfgCb.sib2Cfg.nB];
    N  = Nb;
  }

  Ns = uehGetCellCb()->ns;

  /* Calculate the Value of pf
   * SFN Mod T = (T div N) * (UE_ID mod N)
   * */
  pf = ((T / N) * ((pagingInfo->ueId) % N));

  /* Calculate the Value of po */
  i_s = ((((pagingInfo->ueId) / N)) % Ns);

  /* SFN % T = PF*/
  curSfn = (uehGetCellCb()->sfn + UEH_SFN_INCREMENT_TO_STORE_PAGMSG) % UEH_MAX_SFN;
  /* UEH_MAX_SFN_PAGING is used to reduce memory usage */
  pagSchSfn = (T * (curSfn / T) + ((curSfn % T) > pf ? (T) : (0)) + pf) % UEH_MAX_SFN_PAGING;

  logInfo("uehFillAndAddPagingRecord: PF: [%u], T: [%u],  curSfn: [%u] pagSchSfn: [%u] \n", pf, T,  uehGetCellCb()->sfn, pagSchSfn);
  isPagingRecAdded = uehAddPagingRecord(pagingInfo, pagSchSfn, T , i_s);

  RETVALUE(isPagingRecAdded);

} /* end of uehFillAndAddPagingRecord */

/*
*
*       Fun:   uehFillPageRecordInPageOccupation
*
*       Desc:  Fills RRC  Paging message into paging occupation data structure.. This can be used to store in linked list
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillPageRecordInPageOccupation
(
  NhuDatReqSdus    *datReqSdus,
  NhuPag           *page,
  UehPagingRecord  *pendPagUesLstPtr,
  UINT16            pagingRecIndex
  )
{
  U8   noOfImsi = 0;
  U16  idx1 = 0;

  /* The number of digits in IMSI shall be twice the number of
  * bytes in the octet string */
  noOfImsi = pendPagUesLstPtr->val.imsi.noOfImsiDigits;

  UEH_FILL_TKN_UINT(page->pagingRecordLst.member[pagingRecIndex].pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(page->pagingRecordLst.member[pagingRecIndex].cn_Domain,
                 pendPagUesLstPtr->cnDomain);
  UEH_FILL_TKN_UINT(page->pagingRecordLst.member[pagingRecIndex].ue_Identity.choice,
                 pendPagUesLstPtr->choice);
  switch(pendPagUesLstPtr->choice)
  {
    case UEID_STMSI: /* s-TMSI based */
    {
      UEH_FILL_TKN_UINT(page->pagingRecordLst.member[pagingRecIndex].ue_Identity.val.s_TMSI.pres, PRSNT_NODEF);
      page->pagingRecordLst.member[pagingRecIndex].ue_Identity.val.s_TMSI.mmec.len = 1;
      UEH_FILL_BIT_STR(page->pagingRecordLst.member[pagingRecIndex].ue_Identity.val.s_TMSI.mmec, 8, (pendPagUesLstPtr->val.sTmsi.mmec), datReqSdus);
      page->pagingRecordLst.member[pagingRecIndex].ue_Identity.val.s_TMSI.m_TMSI.len = 4;
      UEH_FILL_BIT_STR_VAL(page->pagingRecordLst.member[pagingRecIndex].ue_Identity.val.s_TMSI.m_TMSI, 32, (pendPagUesLstPtr->val.sTmsi.sTMSIVal), datReqSdus);
      break;
    } /* end of case 0 */
    case UEID_IMSI: /* IMSI based */
    {
      UEH_FILL_TKN_UINT(page->pagingRecordLst.member[pagingRecIndex].ue_Identity.val.imsi.noComp, noOfImsi);
      UEH_GET_MEM(datReqSdus, (page->pagingRecordLst.member[pagingRecIndex].ue_Identity.val.imsi.noComp.val) * (sizeof(NhuIMSI_Digit)), &(page->pagingRecordLst.member[pagingRecIndex].ue_Identity.val.imsi.member));
      /* Fill the Digits of the IMSI */
      for(idx1 = 0;  idx1 < noOfImsi; idx1++)
      {
        UEH_FILL_TKN_UINT(page->pagingRecordLst.member[pagingRecIndex].ue_Identity.val.imsi.member[idx1],pendPagUesLstPtr->val.imsi.imsiVal[idx1]);
      } /* end of for loop */
      break;
    } /* end of case UEID_IMSI */
  } /* end of switch statement */
  RETVALUE(ROK);
}


/*
*
*       Fun:   uehFillRRCPagingMsg
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
*       File:  ueh_bldmsg.c
*
*/

PUBLIC S16 uehFillRRCPagingMsg
(
  NhuDatReqSdus    *datReqSdus[UEH_MAX_I_S],
  U16              pagSfn
)
{
  U8               tempIs = 0;
  U8               noOfPagMsgAddedToRrcMsg[UEH_MAX_I_S] ;
  U32              idx = 0 , count = 0;
  U32              noOfPendingPagMess = 0;
  Mem              sMem;
  UehPagingRecord  *pendPagUesLstPtr = NULLP;
  CmLList          *tmpUePagingInfo = NULLP;
  CmLList          *pagingRecord = NULLP;
  CmLList          *listCurNode = NULLP;
  NhuPCCH_Msg      *pcchMsg;
  NhuPag           *page;

  sMem.region = 0;
  sMem.pool = 0;

  UehCellCb *uehCellCb = uehGetCellCb();

  for(tempIs = 0; tempIs < UEH_MAX_I_S; tempIs ++)
  {
    noOfPagMsgAddedToRrcMsg[tempIs] = 0;
  }
  /* Allocate memory for paging records for sending to RRC */
  for(tempIs = 0; tempIs < UEH_MAX_I_S; tempIs ++)
  {
    if((uehCellCb->uehPagingRecordInfo.noOfRecInGroup[pagSfn][tempIs]) > 0)
    {
      noOfPendingPagMess = uehCellCb->uehPagingRecordInfo.noOfRecInGroup[pagSfn][tempIs];
      logInfo("uehFillRRCPagingMsg: noOfPendingPagMess: [%u] ", (unsigned int)noOfPendingPagMess );
      /* Get pcch message based on Paging Occupation */
      pcchMsg = &(datReqSdus[tempIs]->sdu.ueMsg.msg.cmnMsg.cmn.pcchMsg);
      UEH_FILL_TKN_UINT(pcchMsg->pres, PRSNT_NODEF);
      UEH_FILL_TKN_UINT(pcchMsg->message.choice,0);
      if(!(pcchMsg->message.choice.val))
      {
       UEH_FILL_TKN_UINT(pcchMsg->message.val.c1.choice,0);
       page = &(pcchMsg->message.val.c1.val.paging);

       UEH_FILL_TKN_UINT(page->pres, PRSNT_NODEF);
       /*BS-1799 cell barring */
       if (uehCellCb->isSysInfoNeedToSend == TRUE)
       {
         UEH_FILL_TKN_UINT(page->systemInfoModfn, TRUE); /* Indicates system information has changed and need to send in next modification period */
       //  logError ("BS-1799 uehFillRRCPagingMsg page->systemInfoModfn ", page->systemInfoModfn);
       }
       else
       {
         UEH_FILL_TKN_UINT(page->systemInfoModfn, FALSE); 
         //logError ("BS-1799 uehFillRRCPagingMsg page->systemInfoModfn %d", page->systemInfoModfn);
       }
       
       UEH_FILL_TKN_UINT(page->pagingRecordLst.noComp, noOfPendingPagMess);

#ifndef NHU_VER2
       datReqSdus[tempIs]->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.tbsSize = 7 * noOfPendingPagMess;
#endif
       UEH_GET_MEM(datReqSdus[tempIs], (page->pagingRecordLst.noComp.val)*(sizeof(NhuPagRecord)), &(page->pagingRecordLst.member));
      }
    }
  }

  pagingRecord = NULL;
  /* Get First Node from List */
  CM_LLIST_FIRST_NODE(&(uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[pagSfn]), pagingRecord);

  logInfo("uehFillRRCPagingMsg: uehPagingRecordInfo : [%d]",uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[pagSfn].count);
  count  = uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[pagSfn].count;
  /* Process all paging records in the list */

  for(idx = 0;  idx < count; idx++)
  {
    if(NULLP EQ pagingRecord)
    {
      RETVALUE(ROK);
    }
    pendPagUesLstPtr = (UehPagingRecord*)pagingRecord->node;

    /* Maximum number of paging records in one paging occasion are 16 */
    if(((NULLP != pendPagUesLstPtr)) && (noOfPagMsgAddedToRrcMsg[pendPagUesLstPtr->i_s] < MAX_PAGE_REC))
    {
      page = &(datReqSdus[pendPagUesLstPtr->i_s]->sdu.ueMsg.msg.cmnMsg.cmn.pcchMsg.message.val.c1.val.paging);
      uehFillPageRecordInPageOccupation(datReqSdus[pendPagUesLstPtr->i_s] ,page, pendPagUesLstPtr, noOfPagMsgAddedToRrcMsg[pendPagUesLstPtr->i_s]);
      noOfPagMsgAddedToRrcMsg[pendPagUesLstPtr->i_s]++;

      tmpUePagingInfo = pagingRecord;
      listCurNode = pagingRecord->next;
      /* Get Next Node from List */
      CM_LLIST_NEXT_NODE(&(uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[pagSfn]), pagingRecord);

      /* Free memory and also need to remove from the linked list*/
      UEH_FREE(tmpUePagingInfo->node, sizeof(UehPagingRecord));
      cmLListDelFrm(&(uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[pagSfn]), tmpUePagingInfo);
      uehCellCb->uehPagingRecordInfo.noOfRecInGroup[pagSfn][pendPagUesLstPtr->i_s]--;
      // Current should be updated
      uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[pagSfn].crnt = listCurNode;
    }
    else
    {
      CM_LLIST_NEXT_NODE(&(uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[pagSfn]), pagingRecord);
    }
    logInfo("uehFillRRCPagingMsg: idx : [%u]\n", idx );
  }

  if(uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[pagSfn].count > 0)
  {
    logInfo("uehFillRRCPagingMsg: Actual paging records: [%u] Remaining paging records: [%u]",
          count, uehCellCb->uehPagingRecordInfo.uehPagingRecordInfo[pagSfn].count);
  }

  RETVALUE(ROK);
} /* uehFillRRCPagingMsg */

/*
*   Fun:  uehFillUlPwrReconf        
*          
*   Desc: Fills UplinkPowerControlDedicated fields to reset UE ul pucch power
*         
*  Ret:   ROK
          
*  Notes: None
*         
*   File: ueh_bldmsg.c
*/

PUBLIC S16 uehFillAndSndUlPwrReconf(
   UehUeCb                    *ueCb
  )
{
  NhuDatReqSdus                    *nhDatReqEvntSdu = NULLP;
  NhuDL_DCCH_Msg                   *dcchMsg         = NULLP;
  NhuRRCConRecfgn                  *rrcConReCfg     = NULLP;
  NhuRRCConRecfgn_r8_IEs           *conRecfgIEs     = NULLP;
  NhuRadioResourceConfigDedicated  *radioResCfg     = NULLP;
  NhuUlnkPowerCntrlDedicated       *uplnkPwrCntrl   = NULLP;

#ifdef RRM_PRL_FLAG 
S16 ret = RFAILED;
Buff *buff = NULL;
#endif
  
  /* Allocate memory for event structure,
   * Fill HDR Info,
   * Build RRCReconfiguration Message */
  UEH_ALLOCEVNT_OR_RET(&nhDatReqEvntSdu, sizeof(NhuDatReqSdus));

  uehFillNhuEvntHdr( &(nhDatReqEvntSdu->hdr), ueCb->cellId, ueCb->crnti, C4_RRCCONRECFGN );
  
  nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = UEH_PDCP_ID_SRB1;
  dcchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
  nhDatReqEvntSdu->sdu.ueMsg.isMsgDed = TRUE;
  UEH_FILL_TKN_UINT(dcchMsg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(dcchMsg->message.choice, DL_DCCH_MSGTYP_C4);
  UEH_FILL_TKN_UINT(dcchMsg->message.val.c1.choice, C4_RRCCONRECFGN);
  rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
  UEH_FILL_TKN_UINT(rrcConReCfg->pres, PRSNT_NODEF);

  UEH_FILL_TKN_UINT(rrcConReCfg->rrc_TrnsxnId, UEH_VAL_THREE);
  UEH_FILL_TKN_UINT(rrcConReCfg->criticalExtns.choice, CRITICALEXTNS_C12);
  UEH_FILL_TKN_UINT(rrcConReCfg->criticalExtns.val.c1.choice, C12_RRCCONRECFGN_R8);

    conRecfgIEs = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
    UEH_FILL_TKN_UINT(conRecfgIEs->pres, PRSNT_NODEF);
    radioResCfg = &conRecfgIEs->radioResourceConfigDedicated;
    UEH_FILL_TKN_UINT(radioResCfg->pres,PRSNT_NODEF);
    UEH_FILL_TKN_UINT(radioResCfg->physicalConfigDedicated.pres, PRSNT_NODEF);
    
    uplnkPwrCntrl = &(radioResCfg->physicalConfigDedicated.uplinkPowerCntrlDedicated);

    
  UEH_FILL_TKN_UINT(uplnkPwrCntrl->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUSCH, uehCb.uehCfgCb.phyCfg.poUePusch);/*uehDfltP0UEPUSCH */
  UEH_FILL_TKN_UINT(uplnkPwrCntrl->deltaMCS_Enabled,smCb.cfgCb.uehConfig.deltaMcsEnabled);//uehDfltDeltaMCSEnabled);
  UEH_FILL_TKN_UINT(uplnkPwrCntrl->accumulationEnabled, smCb.cfgCb.uehConfig.accumulationEnabled);//uehDfltAccumulationEnabled);
  /** toggle the value */
  if (ueCb->poUePucch EQ 0) { ueCb->poUePucch = 1; } else { ueCb->poUePucch = 0; }
  UEH_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUCCH, ueCb->poUePucch); /* uehCb.uehCfgCb.phyCfg.poUePucch */
  UEH_FILL_TKN_UINT(uplnkPwrCntrl->pSRS_Offset, uehCb.uehCfgCb.phyCfg.pSrsOffset);/*uehDfltpSRSOffset*/
  UEH_FILL_TKN_UINT(uplnkPwrCntrl->filterCoefficient, uehCb.uehCfgCb.cellCfg.rsrpFilterCoefficient);/*uehDfltFilterCoef*/
  nhDatReqEvntSdu->sdu.isUeCfgPres = FALSE;
  /* Send RRCReconfigMessage through NhuDatRsp Primitive*/
 
  ueCb->reCfgProcComp = FALSE; /* wait for response. Set to true when response=rrcConnRecongCmpl is recd */
  ret = uehSndMsgOut (ENTNH, EVENTTYPE_RRCCONNRECONFIG, ueCb, TRANSID_NONE, nhDatReqEvntSdu);
#ifdef RRM_PRL_FLAG  
  if((islogApiEnabled() && ret EQ ROK))
   {
     buff = GET_UEHCB_PRNT_LIB_BUFF();
     printNhUiNhuDatReq(nhDatReqEvntSdu, buff);
     logAPI("RRM - RRC :uehFillAndSndUlPwrReconf:nhDatReqEvntSdu STRUCTURE %s", buff->startAddr);
     buff->numBytesWritten = 0;
   }
#endif
   
  RETVALUE(ret);
  return ret;
}

/*     Fun:   uehFillNhuRrcConReCfgn
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
void uehFillNhuRrcConReCfgn(
    UehUeCb        *ueCb,
    NhuDatReqSdus  *nhDatReqEvntSdu
   )
{
  NhuDL_DCCH_Msg                   *dcchMsg     = NULLP;
  NhuRRCConRecfgn                  *rrcConReCfg = NULLP;
  NhuRRCConRecfgn_r8_IEs           *conRecfgIEs = NULLP;

  logFunc("%s", "uehFillNhuRrcConReCfgn");

  nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = UEH_PDCP_ID_SRB1;
  nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.isMsgEncoded = FALSE;
  dcchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
  nhDatReqEvntSdu->sdu.ueMsg.isMsgDed = TRUE;

  UEH_FILL_TKN_UINT(dcchMsg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(dcchMsg->message.choice, DL_DCCH_MSGTYP_C4);
  UEH_FILL_TKN_UINT(dcchMsg->message.val.c1.choice, C4_RRCCONRECFGN);

  rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
  UEH_FILL_TKN_UINT(rrcConReCfg->pres, PRSNT_NODEF);

  UEH_FILL_TKN_UINT(rrcConReCfg->rrc_TrnsxnId, UEH_VAL_ONE);
  UEH_FILL_TKN_UINT(rrcConReCfg->criticalExtns.choice, CRITICALEXTNS_C12);
  UEH_FILL_TKN_UINT(rrcConReCfg->criticalExtns.val.c1.choice, C12_RRCCONRECFGN_R8);

  conRecfgIEs = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
  UEH_FILL_TKN_UINT(conRecfgIEs->pres, PRSNT_NODEF);

  uehFillNhuSRBAddModLst((Ptr)nhDatReqEvntSdu, &(conRecfgIEs->radioResourceConfigDedicated), UEH_PDCP_ID_SRB2);

  /* Save the ERAB Information in UeCB and copy the NAS Info
   * DedicatedInfoNAS List */
  uehFillRRC_ReCfg_ERABSetupLst( nhDatReqEvntSdu, ueCb, conRecfgIEs );

  /* MAC-MainConfig  */
  uehFillNhuMACMainCfg((Ptr)nhDatReqEvntSdu, &conRecfgIEs->radioResourceConfigDedicated, ueCb);

  /*Phy dedicated config*/
  uehFillRadResPhyCfgDed((Ptr)nhDatReqEvntSdu, &conRecfgIEs->radioResourceConfigDedicated, ueCb);

} /* End of uehFillNhuRrcConReCfgn */


/*
*
*       Fun:   uehFillRRC_ReCfg_ERABSetupLst
*
*       Desc:  Fills connection reconfiguration message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillRRC_ReCfg_ERABSetupLst(
    Ptr                      evnt,
    UehUeCb                 *ueCb,
    NhuRRCConRecfgn_r8_IEs  *reCfgIes
)
{

  uehFillNhuDRBAddModLst(evnt, &(reCfgIes->radioResourceConfigDedicated), ueCb); 

  logFunc("%s", "uehFillRRC_ReCfg_ERABSetupLst");

  if (ueCb->rbInfo.noNasPdus > 0)
  {
    /* ADD NAS Info */
    U16 idx = 0;
    NhuRRCConRecfgn_r8_IEsdedicatedInfoNASLst *nasLst = NULLP;
    for(idx = 0; idx < ueCb->rbInfo.noNasPdus; idx++)
    {
      /* Create a RRC NAS IE and append it to this message,
       * We attach all NAS messages one after the other in RRC ReConfig  Message*/
       nasLst = &(reCfgIes->dedicatedInfoNASLst);
       nasLst->noComp.pres = PRSNT_NODEF;
       nasLst->noComp.val++;

       /* Allocating memory for TknStrOSXL */
       UEH_GET_MEM(evnt, (sizeof(NhuDedicatedInfoNAS)), &nasLst->member);

       /*ve002.101 :fix added for attach accept NAS pdu ,removed sizeof operator*/
       /* Allocate memory for the octet string */
       /* fix added for attach accept */
       UEH_GET_MEM( evnt, ueCb->rbInfo.nasPdu[idx].pduLen, &(nasLst->member[idx].val));
       /*ve002.101 :added len and pres value*/
       nasLst->member[idx].pres = PRSNT_NODEF;
       nasLst->member[idx].len = ueCb->rbInfo.nasPdu[idx].pduLen;
       cmMemcpy( (U8 *)nasLst->member[idx].val, (U8 *)ueCb->rbInfo.nasPdu[idx].pdu, nasLst->member[idx].len );
    }
    /* Once all the NAS Messages are sent to UE, Reset the NumberofNas filed */
    ueCb->rbInfo.noNasPdus=0;
  }
  
  RETVALUE(ROK);

} /* uehFillRRC_ReCfg_ERABSetupLst */




/*
*
*       Fun:   uehFillNhuSRBAddModLst
*
*       Desc:  This function fills details of one SRB (SRB1 or SRB2) into radioResCfg.
*              Memory is allocated from evnt.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillNhuSRBAddModLst(
  Ptr                              evnt,
  NhuRadioResourceConfigDedicated *radioResCfg,
  U8                               srbId
 )
{
  NhuSRB_ToAddMod      *srbIe    = NULLP;
  NhuRLC_Configam      *rlcCfgAm = NULLP;
  NhuLogChannelConfig  *logChCfg = NULLP;

  UEH_FILL_TKN_UINT(radioResCfg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(radioResCfg->srb_ToAddModLst.noComp, 1);
  UEH_GET_MEM(evnt, (sizeof(NhuSRB_ToAddMod) * radioResCfg->srb_ToAddModLst.noComp.val), &(radioResCfg->srb_ToAddModLst.member));
  srbIe = &(radioResCfg->srb_ToAddModLst.member[0]);

  /* Start filling the IE */

  UEH_FILL_TKN_UINT(srbIe->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(srbIe->srb_Identity, srbId);

  UEH_FILL_TKN_UINT(srbIe->rlc_Config.choice, RLC_CONFIG_EXPLICITVALUE);

  UEH_FILL_TKN_UINT(srbIe->rlc_Config.val.explicitValue.choice, RLC_CONFIG_AM);

  /* RLC Configuration for AM Mode */
  rlcCfgAm = &(srbIe->rlc_Config.val.explicitValue.val.am);
  UEH_FILL_TKN_UINT(rlcCfgAm->pres, PRSNT_NODEF);

  /* Uplink AM RLC Config */
  UEH_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.t_PollRetransmit, smCb.cfgCb.uehConfig.srbPollRetrans);
  UEH_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pollPDU, smCb.cfgCb.uehConfig.srbPollPdu);
  UEH_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.pollByte, smCb.cfgCb.uehConfig.srbPollByte );
  UEH_FILL_TKN_UINT(rlcCfgAm->ul_AM_RLC.maxRetxThreshold, smCb.cfgCb.uehConfig.srbMaxReTxThresh);

  /* Downlink AM RLC Config */
  UEH_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.t_Reordering, smCb.cfgCb.uehConfig.srbTreordrng);
  UEH_FILL_TKN_UINT(rlcCfgAm->dl_AM_RLC.t_StatusProhibit, smCb.cfgCb.uehConfig.srbTStatProhibit);

  /* Logical Channel Configuration for SRB */
  UEH_FILL_TKN_UINT(srbIe->logicalChannelConfig.choice, LOGICALCHANNELCONFIG_EXPLICITVALUE);
  logChCfg = &(srbIe->logicalChannelConfig.val.explicitValue);
  UEH_FILL_TKN_UINT(logChCfg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(logChCfg->ul_SpecificParams.pres, PRSNT_NODEF);

  if (srbId EQ UEH_PDCP_ID_SRB1)
  {
    UEH_FILL_TKN_UINT(logChCfg->ul_SpecificParams.priority, smCb.cfgCb.uehConfig.srb1LogchcfgPrior);
  }
  else
  {
    UEH_FILL_TKN_UINT(logChCfg->ul_SpecificParams.priority, smCb.cfgCb.uehConfig.srb2LogchcfgPrior);
  }

  /* Manu:24July12: SRB Configration Changed.
   *  1. Changed prioritisedBitRate (PBR) of SRBs to infinity (hard-coded).
   *  2. Changed bucketSizeDuration (BSD) of SRBs to ms50.
   *
   *  Rationale:
   *  - If there is data on any SRB, it must be scheduled before data channels.
   *  - Hence PBR of SRBs is set to infinity. 
   *  - This is also the default configuration as per 3GPP 36.331
   *  - And when PBR is infinity, BSD does not matter. Hence set it to any valid value (ms50).
   *
   */
  UEH_FILL_TKN_UINT(logChCfg->ul_SpecificParams.prioritisedBitRate, smCb.cfgCb.uehConfig.srbPrioritisedBitRate);
  UEH_FILL_TKN_UINT(logChCfg->ul_SpecificParams.bucketSizeDuration,  NhuLogChannelConfigul_SpecificParamsbucketSizeDurationms50Enum);
  UEH_FILL_TKN_UINT(logChCfg->ul_SpecificParams.logicalChannelGroup, UEH_SRB_LC_GRP);

  RETVALUE(ROK);

} /* uehFillNhuSRBAddModLst */


PUBLIC S16 uehFillNhuDRBAddModLst(
    Ptr                                  evnt,
    NhuRadioResourceConfigDedicated     *rrcCfgDed,
    UehUeCb                             *ueCb
  )
{
  NhuDRB_ToAddMod                       *drbIe         = NULLP;
  NhuLogChannelConfigul_SpecificParams  *chCfgParams   = NULLP;
  U8  rbIdx = 0;
  U16 idx = 0;

  logFunc("%s", "uehFillNhuDRBAddModLst");

  UEH_FILL_TKN_UINT(rrcCfgDed->pres, PRSNT_NODEF);
  /* Allocate memory for each Rab */
  UEH_FILL_TKN_UINT(rrcCfgDed->drb_ToAddModLst.noComp, ueCb->rbInfo.numOfRbCfgd);
  UEH_GET_MEM(evnt, (sizeof(NhuDRB_ToAddMod) * ueCb->rbInfo.numOfRbCfgd), &(rrcCfgDed->drb_ToAddModLst.member));

  for(idx = 0; idx < ueCb->rbInfo.numOfRbCfgd; idx++)
  {
    /* Now Fill the RRC Request from stored values
     * Update DRB to ADD Mod List */
    drbIe = &(rrcCfgDed->drb_ToAddModLst.member[idx]);
    UEH_FILL_TKN_UINT(drbIe->pres, PRSNT_NODEF);
    UEH_FILL_TKN_UINT(drbIe->eps_BrIdentity, ueCb->rbInfo.drb[rbIdx].eRABId);
    UEH_FILL_TKN_UINT(drbIe->drb_Identity, ueCb->rbInfo.drb[rbIdx].rbId);
    /* PDCP-Config, We use RLC and Header Compression is not supported */
    UEH_FILL_TKN_UINT(drbIe->pdcp_Config.pres, PRSNT_NODEF);
    UEH_FILL_TKN_UINT(drbIe->pdcp_Config.discardTimer,uehCb.uehCfgCb.qosCfg.discardTimer); /* uehDfltDiscardTimer */

    /* Manu, 24July12: Check if RLC AM or UM Mode is to be used */
    if (uehCb.uehCfgCb.qosCfg.rlcMode EQ RLCMODE_AM) 
    {
      logInfo("UE=%d | ADD DRB=%d | RLC_AM", ueCb->crnti, ueCb->rbInfo.drb[rbIdx].rbId);
      
      /* PDCP Config */
      UEH_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_AM.pres, PRSNT_NODEF);
      UEH_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_AM.statusReportReqd, uehCb.uehCfgCb.qosCfg.statusReportRequired);
      UEH_FILL_TKN_UINT(drbIe->pdcp_Config.headerCompression.choice, HEADERCOMPRESSION_NOTUSED);

      /* RLC-Config */
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.choice, RLC_CONFIG_AM);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.am.pres, PRSNT_NODEF);
      /* UL-AM-RLC */
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.pres, PRSNT_NODEF);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.t_PollRetransmit, uehCb.uehCfgCb.qosCfg.amRlcTPollRetransmit);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.pollPDU, uehCb.uehCfgCb.qosCfg.amRlcPollPDU);
      /* TODO: amRlcPollByte = UEH_GET_MIN(smCb.cfgCb.uehConfig.ueCatPollByteIdx, uehCb.uehCfgCb.qosCfg.amRlcPollByte); */
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.pollByte, uehCb.uehCfgCb.qosCfg.amRlcPollByte);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.am.ul_AM_RLC.maxRetxThreshold, uehCb.uehCfgCb.qosCfg.amRlcMaxRetxThreshold);
      /* DL-AM-RLC */
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.am.dl_AM_RLC.pres, PRSNT_NODEF);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.am.dl_AM_RLC.t_Reordering, uehCb.uehCfgCb.qosCfg.amRlcTReordering);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.am.dl_AM_RLC.t_StatusProhibit,uehCb.uehCfgCb.qosCfg.amRlcTStatusProhibit);
   } /* RLCMODE_AM */
   else
   {
      logInfo("UE=%d | ADD DRB=%d | RLC_UM", ueCb->crnti, ueCb->rbInfo.drb[rbIdx].rbId);
      UEH_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_UM.pres, PRSNT_NODEF);
      /* TODO: Why is the PDCP SN value hard-coded?
       * Why are UM values taken from uehConfig while AM values are taken from qosCfg?
       */
      UEH_FILL_TKN_UINT(drbIe->pdcp_Config.rlc_UM.pdcp_SN_Size, NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum);
      UEH_FILL_TKN_UINT(drbIe->pdcp_Config.headerCompression.choice, HEADERCOMPRESSION_NOTUSED);

      UEH_FILL_TKN_UINT(drbIe->rlc_Config.choice, RLC_CONFIG_UM_BI_DIRECTIONAL);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.pres, PRSNT_NODEF);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.ul_UM_RLC.pres, PRSNT_NODEF);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.ul_UM_RLC.sn_FieldLength, smCb.cfgCb.uehConfig.ulUmSnFieldLen);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.dl_UM_RLC.pres, PRSNT_NODEF);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.dl_UM_RLC.sn_FieldLength, smCb.cfgCb.uehConfig.dlUmSnFieldLen);
      UEH_FILL_TKN_UINT(drbIe->rlc_Config.val.um_Bi_Directional.dl_UM_RLC.t_Reordering, smCb.cfgCb.uehConfig.umRlcTReordering);   }

    /* logicalChannelIdentity  from 3 .. 10 */
    UEH_FILL_TKN_UINT(drbIe->logicalChannelIdentity, ueCb->rbInfo.drb[rbIdx].rbId);

    /* LogicalChannelConfig */
    UEH_FILL_TKN_UINT(drbIe->logicalChannelConfig.pres, PRSNT_NODEF);
    chCfgParams = &(drbIe->logicalChannelConfig.ul_SpecificParams);
    UEH_FILL_TKN_UINT(chCfgParams->pres, PRSNT_NODEF);
    UEH_FILL_TKN_UINT(chCfgParams->priority, uehCb.uehCfgCb.qosCfg.drbPriority);
    UEH_FILL_TKN_UINT(chCfgParams->prioritisedBitRate, uehCb.uehCfgCb.qosCfg.drbPrioritisedBitRate);
    UEH_FILL_TKN_UINT(chCfgParams->bucketSizeDuration, uehCb.uehCfgCb.qosCfg.drbBucketSizeDuration);
    UEH_FILL_TKN_UINT(chCfgParams->logicalChannelGroup, uehCb.uehCfgCb.qosCfg.logicalChannelGroup);
  }

  RETVALUE(ROK);

} /* uehFillNhuDRBAddModLst */

/**
  *
  *       Fun:   uehFillUeRadioAccCapContPdu
  *
  *       Desc:  Fills UE Capability RAT Container PDU
  *
  *       Ret:   ROK
  *
  *       Notes: None
  *
  *       File:  ueh_bldmsg.c
  *
  **/
PUBLIC S16 uehFillUeRadioAccCapContPdu(
    NhuEncReqSdus         *nhuEncReqSdus,
    NhuUE_CapRAT_ContLst  *ue_RadioAccessCapInfo,
    UehUeCb               *ueCb
  )
{
  UEH_FILL_TKN_UINT(ue_RadioAccessCapInfo->noComp, 1);

  UEH_GET_MEM((Ptr)nhuEncReqSdus, (ue_RadioAccessCapInfo->noComp.val) * (sizeof(NhuUE_CapRAT_Cont)), &(ue_RadioAccessCapInfo->member));
  UEH_FILL_TKN_UINT(ue_RadioAccessCapInfo->member[0].pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(ue_RadioAccessCapInfo->member[0].rat_Typ, 0); /* E-UTRAN */

  ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.pres = PRSNT_NODEF;
  ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.len = ueCb->ueCapRatCntr.infoLen;
  UEH_GET_MEM((Ptr)nhuEncReqSdus, ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.len, &(ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.val));
  UEH_MEM_COPY(ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.val, ueCb->ueCapRatCntr.ueCapRatCntr, ue_RadioAccessCapInfo->member[0].ueCapRAT_Cont.len);
  
  RETVALUE(ROK);
} /* uehFillUeRadioAccCapContPdu */


/*
*
*       Fun:   uehFillNhuMACMainCfg
*
*       Desc:  This function fills NHU Radio Resource Configuration
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
void uehFillNhuMACMainCfg(
  Ptr         evnt,
  NhuRadioResourceConfigDedicated *radioResCfg,
  UehUeCb     *ueCb
)
{
  NhuMAC_MainConfig *macCfg = NULLP;

  logFunc("%s", "uehFillNhuMACMainCfg");

  /* MAC-MainConfig  */
  UEH_FILL_TKN_UINT(radioResCfg->mac_MainConfig.choice, MAC_MAINCONFIG_EXPLICITVALUE);
  macCfg = &(radioResCfg->mac_MainConfig.val.explicitValue);

  UEH_FILL_TKN_UINT(macCfg->pres, PRSNT_NODEF);

  /* ul-SCH-Config */
  UEH_FILL_TKN_UINT(macCfg->ul_SCH_Config.pres, PRSNT_NODEF);

  UEH_FILL_TKN_UINT(macCfg->ul_SCH_Config.maxHARQ_Tx, uehCb.uehCfgCb.schCfg.maxUlHarqTx);//uehDfltMacCfgMaxHARQTx

  UEH_FILL_TKN_UINT(macCfg->ul_SCH_Config.periodicBSR_Timer, uehCb.uehCfgCb.schCfg.periodicBsrTimer); /* uehDfltMacCfgPeriodicBSRTimer */

  UEH_FILL_TKN_UINT(macCfg->ul_SCH_Config.retxBSR_Timer, uehCb.uehCfgCb.schCfg.retxBsrTimer);//Need to be taken from BSM -TODO*/

  UEH_FILL_TKN_UINT(macCfg->ul_SCH_Config.ttiBundling, smCb.cfgCb.uehConfig.macCfgTtiBundling);//uehDfltMacCfgTtiBundling);

  //if(!(ueCb->ueProcedure == UEH_INITIAL_CXT_SETUP))
  //{
  UEH_FILL_TKN_UINT(macCfg->drx_Config.choice, DRX_CONFIG_RELEASE);
  //}
  /* TimeAlignmentTimer */
  UEH_FILL_TKN_UINT(macCfg->timeAlignmentTimerDedicated,uehCb.uehCfgCb.sib2Cfg.timeAlignmentTimer);

    /* phr-Config */
  UEH_FILL_TKN_UINT(macCfg->phr_Config.choice, PHR_CONFIG_RELEASE);
  /*
  UEH_FILL_TKN_UINT(macCfg->phr_Config.choice, PHR_CONFIG_SETUP);
  UEH_FILL_TKN_UINT(macCfg->phr_Config.val.setup.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(macCfg->phr_Config.val.setup.periodicPHR_Timer, uehCb.uehCfgCb.schCfg.periodicPhrTimer);
  UEH_FILL_TKN_UINT(macCfg->phr_Config.val.setup.prohibitPHR_Timer, uehCb.uehCfgCb.schCfg.prohibitPhrTimer);
  UEH_FILL_TKN_UINT(macCfg->phr_Config.val.setup.dl_PathlossChange, uehCb.uehCfgCb.schCfg.dlPathlossChange);
  */

} /* uehFillNhuMACMainCfg */

/*
*
*       Fun:   uehFillRadResPhyCfgDed
*
*       Desc:  This function fills Physical Configuration for UE
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
void uehFillRadResPhyCfgDed(
    Ptr                              evnt,
    NhuRadioResourceConfigDedicated *radioResCfg,
    UehUeCb                         *ueCb
  )
{
  NhuPDSCH_ConfigDedicated              *pdschCfg             = NULLP;
  NhuPUCCH_ConfigDedicated              *pucchCfg             = NULLP;
  NhuPUSCH_ConfigDedicated              *puschCfg             = NULLP;
  NhuUlnkPowerCntrlDedicated            *uplnkPwrCntrl        = NULLP;
  NhuCQI_ReportConfig                   *cqiRprtng            = NULLP;
  NhuSoundingRS_UL_ConfigDedicated      *soundngRsUlCfg       = NULLP;
  NhuSchedulingRqstConfig               *schedulingRqstConfig = NULLP;
  NhuPhysicalConfigDedicatedantennaInfo *antennaInfo          = NULLP;

  logFunc("%s", "uehFillRadResPhyCfgDed");

  UEH_FILL_TKN_UINT(radioResCfg->physicalConfigDedicated.pres, PRSNT_NODEF);
 
  /* Added for not sending the below IEs in case of rrcConnectionReconfiguration */
  if(ueCb->ueProcedure == UEH_RRC_CON_SETUP)
  {
    logInfo("%s", "uehFillRadResPhyCfgDed for RRC Connection Setup");
    /* PDSCH Configuration */
    pdschCfg = &(radioResCfg->physicalConfigDedicated.pdsch_ConfigDedicated);

    UEH_FILL_TKN_UINT(pdschCfg->pres,PRSNT_NODEF);
    /* def PUCCH_RESOURCE_MGMT  */
    UEH_FILL_TKN_UINT(pdschCfg->p_a,uehCb.uehCfgCb.phyCfg.cellPhyPa); /* uehDfltDedPhyCfgPa */
    /* Acc To Software Design of RRC Connection Setup Rev 1.1 */
    /* PUCCH Configuration */
    pucchCfg = &(radioResCfg->physicalConfigDedicated.pucch_ConfigDedicated);
    UEH_FILL_TKN_UINT(pucchCfg->pres, PRSNT_NODEF);
    /* Default Release - 36.508 */
    UEH_FILL_TKN_UINT(pucchCfg->ackNackRepetition.choice, ACKNACKREPETITION_RELEASE);
    /* absent for FDD & TDD use bundling */
#ifdef LTE_TDD
  UEH_FILL_TKN_UINT(pucchCfg->tdd_AckNackFeedbackMode, uehDfltTddAckNackFdbckMode);
#endif
    /* PUSCH ConfigDedicated */
    puschCfg = &(radioResCfg->physicalConfigDedicated.pusch_ConfigDedicated);
    UEH_FILL_TKN_UINT(puschCfg->pres, PRSNT_NODEF);
    UEH_FILL_TKN_UINT(puschCfg->betaOffset_ACK_Idx, uehCb.uehCfgCb.phyCfg.betaOffsetAckIndex);/* uehDfltBetaOffACKIdx*/
    UEH_FILL_TKN_UINT(puschCfg->betaOffset_RI_Idx, uehCb.uehCfgCb.phyCfg.betaOffsetRiIndex);/* uehDfltBetaOffRIIdx */
    UEH_FILL_TKN_UINT(puschCfg->betaOffset_CQI_Idx, uehCb.uehCfgCb.phyCfg.betaOffsetCqiIndex); /* uehDfltBetaOffCQIIdx*/

    /* Uplink Power Control Configuration */
    uplnkPwrCntrl = &(radioResCfg->physicalConfigDedicated.uplinkPowerCntrlDedicated);
    //UEH_FILL_TKN_UINT_NOTPRSNT(uplnkPwrCntrl->pres, 0);
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->pres, PRSNT_NODEF);
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUSCH, uehCb.uehCfgCb.phyCfg.poUePusch);/*uehDfltP0UEPUSCH */
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->deltaMCS_Enabled,smCb.cfgCb.uehConfig.deltaMcsEnabled);//uehDfltDeltaMCSEnabled);
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->accumulationEnabled, smCb.cfgCb.uehConfig.accumulationEnabled);//uehDfltAccumulationEnabled);
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUCCH, uehCb.uehCfgCb.phyCfg.poUePucch);/*uehDfltP0UEPUCCH*/
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->pSRS_Offset, uehCb.uehCfgCb.phyCfg.pSrsOffset);/*uehDfltpSRSOffset*/
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->filterCoefficient, uehCb.uehCfgCb.cellCfg.rsrpFilterCoefficient);/*uehDfltFilterCoef*/

    /* CQI Reporting Related Information */
    cqiRprtng = &(radioResCfg->physicalConfigDedicated.cqi_ReportConfig);

    cqiRprtng->pres.pres = PRSNT_NODEF;
    /* NhuCQI_ReportAPeriodicsetup - empty */
    cqiRprtng->cqi_ReportModeAperiodic.pres = NOTPRSNT;
    /* UEH_FILL_TKN_UINT(cqiRprtng->cqi_ReportModeAperiodic, 3); */
    UEH_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset, 0);
    if (1 == smCb.cfgCb.uehConfig.isDlLaEnabled)
    {
    cqiRprtng->cqi_ReportPeriodic.choice.pres = PRSNT_NODEF;
    UEH_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.choice, CQI_REPORTPERIODIC_SETUP);
    UEH_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset, 0);
    UEH_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.pres, PRSNT_NODEF);
    UEH_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_PUCCH_ResourceIdx, ueCb->acParams.cqipmiriInfo.cqiPucchRscIndex); /* uehDfltCqiPUCCHResIdx */
    UEH_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_pmi_ConfigIdx, ueCb->acParams.cqipmiriInfo.cqipmiConfigIndex);/* uehDfltCqiPmiConfigIdx */
    UEH_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_FormatIndicatorPeriodic.choice,0);
    /*
    UEH_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_FormatIndicatorPeriodic.val.subbandCQI.pres,PRSNT_NODEF);
    UEH_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.cqi_FormatIndicatorPeriodic.val.subbandCQI.k, smCb.cfgCb.uehConfig.cqiCfgk);
*/

    UEH_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.ri_ConfigIdx,ueCb->acParams.cqipmiriInfo.riConfigIndex); /* uehDfltCqiPUCCHResIdx */


    UEH_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.simultaneousAckNackAndCQI, smCb.cfgCb.uehConfig.simultAckNackAndCQI);
    }
    else
    {
      cqiRprtng->cqi_ReportPeriodic.choice.pres = NOTPRSNT;
    }
    /* Fill SRS Config */
    soundngRsUlCfg = &(radioResCfg->physicalConfigDedicated.soundingRS_UL_ConfigDedicated);
    /*      if( 0 == uehGetSrsPeriodicity() )
    {*/
      soundngRsUlCfg->choice.pres = NOTPRSNT;
    /*}
        else
    {
      UEH_FILL_TKN_UINT(soundngRsUlCfg->choice, SOUNDINGRS_UL_CONFIGDEDICATED_SETUP);
      UEH_FILL_TKN_UINT(soundngRsUlCfg->val.setup.pres, PRSNT_NODEF);
      UEH_FILL_TKN_UINT(soundngRsUlCfg->val.setup.srs_Bandwidth, ueCb->ueSrsInfo.ueSrsBandwidth);
      UEH_FILL_TKN_UINT(soundngRsUlCfg->val.setup.srs_HoppingBandwidth, ueCb->ueSrsInfo.ueSrsHoppingBw);
      UEH_FILL_TKN_UINT(soundngRsUlCfg->val.setup.freqDomainPosition, ueCb->ueSrsInfo.ueFreqDomainPos);
      UEH_FILL_TKN_UINT(soundngRsUlCfg->val.setup.duration, ueCb->ueSrsInfo.ueDuration);
      UEH_FILL_TKN_UINT(soundngRsUlCfg->val.setup.srs_ConfigIdx, ueCb->ueSrsInfo.ueSrsCfgIndex);
      UEH_FILL_TKN_UINT(soundngRsUlCfg->val.setup.transmissionComb, ueCb->ueSrsInfo.ueTransmissionComb);
      UEH_FILL_TKN_UINT(soundngRsUlCfg->val.setup.cyclicShift, ueCb->ueSrsInfo.ueDedCyclicShift);
    } */
  } /*if(ueCb->ueProcedure == UEH_RRC_CON_SETUP)*/
  else 
  {
    pdschCfg = &(radioResCfg->physicalConfigDedicated.pdsch_ConfigDedicated);
    UEH_FILL_TKN_UINT_NOTPRSNT (pdschCfg->pres, NOTPRSNT);

    pucchCfg = &(radioResCfg->physicalConfigDedicated.pucch_ConfigDedicated);
    UEH_FILL_TKN_UINT_NOTPRSNT (pucchCfg->pres, NOTPRSNT);

    uplnkPwrCntrl = &(radioResCfg->physicalConfigDedicated.uplinkPowerCntrlDedicated);
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->pres, PRSNT_NODEF);
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUSCH, uehCb.uehCfgCb.phyCfg.poUePusch);/*uehDfltP0UEPUSCH */
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->deltaMCS_Enabled,smCb.cfgCb.uehConfig.deltaMcsEnabled);//uehDfltDeltaMCSEnabled);
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->accumulationEnabled, smCb.cfgCb.uehConfig.accumulationEnabled);//uehDfltAccumulationEnabled);
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUCCH, uehCb.uehCfgCb.phyCfg.poUePucch);/*uehDfltP0UEPUCCH*/
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->pSRS_Offset, uehCb.uehCfgCb.phyCfg.pSrsOffset);/*uehDfltpSRSOffset*/
    UEH_FILL_TKN_UINT(uplnkPwrCntrl->filterCoefficient, uehCb.uehCfgCb.cellCfg.rsrpFilterCoefficient);/*uehDfltFilterCoef*/

     /* CQI Reporting Related Information */
    cqiRprtng = &(radioResCfg->physicalConfigDedicated.cqi_ReportConfig);
    cqiRprtng->pres.pres = NOTPRSNT;
    cqiRprtng->cqi_ReportModeAperiodic.pres = NOTPRSNT;
    if (0 == smCb.cfgCb.uehConfig.isDlLaEnabled)
    {
      cqiRprtng->cqi_ReportPeriodic.choice.pres = NOTPRSNT;
    }
    else
    {
      radioResCfg->physicalConfigDedicated.soundingRS_UL_ConfigDedicated.choice.pres = NOTPRSNT;
    }
  }
  /*  Antenna Configuration */
  antennaInfo = &(radioResCfg->physicalConfigDedicated.antennaInfo);
  UEH_FILL_TKN_UINT(antennaInfo->choice, ANTENNAINFO_EXPLICITVALUE);
  UEH_FILL_TKN_UINT(antennaInfo->val.explicitValue.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(antennaInfo->val.explicitValue.transmissionMode, uehCb.uehCfgCb.cellCfg.antInfoTxMode);
  if (2 == smCb.cfgCb.uehConfig.numOfTxAnt)
  {
    UEH_FILL_TKN_UINT(antennaInfo->val.explicitValue.codebookSubsetRestrn.choice,\
         CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM3);
    UEH_FILL_BIT_STR(antennaInfo->val.explicitValue.codebookSubsetRestrn.val.n2TxAntenna_tm3,\
         2, 3, evnt); /* bit string 11 */
  }
  UEH_FILL_TKN_UINT(antennaInfo->val.explicitValue.ue_TransmitAntennaSelection.choice, UE_TRANSMITANTENNASELECTION_RELEASE);

  /* Scheduling Request Configuration */
  schedulingRqstConfig = &(radioResCfg->physicalConfigDedicated.schedulingRqstConfig);
  UEH_FILL_TKN_UINT(schedulingRqstConfig->choice,SCHEDULINGRQSTCONFIG_SETUP);
  UEH_FILL_TKN_UINT(schedulingRqstConfig->val.setup.pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_PUCCH_ResourceIdx,ueCb->acParams.srInfo.srPucchRscIndex);/* uehDfltSrPUCCHResIdx */

  /* This function is called during Ctf Cfg Req
   * uehGetSRCfgIdx(&schedulingRqstConfig->val.setup.sr_ConfigIdx);
   * and the cfg idx is derived then */
  UEH_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_ConfigIdx,ueCb->acParams.srInfo.srConfigIndex );/* ueCb->srCfgIdx */
  UEH_FILL_TKN_UINT(schedulingRqstConfig->val.setup.dsr_TransMax, smCb.cfgCb.uehConfig.dsrTransMax);//uehDfltDsrTransMax);

} /* End of uehFillRadResPhyCfgDed */


/* mapping OAM config to NHU Config for rlcMode */

U8 oam2nhuRlcMode(RlcMode oam)
{
  if(RLCMODE_UM EQ oam) return NHU_RLC_MODE_UM;
  else if (RLCMODE_AM EQ oam) return NHU_RLC_MODE_AM;
  else {
    logError("Unexpected rlcmode=%d", oam);
    return NHU_RLC_MODE_TM;
  } 
}

/*
*
*       Fun:   uehFillUeAddRbCfg
*
*       Desc:  This function fills UE ADD RB Configuration 
*               in ueCfgInfo structure of euehnt structure
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillUeAddRbCfg (
    NhuRbCfgList  **rbCfgList,
    UehUeCb        *ueCb,
    U8            msgType
  )
{
  NhuRbCfgList *lclRbCfgList = NULLP;
  NhuRbCfgInfo *rbCfg = NULLP;

  U16       idx         = 0;
  U8        rabId       = 0;
  U8        rbtype      = 0;
  U16       logChType   = 0;
  U8        rlcMode     = NHU_RLC_MODE_AM;


  logFunc("%s", "uehFillUeAddRbCfg");

  lclRbCfgList = *rbCfgList;

  /* for all RABs to be cfgd. */
  for(idx = 0; idx < lclRbCfgList->numOfRbCfgInfo; idx++)
  {
    rbCfg = &(lclRbCfgList->rbCfgInfoList[idx]);

    /* Message Type based RB cfgbitmask is set */
    if(C4_SECURITYMODECMD == msgType)
    {
      /* only integrity & ciphering need to be activated */
      rbCfg->rbCfgBitMsk = NHU_RB_CFG_PDCP_INFO; 
    }
    else
    {
      rbCfg->rbCfgBitMsk = (NHU_RB_CFG_UL | NHU_RB_CFG_DL | NHU_RB_CFG_PDCP_INFO); 
    }
    switch(msgType)
    {
      case C3_RRCCONSETUP:
      {
        /* RRC Connection Setup Msg. */ 
        logChType = CM_LTE_LCH_DCCH;
        rabId     = UEH_PDCP_ID_SRB1;
        rbtype    = CM_LTE_SRB;
        break;
      }

      case C4_RRCCONRECFGN:
      {
        /* RRC Reconfiguration Msg. */
        if (idx == 0)
        {
          logChType = CM_LTE_LCH_DCCH;
          rabId     = UEH_PDCP_ID_SRB2;
          rbtype    = CM_LTE_SRB;
        }
        else
        {
          if (idx == 1)
          {
            rabId = UEH_PDCP_ID_DRB1;
          }
          else
          {
            rabId++;
          }
          logChType = CM_LTE_LCH_DTCH;
          rlcMode = oam2nhuRlcMode(uehCb.uehCfgCb.qosCfg.rlcMode);
          rbtype = CM_LTE_DRB;
        }
        break;
      }

      case C4_SECURITYMODECMD:
      {
        /* Security Mode Command */ 
        rabId = UEH_PDCP_ID_SRB1;
        rbtype = CM_LTE_SRB;
        rbCfg->rbCfgBitMsk = NHU_RB_CFG_PDCP_INFO;
        break;
      }

      default:
      {
        break;
      }
    }

    rbCfg->rbId.rbId  = rabId;
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
        rbCfg->lcId.lcId      = rabId;
        rbCfg->lcId.logChType = (CmLteLcType)logChType;

        /* Fill Logical Channel Cfg */
        uehFillUlLogChCfg(&(rbCfg->ulRbCfg.ulLogChMap), rabId);

        /* ulRlcMode */
        rbCfg->ulRbCfg.ulRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
        rbCfg->ulRbCfg.ulRlcMode.mode       = rlcMode;
        uehFillUlRlcModeInfo(&(rbCfg->ulRbCfg.ulRlcMode), rabId);
      }
    } /* if loop - Uplink Logical Channel Mapping Info.  */


    /* DlRbCfg */
    if(rbCfg->rbCfgBitMsk & NHU_RB_CFG_DL)
    {
      rbCfg->dlRbCfg.dlRbBitMsk |= (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

      /* Downlink Logical Channel Mapping Info. */
      {
        /* Logical Channel ID and type */
        rbCfg->lcId.lcId      = rabId;
        rbCfg->lcId.logChType = (CmLteLcType)logChType;
        /* Transport Channel Type */
        rbCfg->dlRbCfg.dlLogChMap.trChType = CM_LTE_TRCH_DL_SCH;
        /* DL-RLC mode */
        rbCfg->dlRbCfg.dlRlcMode.rlcCfgType = NHU_CFG_TYPE_EXPLICIT;
        rbCfg->dlRbCfg.dlRlcMode.mode       = rlcMode;
        uehFillDlRlcModeInfo(&(rbCfg->dlRbCfg.dlRlcMode), rabId, ueCb->ueEutraCap.ueCategory);
      }
    } /* if loop - Downlink Logical Channel Mapping Info. */

    /* PDCP Cfg */
    if(rbCfg->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
    {
       clearBytes(&(rbCfg->pdcpCfg), sizeof(NhuPdcpCfg));
      if (rbtype EQ CM_LTE_SRB)
      {
        /* uehFillPdcpCfg(&(rbCfg->pdcpCfg), rabId, TRUE); */
        rbCfg->pdcpCfg.pdcpCfgType   = NHU_CFG_TYPE_EXPLICIT;
        rbCfg->pdcpCfg.pdcpCfgBitMsk = NHU_PDCP_CFM_REQD;
        rbCfg->pdcpCfg.cfmReqd       = TRUE;
      }
      else
      {
        uehFillPdcpCfg(&(rbCfg->pdcpCfg), rlcMode);
      }
    }
  } /* for loop */
  RETVALUE(ROK);
} /* uehFillUeAddRbCfg */



/*
*
*       Fun:   uehFillNhuUeCfgInfo
*
*       Desc:  This function fills UeCfgInfo in event structure
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillNhuUeCfgInfo
(
Ptr           evnt,
NhuUeCfgInfo  *ueCfg,
UehUeCb        *ueCb,
U8            msgType
)
{
  NhuRbCfgList  *rbCfgList    = NULLP;
  U8            expCfgBitMask = 0;
  U8            noRAB         = 0;
  U8            noSRB         = 0;

  logFunc("%s", "uehFillNhuUeCfgInfo");

  switch(msgType)
  {
    case C3_RRCCONSETUP:
    {
      /* RRC Con Req Msg */
      expCfgBitMask = (NHU_EXP_CFG_RB_CFG | NHU_EXP_CFG_TRCH_CFG | NHU_EXP_CFG_UE_CFG);
      /* number of RABs to be cfgd. */
      noSRB = UEH_VAL_ONE;
      break;
    }

    case C4_RRCCONRECFGN:
    {
      /* RRC Reconfiguration Msg. */
      expCfgBitMask = (NHU_EXP_CFG_RB_CFG | NHU_EXP_CFG_TRCH_CFG);
      /* number of RABs to be cfgd. */
      noSRB = UEH_VAL_ONE;
      noRAB = ueCb->rbInfo.numOfRbCfgd;
      break;
    }

    case C4_SECURITYMODECMD:
    {
      /* Security Mode Command */ 
      expCfgBitMask = NHU_EXP_CFG_SEC_CFG;
      /* number of RABs to be cfgd. */
      noSRB = UEH_VAL_ONE;
      break;
    }

    default:
    { 
      logError("%s","uehFillNhuUeCfgInfo: default case hit");
       break;
    }
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
      rbCfgList->numOfRbCfgInfo = noRAB + noSRB;
      UEH_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * (noRAB + noSRB)),&(rbCfgList->rbCfgInfoList));
      /* fill further cfg info */ 
      uehFillUeAddRbCfg(&rbCfgList, ueCb, msgType);
    }
  }

  /* Transport Channel Configuration */
  if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_TRCH_CFG)
  {
    uehFillTrChCfg(&(ueCfg->trChCfg));
  }

  /* Security Channel Configuration */
  if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_SEC_CFG)
  {
    uehFillSecCfg(&(ueCfg->secCfg), ueCb);
  }

  /* UE Info Configuration */
  if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_UE_CFG)
  {
    ueCfg->ueInfo.newCrnti = ueCb->crnti;
  }
  RETVALUE(ROK);

} /*-- End of uehFillNhuUeCfgInfo --*/

/*
*
*       Fun:   uehFillPdcpCfg
*
*       Desc:  This function fills PDCP Configuration for DRBs.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillPdcpCfg
(
  NhuPdcpCfg    *pdcpCfg,
  U8            nhuRlcMode /* NHU_RLC_MODE_AM ... */
)
{
	logFunc("%s", "uehFillPdcpCfg");

   pdcpCfg->pdcpCfgType = NHU_CFG_TYPE_EXPLICIT;

   if (nhuRlcMode EQ NHU_RLC_MODE_AM)
   { 
    pdcpCfg->pdcpCfgBitMsk  = NHU_PDCP_DISCARD_TMR | NHU_PDCP_STATUS_RPT | NHU_PDCP_CFM_REQD;
   }
   else if (nhuRlcMode EQ NHU_RLC_MODE_UM)
   {
     pdcpCfg->pdcpCfgBitMsk = (NHU_PDCP_SN_SIZE | NHU_PDCP_DISCARD_TMR);
   }
   else {
     logError("unexpected RLC Mode=%d", nhuRlcMode);
     RETVALUE(RFAILED);
   }

   if(pdcpCfg->pdcpCfgBitMsk & NHU_PDCP_DISCARD_TMR)
   {
      pdcpCfg->discardTmr = discTmrForOamVal[uehCb.uehCfgCb.qosCfg.discardTimer];
   }
   if(pdcpCfg->pdcpCfgBitMsk & NHU_PDCP_STATUS_RPT)
   {
     pdcpCfg->statusRptReq = uehCb.uehCfgCb.qosCfg.statusReportRequired;
   }

   if(pdcpCfg->pdcpCfgBitMsk & NHU_PDCP_SN_SIZE)
   {
     pdcpCfg->snSize         = NHU_PDCP_SN_SIZE_7BITS;
   }
   RETVALUE(ROK);
} /* End of uehFillPdcpCfg */

/*
*
*       Fun:   uehFillTrChCfg
*
*       Desc:  This function fills Transport Channel Configuration
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillTrChCfg
(
  NhuTrChCfg    *trChCfg
)
{
   trChCfg->trChCfgType               = NHU_CFG_TYPE_EXPLICIT;
   trChCfg->trChBitMsk                = (NHU_TRCH_CFG_UL_HARQ);
   trChCfg->ulHarqCfg.ulMaxHarqTx     = maxHargTxForOamVal[uehCb.uehCfgCb.schCfg.maxUlHarqTx];
   trChCfg->ulHarqCfg.deltaHarqOffset = 0;
   RETVALUE(ROK);
} /* End of uehFillTrChCfg */

/*
*
*       Fun:   uehFillSecCfg
*
*       Desc:  This function fills Security Configuration
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillSecCfg
(
NhuSecurityCfg     *secCfg,
UehUeCb             *ueCb
)
{

   logFunc("%s", "uehFillSecCfg");

   secCfg->secCfgBitMsk = (NHU_SEC_CIPHER_ALGO | NHU_SEC_INTG_ALGO);
   secCfg->intgCfgInfo.action = NHU_ACTION_ADD;
  

   if (1 == ueCb->secCfg.intgAlgoSelected) 
   secCfg->intgCfgInfo.intgAlgo = NHU_INTG_ALGO_EIA1;
   else if (2 == ueCb->secCfg.intgAlgoSelected)
   secCfg->intgCfgInfo.intgAlgo = NHU_INTG_ALGO_EIA2;
   else
   secCfg->intgCfgInfo.intgAlgo = NHU_INTG_ALGO_EIA0;

   logInfo("intgAlgo:%d",secCfg->intgCfgInfo.intgAlgo);

   if (1 == ueCb->secCfg.ciphAlgoSelected)
   secCfg->cipherCfgInfo.ciphAlgo = NHU_CIPHER_ALGO_EEA1;
   else if (2 == ueCb->secCfg.ciphAlgoSelected)
   secCfg->cipherCfgInfo.ciphAlgo = NHU_CIPHER_ALGO_EEA2;
   else 
   secCfg->cipherCfgInfo.ciphAlgo = NHU_CIPHER_ALGO_EEA0;

   logInfo("ciphAlgo:%d",secCfg->cipherCfgInfo.ciphAlgo);
   copyBytes(ueCb->secCfg.intgKey,NHU_INTG_KEY_LEN,secCfg->intgCfgInfo.intgKey);

   copyBytes(ueCb->secCfg.cpCiphKey, NHU_CIPH_KEY_LEN, secCfg->cipherCfgInfo.cipherCpKey);
   copyBytes(ueCb->secCfg.upCiphKey, NHU_CIPH_KEY_LEN, secCfg->cipherCfgInfo.cipherUpKey);

   RETVALUE(ROK);
} /* End of uehFillSecCfg */

#if 0
static UINT16 getDrbprioritisedBitRateValue(
  IN DrbPrioritisedBitRate  bitRate
  )
{
  switch(bitRate)
  {
    case DRBPRIORITISEDBITRATE_KBPS0:    return 0;
    case DRBPRIORITISEDBITRATE_KBPS8:    return 8;
    case DRBPRIORITISEDBITRATE_KBPS16:   return 16;
    case DRBPRIORITISEDBITRATE_KBPS32:   return 32;
    case DRBPRIORITISEDBITRATE_KBPS64:   return 64;
    case DRBPRIORITISEDBITRATE_KBPS128:  return 128;
    case DRBPRIORITISEDBITRATE_KBPS256:  return 256;
    case DRBPRIORITISEDBITRATE_INFINITY: return 1024;  /* This is the assumed value for infinity. Not defined in spec.
                                                           This value is matched with CCPU code. */
  }
}
#endif
/*
*
*       Fun:   uehFillUlLogChCfg
*
*       Desc:  This function fills Uplink Logical Channel Configuration
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillUlLogChCfg(
    NhuUlLogChMap *ulLogChMap,
    U8            rbId
  )
{
   ulLogChMap->lcCfg.logChCfgpres = PRSNT_NODEF;
   ulLogChMap->lcCfgType = NHU_CFG_TYPE_EXPLICIT;
   switch(rbId)
   {
      
      case UEH_PDCP_ID_SRB1:
      case UEH_PDCP_ID_SRB2:
         ulLogChMap->lcCfg.logChGrp = UEH_SRB_LC_GRP;
         break;

      case UEH_PDCP_ID_DRB1:
         ulLogChMap->lcCfg.logChGrp = uehCb.uehCfgCb.qosCfg.logicalChannelGroup;
         break;

      default:
         logError("Unexpected rbId=%d", rbId);
         ulLogChMap->lcCfg.logChGrp = uehCb.uehCfgCb.qosCfg.logicalChannelGroup;
         RETVALUE(RFAILED);
         break;
    }

   RETVALUE(ROK);
} /* End of uehFillUlLogChCfg */

/*
*
*       Fun:   uehFillUlRlcModeInfo
*
*       Desc:  This function fills Uplink RLC Mode information
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillUlRlcModeInfo(
    NhuUlRlcMode  *ulRlcMode,
    U8            rabId
  )
{
   if(ulRlcMode->mode EQ NHU_RLC_MODE_AM)
   {
      if((rabId EQ UEH_PDCP_ID_SRB1) || (rabId EQ UEH_PDCP_ID_SRB2))
      {
         ulRlcMode->m.amRlc.tStatusProh = uehTStatusProhibit[smCb.cfgCb.uehConfig.srbTStatProhibit];
         ulRlcMode->m.amRlc.tReordring  = uehTReordring[smCb.cfgCb.uehConfig.srbTreordrng];
      }
      else
      {
         //uehGetStatProhForOamVal(uehCb.uehCfgCb.qosCfg.amRlcTStatusProhibit);
         ulRlcMode->m.amRlc.tStatusProh = uehTStatusProhibit[uehCb.uehCfgCb.qosCfg.amRlcTStatusProhibit];
         //ulRlcMode->m.amRlc.tReordring  = uehCb.uehCfgCb.qosCfg.amRlcTReordering;
         ulRlcMode->m.amRlc.tReordring  = uehTReordring[uehCb.uehCfgCb.qosCfg.amRlcTReordering];
      }
   }
   else if (ulRlcMode->mode EQ NHU_RLC_MODE_UM)
   {      
      ulRlcMode->m.umRlc.snFieldLen = uehSnFieldLen[smCb.cfgCb.uehConfig.ulUmSnFieldLen];
      ulRlcMode->m.umRlc.tReordring = uehTReordring[smCb.cfgCb.uehConfig.umRlcTReordering];
   }

   RETVALUE(ROK);
} /* End of uehFillUlRlcModeInfo */

/*
*
*       Fun:   uehFillDlRlcModeInfo
*
*       Desc:  This function fills Downlink RLC Mode information
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillDlRlcModeInfo(
    NhuDlRlcMode  *dlRlcMode,
    U8            rabId,
    U8            ueCategory
  )
{
  if(dlRlcMode->mode EQ NHU_RLC_MODE_AM)
  {
    if((rabId EQ UEH_PDCP_ID_SRB1) || (rabId EQ UEH_PDCP_ID_SRB2))
    {
      /* Using default values specified in 36.331 */
      dlRlcMode->m.amRlc.maxRetxThresh  = maxRetxThreForOamVal[smCb.cfgCb.uehConfig.srbMaxReTxThresh];
      dlRlcMode->m.amRlc.pollByte       = pollByteForOamVal[smCb.cfgCb.uehConfig.srbPollByte];
      dlRlcMode->m.amRlc.pollPdu        = pollPduForOamVal[smCb.cfgCb.uehConfig.srbPollPdu];
      dlRlcMode->m.amRlc.tPollRetx      = pollRetxForOamVal[smCb.cfgCb.uehConfig.srbPollRetrans];
    }
    else /* DRB */
    {
      dlRlcMode->m.amRlc.maxRetxThresh  = maxRetxThreForOamVal[uehCb.uehCfgCb.qosCfg.amRlcMaxRetxThreshold];
      /* TODO: Find the limit as per UE category and take the lower one.
       *       maxRlcPollByteForUeCat  = getMaxRlcPollByteByUeCate(ueCategory);
       *       amRlcPollByte = UEH_GET_MIN(maxRlcPollByteForUeCat, uehCb.uehCfgCb.qosCfg.amRlcPollByte);
       *       In the above comparison, both should be enums - do not mix enum with converted number.
       *       Remove smCb.cfgCb.uehConfig.ueCatPollByteIdx
       */
      dlRlcMode->m.amRlc.pollByte       = pollByteForOamVal[uehCb.uehCfgCb.qosCfg.amRlcPollByte];
      dlRlcMode->m.amRlc.pollPdu        = pollPduForOamVal[uehCb.uehCfgCb.qosCfg.amRlcPollPDU];
      dlRlcMode->m.amRlc.tPollRetx      = pollRetxForOamVal[uehCb.uehCfgCb.qosCfg.amRlcTPollRetransmit];
    }
  }
  else if(dlRlcMode->mode EQ NHU_RLC_MODE_UM)   
  {      
    dlRlcMode->m.umRlc.snFieldLen = uehSnFieldLen[smCb.cfgCb.uehConfig.dlUmSnFieldLen];
  }

  RETVALUE(ROK);
} /* End of uehFillDlRlcModeInfo */


/*
*
*       Fun:   uehFillNhuEvntHdr
*
*       Desc:  This function fills NHU Event Header 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
void uehFillNhuEvntHdr
(
NhuHdr      *nhuHdr,
U16         cellId,
U16         crnti,
U8          msgType
)
{

   logFunc("%s", "uehFillNhuEvntHdr");

   nhuHdr->cellId  = cellId;
   nhuHdr->ueId    = crnti;
   nhuHdr->transId = msgType;
} /* End of uehFillNhuEvntHdr */


/* ======================================= */
/*S1AP S1 SETUP REQUEST MESSAGE BUILDING*/
/* ======================================= */
#ifdef FT_ERAB_SETUP_REL
/*
*
*       Fun:   veUtlSztFillERABSetRsp
*
*       Desc:  Fill E-RAB Setup Response
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillERABSetRsp
(
VeS1ConCb *s1apCon,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillERABSetRsp(s1apCon, pdu)
VeS1ConCb *s1apCon;
S1apPdu      **pdu;
#endif
{
   S1apPdu *szERABRspPdu = NULLP;
   S16  ret;
   U16  numComp;
   SztProtIE_Field_E_RABSetupRespIEs *ie = NULLP;


   logFunc("%s", "veUtlSztFillERABSetRsp");

   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &veCb.mem,
         (Ptr *)&szERABRspPdu);

   if(ret != ROK)
   {
      RETVALUE(RFAILED);
   }


   veSzFillTknU8(&(szERABRspPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   veSzFillTknU8(&(szERABRspPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   veSzFillTknU32(&(szERABRspPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_E_RABSetup);
   veSzFillTknU32(&(szERABRspPdu->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);

   /* Filling 2 mandatory headers */
   veSzFillTknU8(&(szERABRspPdu->pdu.val.successfulOutcome.value.u.\
            sztE_RABSetupResp.pres),PRSNT_NODEF);

   numComp = 3;

   veSzFillTknU16(&(szERABRspPdu->pdu.val.successfulOutcome.value.u.\
            sztE_RABSetupResp.protocolIEs.noComp), numComp);

   if ((cmGetMem(szERABRspPdu,\
               (numComp * sizeof(SztProtIE_Field_E_RABSetupRespIEs)),\
               (Ptr *)&szERABRspPdu->pdu.val.successfulOutcome.value.u.\
               sztE_RABSetupResp.protocolIEs.member)) !=ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABSetupResp.protocolIEs.member[0];

   veSzFillTknU8(&(ie->pres), PRSNT_NODEF);
   veSzFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veSzFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veSzFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);

   /* Filling ENB ID */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABSetupResp.protocolIEs.member[1];

   veSzFillTknU8(&(ie->pres), PRSNT_NODEF);
   veSzFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   veSzFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veSzFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);

  /* Filling ERAB Setup List from E-RAB Setup Request Pdu */
   veUtlSztFillERABRspFromERABSetReqPdu(s1apCon, &szERABRspPdu);

   /* assign pdu */
   *pdu = szERABRspPdu;

   RETVALUE(ROK);
} /* veUtlSztFillERABSetRsp */

/*
*
*       Fun:   veUtlSztFillERABRelRsp
*
*       Desc:  Fill E-RAB Setup Response
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:
*
*/
#ifdef ANSI
PUBLIC S16 veUtlSztFillERABRelRsp
(
VeS1ConCb *s1apCon,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillERABRelRsp(s1apCon, pdu)
VeS1ConCb *s1apCon;
S1apPdu      **pdu;
#endif
{

   S1apPdu *szERABRspPdu = NULLP;
   S16  ret;
   U16  numComp;
   SztProtIE_Field_E_RABRlsRespIEs *ie = NULLP;

   logFunc("%s", "veUtlSztFillERABRelRsp");

   ret = cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &veCb.mem,
         (Ptr *)&szERABRspPdu);

   if(ret != ROK)
   {
      RETVALUE(RFAILED);
   }


   veSzFillTknU8(&(szERABRspPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   veSzFillTknU8(&(szERABRspPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   veSzFillTknU32(&(szERABRspPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_E_RABRls);
   veSzFillTknU32(&(szERABRspPdu->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);

   /* Filling 2 mandatory headers */
   veSzFillTknU8(&(szERABRspPdu->pdu.val.successfulOutcome.value.u.\
            sztE_RABRlsResp.pres),PRSNT_NODEF);
   numComp = 2;
   veSzFillTknU16(&(szERABRspPdu->pdu.val.successfulOutcome.value.u.\
            sztE_RABRlsResp.protocolIEs.noComp), numComp);

   if ((cmGetMem(szERABRspPdu,\
               (numComp * sizeof(SztProtIE_Field_E_RABRlsRespIEs)),\
               (Ptr *)&szERABRspPdu->pdu.val.successfulOutcome.value.u.\
               sztE_RABRlsResp.protocolIEs.member)) !=ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABRlsResp.protocolIEs.member[0];

   veSzFillTknU8(&(ie->pres), PRSNT_NODEF);
   veSzFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   veSzFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veSzFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);

   /* Filling ENB ID */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABRlsResp.protocolIEs.member[1];

   veSzFillTknU8(&(ie->pres), PRSNT_NODEF);
   veSzFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   veSzFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veSzFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);

   /* assign pdu */
   *pdu = szERABRspPdu;
   RETVALUE(ROK);
} /* veUtlSztFillERABRelRsp */



#endif



/*
*
*       Fun:   uehSzFillTknBStr32
*
*       Desc:  Call handler to fill Bit String of length less than 32 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehSzFillTknBStr32
(
TknBStr32  *ptr,     /* Pointer to the structure */
U16         len,     /* length to be filled */
U32         val      /* value to be filled in */
)
{
   logFunc("%s", "uehSzFillTknBStr32");

   if (len > 32)
   {
      UEH_ERROR_ELMNT_OUT_OF_RANGE("Token length", len, 0, 32);
      RETVALUE(RFAILED);
   }

   /* make the element as present */
   ptr->pres = TRUE;
   ptr->len  = (U8) len;

   *(U32*)ptr->val = (val << (32 - len));

   RETVALUE(ROK);
} /* uehSzFillTknBStr32 */

#ifdef FT_ERAB_SETUP_REL
#ifdef ANSI
PUBLIC S16 veUtlSztFillERABRspFromERABSetReqPdu
(
VeS1ConCb *s1apCon,
S1apPdu      **pdu
)
#else
PUBLIC S16 veUtlSztFillERABRspFromERABSetReqPdu(s1apCon, pdu)
VeS1ConCb *s1apCon;
S1apPdu      **pdu;
#endif
{
   S1apPdu *szERABRspPdu = NULLP;
   S16  ret = ROK;
   SztProtIE_Field_E_RABSetupRespIEs *ie = NULLP;
   SztProtIE_SingleCont_E_RABSetupItemBrSUResIEs *rspMember = NULLP;

   SztE_RABSetupRqst                          *eRABSetupReq = NULLP;
   SztProtIE_Field_E_RABSetupRqstIEs   *member = NULLP;
   SztProtIE_SingleCont_E_RABToBeSetupItemBrSUReqIEs *protIe = NULLP;
   SztE_RABToBeSetupItemBrSUReq *itemIe = NULLP;
   S1apPdu *szERABReqPdu = NULLP;


   U8   gTPId[4];
   U8   len = 0;
   U8   cnt = 0;
   U16 numOfComps = 0;
   U16 memberIdx = 0;
   U16 noERABIds = 0;
   U16 idx = 0;
   U16 rbIdx = 0;
   U32 protId = 0;
   U8 offSet;

   szERABRspPdu = *pdu;
   szERABReqPdu = s1apCon->ueCb->pdu;

   /* retrieve info from S1AP Pdu */
   eRABSetupReq = (SztE_RABSetupRqst *) &(szERABReqPdu->pdu.val.initiatingMsg.\
         value.u.sztE_RABSetupRqst);

   numOfComps = eRABSetupReq->protocolIEs.noComp.val;

   /* ERAB Setup List  */
   ie = &szERABRspPdu->pdu.val.successfulOutcome.value.u.\
        sztE_RABSetupResp.protocolIEs.member[2];

   veSzFillTknU8(&(ie->pres), PRSNT_NODEF);
   veSzFillTknU32(&(ie->id), Sztid_E_RABSetupLstBrSURes);
   veSzFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   veSzFillTknU16(&(ie->value.u.sztE_RABSetupLstBrSURes.noComp), 1);

   if ((cmGetMem(szERABRspPdu,\
               (1 * sizeof(SztProtIE_SingleCont_E_RABSetupItemBrSUResIEs)),\
               (Ptr *)&ie->value.u.sztE_RABSetupLstBrSURes.member)) !=ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Locate ERAB to be setup List IE  */
   for(memberIdx = 0; memberIdx < numOfComps; memberIdx++)
   {
      member = &(eRABSetupReq->protocolIEs.member[memberIdx]);
      protId = member->id.val;
      switch(protId)
      {
         case Sztid_E_RABToBeSetupLstBrSUReq:
            {
               noERABIds = member->value.u.sztE_RABToBeSetupLstBrSUReq.noComp.val;

               for(idx = 0; idx < noERABIds; idx++)
               {
                  rspMember = &(ie->value.u.sztE_RABSetupLstBrSURes.member[idx]);
                  protIe = &(member->value.u.sztE_RABToBeSetupLstBrSUReq.member[idx]);
                  itemIe = &(protIe->value.u.sztE_RABToBeSetupItemBrSUReq);

                  veSzFillTknU8(&(rspMember->pres), PRSNT_NODEF);
                  veSzFillTknU32(&(rspMember->id), Sztid_E_RABSetupItemBrSURes);
                  veSzFillTknU32(&(rspMember->criticality), SztCriticalityrejectEnum);

                  veSzFillTknU8(&(rspMember->value.u.sztE_RABSetupItemBrSURes.pres), PRSNT_NODEF);
                  /* E-RAB ID */
                  cmMemcpy((U8*)&(rspMember->value.u.sztE_RABSetupItemBrSURes.e_RAB_ID), \
                       (U8*)&(itemIe->e_RAB_ID), sizeof(SztE_RAB_ID));
                  /* Transport Layer Address Should be the eNodeB */
                  /* Build bit string from a U32 */
                  len = 4; /* 4bytes */
                  rspMember->value.u.sztE_RABSetupItemBrSURes.transportLyrAddr.pres
                                      = PRSNT_NODEF;
                  rspMember->value.u.sztE_RABSetupItemBrSURes.transportLyrAddr.len
                                          = (len * 8);

                  VE_GET_MEM(szERABRspPdu, len,
                        &rspMember->value.u.sztE_RABSetupItemBrSURes.transportLyrAddr.val);

                  for (cnt = 0; cnt < len; cnt++)
                  {
                     /* compute the offset to right shift the val in U32
                      * shift by 24, 16 & 8 bytes */
                     offSet = (len - (cnt + 1)) * 8;
                     rspMember->value.u.sztE_RABSetupItemBrSURes.transportLyrAddr.val[cnt] =
                        (U8)((veCb.datAppAddr.u.ipv4TptAddr.address >> offSet) & 0xff);
                  }

                  /*cmMemcpy(&(rspMember->value.u.sztE_RABSetupItemBrSURes.transportLyrAddr), \
                        &(itemIe->transportLyrAddr), sizeof(SztTportLyrAddr));*/
                  /* Local  GTP ID */
                  for (rbIdx = 0; rbIdx < VE_MAX_RABS_IN_UE; rbIdx++)
                  {
                     if(itemIe->e_RAB_ID.val ==  s1apCon->ueCb->rbInfo.rab[rbIdx].eRABId)
                     {
                        len = 4;
                        gTPId[0] = (U8)(s1apCon->ueCb->rbInfo.rab[rbIdx].locEgtpTunId >> 24) & 0xff;
                        gTPId[1] = (U8)(s1apCon->ueCb->rbInfo.rab[rbIdx].locEgtpTunId >> 16) & 0xff;
                        gTPId[2] = (U8)(s1apCon->ueCb->rbInfo.rab[rbIdx].locEgtpTunId >> 8) & 0xff;
                        gTPId[3] = (U8)(s1apCon->ueCb->rbInfo.rab[rbIdx].locEgtpTunId & 0xff);
                        ret = veSzFillTknStrOSXL(&(rspMember->value.u.sztE_RABSetupItemBrSURes.gTP_TEID),
                              len, gTPId, &szERABRspPdu);
                        break;
                     }
                  }
                  rspMember->value.u.sztE_RABSetupItemBrSURes.iE_Extns.noComp.pres = NOTPRSNT;

               }/* End of For Loop */
            }
            break;
         default:
            break;
      }
   }
   RETVALUE(ret);
}
#endif /* FT_ERAB_SETUP_REL */


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
#endif

/*
*
*       Fun:   uehFillNhuUeCapEnquiry
*
*       Desc:  Fills UE Capability Enquiry message (DL DCCH)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ueh_bldmsg.c
*
*/
PUBLIC S16 uehFillNhuUeCapEnquiry(
    UehUeCb        *ueCb,
    NhuDatReqSdus  *nhDatReqEvntSdu
  )
{
  NhuDL_DCCH_Msg  *dcchMsg = NULLP;
  NhuUECapEnquiry *ueCapEnq = NULLP;
  NhuUE_CapRqst   *ueCapRqst = NULLP;

  logFunc("%s", "uehFillNhuUeCapEnquiry");

  nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = UEH_PDCP_ID_SRB1;
  nhDatReqEvntSdu->sdu.ueMsg.isMsgDed = TRUE;

  dcchMsg = &(nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);

  UEH_FILL_TKN_UINT(dcchMsg->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(dcchMsg->message.choice, DL_DCCH_MSGTYP_C4);
  UEH_FILL_TKN_UINT(dcchMsg->message.val.c1.choice, C4_UECAPENQUIRY);

  ueCapEnq = &(dcchMsg->message.val.c1.val.ueCapEnquiry);
  UEH_FILL_TKN_UINT(ueCapEnq->pres, PRSNT_NODEF);
  UEH_FILL_TKN_UINT(ueCapEnq->rrc_TrnsxnId, UEH_VAL_ONE);
  UEH_FILL_TKN_UINT(ueCapEnq->criticalExtns.choice, CRITICALEXTNS_C19);
  UEH_FILL_TKN_UINT(ueCapEnq->criticalExtns.val.c1.choice, C19_UECAPENQUIRY_R8);
  UEH_FILL_TKN_UINT(ueCapEnq->criticalExtns.val.c1.val.ueCapEnquiry_r8.pres, PRSNT_NODEF);

  ueCapRqst = &(ueCapEnq->criticalExtns.val.c1.val.ueCapEnquiry_r8.ue_CapRqst);
  UEH_FILL_TKN_UINT(ueCapRqst->noComp, 1);

  UEH_GET_MEM(nhDatReqEvntSdu, (ueCapRqst->noComp.val) * (sizeof(NhuRAT_Typ)), &(ueCapRqst->member));
  UEH_FILL_TKN_UINT(ueCapRqst->member[0], 0); /* EUTRAN */

  RETVALUE(ROK);
} /* End of uehFillNhuUeCapEnquiry */

